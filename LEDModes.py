#!/usr/bin/env python3

import subprocess
#import psutil
from time import sleep

def intersperse(iterable, delimiter):
    it = iter(iterable)
    yield next(it)
    for x in it:
        yield delimiter
        yield x

def sendColor(args):
    args += ['0'] * (8 - len(args))
    colorStr = "".join(list(intersperse(args, ',')))
    print(colorStr)
    subprocess.run(["sendcolors", colorStr, "/dev/ttyUSB1"])

def hexToRGB(hexVal):
    return list(map(lambda x: str(x).zfill(3), [int(hexVal[i: i + 2], 16) for i in (0, 2, 4)]))

def fade(delayVal):
    sendColor(list(map(str, [2, delayVal])))

def singleColor(hexVal):
    sendColor(["0"] + hexToRGB(hexVal))

def blink(delayVal, hexVal1, hexVal2):
    sendColor(["1", str(delayVal)] + hexToRGB(hexVal1) + hexToRGB(hexVal2))

fade(5)
'''
while True:
    percentage = psutil.cpu_percent(interval = 1)
    if percentage < 50.0:
        singleColor("00FF00")
    elif percentage < 75 and percentage > 50:
        singleColor("FF3300")
    else:
        singleColor("FF0000")
'''
