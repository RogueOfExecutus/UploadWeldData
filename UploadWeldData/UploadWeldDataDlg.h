
// UploadWeldDataDlg.h : 头文件
//

#pragma once
#include "mscomm.h"
#include "afxwin.h"
#include <vector>
#include "log4cplus/log4cplus.h"
#include <condition_variable>
#include <thread>
#include <mutex>
#include <vector>
#include "MesFrameWorks/soapMesFrameWorkSoapProxy.h"
#include "afxcmn.h"


// CUploadWeldDataDlg 对话框
class CUploadWeldDataDlg : public CDialogEx
{
// 构造
public:
	CUploadWeldDataDlg(CWnd* pParent = NULL);	// 标准构造函数

	void SetUser(CString empNoInput, CString areaInput, int userLevelInput);

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UPLOADWELDDATA_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	//log4cplus::Initializer initializer;
	bool runFlag;
	void workOneThread();
	bool workOneThreadFlag;
	void plcThread();
	bool plcThreadFlag; 
	void uploadThread();
	bool uploadThreadFlag;
	void ADOThread();
	bool ADOThreadFlag;
	void delayThread(int i);
	//NG超时 0:扫码 1:功率 2:脉宽 3:频率
	bool isDelay[4];
	//接收完成 0:扫码 1:功率 2:脉宽 3:频率
	bool recDone[4];
	bool OpenComm(CMscomm& comm, LPCTSTR setter, CString portName);
	bool OpenAllComm();
	CMscomm omronComm;
	CMscomm honeyWellOneComm;
	CMscomm weldComm;
	CEdit msgShowBox;
	void AddShowMSG(CString msg);
	void initLogger(std::string name, std::string path, std::string aName);
	void WriteLog(std::string logName, std::string logMSG, log4cplus::LogLevel level);
	void initConfig();
	void readConfig();
	void initADO();
	void initListCtrl();
	void writeListCtrl();
	void initAll();
	char ASCII2INT(char ascii);
	const CString parameter = _T(".\\Parameter.ini");
	const LPCTSTR checkTrigger = _T("@00RD801200025F*\r");
	const LPCTSTR resetTrigger = _T("@00WD8013000059*\r");
	const LPCTSTR getPLCData = _T("@00RD800000135C*\r");
	const LPCTSTR setPLCData = _T("@00WD8014800056*\r");
	//const LPCTSTR scanOne = _T("\x16\x54\x0D");
	//const LPCTSTR scanOneClose = _T("\x16\x55\x0D");
	CByteArray maxMopaPower;
	CByteArray maxMopaPw;
	CByteArray maxMopaFreq;
	//const LPCTSTR MAX_MOPA_POWER = _T("\x5A\xA5\x00\x02\x00\x01\x80\x05\xDA\xAD");
	//const LPCTSTR MAX_MOPA_PW = _T("\x5A\xA5\x00\x02\x00\x01\x80\x06\xDA\xAE");
	//const LPCTSTR MAX_MOPA_FREQ = _T("\x5A\xA5\x00\x02\x00\x01\x80\x07\xDA\xAF");
	const LPCTSTR JPT_POWER = _T("$13;*");
	const LPCTSTR JPT_PW = _T("$16;*");
	const LPCTSTR JPT_FREQ = _T("$17;*");
	COleVariant POWER_TRIG;
	COleVariant PW_TRIG;
	COleVariant FREQ_TRIG;
	//条码CR标识
	int isCR;
	//是否接收到CR标识
	bool CRFlag;
	//触发标识
	bool triggerFlag;
	//焊接中标识
	bool isWelding;
	//激光机数据回复标识
	bool laserFlag;
	int trigInterval;
	int sendInterval;
	//获取PLC数据NG延时
	int plcDataNGDelay;
	//获取功率NG延时
	int powerNGDelay;
	//获取脉宽NG延时
	int pwNGDelay;
	//获取频率NG延时
	int freqNGDelay;
	int trigDelay;
	int logLevel;
	int laserType;
	int GetPCPort(std::vector<CString>& coms);
	void initMSComm();
	CComboBox plcComms;
	CComboBox labComms;
	CComboBox laserComms;
	CString plcPort;
	CString labPort;
	CString laserPort; 
	//1:plc信号触发等待锁  2:信息返回完成锁  
	std::mutex mtx1;
	std::condition_variable cv1;
	std::mutex mtx2;
	std::condition_variable cv2;
	std::mutex mtxPlc;
	std::condition_variable cvPlc;
	std::mutex mtxLaser;
	std::condition_variable cvLaser;
	std::mutex mtxUpload;
	std::condition_variable cvUpload;
	std::mutex mtxADO;
	std::condition_variable cvADO;
	//
	CString labCode;
	std::vector<CString> labCodes;
	//功率
	std::vector<float> laserPower;
	//脉宽
	std::vector<int> pulseWidth;
	//频率
	std::vector<int> frequency;
	//压力值、激光测距值、激光速度、激光焊接次数、单个模块焊接点数、当前焊点编号
	std::vector<float> pressure;
	std::vector<float> ranging;
	std::vector<int> speed;
	//std::vector<int> weldTimes;
	int weldSpot;
	std::vector<int> weldNum;
	//补焊模式、报警信息
	bool reweldMode;
	std::vector<int> errNum;
	//
	std::vector<std::string> uploadData;
	std::vector<_variant_t> ADOData;
	//
	UINT allCount;
	int listCount;
	int weldCount;
	int weldSum;
	MesFrameWorkSoapProxy mes;
	std::string empNo;
	std::string area;
	std::string mo;
	std::string errmsg;
	//std::string sn;
	//std::string workStationSN;
	CEdit areaEdit;
	CEdit empNoEdit;
	CEdit moEdit;
	//CEdit snEdit;
	//CEdit workStationSNEdit;
	int userLevel;
	CButton btConfig;
	CButton btExit;
	CButton btLaser;
	CButton btPause;
	CButton btScan;
	CButton btStart;
//	void testScanThread();
	void testLaserThread();
	void testUploadThread();
	char* UnicodeToUtf8(const wchar_t* unicode);
	CString UTF82WCS(const char* szU8);
	void char2Hex(const char* input, char* output, int len);
	//数据库链接对象
	_ConnectionPtr m_pConnect;
	//执行SQL语句的数据库对象
	_CommandPtr m_pCommand;
	//数据库操作对象 记录集
	_RecordsetPtr m_pRecordset;
	// 交换高低位
	void changeHL(void* data);
	CComboBox labCodeCombo;
	CListCtrl dataListCtrl;
	void GetADOData(CString labCode);
	CString deviceSN;
	bool errFlag[12];
	void checkFile(const char* path);
	bool CreateMDB();
	bool CreateTable();
	bool checkTable();
public:
	DECLARE_EVENTSINK_MAP()
	void OnCommOmron();
	void uploadErrMSG(int index);
	void OnCommHoneywellOne();
	void OnCommWeld();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonPause();
	afx_msg void OnBnClickedButtonConfig();
	afx_msg void OnBnClickedButtonScan();
	afx_msg void OnBnClickedButtonGetLaserData();
	afx_msg void OnBnClickedButtonExit();
	afx_msg void OnClose();
};
