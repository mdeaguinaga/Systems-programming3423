#!/bin/bash

#****************************** FIND ***********************************
#
#   Search for the name of the file
#   If is founded, prints the information on the file
#   If is not, prints an error message
#
#***********************************************************************

            read -p "Enter the appartment number: " aptNumber

            #Search for the apartment number
            File=$(grep -l $aptNumber Data/*)

            #Checks if File is Null or not
            if [ ! $File ]; then
                #If is null, it will print an error
                echo "Error: apartment number not found"
                exit 1
            fi
            bash FindActionPrint.bash <$File    #input file



