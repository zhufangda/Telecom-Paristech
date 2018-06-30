# 更改文件编码
# 文件统一改为utf-8无BOM
# -*- coding: UTF-8 -*-
import os
import pandas as pd

#需要把文件改成编码的格式（可以自己随时修改）
coding = 'utf-8'
# 文件夹目录（可以更改文件编码的文件夹~）
file_dir = 'E:/SD/SD201/TP2/Documents/Company'

def run_coding():
    for root, dirs, files in os.walk(file_dir, topdown=False):
        for i in files:
            files_name = os.path.join(root, i)
            try:
                df1 = pd.read_csv(files_name, encoding='utf-8')
            except:
                df1 = pd.read_csv(files_name, encoding='gbk')
            df1.to_csv(files_name, encoding=coding,index=None)

if __name__ == '__main__':
    run_coding()
    print("It's done")