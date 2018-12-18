#/bin.bash

sed -n -f p2a.sed lastlog1.out > result1.out
sed -n -f p2a.sed lastlog2.out > result2.out

cat result1.out result2.out | sort | uniq -c > Count.out

sed -f p2b.sed Count.out > p2b.out
