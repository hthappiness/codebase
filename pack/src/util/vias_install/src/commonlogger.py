#!/usr/bin/python
# -*- coding: UTF-8 -*-

import logging
import re
import os
logging.basicConfig(level = logging.INFO,format = '%(asctime)s - %(name)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)

def debug(msg):
    logger.debug(msg)

def info(msg):
    logger.info(msg)

def warn(msg):
    logger.warn(msg)

def error(msg):
    logger.error(msg)

def exception(msg):
    logger.exception(msg)

# 获取集群所有node的ip地址
def get_node():
    try:
        nodes = os.popen("/root/local/bin/kubectl get node | grep -v NAME | awk '{print $1}'").readlines()
        return nodes
    except:
        logger.error("Get kubernetes node failed!")

# ip地址合法性检查、是否属于集群检查
def val(items):
    # 是否属于集群检查
    # try:
    #     nodes = get_node()
    #     result=set(items).issubset(set(nodes))
    #     if (not result):
    #         logger.error("ERROR! Invalid ip address")
    #         return False
    # except:
    #     logger.error("ERROR! Invalid ip address")
    #     return False
    # 合法性检查
    for item in items:
        if type(item) == unicode:
            name = str(item)
            if not re.match(r'^((25[0-5]|2[0-4]\d|((1\d{2})|([1-9]?\d)))\.){3}(25[0-5]|2[0-4]\d|((1\d{2})|([1-9]?\d)))$',name):
                logger.error("Error, invalid ip: "+name)
                return False
        else:
            logger.error("Error, invalid ip, the ip must be a string value.")
            return False
    return True