#!/bin/bash
input=0
while [ $input ]; do
    if ! read Line; then
        break
    fi
    read Line1
    read Line2
    echo -e "$Line1 \n$Line2" > Line.txt
    
    cat Line.txt date2.txt > Line1.txt    

    awk -f p4b.awk Line1.txt > gp4.sed

    sed -f gp4.sed template.txt > ./Emails/"$Line"
done
    

