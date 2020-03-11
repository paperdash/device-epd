# paperdash-display

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
curl -F 'myImage=@blackPNG.png' http://192.168.178.62/api/face

# erase flash memory
pio run --target erase
```

## access fs over http

> get photos from calendar face
http://192.168.178.65/fs/calendarPhoto.png


# original
echo "${$(openssl s_client -connect raw.githubusercontent.com:443 2>/dev/null </dev/null | sed -ne '/-BEGIN CERTIFICATE-/,/-END CERTIFICATE-/p' | openssl x509 -noout -fingerprint -sha1)//:/ }"


openssl s_client -connect images.unsplash.com:443 2>/dev/null </dev/null | sed -ne '/-BEGIN CERTIFICATE-/,/-END CERTIFICATE-/p' | openssl x509 -noout -fingerprint -sha1