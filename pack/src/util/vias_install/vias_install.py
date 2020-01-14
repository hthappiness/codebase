#! /usr/bin/python
# -*- coding: UTF-8 -*-

import sys
sys.path.append("/var/lib/restful/common/src")

import os
import src
import src.utils
import time
import installApp
from subprocess import Popen, PIPE

dir = os.path.split(os.path.realpath(__file__))[0]
data = src.utils.get_config(dir + "/conf/config.json")
iaas_file = "/var/lib/restful/common/src/installApp.py"

#print输出到日志文档中
output = sys.stdout
outputfile = open("{}/viasinstall.log".format(dir),'w')
sys.stdout = outputfile
print("sys.path is : {}".format(sys.path))
print("dir is : {}".format(dir))
print("data is : {}".format(data))
outputfile.close()
sys.stdout = output

log_file = dir + "/viasinstall.log"

def create_initfile():
    """
    创建pgsql的初始化脚本文件
    """
    init_sh_file = dir + "/psql/template_init.sh"
    new_init_sh_file = dir + "/psql/init.sh"
    
    with open(init_sh_file, "r") as f:
        newfile = []
        for oneline in f:
            for key in data:
                if key in oneline:
                    oneline =  oneline.replace(key, data[key])
            newfile.append(oneline)
    s = ''.join(newfile)
    with open(new_init_sh_file, "w+") as newinit:
        newinit.write(s) 

def shell_box(cmd):
    print("cmd:{}".format(cmd))
    p = Popen(cmd, stdout=PIPE, shell=True)
    p.wait()
    if p.returncode:
        return -1, cmd
    retlines = p.stdout.readlines()
    return 0, retlines

def  get_pgsql_podname():
    """
    获取一个pgsql的pod，并且返回pgsql的名字
    如果获取失败，就返回一个非0值，如果获取成功就返回0值
    """
    cmd = "/root/local/bin/kubectl get pod |grep st-pgsql|awk '{print $1}'"
    code, ret = shell_box(cmd)
    if code:
        return code, ret
    if len(ret) == 0:
        return -1, "cannot get pgsql pod"
    onePod = ret[0][:-1]
    if onePod not in ("st-pgsql-0", "st-pgsql-1", "st-pgsql-2"):
        return -1, "the pgsql pod is not right"
    return 0, onePod

def mkdir_in_pod(podname, workpath):
    """
    在pod的中创建一个目录文件
    """
    shline = " -- sh -c \"test -d {0} || mkdir {0} \"".format(workpath)
    cmd = "/root/local/bin/kubectl exec " + podname + shline
    return shell_box(cmd)


def copy_file_to_pod(podname, workpath):
    """
    将初始化文件拷贝到pod中
    """
    src = dir + "/psql/"
    dst = podname + ":" + workpath
    cmd = "/root/local/bin/kubectl cp " + src + " " + dst
    return shell_box(cmd)

def copy_file_to_kafka_pod(podname, workpath):
    """
    将初始化文件拷贝到pod中
    """
    src = dir + "/kafka/"
    dst = podname + ":" + workpath
    cmd = "/root/local/bin/kubectl cp " + src + " " + dst
    return shell_box(cmd)

def exec_in_pod(podname, workpath):
    """
    在容器内部执行初始化脚本
    """
    operation = " -- sh -c \"cd {0}/psql && chmod +x {0}/psql/init.sh && {0}/psql/init.sh load\" >> {1} 2>&1".format(workpath,log_file)
    cmd = "/root/local/bin/kubectl exec " + podname + operation
    return shell_box(cmd)

def exec_in_kafka_pod(podname, workpath):
    """
    在容器内部执行初始化脚本
    """
    operation = " -- sh -c \"cd {0}/kafka && chmod +x {0}/kafka/init.sh && {0}/kafka/init.sh load\" >> {1} 2>&1 ".format(workpath,log_file)
    cmd = "/root/local/bin/kubectl exec " + podname + operation
    return shell_box(cmd)

def pgsql_init():
    """
    将初始化文件拷贝到pgsql的pod中，然后执行初始化脚本
    """
    try:
        print("begin init pgsql...")
        workpath = "/vias_init.first"
        create_initfile()

        code, podname = get_pgsql_podname()
        if code:
            print("get pgsql name error!!!")
            return False

        code, ret = mkdir_in_pod(podname, workpath)
        if code:
            print("make directory in the pod {0} error, the path is{1}".format(podname, workpath))
            print("the detail error info is: {}".format(ret))
            return False
        code, ret = copy_file_to_pod(podname, workpath)
        if code:
            print("copy the init file to pgsql error")
            print("the detail error info is: {}".format(ret))
            return False
        code, ret = exec_in_pod(podname, workpath)
        if code:
            print("exec the init operation error")
            print("the detail error info is: {}".format(ret))
            return False
        print("init pgsql success.")
        return True
    except:
        print("init pgsql error...")
        return False

def  get_kafka_podname():
    """
    获取一个kafka的pod，并且返回kafka的名字
    如果获取失败，就返回一个非0值，如果获取成功就返回0值
    """
    cmd = "/root/local/bin/kubectl get pod |grep \"^kafka-\"|awk '{print $1}'"
    code, ret = shell_box(cmd)
    if code:
        return code, ret
    if len(ret) == 0:
        return -1, "cannot get kafka pod"
    onePod = ret[0][:-1]
    if onePod not in ("kafka-0", "kafka-1", "kafka-2",
                      "kafka-3", "kafka-4", "kafka-5"):
        return -1, "the kafka pod is not right"
    return 0, onePod

def kafka_init():
    """
    将初始化文件拷贝到kafka的pod中，然后执行初始化脚本
    """
    try:
        print("begin init kafka...")
        workpath = "/vias_init.first"
        code, podname = get_kafka_podname()
        if code:
            print("get kafka name error!!!")
            return False

        code, ret = mkdir_in_pod(podname, workpath)
        if code:
            print("make directory in the pod {0} error, the path is{1}".format(podname, workpath))
            print("the detail error info is: {}".format(ret))
            return False
        code, ret = copy_file_to_kafka_pod(podname, workpath)
        if code:
            print("copy the init file to kafka error")
            print("the detail error info is: {}".format(ret))
            return False
        code, ret = exec_in_kafka_pod(podname, workpath)
        if code:
            print("exec the init operation error")
            print("the detail error info is: {}".format(ret))
            return False
        print("init kafka success...")
        return True
    except:
        print("init kafka error...")
        return False

def write_sql():
    """
    将信息写入数据库表tb_image_repository
    """
    os.popen("docker images |grep -v registry |grep vias >a.log").read()
    for line in open("a.log"):
        wf = open('b.log', 'w')
        wf.write(line)
        wf.close()

        a1=os.popen("cat b.log | awk '{print $1}'").read()
        a2=os.popen("cat b.log | awk -F'-' '{print $3}'").read()
        a3=os.popen("cat b.log | awk '{print $7}'").read()
        a4=os.popen("cat b.log | awk '{print $3}'").read()
        a5=a2[0:5]
        a6=a5[0:1]
        a7=a5[1:3]
        a8=a5[3:5]
        a9=a1[0:len(a1)-1]
        if a6=='0':
            continue
        lServiceType=1
        if a9=='vias-fps':
            lServiceType=0
        if a9=='vias-vr':
            lServiceType=0
        if a9=='vias-ps':
            lServiceType=0
        a10=a7[1:]
        strSql = 'insert into tb_image_repository(image_id, product_type, vendor, sdk_version, name, create_time, size, service_type, descriptions) values'
        strSqlParam = "('%s','%s','%s','%s','%s',%s,'%s',%d,'%s');" % (a5,a10,a6,a8,a9,'current_timestamp(0) at time zone \'-8:00\'',a3,lServiceType,'algo')
        strSql=strSql+strSqlParam
        print strSql
        file=r'./psql/initfiles/vias_db_create.sql'
        with open(file,'a+') as f:
            f.write(strSql)
    os.remove("b.log")
    res = pgsql_init()
    return res

def func_print(func):
    """
    调用函数，print输出到日志文档中，不打印到屏幕
    """
    with open(log_file,'a') as f:
        old_out = sys.stdout
        sys.stdout = f
        res = func()
        sys.stdout = old_out
        return res

def func_perform(res,info):
    """
    判断函数是否执行成功，若不成功则退出
    """
    if res == False:
        print("{} failed, exit.".format(info))
        exit(1)
    elif res == True:
        print("{} successfully".format(info))
    else:
        print("{} no log".format(info))

def shell_perform(res,info):
    """
    shell命令调用执行结果，如果失败则退出
    """
    if res == 0:
        print("{} successfully".format(info))
    else:
        print("{} failed,exit.".format(info))
        exit(1)

if __name__ == "__main__":
    print("### start install vias service ###")
    #kafka判断环境是集群还是单机
    partions = 1
    num=int(os.popen("kubectl get nodes | wc -l").read())
    if num > 2:
        partions = 3
    shPath = dir + "/kafka/initfiles/VIAStopic.sh"
    strCmd = "sed -i 's/COPY_NUM/" + str(partions) + "/g' " + shPath
    res = os.system(strCmd)

    #初始化kafka信息
    print("start initializing kafka...")
    res = func_print(kafka_init)
    func_perform(res,"initialize kafka")

    #修改yaml中的LICENSE信息
    res = os.system("sh {}/product_config/modify_license.sh".format(dir))
    shell_perform(res,"modify license_ip")

    #生成yaml目录，将安装包里的yaml和images拷贝到aix中
    yaml_dir = os.path.exists("/var/lib/restful/common/product/aix/yaml")
    if yaml_dir == False:
        os.mkdir("/var/lib/restful/common/product/aix/yaml")
    os.popen("cp -r {}/configPod /var/lib/restful/common/product/aix".format(dir)).read()
    os.popen("cp -r {}/images /var/lib/restful/common/product/aix".format(dir)).read()

    #判断环境中是否存在vias服务的pod
    podOut = os.popen("kubectl get pod").read()
    if 'vias' in podOut:
        print("delete vias pod.")
        res = os.system("python {0} uninstall aix >> {1} 2>&1".format(iaas_file,log_file))
        shell_perform(res,"delete pod")

    #初始化pgsql信息
    print("start initializing pgsql...")
    res = func_print(pgsql_init)
    func_perform(res,"initialize pgsql")

    #生成images，并将镜像导入镜像仓
    print("start generating images, and push images to the repository...")
    print("please wait about 20 minutes...")
    res = os.system("python {0} loadimage aix >> {1} 2>&1".format(iaas_file,log_file))
    shell_perform(res,"images generated")

    #生成yaml文件
    print("start generating yaml...")
    res = os.system("python {0} createyaml aix >> {1} 2>&1".format(iaas_file,log_file))
    shell_perform(res,"generate yaml")
    
    #修改yaml中的hostAliases配置
    #print("modify the yaml file and add the hostAliases configuration")
    #res = os.system("sh ./product_config/add_senttime_ca_host.sh")
    #shell_perform(res,"add hostAliases configuration to the yaml")

    #拉起pod服务
    print("pull up the pod of vias ")
    res = os.system("python {0} createapp aix >> {1} 2>&1".format(iaas_file,log_file))
    shell_perform(res,"pod pulled up")
    print("vias installed successfully.") 


