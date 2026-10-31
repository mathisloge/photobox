# SPDX-FileCopyrightText: 2026 Mathis Logemann <mathis.opensource@tuta.io>
#
# SPDX-License-Identifier: GPL-3.0-or-later
{
  lib,
  stdenv,
  fetchFromGitHub,
  fetchurl,
  linkFarm,
  cmake,
  git,
  ninja,
  boost,
}:
let
  # stdexec's CMake fetches rapids-cmake and CPM at configure time. Both are
  # provided here so the build works offline in the sandbox.
  # rapids-cmake: the `rapids-cmake-version` default in cmake/RAPIDS.cmake.
  rapids-cmake = fetchFromGitHub {
    owner = "rapidsai";
    repo = "rapids-cmake";
    tag = "v24.02.00";
    hash = "sha256-abl8pT81osBMcxJM/wtB8oBu1DK/lz5k/O1bzqCLPvY=";
  };
  # CPM: the version rapids-cmake's cpm/detail/download.cmake expects. That
  # file ignores CPM_DOWNLOAD_LOCATION and looks in CPM_SOURCE_CACHE instead.
  cpmVersion = "0.38.5";
  cpm-cache = linkFarm "cpm-source-cache" [
    {
      name = "cpm/CPM_${cpmVersion}.cmake";
      path = fetchurl {
        url = "https://github.com/cpm-cmake/CPM.cmake/releases/download/v${cpmVersion}/CPM.cmake";
        hash = "sha256-GSqgzNxX3+db2eSxdr9/tWkv0rPj97CcdIVvw5Vysxw=";
      };
    }
  ];
in
stdenv.mkDerivation {
  pname = "stdexec";
  version = "0.11.0-unstable-2026-10-31";

  src = fetchFromGitHub {
    owner = "NVIDIA";
    repo = "stdexec";
    rev = "362cdc77738e958f1d12fddfb55229a98df29d8a";
    hash = "sha256-e/BoIM+GoVGMugrgMa+Krt3p6bGUHY74YHTu7Qw4DY8=";
  };

  # nixpkgs' Boost is built with b2 and exports no Boost::asio target.
  postPatch = ''
    substituteInPlace cmake/Modules/ConfigureASIO.cmake \
      --replace-fail "Boost::asio" "Boost::headers"
  '';

  nativeBuildInputs = [
    cmake
    ninja
    git
  ];

  propagatedBuildInputs = [ boost ];

  cmakeFlags = [
    (lib.cmakeBool "STDEXEC_BUILD_TESTS" false)
    (lib.cmakeBool "STDEXEC_BUILD_EXAMPLES" false)
    (lib.cmakeBool "STDEXEC_ENABLE_ASIO" true)
    (lib.cmakeFeature "STDEXEC_ASIO_IMPLEMENTATION" "boost")
    (lib.cmakeFeature "FETCHCONTENT_SOURCE_DIR_RAPIDS-CMAKE" "${rapids-cmake}")
    (lib.cmakeFeature "CPM_SOURCE_CACHE" "${cpm-cache}")
    # Unused today (rapids_cpm_find already tries find_package first, and the
    # rapids-cmake source dir skips the download). Kept so a future fetch
    # fails fast or prefers nixpkgs instead of downloading.
    (lib.cmakeBool "FETCHCONTENT_FULLY_DISCONNECTED" true)
    (lib.cmakeBool "CPM_USE_LOCAL_PACKAGES" true)
  ];

  meta = {
    description = "NVIDIA's reference implementation of C++26 std::execution";
    homepage = "https://github.com/NVIDIA/stdexec";
    license = with lib.licenses; WITH asl20 llvm-exception;
    platforms = lib.platforms.linux;
  };
}
