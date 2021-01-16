![paperdash.io](./doc/logo-readme.svg)

[![Software License](https://img.shields.io/badge/license-MIT-brightgreen.svg)]()
[![Generic badge](https://img.shields.io/badge/REST_API-1.0-<COLOR>.svg?logo=swagger)](https://app.swaggerhub.com/apis-docs/paperdash.io/paperdash-epd/0.1)
[![Generic badge](https://img.shields.io/badge/Firmware-1.0-orange.svg?logo=arduino)]()
[![Generic badge](https://img.shields.io/badge/APP-1.0-orange.svg?logo=vuetify)]()
[![Generic badge](https://img.shields.io/badge/3D_Housing-1.0-orange.svg?logo=makerbot)](https://www.thingiverse.com/thing:4724292)

[![ForTheBadge built-with-love](http://ForTheBadge.com/images/badges/built-with-love.svg)](https://GitHub.com/Naereen/)


## Open Source E-Paper Display Platform

* 🌍 [Website](https://paperdash.io/) - Learn more about paperdash.io
* 📡 [Discover](http://local.paperdash.io/) - Find devices in your local network
* 📡 [Case](https://www.thingiverse.com/thing:4724292) - 3D Model
* 👍 Like us on [Instagram](https://instagram.com/paperdash.io)

![Face Calendar](./doc/calendar.jpg)
![Face Picture](./doc/livingroom.jpeg)
![APP](./doc/app.png)


## Goals
* Make E-Paper displays accessible to all kind of developers
* Easy and always update able
* Usable also without developer background
* Deliver the best user experience
* Should work `Out-of-the-box`
* Using standard parts to built

## Features

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

* ESP32
* EPD display: GDEW075T8
* (Optional) 3D printed case TODO link thinkerverse

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
