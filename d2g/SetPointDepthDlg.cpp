// SetPointDepthDlg.cpp : implementation file
//

#include "stdafx.h"
#include "d2g.h"
#include "SetPointDepthDlg.h"


// CSetPointDepthDlg dialog

IMPLEMENT_DYNAMIC(CSetPointDepthDlg, CDialog)

CSetPointDepthDlg::CSetPointDepthDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetPointDepthDlg::IDD, pParent)
	, m_fPointDepth(0)
{

}

CSetPointDepthDlg::~CSetPointDepthDlg()
{
}

void CSetPointDepthDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDE_NEW_POINT_DEPTH, m_fPointDepth);
}


BEGIN_MESSAGE_MAP(CSetPointDepthDlg, CDialog)
END_MESSAGE_MAP()


// CSetPointDepthDlg message handlers
