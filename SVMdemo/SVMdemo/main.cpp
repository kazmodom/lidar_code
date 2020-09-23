#include "main.h"

//TESTOPTION = 1 时各个文件的路径
//const char* LANDINPUT = "K:\\甘泉岛海洋数据\\海陆分类训练集\\LAND.bin";       //陆地
//const char* OCEANINPUT1 = "K:\\甘泉岛海洋数据\\海陆分类训练集\\OCEAN.bin";     //海洋
//const char* TRAINPATH = "K:\\甘泉岛海洋数据\\海陆分类训练集\\train.txt";       //训练样本
//const char* MODELPATH = "K:\\甘泉岛海洋数据\\海陆分类训练集\\model.txt";       //生成模型
//const char* PREDPATH = "K:\\甘泉岛海洋数据\\海陆分类训练集\\predict.txt";        //预测结果

//TESTOPTION = 2 时各个文件的路径
const char* LANDINPUT = "K:\\东岛海洋数据\\东岛分类案例\\LAND.bin";       //陆地
const char* OCEANINPUT1 = "K:\\东岛海洋数据\\东岛分类案例\\OCEAN.bin";     //海洋
const char* INPUT = "K:\\东岛海洋数据\\测试条带4\\588823_588919.bin";
const char* TRAINPATH = "K:\\东岛海洋数据\\东岛分类案例\\train.txt";       //训练样本
const char* MYTRAIN = "K:\\东岛海洋数据\\测试条带4\\mytrain.txt";
const char* MODELPATH = "K:\\东岛海洋数据\\东岛分类案例\\model.txt";       //生成模型
const char* PREDPATH = "K:\\东岛海洋数据\\测试条带4\\predict.txt";

int main()
{
	int i = 0;
	int j = 0;
	int count = 0;
	int count1 = 0;
	int count2 = 0;
	int count3 = 0;
	int num = 10;

	bool States = true;   //定义文件读取状态
	uint64_t fsize = 0;   //文件大小
	uint64_t offset = 0;     //数据头相对文件头的偏移
	uint64_t cursize = 0;    //一个通道的波形所占字节大小
	uint64_t curlength = 0;  //一整个波形的字节大小
	uint64_t pos = 0;        //现在文件指针位置
	uint64_t wavestart_pos = 0;   //记录波形开始指针
	DataHeader DH;
	uint8_t GlobalSign[8] = { 1,35,69,103,137,171,205,239 };    //文件头标识符
	CH CH1;
	CH CH2;
	CH CH3;
	CH CH4;

	CalFeature CF;

	int TESTOPTION = 2;   //测试选项，如果等于1，则选择用训练集测试 如果等于2，则选择用自己创建的训练集测试 

	//打开训练样本文件
	FILE* fTRAIN = fopen(TRAINPATH, "wt");
	if (fTRAIN == NULL)
	{
		cout << "Cannot Open TrainFile!" << endl;
		exit(1);
	}
	//打开自己的训练样本文件
	FILE* fMYTRAIN = fopen(MYTRAIN, "wt");
	if (fMYTRAIN == NULL)
	{
		cout << "Cannot Open TrainFile!" << endl;
		exit(1);
	}

	//处理陆地文件
	//-------------------------------------------------------------------------------------------------------
	FILE* fLAND = fopen(LANDINPUT, "rb");
	if (fLAND == NULL)
	{
		cout << "Cannot Open InputFile!" << endl;
		exit(1);
	}

	//获取输入文件信息并打印
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

		//计算波形特征
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

		//输出分类结果

		CF.Out_Data(fTRAIN, 1);

		//显示进度
		CH2.Show_Process(pos, fsize, curlength, num);
		CH1.Delete();
		CH2.Delete();
		CH3.Delete();
		CH4.Delete();
	}
	fclose(fLAND);
	//-------------------------------------------------------------------------------------------------------
	//处理海洋文件
	//-------------------------------------------------------------------------------------------------------
	num = 10;    //重置处理进度值
	FILE* fSHALLOW = fopen(OCEANINPUT1, "rb");
	if (fSHALLOW == NULL)
	{
		cout << "Cannot Open InputFile!" << endl;
		exit(1);
	}

	//获取输入文件信息并打印
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

		//计算波形特征
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

		//输出分类结果

		CF.Out_Data(fTRAIN, -1);

		//显示进度
		CH2.Show_Process(pos, fsize, curlength, num);
		CH1.Delete();
		CH2.Delete();
		CH3.Delete();
		CH4.Delete();
	}
	fclose(fSHALLOW);
	fclose(fTRAIN);
	//-------------------------------------------------------------------------------------------------------
	//处理输入文件
	//-------------------------------------------------------------------------------------------------------
	num = 10;    //重置处理进度值
	FILE* fTEST = fopen(INPUT, "rb");
	if (fTEST == NULL)
	{
		cout << "Cannot Open InputFile!" << endl;
		exit(1);
	}

	//获取输入文件信息并打印
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

		//计算波形特征
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

		//如果近红外没有回波直接将其置为0
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

		//输出分类结果

		CF.Out_Data(fMYTRAIN, -1);

		//显示进度
		CH2.Show_Process(pos, fsize, curlength, num);
		CH1.Delete();
		CH2.Delete();
		CH3.Delete();
		CH4.Delete();

		//cout << "pos = " << pos << endl;
	}
	fclose(fTEST);
	fclose(fMYTRAIN);

	//用训练集产生模型 

	//count = count1 + count2 + count3;
	count = count1 + count2;
	int ELEM = 7;   //有6个特征值，因此是7，最后一位是-1代表结束

	cout << "开始训练，生成模型！" << endl;
	model = SVMTRAIN(count, ELEM, TRAINPATH, MODELPATH);

	//----------------------------------------------------
//   TESTOPTION=1用训练集做测试集
//   TESTOPTION=2用自己产生的训练集做测试集 
	if (TESTOPTION == 1)
	{
		cout << "你选择用训练集作为测试集！" << endl;
		SVMPREDICT(TRAINPATH, PREDPATH, MODELPATH);
	}
	else if (TESTOPTION == 2)
	{
		cout << "你选择用自己产生的训练集作为测试集！" << endl;
		SVMPREDICT(MYTRAIN, PREDPATH, MODELPATH);
	}

	return 0;
}