#pragma once
#include "standardlib.h"

class CalFeature
{
public:
	CalFeature();
	~CalFeature();

	//查找最大值,Flag=1,查找红外波形最大值,Flag=2,查找浅水波段最大值
	void findMax(uint16_t* DATA, int32_t start, int32_t end, int32_t Flag, double Mean);
	//计算浅水波形平均值
	void mean(uint16_t* DATA, int32_t Len);
	//函数重载,计算平均值
	double mean(double* DATA, uint16_t Len);
	//计算浅水波形方差
	void var(uint16_t* DATA, int32_t Len);

	//计算浅水波形宽度
	void Cal_Wf(int32_t start, int32_t end);
	//计算归一化曲线下面积
	void Cal_Sa(uint16_t* DATA, int32_t start, int32_t end);
	//计算偏度和峰度
	void Cal_SkKu(uint16_t* DATA, int32_t start, int32_t end);
	//计算红外和浅水峰值比值
	void Cal_RIG(uint16_t Imax, uint16_t Gmax);
	//计算红外波段回波宽度
	void Cal_Wir(uint32_t start, uint32_t end);
	//计算红外和深水通道波形宽度比值
	void Cal_RID(int32_t istart, int32_t iend, int32_t dstart, int32_t dend);
	//获得红外通道最大值
	uint16_t Get_Imax();
	//获得浅水通道最大值
	uint16_t Get_Gmax();
	//获得各种特征的数值
	double Get_Wf();
	double Get_Sa();
	double Get_Sk();
	double Get_Ku();
	double Get_RIG();
	double Get_Wir();
	double Get_RID();
	//写入文件
	void Out_Data(FILE* fp, int Flag);

private:

	uint16_t Imax;   //红外波形最大值
	uint16_t Gmax;   //浅水波形最大值
	double Ave;      //浅水波形平均值
	double Std;      //浅水波形标准差
	double Wf;    //浅水波形宽度
	double Sa;    //归一化曲线下面积
	double Sk;    //偏度
	double Ku;    //峰度
	double RIG;   //红外和浅水波段峰值比值
	double Wir;   //红外波段宽度
	double RID;   //红外和深水通通比值
};
