#Reads word by word and print them starting from the last
{
    for(i = NF; i != 0; i--){
        printf("%s ",$i)
    }
    printf("\n")
}
