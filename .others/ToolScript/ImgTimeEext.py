#!/usr/bin/python
#coding=utf-8
### Author：Jiang Zheng
### Time：2018-10-30
### 功能：提取图片的时间戳
### 输入：存放图片(以时间戳命名)的文件夹
### 输出(84坐标文件)：时间戳文件。
### 注意：输出文件未排序，用LibreOffice cal打开升序排列一下。

import sys
import os 
import random 

data_base_dir = "/home/bit/data/sysn_test/extract/cam0"
file_list = [] 
i=0

write_file_name = '/home/bit/data/sysn_test/extract/img.txt'
write_file = open(write_file_name, "w") 
names = os.listdir(data_base_dir) 
for file in names: 
    if file.endswith(".png"): 
      #write_name = file
      #file_list.append(write_name) 
      #sorted(file_list) 
      #number_of_lines = len(file_list) 
      index = file.rfind('.')
      file = file[:index]
      write_file.write(file+'\n')
      i=i+1
print i
write_file.close()



