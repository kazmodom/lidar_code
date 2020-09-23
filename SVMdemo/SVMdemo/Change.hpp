#pragma once
#include "standardlib.h"

class Change
{

public:
	Change();
	~Change();

	//进行数据大小端转换 
	uint16_t CUint16(uint16_t value);
	uint32_t CUint32(uint32_t value);
	double Cdouble64(double value);

private:

};

inline Change::Change()
{

}

inline Change::~Change()
{

}



//----------------------------------------------------------------------
//uint16_t型数据大小端转换 

inline uint16_t Change::CUint16(uint16_t value)
{
	return ((value & 0x00FF) << 8) | ((value & 0xFF00) >> 8);
}
//----------------------------------------------------------------------
//uint32_t型数据大小端转换

inline uint32_t Change::CUint32(uint32_t value)
{
	return ((value & 0x000000FF) << 24) | ((value & 0x0000FF00) << 8) |
		((value & 0x00FF0000) >> 8) | ((value & 0xFF000000) >> 24);
}

//-----------------------------------------------------------------------
//double型数据大小端转换 

typedef union DOUBLE_CONV
{
	double f;
	char c[8];
}double_conv;


inline double Change::Cdouble64(double value)
{
	double_conv d1, d2;
	d1.f = value;
	d2.c[0] = d1.c[7];
	d2.c[1] = d1.c[6];
	d2.c[2] = d1.c[5];
	d2.c[3] = d1.c[4];
	d2.c[4] = d1.c[3];
	d2.c[5] = d1.c[2];
	d2.c[6] = d1.c[1];
	d2.c[7] = d1.c[0];
	return d2.f;
}
//------------------------------------------------------------


