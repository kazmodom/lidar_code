#include "Cal_Feature.h"

CalFeature::CalFeature()
{
	CalFeature::Imax = 0;
	CalFeature::Gmax = 0;
	CalFeature::Ave = 0.0;
	CalFeature::Std = 0.0;
	CalFeature::Wf = 0.0;
	CalFeature::Sa = 0.0;
	CalFeature::Sk = 0.0;
	CalFeature::Ku = 0.0;
	CalFeature::RIG = 0.0;
	CalFeature::Wir = 0.0;
	CalFeature::RID = 0.0;
}

CalFeature::~CalFeature()
{

}

void CalFeature::findMax(uint16_t* DATA, int32_t start, int32_t end, int32_t Flag, double Mean)
{
	uint32_t location = 0;
	uint16_t wavemax = DATA[start];
	for (int i = start; i < end; i++)
	{
		wavemax = max(DATA[i], wavemax);
		if ((DATA[i] >= wavemax))
		{
			location = i;
		}
	}

	if (Flag == 1)
	{
		this->Imax = DATA[location] - Mean;
	}
	else if (Flag == 2)
	{
		this->Gmax = DATA[location] - Mean;
	}
}

void CalFeature::mean(uint16_t* DATA, int32_t Len)
{
	double sum = 0.0;
	int i = 0;

	for (i = 0; i < Len; i++)
	{
		sum += DATA[i];
	}

	this->Ave = 1.0 * sum / Len;
}

double CalFeature::mean(double* DATA, uint16_t Len)
{
	double sum = 0.0;
	int i = 0;

	for (i = 0; i < Len; i++)
	{
		sum += DATA[i];
	}

	sum = 1.0 * sum / Len;

	return sum;
}

void CalFeature::var(uint16_t* DATA, int32_t Len)
{
	int i = 0;
	double sum_std = 0.0;
	for (i = 0; i < Len; i++)
	{
		sum_std += (DATA[i] - this->Ave) * (DATA[i] - this->Ave);
	}
	this->Std = sqrt(sum_std / Len);
}

void CalFeature::Cal_Wf(int32_t start, int32_t end)
{
	this->Wf = 1.0 * (end - start + 1);
	if (end - start <= 0)
	{
		this->Wf = 0.0;
	}
}

void CalFeature::Cal_Sa(uint16_t* DATA, int32_t start, int32_t end)
{
	int i;
	int* x;
	int* xx;
	int length;
	double* y;
	double* yy;
	double area = 0;

	length = end - start + 1;

	x = new int[length];
	xx = new int[length];
	y = new double[length];
	yy = new double[length];

	findMax(DATA, start, end, 2, 0.0);

	for (i = start; i <= end; i++)
	{
		x[i - start] = i;
		y[i - start] = 1.0 * DATA[i] / this->Gmax;
	}
	for (i = 0; i < length - 1; i++)
	{
		xx[i] = x[i + 1] - x[i];
		yy[i] = y[i] + y[i + 1];
	}
	xx[length - 1] = x[0] - x[length - 1];
	yy[length - 1] = y[0] + y[length - 1];

	for (i = 0; i < length; i++)
	{
		area = 1.0 * xx[i] * yy[i] + area;
	}

	if (area < 0)
	{
		area = -area;
	}
	delete[]x;
	delete[]xx;
	delete[]y;
	delete[]yy;
	this->Sa = area / 2;
}

void CalFeature::Cal_SkKu(uint16_t* DATA, int32_t start, int32_t end)
{
	if (end <= start)
	{
		this->Sk = 0.0;
		this->Ku = 0.0;
		return;
	}
	int i;
	double* temp;
	double* x0, * s, * m, * n;
	double s2 = 0.0, m3 = 0.0, m4 = 0.0;
	unsigned int length = end - start + 1;
	x0 = new double[length];
	s = new double[length];
	m = new double[length];
	n = new double[length];
	temp = new double[length];

	for (i = start; i < end + 1; i++)
	{
		temp[i - start] = DATA[i];
	}

	for (i = start; i < end + 1; i++)
	{
		x0[i - start] = DATA[i] - this->Ave;
		s[i - start] = pow(x0[i - start], 2);
		m[i - start] = pow(x0[i - start], 3);
		n[i - start] = pow(x0[i - start], 4);
	}
	s2 = mean(s, length);
	m3 = mean(m, length);
	m4 = mean(n, length);

	this->Sk = m3 / pow(s2, 1.5);
	this->Ku = m4 / pow(s2, 2);

	delete[]x0;
	delete[]s;
	delete[]m;
	delete[]n;
	delete[]temp;
}
//计算红外和浅水峰值比值
void CalFeature::Cal_RIG(uint16_t Imax, uint16_t Gmax)
{
	this->RIG = 1.0 * Imax / Gmax;
}
//计算红外波段回波宽度
void CalFeature::Cal_Wir(uint32_t start, uint32_t end)
{
	this->Wir = end - start + 1;
	if (this->Wir - 1 <= 0 || this->Wir > 200)
	{
		this->Wir = 0.0;
	}
}

void CalFeature::Cal_RID(int32_t istart, int32_t iend, int32_t dstart, int32_t dend)
{
	if (iend <= istart || iend - istart >=300)
	{
		this->RID = 0.0;
		return;
	}
	
	this->RID = 1.0 * (iend - istart) / (dend - dstart);
}

uint16_t CalFeature::Get_Imax()
{
	return this->Imax;
}

uint16_t CalFeature::Get_Gmax()
{
	return this->Gmax;
}

double CalFeature::Get_Wf()
{
	return this->Wf;
}

double CalFeature::Get_Sa()
{
	return this->Sa;
}

double CalFeature::Get_Sk()
{
	return this->Sk;
}

double CalFeature::Get_Ku()
{
	return this->Ku;
}

double CalFeature::Get_RIG()
{
	return this->RIG;
}

double CalFeature::Get_RID()
{
	return this->RID;
}

double CalFeature::Get_Wir()
{
	return this->Wir;
}



void CalFeature::Out_Data(FILE* fp, int Flag)
{
	//Flag=1代表陆地
	if (Flag == 1)
	{
		fprintf(fp, "%d %d:%f %d:%f %d:%f %d:%f %d:%f %d:%f\n", 1, 1, this->Wf, 2, this->RID, 3, this->Sk, 4, this->Ku, 5, this->RIG, 6, this->Wir);
	}
	//Flag=-1代表海洋
	else if (Flag == -1)
	{
		fprintf(fp, "%d %d:%f %d:%f %d:%f %d:%f %d:%f %d:%f\n", -1, 1, this->Wf, 2, this->RID, 3, this->Sk, 4, this->Ku, 5, this->RIG, 6, this->Wir);
	}
}
