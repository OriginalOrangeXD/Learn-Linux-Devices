#!/usr/bin/python3

import random

SAMPLE_SIZE = 1000

headcount = 0
coinflips = []

for i in range(SAMPLE_SIZE):
    newflip = random.randint(0,1)
    if ( newflip == 0 ):
        headcount += 1
    coinflips.append(newflip)

print("Heads: " + str(headcount))
print("Tails: " + str(SAMPLE_SIZE - headcount))
