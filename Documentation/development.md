# Development setup

Photobox is built and developed through the Nix flake in the repository root.
The flake pins every dependency in `flake.lock` and provides the package and a
dev shell with the compiler, Qt, the libraries and the editor tools.

## NixOS-WSL prerequisites

Add these settings to your NixOS-WSL configuration and rebuild:

```nix
{
  nix.settings.experimental-features = [ "nix-command" "flakes" ];

  # VS Code server binaries
  programs.nix-ld.enable = true;

  # Qt Quick rendering through WSLg
  wsl.useWindowsDriver = true;
  hardware.graphics.enable = true;

  programs.direnv.enable = true;
  programs.direnv.nix-direnv.enable = true;
}
```

## Clone and load the environment

```bash
git clone https://github.com/mathisloge/photobox.git
cd photobox
direnv allow
pre-commit install
```

The first `direnv allow` builds the dev shell, which takes a while. After that
the environment loads whenever you enter the directory, and reloads when
`flake.nix`, `flake.lock` or `Nix/*.nix` change.

`pre-commit install` sets up the formatting and commit message hooks. Commit
with the dev shell loaded, because the clang-format hook uses the shell's
clang-format.

## VS Code

1. Connect VS Code to the NixOS-WSL distro.
2. Open the repository and install the recommended extensions. The direnv
   extension (`mkhl.direnv`) loads the dev shell into VS Code, so CMake Tools,
   clangd and the Qt extensions use the tools from the flake.

The direnv extension restarts the other extensions once the environment has
loaded. If CMake Tools still configured the project without it, run
**Developer: Reload Window**, or **CMake: Delete Cache and Reconfigure** if the
configure used a different compiler.

## Building

```bash
cmake --workflow --preset developer
```

This configures, builds and runs the tests in `build/developer`. Start the
app from the build tree:

```bash
build/developer/runtime/PhotoboxApp --config Photobox/Settings/Tests/Assets/ProjectSettings.json
```

Without `--config` the app reads `./photobox.json` and exits if it is missing.
`Photobox/Settings/Tests/Assets/ProjectSettings.json` is a working example.
Add `--dev` to use a mock camera instead of a real one.

A `build/` directory created outside the dev shell points at other compilers
and libraries. Delete it once, then configure again from the dev shell.

## Packaging

```bash
nix build              # builds Photobox and runs its tests, result in ./result
nix run . -- --config path/to/photobox.json
```

To update the dependencies, update the lock file and check that everything
still builds and passes its tests:

```bash
nix flake update
nix flake check
```
