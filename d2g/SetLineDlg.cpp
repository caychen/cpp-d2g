// SetLineDlg.cpp : implementation file
//

#include "stdafx.h"
#include "d2g.h"
#include "SetLineDlg.h"


// CSetLineDlg dialog

IMPLEMENT_DYNAMIC(CSetLineDlg, CDialog)

CSetLineDlg::CSetLineDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetLineDlg::IDD, pParent)
	, m_lineStartX(0)
	, m_lineStartY(0)
	, m_lineEndX(0)
	, m_lineEndY(0)
	, m_lineColor(_T(""))
{

}

CSetLineDlg::~CSetLineDlg()
{
}

void CSetLineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LINE_STARTX, m_lineStartX);
	DDX_Text(pDX, IDC_LINE_STARTY, m_lineStartY);
	DDX_Text(pDX, IDC_LINE_ENDX, m_lineEndX);
	DDX_Text(pDX, IDC_LINE_ENDY, m_lineEndY);
	DDX_CBString(pDX, IDC_LINECOLOR_SELECTCOMBO, m_lineColor);
}


BEGIN_MESSAGE_MAP(CSetLineDlg, CDialog)
END_MESSAGE_MAP()


// CSetLineDlg message handlers

BOOL CSetLineDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	pColorSelectCombo = (CComboBox*)GetDlgItem(IDC_LINECOLOR_SELECTCOMBO);
	pColorSelectCombo->InsertString(0, _TEXT("Black"));
	pColorSelectCombo->InsertString(1, _TEXT("Red"));
	pColorSelectCombo->InsertString(2, _TEXT("Green"));
	pColorSelectCombo->InsertString(3, _TEXT("Blue"));
	pColorSelectCombo->SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
