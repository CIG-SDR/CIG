#!/bin/bash

usage() { echo "Usage: $0 [-i <m|ap-vtech|ap-uniden|dp|wc-bpsk|wc-qpsk|wc-16qam|wc-gfsk|WC-C501>]" 1>&2; exit 1; }

while getopts ":i:" o; do
    case "${o}" in
        i)
           i=${OPTARG}
	    
	   if test "$i" = "m"
           then
	   printf "Generating Microwave Oven Interference\n"
	   python microwave.py	   
            
	   elif test "$i" = "ap-vtech" 
	   then
           printf "Generating Vtech GZ2456 cordless phone interference\n"
           python analog_ph_dsss.py

	   elif test "$i" = "ap-uniden" 
	   then
	   printf "Generating Uniden TRU 4465-2 DSSS cordless phone interference\n"
           python dsss_phone_shyam.py

           elif test "$i" = "dp" 
	   then
           printf "Generating Digital FHSS Cordless Phone Interference\nImplementation:Uniden DCT6485-3HS cordless handset system\n"
           ./sfhss_fh.sh 

           elif test "$i" = "wc-bpsk" 
	   then
           printf "Generating Wireless camera Interference (BPSK Modulation)\n"
	   ./sbpsk_fh.sh

           elif test "$i" = "wc-qpsk" 
	   then
           printf "Generating Wireless camera Interference (QPSK Modulation)\n"
	   ./sqpsk_fh.sh          

           elif test "$i" = "wc-16qam" 
	   then
	   printf "Generating Wireless camera Interference (16QAM Modulation)\n"
           ./s16qam_fh.sh

           elif test "$i" = "wc-gfsk" 
	   then
           printf "Generating Wireless camera Interference (GFSK Modulation)\n"
           ./sgfsk_fh.sh

           elif test "$i" = "wc-C501" 
	   then
	   printf "Generating C-501 wireless monitoring toolkit\n"
	   python fhss_tx.py

           fi
           ;;
        *)
            usage
            ;;
    esac
done
shift $((OPTIND-1))

if [ -z "${i}" ]; then
    usage
fi





