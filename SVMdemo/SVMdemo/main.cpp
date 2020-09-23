#include "main.h"

//TESTOPTION = 1 ʱ�����ļ���·��
//const char* LANDINPUT = "K:\\��Ȫ����������\\��½����ѵ����\\LAND.bin";       //½��
//const char* OCEANINPUT1 = "K:\\��Ȫ����������\\��½����ѵ����\\OCEAN.bin";     //����
//const char* TRAINPATH = "K:\\��Ȫ����������\\��½����ѵ����\\train.txt";       //ѵ������
//const char* MODELPATH = "K:\\��Ȫ����������\\��½����ѵ����\\model.txt";       //����ģ��
//const char* PREDPATH = "K:\\��Ȫ����������\\��½����ѵ����\\predict.txt";        //Ԥ����

//TESTOPTION = 2 ʱ�����ļ���·��
const char* LANDINPUT = "K:\\������������\\�������స��\\LAND.bin";       //½��
const char* OCEANINPUT1 = "K:\\������������\\�������స��\\OCEAN.bin";     //����
const char* INPUT = "K:\\������������\\��������4\\588823_588919.bin";
const char* TRAINPATH = "K:\\������������\\�������స��\\train.txt";       //ѵ������
const char* MYTRAIN = "K:\\������������\\��������4\\mytrain.txt";
const char* MODELPATH = "K:\\������������\\�������స��\\model.txt";       //����ģ��
const char* PREDPATH = "K:\\������������\\��������4\\predict.txt";

int main()
{
	int i = 0;
	int j = 0;
	int count = 0;
	int count1 = 0;
	int count2 = 0;
	int count3 = 0;
	int num = 10;

	bool States = true;   //�����ļ���ȡ״̬
	uint64_t fsize = 0;   //�ļ���С
	uint64_t offset = 0;     //����ͷ����ļ�ͷ��ƫ��
	uint64_t cursize = 0;    //һ��ͨ���Ĳ�����ռ�ֽڴ�С
	uint64_t curlength = 0;  //һ�������ε��ֽڴ�С
	uint64_t pos = 0;        //�����ļ�ָ��λ��
	uint64_t wavestart_pos = 0;   //��¼���ο�ʼָ��
	DataHeader DH;
	uint8_t GlobalSign[8] = { 1,35,69,103,137,171,205,239 };    //�ļ�ͷ��ʶ��
	CH CH1;
	CH CH2;
	CH CH3;
	CH CH4;

	CalFeature CF;

	int TESTOPTION = 2;   //����ѡ��������1����ѡ����ѵ�������� �������2����ѡ�����Լ�������ѵ�������� 

	//��ѵ�������ļ�
	FILE* fTRAIN = fopen(TRAINPATH, "wt");
	if (fTRAIN == NULL)
	{
		cout << "Cannot Open TrainFile!" << endl;
		exit(1);
	}
	//���Լ���ѵ�������ļ�
	FILE* fMYTRAIN = fopen(MYTRAIN, "wt");
	if (fMYTRAIN == NULL)
	{
		cout << "Cannot Open TrainFile!" << endl;
		exit(1);
	}

	//����½���ļ�
	//-------------------------------------------------------------------------------------------------------
	FILE* fLAND = fopen(LANDINPUT, "rb");
	if (fLAND == NULL)
	{
		cout << "Cannot Open InputFile!" << endl;
		exit(1);
	}

	//��ȡ�����ļ���Ϣ����ӡ
	DH.Get_FILE_Message(fLAND, GlobalSign, offset, pos, cursize, curlength, fsize, CH1);
	DH.Print_FILE_Message(LANDINPUT, offset, fsize, curlength);

	pos = offset;

	while (fsize - pos > curlength)
	{
		pos = DH.Read_DataHeader(fLAND, pos);
		pos = CH1.Read_CH_Data(fLAND, pos, States);
		pos = CH2.Read_CH_Data(fLAND, pos, States);
		pos = CH3.Read_CH_Data(fLAND, pos, States);
		pos = CH4.Read_CH_Data(fLAND, pos, States);
		if (States == false)
		{
			continue;
		}
		else
		{
			count1++;
		}

		//���㲨������
		CH1.Find_D1_start();
		CH1.Find_D1_end();
		CH2.Find_D1_start();
		CH2.Find_D1_end();
		CH4.Find_D1_start();
		CH4.Find_D1_end();
		CF.Cal_Wf(CH2.Get_D1_start(), CH2.Get_D1_end());
		//CF.Cal_Sa(CH2.Get_CHD1(), CH2.Get_D1_start(), CH2.Get_D1_end());
		CF.Cal_SkKu(CH2.Get_CHD1(), CH2.Get_D1_start(), CH2.Get_D1_end());
		CF.Cal_Wir(CH1.Get_D1_start(), CH1.Get_D1_end());

		CF.findMax(CH1.Get_CHD1(), CH2.Get_D1_start(), CH2.Get_D1_end(), 1, CH1.Get_CHmean());
		CF.findMax(CH2.Get_CHD1(), CH2.Get_D1_start(), CH2.Get_D1_end(), 2, CH2.Get_CHmean());
		CF.Cal_RIG(CF.Get_Imax(), CF.Get_Gmax());

		CF.Cal_RID(CH1.Get_D1_start(), CH1.Get_D1_end(), CH4.Get_D1_start(), CH4.Get_D1_end());

		//���������

		CF.Out_Data(fTRAIN, 1);

		//��ʾ����
		CH2.Show_Process(pos, fsize, curlength, num);
		CH1.Delete();
		CH2.Delete();
		CH3.Delete();
		CH4.Delete();
	}
	fclose(fLAND);
	//-------------------------------------------------------------------------------------------------------
	//�������ļ�
	//-------------------------------------------------------------------------------------------------------
	num = 10;    //���ô������ֵ
	FILE* fSHALLOW = fopen(OCEANINPUT1, "rb");
	if (fSHALLOW == NULL)
	{
		cout << "Cannot Open InputFile!" << endl;
		exit(1);
	}

	//��ȡ�����ļ���Ϣ����ӡ
	DH.Get_FILE_Message(fSHALLOW, GlobalSign, offset, pos, cursize, curlength, fsize, CH1);
	DH.Print_FILE_Message(OCEANINPUT1, offset, fsize, curlength);

	pos = offset;

	while (fsize - pos > curlength)
	{
		pos = DH.Read_DataHeader(fSHALLOW, pos);
		pos = CH1.Read_CH_Data(fSHALLOW, pos, States);
		pos = CH2.Read_CH_Data(fSHALLOW, pos, States);
		pos = CH3.Read_CH_Data(fSHALLOW, pos, States);
		pos = CH4.Read_CH_Data(fSHALLOW, pos, States);
		if (States == false)
		{
			continue;
		}
		else
		{
			count2++;
		}

		//���㲨������
		CH1.Find_D1_start();
		CH1.Find_D1_end();
		CH2.Find_D1_start();
		CH2.Find_D1_end();
		CH4.Find_D1_start();
		CH4.Find_D1_end();
		CF.Cal_Wf(CH2.Get_D1_start(), CH2.Get_D1_end());
		//CF.Cal_Sa(CH2.Get_CHD1(), CH2.Get_D1_start(), CH2.Get_D1_end());
		CF.Cal_SkKu(CH2.Get_CHD1(), CH2.Get_D1_start(), CH2.Get_D1_end());
		CF.Cal_Wir(CH1.Get_D1_start(), CH1.Get_D1_end());

		CF.findMax(CH1.Get_CHD1(), CH2.Get_D1_start(), CH2.Get_D1_end(), 1, CH1.Get_CHmean());
		CF.findMax(CH2.Get_CHD1(), CH2.Get_D1_start(), CH2.Get_D1_end(), 2, CH2.Get_CHmean());
		CF.Cal_RIG(CF.Get_Imax(), CF.Get_Gmax());

		CF.Cal_RID(CH1.Get_D1_start(), CH1.Get_D1_end(), CH4.Get_D1_start(), CH4.Get_D1_end());

		//���������

		CF.Out_Data(fTRAIN, -1);

		//��ʾ����
		CH2.Show_Process(pos, fsize, curlength, num);
		CH1.Delete();
		CH2.Delete();
		CH3.Delete();
		CH4.Delete();
	}
	fclose(fSHALLOW);
	fclose(fTRAIN);
	//-------------------------------------------------------------------------------------------------------
	//���������ļ�
	//-------------------------------------------------------------------------------------------------------
	num = 10;    //���ô������ֵ
	FILE* fTEST = fopen(INPUT, "rb");
	if (fTEST == NULL)
	{
		cout << "Cannot Open InputFile!" << endl;
		exit(1);
	}

	//��ȡ�����ļ���Ϣ����ӡ
	DH.Get_FILE_Message(fTEST, GlobalSign, offset, pos, cursize, curlength, fsize, CH1);
	DH.Print_FILE_Message(INPUT, offset, fsize, curlength);

	pos = offset;
	//pos = 1433199778;

	while (fsize - pos > curlength)
	{
		pos = DH.Read_DataHeader(fTEST, pos);
		pos = CH1.Read_CH_Data(fTEST, pos, States);
		pos = CH2.Read_CH_Data(fTEST, pos, States);
		pos = CH3.Read_CH_Data(fTEST, pos, States);
		pos = CH4.Read_CH_Data(fTEST, pos, States);
		if (States == false)
		{
			continue;
		}
		else
		{
			count3++;
		}

		//���㲨������
		CH1.Find_D1_start();
		CH1.Find_D1_end();
		CH2.Find_D1_start();
		CH2.Find_D1_end();
		CH4.Find_D1_start();
		CH4.Find_D1_end();
		CF.Cal_Wf(CH2.Get_D1_start(), CH2.Get_D1_end());
		//CF.Cal_Sa(CH2.Get_CHD1(), CH2.Get_D1_start(), CH2.Get_D1_end());
		CF.Cal_SkKu(CH2.Get_CHD1(), CH2.Get_D1_start(), CH2.Get_D1_end());
		CF.Cal_Wir(CH1.Get_D1_start(), CH1.Get_D1_end());

		//���������û�лز�ֱ�ӽ�����Ϊ0
		if (CF.Get_Wir() == 0.0)
		{
			CF.Cal_RIG(0, 1);
		}
		else
		{
			CF.findMax(CH1.Get_CHD1(), CH2.Get_D1_start(), CH2.Get_D1_end(), 1, CH1.Get_CHmean());
			CF.findMax(CH2.Get_CHD1(), CH2.Get_D1_start(), CH2.Get_D1_end(), 2, CH2.Get_CHmean());
			CF.Cal_RIG(CF.Get_Imax(), CF.Get_Gmax());
		}
		
		CF.Cal_RID(CH1.Get_D1_start(), CH1.Get_D1_end(), CH4.Get_D1_start(), CH4.Get_D1_end());

		//���������

		CF.Out_Data(fMYTRAIN, -1);

		//��ʾ����
		CH2.Show_Process(pos, fsize, curlength, num);
		CH1.Delete();
		CH2.Delete();
		CH3.Delete();
		CH4.Delete();

		//cout << "pos = " << pos << endl;
	}
	fclose(fTEST);
	fclose(fMYTRAIN);

	//��ѵ��������ģ�� 

	//count = count1 + count2 + count3;
	count = count1 + count2;
	int ELEM = 7;   //��6������ֵ�������7�����һλ��-1�������

	cout << "��ʼѵ��������ģ�ͣ�" << endl;
	model = SVMTRAIN(count, ELEM, TRAINPATH, MODELPATH);

	//----------------------------------------------------
//   TESTOPTION=1��ѵ���������Լ�
//   TESTOPTION=2���Լ�������ѵ���������Լ� 
	if (TESTOPTION == 1)
	{
		cout << "��ѡ����ѵ������Ϊ���Լ���" << endl;
		SVMPREDICT(TRAINPATH, PREDPATH, MODELPATH);
	}
	else if (TESTOPTION == 2)
	{
		cout << "��ѡ�����Լ�������ѵ������Ϊ���Լ���" << endl;
		SVMPREDICT(MYTRAIN, PREDPATH, MODELPATH);
	}

	return 0;
}