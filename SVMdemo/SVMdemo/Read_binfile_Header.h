#pragma once
#include "standardlib.h"
#include "Read_CH_Data.h"

class CH;
class DataHeader
{
public:
	DataHeader();
	~DataHeader();

	uint64_t Find_Offset(FILE* fp, uint8_t GlobalSign[8]);
	uint64_t Read_DataHeader(FILE* fp, uint64_t offset);

	double Get_GPS_Second();
	double Get_GPS_BreakdownTime();
	double Get_CodeNumber();
	double Get_X();
	double Get_Y();
	//XYZ转成经纬高
	//void WGS84XYZ2BLH(double &B, double &L, double &H);
	void Get_FILE_Message(FILE* fp, uint8_t GlobalSign[8], uint64_t& offset, uint64_t& pos, 
		uint64_t& cursize, uint64_t& curlength, uint64_t& fsize, CH& CH1);
	void Print_FILE_Message(const char* INPUT, uint64_t offset, uint64_t fsize, uint64_t curlength);

private:
	uint8_t Header[8];
	uint16_t fill;
	uint16_t GPS_Week;
	double GPS_Second;
	uint32_t GPS_BreakdownTime;
	double Azimuth;
	double Pitch;
	double Roll;
	double X;
	double Y;
	double Z;
	uint32_t CodeDiscResolution;
	uint32_t CodeNumber;
	uint32_t WaveNumber;
	uint32_t WaveLen;
};