// SetRectangleDlg1.cpp : implementation file
//

#include "stdafx.h"
#include "d2g.h"
#include "SetRectangleDlg1.h"


// CSetRectangleDlg1 dialog

IMPLEMENT_DYNAMIC(CSetRectangleDlg1, CDialog)

CSetRectangleDlg1::CSetRectangleDlg1(CWnd* pParent /*=NULL*/)
	: CDialog(CSetRectangleDlg1::IDD, pParent)
	, m_rectangleColor(_T(""))
	, m_rectangleLeftTopX(0)
	, m_rectangleLeftTopY(0)
	, m_rectangleWidth(0)
	, m_rectangleHeight(0)
{

}

CSetRectangleDlg1::~CSetRectangleDlg1()
{
}

void CSetRectangleDlg1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_RECTANGLECOLOR_SELECT_COMBO1, m_rectangleColor);
	DDX_Text(pDX, IDC_LEFT_TOP_X1, m_rectangleLeftTopX);
	DDX_Text(pDX, IDC_LEFT_TOP_Y1, m_rectangleLeftTopY);
	DDX_Text(pDX, IDC_RECTANGLE_WIDTH, m_rectangleWidth);
	DDX_Text(pDX, IDC_RECTANGLE_HEIGHT, m_rectangleHeight);
}


BEGIN_MESSAGE_MAP(CSetRectangleDlg1, CDialog)
END_MESSAGE_MAP()


// CSetRectangleDlg1 message handlers

BOOL CSetRectangleDlg1::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	pColorSelectCombo = (CComboBox*)GetDlgItem(IDC_RECTANGLECOLOR_SELECT_COMBO1);
	pColorSelectCombo->InsertString(0, _TEXT("Black"));
	pColorSelectCombo->InsertString(1, _TEXT("Red"));
	pColorSelectCombo->InsertString(2, _TEXT("Green"));
	pColorSelectCombo->InsertString(3, _TEXT("Blue"));
	pColorSelectCombo->SetCurSel(0);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
