
// UploadWeldData.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "log4cplus/log4cplus.h"


// CUploadWeldDataApp: 
// �йش����ʵ�֣������ UploadWeldData.cpp
//

class CUploadWeldDataApp : public CWinApp
{
public:
	CUploadWeldDataApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
private:
	log4cplus::Initializer initializer;
};

extern CUploadWeldDataApp theApp;