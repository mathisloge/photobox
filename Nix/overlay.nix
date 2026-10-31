# SPDX-FileCopyrightText: 2026 Mathis Logemann <mathis.opensource@tuta.io>
#
# SPDX-License-Identifier: GPL-3.0-or-later
final: prev: {
  stdexec = final.callPackage ./stdexec.nix { };
}
