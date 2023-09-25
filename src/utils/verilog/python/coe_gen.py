import numpy as np
import scipy.signal

import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt


coebit, innerDepth = 18, 31
# basetap = 32
# dec1, dec2, dec3 = 8,4,2,
# tap1, tap2, tap3 = basetap*dec1//2, basetap*dec2//2, basetap*dec3//2

dec1, dec2, dec3 = 8, 4, 2
tap1 = dec1 * 2 * 8  # clock_rate, symmetric, multiplier
tap2 = dec2 * 2 * 5  # clock_rate, symmetric, multiplier
tap3 = dec3 * 2 * 5  # clock_rate, symmetric, multiplier
print(tap1, tap2, tap3)

s_tmp = scipy.signal.firwin(21, 1 / dec1, window=('chebwin', 120))
s1 = scipy.signal.firwin(tap1, 1 / dec1, window=('chebwin', 120))
s1 = np.round(np.asarray(s1) * 2 ** (23))
s2 = scipy.signal.firwin(tap2, 1 / dec2, window=('chebwin', 120))
s2 = np.round(np.asarray(s2) * 2 ** (coebit))
s3 = scipy.signal.firwin(tap3, 1 / dec3, window=('chebwin', 120))
s3 = np.round(np.asarray(s3) * 2 ** (coebit))
'''
TO_SIGNED(0, 24),TO_SIGNED(0, 24),TO_SIGNED(0, 24),TO_SIGNED(0, 24),
'''
print('-- now stage1 coe')
for i in range(len(s1) // 2):
    if i % 4 == 0:
        print('TO_SIGNED(%d, %d),TO_SIGNED(%d, %d),TO_SIGNED(%d, %d),TO_SIGNED(%d, %d),' %
              (s1[i], coebit, s1[i + 1], coebit, s1[i + 2], coebit, s1[i + 3], coebit))
        # print('TO_SIGNED(%d, coe_width),TO_SIGNED(%d, coe_width),TO_SIGNED(%d, coe_width),TO_SIGNED(%d, coe_width),' %
        #       (s1[i], s1[i + 1], s1[i + 2], s1[i + 3]))

print('-- now stage2 coe')
for i in range(len(s2)//2):
    if i % 4 == 0:
        print('TO_SIGNED(%d, %d),TO_SIGNED(%d, %d),TO_SIGNED(%d, %d),TO_SIGNED(%d, %d),' %
              (s2[i], coebit, s2[i + 1], coebit, s2[i + 2], coebit, s2[i + 3], coebit))
        # print('TO_SIGNED(%d, coe_width),TO_SIGNED(%d, coe_width),TO_SIGNED(%d, coe_width),TO_SIGNED(%d, coe_width),' %
        #       (s2[i], s2[i + 1], s2[i + 2], s2[i + 3]))

print('-- now stage3 coe')
for i in range(len(s3) // 2):
    if i % 4 == 0:
        print('TO_SIGNED(%d, %d),TO_SIGNED(%d, %d),TO_SIGNED(%d, %d),TO_SIGNED(%d, %d),' %
              (s3[i], coebit, s3[i + 1], coebit, s3[i + 2], coebit, s3[i + 3], coebit))
        # print('TO_SIGNED(%d, coe_width),TO_SIGNED(%d, coe_width),TO_SIGNED(%d, coe_width),TO_SIGNED(%d, coe_width),' %
        #       (s3[i], s3[i + 1], s3[i + 2], s3[i + 3]))

s1 = s1 / np.sum(s1)
s2 = s2 / np.sum(s2)
s3 = s3 / np.sum(s3)
w1, h1 = scipy.signal.freqz(s1)
plt.plot(w1 / np.pi, 20 * np.log10(abs(h1)), 'red')
w2, h2 = scipy.signal.freqz(s2)
plt.plot(w2 / np.pi / dec1, 20 * np.log10(abs(h2)), 'blue')
w3, h3 = scipy.signal.freqz(s3)
plt.plot(w3 / np.pi / dec1 / dec2, 20 * np.log10(abs(h3)), 'black')
plt.grid()
plt.show()
print(1)