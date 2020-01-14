#!/usr/bin/python
# -*- coding: UTF-8 -*-

import requests


def get(url):
    return requests.get(url)

def post(url, payload):
    return requests.post(url, data=payload)

def delete(url):
    return requests.delete(url)
# 局部更新资源
def patch(url, payload):
    headers = {"Content-Type": "application/strategic-merge-patch+json"}
    return requests.patch(url, data=payload, headers=headers)
