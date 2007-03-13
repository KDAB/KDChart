#!/bin/bash

cd unpacked

echo unpacking $1
unzip -o ../$1

find -name '*xml' > 0.tmp

while read curline; do

    echo writing reformatted/"$curline"
    xmllint --format "$curline" > ../reformatted/"$curline"

done < 0.tmp

rm 0.tmp
