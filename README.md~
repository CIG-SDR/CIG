# SDR Platform for Controllable Interference Generation

## GNU Radio v3.7
You need at least version 3.7.3.
Refer this link to install GNU Radio 
https://gnuradio.org/redmine/projects/gnuradio/wiki/InstallingGR

## Required Libraries:

1. gr-spread: https://bitbucket.org/daulpavid/gr-spread

2. gr-cdma: https://github.com/anastas/gr-cdma

3. gr-802.15.4: https://github.com/bastibl/gr-ieee802-15-4

4. gr-802.11: https://github.com/bastibl/gr-ieee802-11

5. gr-foo: https://github.com/bastibl/gr-foo.git

The dependencies and installation intructions are provided in the above links. 

## Hardware
* CIG is based and tested on USRP N210 with SBX Daughterboard. 
* Raspberry Pi Model B+
* WiFi Dongle: http://www.adafruit.com/product/1030
* XBee Module: http://www.adafruit.com/products/964
* XBee expansion board: http://www.adafruit.com/products/247
* Bluetooth Dongle: https://www.digitec.ch/en/s1/product/asus-usb-bt400-usb-20-network-adapters-630793?tagIds=7
* BLE Dongle: https://www.bluegiga.com/en-US/products/bled112-bluetooth-smart-dongle/


## Tools Required

* iPerf: https://iperf.fr/
* l2ping: http://linuxcommand.org/man_pages/l2ping1.html
* Raspbian OS: https://www.raspbian.org/

### GRC Files

Contains the grc files which are used as a base for constructing the flowgraphs. The generated python files
from the flowgraphs are further edited to configure them according to device specifications. 

### Record Interference

Contains flow graphs and scripts to record and packpay the behavior of the devices used which are specified above. 
The RX and TX Gain, Bandwidth and the Sampling rate is configured according to the device specifications. The distance
between the device and USRP is 2mts. 

### examples

Contains the scripts to generate and control the interference of prevalent wireless devices operating in the 2.4GHz ISM band

Run the ./cigusrp.sh to know the devices present and configuartion of the CIG. 

### Raspberry Pi Config

Contains the scripts to configure Raspberry Pi as a traffic generator of ZigBee, WiFi, Bluetooth and BLE.  

Run ./cig-rpi to configure the Raspberry Pi as a traffic generator

