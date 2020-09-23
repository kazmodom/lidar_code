#include "Read_binfile_Header.h"
#include "Change.hpp"

DataHeader::DataHeader()
{
	DataHeader::Header[0] = 0;
	DataHeader::Header[1] = 0;
	DataHeader::Header[2] = 0;
	DataHeader::Header[3] = 0;
	DataHeader::Header[4] = 0;
	DataHeader::Header[5] = 0;
	DataHeader::Header[6] = 0;
	DataHeader::Header[7] = 0;
	DataHeader::fill = 0;
	DataHeader::GPS_Week = 0;
	DataHeader::GPS_Second = 0.0;
	DataHeader::GPS_BreakdownTime = 0;
	DataHeader::Azimuth = 0.0;
	DataHeader::Pitch = 0.0;
	DataHeader::Roll = 0.0;
	DataHeader::X = 0.0;
	DataHeader::Y = 0.0;
	DataHeader::Z = 0.0;
	DataHeader::CodeDiscResolution = 0;
	DataHeader::CodeNumber = 0;
	DataHeader::WaveNumber = 0;
	DataHeader::WaveLen = 0;
}

DataHeader::~DataHeader()
{

}

uint64_t DataHeader::Find_Offset(FILE* fp, uint8_t GlobalSign[8])
{
	int i = 0;
	uint64_t offset = 0;
	uint32_t count = 0;

	_fseeki64(fp, 0, SEEK_SET);    //��λ�ڿ�ͷ
	fread(this->Header, sizeof(uint8_t), 8, fp);

	for (i = 0; i < 8; i++)
	{
		if (this->Header[i] == GlobalSign[i])
		{
			count++;
		}
	}
	if (count == 8)
	{
		offset = 0;
		return offset;
	}

	count = 0;

	while (count != 8 && offset <= 100000)    //ѭ����ͷ 
	{
		offset++;

		_fseeki64(fp, offset, SEEK_SET);
		fread(this->Header, sizeof(uint8_t), 8, fp);

		count = 0;
		for (i = 0; i < 8; i++)
		{
			if (this->Header[i] == GlobalSign[i])
			{
				count++;
			}
		}
	}

	if (offset > 100000)    //�Ҳ������� 
	{
		printf("Warning! Cannot Find File Header!\n");
		exit(1);
	}
	else
	{
		return offset;
	}
}

uint64_t DataHeader::Read_DataHeader(FILE* fp, uint64_t offset)
{
	uint32_t i = 0;
	uint32_t count = 0;
	uint64_t begin = 0;
	uint64_t pos = 0;
	Change C_DH;

	begin = offset;

	_fseeki64(fp, offset, SEEK_SET);    //��λ��ͷ�ļ���
	fread(this->Header, sizeof(uint8_t), 8, fp);
	fread(&this->fill, sizeof(uint16_t), 1, fp);
	this->fill = C_DH.CUint16(this->fill);
	fread(&this->GPS_Week, sizeof(uint16_t), 1, fp);
	this->GPS_Week = C_DH.CUint16(this->GPS_Week);
	fread(&this->GPS_Second, sizeof(double), 1, fp);
	this->GPS_Second = C_DH.Cdouble64(this->GPS_Second);
	fread(&this->GPS_BreakdownTime, sizeof(uint32_t), 1, fp);
	this->GPS_BreakdownTime = C_DH.CUint32(this->GPS_BreakdownTime);
	fread(&this->Azimuth, sizeof(double), 1, fp);
	this->Azimuth = C_DH.Cdouble64(this->Azimuth);
	fread(&this->Pitch, sizeof(double), 1, fp);
	this->Pitch = C_DH.Cdouble64(this->Pitch);
	fread(&this->Roll, sizeof(double), 1, fp);
	this->Roll = C_DH.Cdouble64(this->Roll);
	fread(&this->X, sizeof(double), 1, fp);
	this->X = C_DH.Cdouble64(this->X);
	fread(&this->Y, sizeof(double), 1, fp);
	this->Y = C_DH.Cdouble64(this->Y);
	fread(&this->Z, sizeof(double), 1, fp);
	this->Z = C_DH.Cdouble64(this->Z);
	fread(&this->CodeDiscResolution, sizeof(uint32_t), 1, fp);
	this->CodeDiscResolution = C_DH.CUint32(this->CodeDiscResolution);
	fread(&this->CodeNumber, sizeof(uint32_t), 1, fp);
	this->CodeNumber = C_DH.CUint32(this->CodeNumber);
	fread(&this->WaveNumber, sizeof(uint32_t), 1, fp);
	this->WaveNumber = C_DH.CUint32(this->WaveNumber);
	fread(&this->WaveLen, sizeof(uint32_t), 1, fp);
	this->WaveLen = C_DH.CUint32(this->WaveLen);

	_fseeki64(fp, 0, SEEK_CUR);  //��λ���ļ���ǰλ��

	pos = _ftelli64(fp);

	return pos;
}

double DataHeader::Get_GPS_Second()
{
	return 1.0 * this->GPS_Second;
}

double DataHeader::Get_GPS_BreakdownTime()
{
	return 1.0 * this->GPS_BreakdownTime;
}

double DataHeader::Get_CodeNumber()
{
	return 1.0 * this->CodeNumber;
}

/*void DataHeader::WGS84XYZ2BLH(double &B, double &L, double &H)
{
	double PI = 3.1415926535;
	double rad = 180.0 / PI;

	//��Բ������
	double a = 6378137.0;
	//WGS84�������
	double f = 1.0 / 298.257223563;
	//��Բ�̰���
	double b = a * (1.0 - f);
	//�����һƫ����
	double e1 = sqrt(1.0 - (b * b) / (a * a));
	//����ڶ�ƫ����
	double e2 = sqrt((a * a) / (b * b) - 1.0);
	double theta = atan(this->Z * a / sqrt(this->X * this->X + this->Y * this->Y) / b);

	L = atan(this->Y / this->X) * rad;
	if (L < 0)
	{
		L += 180.0;
	}

	double sin3theta = sin(theta) * sin(theta) * sin(theta);
	double cos3theta = cos(theta) * cos(theta) * cos(theta);

	B = atan((this->Z + e2 * e2 * b * sin3theta) / (sqrt(this->X * this->X + this->Y * this->Y) - e1 * e1 * a * cos3theta)) * rad;

	double N = a / sqrt(1.0 - e1 * e1 * sin(B / rad) * sin(B / rad));

	H = sqrt(this->X * this->X + this->Y * this->Y) / cos(B / rad) - N;

	if (B < 0)
	{
		B += 90.0;
	}
}*/

double DataHeader::Get_X()
{
	return this->X;
}

double DataHeader::Get_Y()
{
	return this->Y;
}

void DataHeader::Get_FILE_Message(FILE* fp, uint8_t GlobalSign[8], uint64_t& offset, uint64_t& pos,
	uint64_t& cursize, uint64_t& curlength, uint64_t& fsize, CH& CH1)
{
	_fseeki64(fp, 0, SEEK_END);
	fsize = _ftelli64(fp);

	offset = Find_Offset(fp, GlobalSign);//��ȡ����ͷƫ���ļ�λ��

	pos = Read_DataHeader(fp, offset); //��ȡ��һ������ƫ���ļ�λ��

	cursize = CH1.Find_Cursize(fp, pos); //��ȡһ��ͨ���Ĳ�����ռ�ֽڴ�С

	curlength = 4 * cursize + (uint64_t)88;  //�����¼һ�������ε��ֽڴ�С(һ������=4��ͨ���Ĳ���+����ͷ)
}

void DataHeader::Print_FILE_Message(const char* INPUT, uint64_t offset, uint64_t fsize, uint64_t curlength)
{
	cout << "/////////////////////////////////////////////////////////////////////////////" << endl;
	cout << "/" << "     �ļ���·����" << INPUT << endl;
	cout << "/" << "     �ļ��Ĵ�С��" << fsize << endl;
	cout << "/" << "     ����ͷƫ�ƣ�" << offset << endl;
	cout << "/" << "     һ�������ֽڴ�С(���D1�����ݻ�С�ڴ���)��" << curlength << endl;
	cout << "/////////////////////////////////////////////////////////////////////////////" << endl;

	cout << endl;
}