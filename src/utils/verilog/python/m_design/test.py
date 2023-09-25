import numpy as np
import scipy.signal

import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt


def fir_inst_array(data, coef):
    # '''
    # :param data: 同上 使用数组乘法等规则简化和加快运算过程
    # :param coef:
    # :return:
    # '''
    # FIR_DATA数组使用切片索引等方式实现数组左移并在0位加上新的数据
    fir_data[1:] = fir_data[0:LEN-1]
    fir_data[0] = data
    # 使用数组的点乘获取加权求和的结果
    ret = np.dot(fir_data, coef)
    return ret

# FIR抽头阶数
LEN = 72# FIR滤波器信号数据延时的数组
fir_data = np.zeros(LEN)# 制作一个信号源，对比效果# 采样频率
fs = 10000


t = np.linspace(0, 1, fs)
f = np.cos(2*np.pi*10*t) + 2 * np.cos(2*np.pi*3000*t)# 获取到滤波器，fs/fc = 10：1
coef = scipy.signal.firwin(72, 1 / 100, window=('chebwin', 120))
ret = np.zeros(fs)# 获取FIR延时加权求和的结果# 可以选择FIR_INST函数，操作流程直观
# 可以选择FIR_INST_ARRAY函数，选用了数组点乘切片等方式，去掉循环时间应该更快
for i in range(fs):
    ret[i] = fir_inst_array(f[i],coef)
    # ret[i] = fir_inst(f[i],coef)


def fir_inst(data, coef) :
    # '''
    # :param data: 原始信号 Orignal Signal
    # :param coef: FIR抽头系数数组
    # :return: 滤波之后的信号
    # '''
    ret = 0
    # 加权延时求和操作
    # 第一个抽头和最后的抽头系数一定是0，所以循环只从LEN-1到1
    # 第一个抽头的操作有点不同，它接收进来的data
    for i in range(LEN-1, 0, -1):
        ret = ret + fir_data[i] * coef[i]
        fir_data[i] = fir_data[i-1]
    fir_data[0] = data
    # fir_sum_data[0] = fir_data[0] * coef[0]
    return ret


plt.plot(t, f, "g")
plt.plot(t, ret, "r")
plt.show()
print(1)