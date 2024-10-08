#!/bin/bash

if [ "$#" -lt 1 ]; then
  echo "Benutzung: $0 [app|editor] [optionen]"
  exit 1
fi

# Entscheiden, welches Programm ausgeführt wird
case "$1" in
  app)
    shift # Entfernt das erste Argument ("app")
    exec /app/bin/PhotoboxApp "$@"
    ;;
  editor)
    shift # Entfernt das erste Argument ("editor")
    exec /app/bin/CollageEditorApp "$@"
    ;;
  *)
    echo "Ungültige Auswahl: $1"
    echo "Benutzung: $0 [app|editor] [optionen]"
    exit 1
    ;;
esac