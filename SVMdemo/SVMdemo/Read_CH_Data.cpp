#include "Read_CH_Data.h"
#include "Change.hpp"

CH::CH()
{
	CH::Header = 0;
	CH::ChannelNo = 0;
	CH::S0 = 0;
	CH::L0 = 0;
	CH::D0 = NULL;
	CH::S1 = 0;
	CH::L1 = 0;
	CH::D1 = NULL;
	CH::D1_start = 0;
	CH::D1_end = 0;
	CH::CH_mean = 0.0;
	CH::CH_std = 0.0;
}

CH::~CH()
{

}

uint64_t CH::Find_Cursize(FILE* fp, uint64_t offset)
{
	int i = 0;
	uint64_t pos = 0;
	uint64_t cursize = 0;
	Change FC;

	_fseeki64(fp, offset, SEEK_SET);

	//------------------------------------------------------------------------
	//读通道1数据
	fread(&this->Header, sizeof(uint32_t), 1, fp);
	fread(&this->ChannelNo, sizeof(uint16_t), 1, fp);
	this->ChannelNo = FC.CUint16(this->ChannelNo);
	fread(&this->S0, sizeof(uint16_t), 1, fp);
	this->S0 = FC.CUint16(this->S0);
	fread(&this->L0, sizeof(uint16_t), 1, fp);
	this->L0 = FC.CUint16(this->L0);
	this->D0 = new uint16_t[this->L0];
	for (i = 0; i < this->L0; i++)
	{
		fread(&this->D0[i], sizeof(uint16_t), 1, fp);
		this->D0[i] = FC.CUint16(this->D0[i]);
	}
	fread(&this->S1, sizeof(uint16_t), 1, fp);
	this->S1 = FC.CUint16(this->S1);
	fread(&this->L1, sizeof(uint16_t), 1, fp);
	this->L1 = FC.CUint16(this->L1);
	this->D1 = new uint16_t[this->L1];
	for (i = 0; i < this->L1; i++)
	{
		fread(&this->D1[i], sizeof(uint16_t), 1, fp);
		this->D1[i] = FC.CUint16(this->D1[i]);
	}

	_fseeki64(fp, 0, SEEK_CUR);
	pos = _ftelli64(fp);     //找到当前指针位置
//---------------------------------------------------------------
//计算一个通道数据的长度(包含四个通道)
	cursize = pos - offset;

	return cursize;
}

uint64_t CH::Read_CH_Data(FILE* fp, uint64_t address, bool& state)
{
	uint64_t begin = 0;
	uint64_t pos = 0;
	uint32_t Temp_Header = 0;

	Change C_CH;

	begin = address;

	_fseeki64(fp, begin, SEEK_SET);


	fread(&this->Header, sizeof(uint32_t), 1, fp);

	if (this->Header == 1520799979)
	{
		fread(&this->ChannelNo, sizeof(uint16_t), 1, fp);
		this->ChannelNo = C_CH.CUint16(this->ChannelNo);
		fread(&this->S0, sizeof(uint16_t), 1, fp);
		this->S0 = C_CH.CUint16(this->S0);
		fread(&this->L0, sizeof(uint16_t), 1, fp);
		this->L0 = C_CH.CUint16(this->L0);
		this->D0 = new uint16_t[this->L0];
		for (int i = 0; i < this->L0; i++)
		{
			fread(&this->D0[i], sizeof(uint16_t), 1, fp);
			this->D0[i] = C_CH.CUint16(this->D0[i]);
		}

		//判断是否读取D1数据
		fread(&Temp_Header, sizeof(uint32_t), 1, fp);
		if (Temp_Header == 1520799979 || Temp_Header == 1732584193)
		{
			state = false;
			_fseeki64(fp, -4, SEEK_CUR);
			pos = _ftelli64(fp);
			return pos;
		}
		else
		{
			state = true;
			_fseeki64(fp, -4, SEEK_CUR);
			//读取this->D1数据
			fread(&this->S1, sizeof(uint16_t), 1, fp);
			this->S1 = C_CH.CUint16(this->S1);
			fread(&this->L1, sizeof(uint16_t), 1, fp);
			this->L1 = C_CH.CUint16(this->L1);
			this->D1 = new uint16_t[this->L1];
			for (int i = 0; i < this->L1; i++)
			{
				fread(&this->D1[i], sizeof(uint16_t), 1, fp);
				this->D1[i] = C_CH.CUint16(this->D1[i]);
			}
		}
	}

	_fseeki64(fp, 0, SEEK_CUR);  //定位到文件当前位置

	pos = _ftelli64(fp);

	return pos;
}

uint16_t CH::Get_CHL0()
{
	return this->L0;
}

uint16_t* CH::Get_CHD0()
{
	return this->D0;
}

uint16_t CH::Get_CHL1()
{
	return this->L1;
}

uint16_t* CH::Get_CHD1()
{
	return this->D1;
}

uint16_t CH::Get_CHD0(int i)
{
	return this->D0[i];
}

uint16_t CH::Get_CHD1(int i)
{
	return this->D1[i];
}

uint16_t CH::Get_CHS0()
{
	return this->S0;
}

uint16_t CH::Get_CHS1()
{
	return this->S1;
}

double CH::Get_CHmean()
{
	uint16_t sum = 0;
	for (int i = 0; i < 30; i++)
	{
		sum += this->D1[Get_CHL1() - i - 1];
	}

	this->CH_mean = 1.0 * sum / 30;

	return this->CH_mean;
}

void CH::Get_CHstd()
{
	double sum = 0;
	for (int i = 0; i < 30; i++)
	{
		sum += (this->D1[Get_CHL1() - i - 1] - this->CH_mean) * (this->D1[Get_CHL1() - i - 1] - this->CH_mean);
	}

	this->CH_std = sqrt(sum / 29);
}

uint32_t CH::Get_D1_start()
{
	return this->D1_start;
}

uint32_t CH::Get_D1_end()
{
	return this->D1_end;
}

void CH::Find_D1_start()
{
	int i = 0;
	int count = 0;
	//设置阈值
	//int THRESHOLD = 10;

	Get_CHmean();
	Get_CHstd();

	//设定平均值+5倍标准差作为起始门槛(CH2)
	double threshold_CH = this->CH_mean + 5 * this->CH_std;

	for (i = 0; i < this->L1 - 50; i++)
	{
		if (this->D1[i] >= threshold_CH)
		{
			count++;
			if (count == 3)
			{
				break;
			}
		}
		else
		{
			count = 0;
		}
	}

	this->D1_start = i - 3;
	//如果没找到，降低门槛到平均值+3倍标准差
	/*if (i == this->L1 - 50)
	{
		threshold_CH = this->CH_mean + 3 * this->CH_std;
		for (i = 0; i < this->L1 - 50; i++)
		{
			if (this->D1[i] >= threshold_CH && this->D1[i + 1] >= threshold_CH
				&& this->D1[i + 1] - this->D1[i] > THRESHOLD)
			{
				this->D1_start = i;
				break;
			}
		}
	}*/

	//cout << "this->D1_start= " << this->D1_start << endl;
}

void CH::Find_D1_end()
{
	int i = 0;
	int count = 0;
	//设定平均值+5倍标准差作为起始门槛(CH2) 
	double threshold_CH = this->CH_mean + 5 * this->CH_std;

	for (i = this->L1 - 50; i >= 0; i--)
	{
		if (this->D1[i] >= threshold_CH)
		{
			count++;
			if (count == 5)
			{
				break;
			}
		}
		else
		{
			count = 0;
		}
	}

	this->D1_end = i + 5;
	//cout << "this->D1_end= " << this->D1_end << endl;
}

int CH::Find_Max(uint16_t* DATA, int start, int end)
{
	int location = 0;
	uint16_t wavemax = DATA[start];
	for (int i = start; i < end; i++)
	{
		wavemax = max(DATA[i], wavemax);
		if ((DATA[i] >= wavemax))
		{
			location = i;
		}
	}

	return location;
}


void CH::Show_Process(uint64_t pos, uint64_t fsize, uint64_t curlength, int& num)
{
	double process = 0.0;
	process = 100.0 * pos / fsize;

	if (process >= num)
	{
		cout << "处理进度：" << process << "%" << endl;
		num += 10;
	}
	else if (fsize - pos < curlength)
	{
		cout << "处理进度：100%" << endl;
	}
}

void CH::Delete()
{
	delete[] this->D0;
	this->D0 = NULL;
	if (D1 != NULL)
	{
		delete[] this->D1;
		this->D1 = NULL;
	}
}
