#!/bin/bash

choice=$(printf "Pywal Themes\nMacros" | rofi -dmenu -p "Settings" -theme ~/Documents/vxwmmenu/settings.rasi)

case "$choice" in
*"Pywal Themes")
  bash "$HOME/Documents/vxwmmenu/pywal-menu.sh"
  ;;
*"Macros")
  bash "$HOME/Documents/vxwmmenu/macros-menu.sh"
  ;;
esac
