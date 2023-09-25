import numpy as np
from scipy import signal
import matplotlib.pyplot as plt

t = np.linspace(0, 1, 1000, False)  # 1 second
sig = np.sin(2*np.pi*10*t) + np.sin(2*np.pi*20*t)    # 构造10hz和20hz的两个信号
fig, (ax1, ax2) = plt.subplots(2, 1, sharex=True)
ax1.plot(t, sig)
ax1.set_title('10 Hz and 20 Hz sinusoids')
ax1.axis([0, 1, -2, 2])


sos = signal.butter(10, 15, 'hp', fs=1000, output='sos')     #采样率为1000hz，带宽为15hz，输出sos
filtered = signal.sosfilt(sos, sig)             #将信号和通过滤波器作用，得到滤波以后的结果。在这里sos有点像冲击响应，这个函数有点像卷积的作用。
ax2.plot(t, filtered)
ax2.set_title('After 15 Hz high-pass filter')
ax2.axis([0, 1, -2, 2])
ax2.set_xlabel('Time [seconds]')
plt.tight_layout()
plt.show()

print(1)