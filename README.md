# Small segway based on Arduino 101

## Platformio

Installation

```sh
yaourt -S platformio
```

Programming

```sh
mkdir PROJECT
platformio init --board genuino101 -d PROJECT
platformio run -t upload -d PROJECT/src/CODE.*
platformio device monitor -b 115200
```

If you want to upload with device Arduino UNO.

```sh
platform run -t upload -e uno
```

## Using Arduino command line

```sh
arduino --get-pref
arduino --verify
arduino --port PORTNAME
arduino --board arduino_101
```




