// SetPointDlg.cpp : implementation file
//

#include "stdafx.h"
#include "d2g.h"
#include "SetPointDlg.h"


// CSetPointDlg dialog

IMPLEMENT_DYNAMIC(CSetPointDlg, CDialog)

CSetPointDlg::CSetPointDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetPointDlg::IDD, pParent)
	, m_pointX(0)
	, m_pointY(0)
	, m_pointColor(_T(""))
{

}

CSetPointDlg::~CSetPointDlg()
{
}

void CSetPointDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_POINT_X, m_pointX);
	DDX_Text(pDX, IDC_POINT_Y, m_pointY);
	DDX_CBString(pDX, IDC_POINTCOLOR_SELECT_COMBO, m_pointColor);
}


BEGIN_MESSAGE_MAP(CSetPointDlg, CDialog)
END_MESSAGE_MAP()


// CSetPointDlg message handlers

BOOL CSetPointDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	pColorSelectCombo = (CComboBox*)GetDlgItem(IDC_POINTCOLOR_SELECT_COMBO);
	pColorSelectCombo->InsertString(0, _TEXT("Black"));
	pColorSelectCombo->InsertString(1, _TEXT("Red"));
	pColorSelectCombo->InsertString(2, _TEXT("Green"));
	pColorSelectCombo->InsertString(3, _TEXT("Blue"));
	pColorSelectCombo->SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
