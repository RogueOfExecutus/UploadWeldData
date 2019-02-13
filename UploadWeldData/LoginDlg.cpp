// LoginDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UploadWeldData.h"
#include "LoginDlg.h"
#include "afxdialogex.h"


// CLoginDlg 对话框

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_EMP_NO, empNoEdit);
	DDX_Control(pDX, IDC_EDIT_PASSWORK, passwordEdit);
	DDX_Control(pDX, IDC_EDIT_AREA, areaEdit);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLoginDlg 消息处理程序


void CLoginDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString password;
	passwordEdit.GetWindowText(password);
	if (password == _T("debug"))
	{
		//DEBUG模式
		userLevel = 2;
	}
	else if (password == _T("9999"))
	{
		//工程师权限
		userLevel = 1;
	}
	else if (password == _T("000000"))
	{
		//操作员权限
		userLevel = 0;
	}
	else
	{
		return;
	}
	empNoEdit.GetWindowText(empNo);
	areaEdit.GetWindowText(area);

	WritePrivateProfileString(_T("user"), _T("empNo"), empNo, parameter);
	WritePrivateProfileString(_T("user"), _T("area"), area, parameter);

	CDialogEx::OnOK();
}


BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CFileFind finder;   //查找是否存在ini文件，若不存在，则跳过   
	if (finder.FindFile(parameter))
	{
		GetPrivateProfileString(_T("user"), _T("empNo"), _T(""), empNo.GetBuffer(10), 10, parameter);
		GetPrivateProfileString(_T("user"), _T("area"), _T(""), area.GetBuffer(10), 10, parameter);
		empNoEdit.SetWindowText(empNo);
		areaEdit.SetWindowText(area);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
