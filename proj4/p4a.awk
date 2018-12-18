BEGIN { FS = ",";}
{
    paid = (NF-1); 
    EMAIL = $1;
    FULLNAME = $2;
    TITLE = $3;
    AMOUNT = NF

    if($AMOUNT > $paid){

        #printf("%s %s %s %d\n" ,FULLNAME ,EMAIL ,TITLE ,$AMOUNT);
        printf("%s\nNAME %s \nACCOUNT %s %s %d\n",EMAIL ,FULLNAME ,EMAIL ,TITLE ,$AMOUNT);
    }
}


