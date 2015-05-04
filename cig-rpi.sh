#!/bin/bash

function quitp {
printf "Quitting RPi Interference Box\n"
exit
}

function moreint {

printf "Do you want to generate more interference (y/n)?\n"
        read more
        if test "$more" = "y"
        then
                cigrpi
        else
                quitp
        fi
}

function cigrpi {

printf "==========Raspberry Pi Interference Box==========\nWhich Interference you want to Generate?\n 1. Wi-Fi \n 2. Bluetooth\n 3. Zigbee\n"
read uint

if test "$uint" = "1"
        then
                printf "Install iperf if you don't have one\n say 'sudo apt-get install iperf'\n"
                printf "Say 'iperf -s' on your machine\n"
                sleep 5
                printf "Default Wi-Fi Channel ch 6\n"
                printf "Raspberry Pi will be the Client\n say 'iperf -c IP-address-of-your-machine -p 5001 -t Time you want the Interference to last'"
                iperf -c 192.168.42.11 -p 5001 -t 10
                printf "Do you want to change the Wi-Fi channel (y/n)?\n"
                read cnlwifi
                if test "$cnlwifi" = "y"
                then
                        sudo nano /etc/hostapd/hostapd.conf
                        moreint
                else
                        moreint
                fi


elif test "$uint" = "2"
        then
                sudo l2ping -i 5C:F3:70:64:C5:FE -s 200 -f 00:1A:7D:DA:71:0F
                moreint

elif test "$uint" = "3"
        then
                python /home/pi/Desktop/zigbee/xbee.py
                printf "Do you want to configure zigbee channel (y/n)?\n"
                read cnlxbee
                if test "$cnlxbee" = "y"
                then
                        minicom -o -b 9600 -D /dev/ttyUSB0
                        moreint
                else
                        moreint
                fi

else
        printf "Do you want to generate more interference (y/n)?\n"
        read more
        if test "$more" = "y"
        then
                cigrpi
        else
                quitp
        fi
fi
}

cigrpi

exit

echo foo

