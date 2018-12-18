#Deletes all the 2017 users
/2017$/d
#Deletes the rest of the information
#Exept the users
s/ .*//
#Adds the dollar sign in front of the users
/$/ s/$/$/p

