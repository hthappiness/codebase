###################################################
#一键安装的时候，修改container.yaml中的license IP
###################################################

#! /bin/bash

dir=$(cd $(dirname $0);pwd)
config_file="$dir/../configPod/container.yaml"

while [ 1 == 1 ]
do
    let i=0
    read -p "please input license IP: " license_ip
    num=$(echo $license_ip | awk -F'.' '{print NF}')
    if [ "$num" -ne 4 ]
    then
        continue
    fi
    a=$(echo $license_ip | awk -F'.' '{print $1}')
    b=$(echo $license_ip | awk -F'.' '{print $2}')
    c=$(echo $license_ip | awk -F'.' '{print $3}')
    d=$(echo $license_ip | awk -F'.' '{print $4}')
    for loop in $a $b $c $d
    do
        if [ "$loop" -ge 255 -o "$loop" -lt 0 ]
        then
            break
        fi
        let i++
    done
    if [ $i -eq 4 ]
    then
        break 
    fi
done

sed -i "s/AIX_LICENSE_ADDR:.*/AIX_LICENSE_ADDR: "$license_ip"/g" $config_file