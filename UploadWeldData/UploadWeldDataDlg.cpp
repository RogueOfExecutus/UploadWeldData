
// UploadWeldDataDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UploadWeldData.h"
#include "UploadWeldDataDlg.h"
#include "afxdialogex.h"
#include "nlohmann/json.hpp"
#include "MesFrameWorks/MesFrameWorkSoap.nsmap"
#include "ConfigDlg.h"
#include <io.h>
#include <direct.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TESTHR(x) if FAILED(x) _com_issue_error(x);

using json = nlohmann::json;

using namespace log4cplus;
using namespace std;
using namespace ADOCG;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CUploadWeldDataDlg 对话框



CUploadWeldDataDlg::CUploadWeldDataDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_UPLOADWELDDATA_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUploadWeldDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSCOMM_OMRON, omronComm);
	DDX_Control(pDX, IDC_MSCOMM_HONEYWELL_ONE, honeyWellOneComm);
	DDX_Control(pDX, IDC_MSCOMM_WELD, weldComm);
	DDX_Control(pDX, IDC_EDIT_SHOW_MSG, msgShowBox);
	DDX_Control(pDX, IDC_COMBO_PLC_COMM, plcComms);
	DDX_Control(pDX, IDC_COMBO_SCANNER_COMM, labComms);
	DDX_Control(pDX, IDC_COMBO_LASER_COMM, laserComms);
	DDX_Control(pDX, IDC_EDIT_M_AREA, areaEdit);
	DDX_Control(pDX, IDC_EDIT_M_EMP_NO, empNoEdit);
	DDX_Control(pDX, IDC_EDIT_M_MO, moEdit);
	DDX_Control(pDX, IDC_BUTTON_CONFIG, btConfig);
	DDX_Control(pDX, IDC_BUTTON_EXIT, btExit);
	DDX_Control(pDX, IDC_BUTTON_GET_LASER_DATA, btLaser);
	DDX_Control(pDX, IDC_BUTTON_PAUSE, btPause);
	DDX_Control(pDX, IDC_BUTTON_SCAN, btScan);
	DDX_Control(pDX, IDC_BUTTON_START, btStart);
	DDX_Control(pDX, IDC_COMBO_LAB_CODES, labCodeCombo);
	DDX_Control(pDX, IDC_LIST_DATA, dataListCtrl);
}

BEGIN_MESSAGE_MAP(CUploadWeldDataDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CUploadWeldDataDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, &CUploadWeldDataDlg::OnBnClickedButtonPause)
	ON_BN_CLICKED(IDC_BUTTON_CONFIG, &CUploadWeldDataDlg::OnBnClickedButtonConfig)
	ON_BN_CLICKED(IDC_BUTTON_SCAN, &CUploadWeldDataDlg::OnBnClickedButtonScan)
	ON_BN_CLICKED(IDC_BUTTON_GET_LASER_DATA, &CUploadWeldDataDlg::OnBnClickedButtonGetLaserData)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CUploadWeldDataDlg::OnBnClickedButtonExit)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CUploadWeldDataDlg 消息处理程序

BOOL CUploadWeldDataDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	initAll();

	//test
	/*runFlag = true;
	std::thread th4(&CUploadWeldDataDlg::ADOThread, this);
	th4.detach();*/
	//test

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CUploadWeldDataDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUploadWeldDataDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CUploadWeldDataDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CUploadWeldDataDlg::workOneThread()
{
	workOneThreadFlag = true;
	//WriteLog("work", "工作线程开始", INFO_LOG_LEVEL);
	LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("work")),
		LOG4CPLUS_TEXT("工作线程开始"));
	while (runFlag)
	{
		//WriteLog("work", "work start", DEBUG_LOG_LEVEL);
		//等等触发信号
		unique_lock <mutex> lock1(mtx1);
		cv1.wait(lock1);
		if (!runFlag)
			break;
		lock1.unlock();

		if (labCodes.empty())
		{
			AddShowMSG(_T("没有条码，请扫码后再进行焊接"));
			//WriteLog("work", "焊接开始时条码列表为空", INFO_LOG_LEVEL);
			LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("work")),
				LOG4CPLUS_TEXT("焊接开始时条码列表为空"));
			continue;
		}

		//发送扫码信号，获取焊接信息,发送之前先重置各相关变量
		for (int i = 0; i < 4; i++)
		{
			recDone[i] = false;
			isDelay[i] = false;
		}
		//PLC数据获取
		unique_lock <mutex> lockPlc(mtxPlc);
		/*if (trigDelay > 10)
			Sleep(trigDelay);*/
		omronComm.put_Output(COleVariant(getPLCData));
		lockPlc.unlock();
		std::thread th1(&CUploadWeldDataDlg::delayThread, this, 0);
		th1.detach();
		//激光机数据获取
		unique_lock <mutex> lockLaser(mtxLaser);
		weldComm.put_Output(POWER_TRIG);
		std::thread th2(&CUploadWeldDataDlg::delayThread, this, 1);
		th2.detach();
		cvLaser.wait_for(lockLaser, chrono::milliseconds(sendInterval));
		lockLaser.unlock();

		//Sleep(sendInterval);
		lockLaser.lock();
		weldComm.put_Output(PW_TRIG);
		std::thread th3(&CUploadWeldDataDlg::delayThread, this, 2);
		th3.detach();
		cvLaser.wait_for(lockLaser, chrono::milliseconds(sendInterval));
		lockLaser.unlock();

		//Sleep(sendInterval);
		lockLaser.lock();
		weldComm.put_Output(FREQ_TRIG);
		std::thread th4(&CUploadWeldDataDlg::delayThread, this, 3);
		th4.detach();
		lockLaser.unlock();

		
		//等待结果
		unique_lock <mutex> lock2(mtx2);
		while (runFlag && !(recDone[0] && recDone[1] && recDone[2] && recDone[3]))
			cv2.wait(lock2);
		if (!runFlag)
			break;
		lock2.unlock();
		//复位
		unique_lock <mutex> lockPlc(mtxPlc);
		omronComm.put_Output(COleVariant(resetTrigger));
		lockPlc.unlock();
		writeListCtrl();
		//处理结果
		if (isDelay[0] || isDelay[1] || isDelay[2] || isDelay[3])
		{
			//NG
			if (isDelay[0])
			{
				AddShowMSG(_T("获取PLC数据超时NG"));
				//WriteLog("work", "获取PLC数据超时NG", INFO_LOG_LEVEL);
				LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("work")),
					LOG4CPLUS_TEXT("获取PLC数据超时NG"));
			}
			if (isDelay[1])
			{
				AddShowMSG(_T("获取功率超时NG"));
				//WriteLog("work", "获取功率超时NG", INFO_LOG_LEVEL);
				LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("work")),
					LOG4CPLUS_TEXT("获取功率超时NG"));
			}
			if (isDelay[2])
			{
				AddShowMSG(_T("获取脉宽超时NG"));
				//WriteLog("work", "获取脉宽超时NG", INFO_LOG_LEVEL);
				LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("work")),
					LOG4CPLUS_TEXT("获取脉宽超时NG"));
			}
			if (isDelay[3])
			{
				AddShowMSG(_T("获取频率超时NG"));
				//WriteLog("work", "获取频率超时NG", INFO_LOG_LEVEL);
				LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("work")),
					LOG4CPLUS_TEXT("获取频率超时NG"));
			}
			//continue;
		}
		else
		{
			if (weldCount != weldSpot)
			{

			}
			else
			{
				CString lab = labCodes[0];
				if (reweldMode)
				{
					GetADOData(lab);
				}
				CString allData;
				CString temp;
				for (int i = 0; i < weldSpot; i++)
				{
					if (i)
						allData += _T("|");
					temp.Format(_T("焊接功率%d:%.1f:%c|脉冲宽度%d:%d:ns|脉冲频率%d:%d:khz|压力值%d:%.2f:N|激光测距值%d:%.2f:mm|激光速度%d:%d:%c|焊点编号%d:%d:"),
						i, laserPower[i], '%', i, pulseWidth[i], i, frequency[i], i, pressure[i],
						i, ranging[i], i, speed[i], '%', i, weldNum[i]);
					allData += temp;
				}
				temp.Format(_T("|单个模块焊接点数:%d:"), weldSpot);
				allData += temp;
				char* data = UnicodeToUtf8(allData);
				json j_inside;
				j_inside["M_FLAG"] = 3;
				j_inside["M_DEVICE_SN"] = string(CW2A(deviceSN.GetString()));
				j_inside["M_WORKSTATION_SN"] = "L00102101";
				j_inside["M_EMP_NO"] = empNo;
				j_inside["M_AREA"] = area;
				j_inside["M_MO"] = mo;
				j_inside["M_CONTAINER_SN"] = "";
				j_inside["M_SN"] = string(CW2A(lab.GetString()));
				j_inside["M_UNION_SN"] = "";
				j_inside["M_LEVEL"] = "";
				j_inside["M_EC_FLAG"] = "";
				j_inside["M_ITEMVALUE"] = string(data);//结果
				j_inside["M_TEST_TIME"] = "";
				j_inside["M_DECRIPTION"] = "";
				j_inside["M_ITEM_TYPE"] = 0;
				j_inside["M_MARK1"] = "";
				j_inside["M_MARK2"] = "";
				j_inside["M_MARK3"] = "";
				j_inside["M_MARK4"] = "";
				json j = {
					{ "RES", "OK" },
					{ "CONTROL_TYPE", "" },
					{ "M_COMENT",{ j_inside } }
				};
				unique_lock <mutex> lockUpload(mtxUpload);
				uploadData.push_back(j.dump());
				lockUpload.unlock();
				//上传放到另一条线程

				//删除已使用条码及数据
				labCodes.erase(labCodes.begin());
				labCodeCombo.DeleteString(0);
				labCodeCombo.SetCurSel(0);
				laserPower.clear();
				pulseWidth.clear();
				frequency.clear();
				pressure.clear();
				ranging.clear();
				speed.clear();
				//weldTimes.clear();
				weldSpot = 0;
				weldNum.clear();
				errNum.clear();
				weldCount = 0;
			}

		}
		triggerFlag = false;
	}
	//WriteLog("work", "工作线程停止", INFO_LOG_LEVEL);
	LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("work")),
		LOG4CPLUS_TEXT("工作线程停止"));
	workOneThreadFlag = false;
}

void CUploadWeldDataDlg::plcThread()
{
	plcThreadFlag = true;
	while (runFlag)
	{
		unique_lock <mutex> lock(mtxPlc);
		if (!triggerFlag)
		{
			omronComm.put_Output(COleVariant(checkTrigger));
			cvPlc.wait_for(lock, chrono::milliseconds(trigInterval));
			lock.unlock();
		}
		else
		{
			lock.unlock();
			Sleep(trigInterval);
		}
	}
	plcThreadFlag = false;
}

void CUploadWeldDataDlg::uploadThread()
{
	uploadThreadFlag = true;
	while (runFlag)
	{
		unique_lock <mutex> lockUpload(mtxUpload);
		if (!uploadData.empty())
		{
			_ns1__DxDataUploadJson ns1;
			ns1.jsonText = &uploadData[0];
			_ns1__DxDataUploadJsonResponse response;
			int soap_res = mes.DxDataUploadJson(&ns1, response);
			if (soap_res == SOAP_OK)
			{
				json resJson = *response.DxDataUploadJsonResult;
				//WriteLog("work", resJson.dump(4), INFO_LOG_LEVEL);
				LOG4CPLUS_DEBUG(Logger::getInstance(LOG4CPLUS_TEXT("mes")),
					LOG4CPLUS_STRING_TO_TSTRING(resJson.dump(4)));
				string res = resJson.at("RES").get<string>();
				if (res == "OK")
				{
					//OK
				}
				else
				{
					//NG
					AddShowMSG(CString(res.c_str()));
					LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("mes")),
						LOG4CPLUS_STRING_TO_TSTRING(res));
				}
				LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("mes")),
					LOG4CPLUS_STRING_TO_TSTRING("上传成功，返回信息：" + resJson.dump()));
			}
			else
			{
				//上传失败
				CString temp;
				temp.Format(_T("上传失败，错误码：%d，请检查网络或联系IT"), soap_res);
				AddShowMSG(temp);
				LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("mes")),
					LOG4CPLUS_STRING_TO_TSTRING("上传失败，错误码：" + to_string(soap_res)));
			}
		}
		lockUpload.unlock();
		Sleep(1000);
	}
	uploadThreadFlag = false;
}

void CUploadWeldDataDlg::ADOThread()
{
	ADOThreadFlag = true;
	while (runFlag)
	{
		unique_lock <mutex> lockADO(mtxADO);
		if (ADOData.size() >= 10)
		{
			try
			{
				m_pRecordset->AddNew();
				m_pRecordset->put_Collect(_variant_t("Cell_Code"), ADOData[0]);
				m_pRecordset->put_Collect(_variant_t("laser_power"), ADOData[1]);
				m_pRecordset->put_Collect(_variant_t("pulse_width"), ADOData[2]);
				m_pRecordset->put_Collect(_variant_t("frequency"), ADOData[3]);
				m_pRecordset->put_Collect(_variant_t("pressure"), ADOData[4]);
				m_pRecordset->put_Collect(_variant_t("ranging"), ADOData[5]);
				m_pRecordset->put_Collect(_variant_t("speed"), ADOData[6]);
				//m_pRecordset->put_Collect(_variant_t("weld_times"), ADOData[7]);
				m_pRecordset->put_Collect(_variant_t("weld_spots"), ADOData[7]);
				m_pRecordset->put_Collect(_variant_t("weld_number"), ADOData[8]);
				m_pRecordset->put_Collect(_variant_t("error_number"), ADOData[9]);
				m_pRecordset->Update();    //添加完后要更新

				ADOData.erase(ADOData.begin(), ADOData.begin() + 10);
				AddShowMSG(_T("插入数据库完成"));
			}
			catch (...)
			{
				AddShowMSG(_T("插入数据库失败"));
				lockADO.unlock();
				break;
			}
		}
		lockADO.unlock();
		Sleep(1000);
	}
	ADOThreadFlag = false;
}


void CUploadWeldDataDlg::delayThread(int i)
{
	int stime;
	switch (i)
	{
	case 0:
		stime = plcDataNGDelay;
		break;
	case 1:
		stime = powerNGDelay;
		break;
	case 2:
		stime = pwNGDelay;
		break;
	case 3:
		stime = freqNGDelay;
		break;
	default:
		break;
	}
	Sleep(stime);
	unique_lock <mutex> lock2(mtx2);
	if (!recDone[i])
	{
		if (i == 0)
			weldCount++;
		recDone[i] = true;
		isDelay[i] = true;
		/*if (weldCount == weldSum)
			isWelding = false;*/
			//WriteLog("work", to_string(i) + "类数据接收超时", INFO_LOG_LEVEL);
		LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("work")),
			LOG4CPLUS_STRING_TO_TSTRING(to_string(i) + "类数据接收超时"));
		cv2.notify_all();
		switch (i)
		{
		case 0:
			pressure.push_back(0);
			ranging.push_back(0);
			speed.push_back(0);
			weldNum.push_back(0);
			errNum.push_back(0);
			break;
		case 1:
			laserPower.push_back(0);
			break;
		case 2:
			pulseWidth.push_back(0);
			break;
		case 3:
			frequency.push_back(0);
			break;
		default:
			break;
		}
	}
	lock2.unlock();
}

bool CUploadWeldDataDlg::OpenComm(CMscomm& comm, LPCTSTR setter, CString portName)
{
	portName.Delete(0, 3);
	int portNum = _ttoi(portName);//将其转化为数值类型
	if (comm.get_PortOpen())
		comm.put_PortOpen(FALSE);
	comm.put_CommPort(portNum); //选择COM
	comm.put_InBufferSize(1024); //接收缓冲区
	comm.put_OutBufferSize(1024);//发送缓冲区
	comm.put_InputLen(0);//设置当前接收区数据长度为0,表示全部读取
	comm.put_InputMode(1);//以二进制方式读写数据
	comm.put_RThreshold(1);//接收缓冲区有1个及1个以上字符时，将引发接收数据的OnComm
	comm.put_Settings(setter);//波特率，检验位，数据位，停止位
	if (!comm.get_PortOpen())
	{
		try
		{
			comm.put_PortOpen(TRUE);//打开串口
			//WriteLog("work", "打开串口 COM" + to_string(portNum), DEBUG_LOG_LEVEL);
			LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("work")),
				LOG4CPLUS_STRING_TO_TSTRING("打开串口 COM" + to_string(portNum)));
			return true;
		}
		catch (...)
		{
			//WriteLog("work", "打开串口 COM" + to_string(portNum) + "失败", DEBUG_LOG_LEVEL);
			LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("work")),
				LOG4CPLUS_STRING_TO_TSTRING("打开串口 COM" + to_string(portNum) + "失败"));
			return false;
		}
	}
	return false;
}


bool CUploadWeldDataDlg::OpenAllComm()
{
	if (OpenComm(omronComm, _T("115200,e,7,2"), plcPort) &&
		OpenComm(honeyWellOneComm, _T("115200,n,8,1"), labPort) &&
		OpenComm(weldComm, laserType ? _T("57600,n,8,1") : _T("115200,n,8,1"), laserPort))
		return true;
	return false;
}

BEGIN_EVENTSINK_MAP(CUploadWeldDataDlg, CDialogEx)
	ON_EVENT(CUploadWeldDataDlg, IDC_MSCOMM_OMRON, 1, CUploadWeldDataDlg::OnCommOmron, VTS_NONE)
	ON_EVENT(CUploadWeldDataDlg, IDC_MSCOMM_HONEYWELL_ONE, 1, CUploadWeldDataDlg::OnCommHoneywellOne, VTS_NONE)
	ON_EVENT(CUploadWeldDataDlg, IDC_MSCOMM_WELD, 1, CUploadWeldDataDlg::OnCommWeld, VTS_NONE)
END_EVENTSINK_MAP()


void CUploadWeldDataDlg::OnCommOmron()
{
	// TODO: 在此处添加消息处理程序代码
	unique_lock <mutex> lockPlc(mtxPlc);
	COleSafeArray safearray_inp;
	long len, k;
	if (omronComm.get_CommEvent() == 2) //值为 2 表示接收缓冲区内有字符
	{
		safearray_inp = omronComm.get_Input(); //变量转换
		len = safearray_inp.GetOneDimSize(); //得到有效的数据长度
		byte *rxdata; //设置 BYTE 数组
		rxdata = new byte[len + 1];
		for (k = 0; k < len; k++)
			safearray_inp.GetElement(&k, rxdata + k);
		rxdata[len] = '\0';

		//验证

		//处理数据

		if (len == 19)//判断D985.15是否ON
		{
			char data[2];
			char2Hex((char*)(rxdata + 7), data, 4);
			int plcRunFlag = (data[1] >> 5) % 2;
			reweldMode = (data[1] >> 6) % 2 == 1;
			//int trig = (data[1] >> 7) % 2;

			data[1] &= 0x00FF;
			short int en = *(short int*)(data);

			if (plcRunFlag == 0)
			{
				AddShowMSG(_T("意外停止！"));
				LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("work")),
					LOG4CPLUS_TEXT("接收到意外停止信号"));
				OnBnClickedButtonPause();
			}

			if (en)//报警信息  信号改变则上传，并更改报警标识
				for (int i = 0; i < 12; i++)
					if (((en >> i) % 2 == 1 && !errFlag[i]) || ((en >> i) % 2 == 0 && errFlag[i]))//报警信号ON且报警标识OFF，或报警信号OFF且报警标识ON
						uploadErrMSG(i);
					else
						for (int i = 0; i < 12; i++)
							if (errFlag[i])//所有报警信号OFF，但有报警标识ON
								uploadErrMSG(i);

			if (rxdata[13] == '1' && rxdata[14] == '1' && rxdata[11] == '0' && rxdata[12] == '0')
			{
				//触发
				if (!triggerFlag)
				{
					//omronComm.put_Output(COleVariant(resetTrigger));
					//WriteLog("comm", "plc trigger ON", INFO_LOG_LEVEL);
					LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("comm")),
						LOG4CPLUS_TEXT("plc trigger ON"));
					unique_lock <mutex> lock(mtx1);
					cv1.notify_all();
					lock.unlock();
				}
				triggerFlag = true;//收到触发信号置位标识
			}

			//char trig = ASCII2INT(rxdata[7]);
			//reweldMode = (trig >> 2) % 2 == 1;
			//if ((trig >> 1) % 2 == 0)
			//{
			//	//isWelding = false;
			//	AddShowMSG(_T("电焊机意外停止！"));
			//	OnBnClickedButtonPause();
			//}
			//if ((trig >> 3) % 2 == 1)
			//{
			//	//触发
			//	if (!triggerFlag)
			//	{
			//		//WriteLog("comm", "plc trigger ON", INFO_LOG_LEVEL);
			//		LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("comm")),
			//			LOG4CPLUS_TEXT("plc trigger ON"));
			//		unique_lock <mutex> lock(mtx1);
			//		cv1.notify_all();
			//		lock.unlock();
			//	}
			//	triggerFlag = true;//收到触发信号置位标识
			//}

		}

		if (len == 63)
		{
			//数据解析
			unique_lock <mutex> lock(mtx2);
			if (!recDone[0])
			{
				recDone[0] = true;
				char data[26];
				char2Hex((char*)(rxdata + 7), data, 52);

				float pres = *(float*)(data);
				pressure.push_back(pres);

				float rang = *(float*)(data + 4);
				ranging.push_back(rang);

				int sp = *(int*)(data + 8);
				speed.push_back(sp);

				/*int wt = *(int*)(data + 12);
				weldTimes.push_back(wt);*/

				weldSpot = *(int*)(data + 16);
				//weldSpot.push_back(ws);

				int wn = *(int*)(data + 20);
				weldNum.push_back(wn);

				//reweldMode = ((data[25] >> 6) % 2) == 1;

				data[25] &= 0x00FF;

				short int en = *(short int*)(data + 24);
				errNum.push_back(en);
				/*if (weldCount == 0)
					weldSum = ws;*/
				weldCount++;
				/*if (weldCount == weldSum)
					isWelding = false;*/
				cv2.notify_all();
				/*CString temp;
				temp.Format(_T("压力值：%f，激光测距值：%f，激光速度：%d，激光已焊接次数：%d，单个模块焊接点数：%d，当前焊接模组编号：%d，报警信息：%d"),
					pres, rang, sp, wt, ws, wn, en);
				AddShowMSG(temp);*/
			}
			else
			{
				AddShowMSG(_T("plc数据接收超时！"));
				LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("comm")),
					LOG4CPLUS_TEXT("plc数据接收超时！"));
			}
			lock.unlock();
		}
		//WriteLog("comm", "plc接收信息：" + string((char*)rxdata), INFO_LOG_LEVEL);
		LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("comm")),
			LOG4CPLUS_STRING_TO_TSTRING("plc接收信息：" + string((char*)rxdata) + "数据长度：" + to_string(len)));
		delete[] rxdata;
		cvPlc.notify_all();
	}
	lockPlc.unlock();
}

//
void CUploadWeldDataDlg::uploadErrMSG(int index)
{
	string errStatus;
	if (errFlag[index])//报警标识，为true则上传报警发生，否则上传报警解除
	{
		errFlag[index] = false;
		errStatus = "1";
		LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("work")),
			LOG4CPLUS_STRING_TO_TSTRING(to_string(index) + "类报警恢复"));
	}
	else
	{
		errFlag[index] = true;
		errStatus = "0";
		LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("work")),
			LOG4CPLUS_STRING_TO_TSTRING(to_string(index) + "类报警发生"));
	}
	if (index < 9)
		errmsg += "0";
	errmsg += to_string(index + 1);

	json j_inside;
	j_inside["M_FLAG"] = 7;
	j_inside["M_DEVICE_SN"] = string(CW2A(deviceSN.GetString()));
	j_inside["M_WORKSTATION_SN"] = "";
	j_inside["M_EMP_NO"] = empNo;
	j_inside["M_AREA"] = area;
	j_inside["M_MO"] = "";
	j_inside["M_CONTAINER_SN"] = "";
	j_inside["M_SN"] = "";
	j_inside["M_UNION_SN"] = "";
	j_inside["M_LEVEL"] = "";
	j_inside["M_EC_FLAG"] = "";
	j_inside["M_ITEMVALUE"] = "";//结果
	j_inside["M_TEST_TIME"] = "";
	j_inside["M_DECRIPTION"] = "";
	j_inside["M_ITEM_TYPE"] = 0;
	j_inside["M_ERROR_CODE"] = errmsg;
	j_inside["M_ERROR_LEVEL"] = "";
	j_inside["M_ERROR_STATUS"] = errStatus;
	j_inside["M_MARK1"] = "";
	j_inside["M_MARK2"] = "";
	j_inside["M_MARK3"] = "";
	j_inside["M_MARK4"] = "";
	json j = {
		{ "RES", "OK" },
		{ "CONTROL_TYPE", "" },
		{ "M_COMENT",{ j_inside } }
	};
	unique_lock <mutex> lockUpload(mtxUpload);
	uploadData.push_back(j.dump());
	lockUpload.unlock();
}


void CUploadWeldDataDlg::OnCommHoneywellOne()
{
	// TODO: 在此处添加消息处理程序代码
	if (!CRFlag)
		labCode.Empty();
	COleSafeArray safearray_inp;
	long len, k;
	if (honeyWellOneComm.get_CommEvent() == 2) //值为 2 表示接收缓冲区内有字符
	{
		safearray_inp = honeyWellOneComm.get_Input(); //变量转换
		len = safearray_inp.GetOneDimSize(); //得到有效的数据长度
		byte *rxdata; //设置 BYTE 数组
		rxdata = new byte[len + 1];
		for (k = 0; k < len; k++)
			safearray_inp.GetElement(&k, rxdata + k);
		rxdata[len] = '\0';

		labCode += CString(rxdata);
		if (isCR != 1 || rxdata[len - 1] == 0x0D)
		{
			vector<CString>::iterator rec;
			rec = find(labCodes.begin(), labCodes.end(), labCode);
			if (rec == labCodes.end())
			{
				labCodes.push_back(labCode);
				labCodeCombo.AddString(labCode);
				labCodeCombo.SetCurSel(0);
			}
			if (isCR == 1)
				CRFlag = false;
		}
		else
			CRFlag = true;
		/*if (isCR == 1 && rxdata[len - 1] != 0x0D)
			CRFlag = true;
		else if (isCR == 1 && rxdata[len - 1] == 0x0D)
		{
			vector<CString>::iterator rec;
			rec = find(labCodes.begin(), labCodes.end(), labCode);
			if (rec != labCodes.end())
			{
				labCodes.push_back(labCode);
				labCodeCombo.AddString(labCode);
				labCodeCombo.SetCurSel(0);
			}
			CRFlag = false;
		}
		else
		{
			vector<CString>::iterator rec;
			rec = find(labCodes.begin(), labCodes.end(), labCode);
			if (rec != labCodes.end())
			{
				labCodes.push_back(labCode);
				labCodeCombo.AddString(labCode);
				labCodeCombo.SetCurSel(0);
			}
		}*/
		delete[] rxdata;
	}
}


void CUploadWeldDataDlg::OnCommWeld()
{
	// TODO: 在此处添加消息处理程序代码
	unique_lock <mutex> lockLaser(mtxLaser);
	laserFlag = true;
	COleSafeArray safearray_inp;
	long len, k;
	if (weldComm.get_CommEvent() == 2) //值为 2 表示接收缓冲区内有字符
	{
		safearray_inp = weldComm.get_Input(); //变量转换
		len = safearray_inp.GetOneDimSize(); //得到有效的数据长度
		byte *rxdata; //设置 BYTE 数组
		rxdata = new byte[len];
		for (k = 0; k < len; k++)
			safearray_inp.GetElement(&k, rxdata + k);

		unique_lock <mutex> lock(mtx2);
		CString temp;
		switch (laserType)
		{
		case 0://MAX-MOPA
		{
			if (len != 12)
			{
				AddShowMSG(_T("激光焊接机返回数据格式不对，请检查参数里激光机品牌有无填错！"));
				//WriteLog("comm", "激光焊接机返回数据格式不对，请检查参数里激光机品牌有无填错！", INFO_LOG_LEVEL);
				LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("comm")),
					LOG4CPLUS_STRING_TO_TSTRING("激光焊接机返回数据格式不对，返回数据长度：" + to_string(len)));
				break;
			}
			if (rxdata[7] == 0x05)//脉宽
			{
				if (recDone[2])
				{
					//WriteLog("comm", "获取脉冲宽度超时后接收到数据返回", INFO_LOG_LEVEL);
					LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("comm")),
						LOG4CPLUS_TEXT("获取脉冲宽度超时后接收到数据返回"));
					//WriteLog("mscomm", "d", INFO_LOG_LEVEL);
					break;
				}
				recDone[2] = true;

				short int pw = *(short int*)(rxdata + 8);
				temp.Format(_T("%dns"), pw);
				AddShowMSG(temp);
				changeHL(&pw);
				pulseWidth.push_back(pw);
				temp.Format(_T("%dns"), pw);
				AddShowMSG(temp);
				/*pwEdit.SetWindowText(temp);*/

			}
			else if (rxdata[7] == 0x06)//频率
			{
				if (recDone[3])
				{
					//WriteLog("comm", "获取脉冲频率超时后接收到数据返回", INFO_LOG_LEVEL);
					LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("comm")),
						LOG4CPLUS_TEXT("获取脉冲频率超时后接收到数据返回"));
					break;
				}
				short int freq = (*(short int*)(rxdata + 8)) / 50;
				changeHL(&freq);
				frequency.push_back(freq);
				recDone[3] = true;
				/*temp.Format(_T("%dkhz"), freq);
				freqEdit.SetWindowText(temp);*/
			}
			else if (rxdata[7] == 0x07)//功率
			{
				if (recDone[1])
				{
					//WriteLog("comm", "获取功率超时后接收到数据返回", INFO_LOG_LEVEL);
					LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("comm")),
						LOG4CPLUS_TEXT("获取功率超时后接收到数据返回"));
					break;
				}
				float lp = ((float)rxdata[9] / 255.0f) * 100.0f;
				laserPower.push_back(lp);
				recDone[1] = true;
				/*temp.Format(_T("%.1f%c"), lp, '%');
				powerEdit.SetWindowText(temp);*/
			}

		}
		break;
		case 1://JPT
		{
			if (len != 8)
			{
				AddShowMSG(_T("激光焊接机返回数据格式不对，请检查参数里激光机品牌有无填错！"));
				//WriteLog("comm", "激光焊接机返回数据格式不对，请检查参数里激光机品牌有无填错！", INFO_LOG_LEVEL);
				LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("comm")),
					LOG4CPLUS_STRING_TO_TSTRING("激光焊接机返回数据格式不对，返回数据长度：" + to_string(len)));
				break;
			}
			if (rxdata[4] == 'E')
			{
				//发送指令错误
			}
			if (rxdata[2] == '3')//功率
			{
				if (recDone[1])
				{
					//WriteLog("comm", "获取功率超时后接收到数据返回", INFO_LOG_LEVEL);
					LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("comm")),
						LOG4CPLUS_STRING_TO_TSTRING("获取功率超时后接收到数据返回" + string((char*)rxdata)));
					break;
				}
				float lp = ASCII2INT(rxdata[4]) * 100 + ASCII2INT(rxdata[5]) * 10 + ASCII2INT(rxdata[6]);
				laserPower.push_back(lp);
				recDone[1] = true;
				/*temp.Format(_T("%.1f%c"), lp, '%');
				powerEdit.SetWindowText(temp);*/
			}
			else if (rxdata[2] == '6')//脉宽
			{
				if (recDone[2])
				{
					//WriteLog("comm", "获取脉冲宽度超时后接收到数据返回", INFO_LOG_LEVEL);
					LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("comm")),
						LOG4CPLUS_STRING_TO_TSTRING("获取脉冲宽度超时后接收到数据返回" + string((char*)rxdata)));
					break;
				}
				int pw = ASCII2INT(rxdata[4]) * 100 + ASCII2INT(rxdata[5]) * 10 + ASCII2INT(rxdata[6]);
				pulseWidth.push_back(pw);
				recDone[2] = true;
				/*temp.Format(_T("%dkhz"), pw);
				pwEdit.SetWindowText(temp);*/
			}
			else if (rxdata[2] == '7')//频率
			{
				if (recDone[3])
				{
					//WriteLog("comm", "获取脉冲频率超时后接收到数据返回", INFO_LOG_LEVEL);
					LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("comm")),
						LOG4CPLUS_STRING_TO_TSTRING("获取脉冲频率超时后接收到数据返回" + string((char*)rxdata)));
					break;
				}
				int freq = ASCII2INT(rxdata[4]) * 100 + ASCII2INT(rxdata[5]) * 10 + ASCII2INT(rxdata[6]);
				frequency.push_back(freq);
				recDone[3] = true;
				/*temp.Format(_T("%dns"), freq);
				freqEdit.SetWindowText(temp);*/
			}
		}
		break;
		default:
			break;
		}
		cv2.notify_all();
		lock.unlock();

		delete[] rxdata;
		cvLaser.notify_all();
	}
	lockLaser.unlock();
}


void CUploadWeldDataDlg::AddShowMSG(CString msg)
{
	if (msgShowBox.GetLineCount() > 50)
	{
		CString allMSG;
		msgShowBox.GetWindowText(allMSG);
		int firstLineLen = allMSG.Find(_T("\r\n"), 0);
		msgShowBox.SetSel(0, firstLineLen + 2);
		msgShowBox.ReplaceSel(_T(""));
	}

	int len = msgShowBox.GetWindowTextLengthW();
	msgShowBox.SetSel(len, len);
	msgShowBox.ReplaceSel(msg + _T("\r\n"));
}


void CUploadWeldDataDlg::initLogger(string name, string path, string aName)
{
	SharedAppenderPtr fileAppender(new RollingFileAppender(
		LOG4CPLUS_STRING_TO_TSTRING(path),
		5 * 1024 * 1024,
		5
	)
	);

	fileAppender->setName(LOG4CPLUS_STRING_TO_TSTRING(aName));
	tstring pattern = LOG4CPLUS_TEXT("%D{%y/%m/%d %H:%M:%S,%Q} [%t] %-5p %c - %m [%l]%n");
	fileAppender->setLayout(unique_ptr<Layout>(new PatternLayout(pattern)));

	Logger logger = Logger::getInstance(LOG4CPLUS_STRING_TO_TSTRING(name));
	logger.setLogLevel(logLevel);

	logger.addAppender(fileAppender);
}


void CUploadWeldDataDlg::WriteLog(string logName, string logMSG, LogLevel level)
{
	Logger logger = Logger::getInstance(LOG4CPLUS_STRING_TO_TSTRING(logName));
	switch (level)
	{
	case FATAL_LOG_LEVEL:
		LOG4CPLUS_FATAL(logger, LOG4CPLUS_STRING_TO_TSTRING(logMSG));
		break;
	case ERROR_LOG_LEVEL:
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_STRING_TO_TSTRING(logMSG));
		break;
	case WARN_LOG_LEVEL:
		LOG4CPLUS_WARN(logger, LOG4CPLUS_STRING_TO_TSTRING(logMSG));
		break;
	case INFO_LOG_LEVEL:
		LOG4CPLUS_INFO(logger, LOG4CPLUS_STRING_TO_TSTRING(logMSG));
		break;
	case DEBUG_LOG_LEVEL:
		LOG4CPLUS_DEBUG(logger, LOG4CPLUS_STRING_TO_TSTRING(logMSG));
		break;
	case TRACE_LOG_LEVEL:
		LOG4CPLUS_TRACE(logger, LOG4CPLUS_STRING_TO_TSTRING(logMSG));
		break;
	default:
		break;
	}
}


void CUploadWeldDataDlg::initConfig()
{
	CFileFind finder;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用   
	if (!finder.FindFile(parameter))
	{
		WritePrivateProfileString(_T("log"), _T("LogLevel"), _T("20000"), parameter);
		WritePrivateProfileString(_T("comm"), _T("plc"), _T("COM1"), parameter);
		WritePrivateProfileString(_T("comm"), _T("lab"), _T("COM2"), parameter);
		WritePrivateProfileString(_T("comm"), _T("laser"), _T("COM3"), parameter);
		WritePrivateProfileString(_T("laser"), _T("laser_type"), _T("0"), parameter);
		WritePrivateProfileString(_T("plc"), _T("trigger_interval"), _T("70"), parameter);
		WritePrivateProfileString(_T("plc"), _T("send_interval"), _T("20"), parameter);
		WritePrivateProfileString(_T("plc"), _T("trigger_delay"), _T("40"), parameter);
		WritePrivateProfileString(_T("parameter"), _T("get_plc_data_NG_delay"), _T("1000"), parameter);
		WritePrivateProfileString(_T("parameter"), _T("power_NG_delay"), _T("1000"), parameter);
		WritePrivateProfileString(_T("parameter"), _T("pulse_width_NG_delay"), _T("1000"), parameter);
		WritePrivateProfileString(_T("parameter"), _T("frequency_NG_delay"), _T("1000"), parameter);
		WritePrivateProfileString(_T("parameter"), _T("scanner_with_CR"), _T("0"), parameter);
		WritePrivateProfileString(_T("parameter"), _T("device_SN"), _T(""), parameter);
	}
	readConfig();
}


void CUploadWeldDataDlg::readConfig()
{
	logLevel = GetPrivateProfileInt(_T("log"), _T("LogLevel"), INFO_LOG_LEVEL, parameter);
	GetPrivateProfileString(_T("comm"), _T("plc"), _T("COM1"), plcPort.GetBuffer(7), 7, parameter);
	GetPrivateProfileString(_T("comm"), _T("lab"), _T("COM2"), labPort.GetBuffer(7), 7, parameter);
	GetPrivateProfileString(_T("comm"), _T("laser"), _T("COM3"), laserPort.GetBuffer(7), 7, parameter);
	laserType = GetPrivateProfileInt(_T("laser"), _T("laser_type"), 0, parameter);
	trigInterval = GetPrivateProfileInt(_T("plc"), _T("trigger_interval"), 50, parameter);
	sendInterval = GetPrivateProfileInt(_T("plc"), _T("send_interval"), 30, parameter);
	trigDelay = GetPrivateProfileInt(_T("plc"), _T("trigger_delay"), 40, parameter);
	plcDataNGDelay = GetPrivateProfileInt(_T("parameter"), _T("get_plc_data_NG_delay"), 1000, parameter);
	powerNGDelay = GetPrivateProfileInt(_T("parameter"), _T("power_NG_delay"), 1000, parameter);
	pwNGDelay = GetPrivateProfileInt(_T("parameter"), _T("pulse_width_NG_delay"), 1000, parameter);
	freqNGDelay = GetPrivateProfileInt(_T("parameter"), _T("frequency_NG_delay"), 1000, parameter);
	isCR = GetPrivateProfileInt(_T("parameter"), _T("scanner_with_CR"), 0, parameter);
	GetPrivateProfileString(_T("parameter"), _T("device_SN"), _T(""), deviceSN.GetBuffer(40), 40, parameter);

	plcPort.ReleaseBuffer();
	labPort.ReleaseBuffer();
	laserPort.ReleaseBuffer();
	deviceSN.ReleaseBuffer();

	if (laserType == 0)
	{
		maxMopaPower.Add(0x5A);
		maxMopaPower.Add(0xA5);
		maxMopaPower.Add(0x00);
		maxMopaPower.Add(0x02);
		maxMopaPower.Add(0x00);
		maxMopaPower.Add(0x01);
		maxMopaPower.Add(0x80);
		maxMopaPower.Add(0x07);
		maxMopaPower.Add(0xDA);
		maxMopaPower.Add(0xAF);

		maxMopaPw.Add(0x5A);
		maxMopaPw.Add(0xA5);
		maxMopaPw.Add(0x00);
		maxMopaPw.Add(0x02);
		maxMopaPw.Add(0x00);
		maxMopaPw.Add(0x01);
		maxMopaPw.Add(0x80);
		maxMopaPw.Add(0x05);
		maxMopaPw.Add(0xDA);
		maxMopaPw.Add(0xAD);

		maxMopaFreq.Add(0x5A);
		maxMopaFreq.Add(0xA5);
		maxMopaFreq.Add(0x00);
		maxMopaFreq.Add(0x02);
		maxMopaFreq.Add(0x00);
		maxMopaFreq.Add(0x01);
		maxMopaFreq.Add(0x80);
		maxMopaFreq.Add(0x06);
		maxMopaFreq.Add(0xDA);
		maxMopaFreq.Add(0xAE);
	}

	POWER_TRIG = laserType ? COleVariant(JPT_POWER) : COleVariant(maxMopaPower);
	PW_TRIG = laserType ? COleVariant(JPT_PW) : COleVariant(maxMopaPw);
	FREQ_TRIG = laserType ? COleVariant(JPT_FREQ) : COleVariant(maxMopaFreq);
}

void CUploadWeldDataDlg::initADO()
{
	//检查有无数据库，没有则创建数据库及TABLE
	if (_access(".\\Data.mdb", 0) == -1)
	{
		if (CreateMDB())
		{
			if (!CreateTable())
			{
				AddShowMSG(_T("创建TABLE失败"));
				LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("work")),
					LOG4CPLUS_TEXT("创建数据库后创建TABLE失败"));
				return;
			}
		}
		else
		{
			AddShowMSG(_T("创建数据库失败"));
			LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("work")),
				LOG4CPLUS_TEXT("创建数据库失败"));
			return;
		}	
	}

	HRESULT hr;
	try
	{
		//先创建连接实例
		hr = m_pConnect.CreateInstance(__uuidof(Connection));//创建Connection对象
		if (SUCCEEDED(hr))
		{
			m_pConnect->ConnectionTimeout = 600;//  链接数据库时间限制 
			m_pConnect->CommandTimeout = 120;//  也许是SQL语句执行时间限制
			//然后打开数据库  OPEN函数的参数是很有讲究的
			//Open 方法可打开一个到数据源的连接。当连接打开时，您可以对数据源执行命令
			//一个包含有关连接的信息的字符串值。该字符串由一系列被分号隔开的 parameter=value 语句组成的。
			//一个字符串值，包含建立连接时要使用的用户名称。
			//一个字符串值，包含建立连接时要使用的密码。
			//一个 ConnectOptionEnum 值，确定应在建立连接之后（同步）还是应在建立连接之前（异步）返回本方法。
			hr = m_pConnect->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=Data.mdb", "", "", adModeUnknown);// 
			//ConnectionString 属性有 5 个参数：
			//Provider---》用于连接的提供者的名称。
			//File Name---》提供者特有的文件（例如，持久保留的数据源对象）的名称，这些文件中包含预置的连接信息。
			//Remote Provider--》当打开客户端连接时使用的提供者的名称。（仅限于远程数据服务。）
			//Remote Server--》当打开客户端连接时使用的服务器的路径名称。（仅限于远程数据服务。）
			//url---》标识资源（比如文件或目录）的绝对 URL。
			//打开模式
			/*
			adModeUnknown = 0,
			adModeRead = 1,
			adModeWrite = 2,
			adModeReadWrite = 3,
			adModeShareDenyRead = 4,
			adModeShareDenyWrite = 8,
			adModeShareExclusive = 12,
			adModeShareDenyNone = 16,
			adModeRecursive = 4194304
			*/


		}
	}
	catch (_com_error e)///捕捉异常
	{
		CString errormessage;
		errormessage.Format(_T("连接数据库失败!/r/n错误信息:%s"), e.ErrorMessage());
		AddShowMSG(errormessage);/// 
		LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("work")),
			LOG4CPLUS_STRING_TO_TSTRING(CW2A(errormessage.GetString())));
		return;
	}
	catch (...)
	{
		LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("work")),
			LOG4CPLUS_TEXT("初始化连接数据库失败，未知错误"));
	}
	AddShowMSG(_T("连接数据库成功"));
	LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("work")),
		LOG4CPLUS_TEXT("初始化连接数据库成功"));
	//AfxMessageBox("成功打开数据库");

	if (!checkTable())//检查数据库内有无TABLE
		if (!CreateTable())
		{
			AddShowMSG(_T("创建TABLE失败"));
			LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("work")),
				LOG4CPLUS_TEXT("连接数据库后检测到无TABLE但创建TABLE失败"));
			return;
		}
	m_pRecordset.CreateInstance(__uuidof(Recordset));
	try
	{
		//需要把数据传输给本地，应用举例是断开数据库进行Recordset操作，即内存中操作
		//m_pRecordset->CursorLocation = adUseClient;
		//而不是写入远端数据库。相反的有 adUseServer 是指直接在数据库中操作。
		m_pRecordset->Open("SELECT * FROM WELD_DATA",//SQL 语句  
			m_pConnect.GetInterfacePtr(),
			adOpenStatic,//游标类型adOpenDynamic
			adLockOptimistic,//乐观锁
			adCmdText);//命令类型
	/*
	当您首次打开一个 Recordset 时，当前记录指针将指向第一个记录，
	同时 BOF 和 EOF 属性为 False。如果没有记录，BOF 和 EOF 属性为 True
	*/
	}
	catch (_com_error e)
	{
		CString errormessage;
		errormessage.Format(_T("打开TABLE失败!/r/n错误信息:%s"), e.ErrorMessage());
		AddShowMSG(errormessage);/// 
		LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("work")),
			LOG4CPLUS_STRING_TO_TSTRING(CW2A(errormessage.GetString())));
		return;
	}
	catch (...)
	{
		LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("work")),
			LOG4CPLUS_TEXT("初始化打开TABLE失败，未知错误"));
	}
	AddShowMSG(_T("打开TABLE成功"));
	LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("work")),
		LOG4CPLUS_TEXT("初始化打开TABLE成功"));
}

void CUploadWeldDataDlg::initListCtrl()
{
	DWORD dwStyle = dataListCtrl.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//鼠标选中某行使整行高亮（只适用与属性为report 风格的listctrl ）
	dwStyle |= LVS_EX_GRIDLINES;// 列表有网格线（只适用与report 风格的listctrl ）
	dataListCtrl.SetExtendedStyle(dwStyle); // 设置扩展风格

	dataListCtrl.InsertColumn(0, _T(" "), LVCFMT_LEFT, 20);
	dataListCtrl.InsertColumn(1, _T("条码"), LVCFMT_LEFT, 150);
	dataListCtrl.InsertColumn(2, _T("焊接功率"), LVCFMT_LEFT, 60);
	dataListCtrl.InsertColumn(3, _T("脉冲宽度"), LVCFMT_LEFT, 60);
	dataListCtrl.InsertColumn(4, _T("脉冲频率"), LVCFMT_LEFT, 60);
	dataListCtrl.InsertColumn(5, _T("压力值"), LVCFMT_LEFT, 70);
	dataListCtrl.InsertColumn(6, _T("激光测距值"), LVCFMT_LEFT, 75);
	dataListCtrl.InsertColumn(7, _T("激光速度"), LVCFMT_LEFT, 60);
	dataListCtrl.InsertColumn(8, _T("模块点数"), LVCFMT_LEFT, 60);
	dataListCtrl.InsertColumn(9, _T("焊点编号"), LVCFMT_LEFT, 60);
	dataListCtrl.InsertColumn(10, _T("报警信息"), LVCFMT_LEFT, 60);
}

//写入listCtrl
void CUploadWeldDataDlg::writeListCtrl()
{
	CString temp;
	listCount = dataListCtrl.GetItemCount();
	temp.Format(_T("%d"), allCount);
	dataListCtrl.InsertItem(listCount, temp);
	dataListCtrl.SetItemText(listCount, 1, labCodes.back());
	temp.Format(_T("%.1f%c"), laserPower.back(), '%');
	dataListCtrl.SetItemText(listCount, 2, temp);
	temp.Format(_T("%dns"), pulseWidth.back());
	dataListCtrl.SetItemText(listCount, 3, temp);
	temp.Format(_T("%dkhz"), frequency.back());
	dataListCtrl.SetItemText(listCount, 4, temp);
	temp.Format(_T("%.2fN"), pressure.back());
	dataListCtrl.SetItemText(listCount, 5, temp);
	temp.Format(_T("%.2fmm"), ranging.back());
	dataListCtrl.SetItemText(listCount, 6, temp);
	temp.Format(_T("%d%c"), speed.back(), '%');
	dataListCtrl.SetItemText(listCount, 7, temp);
	//temp.Format(_T("%d"), weldTimes.back());
	//dataListCtrl.SetItemText(listCount, 8, temp);
	temp.Format(_T("%d"), weldSpot);
	dataListCtrl.SetItemText(listCount, 8, temp);
	temp.Format(_T("%d"), weldNum.back());
	dataListCtrl.SetItemText(listCount, 9, temp);
	temp.Format(_T("%d"), errNum.back());
	dataListCtrl.SetItemText(listCount, 10, temp);
	allCount++;

	unique_lock<mutex> lockADO(mtxADO);
	ADOData.push_back(_variant_t(labCodes.back()));
	ADOData.push_back(_variant_t(laserPower.back()));
	ADOData.push_back(_variant_t(pulseWidth.back()));
	ADOData.push_back(_variant_t(frequency.back()));
	ADOData.push_back(_variant_t(pressure.back()));
	ADOData.push_back(_variant_t(ranging.back()));
	ADOData.push_back(_variant_t(speed.back()));
	//ADOData.push_back(_variant_t(weldTimes.back()));
	ADOData.push_back(_variant_t(weldSpot));
	ADOData.push_back(_variant_t(weldNum.back()));
	ADOData.push_back(_variant_t(errNum.back()));

	/*ADOData.push_back(_variant_t("test"));
	ADOData.push_back(_variant_t((long)1));
	ADOData.push_back(_variant_t(2));
	ADOData.push_back(_variant_t(3));
	ADOData.push_back(_variant_t(4));
	ADOData.push_back(_variant_t(5));
	ADOData.push_back(_variant_t(6));
	ADOData.push_back(_variant_t(7));
	ADOData.push_back(_variant_t(8));
	ADOData.push_back(_variant_t(9));*/
	lockADO.unlock();

	//CString temp;
	/*temp.Format(_T("lab:%s,laserpower:%f,pw:%d,freq:%d,pre:%f,rang:%f,speed:%d,wt:%d,ws:%d,wn:%d,en:%d"),
		labCodes.back(), laserPower.back(), pulseWidth.back(), frequency.back(), pressure.back(),
		ranging.back(), speed.back(), weldTimes.back(), weldSpot.back(), weldNum.back(), errNum.back());
	AddShowMSG(temp);*/
	if (listCount > 50)
	{
		dataListCtrl.DeleteItem(0);
	}
}


int CUploadWeldDataDlg::GetPCPort(vector<CString>& comms)
{
	long lReg;
	HKEY hKey;
	DWORD MaxValueLength;
	DWORD dwValueNumber;

	lReg = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"),
		0, KEY_QUERY_VALUE, &hKey);

	if (lReg != ERROR_SUCCESS) //成功时返回ERROR_SUCCESS，
	{
		MessageBox(_T("Open Registry Error!"));
		return 0;
	}

	lReg = RegQueryInfoKey(hKey, NULL, NULL, NULL, NULL, NULL, NULL,
		&dwValueNumber, &MaxValueLength, NULL, NULL, NULL);

	if (lReg != ERROR_SUCCESS) //没有成功
	{
		MessageBox(_T("Getting Info Error!"));
		return 0;
	}

	TCHAR *pValueName, *pCOMNumber;
	DWORD cchValueName, dwValueSize = 254;

	comms = vector<CString>(dwValueNumber);

	for (DWORD i = 0; i < dwValueNumber; i++)
	{
		cchValueName = MaxValueLength + 1;
		dwValueSize = 254;  //端口数
		pValueName = (TCHAR*)VirtualAlloc(NULL, cchValueName, MEM_COMMIT, PAGE_READWRITE);
		lReg = RegEnumValue(hKey, i, pValueName,
			&cchValueName, NULL, NULL, NULL, NULL);

		if ((lReg != ERROR_SUCCESS) && (lReg != ERROR_NO_MORE_ITEMS))
		{
			MessageBox(_T("Enum Registry Error or No More Items!"));
			return FALSE;
		}

		pCOMNumber = (TCHAR*)VirtualAlloc(NULL, 6, MEM_COMMIT, PAGE_READWRITE);
		lReg = RegQueryValueEx(hKey, pValueName, NULL,
			NULL, (LPBYTE)pCOMNumber, &dwValueSize);

		if (lReg != ERROR_SUCCESS)
		{
			MessageBox(_T("Can not get the name of the port"));
			return FALSE;
		}

		CString str(pCOMNumber);
		comms[i] = str;
		VirtualFree(pValueName, 0, MEM_RELEASE);
		VirtualFree(pCOMNumber, 0, MEM_RELEASE);
	}

	return dwValueNumber;
}


void CUploadWeldDataDlg::initMSComm()
{
	vector<CString> comms;
	int len = GetPCPort(comms);
	if (len)
	{
		for (int i = 0; i < len; i++)
		{
			plcComms.AddString(comms[i]);
			labComms.AddString(comms[i]);
			laserComms.AddString(comms[i]);
		}
		plcComms.SetCurSel(plcComms.FindStringExact(0, plcPort));
		labComms.SetCurSel(labComms.FindStringExact(0, labPort));
		laserComms.SetCurSel(laserComms.FindStringExact(0, laserPort));
	}
	else
	{
		//无串口
	}
}


void CUploadWeldDataDlg::initAll()
{
	for (int i = 0; i < 4; i++)
		isDelay[i] = false;
	CRFlag = false;
	//isWelding = false;
	checkFile("log");
	allCount = 0;
	listCount = 0;
	weldCount = 0;
	initConfig();
	initMSComm();
	initListCtrl();
	initADO();
	initLogger("work", ".\\log\\work.log", "file1");
	initLogger("comm", ".\\log\\mscomm.log", "file2");
	initLogger("mes", ".\\log\\mes.log", "file3");
	empNoEdit.SetWindowText(CString(empNo.c_str()));
	areaEdit.SetWindowText(CString(area.c_str()));

	mes.connect_timeout = 3;  //3s
	mes.accept_timeout = 3;
	mes.recv_timeout = 3;
	mes.send_timeout = 3;

	soap_set_mode(&mes, SOAP_C_UTFSTRING);

	if (userLevel == 0)
	{
		plcComms.EnableWindow(FALSE);
		labComms.EnableWindow(FALSE);
		laserComms.EnableWindow(FALSE);
	}
}


char CUploadWeldDataDlg::ASCII2INT(char ascii)
{
	if (ascii > '9')
		return ascii - '7';
	else
		return ascii - '0';
}

void CUploadWeldDataDlg::SetUser(CString empNoInput, CString areaInput, int userLevelInput)
{
	empNo = string(CW2A(empNoInput.GetString()));
	area = string(CW2A(areaInput.GetString()));
	userLevel = userLevelInput;
	areaInput.Delete(0, 1);
	errmsg = "KD" + string(CW2A(areaInput.GetString()));
}


void CUploadWeldDataDlg::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	CString temp;
	plcComms.GetWindowText(temp);
	WritePrivateProfileString(_T("comm"), _T("plc"), temp, parameter);
	labComms.GetWindowText(temp);
	WritePrivateProfileString(_T("comm"), _T("lab"), temp, parameter);
	laserComms.GetWindowText(temp);
	WritePrivateProfileString(_T("comm"), _T("laser"), temp, parameter);
	initConfig();

	laserPower.clear();
	pulseWidth.clear();
	frequency.clear();
	pressure.clear();
	ranging.clear();
	speed.clear();
	//weldTimes.clear();
	//weldSpot.clear();
	weldNum.clear();
	errNum.clear();

	if (OpenAllComm())
	{
		CString temp;
		moEdit.GetWindowText(temp);
		mo = string(CW2A(temp.GetString()));
		moEdit.EnableWindow(FALSE);
		btStart.EnableWindow(FALSE);
		btScan.EnableWindow(FALSE);
		btLaser.EnableWindow(FALSE);

		runFlag = true;

		std::thread th1(&CUploadWeldDataDlg::workOneThread, this);
		th1.detach();
		std::thread th2(&CUploadWeldDataDlg::plcThread, this);
		th2.detach();
		std::thread th3(&CUploadWeldDataDlg::uploadThread, this);
		th3.detach();
		std::thread th4(&CUploadWeldDataDlg::ADOThread, this);
		th4.detach();
		//WriteLog("work", "启动任务！", INFO_LOG_LEVEL);
		LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("work")),
			LOG4CPLUS_TEXT("启动任务！"));
		AddShowMSG(_T("启动任务！"));
	}
	else
	{
		//WriteLog("work", "启动任务时打开串口失败！", INFO_LOG_LEVEL);
		LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("work")),
			LOG4CPLUS_TEXT("启动任务时打开串口失败！"));
		AddShowMSG(_T("启动任务时打开串口失败！"));
		MessageBox(_T("打开串口失败！"));
	}
}


void CUploadWeldDataDlg::OnBnClickedButtonPause()
{
	// TODO: 在此添加控件通知处理程序代码
	runFlag = false;

	unique_lock <mutex> lock1(mtx1);
	cv1.notify_all();
	lock1.unlock();
	unique_lock <mutex> lock2(mtx2);
	cv2.notify_all();
	lock2.unlock();

	while (workOneThreadFlag || plcThreadFlag || uploadThreadFlag || ADOThreadFlag)
		Sleep(10);

	if (omronComm.get_PortOpen())
		omronComm.put_PortOpen(FALSE);
	if (honeyWellOneComm.get_PortOpen())
		honeyWellOneComm.put_PortOpen(FALSE);
	if (weldComm.get_PortOpen())
		weldComm.put_PortOpen(FALSE);

	moEdit.EnableWindow(TRUE);
	btStart.EnableWindow(TRUE);
	btScan.EnableWindow(TRUE);
	btLaser.EnableWindow(TRUE);
}


void CUploadWeldDataDlg::OnBnClickedButtonConfig()
{
	// TODO: 在此添加控件通知处理程序代码
	CConfigDlg configDlg;
	configDlg.DoModal();
	readConfig();
}


void CUploadWeldDataDlg::OnBnClickedButtonScan()
{
	// TODO: 在此添加控件通知处理程序代码
	if (runFlag)
		return;
	labCodeCombo.ResetContent();
	labCode.Empty();
	labCodes.clear();
}


void CUploadWeldDataDlg::OnBnClickedButtonGetLaserData()
{
	// TODO: 在此添加控件通知处理程序代码
	/*if (runFlag)
		return;*/
	if (OpenComm(weldComm, laserType ? _T("57600,n,8,1") : _T("115200,n,8,1"), laserPort)
		&& OpenComm(omronComm, _T("115200,e,7,2"), plcPort))
	{
		std::thread th1(&CUploadWeldDataDlg::testLaserThread, this);
		th1.detach();
		btLaser.EnableWindow(FALSE);
	}
	else
	{
		//WriteLog("work", "手动触发获取焊接数据时打开串口失败", INFO_LOG_LEVEL);
		LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("work")),
			LOG4CPLUS_TEXT("手动触发获取焊接数据时打开串口失败！"));
		AddShowMSG(_T("手动触发获取焊接数据时打开串口失败！"));
		if (omronComm.get_PortOpen())
			omronComm.put_PortOpen(FALSE);
		if (weldComm.get_PortOpen())
			weldComm.put_PortOpen(FALSE);
	}
}


void CUploadWeldDataDlg::OnBnClickedButtonExit()
{
	// TODO: 在此添加控件通知处理程序代码
	::SendMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);
	//std::thread th1(&CUploadWeldDataDlg::testUploadThread, this);
	//th1.detach();
}


void CUploadWeldDataDlg::testUploadThread()
{
	weldSpot = 8;
	for (int i = 0; i < weldSpot; i++)
	{
		laserPower.push_back(96.0f + 0.1f*i);
		pulseWidth.push_back(123 + i * 5);
		frequency.push_back(345 + i * 10);
		pressure.push_back(123.1 + 1.4*i);
		ranging.push_back(135.2 + 1.6*i);
		speed.push_back(80 + i);
		//weldTimes.push_back(1000 + i);
		weldNum.push_back(i + 1);
	}
	CString lab = _T("TWSME04003120C0000000078");
	CString allData = _T("");
	CString temp;
	for (int i = 0; i < weldSpot; i++)
	{
		if (i)
			allData += _T("|");
		temp.Format(_T("焊接功率%d:%.1f:%c|脉冲宽度%d:%d:ns|脉冲频率%d:%d:khz|压力值%d:%.2f:N|激光测距值%d:%.2f:mm|激光速度%d:%d:%c|焊点编号%d:%d:"),
			i, laserPower[i], '%', i, pulseWidth[i], i, frequency[i], i, pressure[i],
			i, ranging[i], i, speed[i], '%', i, weldNum[i]);
		allData += temp;
	}
	temp.Format(_T("|单个模块焊接点数:%d:"), weldSpot);
	allData += temp;
	char* data = UnicodeToUtf8(allData);
	json j_inside;
	j_inside["M_FLAG"] = 3;
	j_inside["M_DEVICE_SN"] = string(CW2A(deviceSN.GetString()));
	j_inside["M_WORKSTATION_SN"] = "L00102101";
	j_inside["M_EMP_NO"] = "";
	j_inside["M_AREA"] = "";
	j_inside["M_MO"] = "";
	j_inside["M_CONTAINER_SN"] = "";
	j_inside["M_SN"] = string(CW2A(lab.GetString()));
	j_inside["M_UNION_SN"] = "";
	j_inside["M_LEVEL"] = "";
	j_inside["M_EC_FLAG"] = "";
	j_inside["M_ITEMVALUE"] = string(data);//结果
	j_inside["M_TEST_TIME"] = "";
	j_inside["M_DECRIPTION"] = "";
	j_inside["M_ITEM_TYPE"] = 0;
	j_inside["M_MARK1"] = "";
	j_inside["M_MARK2"] = "";
	j_inside["M_MARK3"] = "";
	j_inside["M_MARK4"] = "";
	json j = {
		{ "RES", "OK" },
		{ "CONTROL_TYPE", "" },
		{ "M_COMENT",{ j_inside } }
	};

	//MessageBox(UTF82WCS(j.dump(4).c_str()));

	//
	_ns1__DxDataUploadJson ns1;
	_ns1__DxDataUploadJsonResponse response;
	string text = j.dump();
	ns1.jsonText = &text;
	//WriteLog("work", j.dump(4), INFO_LOG_LEVEL);
	LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("work")),
		LOG4CPLUS_STRING_TO_TSTRING(j.dump(4)));
	int soap_res = mes.DxDataUploadJson(&ns1, response);
	if (soap_res == SOAP_OK)
	{
		//json res = *(response.DxDataUploadJsonResult);
		/*if (res["RES"] == "OK")
		{
		}*/
		string *resJson = response.DxDataUploadJsonResult;
		MessageBox(UTF82WCS(resJson->c_str()));
	}
	else
	{

	}
}


void CUploadWeldDataDlg::testLaserThread()
{
	for (int i = 0; i < 4; i++)
		recDone[i] = false;

	labCodes.push_back(_T("TEST"));
	//test
	
	//unique_lock <mutex> lockLaser(mtxLaser);
	//weldComm.put_Output(COleVariant(_T("@00RD0000003E2000003E2001110000011100000111000001010000012189*\r")));
	//lockLaser.unlock();
	//std::thread th1(&CUploadWeldDataDlg::delayThread, this, 0);
	//th1.detach();
	////Sleep(50);
	//unique_lock <mutex> lockPlc(mtxPlc);
	//omronComm.put_Output(COleVariant(_T("\x5A\xA5\x01\x05\x01\x01\x05\x05\x01\x11\x01")));
	//lockPlc.unlock();
	//std::thread th2(&CUploadWeldDataDlg::delayThread, this, 1);
	//th2.detach();
	//Sleep(50);
	//lockPlc.lock();
	//omronComm.put_Output(COleVariant(_T("\x5A\xA5\x01\x05\x01\x01\x06\x06\xC3\x50\x01")));
	//lockPlc.unlock();
	//std::thread th3(&CUploadWeldDataDlg::delayThread, this, 2);
	//th3.detach();
	//Sleep(50);
	//lockPlc.lock();
	//omronComm.put_Output(COleVariant(_T("\x5A\xA5\x01\x05\x01\x01\x07\x07\x01\xF0\x01")));
	//lockPlc.unlock();
	//std::thread th4(&CUploadWeldDataDlg::delayThread, this, 3);
	//th4.detach();
	
	//test

	unique_lock <mutex> lockPlc(mtxPlc);
	omronComm.put_Output(COleVariant(getPLCData));
	lockPlc.unlock();
	std::thread th1(&CUploadWeldDataDlg::delayThread, this, 0);
	th1.detach();
	//发送
	unique_lock <mutex> lockLaser(mtxLaser);
	weldComm.put_Output(COleVariant(POWER_TRIG));
	lockLaser.unlock();
	std::thread th2(&CUploadWeldDataDlg::delayThread, this, 1);
	th2.detach();

	//Sleep(sendInterval);
	lockLaser.lock();
	cvLaser.wait_for(lockLaser, chrono::milliseconds(sendInterval));
	weldComm.put_Output(COleVariant(PW_TRIG));
	lockLaser.unlock();
	std::thread th3(&CUploadWeldDataDlg::delayThread, this, 2);
	th3.detach();

	//Sleep(sendInterval);
	lockLaser.lock();
	cvLaser.wait_for(lockLaser, chrono::milliseconds(sendInterval));
	weldComm.put_Output(COleVariant(FREQ_TRIG));
	lockLaser.unlock();
	std::thread th4(&CUploadWeldDataDlg::delayThread, this, 3);
	th4.detach();

	unique_lock <mutex> lock2(mtx2);
	while (!(recDone[0] && recDone[1] && recDone[2] && recDone[3]))
		cv2.wait(lock2);
	lock2.unlock();
	writeListCtrl();
	labCodes.pop_back();
	weldComm.put_PortOpen(FALSE);
	btLaser.EnableWindow(TRUE);
}


void CUploadWeldDataDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (MessageBox(_T("确定要退出程序吗？"), _T("退出提示！"), MB_ICONINFORMATION | MB_YESNO) != IDYES)
		return;

	if (omronComm.get_PortOpen())
		omronComm.put_PortOpen(FALSE);
	if (honeyWellOneComm.get_PortOpen())
		honeyWellOneComm.put_PortOpen(FALSE);
	if (weldComm.get_PortOpen())
		weldComm.put_PortOpen(FALSE);

	if (m_pRecordset != NULL)
	{
		m_pRecordset->Close();
		m_pRecordset.Release();
		m_pRecordset = NULL;
	}
	if (m_pConnect != NULL)
	{
		m_pConnect->Close();
		m_pConnect.Release();
		m_pConnect = NULL;
	}

	LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("work")),
		LOG4CPLUS_TEXT("退出程序"));
	CDialogEx::OnClose();
}


char* CUploadWeldDataDlg::UnicodeToUtf8(const wchar_t* unicode)
{
	int len;
	len = WideCharToMultiByte(CP_UTF8, 0, unicode, -1, NULL, 0, NULL, NULL);
	char *szUtf8 = (char*)malloc(len + 1);
	memset(szUtf8, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, unicode, -1, szUtf8, len, NULL, NULL);
	return szUtf8;
}


CString CUploadWeldDataDlg::UTF82WCS(const char* szU8)
{
	//预转换，得到所需空间的大小;
	int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), NULL, 0);

	//分配空间要给'\0'留个空间，MultiByteToWideChar不会给'\0'空间
	wchar_t* wszString = new wchar_t[wcsLen + 1];

	//转换
	::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), wszString, wcsLen);

	//最后加上'\0'
	wszString[wcsLen] = '\0';

	CString unicodeString(wszString);

	delete[] wszString;
	wszString = NULL;

	return unicodeString;
}


void CUploadWeldDataDlg::char2Hex(const char* input, char* output, int len)
{
	for (int i = 0; i < len; i++)
	{
		//把ASCII码1-9，A-F转换为对应整形
		char temp = ASCII2INT(input[i]);
		//高低位合并
		if (i % 2)
		{
			output[i / 2] |= temp;
		}
		else
		{
			output[i / 2] = temp << 4;
		}
	}
	for (int i = 0; i < len / 2;)
	{
		changeHL(output + i);
		i += 2;
	}
}


// 交换高低位
void CUploadWeldDataDlg::changeHL(void* data)
{
	char* da;
	da = (char*)data;
	char temp = da[0];
	da[0] = da[1];
	da[1] = temp;
}


void CUploadWeldDataDlg::GetADOData(CString labCode)
{
	_RecordsetPtr RCSP;
	RCSP.CreateInstance(__uuidof(Recordset));
	CString search_sql = _T("SELECT * FROM WELD_DATA WHERE Cell_Code='") + labCode + _T("'");
	RCSP = m_pConnect->Execute(search_sql.AllocSysString(), NULL, adCmdText);
	while (!RCSP->adoEOF)
	{
		int wn = RCSP->GetCollect(_variant_t("weld_number")).intVal;
		bool alreadyInclude;
		if (weldNum.empty())
			alreadyInclude = false;
		else
		{
			vector<int>::iterator rec;
			rec = find(weldNum.begin(), weldNum.end(), wn);
			alreadyInclude = rec != weldNum.end();
		}
		if (!alreadyInclude)
		{
			weldNum.push_back(wn);
			laserPower.push_back(RCSP->GetCollect(_variant_t("laser_power")).fltVal);
			pulseWidth.push_back(RCSP->GetCollect(_variant_t("pulse_width")).intVal);
			frequency.push_back(RCSP->GetCollect(_variant_t("frequency")).intVal);
			pressure.push_back(RCSP->GetCollect(_variant_t("pressure")).fltVal);
			ranging.push_back(RCSP->GetCollect(_variant_t("ranging")).fltVal);
			speed.push_back(RCSP->GetCollect(_variant_t("speed")).intVal);
			//weldTimes.push_back(RCSP->GetCollect(_variant_t("weld_times")).intVal);
			weldSpot = RCSP->GetCollect(_variant_t("weld_spots")).intVal;
			errNum.push_back(RCSP->GetCollect(_variant_t("error_number")).intVal);
			/*CString temp;
			temp.Format(_T("lab:%s,laserpower:%f,pw:%d,freq:%d,pre:%f,rang:%f,speed:%d,wt:%d,ws:%d,wn:%d,en:%d"),
						labCode, lp, pw, freq, pre,	rang, spe, wt, ws, wn, en);
			AddShowMSG(temp);*/
		}

		RCSP->MoveNext();
	}
	RCSP->Close();
	RCSP.Release();
	RCSP = NULL;
}


void CUploadWeldDataDlg::checkFile(const char* path)
{
	if (_access(path, 0) == -1)
		_mkdir(path);
}


bool CUploadWeldDataDlg::CreateMDB()
{
	CString strMdbConn = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=Data.mdb");
	try
	{
		HRESULT hr = S_OK;
		_CatalogPtr pCatalog = NULL;
		_bstr_t cnnstring(strMdbConn);
		TESTHR(hr = pCatalog.CreateInstance(__uuidof (Catalog)));
		pCatalog->Create(cnnstring);
	}
	catch (_com_error e)
	{
		_bstr_t bstrDescription(e.Description());
		CString strErro = CString(_T("创建ACCEESS数据库出错: "))
			+ (LPCSTR)e.Description()
			+ CString(_T("Create ACCESS DB error: "))
			+ (LPCSTR)e.Description();
		AddShowMSG(strErro);
		return false;
	}
	return true;
}


bool CUploadWeldDataDlg::CreateTable()
{
	HRESULT hr = S_OK;
	_CatalogPtr pCatalog = NULL;
	_TablePtr pTableNew = NULL;
	_IndexPtr pIndexNew = NULL;
	_IndexPtr pIndex = NULL;
	_ColumnPtr pColumn = NULL;
	_bstr_t strcnn("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=Data.mdb");
	try
	{
		TESTHR(hr = pCatalog.CreateInstance(__uuidof(Catalog)));
		TESTHR(hr = pTableNew.CreateInstance(__uuidof(Table)));
		TESTHR(hr = pIndexNew.CreateInstance(__uuidof(Index)));
		TESTHR(hr = pIndex.CreateInstance(__uuidof(Index)));
		TESTHR(hr = pColumn.CreateInstance(__uuidof(Column)));

		// 连接
		pCatalog->PutActiveConnection(strcnn);
		// 表名
		pTableNew->Name = _bstr_t(_T("WELD_DATA"));
		pTableNew->ParentCatalog = pCatalog;
		//
		//m_pTableNew->Columns->Append("ContactId", ::adInteger,0);          
		//m_pTableNew->Columns->GetItem("ContactId")->Properties->GetItem("AutoIncrement")->Value = true;

		// 加入字段
		CString str;
		str = _T("ID");
		pTableNew->Columns->Append(_variant_t(str), ADOCG::adInteger, 0);
		pTableNew->Columns->GetItem(_variant_t(str))->Properties->GetItem("Jet OLEDB:Allow Zero Length")->Value = false; //将必填字段设置为否
		pTableNew->Columns->GetItem(_variant_t(str))->Properties->GetItem("AutoIncrement")->Value = true;
		str = _T("Cell_Code");
		pTableNew->Columns->Append(_variant_t(str), ADOCG::adVarWChar, 255);
		pTableNew->Columns->GetItem(_variant_t(str))->Properties->GetItem("Jet OLEDB:Allow Zero Length")->Value = false;
		str = _T("laser_power");
		pTableNew->Columns->Append(_variant_t(str), ADOCG::adSingle, 50);
		pTableNew->Columns->GetItem(_variant_t(str))->Properties->GetItem("Jet OLEDB:Allow Zero Length")->Value = false;
		str = _T("pulse_width");
		pTableNew->Columns->Append(_variant_t(str), ADOCG::adInteger, 50);
		pTableNew->Columns->GetItem(_variant_t(str))->Properties->GetItem("Jet OLEDB:Allow Zero Length")->Value = false;
		str = _T("frequency");
		pTableNew->Columns->Append(_variant_t(str), ADOCG::adInteger, 50);
		pTableNew->Columns->GetItem(_variant_t(str))->Properties->GetItem("Jet OLEDB:Allow Zero Length")->Value = false;
		str = _T("pressure");
		pTableNew->Columns->Append(_variant_t(str), ADOCG::adSingle, 50);
		pTableNew->Columns->GetItem(_variant_t(str))->Properties->GetItem("Jet OLEDB:Allow Zero Length")->Value = false;
		str = _T("ranging");
		pTableNew->Columns->Append(_variant_t(str), ADOCG::adSingle, 50);
		pTableNew->Columns->GetItem(_variant_t(str))->Properties->GetItem("Jet OLEDB:Allow Zero Length")->Value = false;
		str = _T("speed");
		pTableNew->Columns->Append(_variant_t(str), ADOCG::adInteger, 50);
		pTableNew->Columns->GetItem(_variant_t(str))->Properties->GetItem("Jet OLEDB:Allow Zero Length")->Value = false;
		str = _T("weld_spots");
		pTableNew->Columns->Append(_variant_t(str), ADOCG::adInteger, 50);
		pTableNew->Columns->GetItem(_variant_t(str))->Properties->GetItem("Jet OLEDB:Allow Zero Length")->Value = false;
		str = _T("weld_number");
		pTableNew->Columns->Append(_variant_t(str), ADOCG::adInteger, 50);
		pTableNew->Columns->GetItem(_variant_t(str))->Properties->GetItem("Jet OLEDB:Allow Zero Length")->Value = false;
		str = _T("error_number");
		pTableNew->Columns->Append(_variant_t(str), ADOCG::adInteger, 50);
		pTableNew->Columns->GetItem(_variant_t(str))->Properties->GetItem("Jet OLEDB:Allow Zero Length")->Value = false; 
		str = _T("weld_data");
		pTableNew->Columns->Append(_variant_t(str), ADOCG::adDate, 255);
		pTableNew->Columns->GetItem(_variant_t(str))->Properties->GetItem("Jet OLEDB:Allow Zero Length")->Value = false;
		pTableNew->Columns->GetItem(_variant_t(str))->Properties->GetItem("Default")->Value = "Now()";

		/*_variant_t vIndex;
		for (long i = 0; i < pTableNew->Columns->Count; i++) {
			vIndex = i;
			_variant_t lIndex;
			for (long j = 0; j < pTableNew->Columns->GetItem(vIndex)->GetProperties()->Count; j++)
			{
				lIndex = j;
				CString name = CString(_T("")) + (LPCSTR)pTableNew->Columns->GetItem(vIndex)->GetProperties()->GetItem(lIndex)->GetName();
				AddShowMSG(name);
			}
		}*/
		/*      
		// 加入主键
		pIndexNew->Name = "pryIndex";//"日期";
		pIndexNew->Columns->Append(_variant_t(str), ADOCG::adVarWChar, 50);

		pIndexNew->PutPrimaryKey(-1);
		pIndexNew->PutUnique(-1);
		pTableNew->Indexes->Append(_variant_t((IDispatch*)pIndexNew));
		//*/

		// 加入表中
		pCatalog->Tables->Append(_variant_t((IDispatch*)pTableNew));
		pCatalog->Tables->Refresh();

	}
	catch (_com_error &e)
	{
		_bstr_t bstrSource(e.Source());
		_bstr_t bstrDescription(e.Description());
		CString msg = CString(_T("")) + (LPCSTR)e.Description();
		AddShowMSG(msg);
		return false;

	}
	return true;
}


bool CUploadWeldDataDlg::checkTable()
{
	_RecordsetPtr pRstSchema = NULL;
	bool res = false;
	try
	{
		pRstSchema = m_pConnect->OpenSchema(adSchemaTables);

		while (!pRstSchema->adoEOF)
		{
			_bstr_t table_name = pRstSchema->Fields->GetItem("TABLE_NAME")->Value;
			_bstr_t table_type = pRstSchema->Fields->GetItem("TABLE_TYPE")->Value;
			if (table_type == _bstr_t("TABLE") && table_name == _bstr_t("WELD_DATA"))
			{
				res = true;
				break;
			}
			pRstSchema->MoveNext();
		}
	}
	catch (_com_error e)// 
	{
		if (pRstSchema != NULL)
		{
			pRstSchema->Close();
			pRstSchema.Release();
			pRstSchema = NULL;
		}
		res = false;
	}
	if (pRstSchema != NULL)
	{
		pRstSchema->Close();
		pRstSchema.Release();
		pRstSchema = NULL;
	}
	return res;
}
