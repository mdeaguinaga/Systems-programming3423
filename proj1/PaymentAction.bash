#!/bin/bash
#****************************** PAYMENT ********************************
#print an error if the file is not found
#***********************************************************************

    read -p "Enter Customer Email: " eMail
    read -p "Enter Payment Ammount: " Amount
    

    if [ ! -f Data/$eMail ]; then 
        #If does not exist, will print an error message
        echo "Error: customer not found"
        exit 1
    fi
    export Amount
    bash PaymentActionPrint.bash < Data/$eMail

