#!/bin/bash

export DEBIAN_FRONTEND=noninteractive
apt update
apt -y install g++ gcc make cmake
rm -rf /var/lib/apt/lists/*

cd ${IGNIS_HOME}/core/cpp/core/lib/
for file in *; do
  if ! [ -e ${IGNIS_HOME}/lib/$file ]; then
    ln -s ${IGNIS_HOME}/core/cpp/core/lib/$file ${IGNIS_HOME}/lib/$file
  fi
    if ! [ -e /usr/local/lib/$file ]; then
      ln -s ${IGNIS_HOME}/core/cpp/core/lib/$file /usr/local/lib/$file
    fi
done

cp -R  ${IGNIS_HOME}/core/cpp/core/include/* /usr/local/include
cp -R  ${IGNIS_HOME}/core/cpp/core/include/* /opt/ignis/include
ldconfig
