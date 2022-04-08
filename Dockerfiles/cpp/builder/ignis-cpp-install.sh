#!/bin/bash

export DEBIAN_FRONTEND=noninteractive
apt update
apt -y install g++
rm -rf /var/lib/apt/lists/*

cp -R ${IGNIS_HOME}/core/cpp/lib/* ${IGNIS_HOME}/lib
rm -fR ${IGNIS_HOME}/core/cpp/lib
ldconfig
