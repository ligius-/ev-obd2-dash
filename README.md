# Rationale #
This is a firmware project for an ESP32 board to display real-time EV data on a screen.
Currently developed for Ford Mustang Mach-E and shows real-time power usage or generation (7 updates per second), motor power per axle (every 1.5s), interior temperature, battery temperature, real state-of-charge and battery cooling mode (every 5s in sequence).

## Warning ##
This project is at this time intended for developers or people familiar with embedded systems. Though no harm can be done to the vehicle or to the additional hardware, you are taking a risk. Mosly time is at risk.

# Hardware #
Developed using an esp32-2432S028Rv3 board, though most "CYD" ESP32 boards would work at this stage. Second requirement, apart from the USB power cable. is a Bluetooth OBD2 ELM327 dongle, the cheap ones. The OBD2 dongle goes into the car port (left side under the steering wheel) and the ESP32 board can just be powered up from any 5V supply, powerbank or other USB cable. It might take 20s to connect to the dongle. IF the connection fails, see the troubleshooting section.

# Requirements #
 - esp32-2432S028Rv3 board
 - ELM327 BT V2.1 module (blue dongle)
 - PlatformIO ( https://platformio.org/platformio-ide ) - I used VsCode. This will require Python as well
 - Optional: EEZ Studio, only for the GUI part. It again requires Python as well possibly as some other dependencies.

The UI is generated with EEZ Studio 0.23.2
 
# Getting started #

A ready-made firmware is provided for convenience, in "releases", which can be flashed with https://github.com/esphome/esphome-flasher/releases/download/1.4.0/ESPHome-Flasher-1.4.0-Windows-x64.exe (or similar). However you do lose to option to customize anything.

The first requirement is to do a git clone of this project and check that the 'boards' folder has your variant of the board. If not, you can perhaps download it from https://github.com/rzeldent/platformio-espressif32-sunton/tree/ff4ef01c23cdb6ffe1e0033c2c62fd76a8a1b0c0 . In the future this should be a linked GitHub repo, however, only one board variant is supported at the moment.

Once you've installed PlatformIO, you can run `pio run`. Once the project attempts to upload, pressing and/or holding the BOOT button on the board (the one closer to the middle) will allow the framework to upload the new code.

The UI code is already generated, however, you can open the `obd2v2.eez-project` into the EEZ Studio editor and click on "Build". The generated code should not be changed by hand.

# Troubleshooting #
If you have troubles connecting to the ELM327 you need to either modify the SerialBluetooth.h and .cpp files to allow pin entry for "1234" or upload another sketch with pin entry. After doing this one time for one board, it should work.

Uploading requires that the BOOT button is pressed before the software attempts to upload. This is more easily achieved by holding down the button while the code is compiling.

`pio monitor` - the serial monitor should be used if you do any changes to the code as well as enabling debug statements in the main "sketch" or the platformio file. You should edit platformio.ini.to specify the `monitor_port`.

The schematic for the V3 board seems to match this one https://easyeda.com/editor#id=!c96dd5b38d46447fa982ac68cd9256cf

The firmware was tested with the cheap blue OBD2 dongles which have the MAC address `aa:bb:cc:11:22:33`. The fast startup is achieved by connecting to this address directly, though other dongles might have a different MAC address. In that case, make sure to comment out the define for `BT_QUICK_CONNECT` and it will try to connect to a BT device named "OBDII". If that fails, either you need to enable the PIN (default is "1234") or change the name in main.cpp.

# Units: Celsius, km/h, kW, Nm #
As of now, the firmware displays SI-based units only. The functions and macros exported for LVGL would need to be updated if you want other units. Some of the labels need updating as well.