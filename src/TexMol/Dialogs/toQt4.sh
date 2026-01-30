#!/bin/bash


for f in *.Qt3.ui
do  
    echo $f
    f1=`basename $f .Qt3.ui`
    uic3 -convert $f >${f1}.Qt4.ui
done