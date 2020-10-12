# MFJLoopController
A ESP8266-based remote controller for MFJ magnetic loops.

## Prerequisites

* WEMOS D1 mini Lite development board (or clone). [Example](https://www.amazon.com/gp/product/B081PX9YFV/ref=ppx_yo_dt_b_asin_title_o03_s00?ie=UTF8&psc=1)
* [Adafruit ADS1115 ADC](https://www.adafruit.com/product/1085)
* Arduino IDE with ESP boards and [LittleFS data uploader tool](https://arduino-esp8266.readthedocs.io/en/latest/filesystem.html#uploading-files-to-file-system) installed
* [MFJ Bias Tee](https://mfjenterprises.com/products/mfj-4116)
* [H bridge circuit](http://www.modularcircuits.com/blog/articles/h-bridge-secrets/h-bridges-the-basics/) to produce +/- 12V. Initial tests done with [LED based solid state relays](https://www.mouser.com/ProductDetail/IXYS-Integrated-Circuits/LCC120?qs=TzJIIqIe2vmIthf%252BEZpoaA%3D%3D) linked together in series to reduce required pin count to 2 (one for each direction).
* Isolated power supply (e.g. **NOT** connected to radio's ground). [This Mouser project](https://www.mouser.com/ProjectManager/ProjectDetail.aspx?AccessID=d2ae561f5a) seems to work reasonably well in initial testing to permit sharing 12V power supply with rest of shack (see datasheets for the TMR 9-1212 for required support circuitry to reduce possible RFI).
* Soon: SWR bridge capable of reading 100W and providing 3.3V max analog signals to the ADS1115 ([possible option](https://en.golb.be/automatic-magnetic-loop-tuner-swr-bridge/))

## Setup

1. Set configuration options (pins, Wi-Fi info, etc.) in config.h.
2. Build and upload software using Arduino IDE to device. *Note: the initial upload must be done using USB or UART; subsequent uploads can be done OTA.*
3. Upload filesystem using Tools > ESP8266 LittleFS Data Upload in the Arduino IDE.

## Usage

By default, the software uses DHCP to get an IP address. Simply go to its assigned IP address on port 80 in a Web browser and click on the desired tuning direction buttons; releasing the associated button results in tuning stop. Note that you're looking for maximum signal/noise for the initial pass at tuning; the eventual auto-tune feature will perform fine tuning for minimal SWR.

## To Do

* Auto-tuning
* SSL support?
* PCB combining all hardware and eliminating loose wires

## License

BSD 3-clause (TBD)
