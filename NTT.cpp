// NTT.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include<stdio.h>

#define g 3//NTT变换是原根，或者是主根
#define mod 17//NTT变换的模数，一般是素数

int hash[2 ^ 16];
int rev[2 ^ 16];

void swap(int*x, int*y)
{
	int temp;

	temp = *x;
	*x = *y;
	*y = temp;

}

int mod_pow(int a, int b) {
	int res = 1;
	for (; b; b >>= 1) {
		if (b & 1) res = res * a % mod;
		a = a * a % mod;
	}
	return res;
}

//NTT数论变换，采用快速数论变换实现的方式实现
//参数：*data  进行数论变换的指针
//		len	   进行数论变换的数据长度
//		inv	   当值为1时进行NTT反变换，当值为0时进行NTT正变换
//返回值： 0	变换成功
//		  -1    变换失败
int ntt(int *data, int len, int is_intt)
{
	int i, j, k;//循环变量
	int bit_width;//快速NTT的级数
	int	w, wn, x, y, inv;

	bit_width = 0;
	//计算进行NTT变换的数据位宽，
	while ((1 << bit_width) < len) bit_width++;
	//重排进行NTT变换的数据顺序，其重排依据的是其索引的逆序数
	for (i = 0; i < len; i++) {
		rev[0] = 0;
		rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (bit_width - 1));
		if (i < rev[i]) swap(&data[i], &data[rev[i]]);
	}
	//第一层循环，循环为层级循环
	for (i = 1; i < len; i *= 2) {
		wn = mod_pow(g, (mod - 1) / (i * 2));
		if (is_intt) wn = mod_pow(wn, mod - 2);
		w = 1;
		//第二层循环，每一层级循环中，其每组成员数为i*2，遍历第一组前半部分，其数量为i
		for (j = 0; j < i; j++) {
			//第三层循环，依次循环执行每组中相同变换因子（W）的蝶形计算
			for (k = j; k < len; k += i * 2) {
				x = data[k];
				y = w * data[k + i] % mod;
				data[k] = (x + y) % mod;
				data[k + i] = (x - y + mod) % mod;
			}
			w = w * wn % mod;
		}
	}
	if (is_intt) {
		inv = mod_pow(len, mod - 2);//Fermat定理
		for (i = 0; i < len; i++) data[i] = data[i] * inv % mod;
	}
	return 0;
}


int main()
{
	int i;
	int a[8] = { 1,0,1,1,0,0,0,1 };
	printf("\n");
	printf("\tNTT DATA:\t");
	for (i = 0; i < 8; i++)
	{
		printf("%d\t", a[i]);
	}
	printf("\n");
	printf("\tNTT result:\t");
	ntt(a, 8, 0);
	for (i = 0; i < 8; i++)
	{
		printf("%d\t", a[i]);
	}
	printf("\n");
	printf("\tINTT result:\t");
	ntt(a, 8, 1);
	for (i = 0; i < 8; i++)
	{
		printf("%d\t", a[i]);
	}
	printf("\n");
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
