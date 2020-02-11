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
```