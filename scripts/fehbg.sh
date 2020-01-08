#!/usr/bin/bash
DISPLAY=:0 feh --randomize --bg-fill --no-fehbg /home/aezakmi/Pictures/wallpapers
if pgrep -x root-tail >> /dev/null; then
    pkill root-tail
fi
root-tail -g 1200x270+10-10 -font -*-terminus-medium-*-normal-*-12-*-*-*-*-*-*-* -outline -color rgb:eacc/cc/cc --update --noflicker --wordwrap /var/log/socklog/everything/current &
