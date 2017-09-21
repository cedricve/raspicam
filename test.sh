#!/bin/bash
# Run this script to test the library's functionality of various formats and sizes
OPTS=""

func () {
	f=$1_$2x$3
	mkdir -p "$f"; cd "$f"
	../../build/utils/raspicam_test $OPTS -$1 -w $2 -h $3
	cd ..
}

func2 () {
	func $1 3280 2464
	func $1 1640 1232
	func $1 1280 720
}


mkdir -p out; cd out

func2 rgb
func2 yuv
func2 gr

cd ..
