############################################
#卸载镜像仓中的镜像，删除所有的vias镜像
############################################

#! /bin/bash

dir=$(cd $(dirname $0);pwd)
config_file="$dir/../configPod/container.yaml"

#判断pod是否删除完成
function judge_delete_pod
{
    i=0
    while [ 1 == 1 ]
    do
        sleep 10s
        pod_num=$(kubectl get pod | grep vias | wc -l)
        svc_num=$(kubectl get svc | grep vias | wc -l)
        if [ "$pod_num" -eq 0 -a "$svc_num" -eq 0 ]
        then
            return 0
        fi
        
        let i++
        
        if [ "$i" -gt 3 ]
        then
            for pod_name in `kubectl get pod | grep vias | awk '{print $1}'`
            do
                podname=$(echo ${pod_name%-*})
                kubectl delete sts $podname
                if [ $? -ne 0 ]
                then
                    kubectl delete pod $pod_name --force --grace-period=0
                fi

                sleep 10s
                delepod_name=$(kubectl get pod | grep "$pod_name")
                if [ -n "$delepod_name" ]
                then
                    echo "delete $pod_name failed"
                    exit 1
                fi
            done
            
            for svc_name in `kubectl get svc | grep vias | awk '{print $1}'`
            do
                kubectl delete svc $svc_name
                sleep 5s
                delesvc_name=$(kubectl get svc | grep "$svc_name")
                if [ -n "$delesvc_name" ]
                then
                    echo "delete $svc_name failed"
                    exit 1
                fi
            done
        fi
    done
}

judge_delete_pod

#cat $config_file | grep "image" | awk -F: '{print $2":"$3}' > mirror_name.txt
docker images | grep "vias" | awk -F' ' '{print $1":"$2}' > mirror_name.txt

for mirror_name in `cat mirror_name.txt`
do
    docker rmi $mirror_name -f

    if [ $? -ne 0 ]
    then
        echo "uninstall $mirror_name failed"
    fi
done

rm mirror_name.txt

rm -r /var/lib/restful/common/product/aix/*