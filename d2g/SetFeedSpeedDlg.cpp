// SetFeedSpeedDlg.cpp : implementation file
//

#include "stdafx.h"
#include "d2g.h"
#include "SetFeedSpeedDlg.h"


// CSetFeedSpeedDlg dialog

IMPLEMENT_DYNAMIC(CSetFeedSpeedDlg, CDialog)

CSetFeedSpeedDlg::CSetFeedSpeedDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetFeedSpeedDlg::IDD, pParent)
	, m_iNewFeedSpeed(0)
{

}

CSetFeedSpeedDlg::~CSetFeedSpeedDlg()
{
}

void CSetFeedSpeedDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDE_NEW_FEEDSPEED, m_iNewFeedSpeed);
}


BEGIN_MESSAGE_MAP(CSetFeedSpeedDlg, CDialog)
END_MESSAGE_MAP()


// CSetFeedSpeedDlg message handlers

BOOL CSetFeedSpeedDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	GetDlgItem(IDE_NEW_FEEDSPEED)->SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
