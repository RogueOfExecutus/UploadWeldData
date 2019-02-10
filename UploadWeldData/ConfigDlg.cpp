// ConfigDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UploadWeldData.h"
#include "ConfigDlg.h"
#include "afxdialogex.h"


// CConfigDlg 对话框

IMPLEMENT_DYNAMIC(CConfigDlg, CDialogEx)

CConfigDlg::CConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CONFIGURATION_DIALOG, pParent)
{

}

CConfigDlg::~CConfigDlg()
{
}

void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LOG_LEVEL, comboLogLevel);
	DDX_Control(pDX, IDC_COMBO_LASER_TYPE, comboLaserType);
	DDX_Control(pDX, IDC_EDIT_TRIGGER_INTERVAL, editTrigInterval);
	DDX_Control(pDX, IDC_EDIT_SEND_INTERVAL, editSendInterval);
	DDX_Control(pDX, IDC_EDIT_TRIGGER_DELAY, editTrigDelay);
	DDX_Control(pDX, IDC_EDIT_LASER_NG_DELAY, editLaserNGDelay);
	DDX_Control(pDX, IDC_EDIT_PLC_NG_DELAY, editPLCNGDelay);
	DDX_Control(pDX, IDC_CHECK_CR, checkIsCR);
	DDX_Control(pDX, IDC_EDIT_DEVICE_SN, editDeviceSN);
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CConfigDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CConfigDlg 消息处理程序


BOOL CConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CFileFind finder;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用   
	if (finder.FindFile(parameter))
	{
		logLevel = GetPrivateProfileInt(_T("log"), _T("LogLevel"), 20000, parameter);
		laserType = GetPrivateProfileInt(_T("laser"), _T("laser_type"), 0, parameter);
		GetPrivateProfileString(_T("plc"), _T("trigger_interval"), _T("50"), trigInterval.GetBuffer(5), 5, parameter);
		GetPrivateProfileString(_T("plc"), _T("send_interval"), _T("30"), sendInterval.GetBuffer(5), 5, parameter);
		GetPrivateProfileString(_T("plc"), _T("trigger_delay"), _T("40"), trigDelay.GetBuffer(5), 5, parameter);
		GetPrivateProfileString(_T("parameter"), _T("get_plc_data_NG_delay"), _T("1000"), plcDataNGDelay.GetBuffer(10), 10, parameter);
		GetPrivateProfileString(_T("parameter"), _T("power_NG_delay"), _T("1000"), laserDataNGDelay.GetBuffer(10), 10, parameter);
		GetPrivateProfileString(_T("parameter"), _T("device_SN"), _T(""), deviceSN.GetBuffer(40), 40, parameter);
		//pwNGDelay = GetPrivateProfileInt(_T("parameter"), _T("pulse_width_NG_delay"), 1000, parameter);
		//freqNGDelay = GetPrivateProfileInt(_T("parameter"), _T("frequency_NG_delay"), 1000, parameter);
		isCR = GetPrivateProfileInt(_T("parameter"), _T("scanner_with_CR"), 0, parameter);

		trigInterval.ReleaseBuffer();
		sendInterval.ReleaseBuffer();
		trigDelay.ReleaseBuffer();
		plcDataNGDelay.ReleaseBuffer();
		laserDataNGDelay.ReleaseBuffer();
		deviceSN.ReleaseBuffer();

		comboLogLevel.InsertString(0, _T("TRACE"));
		comboLogLevel.InsertString(1, _T("DEBUG"));
		comboLogLevel.InsertString(2, _T("INFO"));
		comboLogLevel.InsertString(3, _T("WARN"));
		comboLogLevel.InsertString(4, _T("ERROR"));
		comboLogLevel.InsertString(5, _T("FATAL"));
		comboLogLevel.SetCurSel(logLevel / 10000);

		comboLaserType.InsertString(0, _T("MAX-MOPA"));
		comboLaserType.InsertString(1, _T("JPT"));
		comboLaserType.SetCurSel(laserType);

		editTrigInterval.SetWindowText(trigInterval);
		editSendInterval.SetWindowText(sendInterval);
		editTrigDelay.SetWindowText(trigDelay);
		editLaserNGDelay.SetWindowText(laserDataNGDelay);
		editPLCNGDelay.SetWindowText(plcDataNGDelay);
		editDeviceSN.SetWindowText(deviceSN);

		checkIsCR.SetCheck(isCR);
	}
	else
	{
		MessageBox(_T("配置文件未找到，请检查！"));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CConfigDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString temp;
	temp.Format(_T("%d"), comboLogLevel.GetCurSel() * 10000);
	WritePrivateProfileString(_T("log"), _T("LogLevel"), temp, parameter);
	temp.Format(_T("%d"), comboLaserType.GetCurSel());
	WritePrivateProfileString(_T("laser"), _T("laser_type"), temp, parameter);
	editTrigInterval.GetWindowText(trigInterval);
	editSendInterval.GetWindowText(sendInterval);
	editTrigDelay.GetWindowText(trigDelay);
	editLaserNGDelay.GetWindowText(laserDataNGDelay);
	editPLCNGDelay.GetWindowText(plcDataNGDelay);
	editDeviceSN.GetWindowText(deviceSN);
	WritePrivateProfileString(_T("plc"), _T("trigger_interval"), trigInterval, parameter);
	WritePrivateProfileString(_T("plc"), _T("send_interval"), sendInterval, parameter);
	WritePrivateProfileString(_T("plc"), _T("trigger_delay"), trigDelay, parameter);
	WritePrivateProfileString(_T("parameter"), _T("get_plc_data_NG_delay"), plcDataNGDelay, parameter);
	WritePrivateProfileString(_T("parameter"), _T("power_NG_delay"), laserDataNGDelay, parameter);
	WritePrivateProfileString(_T("parameter"), _T("pulse_width_NG_delay"), laserDataNGDelay, parameter);
	WritePrivateProfileString(_T("parameter"), _T("frequency_NG_delay"), laserDataNGDelay, parameter);
	WritePrivateProfileString(_T("parameter"), _T("device_SN"), deviceSN, parameter);
	temp.Format(_T("%d"), checkIsCR.GetCheck());
	WritePrivateProfileString(_T("parameter"), _T("scanner_with_CR"), temp, parameter);

	CDialogEx::OnOK();
}
