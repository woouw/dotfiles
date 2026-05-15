#!/bin/bash

WALLPAPER_DIR="$HOME/Documents/vxwmmenu/wallpapers"

selected=$(ls "$WALLPAPER_DIR" | rofi -dmenu -p "Select Wallpaper" -theme ~/Documents/vxwmmenu/settings.rasi)

[ -z "$selected" ] && exit

wal -i "$WALLPAPER_DIR/$selected" && polybar-msg cmd restart
