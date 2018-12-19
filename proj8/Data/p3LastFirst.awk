{
    printf("%s ", $NF);
    for (i= 1; i <= NF; i++)
        printf("%s ", $i);
    printf("\n");
}
