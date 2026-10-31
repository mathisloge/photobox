# SPDX-FileCopyrightText: 2026 Mathis Logemann <mathis.opensource@tuta.io>
#
# SPDX-License-Identifier: GPL-3.0-or-later
{
  lib,
  stdenv,
  cmake,
  ninja,
  pkg-config,
  qt6,
  boost,
  fmt_12,
  nlohmann_json,
  cpptrace,
  stdexec,
  quill-log,
  lunasvg,
  plutovg,
  libgphoto2,
  libusb1,
  catch2_3,
}:
stdenv.mkDerivation (finalAttrs: {
  pname = "photobox";
  version = "0.0.1";

  # Only what the build reads, so docs and CI edits don't trigger rebuilds.
  src = lib.fileset.toSource {
    root = ../.;
    fileset = lib.fileset.unions [
      ../CMakeLists.txt
      ../CMake
      ../Photobox
    ];
  };

  nativeBuildInputs = [
    cmake
    ninja
    pkg-config
    qt6.wrapQtAppsHook
  ];

  buildInputs = [
    qt6.qtbase
    qt6.qtdeclarative
    qt6.qtmultimedia
    qt6.qtsvg
    qt6.qtwayland
    boost
    fmt_12
    nlohmann_json
    cpptrace
    stdexec
    quill-log
    lunasvg
    plutovg
    libgphoto2
    libusb1
  ];

  checkInputs = [ catch2_3 ];

  # The package build sets its own flags instead of using CMakePresets.json.
  cmakeFlags = [ (lib.cmakeBool "BUILD_TESTING" finalAttrs.doCheck) ];

  doCheck = true;

  preCheck = ''
    export QT_QPA_PLATFORM=offscreen
    export HOME=$TMPDIR
  '';

  meta = {
    description = "Photobox application";
    homepage = "https://github.com/mathisloge/photobox";
    license = lib.licenses.gpl3Plus;
    platforms = lib.platforms.linux;
    mainProgram = "PhotoboxApp";
  };
})
