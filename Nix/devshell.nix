# SPDX-FileCopyrightText: 2026 Mathis Logemann <mathis.opensource@tuta.io>
#
# SPDX-License-Identifier: GPL-3.0-or-later
{
  lib,
  mkShell,
  qt6,
  llvmPackages_22,
  gdb,
  ccache,
  pre-commit,
  reuse,
  python3,
  photobox,
}:
let
  # Lets binaries built in build/ find Qt plugins and QML modules without
  # wrapQtAppsHook.
  qtModules = [
    qt6.qtbase
    qt6.qtdeclarative
    qt6.qtmultimedia
    qt6.qtsvg
    qt6.qtwayland
  ];
in
mkShell {
  inputsFrom = [ photobox ];

  packages = [
    # LLVM 22 matches the repo's previous clang-format pin.
    llvmPackages_22.clang-tools
    gdb
    ccache
    pre-commit
    reuse
    python3
  ];

  QT_PLUGIN_PATH = lib.makeSearchPath qt6.qtbase.qtPluginPrefix qtModules;
  QML_IMPORT_PATH = lib.makeSearchPath qt6.qtbase.qtQmlPrefix qtModules;

  # pip treats packages on PYTHONPATH as installed, so pre-commit's hook
  # environments would miss dependencies. The tools above are wrapped and
  # don't need it. Add Python libraries as python3.withPackages, not as bare
  # python3Packages.
  shellHook = ''
    unset PYTHONPATH
  '';
}
