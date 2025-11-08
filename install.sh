#/usr/bin/env bash

cmake -Bbuild -GNinja
cmake --build build
cmake --install build
