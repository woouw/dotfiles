#!/bin/sh

export XDG_SESSION_TYPE=x11

xrandr --output eDP1 --rotate inverted
xrandr --output DP1 --above eDP1
xrandr --output DP1 --primary

cpupower frequency-set -g performance &
wal -R &
cnice &
polybar &
picom -b &

exec vxwm
