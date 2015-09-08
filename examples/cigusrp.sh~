#!/bin/bash

function quitp {
printf "Quitting USRP Interference Box\n"
exit
}

function moreint {

printf "Do you want to generate more interference (y/n)?\n"
        read more
        if test "$more" = "y"
        then
                cigusrp
        else
                quitp
        fi
}

function cigusrp {

printf "==========USRP Interference Box==========\nWhich Interference you want to Generate?\n 1. Microwave Oven\n 2. Analog Cordless Phone \n 3. Digital FHSS Cordless Phone\n 4. Wireless Camera\n 5. Record and Playback device behavior\n"
read uint

if test "$uint" = "1"
        then
                printf "Generating Microwave Oven Interference\n"
                python examples/microwave.py
                moreint

elif test "$uint" = "2"
        then
                printf "select the implementation\n 1. Vtech GZ2456 cordless handset\n 2. Uniden TRU 4465-2 DSSS cordless handset system\n"
                read impdsss
 
                if test "$impdsss" = "1"
			then 
				printf "Generating Vtech GZ2456 cordless phone interference\n"
                                python analog_ph_dsss.py

                        elif test "$impdsss" = "2" 
				then
				printf "Generating Uniden TRU 4465-2 DSSS cordless phone interference\n"
	                        python dsss_phone_shyam.py
	                fi
                 moreint

elif test "$uint" = "3"
	then
		printf "Generating Digital FHSS Cordless Phone Interference\nImplementation:Uniden DCT6485-3HS cordless handset system\n"
                ./sfhss_fh.sh                
		moreint

elif test "$uint" = "4"
        then
                printf "select the modulation format\n 1. BPSK\n 2. QPSK\n 3. GFSK\n 4. 16QAM\n 5.C-501 wireless monitoring toolkit Implementation\n"
                read mod
 
                if test "$mod" = "1"
			then 
				printf "Generating Wireless camera Interference (BPSK Modulation)\n"
				./sbpsk_fh.sh
                                moreint

              elif test "$mod" = "2" 
			then
				printf "Generating Wireless camera Interference (QPSK Modulation)\n"
				./sqpsk_fh.sh
				moreint

	      elif test "$mod" = "3" 
			then
				printf "Generating Wireless camera Interference (GFSK Modulation)\n"
				./sgfsk_fh.sh
				moreint

	      elif test "$mod" = "4" 
			then
				printf "Generating Wireless camera Interference (16QAM Modulation)\n"
				./s16qam_fh.sh
				moreint		

              elif test "$mod" = "5" 
			then
				printf "Generating C-501 wireless monitoring toolkit\n"
				python fhss_tx.py
				moreint		
	        fi
                moreint

elif test "$uint" = "5"
	then
		printf "select the device you want to record\n 1. Microwave Oven\n 2. Wireless Camera\n 3. Analog Cordless Phone\n 4. Digital FHSS Phone\n"
		read dev
			
		if test "$dev" = "1"
			then 
				printf "Recording microwave oven\n"
				python /home/vaibhav/Desktop/cigusrp/record/microwave_record.py	
                                sleep 2
                                python /home/vaibhav/Desktop/cigusrp/record/microwave_playback.py		
				moreint


		elif test "$dev" = "2"
			then 
				printf "Recording wireless camera\n"
				python /home/vaibhav/Desktop/cigusrp/record/wireless_cam_record.py
                                sleep 2	
				python /home/vaibhav/Desktop/cigusrp/record/wicam_playback.py	
				moreint	


		elif test "$dev" = "3"
			then 
				printf "Recording Analog Cordless Phone\n"
				python /home/vaibhav/Desktop/cigusrp/record/analog_ph_record.py	
				sleep 2
                                python /home/vaibhav/Desktop/cigusrp/record/analog_ph_playback.py	
				moreint		

		elif test "$dev" = "4"
			then 
				printf "Recording Digital FHSS Phone\n"
				python /home/vaibhav/Desktop/cigusrp/record/digital_ph_record.py
				sleep 2
				python /home/vaibhav/Desktop/cigusrp/record/digital_ph_playback.py		
				moreint

		fi
		moreint
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
cigusrp

exit

echo "foo"
