# Nix flake for development and builds — design

Date: 2026-10-31
Status: draft, awaiting approval

## Intent

Getting a working local build of Photobox is a chore. CPM fetches most
dependencies, but Qt 6.12, libgphoto2 and gRPC (pulled in by `quite`) are
expected on the machine and nothing provides them. On top of that every
dependency is pinned twice, in `CMakeLists.txt` and in `Flatpak/*.yaml`, and
the two have drifted.

The author wants one way to get a working environment and build, with versions
pinned in one place. A future goal is a NixOS image for a Raspberry Pi running
Photobox as a kiosk, so the author chose Nix as that one way: it serves the dev
environment now and the device image later. Flatpak is dropped.

This document covers the first project: a Nix flake that provides the
Photobox package and the dev shell, CMake switched to plain `find_package`,
CI moved to Nix, and Flatpak removed. The Pi image is a separate, later
project.

## Decisions made by the author

- Nix is the only supported way to build Photobox. No non-Nix fallback.
- CMake uses plain `find_package` (or `pkg_check_modules` where a library has
  no CMake config) for every dependency. CPM is removed entirely.
- Flatpak is removed.
- `quite` is out of scope for now. The author will add a Nix package to the
  quite repo later and update its dependencies there.
- Development happens on NixOS-WSL (the author will switch from Ubuntu WSL).
- A plain flake, no flake-parts or devenv.

## Goals

- On NixOS-WSL with the prerequisites below: clone, `direnv allow`, then
  `cmake --workflow --preset developer` configures, builds and passes its tests.
- PhotoboxApp launched from the dev build shows its window on the Windows
  desktop via WSLg.
- The existing VS Code setup (`.vscode/settings.json`: clangd, CMake Tools with
  presets, gdb with Qt pretty printers, Qt/QML extensions) works when VS Code
  is connected to NixOS-WSL, with the dev shell environment loaded.
- `nix build` builds Photobox and runs its tests, fully offline inside the Nix
  sandbox.
- Dependency versions are pinned in exactly one place: `flake.lock` (nixpkgs
  revision) plus the one package (stdexec) defined in the repo's `Nix/` folder.
- CI validates PRs with Nix, and `flake.lock` updates arrive as PRs.

## Non-goals

- The Raspberry Pi / NixOS image (later project). The flake defines outputs for
  `aarch64-linux` as well as `x86_64-linux` so that project can build on it,
  but aarch64 is not built in CI yet.
- Packaging `quite`, gRPC or asio-grpc.
- Building Photobox without Nix.
- Managing the author's NixOS-WSL system configuration from this repo.
- Publishing binaries or release artifacts (the Flatpak bundles go away and
  nothing replaces them yet).
- Re-enabling the top-level `Tests/` directory (still commented out in
  `CMakeLists.txt`, and its `CMakeLists.txt` references missing files).
- CollageEditorApp. It has not been built since #79 removed it from
  `Photobox/CMakeLists.txt`, and its code predates the `Pbox/...` refactor. The
  author will refactor and bring it back after this project. It gets no
  `nix run` entry and stays out of the build.

## Anti-goals

- Setup steps beyond the documented NixOS-WSL settings and `direnv allow`.
- Machine-specific paths (e.g. `/nix/store/...`) committed in repo settings.
- `nix build` needing network access or impure flags.
- A dependency version pinned in more than one place.

## Approaches considered

For the overall direction:

1. **Devcontainer** (Ubuntu image + aqtinstall Qt, CPM for the rest). Easiest
   start with Docker already installed, but versions stay in two places and it
   does nothing for a future Pi image. Dropped once the Pi image came up.
2. **Develop inside the Flatpak SDK.** Single source of truth, but awkward
   editor integration, and Qt 6.12 is not published as a KDE Flatpak runtime
   yet (Flathub serves 6.10 and 6.11; the CI image tags stop at `kde-6.11`).
3. **Conan.** Qt and gRPC would mostly build from source for this compiler and
   C++ standard, libgphoto2 and quite need custom recipes, and it helps neither
   Flatpak nor a device image.
4. **Nix** (chosen). One flake gives the dev shell, the package and later the
   NixOS Pi image. Trade-off: every dependency must come from Nix, and versions
   follow nixpkgs rather than hand-picked pins.

For the flake structure:

1. **Plain flake** (chosen): `flake.nix` with nixpkgs as the only input, an
   overlay for the missing packages, package + dev shell outputs.
2. **flake-parts**: modular structure; mostly boilerplate at this size, easy
   to adopt later when the Pi image adds outputs.
3. **devenv**: nicer shell features, but the package still has to be plain Nix,
   so it means two layers and another tool.

## Design

### nixpkgs channel

Follow `nixos-unstable`, the only channel with Qt 6.12 (6.12.0 as of
2026-10-31; `nixos-26.05` has 6.11.2). `flake.lock` pins the exact revision.
Moving to `nixos-26.11` once it is released is a one-line change and the better
base for the Pi image. nixos-unstable's default GCC is 16, so C++26 is fine.

### Dependencies

From nixpkgs as-is (versions on nixos-unstable, 2026-10-31):

| Dependency | nixpkgs | Notes |
|---|---|---|
| Qt 6 | 6.12.0 | Core, Gui, Qml, Quick, QuickControls2, Network, Multimedia, PrintSupport, Test; plus qtwayland for running under WSLg |
| Boost | `boost` (1.91) | nixpkgs builds Boost with b2, so header-only libs may not export targets like `Boost::asio`; `Photobox/GPhoto2` already guards that with `TARGET_EXISTS` |
| fmt | `fmt_12` | |
| nlohmann_json | 3.12.0 | |
| cpptrace | 1.0.4 | nixpkgs builds it with libunwind, libdwarf and zstd |
| lunasvg | 3.5.0 | uses nixpkgs' plutovg |
| quill | `quill-log` (13.1.0) | odygrd/quill, maintained in nixpkgs by quill's author. (nixpkgs' `quill` is an unrelated project.) Quill makes breaking majors often; those arrive through the validated lock-update PR. (Claude's call) |
| Catch2 | `catch2_3` (3.16.0) | |
| libgphoto2 | 2.5.34 | via pkg-config, as today |
| libusb | `libusb1` | dependency of libgphoto2 |

Defined in the repo under `Nix/` (an overlay applied in the flake):

- **stdexec** at the commit the author's working tree pins
  (`362cdc77738e958f1d12fddfb55229a98df29d8a`), built with
  `STDEXEC_ENABLE_ASIO=ON` and `STDEXEC_ASIO_IMPLEMENTATION=boost` against
  nixpkgs' Boost, tests and examples off. Its CMake fetches rapids-cmake and
  CPM at configure time; those must be provided as fixed-output sources.
  `Flatpak/stdexec.yaml` is the reference for what it needs (rapids-cmake, CPM,
  `execution.bs`), though the newer commit may need newer versions of them.
  At this commit the asio integration (`exec/asio/*.hpp`) lives in a separate
  `STDEXEC::asioexec` target that links `Boost::asio`, which nixpkgs' b2-built
  Boost does not export. The package patches or maps that to `Boost::headers`,
  and Photobox targets that include `exec/asio/...` (`Photobox/Core`,
  `Photobox/GPhoto2`) link `STDEXEC::asioexec`.

stdexec is the only package defined in the repo.

### CMake changes

- Remove `include(CPM)`, `CMake/CPM.cmake` and every `cpmaddpackage` call.
- Replace them with `find_package(... CONFIG REQUIRED)` for Boost, fmt,
  nlohmann_json, cpptrace, stdexec, quill and lunasvg, and Catch2 when
  `BUILD_TESTING`. Keep the target names the subdirectories already link
  (`fmt::fmt`, `quill::quill`, `STDEXEC::stdexec`, `cpptrace::cpptrace`, …).
- Catch2: `include(Catch)` must find `Catch.cmake` from the installed package
  (the current two `CMAKE_MODULE_PATH` lines for local vs Flatpak builds
  collapse into one).
- quite: `find_package(quite CONFIG QUIET)`. The collage renderer test in
  `Photobox/CollageSession/Tests` (the only user of quite, via `quite::core`)
  is only added when quite is found, with a status message when it is skipped.
  The Settings tests and the rest of CollageSession are unaffected.
- Qt and libgphoto2 lookups stay as they are.
- Keep the author's uncommitted changes that still matter: `CMAKE_CXX_STANDARD
  26` and the Qt `6.12` minimum. The other version bumps in the working tree
  become irrelevant (CPM pins and Flatpak files are removed).
- `CMakePresets.json`: remove `CPM_SOURCE_CACHE`. The `developer` preset stays
  the dev workflow.
- Remove `launcher.sh` and its `install(PROGRAMS …)` rule: it exists only for
  Flatpak (`/app/bin` paths, Flatpak `command:`). The package exposes
  PhotoboxApp directly.

### C++ source changes

The author's version bumps (stdexec `362cdc7`, quill 11 → 13, C++26, Qt 6.12)
have never been compiled, and nixos-unstable adds GCC 16 and nixpkgs' versions
of the other libraries. Fixing Photobox's C++ code so it builds and its tests
pass with these versions is in scope (author's decision). Keep those fixes to
what the new versions require; no unrelated refactoring.

### Flake outputs

- `packages.<system>.default`: Photobox, built with CMake + Ninja,
  `wrapQtAppsHook` so installed apps find their Qt plugins and QML modules.
  `doCheck = true`; tests run with `QT_QPA_PLATFORM=offscreen`.
- `apps.<system>.default`: `nix run` starts PhotoboxApp.
- `devShells.<system>.default`: everything from the package's inputs, plus
  clang-tools (clangd, clang-tidy, clang-format), gdb, ccache, pre-commit,
  gersemi and reuse. The shell sets the Qt plugin and QML import paths so
  binaries built in `build/` run without wrapping.
- pre-commit: the `clang-format` and `gersemi` hooks switch to
  `language: system` and use the dev shell's binaries, so the commit hook and
  clangd's format-on-save run the same versions, pinned by `flake.lock`. The
  other hooks keep pre-commit's own pins (their wheels run on NixOS via
  nix-ld). (Claude's call)
- `formatter` is optional, builder's call.
- Systems: `x86_64-linux` and `aarch64-linux`.

### Dev environment

- `.envrc` with `use flake`; VS Code gets the environment through the direnv
  extension (`mkhl.direnv`, added to `.vscode/extensions.json`).
- `.vscode/settings.json` keeps working unchanged where possible. If the Qt
  extensions need to be told where Qt is, that must not be a hardcoded store
  path in committed settings.
- Prerequisites on the author's NixOS-WSL config, documented, not managed
  here: flakes enabled, `programs.nix-ld.enable = true` (VS Code server),
  `wsl.useWindowsDriver = true` and `hardware.graphics.enable = true` (Qt Quick
  via WSLg), direnv with nix-direnv.
- `.gitignore`: add `result`, `result-*`, `.direnv`; drop the Flatpak entries.

### CI

- `PrValidation.yml` (push to main + PRs): install Nix and run
  `nix flake check`, which builds the package (including tests) and the dev
  shell, on x86_64. Cache Nix store paths in the GitHub Actions cache so stdexec
  and Photobox's build inputs aren't rebuilt every run. No extra accounts or
  repo secrets. (Claude's call)
- Replace `FlatpakUpdates.yml` with a scheduled workflow (weekly) that updates
  `flake.lock`, runs `nix flake check` itself, and opens the PR with the
  result. It has to validate itself because PRs opened with the default
  `GITHUB_TOKEN` don't trigger `PrValidation.yml`. (Claude's call)
- Delete `FlatpakBuild.yml`. `Documentation.yml` is unchanged.

### Flatpak removal

Delete `Flatpak/`, `de.matnel.photobox.yaml`, `de.matnel.photobox.Devel.yaml`,
`photobox.flatpakref`, `launcher.sh`, the two Flatpak workflows, and their
entries in `REUSE.toml` and `.gitignore`. Add `REUSE.toml` annotations for new
files that can't carry an SPDX header (`flake.lock`, `.envrc` if needed).

### Documentation

A "Development setup" page in `Documentation/` (mkdocs): NixOS-WSL
prerequisites, clone + `direnv allow`, the developer workflow preset, running
the apps from the build tree, `nix build` / `nix run`, updating `flake.lock`.

## Starting point and where the work happens

The author had uncommitted changes on `main` (C++26, Qt 6.12, version bumps,
CPM update). These are committed to a feature branch together with this
design and the implementation plan. The implementation itself happens in a new
session on the author's NixOS-WSL, on a fresh clone of that branch. That
machine is where everything is built and verified, including the WSLg window
and VS Code connection goals.

## Left to the builder

- Exact Nix code, overlay layout and file names inside `Nix/`.
- Versions of rapids-cmake / CPM / other sources stdexec needs offline.
- Whether `find_package` calls carry minimum versions.
- Whether the Nix package build uses the CMake presets or its own flags.
- How the dev shell sets the Qt environment for running from `build/`.
- Which Nix install action and binary cache CI uses, and the lock-update action.
- If an existing test fails for reasons unrelated to the environment, raise it
  with the author instead of disabling it.
