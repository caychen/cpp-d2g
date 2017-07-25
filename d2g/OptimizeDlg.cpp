// OptimizeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "d2g.h"
#include "OptimizeDlg.h"


// COptimizeDlg dialog

IMPLEMENT_DYNAMIC(COptimizeDlg, CDialog)

COptimizeDlg::COptimizeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptimizeDlg::IDD, pParent)
	, m_BasePointX(0)
	, m_BasePointY(0)
	, m_bIsOptimizeAll(1)
	, m_nOptimizeSelected(0)
{

}

COptimizeDlg::~COptimizeDlg()
{
}

void COptimizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_BasePointX);
	DDX_Text(pDX, IDC_EDIT11, m_BasePointY);
	DDX_Radio(pDX, IDC_RADIO3, m_bIsOptimizeAll);
	DDX_Radio(pDX, IDC_RADIO_YES, m_nOptimizeSelected);
}


BEGIN_MESSAGE_MAP(COptimizeDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO_NO, &COptimizeDlg::OnBnClickedRadioNo)
	ON_BN_CLICKED(IDC_RADIO_YES, &COptimizeDlg::OnBnClickedRadioYes)
END_MESSAGE_MAP()


BOOL COptimizeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CheckRadioButton(IDC_RADIO3, IDC_RADIO4, IDC_RADIO3);
	CheckRadioButton(IDC_RADIO_YES, IDC_RADIO_NO, IDC_RADIO_YES);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void COptimizeDlg::OnBnClickedRadioNo()
{
	// TODO: Add your control notification handler code here
	EnableControl(IDC_STATIC_TIP, FALSE);
	EnableControl(IDC_RADIO3, FALSE);
	EnableControl(IDC_RADIO4, FALSE);
	/*((CButton*)GetDlgItem(IDC_RADIO_YES))->SetCheck(0);
	m_nOptimizeSelected = 1;*/
}

void COptimizeDlg::OnBnClickedRadioYes()
{
	// TODO: Add your control notification handler code here
	EnableControl(IDC_STATIC_TIP);
	EnableControl(IDC_RADIO3);
	EnableControl(IDC_RADIO4);
	/*((CButton*)GetDlgItem(IDC_RADIO_NO))->SetCheck(0);
	m_nOptimizeSelected = 0;*/
}

void COptimizeDlg::EnableControl( UINT nID, BOOL bEnabled /*= TRUE*/ )
{
	GetDlgItem(nID)->EnableWindow(bEnabled);
}