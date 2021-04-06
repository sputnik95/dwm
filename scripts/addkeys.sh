#!/bin/sh -

if [[ ! -z $(mpc current 2>/dev/null) ]]; then PLAYER=mpd; fi
if [[ ! -z $(playerctl metadata 2>/dev/null) ]]; then PLAYER=playerctl; fi


case "$1" in
  "lower")
    amixer set Master 5%- >> /dev/null
    ;;
  "raise")
    amixer set Master 5%+ >> /dev/null
    ;;
  "mute")
    amixer set Master toggle >> /dev/null
    ;;
  "mute_mic")
    amixer set Capture toggle >> /dev/null
    ;;
  "musprev")
    if [ "$PLAYER" == "playerctl" ]; then playerctl previous; elif [ "$PLAYER" ==  "mpd" ]; then mpc prev >/dev/null; else cmus-remote -r; fi
    ;;
  "musnext")
    if [ "$PLAYER" == "playerctl" ]; then playerctl next; elif [ "$PLAYER" ==  "mpd" ]; then mpc next >/dev/null; else cmus-remote -n; fi
    ;;
  "mustoggle")
    if [ "$PLAYER" == "playerctl" ]; then playerctl play-pause; elif [ "$PLAYER" ==  "mpd" ]; then mpc toggle >/dev/null; else cmus-remote -u; fi
    ;;
  "musstop")
    if [ "$PLAYER" == "playerctl" ]; then playerctl stop; elif [ "$PLAYER" ==  "mpd" ]; then mpc stop >/dev/null; else cmus-remote -s; fi
    ;;
  "update")
    ;;
  "brightup")
    sudo xbacklight -inc 5
    ;;
  "brightdn")
    sudo xbacklight -dec 5
    ;;
  "screenshot_part")
    sleep 0.2
    maim -s ~/Pictures/screenshots/$(date +%F_%T).png
    ;;
  "screenshot_full")
    sleep 0.2
    maim ~/Pictures/screenshots/$(date +%F_%T).png
    ;;
  "bar_toggle")
    herbstclient pad 0 0
    polybar-msg cmd toggle >/dev/null
    ;;
  "fehbg")
    FILE=`ls -p ~/Pictures/wallpapers | grep -v / | shuf -n 1`
    echo "/home/aezakmi/Pictures/wallpapers/$FILE" > /tmp/wallpaper_path
    DISPLAY=:0 feh --bg-fill --no-fehbg $(cat /tmp/wallpaper_path)
    ;;
  "lock_screen")
    i3lockr --blur 45 -- --nofork --ignore-empty-password
    #  MD5_WP=`md5sum $(cat /tmp/wallpaper_path 2>/dev/null) | awk '{ print $1 }'`
    #  MD5_LOCK=$(cat /tmp/lock_md5 2>/dev/null | awk '{ print $1 }')
    #  if [ "$MD5_WP" != "$MD5_LOCK" ]
    #  then
    #    md5sum $(cat /tmp/wallpaper_path) > /tmp/lock_md5
    #    convert -resize 1400 -quality 20 -define jpeg:extent=400kb -blur 0x20 $(cat /tmp/wallpaper_path) /tmp/lock.png
    #    chown $UID:$GID /tmp/lock.png /tmp/lock_md5
    #  else
    #      :
    #  fi
    #i3lock -eft -p win -i /tmp/lock.png
    ;;
  "dmenu_brightness")
    echo $(cat /sys/class/backlight/acpi_video0/brightness | dmenu -i -fn "Terminus:size=8" -nb "#111111" -nf "#8b8792" -sb "#111111" -sf "#cfcfcf" -p "New brightness level: ") | awk '{print int(($1+5)/10) * 10}' | sudo tee /sys/class/backlight/acpi_video0/brightness
    ;;
  "rename_title")
    xdotool set_window --name "$(echo $(xdotool getactivewindow getwindowname | dmenu -i -fn "Terminus:size=8" -nb "#111111" -nf "#8b8792" -sb "#111111" -sf "#cfcfcf" -p "New window title: "))" `xdotool getactivewindow`
    ;;
  "rofi-bangs")
      ~/.local/scripts/rofi/rofi-bangs.sh
      ;;
  "polybar-mon")
      if type "xrandr"; then
        for m in $(xrandr --query | grep " connected" | cut -d" " -f1); do
            MONITOR=$m polybar --reload i3 &
        done
      else
        polybar --reload i3 &
      fi
      ;;
  *)
    exit 1
    ;;
esac

