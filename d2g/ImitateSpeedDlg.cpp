// ImitateSpeedDlg.cpp : implementation file
//

#include "stdafx.h"
#include "d2g.h"
#include "ImitateSpeedDlg.h"

extern vector<_2PointData> g_ImitatePerPointDataVector;
extern vector<_2PointData> g_DrawVector;
extern int CurrentImitateIndex;

// CImitateSpeedDlg dialog

IMPLEMENT_DYNAMIC(CImitateSpeedDlg, CDialog)

CImitateSpeedDlg::CImitateSpeedDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImitateSpeedDlg::IDD, pParent)
	, m_SliderPosValue(5)
	, m_ImitateStatus(PAUSE)
{
	hStartIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_START));
	hPauseIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_PAUSE));
	hFasterIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_FORWARD));
	hSlowerIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_BACKWARD));
	hStopIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_STOP));
	hOpenIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_OPEN));
	hParentWnd = NULL;
}

CImitateSpeedDlg::~CImitateSpeedDlg()
{
}

void CImitateSpeedDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER, m_SliderCtrl);
	//DDX_Control(pDX, IDC_BTN_ENDDLG, m_EndDlgBtn);
	//DDX_Control(pDX, IDC_BTN_FASTER, m_FasterBtm);
	DDX_Control(pDX, IDC_PROGRESS, m_PrgCtrl);
}


BEGIN_MESSAGE_MAP(CImitateSpeedDlg, CDialog)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER, &CImitateSpeedDlg::OnNMCustomdrawSlider)
	ON_BN_CLICKED(IDC_BTN_SLOWER, &CImitateSpeedDlg::OnBnClickedBtnSlower)
	ON_BN_CLICKED(IDC_BTN_FASTER, &CImitateSpeedDlg::OnBnClickedBtnFaster)
	ON_BN_CLICKED(IDC_BTN_START, &CImitateSpeedDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_PAUSE, &CImitateSpeedDlg::OnBnClickedBtnPause)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_ENDDLG, &CImitateSpeedDlg::OnBnClickedBtnEndDlg)
	ON_WM_PAINT()
	ON_MESSAGE(WM_UPDATE_PROGRESS,&CImitateSpeedDlg::OnUpdatePrg)
	ON_BN_CLICKED(IDC_BTN_OPEN, &CImitateSpeedDlg::OnBnClickedBtnOpen)
END_MESSAGE_MAP()


// CImitateSpeedDlg message handlers

void CImitateSpeedDlg::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	DeleteObject(hFasterIcon);
	DeleteObject(hSlowerIcon);
	DeleteObject(hStartIcon);
	DeleteObject(hPauseIcon);
	DeleteObject(hStopIcon);
	DeleteObject(hOpenIcon);
	CDialog::PostNcDestroy();
}

BOOL CImitateSpeedDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_SliderCtrl.SetRange(1, 10);
	m_SliderCtrl.SetTicFreq(1);
	m_SliderCtrl.SetTic(1);
	m_SliderCtrl.SetPos(5);

	if (m_ToolTipCtrl.m_hWnd == NULL)
	{
		m_ToolTipCtrl.Create(this, TTS_ALWAYSTIP);
		m_ToolTipCtrl.AddTool( GetDlgItem(IDC_BTN_START), L"开始"); //IDC_BUTTON为你要添加提示信息的按钮的ID
		m_ToolTipCtrl.AddTool( GetDlgItem(IDC_BTN_PAUSE), L"暂停");
		m_ToolTipCtrl.AddTool( GetDlgItem(IDC_BTN_FASTER), L"加速");
		m_ToolTipCtrl.AddTool( GetDlgItem(IDC_BTN_SLOWER), L"减速");
		m_ToolTipCtrl.AddTool( GetDlgItem(IDC_BTN_ENDDLG), L"结束");
	}

	m_ToolTipCtrl.SetDelayTime(200); //设置延迟
	m_ToolTipCtrl.SetTipTextColor( RGB(0, 0, 255) ); //设置提示文本的颜色
	m_ToolTipCtrl.SetTipBkColor( RGB(255, 255, 255)); //设置提示框的背景颜色
	m_ToolTipCtrl.Activate(TRUE); //设置是否启用提示

	((CButton*)GetDlgItem(IDC_BTN_FASTER))->SetIcon(hFasterIcon);
	((CButton*)GetDlgItem(IDC_BTN_SLOWER))->SetIcon(hSlowerIcon);
	((CButton*)GetDlgItem(IDC_BTN_START))->SetIcon(hStartIcon);
	((CButton*)GetDlgItem(IDC_BTN_PAUSE))->SetIcon(hPauseIcon);
	((CButton*)GetDlgItem(IDC_BTN_ENDDLG))->SetIcon(hStopIcon);
	((CButton*)GetDlgItem(IDC_BTN_OPEN))->SetIcon(hOpenIcon);

	GetDlgItem(IDC_BTN_START)->EnableWindow();
	GetDlgItem(IDC_BTN_PAUSE)->EnableWindow(FALSE);
	m_PrgCtrl.SetRange(0, 100);
	m_PrgCtrl.SetPos(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CImitateSpeedDlg::OnNMCustomdrawSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_SliderPosValue = m_SliderCtrl.GetPos();
	if(m_SliderPosValue == m_SliderCtrl.GetRangeMin())
		GetDlgItem(IDC_BTN_SLOWER)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_BTN_SLOWER)->EnableWindow();

	if(m_SliderPosValue == m_SliderCtrl.GetRangeMax())
		GetDlgItem(IDC_BTN_FASTER)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_BTN_FASTER)->EnableWindow();

	UpdateData(false);

	*pResult = 0;
}
void CImitateSpeedDlg::OnBnClickedBtnSlower()
{
	// TODO: Add your control notification handler code here
	if(m_SliderPosValue <= m_SliderCtrl.GetRangeMin())
		return;

	m_SliderCtrl.SetPos(--m_SliderPosValue);
}

void CImitateSpeedDlg::OnBnClickedBtnFaster()
{
	// TODO: Add your control notification handler code here
	if(m_SliderPosValue >= m_SliderCtrl.GetRangeMax())
		return;

	m_SliderCtrl.SetPos(++m_SliderPosValue);
}

void CImitateSpeedDlg::OnBnClickedBtnStart()
{
	// TODO: Add your control notification handler code here
	m_ImitateStatus = START;
	if(!CurrentImitateIndex)
		g_DrawVector.clear();

	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_PAUSE)->EnableWindow();
	GetDlgItem(IDC_BTN_PAUSE)->SetFocus();
}

void CImitateSpeedDlg::OnBnClickedBtnPause()
{
	// TODO: Add your control notification handler code here
	m_ImitateStatus = PAUSE;
	GetDlgItem(IDC_BTN_START)->EnableWindow();
	GetDlgItem(IDC_BTN_PAUSE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_START)->SetFocus();
}
BOOL CImitateSpeedDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	m_ToolTipCtrl.RelayEvent(pMsg);
	return CDialog::PreTranslateMessage(pMsg);
}

void CImitateSpeedDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	ShowWindow(SW_HIDE);
}

void CImitateSpeedDlg::SetParentWnd(HWND hWnd)
{
	hParentWnd = hWnd;
}

HWND CImitateSpeedDlg::GetParentWnd(void)
{
	return hParentWnd;
}

void CImitateSpeedDlg::OnBnClickedBtnEndDlg()
{
	// TODO: Add your control notification handler code here
	::PostMessage(hParentWnd,WM_ENDDLG, 0, 0);
	g_DrawVector.clear();
	g_ImitatePerPointDataVector.clear();
	//::PostMessage(hParentWnd,WM_PAINT,0,0);
	CWnd::FromHandle(hParentWnd)->Invalidate();
	CDialog::DestroyWindow();
}

void CImitateSpeedDlg::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages

	CRect rt;
	CPaintDC dc(this);
	GetClientRect(&rt);
	dc.FillSolidRect(rt, RGB(255, 255, 255));
}

LRESULT CImitateSpeedDlg::OnUpdatePrg(WPARAM wParam, LPARAM lParam)
{
	UINT uPrg = (UINT)wParam;
	m_PrgCtrl.SetPos(uPrg);

	CString strValue;
	strValue.Format(L"%d %%", uPrg);
	SetDlgItemText(IDC_PROGRESS_VALUE, strValue);
	return 0;
}
void CImitateSpeedDlg::OnBnClickedBtnOpen()
{
	// TODO: Add your control notification handler code here
	CString strFileName;
	CFileDialog fileDlg(TRUE);
	fileDlg.m_ofn.lpstrFilter = L"TXT Files(*.txt)\0*.txt\0NC Files(*.NC)\0*.NC\0\0";
	if(IDOK == fileDlg.DoModal())
	{
		strFileName = fileDlg.GetPathName();

		AfxGetMainWnd()->SetWindowText(strFileName);
		LPTSTR lpsz = (LPTSTR)(LPCTSTR)strFileName;

		CWnd::FromHandle(hParentWnd)->Invalidate();
		::SendMessage(GetParentWnd(), WM_LOADFILE, (WPARAM)lpsz, 0);
	}
}