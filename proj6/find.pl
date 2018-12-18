#!/usr/bin/perl -w

my @files = @ARGV;

if($files[0] eq "-i"){
    for ($i = 2; $i <= $#files; $i+=1){
        my $file = "$ARGV[1]";    

        if($files[$i] !~ m/$file/){
            my $printfile = 0;
            open(INPUT,"<",$files[$i])
                or die "could not open INPUT: $!\n";
            while(my $line = <INPUT>){
                if($line =~ m/$file/){
                    $printfile = 1;
                }
            }
            if($printfile == 0){
                my @printfile1 = split(/\//,$files[$i]);
                print "$printfile1[1]\n";
            }
            close(INPUT);
        }
    }
}

else{
    for ($i = 1; $i <= $#files; $i+=1){
        my $file = "$ARGV[0]";
        my @printfile2_1;

        if($files[$i] =~ m/$file/){
            @printfile2_1 = split(/\//,$files[$i]);
            print "$printfile2_1[1]\n";
        }
        else{
            my $printfile2_2 = 0;
            my $printLine;
            open(INPUT,"<",$files[$i])
                or die "could not open INPUT: $!\n";
            while(my $line = <INPUT>){
                if($line =~ m/$file/){
                    $printfile2_2 = 1;
                    $printLine = $line;
                }
            }
            if($printfile2_2 == 1){
                @printfile2_1 = split(/\//,$files[$i]);
                print "$printfile2_1[1]: $printLine";
            }
        }   
        close(INPUT);
    }
}


