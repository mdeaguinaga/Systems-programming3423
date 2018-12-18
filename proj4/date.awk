BEGIN{FS="/"}
{
    date = $1"\\/"$2"\\/"$3;
    printf("DATE %s\n",date);
}
