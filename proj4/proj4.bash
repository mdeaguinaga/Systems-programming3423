#!/bin/bash
echo -e "$1" > date1.txt
awk -f date.awk date1.txt > date2.txt


#creates an output divided by name and account
awk -f p4a.awk p4Customer.txt > p4Spaces.txt


File=$(find . -name "Emails")
if [ -z "$File" ]; then
#    echo "doest: $File"
    mkdir Emails
else
#    echo "exist $File "
   rm -r Emails
   mkdir Emails
fi

./p4.bash < p4Spaces.txt

