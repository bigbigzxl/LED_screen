import numpy as np
from scipy.fftpack import fft
from scipy import signal
import matplotlib.pyplot as plt
from matplotlib.pylab import mpl
mpl.rcParams['font.sans-serif'] = ['SimHei']   #显示中文
mpl.rcParams['axes.unicode_minus']=False       #显示负号
N = 1024 #采样点的个数
x = np.arange(0,2*np.pi,2*np.pi/N)
#产生频率为120、500、10hz的信号进行模拟
y = 7 * np.sin(120*x) + 5 * np.sin(500 * x) + 9 * np.sin(10 * x)
# plt.plot(x, y, "-o")

# s1 = signal.firwin(128, 2*50 / N, window=('chebwin', 120))
# filtered = signal.sosfilt(s1, y)             #将信号和通过滤波器作用，得到滤波以后的结果。在这里sos有点像冲击响应，这个函数有点像卷积的作用。
plt.plot(x, filtered)
plt.show()

w = np.arange(0,N,1)#频域轴

b1 = signal.firwin(N,  2*100/N, window='hamming')#哈明窗，截至频率100Hz
w1, h = signal.freqz(b1) #求频响
plt.figure(1)
plt.title("hamming频率响应")
plt.plot(w1/2/np.pi*N, 20*np.log10(np.abs(h)+0.01))

b2 = signal.firwin(N,  2*100/N, window='hann')#汉宁窗，截至频率100Hz
w1, h = signal.freqz(b2) #求频响
plt.figure(2)
plt.title("hann频率响应")
plt.plot(w1/2/np.pi*N, 20*np.log10(np.abs(h)+0.01))

b3 = signal.firwin(N,  2*100/N, window='blackman')#布莱克曼窗，截至频率100Hz
w1, h = signal.freqz(b3) #求频响
plt.figure(3)
plt.title("blackman频率响应")
plt.plot(w1/2/np.pi*N, 20*np.log10(np.abs(h)+0.01))

b4 = signal.firwin(N,  2*100/N, window='boxcar')#矩形窗，截至频率100Hz
w1, h = signal.freqz(b4) #求频响
plt.figure(4)
plt.title("boxcar频率响应")
plt.plot(w1/2/np.pi*N, 20*np.log10(np.abs(h)+0.01))

plt.show()
print(1)