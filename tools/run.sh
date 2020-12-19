#!/bin/bash

while true; do
	sleep 1;
	if [ -f /tmp/tod-reload ];
	then
		rm /tmp/tod-reload;
		echo "Got start-signal, starting..."
		./taleofdagon debug test.txt
	elif [ -f /tmp/tod-stop ];
	then
		rm /tmp/tod-stop;
		echo "Got stop-signal, stopping..."
		pkill taleofdagon
	fi
done
