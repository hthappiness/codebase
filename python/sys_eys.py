
#!/usr/bin/env Python 
import os, sys, time 

def testPython():
    #几大数据结构
    a = []  #列表
    b = {}  #字典
    c = ('physics', 'chemistry', 1997, 2000)  #元组

    #控制结构
    fruits = ['banana', 'apple',  'mango']
    for fruit in fruits:        # 第二个实例
        print '当前水果 :', fruit

    count = 0
    while (count < 9):
        print 'The count is:', count
        count = count + 1
 
    print "Good bye!"
    #第三方库
    ret = os.mkdir("testOs.txt")

    #函数

    #面向对象

def test():
    while True: 
        time.sleep(4) 
        try: 
            ret = os.popen('ps -C apache -o pid,cmd').readlines() 
            if len(ret) < 2: 
                print "apache quit,restart after 4" 
                time.sleep(3) 
                os.system("service apache2 restart") 
        except: 
            print "Error", sys.exc_info()[1]

if len(sys.argv) > 1:
    INTERFACE = sys.argv[1]
else:
    INTERFACE = 'eth0'
STATS = []
print 'Interface:',INTERFACE

def rx():
    ifstat = open('/proc/net/dev').readlines()
    for interface in ifstat:
      if INTERFACE in interface:
          print "interface is:", interface   
          stat = float(interface.split()[1])
          STATS[0:] = [stat]
def tx():
    ifstat = open('/proc/net/dev').readlines()
    for interface in ifstat:
        if INTERFACE in interface:
            stat = float(interface.split()[9])
            STATS[1:] = [stat]
print  'In     Out'
rx()
tx()
while  True:
    time.sleep(1)
    rxstat_o = list(STATS)
    rx()
    tx()
    RX = float(STATS[0])
    RX_O = rxstat_o[0]
    TX = float(STATS[1])
    TX_O = rxstat_o[1]
    RX_RATE = round((RX - RX_O)/1024/1024,3)
    TX_RATE = round((TX - TX_O)/1024/1024,3)
    print RX_RATE ,'MB   ',TX_RATE ,'MB'