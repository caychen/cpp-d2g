// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "d2g.h"

#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_MACHINE_POS,
	ID_WINDOWS_POS,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

	
// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}

	// TODO: �������Ҫ��������ͣ������ɾ��������
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	ShowWindow(SW_MAXIMIZE);
	//���ð�ť�ı�
	m_wndToolBar.GetToolBarCtrl().SetBitmapSize(CSize(16, 16));

	m_wndToolBar.SetButtonText(0, L"�½�");
	m_wndToolBar.SetButtonText(1, L"��");
	m_wndToolBar.SetButtonText(2, L"����");
	//3---�ָ���
	m_wndToolBar.SetButtonText(4, L"����");
	m_wndToolBar.SetButtonText(5, L"����");
	m_wndToolBar.SetButtonText(6, L"ճ��");
	//7---�ָ���
	m_wndToolBar.SetButtonText(8, L"��ӡ");
	m_wndToolBar.SetButtonText(9, L"����");
	//10---�ָ���
	m_wndToolBar.SetButtonText(11, L"ˢ��");
	//12---�ָ���
	m_wndToolBar.SetButtonText(13, L"Բ��");
	m_wndToolBar.SetButtonText(14, L"ֱ��");
	m_wndToolBar.SetButtonText(15, L"��Բ");
	m_wndToolBar.SetButtonText(16, L"Բ��");
	m_wndToolBar.SetButtonText(17, L"��Բ");
	m_wndToolBar.SetButtonText(18, L"����");
	//19---�ָ���
	m_wndToolBar.SetButtonText(20, L"ѡ��");
	m_wndToolBar.SetButtonText(21, L"�Ŵ�");
	m_wndToolBar.SetButtonText(22, L"��С");
	m_wndToolBar.SetButtonText(23, L"��ԭ");
	m_wndToolBar.SetButtonText(24, L"����");
	m_wndToolBar.SetButtonText(25, L"ƽ��");
	//m_wndToolBar.SetButtonText(26,L"��ק");

	CRect temp;
	m_wndToolBar.GetItemRect(0, &temp);
	m_wndToolBar.GetToolBarCtrl().SetButtonSize(CSize(temp.Width(), temp.Height()));

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	//cs.style &= ~WS_MAXIMIZEBOX; //��ֹ�������
	//cs.style &= ~WS_MINIMIZEBOX; //��ֹ������С��
	//cs.style &= ~WS_THICKFRAME;//ʹ���ڲ��������ı��С
	cs.lpszClass = AfxRegisterWndClass(0);

	return TRUE;

}


// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame ��Ϣ�������