# paperdash-display

## build for deployment
```properties
pio run --environment lolin32
platformio run --target buildfs
```


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

# get current picture as bmp
curl http://paperdash-epd/api/device/screen --output current.bmp

# erase flash memory
pio run --target erase
```

## access fs over http

> get photos from calendar face
http://paperdash-epd/fs/calendarPhoto.png
