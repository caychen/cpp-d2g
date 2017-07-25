// SetRectangleDlg2.cpp : implementation file
//

#include "stdafx.h"
#include "d2g.h"
#include "SetRectangleDlg2.h"


// CSetRectangleDlg2 dialog

IMPLEMENT_DYNAMIC(CSetRectangleDlg2, CDialog)

CSetRectangleDlg2::CSetRectangleDlg2(CWnd* pParent /*=NULL*/)
	: CDialog(CSetRectangleDlg2::IDD, pParent)
	, m_rectangleColor(_T(""))
	, m_rectangleLeftTopX(0)
	, m_rectangleLeftTopY(0)
	, m_rectangleRightBottomX(0)
	, m_rectangleRightBottomY(0)
{

}

CSetRectangleDlg2::~CSetRectangleDlg2()
{
}

void CSetRectangleDlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_RECTANGLECOLOR_SELECT_COMBO2, m_rectangleColor);
	DDX_Text(pDX, IDC_LEFT_TOP_X2, m_rectangleLeftTopX);
	DDX_Text(pDX, IDC_LEFT_TOP_Y2, m_rectangleLeftTopY);
	DDX_Text(pDX, IDC_RIGHT_BOTTOM_X, m_rectangleRightBottomX);
	DDX_Text(pDX, IDC_RIGHT_BOTTOM_Y, m_rectangleRightBottomY);
}


BEGIN_MESSAGE_MAP(CSetRectangleDlg2, CDialog)
END_MESSAGE_MAP()


// CSetRectangleDlg2 message handlers

BOOL CSetRectangleDlg2::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	pColorSelectCombo = (CComboBox*)GetDlgItem(IDC_RECTANGLECOLOR_SELECT_COMBO2);
	pColorSelectCombo->InsertString(0, _TEXT("Black"));
	pColorSelectCombo->InsertString(1, _TEXT("Red"));
	pColorSelectCombo->InsertString(2, _TEXT("Green"));
	pColorSelectCombo->InsertString(3, _TEXT("Blue"));
	pColorSelectCombo->SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
