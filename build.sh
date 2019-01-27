#!/bin/bash

mkdir build
pushd build
cmake -G "Unix Makefiles" ../
make -j8
popd
