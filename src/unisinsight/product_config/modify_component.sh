###################################################
#一键安装的时候，修改component.json中的值
#将vias模块设为true，其他模块设为false
###################################################

#! /bin/bash

file_dir="/var/lib/restful/common/src"
modify_file="$file_dir/component.json"

cp $modify_file $file_dir/component_copy.json

let i=1
while read -r line
do
    str_true=$(echo $line | grep "true")
    str_false=$(echo $line | grep "false")
    str_AIX=$(echo $line | grep "isNeedaix")
    if [ -n "$str_true" -o -n "$str_false" ] 
    then
        if [ -n "$str_AIX" ] 
        then
            sed -i ""$i"s/: [a-z]*/: true/" "$modify_file"
        else
            sed -i ""$i"s/: [a-z]*/: false/" "$modify_file"
        fi
    fi

    let i++
done < "$file_dir/component_copy.json"

rm $file_dir/component_copy.json
