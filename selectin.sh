#!/bin/bash

printf "Select the interface\n1:SDR Radio\n2:Standard communication dongles\n"
        read more
        if test "$more" = "1"
        then
                ./cig-rpi.sh
        else
                ./cigusrp.sh
        fi
exit

echo foo
