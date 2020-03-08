# 3BtnTimer

[![Build Status](https://travis-ci.org/cat-in-136/M5Stack-3BtnTimer.svg?branch=master)](https://travis-ci.org/cat-in-136/M5Stack-3BtnTimer)

* 3BtnTimer is a simple kitchen timer app for [M5Stack](http://m5stack.com/).
* It is inspired by a simple kitchen timer which is sold at the DAISO (100-yen shop).

![](jpg/3BtnTimer.jpg)

## Requirement

Make sure you have the following libraries:

* [M5Stack library](https://github.com/m5stack/M5Stack) : It is installed if you have followed the m5stack quick-start document.
* [M5Stack-SD-Updater](https://github.com/tobozo/M5Stack-SD-Updater)

## Usage

### Install to M5Stack

1. Build the app in usual way (using Arduino IDE).
2. Put `3BtnTimer.bin` to root directory of SD card.
3. Put `jpg/*` to `jpg` directory of SD card.
4. Put `json/3BtnTimer.json` to `json` directory of SD card.
5. Boot `3BtnTimer` from SD Updater or [LovyanLauncher](https://github.com/lovyan03/M5Stack_LovyanLauncher).

### Features

* To reset, push **Min** (Button A) and **Sec** (Button B) at the same time
* To toggle beep ON/OFF, push **Min** (Button A) and **Sec** (Button B) at the same time again.
* To set the timer, push **Min** (Button A) and **Sec** (Button B). And then push **Start** (Button C) to start the timer.
* To temporary stop, push **Stop** (Button C). And then push **Start** (Button C) to resume.
* When expired the timer, the M5Stack beeps. To stop beep, push **Stop** (any button).
