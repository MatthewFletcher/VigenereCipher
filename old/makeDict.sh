#!/bin/bash

DICT=~/Documents/FA20/CS485/Viginere
MYDICT=/tmp/myDict.txt

sed -e 's/$/,/g' $DICT  > $MYDICT

truncate -s-1 $MYDICT
