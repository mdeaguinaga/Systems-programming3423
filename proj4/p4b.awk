{
    if($1 == "NAME"){
        i = NF
        NAME = $i;
        FULLNAME = $2" "$3" "$4;
    }
    else if($1 == "ACCOUNT"){
        EMAIL = $2;
        TITLE = $3;
        AMOUNT = $4;
    }
    else if($1 == "DATE"){        
        DATE = $2;
    #here you going to print the sed file with .sed extention
    printf("s/FULLNAME/%s/\ns/EMAIL/%s/\ns/TITLE/%s/\ns/NAME/%s/\ns/AMOUNT/%0.2f/\ns/DATE/%s/\n",FULLNAME, EMAIL, TITLE, NAME, AMOUNT, DATE);
    }

}
