#!/usr/bin/python
# -*- coding: UTF-8 -*-
import os
import re
import commonlogger as logger


def order(script, action):
    """
    命令执行函数
    :param script:要执行的命令
    :param action:读取结果的方式
    :return:执行结果
    """
    cmd = os.popen(script)
    if action == "read":
        result = cmd.read()
    elif action == "readline":
        result = cmd.readline()
    elif action == "readlines":
        result = cmd.readlines()
    else:
        logger.error("Error arg for order")
    cmd.close()
    return result


def get_json(obj, key):
    """
    获取key对应的value
    :param obj: json对象
    :param key: 键值
    :return: 对应的value值，出错则为False
    """
    if type(key) == unicode:
        key = str(key)
    if key in obj:
        return obj[key]
    else:
        logger.error("Wrong configuration with " + key + "!")
        return False


# 获取集群所有node的ip地址
def get_node():
    try:
        scripts = "/root/local/bin/kubectl get node | grep -v NAME | awk '{print $1}'"
        nodes = order(scripts, "readlines")
        return nodes
    except:
        logger.error("Get kubernetes node failed!")


# ip地址合法性检查、是否属于集群检查
def val(items):
    # 合法性检查
    for item in items:
        if type(item) == unicode:
            name = str(item)
            if not re.match(
                    r'^((25[0-5]|2[0-4]\d|((1\d{2})|([1-9]?\d)))\.){3}(25[0-5]|2[0-4]\d|((1\d{2})|([1-9]?\d)))$', name):
                logger.error("Error, invalid ip: " + name)
                return False
        else:
            logger.error("Error, invalid ip, the ip must be a string value.")
            return False
    return True
