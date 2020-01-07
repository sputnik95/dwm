#!/bin/bash

print_date() {
	FORMAT="%R %b %d %a"
	DATE=`date "+${FORMAT}"`
	echo "${DATE}"
}

separate() {
    echo -n "|"
}

print_bat() {
    CH_STATUS=`cat /sys/class/power_supply/BAT0/status`
    if [[ $CH_STATUS == 'Discharging' ]]; then
        CH_STATUS='dis.'
    elif [[ $CH_STATUS == 'Charging' ]]; then
        CH_STATUS='ch.'
    elif [[ $CH_STATUS == 'Unknown'  || $CH_STATUS == 'Full' ]]; then
        CH_STATUS='bat.'
    fi
    BAT_LEVEL=`cat /sys/class/power_supply/BAT0/capacity`
    OUTPUT="${CH_STATUS}${BAT_LEVEL}%"
    if [[ $BAT_LEVEL > '65' && $BAT_LEVEL < '99' && $CH_STATUS == 'dis.' ]]; then
        OUTPUT="\x03${CH_STATUS}${BAT_LEVEL}%\x01"
    fi
    if [[ $BAT_LEVEL -le '65' && $CH_STATUS == 'dis.' ]]; then
        OUTPUT="\x04${CH_STATUS}${BAT_LEVEL}%\x01"
    fi
    echo -e "${OUTPUT}"
}

print_temp() {
    CUR_TEMP=`cat /sys/devices/platform/thinkpad_hwmon/hwmon/hwmon0/temp1_input | rev | cut -c4- | rev`
    if [[ $CUR_TEMP > '70' ]]; then
        CUR_TEMP="${CUR_TEMP}°C"
    fi
    OUTPUT="${CUR_TEMP}°C"
    if [[ $CUR_TEMP > '55' && $CUR_TEMP < '65' ]]; then
        OUTPUT="\x03${CUR_TEMP}°C\x01"
    fi
    if [[ $CUR_TEMP -ge '65' ]]; then
        OUTPUT="\x04${CUR_TEMP}°C\x01"
    fi
    echo -e "${OUTPUT}"
}

print_vol() {
    CURRENT_STATE=`amixer get Master | egrep 'Playback.*?\[o' | egrep -o '\[o.+\]'`
    if [[ $CURRENT_STATE == '[off]' ]]; then
        echo -n "mute"
    elif [[ $CURRENT_STATE == '[on]' ]]; then
        VOL_LEVEL=`amixer get Master | awk '$0~/%/{print $4}' | tr -d '[]'`
        echo -n "vol.${VOL_LEVEL}"
    fi
}

print_layout() {
    LAYOUT=`xkblayout-state print %s`
    echo -n "${LAYOUT}"
}

print_song() {
    INFO_CMUS=$(cmus-remote -Q 2>/dev/null)
    if [[ $? -eq 0 ]]; then
        INFO_TITLE=$(echo "${INFO_CMUS}" | sed -n -e 's/^.*title//p' | head -n 1)
        INFO_ALBUM=$(echo "${INFO_CMUS}" | sed -n -e 's/^.*album//p' | head -n 1)
        INFO_ARTIST=$(echo "${INFO_CMUS}" | sed -n -e 's/^.*artist //p' | head -n 1)
        INFO_SEC_DUR=$(cmus-remote -Q | sed -n -e 's/^duration//p' | cut -c2-)
        INFO_SEC_POS=$(cmus-remote -Q | sed -n -e 's/^position//p' | cut -c2-)
        INFO_DUR=$(printf '%d:%02d' $(($INFO_SEC_DUR%3600/60)) $(($INFO_SEC_DUR%60)))
        INFO_POS=$(printf '%d:%02d' $(($INFO_SEC_POS%3600/60)) $(($INFO_SEC_POS%60)))
        if [[ "${INFO_ARTIST}" ]] && [[ "${INFO_TITLE}" ]]; then
            OUT_TEXT="${INFO_ARTIST} -${INFO_TITLE} [${INFO_POS}/${INFO_DUR}] |"
        elif [[ "${INFO_TITLE}" ]]; then
            OUT_TEXT="${INFO_TITLE} ^fg(${SEPARATOR})|^fg(${DEFAULT_FG}) "
        elif [ -z "${INFO_TITLE}" ]; then
            OUT_TEXT=""
        fi
    else
        OUT_TEXT=""
    fi
    echo -n "${OUT_TEXT}"
}

print_netspeed() {
    # Get custom IN and OUT labels if provided by command line arguments
    while [[ $# -gt 1 ]]; do
        key="$1"
        case "$key" in 
            -i|--inlabel)
                INLABEL="$2"
                shift;;
            -o|--outlabel)
                OUTLABEL="$2"
                shift;;
        esac
        shift
    done
    
    [[ -z "$INLABEL" ]] && INLABEL="IN "
    [[ -z "$OUTLABEL" ]] && OUTLABEL="OUT "
    
    # Use the provided interface, otherwise the device used for the default route.
    if [[ -z $INTERFACE ]] && [[ -n $BLOCK_INSTANCE ]]; then
      INTERFACE=$BLOCK_INSTANCE
    elif [[ -z $INTERFACE ]]; then
      INTERFACE=$(ip route | awk '/^default/ { print $5 ; exit }')
    fi
    
    # Exit if there is no default route
    [[ -z "$INTERFACE" ]]
    
    TREAT_UNKNOWN_AS_UP=1
    # Issue #36 compliant.
    if ! [ -e "/sys/class/net/${INTERFACE}/operstate" ] || \
        (! [ "$TREAT_UNKNOWN_AS_UP" = "1" ] && 
        ! [ "`cat /sys/class/net/${INTERFACE}/operstate`" = "up" ])
    then
        echo "$INTERFACE down"
        echo "$INTERFACE down"
        echo "#FF0000"
    fi
    
    # path to store the old results in
    path="/dev/shm/$(basename $0)-${INTERFACE}"
    
    # grabbing data for each adapter.
    read rx < "/sys/class/net/${INTERFACE}/statistics/rx_bytes"
    read tx < "/sys/class/net/${INTERFACE}/statistics/tx_bytes"
    
    # get time
    time=$(date +%s)
    
    # write current data if file does not exist. Do not exit, this will cause
    # problems if this file is sourced instead of executed as another process.
    if ! [[ -f "${path}" ]]; then
      echo "${time} ${rx} ${tx}" > "${path}"
      chmod 0666 "${path}"
    fi
    
    # read previous state and update data storage
    read old < "${path}"
    echo "${time} ${rx} ${tx}" > "${path}"
    
    # parse old data and calc time passed
    old=(${old//;/ })
    time_diff=$(( $time - ${old[0]} ))
    
    # sanity check: has a positive amount of time passed
    [[ "${time_diff}" -gt 0 ]] || exit
    
    # calc bytes transferred, and their rate in byte/s
    rx_diff=$(( $rx - ${old[1]} ))
    tx_diff=$(( $tx - ${old[2]} ))
    rx_rate=$(( $rx_diff / $time_diff ))
    tx_rate=$(( $tx_diff / $time_diff ))
    
    # shift by 10 bytes to get KiB/s. If the value is larger than
    # 1024^2 = 1048576, then display MiB/s instead
    
    # incoming
    echo -n "$INLABEL"
    rx_kib=$(( $rx_rate >> 10 ))
    if hash bc 2>/dev/null && [[ "$rx_rate" -gt 1048576 ]]; then
      printf '%sM' "`echo "scale=1; $rx_kib / 1024" | bc`"
    else
      echo -n "${rx_kib}K"
    fi
    
    echo -n " "
    
    # outgoing
    echo -n "$OUTLABEL"
    tx_kib=$(( $tx_rate >> 10 ))
    if hash bc 2>/dev/null && [[ "$tx_rate" -gt 1048576 ]]; then
      printf '%sM |' "`echo "scale=1; $tx_kib / 1024" | bc`"
    else
      echo -n "${tx_kib}K |"
    fi
}

SLEEP_SEC=1
while :; do
    xsetroot -name "$(print_song) $(print_netspeed) $(print_temp) $(separate) $(print_vol) $(separate) $(print_layout) $(separate) $(print_bat) $(separate) $(print_date)"
    sleep $SLEEP_SEC
done
