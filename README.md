# Animation effects for NeoPixels controlled by a NodeMCU

__This is work in progress. I've just uploaded my CRUD project files for now. Please be patient with me.__

### TL;DR

* Hook up your NeoPixel to `D7`.
* Adjust `NUMPIXELS_1` to the number of leds on your led strip or chain.
* Run a couple of commands:
```bash
$ cp ./platformio-dist.ini platformio.ini
$ pio run -e nodemcuv2 -t upload && pio serialports monitor -b 115200
```

Weee :-)


# NeoPixelAnimations library


### Library is:

* Designed to be able to run several animations at the same time on different databusses. Example in [examples/main-3-databus.cpp](examples/main-3-databus.cpp).
* Suited for your halloween dress. Example in [examples/main-1-databus-dress.cpp](examples/main-1-databus-dress.cpp)
* Suited for NeoPixel led strips, rings, bars and other kind of led chains.
* Suited for [WS2811 12mm diffused led chains](https://www.aliexpress.com/item/50-Pcs-string-12mm-WS2811-2811-IC-RGB-LED-Pixels-Module-String-Light-Black-Wire-cable/1854864234.html?spm=2114.13010608.0.0.N5H7Hk)  [examples/main-1-databus-xmas-tree.cpp](examples/main-1-databus-xmas-tree.cpp)
* Suited for toilet light display via MQTT [examples/main-1-databus-toilet-light.cpp](examples/main-1-databus-toilet-light.cpp)
... more to come.


## Build the Software

Build and make new binary for over the air updates:

```bash
$ pio run -e nodemcuv2 && cp .pioenvs/nodemcuv2/firmware.bin bin/nodemcu/toilet-light/
```

Manual build and upload:
```bash
$ pio run -e nodemcuv2 -t upload && pio serialports monitor -b 115200
```

## Software update Over The Air

To update the NodeMCU with chip id `1548096`; post this to MQTT topic `awsiot_to_localgateway_toilet`.

```json
{
    "chipId": 1548096,
    "doUpdate": 1
}
```


## Other resources

* [Getting started with NodeMCU, Arduino or Wemos D1 mini](https://github.com/5orenso/arduino-getting-started)
* [NodeMCU code for the toilet sensors integrated with AWS IoT](https://github.com/5orenso/nodemcu-mqtt-toilet-project)
* [NodeMCU code for the toilet lights integrated with AWS IoT](https://github.com/5orenso/nodemcu-mqtt-toilet-project-display)
* [NodeMCU code for home sensors integrated with AWS IoT](https://github.com/5orenso/nodemcu-mqtt-home-sensors)
* [NodeMCU code for BME280 sensor integrated with AWS IoT](https://github.com/5orenso/nodemcu-mqtt-bme280)
* [NodeMCU code for DallasTemperature sensor integrated with AWS IoT](https://github.com/5orenso/nodemcu-mqtt-dallastemperature)
* [Arduino code for Neopixel animations](https://github.com/5orenso/nodemcu-neopixel-animations)
* [AMI for MQTT Broker](https://github.com/5orenso/aws-ami-creation)
* [All PlatforIO.org libs by Sorenso](http://platformio.org/lib/search?query=author%253A%2522Sorenso%2522)


## Contribute

Please contribute with pull-requests.
