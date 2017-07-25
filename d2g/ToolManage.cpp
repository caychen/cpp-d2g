// ToolManage.cpp : implementation file
//

#include "stdafx.h"
#include "d2g.h"
#include "ToolManage.h"


// CToolManage dialog

IMPLEMENT_DYNAMIC(CToolManage, CDialog)

CToolManage::CToolManage(CWnd* pParent /*=NULL*/)
	: CDialog(CToolManage::IDD, pParent)
{

}

CToolManage::~CToolManage()
{
}

void CToolManage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CToolManage, CDialog)
END_MESSAGE_MAP()


// CToolManage message handlers
