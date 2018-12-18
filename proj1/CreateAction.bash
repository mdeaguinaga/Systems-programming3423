#!/bin/bash


#**************************** CREATE **********************************
#Creates New apartment accout if it does not exist
#**********************************************************************

           read -p "Enter the Email: " eMail 
           read -p "Enter the full name: " fullName
           read -p "Enter the Apartment Number: " aptNumber
           read -p "Enter Monthly Rent Amt: " rentAmt
           read -p "Enter Next Due Date: " nextDueDate

            #Default the account balance to zero
            accountBalance="0"

            #Check if the account exist.
            File=$(find . -iname "$eMail") #Checks for existing account

            if [ -z "$File" ]; then #-z check if variable is NULL
                
                #If does not exist, it create a new account using 
                #customer's email as file name
                echo -e "$eMail $fullName \n$aptNumber $rentAmt $accountBalance $nextDueDate" > ./Data/"$eMail"
                echo "Message: new account has been added"
            #If the file exists, show an error message (and do nothing)
            else echo "Customer already exist"
            fi             

