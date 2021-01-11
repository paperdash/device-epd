![paperdash.io](./doc/logo-readme.svg)

![Maintenance](https://img.shields.io/badge/Maintained%3F-yes-green.svg)
![Software License](https://img.shields.io/badge/license-MIT-brightgreen.svg)
[![Generic badge](https://img.shields.io/badge/REST_API-0.1-<COLOR>.svg?logo=swagger)](https://app.swaggerhub.com/apis-docs/paperdash.io/paperdash-epd/0.1)
![Generic badge](https://img.shields.io/badge/Firmware-0.1-orange.svg?logo=arduino)
![Generic badge](https://img.shields.io/badge/APP-0.1-orange.svg?logo=vuetify)
![Lines of code](https://img.shields.io/tokei/lines/github/paperdash/device-epd)

[![ForTheBadge built-with-love](http://ForTheBadge.com/images/badges/built-with-love.svg)](https://GitHub.com/Naereen/)
<form action="https://www.paypal.com/donate" method="post" target="_top">
	<input type="hidden" name="hosted_button_id" value="HBVQC64JTW68N" />
	<input type="image" src="https://www.paypalobjects.com/en_US/i/btn/btn_donate_LG.gif" border="0" name="submit" title="PayPal - The safer, easier way to pay online!" alt="Donate with PayPal button" />
	<img alt="" border="0" src="https://www.paypal.com/en_DE/i/scr/pixel.gif" width="1" height="1" />
</form>


<!--
[![GitHub license](https://img.shields.io/github/license/Naereen/StrapDown.js.svg)](https://github.com/Naereen/StrapDown.js/blob/master/LICENSE)
[![saythanks](https://img.shields.io/badge/say-thanks-ff69b4.svg)](https://saythanks.io/to/kennethreitz)
-->


## Open Source E-Paper Display Platform

hello world beispiel für neue face

***out of the box device***

***usable for everyone and not only developer***


TODO, up to 3 images (2x device, 1x web app)

## Features
TODO ......

### Firmware
* OTA update
* [REST API](https://app.swaggerhub.com/apis-docs/paperdash.io/paperdash-epd/0.1)
* JPEG decoder (some limitations)
* PNG decoder (work in progress)
* Out-of-the-box slideshow with
  * Current weather condition and forecast (https://openweathermap.org/ api key requried)
  * Calendar view with random picture
  * Today view with fullscreen random picture
* Wifi STA fallback if connection failed

### Web APP
* ESP-32 optimized vue.js Web APP
* [Material Design Framework](https://vuetifyjs.com/)
* First run setup wizard
* Screen preview of epd
* Basic PWA possibility


# Getting started

## Necessary parts to build the project

* ESP-32 microprocessor
  * Amazon affiliate link [DE](https://amzn.to/34OYBol), US, UK, SP
* TODO 640x384, 7.5inch E-Ink display. Black and white ONLY. (GDEW075T8 7.5" b/w)
  * Amazon affiliate link [DE](https://amzn.to/34OYNnz)
* 3D printed case

## ESP-32 firmware

> The whole architecture was inspired from the arduino concept with a setup() and loop() function.
> This is the reason why everything is broken down into separate modules below `/lib`. Each module has basically a `setupXXX()` and `loopXXX()` method.

### Build (with PlatformIO)
```
$ pio run --environment esp32

// update esp
$ platformio run --target upload
```

## Web APP

> Based on state of the art technologie [Vue.js](https://vuejs.org/) + [Vuetify](https://vuetifyjs.com/) + [Webpack](https://webpack.js.org/)

### Build
```
$ yarn --cwd app
$ yarn --cwd app build

// update esp
$ platformio run --target uploadfs

// alternative, create spiffs.bin
$ platformio run --target buildfs

```


## API examples

Post a image to display
```
$ curl -F '=@demo.jpg;filename=dithering' http://paperdash-epd/api/device/screen
```

Access root filesystem

http://paperdash-epd/fs/calendarPhoto.png



## Similar projects

> During my research, I have found exciting projects that are very similar to this one.
- https://github.com/caedm/wall-ink
- https://github.com/rgujju/paperdink
- https://github.com/aceisace/Inkycal
- https://github.com/ankesreich/eInkDisplayDriver

------------------------------------------------


## compile and upload new firmware
```properties
platformio run --target upload
```

## app development
```properties
# build new dist files
yarn --cwd app build

# upload to device filesystem
platformio run --target uploadfs

# send a picture to display over json
curl -F '=@test3.png;filename=dithering' http://paperdash-epd/api/device/screen
curl -F 'filename=@test1.jpg' http://paperdash-epd/api/device/screen

# get current picture as bmp
curl http://paperdash-epd/api/device/screen --output current.bmp

# erase flash memory
pio run --target erase
```

## access fs over http

> get photos from calendar face
http://paperdash-epd/fs/calendarPhoto.png



## swagger documentation
https://app.swaggerhub.com/apis-docs/paperdash.io/paperdash-epd/0.1