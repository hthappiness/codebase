####################################################################
#修改xxxpodState.yaml，添加nfs相关配置
####################################################################

#! /bin/bash

function add_nfs_config
{
    yaml_file=$1
    server_ip=$2
    
    line_a=$(cat -n $yaml_file | grep volumeMounts | awk '{print $1}')
    
    content_a="- name: storagepath"
    content_b="mountPath: /mnt/media"
    content_c="nfs:"
    content_d="path: /NAS/CAPFS/data/nfs/vids"
    content_e="server: $server_ip"
    
    sed -i "${line_a}a\          ${content_b}" $yaml_file
    sed -i "${line_a}a\        ${content_a}" $yaml_file
    
    
    line_b=$(cat -n $yaml_file | grep volumes | awk '{print $1}')
    
    sed -i "${line_b}a\          ${content_e}" $yaml_file
    sed -i "${line_b}a\          ${content_d}" $yaml_file
    sed -i "${line_b}a\        ${content_c}" $yaml_file
    sed -i "${line_b}a\      ${content_a}" $yaml_file
}

cd /var/lib/restful/common/product/aix/yaml/

add_nfs_config vias-fvs-10001-podStatefulSet.yaml 192.168.108.155
add_nfs_config vias-fvs-50001-podStatefulSet.yaml 192.168.108.155
add_nfs_config vias-vs-40201-podStatefulSet.yaml 192.168.108.155
add_nfs_config vias-vs-50201-podStatefulSet.yaml 192.168.108.155

