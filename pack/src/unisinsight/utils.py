# encoding=utf-8
import os
import json
import commonlogger as logger
import socket
import HttpUtil

import commonUtils

dir = os.path.split(os.path.realpath(__file__))[0]
path = dir + "/conf/config.json"


def get_host_ip():
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.connect(('8.8.8.8', 80))
        ip = s.getsockname()[0]
    finally:
        s.close()
    return ip


def get_config(path):
    """
    读取配置文件
    :param path:配置文件所在路径
    :return: 读取到的配置文件内容，出错则返回False
    """
    try:
        with open(path, "r") as file:
            data = json.load(file)
            if data:
                return data
            else:
                logger.error("Get config failed!")
                return False
    except:
        logger.error("Illegal configuration file!")
        return False


def check_conf(data):
    """
    检查配置文件
    :param data:读取的配置文件
    :param action:install 或 scale
    :return:检查结果
    """
    data1 = commonUtils.get_json(data, "CONFIG_CLUSTERIP")
    data2 = commonUtils.get_json(data, "CONFIG_POSTGRES_USER")
    data3 = commonUtils.get_json(data, "CONFIG_POSTGRES_PASSWORD")
    data4 = commonUtils.get_json(data, "CONFIG_POSTGRES_DB")

    if data1 and data2 and data3 and data4:
        return True
    else:
        return False


def httpOrder(scripts):
    """
    访问k8s的APIServer
    :return: 执行结果
    """
    res = HttpUtil.get(scripts)
    return json.loads(res.text)


def getFQDN():
    """
    获取zookeeper集群的正式域名
    :return:
    """
    # 获取zookeeper集群总数
    script = "/root/local/bin/kubectl get statefulset zookeeper -o wide | awk 'NR>1{print $3}'"
    current = commonUtils.order(script, "read")
    if int(current) > 0:
        name = []
        for i in range(0, int(current)):
            script = "/root/local/bin/kubectl exec zookeeper-" + str(i) + " -- hostname -f"
            fqdn = commonUtils.order(script, "read")[:-2]
            name.append(fqdn)
        result = ""
        for i in name:
            result = result + (i + ":2181,")
        return result[:-1]
    else:
        logger.error("No available zookeeper!")
        return False


def createYaml(data):
    """
    生成yaml文件
    1.若原有配置文件存在则删除
    2.根据模板和需要替换的参数来生成yaml文件
    :param data:读取的配置文件内容
    :return:
    """
    if os.path.exists(dir + "/yaml/st-pgsql.yaml"):
        os.system("rm -f " + dir + "/yaml/st-pgsql.yaml")
    if os.path.exists(dir + "/yaml/se-pgsql.yaml"):
        os.system("rm -f " + dir + "/yaml/se-pgsql.yaml")

    pgsql_yaml = []
    with open(dir + "/yaml/template-st-pgsql.yml", "r") as template:
        for line in template:
            for key in data:
                if key in line:
                    line = line.replace(key, data[key])
            pgsql_yaml.append(line)
    s = ''.join(pgsql_yaml)
    with open(dir + "/yaml/st-pgsql.yml", "w+") as pgsql:
        pgsql.write(s)

    pgsql_yaml = []
    with open(dir + "/yaml/template-se-pgsql.yml", "r") as template:
        for line in template:
            for key in data:
                if key in line:
                    line = line.replace(key, data[key])
            pgsql_yaml.append(line)
    s = ''.join(pgsql_yaml)
    with open(dir + "/yaml/se-pgsql.yml", "w+") as pgsql:
        pgsql.write(s)



def isExist():
    """
    判断集群是否已存在，用于安装前检查
    :return:检查结果,true代表不存在，false代表存在
    """
    # 检查service
    scripts = "/root/local/bin/kubectl get service | grep svc-pgsql | awk '{print $1}'"
    services = commonUtils.order(scripts, "readlines")
    if services:
        for service in services:
            logger.error("service:" + service.split("\n")[0] + " is exist!")
        return True
    # 检查statefulset

    scripts = "/root/local/bin/kubectl get statefulset | grep st-pgsql | awk '{print $1}'"
    sta = commonUtils.order(scripts, "readlines")
    if sta:
        for s in sta:
            logger.error("statefulset:" + s.split("\n")[0] + "is exist!")
        return True
    return False


def label():
    """
    为指定ip打上标签
    打标签之前先检查集群所有节点，防止指定ip地址外有标签的存在
    :return:
    """
    nodes = commonUtils.get_node()
    if len(nodes) > 1:
        logger.error("Illegal node address")
    else:
        scripts = "/root/local/bin/kubectl get node --show-labels | grep pgsql=pgsql | awk '{print $1}'"
        node = commonUtils.order(scripts, "readlines")
        if node:
            logger.info("Node had labled")
        else:
            for node in nodes:
                node = node.split("\n")[0]
                os.system('/root/local/bin/kubectl label node {} pgsql=pgsql --overwrite=true 2>&1\n'.format(node))

# if __name__ == '__main__':
#     data = get_config(path)
#     print data
#     createYaml(data)
