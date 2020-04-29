#./yourProgram < FIFO_1.txt > FIFO_1_stdout.txt
#dmesg | grep Project1 > FIFO_1_dmesg.txt

#!/bin/bash

# sudo ./scheduler < $1 > output2.txt
# ./calc output2.txt < $1
for t in RR SJF PSJF FIFO ; do
    for i in {1..5} ; do

        echo "----------------------"$t"_$i----------------------------"
        sudo dmesg --clear && clear
        sudo ./scheduler < "test/"$t"_$i.txt" > "output/"$t"_$i""_stdout.txt"
        dmesg | grep Project1 > "output/"$t"_$i""_dmesg.txt"
    done
done