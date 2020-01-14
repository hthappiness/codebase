###################################################
#一键安装的时候，修改container.yaml中对应的版本号
###################################################

#! /bin/bash

internal_version=$1

#version="V1.0.4.0"
#internal_version="VIASV100R001B01D004"

dir=$(cd $(dirname $0);pwd)
config_file="$dir/../configPod/container.yaml"

cp $config_file $dir/container_copy.yaml
dos2unix $config_file
dos2unix $dir/container_copy.yaml

let i=1
alg="00000"
while read -r line
do
    
    modify_line=$(echo $line | grep "image: ")
    if [ -n "$modify_line" ] 
    then
        sed -i ""$i"s/[A-Z].*/"$internal_version"-"$alg"/" "$config_file"
    fi
    
    alg_line=$(echo $line | grep "name")
    if [ -n "$alg_line" ] 
    then
        alg=$(echo $line | awk -F- '{print $3}')
        if [ -z "$alg" ]
        then
            alg="00000"
        fi
    else
        alg="00000"
    fi

    let i++
done < "$dir/container_copy.yaml"

rm $dir/container_copy.yaml
