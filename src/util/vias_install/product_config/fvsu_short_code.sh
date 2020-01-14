########################################################
#一键安装的时候，修改container.yaml中对应的长短码信息
########################################################

#! /bin/bash

dir=$(cd $(dirname $0);pwd)
config_file="$dir/../configPod/container.yaml"

while [ 1 == 1 ]
do
    echo "Select general or fast retrieval mode:"
    echo "0 : general mode"
    echo "1 : fast retrieval mode"
    echo -e "the AIX_FVSU_SHORT_CODE is : \c"
    read answer
    if [ "$answer" == 1 -o "$answer" == 0 ]
    then
        sed -i "s/AIX_FVSU_SHORT_CODE: .*/AIX_FVSU_SHORT_CODE: "$answer"/g" $config_file
        sed -i "s/AIX_FPSU_SHORT_CODE: .*/AIX_FPSU_SHORT_CODE: "$answer"/g" $config_file
        exit 0
    fi
    echo -e "the general or fast retrieval mode is 0 or 1 \n"
done
