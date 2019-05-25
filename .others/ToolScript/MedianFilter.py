#! /usr/bin/python3.4
#coding=utf-8
### Author：Jiang Zheng
### Time：2019-01-10
### 功能：磁力计数据中值滤波，绘制滤波前后对比图
### 输入：原始磁力计数据文件
### 输出：滤波前后对比图

import numpy as np
#import pylab as p
#import matplotlib
#import matplotlib.pyplot as plt
import matplotlib.pyplot as plt
import scipy.signal as signal
import csv
import os

open_file_name = u'test.csv'

csvRfile = file(open_file_name,'rb')
csv_rb = csv.reader(csvRfile)

datax = []
datay = []
#i = 0
for item in csv_rb:        
    datax.append( item[0] ) 
    datay.append( item[1] ) 

print "length of data:",len(datay)
#print data
csvRfile.close()

datay = map(float,datay)

plt.subplot(121)
plt.xlim(xmax=3205,xmin=0)
plt.ylim(ymax=360,ymin=0)
plt.xlabel("x")
plt.ylabel("y")
plt.plot(datax,datay,'ro')

plt.subplot(122)
plt.xlim(xmax=3205,xmin=0)
plt.ylim(ymax=360,ymin=0)
plt.xlabel("x")
plt.ylabel("y")
plt.plot(datax,signal.medfilt(datay,21),'ro')

plt.show()
# get some linear data
#x = np.linspace(0,3,101)
#print('Without noisy:',x)

# ass some noisy signal
#x[5::10] = 1.5
#print('With noisy:',x)
#p.plot(data)
#p.plot(signal.medfilt(data,3))

#p.legend(['original signal', 'length 3', 'length 5'])
#p.show()

#p.plot(signal.medfilt(data,7))
#p.show()