// SizeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "d2g.h"
#include "SizeDlg.h"


// CSizeDlg dialog


IMPLEMENT_DYNAMIC(CSizeDlg, CDialog)

CSizeDlg::CSizeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSizeDlg::IDD, pParent)
	, m_length(0)
	, m_width(0)
{

}

CSizeDlg::~CSizeDlg()
{
}

void CSizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LENGTH, m_length);
	DDX_Text(pDX, IDC_WIDTH, m_width);
}


BEGIN_MESSAGE_MAP(CSizeDlg, CDialog)
END_MESSAGE_MAP()


// CSizeDlg message handlers
