# Animation effects for NeoPixels controlled by a NodeMCU

__This is work in progress. I've just uploaded my CRUD project files for now. Please be patient with me.__

### TL;DR

* Hook up your NeoPixel to `D5`.
* Adjust `NUMPIXELS_1` to the number of leds on your led strip or chain.
* Run a couple of commands:
```bash
$ cp ./platformio-dist.ini platformio.ini
$ pio run -e nodemcuv2 -t upload && pio serialports monitor -b 115200
```

Weee :-)


# NeoPixelAnimations library

Library is:
* designed to be able to run several animations at the same time on different databusses.
* suited for your halloween dress.
* suited for NeoPixel led strips.

... more to come.

# Contribute
Please contribute with pull-requests.