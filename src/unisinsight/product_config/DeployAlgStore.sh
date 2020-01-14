#####################################
# 入参: 模块-算法(vs-40201)
#./DeployAlgStore.sh vs-40201 1
#####################################

#!/bin/bash

# 定义环境变量
export LANG="en_US.UTF-8"

#输入参数个数检查
if [ $# -le 1 ]; then
    echo  "脚本参数错误"
    echo -e "Usage: ./DeployAlgStore.sh <module-algorithm:vs-40201> <service_type:1/2/3/4> <descriptions>\nThe arguments must be in order."
    exit 1
fi

module_alg=$1
module=$(echo $module_alg | awk -F'-' '{print $1}')
algorithm=$(echo $module_alg | awk -F'-' '{print $2}')

#判断环境是否安装对应服务
pod_name_num=$(kubectl get pod | grep $module_alg | awk '{print $1}')
pod_name=$(echo $pod_name_num | awk '{sub(/.{2}$/,"")}1')
image_name=$(docker images | grep -v "registry" | grep $module | grep $algorithm | awk '{print $1":"$2}')
image_regis_name=$(docker images | grep "registry" | grep $module | grep $algorithm | awk '{print $1":"$2}')
if [ -n "$pod_name" ]
then
    kubectl delete sts "$pod_name"
    docker rmi "$image_name" -f
    docker rmi "$image_regis_name" -f
fi


#查找对应的image安装包和yaml文件
yaml_file=$(find . -type f -name "*.yaml" | grep "$module_alg")
image_file=$(find . -type f -name "*.tar.gz" | grep "$module_alg")

#解压镜像文件
image_name=$(docker load -i "$image_file")
version=$(echo $image_name | awk '{print $3}')
docker tag "$version" registry:5500/"$version"
docker push registry:5500/"$version"

#替换yaml中的image值
sed -i "s#image: .*#image: registry:5500/"$version"#g" "$yaml_file"

#安装服务
kubectl create -f "$yaml_file"


#添加数据库记录
docker images | grep $module | grep $algorithm > a.log
a1=$(sed -n "1,1p" a.log |awk '{print $1}')
a2=$(sed -n "1,1p" a.log |awk '{print $2}')
a3=$(sed -n "1,1p" a.log |awk '{print $7}')
a4=$(sed -n "1,1p" a.log |awk '{print $3}')

#算法厂家
a5=${algorithm:0:1}
#业务类型
a6=${algorithm:2:1}
#算法版本号
a7=${algorithm:3:2}
#服务类型 1-图片流 2-视频流 3-特征比对 4-库管理
service_type=$2
#描述
descriptions=$3

#添加数据库记录
    TABLENAME="tb_image_repository"
    /usr/local/pgsql/bin/psql -h localhost -p 5432 -U postgres -d vias << EOF
    insert into ${TABLENAME}(image_id, product_type, vendor, sdk_version, name, create_time, size, service_type, descriptions) 
                     values('$algorithm', '$a6', '$a5', '$a7', '$a1', current_timestamp,'$a3', ${service_type}, '${descriptions}')
EOF






