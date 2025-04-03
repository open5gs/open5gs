#!/bin/bash

meson setup --wipe build && ninja -C build
mkdir sudo mkdir -p /usr/local/lib/freeDiameter
sudo cp /home/ilinsky/open5gs_build/open5gs/build/subprojects/freeDiameter/extensions/* /usr/local/lib/freeDiameter/ -R
sudo cp /home/ilinsky/open5gs_build/open5gs/build/subprojects/freeDiameter/extensions/* /usr/local/lib/aarch64-linux-gnu/freeDiameter/ -R
sudo cp /home/ilinsky/open5gs_build/open5gs/build/subprojects/freeDiameter/extensions/dict_dcca_3gpp/*  /usr/local/lib/aarch64-linux-gnu/freeDiameter/ -R