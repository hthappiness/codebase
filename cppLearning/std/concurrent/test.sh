#!/bin/bash

i=0
while [ $i -le 100000 ]
do
    ./morder4
    #echo " loop ", $i
    let i++
    sleep 0.1
done


 
