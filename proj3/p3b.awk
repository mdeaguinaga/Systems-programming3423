#Prints the header
BEGIN{printf"%-15s %6s %10s %13s\n", "Name", "Flight", "Seats", "Total Cost";}
{
    #Condition separator
    #CUST set total to 0 and saves first and last name
    if($1 == "CUST"){
        TotalCost = 0
        nameLast = $6
    }
    #RESERVE adds total, adds total cost, prints info and creates array
    if($1 == "RESERVE"){
        cost = $3 * $4
        TotalCost += cost
        printf("%-15s %5s %6d %13.2f \n",nameLast ,$2 ,$3 ,cost );
        FlyTotal[$2] += $3;
    }
    #ENDCUST Prints information of Total cost
    if($1 == "ENDCUST"){
        printf("%34s %12.2f \n", "Total ", TotalCost); 
    }
}
END{ 
    #prints array with the total os seats
    printf"%-10s %-12s\n","Flight", "Total Seats";
    for(key in FlyTotal)
        printf"%-15s %3s\n", key, FlyTotal[key];        
    }

