#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

/**
    8位存储方式（我们比较常用的方式）
**/

unsigned char ip_hdr_8[] =
{
    0x45, 0x00,
    0x00, 0x3c,
    0x00, 0x00,
    0x00, 0x00,
    0x40, 0x11,
    0x00, 0x00, ///checksum
    0xc0, 0xa8,
    0x2b, 0xc3,
    0x08, 0x08,
    0x08, 0x08,
    0x11
};

/**
 * @brief checksum_8
 *      8 bit checksum 构建
 * @param buf :
 * @param size : size of buf
 * @return 成功返回 checksum， 失败返回 0 ？
 */
ushort checksum_8(uint8_t* buf, uint size)
{
    if(buf == nullptr || size == 0) {
        return 0;
    }
    uint cksum = 0; /// 存放返回值 checksunm
    int num = 0;
    ushort high_bit;
    ushort low_bit;

    /** 每2个字节分组，进行累加 pBuffer[0] pBuffer[1] (高位，低位) + pBuffer[2] pBuffer[3] (高位，地位) .....*/
    while(size >= 1) {
        if(size == 1) { /// data 数组 为单数，checksum 计算最后一组数据 补 0x00 (仅在最后一位且总长度为奇数时，才会触发该分支)
            high_bit = buf[num];
            low_bit = 0x0000;
            cksum += (high_bit<<8) + low_bit;
            size = 1;
            num += 1;
            break;
        } else {        /// data 数组 为双数，checksum 无需补位
            high_bit = buf[num];
            low_bit = buf[num+1] & 0x00FF;
            cksum += (high_bit<<8) + low_bit;
            size -= 2;
            num += 2;
        }
    }

    /** 将32bit值的高16bit与低16bit相加到一个新的32bit值中，若新的32bit值大于0Xffff, 再将新值的高16bit与低16bit相加； */
    while (cksum>>16) {
        cksum = (cksum&0xFFFF) + (cksum>>16);
    }
    ushort s_cksum = cksum;

    return ~s_cksum;
}


int main()
{
    int cksum = checksum_8(ip_hdr_8, 21);
    printf("chsum = %0x \n", cksum);
    return 0;
}
