#!/bin/bash

width='50'
height='50'
unitsTotal='10'
filename='test.map'

#echo -n "$width$height$unitsTotal" | xxd -p -r | xxd

printf "%02hx00%02hx00%02hx00" $width $height $unitsTotal | xxd -p -r > $filename;

for i in `seq 1 $width`; do
	for j in `seq 1 $height`; do
		tileType='2'
		areaNumber='1'
		objectNumber='0'

		printf "%02hx%02hx%02hx" $tileType $areaNumber $objectNumber | xxd -p -r >> $filename
	done
done


for i in `seq 1 $unitsTotal`; do
	unitType='1'
	x='0'
	y='0'
	printf "%02hx%04hx%04hx" $unitType $x $y | xxd -p -r >> $filename
done

cat $filename | xxd

