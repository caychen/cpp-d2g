// BasePointDlg.cpp : implementation file
//

#include "stdafx.h"
#include "d2g.h"
#include "PreSaveDlg.h"


// CPreSaveDlg dialog

IMPLEMENT_DYNAMIC(CPreSaveDlg, CDialog)

CPreSaveDlg::CPreSaveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPreSaveDlg::IDD, pParent)
	, m_basePoint_X(0)
	, m_basePoint_Y(0)
	, m_SelectIndex(0)
	, m_saveAllOrSelected(0)
	, m_nOptimizeSelected(0)
{

}

CPreSaveDlg::~CPreSaveDlg()
{
}

void CPreSaveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_BASEPOINT_X, m_basePoint_X);
	DDX_Text(pDX, IDC_BASEPOINT_Y, m_basePoint_Y);
	DDX_Radio(pDX, IDC_RADIO1, m_SelectIndex);
	DDX_Radio(pDX, IDC_RADIO_ALL, m_saveAllOrSelected);
	DDX_Radio(pDX, IDC_RADIO5, m_nOptimizeSelected);
}


BEGIN_MESSAGE_MAP(CPreSaveDlg, CDialog)
END_MESSAGE_MAP()


// CPreSaveDlg message handlers

BOOL CPreSaveDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	CheckRadioButton(IDC_RADIO_YES, IDC_RADIO_NO, IDC_RADIO_YES);
	CheckRadioButton(IDC_RADIO5, IDC_RADIO7, IDC_RADIO5);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
