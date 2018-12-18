#!/bin/bash

# Axk for an input and base on it, does an acction

input=0
while [ $input ]; do

    echo "Enter one of the following actions or press CTRL-D to exit:"
    echo "C - create a customer file"
    echo "P - accept a customer payment"
    echo "F - find customer by Appartment " 

    if ! read sAction; then
        break
    fi

    case "$sAction" in

        "C")    #Creates an accout
            ./CreateAction.bash
            ;;

        "P")    #Payment to account
            ./PaymentAction.bash
            ;;

        "F")    #Find information
            ./FindAction.bash
            ;;
        *)      #Prints an error

        echo "Error: invalid action value"
        exit 1;;
    esac
done

