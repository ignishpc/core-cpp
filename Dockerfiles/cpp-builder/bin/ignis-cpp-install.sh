#!/bin/bash

export DEBIAN_FRONTEND=noninteractive
apt update
apt -y install g++ gcc make cmake
rm -rf /var/lib/apt/lists/*

for file in ${IGNIS_HOME}/core/cpp/core/lib/*; do
  if ! [ -e ${IGNIS_HOME}/lib/$file ]; then
    ln -s ${IGNIS_HOME}/core/cpp/core/lib/$file ${IGNIS_HOME}/lib/$file
  fi
done
cp -R  ${IGNIS_HOME}/core/cpp/core/include/* /usr/local/include
