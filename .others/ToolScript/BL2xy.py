#!/usr/bin/python
#coding=utf-8
### Author：Jiang Zheng
### Time：2018-11-30
### 功能：将经纬度高斯投影转为WGS-84坐标
### 输入(经纬度文件)：BL_raw.csv
### 输出(84坐标文件)：xy_save.csv
from math import *
import csv
import os

open_file_name = u'BL_raw.csv'
to_file_name = u'xy_save.csv'
#参数
a  = 6378137                #椭球长半轴
b  = 6356752.3142           #椭球短半轴
f  = 1-b/a                  #椭球扁率
e  = sqrt(a**2-b**2)/a      #椭球第一偏心率
et = sqrt(a**2-b**2)/b      #椭球第二偏心率
#pt = 1.0#180.0*3600/pi      #如果进行了弧度变换,就不需要这个参数
#L0 = 111.0                  #中心轴线   可设  后面设为自动计算
zonewide = 6                 #6分带或3分带
	
#参数m集合类
class arg_m():
    def __init__(self,a,e):
        self.a  = float(a)      #椭圆短半轴
        self.e  = float(e)      #第一偏心率
        self.m0 = self.a*(1-e**2)
        self.m2 = 3*self.e**2*self.m0/2
        self.m4 = 5*self.e**2*self.m2/4     
        self.m6 = 7*self.e**2*self.m4/6
        self.m8 = 9*self.e**2*self.m6/8

#参数a集合类,继承m
class arg_a(arg_m):
    def __init__(self,a,e):
        arg_m.__init__(self,a,e)
        self.a0 = self.m0+self.m2/2+3*self.m4/8+5*self.m6/16+35*self.m8/128
        self.a2 = self.m2/2+self.m4/2+15*self.m6/32+7*self.m8/16
        self.a4 = self.m4/8+3*self.m6/16+7*self.m8/32
        self.a6 = self.m6/32+self.m8/16
        self.a8 = self.m8/128

#参数x集合类,继承a,计算X
class arg_x(arg_a):
    def __init__(self,a,e,B):
        arg_a.__init__(self,a,e)
        self.B  = float(B)        
        self.X  = self.a0*self.B-sin(B)*cos(B)*(
            (self.a2-self.a4+self.a6)+
            (2*self.a4-16*self.a6/3)*sin(B)**2+
            16*self.a6*sin(B)**4/3
            )

        
def gaosi_Y(B,L):
    global a
    global et
    global L0
    global pt
    B  = radians(B)
    #L  = radians(L)
    t  = tan(B)
    nn = et**2*cos(B)**2
    Lt = radians(L-L0)
    N  = a/sqrt(1-e**2*sin(B)**2)
    
    y1 = N*cos(B)*Lt
    y2 = N*cos(B)**3*(1-t**2+nn**2)*Lt**3/(6)
    y3 = N*cos(B)**5*(5-18*t**2+t**4+14*nn**2-58*nn**2*t**2)*Lt**5/(120)
    Y  = y1 + y2 + y3 + 500000
    return Y

def gaosi_X(B,L):
    global a
    global e
    global L0
    global pt
    B  = radians(B)
    #L  = radians(L) //手动固定L值
    t  = tan(B)
    Lt = radians(L-L0)
    xX = arg_x(a,e,B)
    N  = a/sqrt(1-e**2*sin(B)**2)
    nn = et**2*cos(B)**2

    x1 = N*sin(B)*cos(B)*Lt**2/(2)
    x2 = N*sin(B)*cos(B)**3*(5-t**2+9*nn**2+4*nn**4)*Lt**4/(24)
    x3 = N*sin(B)*cos(B)**5*(61-58*t**2+t**4)*Lt**6/(720)
    X  = xX.X + x1 + x2 + x3
    return X
    
def gaosi(B,L):
    global zonewide
    global L0
    if zonewide == 6:
        prjno = int(L / zonewide) + 1
        L0 = prjno * zonewide - 3
    else:
        prjno = int((L - 1.5) / 3) + 1
        L0 = prjno * 3
    print(L0)

    x = gaosi_X(B,L)
    y = gaosi_Y(B,L)
    return x,y

#print "test_x:%s test_y:%s" % gaosi(19.6903977,110.0090321)

if __name__ == ('__main__'):

    csvRfile = file(open_file_name,'rb')
    csv_rb = csv.reader(csvRfile)
    csvWfile = file(to_file_name,'w')
    csv_wb = csv.writer(csvWfile)

    #save_txt = file(to_file_name,'w')
    i = 1
    w_data = []
    for line in csv_rb:
        if i==1:
            i=i+1
        else:
            print(line)
            #print(line[0])
            x = float(line[0])
            y = float(line[1])
            xx,yy = gaosi(x,y)
            print([xx,yy])
            print('**********************************')
            csv_wb.writerow([xx,yy])
    csvRfile.close()
    csvWfile.close()
