# MFRC522 MFCCLI User Manual

MFRC522 MFCCLI is a Mifare Classic CLI tool for Arduino MFRC522-compliant reader/writers.

## Dependencies

Make sure you have the [MFRC522](https://github.com/miguelbalboa/rfid) library installed. On the Arduino IDE, this is credited to "GithubCommunity."

## Quick Guide

1. Follow the [wiring guide](./wiring.md) to physically wire the RFID reader/writer to the Arduino.
2. Connect to the Arduino via serial (typically using a USB cable) and upload the [Sketch](../src/ino/mfrc522-mfccli/) to the Arduino.
3. Make and run the [CLI program](../src/c/).

```sh
$ make
$ ./mfrc522-mfccli <DEVICE>
```

`<DEVICE>` is typically `/dev/ttyACM<n>` or `/dev/ttyUSB<n>` on \*NIX devices, or `COM<n>` on Windows devices, where `<n>` is a number. You can find out through the Arduino IDE.

## MFRC522 MFCCLI Reference Guide

**Starting MFRC522 MFCCLI**

```sh
$ ./mfrc522-mfccli <DEVICE>
$ ./mfrc522-mfccli /dev/ttyACM0
Waiting for worker ready signal... done.
mfrc522-mfccli $
```

**Starting MFRC522 MFCCLI (DEBUG)**

```sh
$ ./mfrc522-mfccli /dev/ttyACM0 -d
DBG: Debug mode is enabled.
Waiting for worker ready signal... done.
mfrc522-mfccli $
```

**Exiting MFRC522 MFCCLI**

```sh
mfrc522-mfccli $ exit
$
```

**Getting Help**

```sh
mfrc522-mfccli $ help
Syntax: help <COMMAND>
    <COMMAND>  read write auth detect reset
```

**Read UID**

```sh
mfrc522-mfccli $ read uid
01 02 03 04
```

**Read ATQA**

```sh
mfrc522-mfccli $ read atqa
04 00
```

**Read SAK**

```sh
mfrc522-mfccli $ read sak
08
```

**Read Block**

```sh
mfrc522-mfccli $ read block <BLOCK NUMBER>
mfrc522-mfccli $ read block 10
42 6C 6F 63 6B 20 64 61 74 61 20 68 65 72 65 2E
```

`<BLOCK NUMBER>` should be in decimal.

**Write**

```sh
mfrc522-mfccli $ write <BLOCK NUMBER> <DATA>
mfrc522-mfccli $ write 10 426C6F636B206461746120686572652E
Writing data... done.
```

`<DATA>` should be in hexadecimal, without spaces.

**Authenticate**

```sh
mfrc522-mfccli $ auth <BLOCK NUMBER> <KEY A> <KEY B>
mfrc522-mfccli $ auth 5 FFFFFFFFFFFF FFFFFFFFFFFF
Authenticating to block 5 (trailer block 7)... done.

mfrc522-mfccli $ auth A <BLOCK NUMBER> <KEY A>
mfrc522-mfccli $ auth A 5 FFFFFFFFFFFF
Authenticating to block 5 (trailer block 7)... done.

mfrc522-mfccli $ auth B <BLOCK NUMBER> <KEY B>
mfrc522-mfccli $ auth B 5 FFFFFFFFFFFF
Authenticating to block 5 (trailer block 7)... done.
```

`<BLOCK NUMBER>` should be in decimal.

`<KEY A>` and `<KEY B>` should be in hexadecimal, without spaces.

**Detect Card**

```sh
mfrc522-mfccli $ detect card
Detecting card... done.
```

**Reset**

```sh
mfrc522-mfccli $ reset
Resetting crypto... done.
```
