#!/usr/bin/perl -w
#read input DATE
$date = $ARGV[0];

#creates Email directory

my $dir = "Email";
unlink glob "$dir/*";
unless(-e $dir or mkdir $dir, ) {
   die "Unable to create $dir\n";
}

#opens p5Customer.txt file
open(INFILE1, "<", "p5Customer.txt")
    or die "could not open INFILE1: $!\n";

while(my $infile1 = <INFILE1>){
    
    @words = split("[,]",$infile1);
    
    if($words[3] < $words[4]){
        #split and assigns the full name into other array
        @name = split("[ ]",$words[1]); 
        #opens template.txt
        open(INFILE2, "<", "template.txt")
            or die "could not open INFILE2: $!\n";

        while(my $infile2 = <INFILE2>){
            
            $template = $infile2;
            #substitute the condition with the info in the array 
            $template =~ s/EMAIL/$words[0]/g;
            $template =~ s/FULLNAME/$words[1]/g;
            $template =~ s/NAME/$name[$#name]/g;
            $template =~ s/TITLE/$words[2]/g;
            $template =~ s/AMOUNT/$words[3]/g;
            $template =~ s/DATE/$date/g;

            #output into a file named as the email $words[0].txt;

            open(OUTFILE, ">>", "$dir/$words[0]");
            print OUTFILE $template;
            close(OUTFILE);
        }
        close(INFILE2);
    }
}
close(INFILE1);



