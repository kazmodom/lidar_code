#pragma once
#include "standardlib.h"
//#include "Gauss_Fit.hpp"
//#include "gauss_Filter.h"

class CH
{
public:
	CH();
	~CH();

	uint64_t Find_Cursize(FILE* fp, uint64_t offset);    //��ȡһ��ͨ������ռ�ֽڴ�С
	uint64_t Read_CH_Data(FILE* fp, uint64_t address, bool& state);   //��ȡһ��ͨ���Ĳ�������

	uint16_t Get_CHL0();   //��ȡL0
	uint16_t Get_CHL1();   //��ȡL1
	uint16_t Get_CHD0(int i);  //��ȡ��i��λ�õ�D0����
	uint16_t* Get_CHD0();      //�������ط���D0����ĵ�ַ
	uint16_t Get_CHD1(int i);  //��ȡ��i��λ�õ�D1����
	uint16_t* Get_CHD1();      //�������ط���D1����ĵ�ַ
	uint16_t Get_CHS0();       //���S0
	uint16_t Get_CHS1();       //���S1
	uint32_t Get_D1_start();   //���D1_start
	uint32_t Get_D1_end();   //���D1_end
	double Get_CHmean();       //�����������50λ��ƽ��ֵ
	void Get_CHstd();        //�����������50λ�ķ���
	void Find_D1_start();              //Ѱ��D1�Ĳ�����ʼλ��
	void Find_D1_end();              //Ѱ��D1�Ĳ�����ֹλ��
	int Find_Max(uint16_t* DATA, int start, int end);     //����ָ�������ڵ����ֵ

	void Show_Process(uint64_t pos, uint64_t fsize, uint64_t curlength, int& num);
	void Delete();                   //�ͷſռ�

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
