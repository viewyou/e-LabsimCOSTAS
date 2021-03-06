// e-LabsimCOSTAS.cpp: 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "e-LabsimCOSTAS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。  这意味着
//		它必须作为以下项中的第一个语句:
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CeLabsimCOSTASApp

BEGIN_MESSAGE_MAP(CeLabsimCOSTASApp, CWinApp)
END_MESSAGE_MAP()


// CeLabsimCOSTASApp 构造

CeLabsimCOSTASApp::CeLabsimCOSTASApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CeLabsimCOSTASApp 对象

CeLabsimCOSTASApp theApp;


// CeLabsimCOSTASApp 初始化

BOOL CeLabsimCOSTASApp::InitInstance()
{
	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	return TRUE;
}

//创建一个算法对象，并返回算法对象指针
void *LtCreateObject()
{
	CAlgorithm *pAlgorithm = new CAlgorithm();
	//UserGui->Create(IDD_DIALOG1,NULL);
	return static_cast<void*>(pAlgorithm);
}

//删除一个算法对象，此函数期待的参数（pObject）正是LtCreateObject()的返回值
void LtDestroyObject(void * pObject)
{
	ASSERT(pObject != NULL);
	ASSERT(!IsBadWritePtr(pObject, sizeof(CAlgorithm)));
	CAlgorithm * pAlgorithm = static_cast<CAlgorithm *>(pObject);
	//pAlgorithm->DestroyWindow();
	delete pAlgorithm;	//删除一个算法对象
}

void LtDLLMain(void * pObject, const bool *pbIsPortUsing, const double *pdInput, double *pdOutput, const int nSimuStep)
{
	ASSERT(pObject != NULL);
	CAlgorithm * pAlgorithm = static_cast<CAlgorithm *>(pObject);
	pAlgorithm->RunAlgorithm(pdInput, pdOutput);
}

void LtResetModule(void *pObject)
{
	ASSERT(pObject != NULL);
	ASSERT(!IsBadWritePtr(pObject, sizeof(CAlgorithm)));
	CAlgorithm * pAlgorithm = static_cast<CAlgorithm *>(pObject);
	pAlgorithm->ResetCostas();
}

CAlgorithm::CAlgorithm()
{
	m_Costas_v3 = 0;
	m_Costas_v4 = 0;
	m_Costas_v7 = 0;
	m_Costas_v5 = 0;
	memset(m_Costas_temp_v3, 0, sizeof(m_Costas_temp_v3));
	m_Costas_temp_v5 = 0;
	m_Costas_v6 = 0;
	memset(m_Costas_temp_v4, 0, sizeof(m_Costas_temp_v4));
	m_Costas_temp_v6 = 0;
	m_Costas_vco_out = 0;
	m_Costas_sum_ut = 0;
	m_Costas_v8 = 0;
	m_Costas_temp_v8 = 0;
	m_Costas_fra_cout = -1;
	memset(m_Costas_temp_fra_v7, 0, sizeof(m_Costas_temp_fra_v7));
	m_Costas_v9 = 1;
	m_Costas_v10 = 1;
	m_Costas_temp_vout = 1;
	m_Costas_phas_fra_cout = 0;
	m_Costas_Counter = 0;
	m_Costas_temp_vout_state = 0;
	m_Costas_v1 = 0;
	m_Costas_v2 = 0;
	m_Costas_temp_v1 = 0;
	m_Costas_temp_v2 = 0;
	memset(m_Costas_temp_v9, 0, sizeof(m_Costas_temp_v9));
	memset(m_Costas_temp_v10, 0, sizeof(m_Costas_temp_v10));
	m_Costas_t = 0;
	ResetCostas();
}

CAlgorithm::~CAlgorithm()
{
}

void CAlgorithm::ResetCostas()
{
	m_Costas_v3 = 0;
	m_Costas_v4 = 0;
	m_Costas_v7 = 0;
	m_Costas_v5 = 0;
	memset(m_Costas_temp_v3, 0, sizeof(m_Costas_temp_v3));
	m_Costas_temp_v5 = 0;
	m_Costas_v6 = 0;
	memset(m_Costas_temp_v4, 0, sizeof(m_Costas_temp_v4));
	m_Costas_temp_v6 = 0;
	m_Costas_vco_out = 0;
	m_Costas_sum_ut = 0;
	m_Costas_v8 = 0;
	m_Costas_temp_v8 = 0;
	m_Costas_fra_cout = -1;
	memset(m_Costas_temp_fra_v7, 0, sizeof(m_Costas_temp_fra_v7));
	m_Costas_v9 = 1;
	m_Costas_v10 = 1;
	m_Costas_temp_vout = 1;
	m_Costas_phas_fra_cout = 0;
	m_Costas_Counter = 0;
	m_Costas_temp_vout_state = 0;
	m_Costas_v1 = 0;
	m_Costas_v2 = 0;
	m_Costas_temp_v1 = 0;
	m_Costas_temp_v2 = 0;
	memset(m_Costas_temp_v9, 0, sizeof(m_Costas_temp_v9));
	memset(m_Costas_temp_v10, 0, sizeof(m_Costas_temp_v10));
	m_Costas_t = 0;
}
void CAlgorithm::RunAlgorithm(const double *pdInput, double *pdOutput)
{
	
	//costas提取载波 IN6 载频信号  W2 压控振荡 
	Costas(pdInput[IN6], pdInput[W2], pdOutput);
	//Output[OUT5] = m_Costas_output * 3.3;	//同相载波输出
	//Output[OUT6] = m_Costas_v1 * 3.3;	//正交载波输出
	//Output[OUT7] = m_Costas_v5;	//低通滤波器输出
	//Output[OUT8] = m_Costas_v6;	//低通滤波器输出
	//Output[OUT9] = m_Costas_v8 + VCO_Ctrl;	//压控晶振电压
	//Output[OUT10] = m_Costas_vco_out;	//VCXO输出
}

void CAlgorithm::Costas(const double DataIn, const double VCO_Ctrl, double *Output)
{
	m_Costas_Counter++;
	if (m_Costas_Counter > 31)
	{
		m_Costas_Counter = 0;
		/***********数据处理****************/
		m_Costas_v3 = m_Costas_v1 * DataIn;						//product1
		m_Costas_v4 = m_Costas_v2 * DataIn;						//product2
		lpfilter_pro();						//滤波器处理程序
		m_Costas_v7 = m_Costas_v5 * m_Costas_v6;					//product3
		loopfilter();						//v7通过环路滤波器
		VCO(VCO_Ctrl);						//v6控制压控振荡器输出
		phas_shift();						//分频移相
		bandpass_filter();					//调用滤波器函数输出正弦波
		m_Costas_output = m_Costas_v2;		//输出信号与v1相同
	}
	Output[OUT5] = m_Costas_output * 3.3;	//同相载波输出
	Output[OUT6] = m_Costas_v1 * 3.3;	//正交载波输出
	Output[OUT7] = m_Costas_v5;	//低通滤波器输出
	Output[OUT8] = m_Costas_v6;	//低通滤波器输出
	Output[OUT9] = m_Costas_v8 + VCO_Ctrl;	//压控晶振电压
	Output[OUT10] = m_Costas_vco_out;	//VCXO输出
}

void CAlgorithm::lpfilter_pro()
{
	//滤波器1的处理程序=====================================
	m_Costas_temp_v5 = 0;							//复位
	for (int i = 0; i<CL; ++i)				//数据累加
	{
		m_Costas_temp_v5 = coef[i] * m_Costas_temp_v3[i] + m_Costas_temp_v5;
	}
	m_Costas_v5 = m_Costas_temp_v5;							//输出数据
															//x[n]保存在temp[0],x[n-CL+1]存储在temp[CL-1]
	for (int j = CL - 1; j>0; --j)				////缓冲数据右移	
	{
		m_Costas_temp_v3[j] = m_Costas_temp_v3[j - 1];

	}
	m_Costas_temp_v3[0] = m_Costas_v3;						//x[n]保存在tem_input最低位


															//滤波器2处理程序==================================
	m_Costas_temp_v6 = 0;							//复位
	for (int i = 0; i<CL; ++i)				//数据累加
	{
		m_Costas_temp_v6 = coef[i] * m_Costas_temp_v4[i] + m_Costas_temp_v6;
	}
	m_Costas_v6 = m_Costas_temp_v6;							//输出数据
															//x[n]保存在temp[0],x[n-CL+1]存储在temp[CL-1]
	for (int j = CL - 1; j>0; --j)				////缓冲数据右移
	{
		m_Costas_temp_v4[j] = m_Costas_temp_v4[j - 1];
	}
	m_Costas_temp_v4[0] = m_Costas_v4;						//x[n]保存在tem_input最低位		
}

void CAlgorithm::VCO(const double dInverse)
{
	//处理数据截断带来的误差而累计成错误
	//if( m_Costas_v8 < 0.0067 )
	//	m_Costas_v8 = 0;
	m_Costas_t += Ot;//时间累加
	m_Costas_vco_out = Ac * cos(2 * pi*fc*m_Costas_t + 2 * pi*kc*m_Costas_sum_ut + in_phas);//输出正弦波
	m_Costas_sum_ut += Ot * m_Costas_v8*(dInverse - 2.0) * 500;
	TRACE("dInverse is : %f\n", dInverse);
}

void CAlgorithm::loopfilter()
{
	m_Costas_fra_cout++;									//分频计数器自加1
	if (63 < m_Costas_fra_cout)								//等于零的时候
	{
		m_Costas_fra_cout = 0;
		m_Costas_temp_v8 = 0;							//输出缓存复位
		for (int i = 0; i<L_CL; ++i)			//数据累加
		{
			m_Costas_temp_v8 = Loop_coef[i] * m_Costas_temp_fra_v7[i] + m_Costas_temp_v8;
		}
		m_Costas_v8 = m_Costas_temp_v8;							//输出数据
																//x[n]保存在temp[0],x[n-CL+1]存储在temp[CL-1]
		for (int j = L_CL - 1; j>0; --j)			////缓冲数据右移
		{
			m_Costas_temp_fra_v7[j] = m_Costas_temp_fra_v7[j - 1];
		}
		m_Costas_temp_fra_v7[0] = m_Costas_v7;					//x[n]保存在tem_input最低位
	}
}

void CAlgorithm::phas_shift()
{
	m_Costas_temp_vout = m_Costas_vco_out;
	//分频生成相位相差π/2的128K方波信号=========
	if (m_Costas_temp_vout>0.1)							//在temp_vout上升沿发生动作
	{
		if (0 == m_Costas_temp_vout_state)					//在temp_vout上升沿发生动作
		{
			m_Costas_temp_vout_state = 1;
			m_Costas_phas_fra_cout++;
			m_Costas_phas_fra_cout %= 4;					//四分频
			if (1 == m_Costas_phas_fra_cout || 3 == m_Costas_phas_fra_cout)
			{
				if (1 == m_Costas_v9)
					m_Costas_v9 = 0;
				else
					m_Costas_v9 = 1;
			}
			else if (0 == m_Costas_phas_fra_cout || 2 == m_Costas_phas_fra_cout)
			{
				if (1 == m_Costas_v10)
					m_Costas_v10 = 0;
				else
					m_Costas_v10 = 1;
			}
		}
	}
	else
		m_Costas_temp_vout_state = 0;
}

void CAlgorithm::bandpass_filter()
{
	//滤波器1的处理程序V9,v1,temp_v9[],temp_v1;
	m_Costas_temp_v1 = 0;					//输出复位
	for (int i = 0; i<B_CL; ++i)			//数据累加
	{
		m_Costas_temp_v1 = Band_coef[i] * m_Costas_temp_v9[i] + m_Costas_temp_v1;
	}
	m_Costas_v1 = m_Costas_temp_v1;		//输出数据

	for (int j = B_CL - 1; j>0; --j)			//缓冲数据右移
	{
		m_Costas_temp_v9[j] = m_Costas_temp_v9[j - 1];
	}
	m_Costas_temp_v9[0] = m_Costas_v9;						//x[n]保存在tem_input最低位


															//滤波器2处理程序v10,v2,temp_v10[],temp_v1
	m_Costas_temp_v2 = 0;							//复位
	for (int m = 0; m<B_CL; ++m)			//数据累加
	{
		m_Costas_temp_v2 = Band_coef[m] * m_Costas_temp_v10[m] + m_Costas_temp_v2;
	}
	m_Costas_v2 = m_Costas_temp_v2;							//输出数据
	if (m_Costas_v2 == 0)
		m_Costas_v2 = 0;
	//x[n]保存在temp[0],x[n-CL+1]存储在temp[CL-1]
	for (int n = B_CL - 1; n>0; --n)			//缓冲数据右移
	{
		m_Costas_temp_v10[n] = m_Costas_temp_v10[n - 1];
	}
	m_Costas_temp_v10[0] = m_Costas_v10;					//x[n]保存在tem_input最低位		
}
