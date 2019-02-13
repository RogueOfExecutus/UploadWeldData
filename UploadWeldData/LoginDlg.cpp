// LoginDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UploadWeldData.h"
#include "LoginDlg.h"
#include "afxdialogex.h"


// CLoginDlg �Ի���

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


// CLoginDlg ��Ϣ�������


void CLoginDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString password;
	passwordEdit.GetWindowText(password);
	if (password == _T("debug"))
	{
		//DEBUGģʽ
		userLevel = 2;
	}
	else if (password == _T("9999"))
	{
		//����ʦȨ��
		userLevel = 1;
	}
	else if (password == _T("000000"))
	{
		//����ԱȨ��
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CFileFind finder;   //�����Ƿ����ini�ļ����������ڣ�������   
	if (finder.FindFile(parameter))
	{
		GetPrivateProfileString(_T("user"), _T("empNo"), _T(""), empNo.GetBuffer(10), 10, parameter);
		GetPrivateProfileString(_T("user"), _T("area"), _T(""), area.GetBuffer(10), 10, parameter);
		empNoEdit.SetWindowText(empNo);
		areaEdit.SetWindowText(area);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
