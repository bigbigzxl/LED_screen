import numpy as np
import matplotlib.pyplot as plt
import scipy
def sin_wave(A, f, fs, phi, t):
    '''
    :params A:    振幅
    :params f:    信号频率
    :params fs:   采样频率
    :params phi:  相位
    :params t:    时间长度
    '''
    # 若时间序列长度为 t=1s, 
    # 采样频率 fs=1000 Hz, 则采样时间间隔 Ts=1/fs=0.001s
    # 对于时间序列采样点个数为 n=t/Ts=1/0.001=1000, 即有1000个点,每个点间隔为 Ts
    Ts = 1/fs
    n = t / Ts
    n = np.arange(n)
    y = A*np.sin(2*np.pi*f*n*Ts + phi*(np.pi/180))
    return y


def fourBin2OneHex(four_bin: str) -> str:
    '''
    函数功能：4位2进制字符串 -> 1位16进制字符串\n
    输入：4位2进制字符串，输入范围0000~1111\n
    输出：1位16进制字符串
    '''
    if (four_bin == '0000'):
        return '0'
    elif (four_bin == '0001'):
        return '1'
    elif (four_bin == '0010'):
        return '2'
    elif (four_bin == '0011'):
        return '3'
    elif (four_bin == '0100'):
        return '4'
    elif (four_bin == '0101'):
        return '5'
    elif (four_bin == '0110'):
        return '6'
    elif (four_bin == '0111'):
        return '7'
    elif (four_bin == '1000'):
        return '8'
    elif (four_bin == '1001'):
        return '9'
    elif (four_bin == '1010'):
        return 'a'
    elif (four_bin == '1011'):
        return 'b'
    elif (four_bin == '1100'):
        return 'c'
    elif (four_bin == '1101'):
        return 'd'
    elif (four_bin == '1110'):
        return 'e'
    elif (four_bin == '1111'):
        return 'f'
    else:
        int('输入2进制字符' + four_bin + '错误，2进制只能包含0或1')

def signed_bin2hex(bin_str: str, hex_width: int = -1) -> str:
    '''
    函数功能：2进制补码字符串 -> 16进制补码字符串\n
    输入参数1：2进制补码字符串，不可带正负号，前后可加任意个 \\n 和 空格，数字间可加下划线\n
    输入参数2：可选，16进制补码字符串宽度，若实际输出宽度>此参数，警告并原样输出；若实际输出宽度<=此参数，高位补若干符号位\n
    输出：16进制补码字符串
    '''
    input_bin_str = bin_str
    bin_str = bin_str.strip()
    if (bin_str[:2] == '0b'): # 2进制字符串以0b开头
        bin_str = bin_str[2:]
    elif (bin_str[0] == '0' or bin_str[0] == '1'):
        pass
    else:
        int('输入 ' + bin_str + ' 不合法，输入必须为2进制补码，不允许带正负号 且 首字符不能是下划线')
    # 检查输入是否合法，末尾字符不能是下划线 且 不能出现连续的两个下划线
    if (bin_str[-1] == '_' or '__' in bin_str):
        int('输入 ' + bin_str + ' 不合法，末尾字符不能是下划线 且 不能出现连续的两个下划线')
    else:
        bin_str = bin_str.replace('_', '') # 输入合法则去除下划线
    # 去掉2进制补码字符串前面多余的符号位，保留两位
    for i in range(len(bin_str)-1):
        if (bin_str[i+1] == bin_str[0]):
            if (i + 1 == len(bin_str)-1):
                bin_str = bin_str[i:]
            else:
                continue
        else:
            bin_str = bin_str[i:]
            break
    if (len(bin_str) % 4 > 0): # 补符号位到位宽为4的倍数
        bin_str = bin_str[0] * (4 - len(bin_str) % 4) + bin_str
    hex_str = ''
    for i in range(int(len(bin_str)/4)):
        hex_str += fourBin2OneHex(bin_str[i*4 : i*4+4])
    if (hex_width == -1):
        pass
    elif (hex_width < len(hex_str)):
        print('位宽参数' + str(hex_width) + ' < 2进制补码' + input_bin_str + '输出16进制补码'
            + '0x' + hex_str +'实际位宽' + str(len(hex_str)) + '，请修正位宽参数')
    else:
        if (hex_str[0] in ['0', '1', '2', '3', '4', '5', '6', '7']):
            hex_str = '0' * (hex_width - len(hex_str)) + hex_str
        else:
            hex_str = 'f' * (hex_width - len(hex_str)) + hex_str
    return '0x' + hex_str
def signed_dec2bin(dec_num: int, bin_width: int = -1) -> str:
    '''
    函数功能：10进制数/字符串 -> 2进制补码字符串\n
    输入参数1：10进制数/字符串，可带正负号，前后可加任意个 \\n 和 空格，数字间可加下划线\n
    输入参数2：可选，2进制补码字符串宽度，若实际输出宽度>此参数，警告并原样输出；若实际输出宽度<=此参数，高位补若干符号位\n
    输出：2进制补码字符串
    '''
    dec_num_str = str(dec_num)
    if (type(dec_num) == str):
        dec_num = int(dec_num.strip())
    if (dec_num == 0):
        bin_str = '0'
    elif (dec_num > 0):
        bin_str = '0' + bin(dec_num)[2:] # 补符号位0
    else:
        for i in range(10000):
            if (2**i + dec_num >= 0):
                bin_str = bin(2**(i+1) + dec_num)[2:] # 一个负数num的补码等于（2**i + dec_num)
                break
    if (bin_width == -1):
        pass
    elif (bin_width < len(bin_str)):
        # 实际位宽大于设定位宽则报警告，然后按实际位宽输出
        print('位宽参数' + str(bin_width) + ' < 10进制' + dec_num_str + '输出2进制补码'
            + '0b' + bin_str + '实际位宽' + str(len(bin_str)) + '，请修正位宽参数')
    else:
        bin_str = bin_str[0] * (bin_width - len(bin_str)) + bin_str # 实际位宽小于设定位宽则补符号位
    return '0b' + bin_str

# 原文链接：https://blog.csdn.net/weixin_42837669/article/details/121446738
def signed_dec2hex(dec_num: int, hex_width = -1) -> str:
    '''
    函数功能：10进制数/字符串 -> 16进制补码字符串\n
    输入参数1：10进制数/字符串，可带正负号，前后可加任意个 \\n 和 空格，数字间可加下划线_\n
    输入参数2：可选，16进制补码字符串宽度，若实际输出宽度>此参数，警告并原样输出；若实际输出宽度<=此参数，高位补若干符号位\n
    输出：16进制补码字符串
    '''
    hex_str = signed_bin2hex(signed_dec2bin(dec_num, hex_width*4), hex_width)[2:]
    if (hex_width == -1):
        pass
    elif (hex_width < len(hex_str)):
        print('位宽参数' + str(hex_width) + ' < 10进制' + str(dec_num) + '输出16进制补码' + '0x' +
            hex_str + '实际位宽' + str(len(hex_str)) + '，请修正位宽参数')
    else:

        if (hex_str[0] in ['0', '1', '2', '3', '4', '5', '6', '7']):
            hex_str = '0' * (hex_width - len(hex_str)) + hex_str
        else:
            hex_str = 'f' * (hex_width - len(hex_str)) + hex_str
    # return '0x' + hex_str
    return hex_str


def oneHex2fourBin(one_hex: str) -> str:
    '''
    函数功能：1位16进制字符串 -> 4位2进制字符串\n
    输入：1位16进制字符串，输入范围0~9, a~f或A~F\n
    输出：4位2进制字符串
    '''
    if (one_hex == '0'):
        return '0000'
    elif (one_hex == '1'):
        return '0001'
    elif (one_hex == '2'):
        return '0010'
    elif (one_hex == '3'):
        return '0011'
    elif (one_hex == '4'):
        return '0100'
    elif (one_hex == '5'):
        return '0101'
    elif (one_hex == '6'):
        return '0110'
    elif (one_hex == '7'):
        return '0111'
    elif (one_hex == '8'):
        return '1000'
    elif (one_hex == '9'):
        return '1001'
    elif (one_hex == 'a' or one_hex == 'A'):
        return '1010'
    elif (one_hex == 'b' or one_hex == 'B'):
        return '1011'
    elif (one_hex == 'c' or one_hex == 'C'):
        return '1100'
    elif (one_hex == 'd' or one_hex == 'D'):
        return '1101'
    elif (one_hex == 'e' or one_hex == 'E'):
        return '1110'
    elif (one_hex == 'f' or one_hex == 'F'):
        return '1111'
    else:
        int('输入16进制字符' + one_hex + '错误，16进制只能包含0~9, a~f或A~F')

def signed_hex2bin(hex_str: str, bin_width: int = -1) -> str:
    '''
    函数功能：16进制补码字符串 -> 2进制补码字符串\n
    输入参数1：16进制补码字符串，不可带正负号，前后可加任意个 \\n 和 空格，数字间可加下划线\n
    输入参数2：可选，2进制补码字符串宽度，若实际输出宽度>此参数，警告并原样输出；若实际输出宽度<=此参数，高位补若干符号位\n
    输出：2进制补码字符串
    '''
    input_hex_str = hex_str
    hex_str = hex_str.strip()
    # 检查输入是否合法，不允许带正负号，首尾不能是下划线，不能出现连续两个下划线
    if (hex_str[0] in ['+', '-', '_'] or hex_str[-1] == '_' or '__' in hex_str):
        int('输入' + input_hex_str + '不合法，必须为16进制补码，不允许带正负号, '
            + '首尾不能是下划线，不能出现连续两个下划线')
    elif (hex_str[:2] == '0x'):
        hex_str = hex_str[2:]
    hex_str = hex_str.replace('_', '') # 输入合法则去除下划线
    bin_str = ''
    for i in hex_str:
        bin_str += oneHex2fourBin(i)
    # 去掉2进制补码字符串前面多余的符号位，保留两位
    for i in range(len(bin_str)-1):
        if (bin_str[i+1] == bin_str[0]):
            if (i + 1 == len(bin_str)-1):
                bin_str = bin_str[i:]
            else:
                continue
        else:
            bin_str = bin_str[i:]
            break
    if (bin_str == '00'):
        bin_str = '0'
    if (bin_width == -1):
        pass
    elif (bin_width < len(bin_str)):
        # 实际位宽大于设定位宽则报警告，然后按实际位宽输出
        print('位宽参数' + str(bin_width) + ' < 16进制补码' + input_hex_str + '输出2进制补码'
            + '0b' + bin_str + '实际位宽' + str(len(bin_str)) + '，请修正位宽参数')
    else:
        bin_str = bin_str[0] * (bin_width - len(bin_str)) + bin_str # 实际位宽小于设定位宽则补符号位
    return '0b' + bin_str
def signed_bin2dec(bin_str: str) -> int:
    '''
    函数功能：2进制补码字符串 -> 10进制整数\n
    输入：2进制补码字符串，不可带正负号，前后可加任意个 \\n 和 空格，数字间可加下划线\n
    输出：10进制整数，只保留负号，正号不保留
    '''
    bin_str = bin_str.strip()
    if (bin_str[:2] == '0b'):
        if (bin_str[2] == '_'):
            bin_str = bin_str[3:]
        else:
            bin_str = bin_str[2:]
    if (bin_str[0] == '_'):
        int ('输入 ' + bin_str + ' 不合法，首字符不能是下划线 且 不允许出现连续两个下划线')
    elif (bin_str[0] == '0'):
        return int(bin_str, base = 2)
    elif (bin_str[0] == '1'):
        a = int(bin_str, base = 2) # 此语句可检查输入是否合法
        bin_str = bin_str.replace('_', '')
        return a - 2**len(bin_str)
    else:
        int('输入 ' + bin_str + ' 不合法，必须为2进制补码，不允许带正负号')

def signed_hex2dec(hex_str: str) -> int:
    '''
    函数功能：16进制补码字符串 -> 10进制整数\n
    输入：16进制补码字符串，不可带正负号，前后可加任意个 \\n 和 空格，数字间可加下划线\n
    输出：10进制整数，只保留负号，正号不保留
    '''
    return signed_bin2dec(signed_hex2bin(hex_str))


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

# hz_50 = sin_wave(A=1, f=50, fs=fs, phi=0, t=0.08)
# hz_50_30 = sin_wave(A=1, f=50, fs=fs, phi=30, t=0.08)
# hz_50_60 = sin_wave(A=1, f=50, fs=fs, phi=60, t=0.08)
# hz_50_90 = sin_wave(A=1, f=50, fs=fs, phi=90, t=0.08)
# x = np.arange(0, 0.08, 1/fs)
# plt.xlabel('t/s')
# plt.ylabel('y')
# plt.grid()
# plt.plot(x, hz_50, 'k')
# plt.plot(x, hz_50_30, 'r-.')
# plt.plot(x, hz_50_60, 'g--')
# plt.plot(x, hz_50_90, 'b-.')
# plt.legend(['phase 0', 'phase 30', 'phase 60', 'phase 90'], loc=1)




if __name__ == "__main__":




    fs = 2000
    duration = 1/10 # 1s
    hz_1 = sin_wave(A=1, f=10, fs=fs, phi=0, t=duration)
    hz_2 = sin_wave(A=1, f=500, fs=fs, phi=0, t=duration)
    signal = hz_1 + hz_2

    x = np.arange(0,duration, 1/fs)
    plt.plot(x, signal, 'g--')



    # FIR抽头阶数
    LEN = 40# FIR滤波器信号数据延时的数组
    fir_data = np.zeros(LEN)# 制作一个信号源，对比效果# 采样频率

    t = np.linspace(0, 1, fs)
    f = np.cos(2*np.pi*10*t) + 2 * np.cos(2*np.pi*3000*t)# 获取到滤波器，fs/fc = 10：1
    coef = scipy.signal.firwin(40, 1 / 100, window=('chebwin', 120))
    ret = np.zeros(fs)# 获取FIR延时加权求和的结果# 可以选择FIR_INST函数，操作流程直观
    # 可以选择FIR_INST_ARRAY函数，选用了数组点乘切片等方式，去掉循环时间应该更快
    for i in range(fs):
        ret[i] = fir_inst_array(f[i],coef)
        # ret[i] = fir_inst(f[i],coef)
    plt.plot(t, ret, "r")
    plt.show()



    selected_datas = np.array(signal) / np.max(np.abs(signal))
    data_int32 = np.round(selected_datas * 2**31).astype(np.int64)
    data_int32 = np.clip(data_int32, -2**31, 2**31 - 1)
    x1 = np.arange(0, len(signal), 1)
    # plt.plot(x1, signal, 'r-o')
    plt.plot(x1, data_int32, 'r-o')
    plt.show()

    req_data = []
    with open("E:\\FPGA\\proj\\fir\\data_in.txt","w") as f:
        for d in data_int32:
            # req_data.append(signed_hex2dec(signed_dec2hex(d)))
            f.write(signed_dec2hex(d))
            f.write("\n")
    # np.save('./data_in.bin', data_int32)
    # print(data_int32 - np.array(req_data))
    print(1)