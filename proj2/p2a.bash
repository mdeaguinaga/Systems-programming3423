#!/bin/bash
#Outputs just the user ID with the dollar at the end of user
sed -n -f p2aDollar.sed lastlog1.out > test1.out
#Outputs just the user ID
sed -n -f p2a.sed lastlog2.out > test2.out
#Outputs the information analized 
grep -f test1.out test2.out > p2a.out


