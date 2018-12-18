#1/bin/bash
#Reads the information
read eMail Name LastName
read aptNumber rentAmt accountBalance DueDate

#Prints the information
NewBalance=$(($accountBalance + $Amount))
echo $eMail $Name $LastName > Data/$eMail
echo $aptNumber $rentAmt $NewBalance $DueDate >> Data/$eMail
echo "Ammount will be added"
