#pragma once
#include "standardlib.h"
//#include "Gauss_Fit.hpp"
//#include "gauss_Filter.h"

class CH
{
public:
	CH();
	~CH();

	uint64_t Find_Cursize(FILE* fp, uint64_t offset);    //获取一个通道波形占字节大小
	uint64_t Read_CH_Data(FILE* fp, uint64_t address, bool& state);   //读取一个通道的波形数据

	uint16_t Get_CHL0();   //获取L0
	uint16_t Get_CHL1();   //获取L1
	uint16_t Get_CHD0(int i);  //获取第i个位置的D0数据
	uint16_t* Get_CHD0();      //函数重载返回D0数组的地址
	uint16_t Get_CHD1(int i);  //获取第i个位置的D1数据
	uint16_t* Get_CHD1();      //函数重载返回D1数组的地址
	uint16_t Get_CHS0();       //获得S0
	uint16_t Get_CHS1();       //获得S1
	uint32_t Get_D1_start();   //获得D1_start
	uint32_t Get_D1_end();   //获得D1_end
	double Get_CHmean();       //返回数据最后50位的平均值
	void Get_CHstd();        //返回数据最后50位的方差
	void Find_D1_start();              //寻找D1的波形起始位置
	void Find_D1_end();              //寻找D1的波形终止位置
	int Find_Max(uint16_t* DATA, int start, int end);     //查找指定区间内的最大值

	void Show_Process(uint64_t pos, uint64_t fsize, uint64_t curlength, int& num);
	void Delete();                   //释放空间

private:

	uint32_t Header;
	uint16_t ChannelNo;
	uint16_t S0;
	uint16_t L0;
	uint16_t* D0;
	uint16_t S1;
	uint16_t L1;
	uint16_t* D1;
	uint32_t D1_start;
	uint32_t D1_end;
	double CH_mean;
	double CH_std;

};
