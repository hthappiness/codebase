####################################################################
#修改xxxpodState.yaml，添加nfs相关配置
####################################################################

#! /bin/bash

function add_hosts_config
{
    yaml_file=$1
    
    line_a=$(cat -n $yaml_file | grep initContainers | awk '{print $1}')
    
    content_a="hostAliases:"
    content_b="- ip: \"192.168.104.152\""
    content_c="hostnames:"
    content_d="- \"private.ca.sensetime.com\""
    content_e="- ip: \"192.168.104.152\""
    content_f="hostnames:"
    content_g="- \"slave.private.ca.sensetime.com\""
    
    sed -i "${line_a}a\        ${content_g}" $yaml_file
    sed -i "${line_a}a\        ${content_f}" $yaml_file
    sed -i "${line_a}a\      ${content_e}" $yaml_file
    sed -i "${line_a}a\        ${content_d}" $yaml_file
    sed -i "${line_a}a\        ${content_c}" $yaml_file
    sed -i "${line_a}a\      ${content_b}" $yaml_file
    sed -i "${line_a}a\      ${content_a}" $yaml_file	
    
    
}

cd /var/lib/restful/common/product/aix/yaml/

add_hosts_config vias-fps-30001-podStatefulSet.yaml 
add_hosts_config vias-ffc-30001-podStatefulSet.yaml

