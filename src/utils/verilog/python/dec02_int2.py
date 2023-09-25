import numpy as np
import scipy.signal
import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt

coebit, innerDepth = 18, 50
# basetap = 32
# dec1, dec2, dec3 = 8,4,2,
# tap1, tap2, tap3 = basetap*dec1//2, basetap*dec2//2, basetap*dec3//2

dec1, dec2, dec3 = 8, 4, 2
tap1 = dec1 * 2 * 8  # clock_rate, symmetric, multiplier
tap2 = dec2 * 2 * 5  # clock_rate, symmetric, multiplier
tap3 = dec3 * 2 * 5  # clock_rate, symmetric, multiplier
print(tap1, tap2, tap3)

s1 = scipy.signal.firwin(tap1, 1 / dec1, window=('chebwin', 120))
s1 = np.round(np.asarray(s1) * 2 ** (23)).astype(np.int64)
s2 = scipy.signal.firwin(tap2, 1 / dec2, window=('chebwin', 120))
s2 = np.round(np.asarray(s2) * 2 ** (coebit)).astype(np.int64)
s3 = scipy.signal.firwin(tap3, 1 / dec3, window=('chebwin', 120))
s3 = np.round(np.asarray(s3) * 2 ** (coebit)).astype(np.int64)

# ref = np.load('y4o256x.npy')
# x = np.load('sdm6o256x.npy').astype(np.int64)
x = np.load('E:\\FPGA\\proj\\fir\\python\\sdm6o64_192ksine.npy').astype(np.int64)
# x = np.load('sdm6o256x_square.npy').astype(np.int64)
x1 = scipy.signal.upfirdn(s1, x, 1)
x1 = np.round(x1).astype(np.int32)  # coebit+7 dont re-quantize
o1 = x1[:len(x)].reshape(-1, dec1)[:, -1]


# from gen_single import signed_dec2hex
# with open("E:\\FPGA\\proj\\fir\\data_in.txt","w") as f:
#     for d in o1[:1000]:
#         # req_data.append(signed_hex2dec(signed_dec2hex(d)))
#         f.write(signed_dec2hex(d, 32//4))
#         f.write("\n")

x2 = scipy.signal.upfirdn(s2, o1, 1)
x2 = np.floor(x2 / 2 ** (coebit)).astype(np.int32)
o2 = x2[:len(o1)].reshape(-1, dec2)[:, -1]


from gen_single import signed_dec2hex
with open("E:\\FPGA\\proj\\fir\\data_in.txt","w") as f:
    for d in x2[:1000]:
        # req_data.append(signed_hex2dec(signed_dec2hex(d)))
        f.write(signed_dec2hex(d, 32//4))
        f.write("\n")
# from gen_single import signed_dec2hex
# with open("E:\\FPGA\\proj\\fir\\data_in.txt","w") as f:
#     for d in o2[:1000]:
#         # req_data.append(signed_hex2dec(signed_dec2hex(d)))
#         f.write(signed_dec2hex(d, 32//4))
#         f.write("\n")
# #mine
# LEN = 40
# fir_data = np.zeros(LEN)

# def fir_inst_array(data, coef):
#     # '''
#     # :param data: 同上 使用数组乘法等规则简化和加快运算过程
#     # :param coef:
#     # :return:
#     # '''
#     # FIR_DATA数组使用切片索引等方式实现数组左移并在0位加上新的数据
#     fir_data[1:] = fir_data[0:LEN-1]
#     fir_data[0] = data
#     # 使用数组的点乘获取加权求和的结果
#     ret = np.dot(fir_data, coef)
#     return ret
# point_num = len(o1)
# ret = np.zeros(point_num)# 获取FIR延时加权求和的结果# 可以选择FIR_INST函数，操作流程直观
# # 可以选择FIR_INST_ARRAY函数，选用了数组点乘切片等方式，去掉循环时间应该更快
# for i in range(point_num):
#     ret[i] = fir_inst_array(o1[i], s2)

# ret =  ret[:len(o1)].reshape(-1, dec2)[:, -1]
# ret = np.round(ret / 2 ** (coebit)).astype(np.int32)

# plt.plot(ret[:500])
# # plt.plot(o2[:500])
# plt.show()
x3_ori = scipy.signal.upfirdn(s3, o2, 1)
x3 = np.floor(x3_ori / 2 ** (coebit)).astype(np.int32)
o3 = x3[:len(o2)].reshape(-1, dec3)[:, -1]

# print(o1[:100])
# print('')
# print(o2[:100])
# print('')
# print(o3[:100])
# print('')
# plt.step(np.arange(len(x1)),x1,'red')
# plt.step(np.arange(len(x2)),x2-0.25,'blue')
# plt.step(np.arange(len(x3)),x3-0.5,'black')
# plt.plot(o3-0.5,'black')
# plt.grid()
# plt.show()
np.save('o1.npy',o1.astype(np.int64))
np.save('o2.npy',o2.astype(np.int64))
np.save('o3.npy',o3.astype(np.int64))
#
#
#
#
#
#
#
o3 = o3.astype(np.float64)
o3 -= o3.mean()
o3 = o3[1100:-1100] / np.max(np.abs(o3[1100:-1100]))
nFFT = len(o3)//4
window = scipy.signal.hann(nFFT)
spec3 = np.fft.fft(o3[:nFFT] * window) / (nFFT/4)
fscale = np.fft.fftfreq(nFFT, d=1. / 48000/(256/dec1/dec2/dec3))

fig = plt.figure()
plt.plot(fscale[10:nFFT // 2], 20 * np.log10(spec3[10:nFFT // 2]))
plt.plot(fscale[10:nFFT // 2], np.ones_like(spec3[10:nFFT // 2])*(-144),'red')
plt.grid()
plt.show()
print(1)
#


