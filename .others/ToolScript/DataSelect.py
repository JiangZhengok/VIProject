#!/usr/bin/python
#coding=utf-8
### Author：Jiang Zheng
### Time：2018-12-15
### 功能：数据隔行提取，主要用于数据降频
### 输入：原始数据csv文件 dmc.csv
### 输出：降频后的csv文件 result.csv

import os
import csv
import sys
__author__ = 'Jiang Zheng'
#解决中文乱码问题
defaultencoding = 'utf-8'
if sys.getdefaultencoding() != defaultencoding:
    reload(sys)
    sys.setdefaultencoding(defaultencoding)
#全局变量，存放从csv中读取出来的特定行
global data
data = []
i = 1

csvFile = open('/home/bit/data/11.28/dmc.csv','r')
reader = csv.reader(csvFile)
for item in reader:
    i=i+1              #如果需要跳过表头，将此句放在循环最后
    if len(item) == 1:
        pass
    else:
        if i%10 == 0: #10倍降频，可更改
            data.append(item)

print "length of data:",len(data)
csvFile.close()
#解析后的文件存放到result.csv文件中
csvFile2 = open('result.csv','wb')#mode要写成wb,如果是w，则会出现空行的情况。
writer = csv.writer(csvFile2)
m = len(data)
for i in range(m):
    writer.writerow(data[i])
#writer.writerow(newTitle)
csvFile2.close()
