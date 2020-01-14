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
outputfile = open("{}/viasuninstall.log".format(dir),'w')
sys.stdout = outputfile
print("sys.path is : {}".format(sys.path))
print("dir is : {}".format(dir))
print("data is : {}".format(data))
outputfile.close()
sys.stdout = output

log_file = dir + "/viasuninstall.log"

def create_uninfile():
    """
    创建pgsql的初始化脚本文件
    """
    unin_sh_file = dir + "/psql/template_unin.sh"
    new_unin_sh_file = dir + "/psql/unin.sh"
    
    with open(unin_sh_file, "r") as f:
        newfile = []
        for oneline in f:
            for key in data:
                if key in oneline:
                    oneline =  oneline.replace(key, data[key])
            newfile.append(oneline)
    s = ''.join(newfile)
    with open(new_unin_sh_file, "w+") as newunin:
        newunin.write(s) 

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

def exec_in_pod(podname, workpath):
    """
    在容器内部执行初始化脚本
    """
    operation = " -- sh -c \"cd {0}/psql && chmod +x {0}/psql/unin.sh && {0}/psql/unin.sh load\" >> {1} 2>&1".format(workpath,log_file)
    cmd = "/root/local/bin/kubectl exec " + podname + operation
    return shell_box(cmd)

def pgsql_unin():
    """
    将初始化文件拷贝到pgsql的pod中，然后执行初始化脚本
    """
    try:
        print("begin unin pgsql...")
        workpath = "/vias_unin.first"
        create_uninfile()
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
            print("copy the unin file to pgsql error")
            print("the detail error info is: {}".format(ret))
            return False
        print("podname:{0}, workpath: {1}".format(podname, workpath))    
        code, ret = exec_in_pod(podname, workpath)
        if code:
            print("exec the unin operation error")
            print("the detail error info is: {}".format(ret))
            return False
        print("unin pgsql success.")
        return True
    except:
        print("unin pgsql error...")
        return False

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
    print("### start uninstall vias service ###")

    #判断是否删除数据库
    select = raw_input("Delete the pgsql data or not?(Y/N)")
    if select == "Y" or select == "y":
        print("begin uninstall pgsql...")
        res = func_print(pgsql_unin)
        func_perform(res,"pgsql deleted")
    elif select == "N" or select == "n": 
        print("pgsql data not delete.")    
    else:
        print("Input error, exit.")
        exit(1)

    #删除pod
    print("delete vias pod.")
    res = os.system("python {0} uninstall aix >> {1} 2>&1".format(iaas_file,log_file))
    shell_perform(res,"delete pod")

    #删除images
    print ("delete all images of vias")
    res=os.system("sh {0}/product_config/uninstall_mirror.sh >> {1} 2>&1".format(dir,log_file))
    shell_perform(res,"delete images")
    print("vias uninstall successful.") 


