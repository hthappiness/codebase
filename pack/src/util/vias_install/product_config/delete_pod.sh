#选择是否删除fps/fvs/vr/vs pod服务

#! /bin/bash

function delete_pod
{
    service_name=$1
    alg_name=$2
    echo -e "whether to delete "$service_name"-"$alg_name"-pod : y/n \c"
    read answer
    if [ "$answer" == "y" -o "$answer" == "Y" ]
    then
        kubectl delete sts vias-"$service_name"-"$alg_name"-pod
        if [ $? -eq 0 ]
        then
            return 0
        else
            kubectl delete pod vias-"$service_name"-"$alg_name"-pod-0 --force --grace-period=0
            if [ $? -eq 0 ]
            then
                return 0
            fi
        fi
    elif [ "$answer" == "n" -o "$answer" == "N" ]
    then
        return 0
    fi
    echo "delete fps-pod failed"
}

delete_pod fps 10001
delete_pod fps 50001
delete_pod fvs 10001
delete_pod fvs 50001
delete_pod vs 40201
delete_pod vs 50201
delete_pod vr 40101