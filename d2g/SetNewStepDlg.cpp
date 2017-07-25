// SetNewStepDlg.cpp : implementation file
//

#include "stdafx.h"
#include "d2g.h"
#include "SetNewStepDlg.h"


// CSetNewStepDlg dialog

IMPLEMENT_DYNAMIC(CSetNewStepDlg, CDialog)

CSetNewStepDlg::CSetNewStepDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetNewStepDlg::IDD, pParent)
	, m_fNewStep(0)
{

}

CSetNewStepDlg::~CSetNewStepDlg()
{
}

void CSetNewStepDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDE_NEWSTEP, m_fNewStep);
	DDV_MinMaxDouble(pDX, m_fNewStep, 0.000001, RAND_MAX);
}


BEGIN_MESSAGE_MAP(CSetNewStepDlg, CDialog)
END_MESSAGE_MAP()


// CSetNewStepDlg message handlers

BOOL CSetNewStepDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	GetDlgItem(IDE_NEWSTEP)->SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
