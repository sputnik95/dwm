#/bin/sh
case "$1" in
  "lower")
    amixer set Master 5%- >> /dev/null
    pkill -SIGTRAP dwmstatus
    ;;
  "raise")
    amixer set Master 5%+ >> /dev/null
    pkill -SIGTRAP dwmstatus
    ;;
  "mute")
    amixer set Master toggle >> /dev/null
    pkill -SIGTRAP dwmstatus
    ;;
  "musprev")
    cmus-remote -r
    pkill -SIGTRAP dwmstatus
    ;;
  "musnext")
    cmus-remote -n
    pkill -SIGTRAP dwmstatus
    ;;
  "mustoggle")
    cmus-remote -u
    pkill -SIGTRAP dwmstatus
    ;;
  "musstop")
    cmus-remote -s
    pkill -SIGTRAP dwmstatus
    ;;
  "update")
    pkill -SIGTRAP dwmstatus
    ;;
  "brightup")
    sudo xbacklight -inc 5
    ;;
  "brightdn")
    sudo xbacklight -dec 5
    ;;
  "screenshot_part")
    sleep 0.2
    scrot -s ~/Pictures/screenshots/$(date +%F).png
    ;;
  "screenshot_full")
    sleep 0.2
    scrot ~/Pictures/screenshots/$(date +%F).png
    ;;
  *)
    exit 1
    ;;
esac
