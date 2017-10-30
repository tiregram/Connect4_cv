#!/usr/bin/env bash


for img_path in $(find "$1" -name "*.jpg"); do
    echo "Img : $img_path"
    ./C4cv "$img_path" 2>/dev/null
done
