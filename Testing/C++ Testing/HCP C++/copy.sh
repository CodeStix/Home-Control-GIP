#!/bin/bash
# Copy created files to other location. Stijn Rogiest 2/4/2019

for value in "Device.cpp" "Device.h" "Request.cpp" "Request.h" "Packet.cpp" "Packet.h" "HCPClient.cpp" "HCPClient.h"
do
    echo "Copying $value..."
    yes | cp -rf "$value" "./../../../Home Control Protocol/HCP_MCU_v3/$value"
done