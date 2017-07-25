// MainFrm.cpp : CMainFrame 类的实现
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
	ID_SEPARATOR,           // 状态行指示器
	ID_MACHINE_POS,
	ID_WINDOWS_POS,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

	
// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
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
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}

	// TODO: 如果不需要工具栏可停靠，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	ShowWindow(SW_MAXIMIZE);
	//设置按钮文本
	m_wndToolBar.GetToolBarCtrl().SetBitmapSize(CSize(16, 16));

	m_wndToolBar.SetButtonText(0, L"新建");
	m_wndToolBar.SetButtonText(1, L"打开");
	m_wndToolBar.SetButtonText(2, L"保存");
	//3---分隔符
	m_wndToolBar.SetButtonText(4, L"剪切");
	m_wndToolBar.SetButtonText(5, L"复制");
	m_wndToolBar.SetButtonText(6, L"粘贴");
	//7---分隔符
	m_wndToolBar.SetButtonText(8, L"打印");
	m_wndToolBar.SetButtonText(9, L"关于");
	//10---分隔符
	m_wndToolBar.SetButtonText(11, L"刷新");
	//12---分隔符
	m_wndToolBar.SetButtonText(13, L"圆点");
	m_wndToolBar.SetButtonText(14, L"直线");
	m_wndToolBar.SetButtonText(15, L"整圆");
	m_wndToolBar.SetButtonText(16, L"圆弧");
	m_wndToolBar.SetButtonText(17, L"椭圆");
	m_wndToolBar.SetButtonText(18, L"矩形");
	//19---分隔符
	m_wndToolBar.SetButtonText(20, L"选择");
	m_wndToolBar.SetButtonText(21, L"放大");
	m_wndToolBar.SetButtonText(22, L"缩小");
	m_wndToolBar.SetButtonText(23, L"还原");
	m_wndToolBar.SetButtonText(24, L"适中");
	m_wndToolBar.SetButtonText(25, L"平移");
	//m_wndToolBar.SetButtonText(26,L"拖拽");

	CRect temp;
	m_wndToolBar.GetItemRect(0, &temp);
	m_wndToolBar.GetToolBarCtrl().SetButtonSize(CSize(temp.Width(), temp.Height()));

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	//cs.style &= ~WS_MAXIMIZEBOX; //禁止窗口最大化
	//cs.style &= ~WS_MINIMIZEBOX; //禁止窗口最小化
	//cs.style &= ~WS_THICKFRAME;//使窗口不能用鼠标改变大小
	cs.lpszClass = AfxRegisterWndClass(0);

	return TRUE;

}


// CMainFrame 诊断

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


// CMainFrame 消息处理程序