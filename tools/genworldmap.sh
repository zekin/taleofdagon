#!/bin/bash
__iferror_logandexit () {
    if (( $1 != 0 ))
    then
        echo $2
        exit 1
    fi
}

tod_filename="WorldMap_Uncompressed.bmp"
filename="ui_world_map.png"
filepath="graphics/ui/$filename"

echo "Running taleofdagon with map parameter"
./taleofdagon map;
__iferror_logandexit $? "Failed to find taleofdagon or failed to generate map"

echo "Running imagemagick conversion"

convert $tod_filename PNG8:$filename
__iferror_logandexit $? "Failed to convert - do you have imagemagick and convert in your PATH? - aborting."

mv $filename $filepath
__iferror_logandexit $? "Failed to move generated file to $filepath"

exit 0

