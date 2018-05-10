#!/bin/bash
cd "$(dirname "$0")"

if [ $# -ne 1 ] || [ ! -f "$1"/ignis/rpc/*.thrift ]; 
    then echo "usage thrift.sh <rpc-folder>"
    exit
fi

rm -fr ignis/rpc
out=`realpath $(dirname "$0")`
rpc=`realpath "$1"`
for file in `cd $1 && find ignis -name "*thrift"`; do
    cd `dirname $rpc/$file` &&
    mkdir -p $out/`dirname $file` && 
    thrift --gen cpp:no_skeleton,include_prefix -out $out/`dirname $file` `basename $file` &
done
wait
rm -fr ignis/rpc/manager
