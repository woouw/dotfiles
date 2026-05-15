#!/bin/bash

MACROS_DIR="$HOME/Documents/vxwmmenu/macros"

selected=$(ls "$MACROS_DIR"/*.sh 2>/dev/null | xargs -I{} basename {} .sh | rofi -dmenu -p "Macros" -theme ~/Documents/vxwmmenu/settings.rasi)

[ -z "$selected" ] && exit

bash "$MACROS_DIR/$selected.sh"
