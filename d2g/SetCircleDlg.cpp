// SetCircleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "d2g.h"
#include "SetCircleDlg.h"


// CSetCircleDlg dialog

IMPLEMENT_DYNAMIC(CSetCircleDlg, CDialog)

CSetCircleDlg::CSetCircleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetCircleDlg::IDD, pParent)
	, m_circleColor(_T(""))
	, m_centerX(0)
	, m_centerY(0)
	, m_fCircleRadius(0)
{

}

CSetCircleDlg::~CSetCircleDlg()
{
}

void CSetCircleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_CIRCLECOLOR_SELECT_COMBO, m_circleColor);
	DDX_Text(pDX, IDC_CIRCLE_CENTER_X, m_centerX);
	DDX_Text(pDX, IDC_CIRCLE_CENTER_Y, m_centerY);
	DDX_Text(pDX, IDC_CIRCLE_RADIUS, m_fCircleRadius);
}


BEGIN_MESSAGE_MAP(CSetCircleDlg, CDialog)
END_MESSAGE_MAP()


// CSetCircleDlg message handlers

BOOL CSetCircleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	pColorSelectCombo = (CComboBox*)GetDlgItem(IDC_CIRCLECOLOR_SELECT_COMBO);
	pColorSelectCombo->InsertString(0, _TEXT("Black"));
	pColorSelectCombo->InsertString(1, _TEXT("Red"));
	pColorSelectCombo->InsertString(2, _TEXT("Green"));
	pColorSelectCombo->InsertString(3, _TEXT("Blue"));
	pColorSelectCombo->SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
