// d2g.h : d2g Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// Cd2gApp:
// �йش����ʵ�֣������ d2g.cpp
//

class Cd2gApp : public CWinApp
{
public:
	Cd2gApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Cd2gApp theApp;