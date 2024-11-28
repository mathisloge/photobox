#!/bin/bash
# SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
#
# SPDX-License-Identifier: GPL-3.0-or-later

if [ "$#" -lt 1 ]; then
  echo "Benutzung: $0 [app|editor] [optionen]"
  exit 1
fi

case "$1" in
  app)
    shift # removes ("app")
    exec /app/bin/PhotoboxApp "$@"
    ;;
  editor)
    shift # removes ("editor")
    exec /app/bin/CollageEditorApp "$@"
    ;;
  *)
    echo "Ungültige Auswahl: $1"
    echo "Benutzung: $0 [app|editor] [optionen]"
    exit 1
    ;;
esac
