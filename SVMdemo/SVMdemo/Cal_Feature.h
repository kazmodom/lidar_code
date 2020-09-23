#pragma once
#include "standardlib.h"

class CalFeature
{
public:
	CalFeature();
	~CalFeature();

	//�������ֵ,Flag=1,���Һ��Ⲩ�����ֵ,Flag=2,����ǳˮ�������ֵ
	void findMax(uint16_t* DATA, int32_t start, int32_t end, int32_t Flag, double Mean);
	//����ǳˮ����ƽ��ֵ
	void mean(uint16_t* DATA, int32_t Len);
	//��������,����ƽ��ֵ
	double mean(double* DATA, uint16_t Len);
	//����ǳˮ���η���
	void var(uint16_t* DATA, int32_t Len);

	//����ǳˮ���ο��
	void Cal_Wf(int32_t start, int32_t end);
	//�����һ�����������
	void Cal_Sa(uint16_t* DATA, int32_t start, int32_t end);
	//����ƫ�Ⱥͷ��
	void Cal_SkKu(uint16_t* DATA, int32_t start, int32_t end);
	//��������ǳˮ��ֵ��ֵ
	void Cal_RIG(uint16_t Imax, uint16_t Gmax);
	//������Ⲩ�λز����
	void Cal_Wir(uint32_t start, uint32_t end);
	//����������ˮͨ�����ο�ȱ�ֵ
	void Cal_RID(int32_t istart, int32_t iend, int32_t dstart, int32_t dend);
	//��ú���ͨ�����ֵ
	uint16_t Get_Imax();
	//���ǳˮͨ�����ֵ
	uint16_t Get_Gmax();
	//��ø�����������ֵ
	double Get_Wf();
	double Get_Sa();
	double Get_Sk();
	double Get_Ku();
	double Get_RIG();
	double Get_Wir();
	double Get_RID();
	//д���ļ�
	void Out_Data(FILE* fp, int Flag);

private:

	uint16_t Imax;   //���Ⲩ�����ֵ
	uint16_t Gmax;   //ǳˮ�������ֵ
	double Ave;      //ǳˮ����ƽ��ֵ
	double Std;      //ǳˮ���α�׼��
	double Wf;    //ǳˮ���ο��
	double Sa;    //��һ�����������
	double Sk;    //ƫ��
	double Ku;    //���
	double RIG;   //�����ǳˮ���η�ֵ��ֵ
	double Wir;   //���Ⲩ�ο��
	double RID;   //�������ˮͨͨ��ֵ
};
