#!/bin/bash
#Reads the information
read eMailread eMail Name LastName
read aptNumber rentAmt Amount DueDate

#Prints the information
echo "Email:    $eMail" 
echo "Name:     $Name $LastName"
echo "Apt:      $aptNumber"
echo "Balance:  $rentAmt"
echo "Rent Amt: $Amount"
echo "Due Date: $DueDate" 
