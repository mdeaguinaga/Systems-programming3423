#!/usr/bin/perl -w
use File::Copy;
my @files = glob("$ARGV[0]/*");
my @fileArray;
my @fileDir;

for ($i = 0; $i <= $#files; $i+=1){
    
    my @match = split(/\//,$files[$i]);
    my $dirName;

    if(($dirName)=$match[1] =~ m/proj(\w*)\./){    
        $dirName = "assignment$dirName";
        unlink glob "$dirName/*";
        unless(-e "$match[0]/$dirName" or mkdir "$match[0]/$dirName") {
            die "Unable to create $dirName\n";
        }
        move($files[$i], "$match[0]/$dirName") or die "The move operation failed: $!";
        push(@fileArray, $dirName);
        push(@fileDir, $match[1]);
    }
    else{
        $dirName = "misc";
        unlink glob "$dirName/*";
        unless(-e "$match[0]/$dirName" or mkdir "$match[0]/$dirName") {
            die "Unable to create $dirName\n";
        }
        move($files[$i], "$match[0]/$dirName") or die "The move operation failed: $!";
        push(@fileArray, $dirName);
        push(@fileDir, $match[1]);
    }
}


#prints the files that were created
#@fileArray =  (sort(@fileArray));
#for($i = 0; $i <= $#fileArray; $i+=1){
#    if("$fileArray[$i-1]" ne "$fileArray[$i]"){
#        print"$fileArray[$i]/\n";
        



