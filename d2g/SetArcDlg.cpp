// SetArcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "d2g.h"
#include "SetArcDlg.h"


// CSetArcDlg dialog

IMPLEMENT_DYNAMIC(CSetArcDlg, CDialog)

CSetArcDlg::CSetArcDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetArcDlg::IDD, pParent)
	, m_arcColor(_T(""))
	, m_arcCenterX(0)
	, m_arcCenterY(0)
	, m_arcRadius(0)
	, m_arcStartAngle(0)
	, m_arcEndAngle(0)
	, m_arcDirection(0)
{

}

CSetArcDlg::~CSetArcDlg()
{
}

void CSetArcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_ARCCOLOR_SELECT_COMBO, m_arcColor);
	DDX_Text(pDX, IDC_ARCCENTER_X, m_arcCenterX);
	DDX_Text(pDX, IDC_ARCCENTER_Y, m_arcCenterY);
	DDX_Text(pDX, IDC_ARC_RADIUS, m_arcRadius);
	DDX_Text(pDX, IDC_ARC_STARTANGLE, m_arcStartAngle);
	DDX_Text(pDX, IDC_ARC_ENDANGLE, m_arcEndAngle);
	DDX_Radio(pDX, IDC_RADIO1, m_arcDirection);
}


BEGIN_MESSAGE_MAP(CSetArcDlg, CDialog)
END_MESSAGE_MAP()


// CSetArcDlg message handlers

BOOL CSetArcDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	pColorSelectCombo = (CComboBox*)GetDlgItem(IDC_ARCCOLOR_SELECT_COMBO);
	pColorSelectCombo->InsertString(0, _TEXT("Black"));
	pColorSelectCombo->InsertString(1, _TEXT("Red"));
	pColorSelectCombo->InsertString(2, _TEXT("Green"));
	pColorSelectCombo->InsertString(3, _TEXT("Blue"));
	pColorSelectCombo->SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
