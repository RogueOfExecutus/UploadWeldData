#pragma once
#include "afxwin.h"


// CConfigDlg �Ի���

class CConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConfigDlg)

public:
	CConfigDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CConfigDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONFIGURATION_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
private:
	CComboBox comboLogLevel;
	CComboBox comboLaserType;
	CEdit editTrigInterval;
	CEdit editSendInterval;
	CEdit editTrigDelay;
	CEdit editLaserNGDelay;
	CEdit editPLCNGDelay;
	CButton checkIsCR;
	const CString parameter = _T(".\\Parameter.ini");
	int logLevel;
	int laserType;
	CString trigInterval;
	CString sendInterval;
	CString trigDelay;
	CString plcDataNGDelay;
	CString laserDataNGDelay;
	CString deviceSN;
	int isCR;
	CEdit editDeviceSN;
};
