// d2gView.cpp : Cd2gView 类的实现
//

#include "stdafx.h"

#include "d2g.h"

#include "d2gDoc.h"
#include "d2gView.h"

#include <math.h>
#include "Test_CreationClass.h"
#include "d2gDoc.h"
#include <io.h>
#include "Global.h"
#include "UndoOperator.h"
//#include "SerialDll.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Cd2gView

CWnd* pClientWnd;
int CurrentImitateIndex = 0;
stack<CUndoOperator> g_UndoOperatorStack;

extern std::vector<int> tmSelect;
extern std::vector<ToolManage> tmVector;
extern BOOL bAutoBreath;

CString GetModuleFolderPath()
{
	CString strExePath = L"";
	GetModuleFileName(NULL, strExePath.GetBuffer(MAX_PATH), MAX_PATH);
	strExePath.ReleaseBuffer();

	strExePath = strExePath.Left(strExePath.ReverseFind(_T('\\')));	

	return strExePath;
}

IMPLEMENT_DYNCREATE(Cd2gView, CView)

BEGIN_MESSAGE_MAP(Cd2gView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)

	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, OnToolTipNotify)

	ON_COMMAND(ID_LINEBUTTON, OnDrawLine)
	ON_COMMAND(ID_CIRCLEBUTTON, OnDrawCirCle)
	ON_COMMAND(ID_ARCBUTTON, OnDrawArc)
	//ON_COMMAND(ID_ELLIPSEBUTTON, OnDrawEllipse)
	ON_COMMAND(IDM_SELECT, OnSelect)
	ON_COMMAND(ID_POINT, OnDrawPoint)
	ON_COMMAND(ID_RECTANGLEBUTTON, OnDrawRectangle)
	ON_COMMAND(ID_BIGGER, OnBigger)
	ON_COMMAND(ID_SMALLER, OnSmaller)
	ON_COMMAND(IDC_SCREEN_MOVE, OnScreenMove)
	ON_COMMAND(ID_SUITABLE, OnScreenSuitable)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_FILE_SAVE, &Cd2gView::OnFileSave)
	ON_COMMAND(IDM_SAVESETTING, &Cd2gView::OnSaveSetting)
	ON_COMMAND(ID_FILE_OPEN, &Cd2gView::OnFileOpen)
	ON_COMMAND(IDM_LINK, &Cd2gView::OnLink)
	ON_COMMAND(IDM_UNLINK, &Cd2gView::OnUnlink)
	ON_COMMAND(IDC_COPY, &Cd2gView::OnCopy)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(IDC_MOVE, &Cd2gView::OnMove)
	ON_COMMAND(IDC_DELETE, &Cd2gView::OnDelete)
	ON_COMMAND(IDC_ROTATE, &Cd2gView::OnRotate)
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_SAVE_AS, &Cd2gView::OnFileSaveAs)
	ON_COMMAND(IDC_SHOWDIRECTION, &Cd2gView::OnShowDirection)
	ON_WM_KEYDOWN()
	ON_COMMAND(IDC_RESERVEDIRECTION, &Cd2gView::OnReserveDirection)
	ON_COMMAND(IDC_UNSHOWDIRECTION, &Cd2gView::OnUnShowDirection)
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(IDC_REFRESH, &Cd2gView::OnRefresh)
	ON_COMMAND(ID_REFRESHBUTTON, OnRefresh)
	ON_COMMAND(ID_SETPOINT, &Cd2gView::OnSetPoint)

	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_UPDATE_COMMAND_UI(IDM_LINK, &Cd2gView::OnUpdateLink)
	ON_UPDATE_COMMAND_UI(IDM_UNLINK, &Cd2gView::OnUpdateUnlink)
	ON_COMMAND(ID_FILE_NEW, &Cd2gView::OnFileNew)
	ON_COMMAND(IDC_BOTH_GROW, &Cd2gView::OnBothGrow)
	ON_COMMAND(IDC_FRONT_GROW, &Cd2gView::OnFrontGrow)
	ON_COMMAND(IDC_ENDING_GROW, &Cd2gView::OnEndingGrow)
	ON_COMMAND(IDC_MERGE, &Cd2gView::OnMerge)
	ON_COMMAND(ID_RESET, &Cd2gView::OnReSet)
	ON_COMMAND(ID_BETTER_PATH, &Cd2gView::OnBetterPath)

	ON_COMMAND(ID_SETLINE_START_AND_END, &Cd2gView::OnSetLineStartAndEnd)
	ON_COMMAND(ID_SETCIRCLE_CENTER_AND_RADIUS, &Cd2gView::OnSetCircleCenterAndRadius)
	ON_COMMAND(ID_SETARC_CENTER_RAIDUS_ANGLE, &Cd2gView::OnSetArcCenterRaidusAngle)
	ON_COMMAND(ID_SETRECTANGLE_LEFTTOP_WIDTH_AND_HEIGHT, &Cd2gView::OnSetRectangleLeftTopWidthAndHeight)
	ON_COMMAND(ID_SETRECTNAGLE_LEFTTOP_AND_RIGHTBOTTOM, &Cd2gView::OnSetRectnagleLeftTopAndRightBottom)
	ON_COMMAND(IDM_PRUNE, &Cd2gView::OnPrune)
	ON_COMMAND(IDM_SELECT_ALL, &Cd2gView::OnSelectAll)
	ON_COMMAND(IDC_SELECTALL, &Cd2gView::OnSelectAll)
	ON_COMMAND(ID_END_COMMAND, &Cd2gView::OnEndCommand)
	ON_WM_LBUTTONUP()
	ON_UPDATE_COMMAND_UI(IDC_ADJUSTLOCATION, &Cd2gView::OnUpdateAdjustLocation)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_COMMAND(ID_CONTROL_SPEED, &Cd2gView::OnControlSpeed)
	ON_MESSAGE(WM_LOADFILE, &Cd2gView::OnLoadFile)
	ON_COMMAND(IDM_TOOLMANAGE, &Cd2gView::OnToolManage)
	ON_COMMAND(IDM_CANCEL_GROW, &Cd2gView::OnCancelGrow)
	ON_COMMAND(IDM_SET_NEWSTEP, &Cd2gView::OnSetNewStep)
	ON_COMMAND(IDM_CANCEL_NEWSTEP, &Cd2gView::OnCancelNewStep)
	ON_COMMAND(IDM_SET_NEWFEEDSPEED, &Cd2gView::OnSetNewFeedSpeed)
	ON_COMMAND(IDM_CANCEL_NEWFEEDSPEED, &Cd2gView::OnCancelNewFeedSpeed)
	ON_COMMAND(IDM_SHOW_BETTER_PATH, &Cd2gView::OnShowBetterPath)
	ON_COMMAND(IDM_MODIFY_POINT_DEPTH, &Cd2gView::OnModifyPointDepth)
	ON_COMMAND(IDM_CANCEL_POINT_DEPTH, &Cd2gView::OnCancelPointDepth)
END_MESSAGE_MAP()


// Cd2gView 构造/析构

Cd2gView::Cd2gView()
: m_iDrawType(0)
, m_startPt(0)
, m_oldPt(0)
, m_iArcCount(0)
, m_bFlag(FALSE)//默认为FALSE
, m_savePoint(0)
, m_lastPt(0)
, m_fCircleRadius(0)
, pGraph(NULL)
, qGraph(NULL)
, nuArray(NULL)
, m_filePathName(_T(""))
, m_bIsOrder(FALSE)
, m_bIsLink(FALSE)
, rectStartPoint(0)
, rectEndPoint(0)
, m_OperationPoint(0, 0)
, m_bIsOnMove(FALSE)
, m_movePoint(0, 0)
, color(_T("Black"))
, m_bIsOnRotate(FALSE)
, m_savePath(_T(""))
, m_DefaultFileName(_T("程序.txt"))
, m_fileName(L"")
, m_iEllipseCount(0)
, m_centerPoint(0)
, m_firstPoint(0)
, m_secondPoint(0)
, m_fMajorLength(0)
, m_fMinorLength(0)
, m_fAngle(0)
, m_iLineCount(0)
, m_iVirtualRectCount(0)
, m_iRectCount(0)
, m_iCircleCount(0)
, pt2Array(CPoint2D(0, 0), CPoint2D(0, 0))
, pt3Array(CPoint2D(0, 0), CPoint2D(0, 0), CPoint2D(0, 0))
, m_nVSize(0)
, m_nHSize(0)
, m_iWorkStation(CARVE)
, m_fPointRadius(0)
, m_polySelectNum(0)
, m_CommandType(CT_NONE)
, m_iMoveCount(0)
, m_downPoint(0)
, m_oldMovePoint(0)
, m_RealNcPathNum(0)
, m_screenMoveStartPoint(0, 0)
, oldPBrush(NULL)
, oldPen(NULL)
, pen2(NULL)
, m_OldHSetOffset(INIT_HSETOFFSET)
, m_OldVSetOffset(INIT_VSETOFFSET)
, m_bStartImitate(FALSE)
, m_bSetTimerFlag(FALSE)
{ 
	// TODO: 在此处添加构造代码
	pen1 = new CPen(PS_SOLID, 1, RGB(0, 0, 0));
	selectPen = new CPen(PS_SOLID, 1, RGB(255, 0, 0));

	m_strSaveSettingPath = L"";

	TCHAR* oldLocale = _tcsdup( _tsetlocale(LC_CTYPE, NULL) );
	_tsetlocale( LC_CTYPE, L"chs" );//设定<ctpye.h>中字符处理方式

	ToolManage tm;
	ZeroMemory(&tm, sizeof(tm));

	CString strExeFolderName = GetModuleFolderPath();
	CString ConfigToolManageFilePath = strExeFolderName + _T("\\") + _T("ToolManage.ini");

	FILE *fp = NULL;
	_tfopen_s(&fp, ConfigToolManageFilePath, L"r");
	if(fp)
	{
		while(!feof(fp))
		{
			fwscanf_s(fp, L"%s %s %lf %d", 
				tm.ToolNo, 10,
				tm.ToolName, 10,
				&tm.ToolRadius, 
				&tm.RotationRate);
			tmVector.push_back(tm);
		}
		fclose(fp);
	}
	//07-06修改Cf
	//Start
	else
	{
		tmVector.clear();
	}
	//End

	//tmSelect.push_back(0);

	_tsetlocale( LC_CTYPE, oldLocale );
	free( oldLocale );//还原区域设定

	m_strSaveSettingPath = GetModuleFolderPath() + _T("\\") + _T("Setting.ini");

	m_HCross = AfxGetApp()->LoadStandardCursor(IDC_CROSS);//初始化为十字光标

	InitializeCriticalSection(&g_csObject);

}

Cd2gView::~Cd2gView()
{
	DeleteCriticalSection(&g_csObject);
	ClearData();
}

void Cd2gView::ClearData(void)
{
	m_iDrawType = 0;
	m_startPt = 0;
	m_oldPt = 0;
	m_iArcCount = 0;
	m_bFlag = FALSE;//默认为FALSE
	m_savePoint = 0;
	m_lastPt = 0;
	m_fCircleRadius = 0;
	m_filePathName = _T("");
	m_fileName = L"";
	m_bIsOrder = FALSE;
	m_bIsLink = FALSE;
	rectStartPoint = 0;
	rectEndPoint = 0;
	m_OperationPoint = 0;
	m_bIsOnMove = FALSE;
	m_movePoint = 0;
	color = _T("Black");
	m_bIsOnRotate = FALSE;
	m_savePath = _T("");
	m_DefaultFileName = L"";
	m_iEllipseCount = 0;
	m_centerPoint = 0;
	m_firstPoint = 0;
	m_secondPoint = 0;
	m_fMajorLength = 0;
	m_fMinorLength = 0;
	m_fAngle = 0;
	m_iLineCount = 0;
	m_iVirtualRectCount = 0;
	m_iCircleCount = 0;
	pt2Array = CPointArray(CPoint2D(0, 0), CPoint2D(0, 0));
	pt3Array = CPointArray(CPoint2D(0, 0), CPoint2D(0, 0), CPoint2D(0, 0));
	m_nVSize = 0;
	m_nHSize = 0;

	m_fPointRadius = 0;
	m_polySelectNum = 0;

	g_iSpacing = 5;							//标尺间距
	g_fRate = 1;
	g_vSetOffset = INIT_VSETOFFSET;
	g_hSetOffset = INIT_HSETOFFSET;
	m_OldHSetOffset = INIT_HSETOFFSET;
	m_OldVSetOffset = INIT_VSETOFFSET;

	g_vOffset = 0;
	g_hOffset = 0;
	m_CommandType = CT_NONE;
	m_RealNcPathNum = 0;

	//Graph*
	if(pGraph != NULL)
	{
		delete pGraph;
		pGraph = NULL;
	}
	if(qGraph != NULL)
	{
		delete qGraph;
		qGraph = NULL;
	}
	if(nuArray != NULL)
	{
		delete []nuArray;
		nuArray = NULL;
	}
	if(pAllBrush)
	{
		pAllBrush->DeleteObject();
		pAllBrush = NULL;
	}
	if(oldPBrush)
	{
		oldPBrush->DeleteObject();
		oldPBrush = NULL;
	}
	if(oldPen)
	{
		oldPen->DeleteObject();
		oldPen = NULL;
	}
	if(pen1)
	{
		pen1->DeleteObject();
		pen1 = NULL;
	}
	if(pen2)
	{
		pen2->DeleteObject();
		pen2 = NULL;
	}
	if(selectPen)
	{
		selectPen->DeleteObject();
		selectPen = NULL;
	}
	if(g_idNo)
	{
		delete []g_idNo;
		g_idNo = NULL;
	}

	if(g_BetterPathPoint)
	{
		ZeroMemory(g_BetterPathPoint, sizeof(g_BetterPathPoint));
		g_BetterPathPoint = NULL;
	}

	for(int i = 0 ; i < m_RealNcPathNum; ++i)
		m_OutPutArray[i].RemoveAll();
	m_OutPutArray->RemoveAll();

	g_GraphArray.RemoveAll();
	g_CircleArray.RemoveAll();
	g_ArcArray.RemoveAll();
	g_EllipseArray.RemoveAll();
	g_PolyArray.RemoveAll();
	g_TextArray.RemoveAll();

	m_PolyLineArray.RemoveAll();
	g_SplineArray.RemoveAll();

	g_OperationArray.RemoveAll();
	g_SaveArray.RemoveAll();
	g_NodeArray.RemoveAll();
}

BOOL Cd2gView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// Cd2gView 绘制


/***********************************
** 重绘函数
** 参数
** 时间：2013-5-10
***********************************/
void Cd2gView::OnDraw(CDC *pDC)
{
	Cd2gDoc* pDoc = GetDocument();

	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;

	CPoint2D startPoint, endPoint, centerPoint;
	double fMajor = 0, fMinor = 0, degree = 0;
	BOOL bDirection;
	double fRadius;
	UINT type;
	double kRate = 0.0;
	CPoint2D p1, p2, p3;

	CPoint2D ptStart, ptEnd1, ptEnd2;
	CPoint2D graphStartPoint;
	double lastAngle = 0.0, startAngle = 0, endAngle = 0;
	double lastX = 0.0, lastY = 0.0, nextX = 0.0, nextY = 0.0;
	double angle = 0.0;

	COLORREF clrGraph;
	CArray <CPoint2D>PeakGroup;

	int VertexNum, uFlags;
	double *fPoint;
	CPoint2D ptM;

	CBspline bCurve;
	vector<vec2>NodeArray;
	NodeArray.clear();

	UINT cutType = 0;
	UINT uPrg = 0;

	CRect rt;
	GetClientRect(&rt);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	HBITMAP hBitmap = ::CreateCompatibleBitmap(pDC->m_hDC, rt.Width(), rt.Height());
	HBITMAP hOldBitmap = (HBITMAP)memDC.SelectObject(hBitmap);

	memDC.FillSolidRect(0, 0, rt.Width(), rt.Height(), RGB(255, 255, 255));

	pAllBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	//pDC->SelectObject(pAllBrush);
	oldPBrush = memDC.SelectObject(pAllBrush);

	CPen orgPen(PS_SOLID, 1, RGB(255,0,0)), *pOldOrgPen, *oldPen1;
	pOldOrgPen = memDC.SelectObject(&orgPen);

	CPoint2D orgPoint(0,0);
	CPoint2D tmpPoint = orgPoint;
	orgPoint = funArray.MyQuadrantToScreen(orgPoint) + CPoint2D(-g_hOffset,g_vOffset);
	memDC.MoveTo(orgPoint.x - 5, orgPoint.y);
	memDC.LineTo(orgPoint.x + 20, orgPoint.y);
	memDC.MoveTo(orgPoint.x, orgPoint.y - 20);
	memDC.LineTo(orgPoint.x, orgPoint.y + 5);
	memDC.SelectObject(pOldOrgPen);

	/*if(!g_RectNew.IsRectEmpty())
	{*/

	funArray.MyQuadrantToScreen1(tmpPoint);
	POINT ltpt = {0,0}, lbpt = {0,0}, rtpt = {0,0}, rbpt = {0,0};
	lbpt.x = tmpPoint.x, lbpt.y = tmpPoint.y;
	ltpt.x = tmpPoint.x, ltpt.y = lbpt.y - g_iWidth / g_fRate;
	rbpt.x = tmpPoint.x + g_iLength / g_fRate, rbpt.y = tmpPoint.y;
	rtpt.x = rbpt.x, rtpt.y = ltpt.y;

	pOldOrgPen = memDC.SelectObject(&orgPen);
	memDC.MoveTo(lbpt);
	memDC.LineTo(ltpt);
	memDC.LineTo(rtpt);
	memDC.LineTo(rbpt);
	memDC.LineTo(lbpt);
	memDC.SelectObject(&pOldOrgPen);

	//}

	// TODO: 在此处为本机数据添加绘制代码
	if(CT_IMITATE != m_CommandType)//
	{
		int i;
		for(i = 0; i < g_GraphArray.GetSize(); ++i)
		{
			type = ((CGraph*)g_GraphArray.GetAt(i))->m_iDrawType;

			if(m_iWorkStation != WATERJET)
			{
				clrGraph = ((CGraph*)g_GraphArray.GetAt(i))->clrREF;

				if(pen2)
				{
					pen2->DeleteObject();
					pen2 = NULL;
				}
				pen2 = new CPen(PS_SOLID, 1, clrGraph);
				//oldPen = (CPen*)pDC->SelectObject(pen2);
				oldPen = (CPen*)memDC.SelectObject(pen2);
			}
			else
			{
				if(m_CommandType != CT_ADJUSTLOCATION)
					//oldPen = (CPen*)pDC->SelectObject(pen1);
					oldPen = (CPen*)memDC.SelectObject(pen1);
				else
				{
					if(((CGraph*)g_GraphArray.GetAt(i))->m_bIsPitched)
						//oldPen = (CPen*)pDC->SelectObject(selectPen);
						oldPen = (CPen*)memDC.SelectObject(selectPen);
					else
						//oldPen = (CPen*)pDC->SelectObject(pen1);
						oldPen = (CPen*)memDC.SelectObject(pen1);
				}
			}

			switch(type)
			{
			case 2:
				startPoint = ((CGraph*)g_GraphArray.GetAt(i))->m_ptOrigin;
				endPoint = ((CGraph*)g_GraphArray.GetAt(i))->m_ptEnd;

				startPoint /= g_fRate;
				endPoint /= g_fRate;

				startPoint = funArray.MyQuadrantToScreen(startPoint);
				endPoint = funArray.MyQuadrantToScreen(endPoint);

				/*pDC->MoveTo(startPoint.x,startPoint.y);
				pDC->LineTo(endPoint.x,endPoint.y);*/
				memDC.MoveTo(startPoint.x, startPoint.y);
				memDC.LineTo(endPoint.x, endPoint.y);
				break;

			case 3:
				endPoint = ((CGraph*)g_GraphArray.GetAt(i))->m_ptCenter;
				fRadius = ((CGraph*)g_GraphArray.GetAt(i))->m_fRadius;
				bDirection = ((CGraph*)g_GraphArray.GetAt(i))->m_bDirection;

				//pDC->Ellipse(CRect((endPoint.x - fRadius) / g_fRate + 20 + g_hSetOffset
				//	,g_Rect.Height() - g_vSetOffset - (endPoint.y - fRadius) / g_fRate
				//	,(endPoint.x + fRadius) / g_fRate + 20 + g_hSetOffset
				//	,g_Rect.Height() - g_vSetOffset - (endPoint.y + fRadius) / g_fRate));

				memDC.Ellipse(CRect((endPoint.x - fRadius) / g_fRate + 20 + g_hSetOffset
					, g_Rect.Height() - g_vSetOffset - (endPoint.y - fRadius) / g_fRate
					, (endPoint.x + fRadius) / g_fRate + 20 + g_hSetOffset
					, g_Rect.Height() - g_vSetOffset - (endPoint.y + fRadius) / g_fRate));
				break;

			case 4:
				startPoint = ((CGraph*)g_GraphArray.GetAt(i))->m_ptOrigin;
				centerPoint = ((CGraph*)g_GraphArray.GetAt(i))->m_ptCenter;
				endPoint = ((CGraph*)g_GraphArray.GetAt(i))->m_ptEnd;
				fRadius = ((CGraph*)g_GraphArray.GetAt(i))->m_fRadius ;
				bDirection = ((CGraph*)g_GraphArray.GetAt(i))->m_bDirection;

				if((int)startPoint.x / g_fRate == (int)endPoint.x / g_fRate && (int)startPoint.y / g_fRate == (int)endPoint.y / g_fRate)
				{
					startPoint /= g_fRate;
					startPoint = funArray.MyQuadrantToScreen(startPoint);

					//pDC->SetPixel(CPoint(startPoint.x,startPoint.y),clrGraph);
					memDC.SetPixel(CPoint(startPoint.x, startPoint.y), clrGraph);
					break;
				}
				else if(funArray.GetDistance(startPoint / g_fRate, endPoint / g_fRate) < 3)
				{
					startPoint /= g_fRate;
					endPoint /= g_fRate;
					startPoint = funArray.MyQuadrantToScreen(startPoint);
					endPoint = funArray.MyQuadrantToScreen(endPoint);

					//pDC->MoveTo(startPoint.x,startPoint.y);
					//pDC->LineTo(endPoint.x,endPoint.y);
					memDC.MoveTo(startPoint.x, startPoint.y);
					memDC.LineTo(endPoint.x, endPoint.y);
					break;
				}

				if(bDirection)
					/*pDC->Arc(CRect((centerPoint.x - fRadius) / g_fRate + 20 + g_hSetOffset,
					g_Rect.Height() - g_vSetOffset - (centerPoint.y  - fRadius) / g_fRate,
					(centerPoint.x + fRadius) / g_fRate + 20 + g_hSetOffset,
					g_Rect.Height() - g_vSetOffset - (centerPoint.y + fRadius) / g_fRate),
					CPoint(endPoint.x / g_fRate + 20 + g_hSetOffset,
					g_Rect.Height() - g_vSetOffset - endPoint.y / g_fRate),
					CPoint(startPoint.x / g_fRate + 20 + g_hSetOffset,
					g_Rect.Height() - g_vSetOffset - startPoint.y / g_fRate));*/

					memDC.Arc(CRect((centerPoint.x - fRadius) / g_fRate + 20 + g_hSetOffset, 
					g_Rect.Height() - g_vSetOffset - (centerPoint.y  - fRadius) / g_fRate, 
					(centerPoint.x + fRadius) / g_fRate + 20 + g_hSetOffset, 
					g_Rect.Height() - g_vSetOffset - (centerPoint.y + fRadius) / g_fRate), 
					CPoint(endPoint.x / g_fRate + 20 + g_hSetOffset, 
					g_Rect.Height() - g_vSetOffset - endPoint.y / g_fRate), 
					CPoint(startPoint.x / g_fRate + 20 + g_hSetOffset, 
					g_Rect.Height() - g_vSetOffset - startPoint.y / g_fRate));
				else
					/*pDC->Arc(CRect((centerPoint.x - fRadius) / g_fRate + 20 + g_hSetOffset,
					g_Rect.Height() - g_vSetOffset - (centerPoint.y  - fRadius) / g_fRate,
					(centerPoint.x + fRadius) / g_fRate + 20 + g_hSetOffset,
					g_Rect.Height() - g_vSetOffset - (centerPoint.y + fRadius) / g_fRate),
					CPoint(startPoint.x / g_fRate + 20 + g_hSetOffset,
					g_Rect.Height() - g_vSetOffset - startPoint.y / g_fRate),
					CPoint(endPoint.x / g_fRate + 20 + g_hSetOffset,
					g_Rect.Height() - g_vSetOffset - endPoint.y / g_fRate));*/

					memDC.Arc(CRect((centerPoint.x - fRadius) / g_fRate + 20 + g_hSetOffset, 
					g_Rect.Height() - g_vSetOffset - (centerPoint.y  - fRadius) / g_fRate, 
					(centerPoint.x + fRadius) / g_fRate + 20 + g_hSetOffset, 
					g_Rect.Height() - g_vSetOffset - (centerPoint.y + fRadius) / g_fRate), 
					CPoint(startPoint.x / g_fRate + 20 + g_hSetOffset, 
					g_Rect.Height() - g_vSetOffset - startPoint.y / g_fRate), 
					CPoint(endPoint.x / g_fRate + 20 + g_hSetOffset, 
					g_Rect.Height() - g_vSetOffset - endPoint.y / g_fRate));
				break;

			case 5:
				startPoint = ((CGraph*)g_GraphArray.GetAt(i))->m_ptOrigin;
				endPoint = ((CGraph*)g_GraphArray.GetAt(i))->m_ptEnd;
				startPoint /= g_fRate;
				endPoint /= g_fRate;

				startPoint = funArray.MyQuadrantToScreen(startPoint);
				endPoint = funArray.MyQuadrantToScreen(endPoint);

				//pDC->Rectangle(startPoint.x,startPoint.y,endPoint.x,endPoint.y);
				memDC.Rectangle(startPoint.x, startPoint.y, endPoint.x, endPoint.y);
				break;

			case 6:
				fMajor = ((CGraph*)g_GraphArray.GetAt(i))->m_fMajor;
				fMinor = ((CGraph*)g_GraphArray.GetAt(i))->m_fMinor;
				degree = ((CGraph*)g_GraphArray.GetAt(i))->m_fDegree;
				centerPoint = ((CGraph*)g_GraphArray.GetAt(i))->m_ptCenter;
				bDirection = ((CGraph*)g_GraphArray.GetAt(i))->m_bDirection;

				startAngle = ((CGraph*)g_GraphArray.GetAt(i))->m_fStartAngle;
				endAngle = ((CGraph*)g_GraphArray.GetAt(i))->m_fEndAngle;

				startPoint = funArray.GetEllipsePoint(fMajor, fMinor, degree, centerPoint, 
					startAngle * 180 / PI, bDirection);
				startPoint /= g_fRate;
				ptM = funArray.MyQuadrantToScreen(startPoint);

				//pDC->MoveTo(startPoint.x / g_fRate + 20 + g_hSetOffset,
				/*memDC.MoveTo(startPoint.x / g_fRate + 20 + g_hSetOffset,
				g_Rect.Height() - startPoint.y / g_fRate - g_vSetOffset);*/
				memDC.MoveTo(ptM.x, ptM.y);
				for (int m = (int)(startAngle * 180 / PI); m <= (int)(endAngle * 180 / PI) + 1; m++)
				{
					//pDC->LineTo(CPoint(funArray.GetEllipsePoint(fMajor,fMinor,degree,centerPoint,
					//m,bDirection).x / g_fRate + 20 + g_hSetOffset,
					//g_Rect.Height() - g_vSetOffset - funArray.GetEllipsePoint(fMajor,fMinor,degree,centerPoint,m,bDirection).y / g_fRate));
					ptM = funArray.GetEllipsePoint(fMajor, fMinor, degree, centerPoint, 
						m, bDirection);
					ptM /= g_fRate;
					ptM = funArray.MyQuadrantToScreen(ptM);

					/*memDC.LineTo(CPoint(funArray.GetEllipsePoint(fMajor,fMinor,degree,centerPoint,
					m,bDirection).x / g_fRate + 20 + g_hSetOffset,
					g_Rect.Height() - g_vSetOffset - funArray.GetEllipsePoint(fMajor,fMinor,degree,centerPoint,m,bDirection).y / g_fRate));*/
					memDC.LineTo(ptM.x,ptM.y);
				}
				break;

			case 7:
				VertexNum = ((CGraph*)g_GraphArray.GetAt(i))->m_iVertexNum;
				uFlags = ((CGraph*)g_GraphArray.GetAt(i))->m_bFlags;
				PeakGroup.Copy(((CGraph*)g_GraphArray.GetAt(i))->m_PeakGroup);
				fPoint = new double[3 * VertexNum];
				for(int j = 0; j < VertexNum; ++j)
				{
					fPoint[j * 3]	 = PeakGroup[j].x;
					fPoint[j * 3 + 1] = PeakGroup[j].y;
					fPoint[j * 3 + 2] = PeakGroup[j].con;
				}

				startPoint = CPoint2D(fPoint[0], fPoint[1]);

				for (int j = 0;j < VertexNum - 1;++j)
				{
					startPoint.x = fPoint[j * 3], startPoint.y = fPoint[j * 3 + 1];
					endPoint.x = fPoint[(j + 1) * 3], endPoint.y = fPoint[(j + 1) * 3 + 1];

					lastAngle = fPoint[j * 3 + 2];
					if(lastAngle == 0.0)
					{
						//pDC->MoveTo(CPoint(fPoint[j * 3] / g_fRate + 20 + g_hSetOffset,
						//	g_Rect.Height() - g_vSetOffset - fPoint[j * 3 + 1] / g_fRate));
						//pDC->LineTo(CPoint(fPoint[(j + 1) * 3] / g_fRate + 20 + g_hSetOffset,
						//	g_Rect.Height() - g_vSetOffset - fPoint[(j + 1) * 3 + 1] / g_fRate));

						memDC.MoveTo(CPoint(fPoint[j * 3] / g_fRate + 20 + g_hSetOffset, 
							g_Rect.Height() - g_vSetOffset - fPoint[j * 3 + 1] / g_fRate));
						memDC.LineTo(CPoint(fPoint[(j + 1) * 3] / g_fRate + 20 + g_hSetOffset, 
							g_Rect.Height() - g_vSetOffset - fPoint[(j + 1) * 3 + 1] / g_fRate));
					}
					else
					{
						angle = 4 * atan(fabs(fPoint[j * 3 + 2]));
						fRadius = funArray.GetDistance(fPoint[j * 3], fPoint[j * 3 + 1], 
							fPoint[(j + 1) * 3], fPoint[(j + 1) * 3 + 1]) / (2 * sin(angle / 2));

						if(lastAngle > 0)//正为逆，负为顺
						{
							centerPoint = funArray.GetCenterPoint(fPoint[j * 3], fPoint[j * 3 + 1], 
								fPoint[(j + 1) * 3], fPoint[(j + 1) * 3 + 1], fRadius, angle, FALSE);

							startPoint = CPoint2D(fPoint[j * 3] / g_fRate + 20 + g_hSetOffset, 
								g_Rect.Height() - g_vSetOffset - (fPoint[j * 3 + 1] / g_fRate));
							endPoint = CPoint2D(fPoint[(j + 1) * 3] / g_fRate + 20 + g_hSetOffset, 
								g_Rect.Height() - g_vSetOffset - (fPoint[(j + 1) * 3 + 1] / g_fRate));

							if(fabs(startPoint.x - endPoint.x) < 1 && fabs(startPoint.y - endPoint.y) < 1)
							{
								//pDC->MoveTo(startPoint.x,startPoint.y);
								//pDC->LineTo(endPoint.x,endPoint.y);

								memDC.MoveTo(startPoint.x, startPoint.y);
								memDC.LineTo(endPoint.x, endPoint.y);
								continue;
							}

							//pDC->Arc(CRect((centerPoint.x - fRadius) / g_fRate + 20 + g_hSetOffset,
							//	g_Rect.Height() - g_vSetOffset - (centerPoint.y - fRadius) / g_fRate,
							//	(centerPoint.x + fRadius) / g_fRate + 20 + g_hSetOffset,
							//	g_Rect.Height() - g_vSetOffset - (centerPoint.y + fRadius) / g_fRate),
							//	CPoint(startPoint.x,startPoint.y),CPoint(endPoint.x,endPoint.y));

							memDC.Arc(CRect((centerPoint.x - fRadius) / g_fRate + 20 + g_hSetOffset, 
								g_Rect.Height() - g_vSetOffset - (centerPoint.y - fRadius) / g_fRate, 
								(centerPoint.x + fRadius) / g_fRate + 20 + g_hSetOffset, 
								g_Rect.Height() - g_vSetOffset - (centerPoint.y + fRadius) / g_fRate), 
								CPoint(startPoint.x, startPoint.y), CPoint(endPoint.x, endPoint.y));
						}
						else
						{
							centerPoint = funArray.GetCenterPoint(fPoint[j * 3], fPoint[j * 3 + 1], 
								fPoint[(j + 1) * 3], fPoint[(j + 1) * 3 + 1], fRadius, angle, TRUE);

							startPoint = CPoint2D(fPoint[j * 3] / g_fRate + 20 + g_hSetOffset, 
								g_Rect.Height() - g_vSetOffset - (fPoint[j * 3 + 1] / g_fRate));
							endPoint = CPoint2D(fPoint[(j + 1) * 3] / g_fRate + 20 + g_hSetOffset, 
								g_Rect.Height() - g_vSetOffset - (fPoint[(j + 1) * 3 + 1] / g_fRate));

							if(fabs(startPoint.x - endPoint.x) < 1 && fabs(startPoint.y - endPoint.y) < 1)
							{
								//pDC->MoveTo(endPoint.x,endPoint.y);
								//pDC->LineTo(startPoint.x,startPoint.y);

								memDC.MoveTo(endPoint.x, endPoint.y);
								memDC.LineTo(startPoint.x, startPoint.y);
								continue;
							}

							//pDC->Arc(CRect((centerPoint.x - fRadius) / g_fRate + 20 + g_hSetOffset,
							//	g_Rect.Height() - g_vSetOffset - (centerPoint.y - fRadius) / g_fRate,
							//	(centerPoint.x + fRadius) / g_fRate + 20 + g_hSetOffset,
							//	g_Rect.Height() - g_vSetOffset - (centerPoint.y + fRadius) / g_fRate)
							//	,CPoint(endPoint.x,endPoint.y),CPoint(startPoint.x,startPoint.y));

							memDC.Arc(CRect((centerPoint.x - fRadius) / g_fRate + 20 + g_hSetOffset, 
								g_Rect.Height() - g_vSetOffset - (centerPoint.y - fRadius) / g_fRate, 
								(centerPoint.x + fRadius) / g_fRate + 20 + g_hSetOffset, 
								g_Rect.Height() - g_vSetOffset - (centerPoint.y + fRadius) / g_fRate)
								, CPoint(endPoint.x, endPoint.y), CPoint(startPoint.x, startPoint.y));
						}
					}
				}

				if(uFlags == 1)
				{
					startPoint.x = fPoint[(VertexNum - 1) * 3], startPoint.y = fPoint[(VertexNum - 1) * 3 + 1];
					endPoint.x = fPoint[0], endPoint.y = fPoint[1];
					lastAngle = fPoint[(VertexNum - 1) * 3 + 2];

					if(lastAngle == 0.0)
					{
						//pDC->MoveTo(CPoint(fPoint[(VertexNum - 1) * 3] / g_fRate + 20 + g_hSetOffset,
						//	g_Rect.Height() - g_vSetOffset - (fPoint[(VertexNum - 1) * 3 + 1] / g_fRate)));
						//pDC->LineTo(CPoint(fPoint[0] / g_fRate + 20 + g_hSetOffset,
						//	g_Rect.Height() - g_vSetOffset - fPoint[1] / g_fRate));

						memDC.MoveTo(CPoint(fPoint[(VertexNum - 1) * 3] / g_fRate + 20 + g_hSetOffset, 
							g_Rect.Height() - g_vSetOffset - (fPoint[(VertexNum - 1) * 3 + 1] / g_fRate)));
						memDC.LineTo(CPoint(fPoint[0] / g_fRate + 20 + g_hSetOffset, 
							g_Rect.Height() - g_vSetOffset - fPoint[1] / g_fRate));
					}
					else
					{
						angle = 4 * atan(fabs(lastAngle));
						fRadius = funArray.GetDistance(fPoint[0],fPoint[1], 
							fPoint[(VertexNum - 1) * 3], 
							fPoint[(VertexNum - 1) * 3 + 1]) / (2 * sin(angle / 2));

						if(lastAngle > 0)
						{
							centerPoint = funArray.GetCenterPoint(fPoint[(VertexNum - 1) * 3], fPoint[(VertexNum - 1) * 3 + 1], 
								fPoint[0], fPoint[1], fRadius, angle, FALSE);

							startPoint = CPoint2D(fPoint[(VertexNum - 1) * 3] / g_fRate + 20 + g_hSetOffset, 
								g_Rect.Height() - g_vSetOffset - (fPoint[(VertexNum - 1) * 3 + 1] / g_fRate));
							endPoint = CPoint2D(fPoint[0] / g_fRate + 20 + g_hSetOffset, 
								g_Rect.Height() - g_vSetOffset - (fPoint[1] / g_fRate));

							if(fabs(startPoint.x - endPoint.x) < 1 && fabs(startPoint.y - endPoint.y) < 1)
							{
								//pDC->MoveTo(startPoint.x,startPoint.y);
								//pDC->LineTo(endPoint.x,endPoint.y);

								memDC.MoveTo(startPoint.x, startPoint.y);
								memDC.LineTo(endPoint.x, endPoint.y);
								continue;
							}

							//pDC->Arc(CRect((centerPoint.x - fRadius) / g_fRate + 20 + g_hSetOffset,
							//	g_Rect.Height() - g_vSetOffset - (centerPoint.y - fRadius) / g_fRate,
							//	(centerPoint.x + fRadius) / g_fRate + 20 + g_hSetOffset,
							//	g_Rect.Height() - g_vSetOffset - (centerPoint.y + fRadius) / g_fRate)
							//	,CPoint(startPoint.x,startPoint.y),CPoint(endPoint.x,endPoint.y));

							memDC.Arc(CRect((centerPoint.x - fRadius) / g_fRate + 20 + g_hSetOffset, 
								g_Rect.Height() - g_vSetOffset - (centerPoint.y - fRadius) / g_fRate, 
								(centerPoint.x + fRadius) / g_fRate + 20 + g_hSetOffset, 
								g_Rect.Height() - g_vSetOffset - (centerPoint.y + fRadius) / g_fRate)
								, CPoint(startPoint.x, startPoint.y), CPoint(endPoint.x, endPoint.y));
						}
						else
						{
							centerPoint = funArray.GetCenterPoint(fPoint[(VertexNum - 1) * 3], fPoint[(VertexNum - 1) * 3 + 1], 
								fPoint[0], fPoint[1], fRadius, angle, TRUE);

							startPoint = CPoint2D(fPoint[(VertexNum - 1) * 3] / g_fRate + 20 + g_hSetOffset, 
								g_Rect.Height() - g_vSetOffset - (fPoint[(VertexNum - 1) * 3 + 1] / g_fRate));
							endPoint = CPoint2D(fPoint[0] / g_fRate + 20 + g_hSetOffset, 
								g_Rect.Height() - g_vSetOffset - (fPoint[1] / g_fRate));

							if(fabs(startPoint.x - endPoint.x) < 1 && fabs(startPoint.y - endPoint.y) < 1)
							{
								//pDC->MoveTo(endPoint.x,endPoint.y);
								//pDC->LineTo(startPoint.x,startPoint.y);

								memDC.MoveTo(endPoint.x, endPoint.y);
								memDC.LineTo(startPoint.x, startPoint.y);
								continue;
							}

							//pDC->Arc(CRect((centerPoint.x - fRadius) / g_fRate + 20 + g_hSetOffset,
							//	g_Rect.Height() - g_vSetOffset - (centerPoint.y - fRadius) / g_fRate,
							//	(centerPoint.x + fRadius) / g_fRate + 20 + g_hSetOffset,
							//	g_Rect.Height() - g_vSetOffset - (centerPoint.y + fRadius) / g_fRate)
							//	,CPoint(endPoint.x,endPoint.y),CPoint(startPoint.x,startPoint.y));

							memDC.Arc(CRect((centerPoint.x - fRadius) / g_fRate + 20 + g_hSetOffset, 
								g_Rect.Height() - g_vSetOffset - (centerPoint.y - fRadius) / g_fRate, 
								(centerPoint.x + fRadius) / g_fRate + 20 + g_hSetOffset, 
								g_Rect.Height() - g_vSetOffset - (centerPoint.y + fRadius) / g_fRate)
								, CPoint(endPoint.x, endPoint.y), CPoint(startPoint.x, startPoint.y));
						}
					}
				}

				delete []fPoint;
				break;

			case 8:
				NodeArray.clear();
				NodeArray = vector<vec2>(((CGraph*)g_GraphArray.GetAt(i))->controlNode.begin(), 
					((CGraph*)g_GraphArray.GetAt(i))->controlNode.end());

				bCurve = CBspline(NodeArray);
				bCurve.DrawInterpolation();

				for(int j = 0;j < bCurve.m_interpolationNode.size() - 1;++j)
				{
					lastX = bCurve.m_interpolationNode[j][0], lastY = bCurve.m_interpolationNode[j][1];
					nextX = bCurve.m_interpolationNode[j + 1][0], nextY = bCurve.m_interpolationNode[j + 1][1];
					//pDC->MoveTo(lastX / g_fRate + 20 + g_hSetOffset,
					//	g_Rect.Height() - g_vSetOffset - lastY / g_fRate);
					//pDC->LineTo(nextX / g_fRate + 20 + g_hSetOffset,
					//	g_Rect.Height() - g_vSetOffset - nextY / g_fRate);

					memDC.MoveTo(lastX / g_fRate + 20 + g_hSetOffset, 
						g_Rect.Height() - g_vSetOffset - lastY / g_fRate);
					memDC.LineTo(nextX / g_fRate + 20 + g_hSetOffset, 
						g_Rect.Height() - g_vSetOffset - nextY / g_fRate);
				}

				NodeArray.clear();
				break;

			case 9:
				CBrush pointBrsush;
				pointBrsush.CreateSolidBrush(clrGraph);
				CPen pen(PS_SOLID, 1, clrGraph);
				oldPen = memDC.SelectObject(&pen);
				oldPBrush = memDC.SelectObject(&pointBrsush);

				CPoint2D sPoint;
				sPoint = ((CGraph*)g_GraphArray.GetAt(i))->m_ptOrigin;
				sPoint /= g_fRate;
				sPoint = funArray.MyQuadrantToScreen(sPoint) + CPoint2D(-g_hOffset,g_vOffset);
				DrawPoint(&memDC,sPoint);

				memDC.SelectObject(oldPBrush);
				pointBrsush.DeleteObject();
				break;
			}

			if(((CGraph*)g_GraphArray.GetAt(i))->m_bIsShowArrow)
			{
				CPen pen1(PS_SOLID, 2, RGB(255, 0, 0));
				oldPen1 = memDC.SelectObject(&pen1);

				p1 = ((CGraph*)g_GraphArray.GetAt(i))->pt2Array.p1;
				p2 = ((CGraph*)g_GraphArray.GetAt(i))->pt2Array.p2;
				p1 = funArray.MyQuadrantToScreen(p1 / g_fRate);
				p2 = funArray.MyQuadrantToScreen(p2 / g_fRate);

				//p1 = CPoint(p1.x / g_fRate,p1.y / g_fRate);
				//p2 = CPoint(p2.x / g_fRate,p2.y / g_fRate);
				bDirection = ((CGraph*)g_GraphArray.GetAt(i))->m_bDirection;

				if(type == 7)
				{
					if(lastAngle == 0.0)
						type = 2;
					else 
						type = 4;

					if(lastAngle > 0.0)
						bDirection = FALSE;
					else
						bDirection = TRUE;
				}

				if(type == 8 || type == 6)
					type = 2;

				pt3Array = funArray.GetArrowPointArray(type, p1, p2, bDirection);

				ptStart = pt3Array.p1 + CPoint2D(-g_hOffset, g_vOffset);
				ptEnd1 = pt3Array.p2 + CPoint2D(-g_hOffset, g_vOffset);
				ptEnd2 = pt3Array.p3 + CPoint2D(-g_hOffset, g_vOffset);
				memDC.MoveTo(ptStart.x, ptStart.y);
				memDC.LineTo(ptEnd1.x, ptEnd1.y);

				memDC.MoveTo(ptStart.x, ptStart.y);
				memDC.LineTo(ptEnd2.x, ptEnd2.y);

				memDC.SelectObject(oldPen1);
				memDC.SelectObject(oldPBrush);

			}

			if(m_CommandType != CT_ADJUSTLOCATION)
			{
				if(((CGraph*)g_GraphArray.GetAt(i))->m_bIsPitched)
					DrawFrame(&memDC, ((CGraph*)g_GraphArray.GetAt(i)));
			}

			if(pen2)
			{
				pen2->DeleteObject();
				pen2 = NULL;
			}
		}


		for(i = 0; i < g_TextArray.GetCount(); ++i)
		{
			CFont *pFont = new CFont;
			CFont *pOldFont = NULL;
			int TextStype = ((CTextGraph*)g_TextArray.GetAt(i))->m_TextStyle;
			CString Text = ((CTextGraph*)g_TextArray.GetAt(i))->m_Text;
			double TextHeight = ((CTextGraph*)g_TextArray.GetAt(i))->m_TextHeight;
			double TextWidth = ((CTextGraph*)g_TextArray.GetAt(i))->m_TextWidth;
			CPoint2D InsertPoint = ((CTextGraph*)g_TextArray.GetAt(i))->m_InsertPoint;
			double TextAngle = ((CTextGraph*)g_TextArray.GetAt(i))->m_Angle;
			CString Stype = ((CTextGraph*)g_TextArray.GetAt(i))->m_Style;
			int TextLength = Text.GetLength();
			CPoint2D transPoint = CPoint2D(InsertPoint.x / g_fRate, InsertPoint.y / g_fRate);
			funArray.MyQuadrantToScreen1(transPoint);

			pFont->CreateFont(TextHeight / g_fRate, TextHeight * 0.7 / g_fRate, 0, 0, FW_THIN, FALSE, FALSE, FALSE, 
				ANSI_CHARSET, OUT_CHARACTER_PRECIS, 
				DEFAULT_PITCH, DEFAULT_QUALITY, 
				FF_MODERN, L"Arial");
			pOldFont = memDC.SelectObject(pFont);

			//memDC.TextOut(transPoint.x, transPoint.y, Text);
			CRect TextFrameRect;
			TextFrameRect.SetRect(transPoint.x, transPoint.y, transPoint.x + TextWidth / g_fRate, transPoint.y + TextHeight / g_fRate);
			memDC.DrawText(Text, &TextFrameRect, DT_CENTER | DT_VCENTER);

			memDC.SelectObject(pOldFont);
			pFont->DeleteObject();
			pFont = NULL;
		}

		pDC->BitBlt(0, 0, rt.Width(), rt.Height(), &memDC, 0, 0, SRCCOPY);

		if(pen1)
		{
			pen1->DeleteObject();
			pen1 = NULL;
		}

		if(oldPen)
		{
			oldPen->DeleteObject();
			oldPen = NULL;
		}

		if(pOldOrgPen)
		{
			pOldOrgPen->DeleteObject();
			pOldOrgPen = NULL;
		}

	}
	else if(CT_IMITATE == m_CommandType)
	{
		//memDC.SelectObject(&orgPen);
		CPen RedPen(PS_SOLID, 1, RGB(255, 0, 0)),
			YellowPen(PS_SOLID, 1, RGB(85, 107, 47)),
			BluePen(PS_SOLID, 1, RGB(0, 0, 255)), *pOldImitatePen;
		//CBrush brush(RGB(0,0,255)),*pOldBrush;

		_2PointData pd;
		orgPoint = CPoint2D(0, 0);

		if(m_SpeedDlg.m_hWnd)
		{
			uPrg = (g_DrawVector.size() * 100) / (g_ImitatePerPointDataVector.size());
			::SendMessage(m_SpeedDlg.m_hWnd, WM_UPDATE_PROGRESS, uPrg, 0);
		}

		orgPoint /= g_fRate;
		orgPoint = funArray.MyQuadrantToScreen(orgPoint);
		memDC.MoveTo(orgPoint.x, orgPoint.y);

		for(int ab = 0; ab < g_DrawVector.size(); ++ab)
		{
			pd = g_DrawVector[ab];
			pd.ePoint /= g_fRate;
			pd.ePoint = funArray.MyQuadrantToScreen(pd.ePoint);

			if(pd.ToPointWay == 0)//G01
				pOldImitatePen = memDC.SelectObject(&YellowPen);
			else
				pOldImitatePen = memDC.SelectObject(&RedPen);

			memDC.LineTo(pd.ePoint.x, pd.ePoint.y);

			pOldImitatePen = memDC.SelectObject(&BluePen);
			memDC.SelectObject(pOldImitatePen);


		}
		pDC->BitBlt(0, 0, rt.Width(), rt.Height(), &memDC, 0, 0, SRCCOPY);
		//pOldImitatePen->DeleteObject();

	}
	memDC.SelectObject(hOldBitmap);
	DeleteObject(hBitmap);
	memDC.DeleteDC();
	m_Ruler.Show(this);
}

// Cd2gView 打印

BOOL Cd2gView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void Cd2gView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void Cd2gView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清除过程
}


// Cd2gView 诊断

#ifdef _DEBUG
void Cd2gView::AssertValid() const
{
	CView::AssertValid();
}

void Cd2gView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cd2gDoc* Cd2gView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cd2gDoc)));
	return (Cd2gDoc*)m_pDocument;
}
#endif //_DEBUG


// Cd2gView 消息处理程序
void Cd2gView::OnSelect()
{
	g_OperationArray.RemoveAll();
	g_iNumArray.RemoveAll();

	m_CommandType = CT_DRAW;
	m_bIsLink = FALSE;

	//06-30修改Cf
	//Start
	//for(int j = 0; j < g_GraphArray.GetSize(); ++j)
	//	((CGraph*)g_GraphArray.GetAt(j))->m_bIsPitched = FALSE;
	//End

	m_iDrawType = 1;
	Invalidate();
	SetStatusText(L"选择");
}

void Cd2gView::OnDrawLine()//画直线
{
	SetStatusText(L"画直线");
	m_CommandType = CT_DRAW;
	for(int j = 0; j < g_GraphArray.GetSize(); ++j)
		((CGraph*)g_GraphArray.GetAt(j))->m_bIsPitched = FALSE;

	m_bFlag = TRUE;
	m_iDrawType = 2;
}

void Cd2gView::OnDrawCirCle()//画圆
{
	SetStatusText(L"画整圆");
	m_CommandType = CT_DRAW;
	for(int j = 0; j < g_GraphArray.GetSize(); ++j)
		((CGraph*)g_GraphArray.GetAt(j))->m_bIsPitched = FALSE;

	m_bFlag = TRUE;
	m_iDrawType = 3;
}

void Cd2gView::OnDrawArc()//画圆弧
{
	SetStatusText(L"画圆弧");
	m_CommandType = CT_DRAW;
	for(int j = 0; j < g_GraphArray.GetSize(); ++j)
		((CGraph*)g_GraphArray.GetAt(j))->m_bIsPitched = FALSE;

	m_bFlag = TRUE;
	m_iDrawType = 4;
}

void Cd2gView::OnDrawRectangle()//画矩形
{
	SetStatusText(L"画矩形");
	m_CommandType = CT_DRAW;
	for(int j = 0; j < g_GraphArray.GetSize(); ++j)
		((CGraph*)g_GraphArray.GetAt(j))->m_bIsPitched = FALSE;

	m_bFlag = TRUE;
	m_iDrawType = 5;	
}

void Cd2gView::OnDrawEllipse()//画椭圆
{
	SetStatusText(L"画椭圆");
	m_CommandType = CT_DRAW;
	for(int j = 0; j < g_GraphArray.GetSize(); ++j)
		((CGraph*)g_GraphArray.GetAt(j))->m_bIsPitched = FALSE;

	m_bFlag = TRUE;
	m_iDrawType = 6;
}

void Cd2gView::OnDrawPoint()
{
	SetStatusText(L"画点");
	m_CommandType = CT_DRAW;
	for(int j = 0; j < g_GraphArray.GetSize(); ++j)
		((CGraph*)g_GraphArray.GetAt(j))->m_bIsPitched = FALSE;

	m_bFlag = TRUE;
	m_iDrawType = 9;
}

/***********************************
** 鼠标左键按下触发函数
** 参数
** 时间：2013-5-10
***********************************/
void Cd2gView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CClientDC dc(this);
	CClientDC dc1(this);
	COLORREF clrGraph;

	clrGraph = funArray.StringToRGB(color);
	CPen pen(PS_SOLID, 1, clrGraph);
	CPen pen1(PS_DOT, 1, RGB(0, 0, 0));

	dc.SelectObject(&pen);
	dc1.SelectObject(&pen1);

	CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	dc.SelectObject(pBrush);
	dc1.SelectObject(pBrush);

	double startX = 0, startY = 0;//圆弧起点坐标
	double endX = 0, endY = 0;//圆弧终点坐标		
	double midX = 0, midY = 0;//圆弧的第二点
	CPoint pt1, pt2; 
	double fOX = 0, fOY = 0;
	double k1 = 0, k2 = 0;
	double tempAngle = 0;
	CPoint2D startPoint, endPoint, midPoint, centerPoint;
	CPoint2D newStartPoint, newEndPoint, newMidPoint, newCenterPoint;
	double fRadius = 0;
	double dx = 0, dy = 0;
	double angle = 0, degree = 0;
	double firstDis = 0, secondDis = 0;
	CPoint2D curPoint;
	BOOL bSelected = FALSE;
	CPoint topLeftPt;
	CPoint2D lBPt, leftBottomPt;
	int height = 0, width = 0;
	double rate1 = 0.0, rate2 = 0.0, tmpRate = 0.0;
	double offX = 0.0, offY = 0.0;
	int tmp;
	double mx = 0.0, my = 0.0;
	CBspline bCurve;
	vector<vec2>NodeArray;
	NodeArray.clear();
	CPoint2D p1, p2;
	int pos = 0;
	CGraph *NextGraph = NULL;
	CGraph *CopyGraph = NULL;
	int iFetchGraphNum = 0;
	CString strMsg;


	//选择刀具
	if(g_strToolSelect == "")
	{
		MessageBox(L"您还未选择刀具。。。");
		g_GraphArray.RemoveAll();
		m_iDrawType = 0;
		return;
	}

	else if(CT_SCREENMOVE == m_CommandType)
	{
		m_bFlag = TRUE;
		m_screenMoveStartPoint = CPoint2D(point.x, point.y);
	}
	//单击选择
	else if(m_iDrawType == 0 
		&& m_CommandType != CT_DRAW 
		&& m_CommandType != CT_MOVE 
		&& m_CommandType != CT_MIRROR 
		&& m_CommandType != CT_COPY	//06-27修改Cf
		&& m_CommandType != CT_ADJUSTLOCATION)
	{
		//g_OperationArray.RemoveAll();
		curPoint = CPoint2D(point.x, point.y);
		curPoint = funArray.ScreenToMyQuadrant(curPoint) + CPoint2D(g_hOffset, g_vOffset);
		curPoint *= g_fRate;

		for(int i = 0; i < g_GraphArray.GetSize(); ++i)
		{
			NextGraph = (CGraph*)g_GraphArray.GetAt(i);
			if(!(NextGraph->m_bIsPitched))//选中
			{
				bSelected = funArray.PosInGraph(NextGraph, curPoint);
				if(bSelected != FALSE)
				{
					NextGraph->m_bIsPitched = TRUE;
					g_iNumArray.Add(i);
					g_OperationArray.Add(NextGraph);
					break;
				}
			}
			else if(NextGraph->m_bIsPitched)//取消选中
			{
				bSelected = funArray.PosInGraph(NextGraph, curPoint);
				if(bSelected != FALSE)
				{
					NextGraph->m_bIsPitched = FALSE;
					pos = funArray.GetIndexFromNumberArray(i);
					g_iNumArray.RemoveAt(pos);

					pos = funArray.GetIndexFromOperationArray(NextGraph);
					g_OperationArray.RemoveAt(pos);
					break;
				}
			}
		}
		Invalidate();
		//06-30修改Cf
		//Start
		if(g_OperationArray.GetSize() > 0)
			strMsg.Format(L"选中了 %d 个图形", g_OperationArray.GetSize());
		else
			strMsg = L"";
		SetStatusText(strMsg);
		//End
	}
	else
	{
		if(m_CommandType == CT_DRAW)
		{
			switch(m_iDrawType)
			{
			case 0:
				m_OperationPoint = CPoint2D(point.x, point.y);
				break;

			case 1:
				++m_iVirtualRectCount;
				if(m_iVirtualRectCount == 1)
				{
					m_bFlag = TRUE;
					//鼠标左键单击，设置可以开始绘制
					rectStartPoint = point;//记录开始点
					m_oldPt = point;//设置终点也为开始点
				}
				else
				{
					m_bFlag = FALSE;
					m_iVirtualRectCount = 0;
					m_iDrawType = 0;
					rectEndPoint = point;
					dc1.Rectangle(CRect(rectStartPoint, rectEndPoint));

					m_CommandType = CT_NONE;

					iFetchGraphNum = FetchGraphInRectToArray(CRect(rectStartPoint, rectEndPoint));
					strMsg.Format(L"选中了 %d 个图形", iFetchGraphNum);
					SetStatusText(strMsg);
				}
				break;

			case 2:
				++m_iLineCount;
				if(m_iLineCount == 1)
				{
					//鼠标左键单击，设置可以开始绘制
					if(!m_bIsLink)
					{
						m_startPt = point;//记录开始点
						m_oldPt = point;//设置终点也为开始点
					}
					else
					{
						m_startPt = m_lastPt;
						m_oldPt = m_lastPt;
					}
				}
				else if(m_iLineCount == 2)
				{
					m_iLineCount = 0;
					m_bFlag = FALSE;
					m_lastPt = point;

					//pt2Array = CPointArray(m_startPt,fPoint);
					startPoint = funArray.ScreenToMyQuadrant(m_startPt) 
						+ CPoint2D(g_hOffset, g_vOffset);
					endPoint = funArray.ScreenToMyQuadrant(point) 
						+ CPoint2D(g_hOffset, g_vOffset);

					startPoint *= g_fRate;
					endPoint *= g_fRate;

					pt2Array = CPointArray(startPoint, endPoint);

					//qGraph = new CGraph(m_iDrawType,startPoint,endPoint,clrGraph,pt2Array);
					qGraph = new CGraph;
					qGraph->m_iDrawType = m_iDrawType;
					qGraph->m_ptOrigin = startPoint;
					qGraph->m_ptEnd = endPoint;
					qGraph->clrREF = clrGraph;
					qGraph->pt2Array = pt2Array;

					g_GraphArray.Add(qGraph);

					Invalidate();
					m_iDrawType = 0;
					m_CommandType = CT_NONE;
				}
				break;

			case 3:
				++m_iCircleCount;
				if(m_iCircleCount == 1)
				{
					//鼠标左键单击，设置可以开始绘制
					if(!m_bIsLink)
					{
						m_startPt = point;//记录开始点
						m_oldPt = point;//设置终点也为开始点
					}
					else
					{
						m_startPt = m_lastPt;
						m_oldPt = m_lastPt;
					}
				}
				else if(m_iCircleCount == 2)
				{
					m_bFlag = FALSE;
					m_iCircleCount = 0;
					m_fCircleRadius = funArray.GetDistance(CPoint2D(m_startPt.x,m_startPt.y)
						,CPoint2D(point.x, point.y));
					m_lastPt = m_startPt;

					startPoint = CPoint2D(point.x - m_fCircleRadius, point.y);

					centerPoint = funArray.ScreenToMyQuadrant(point.x, point.y) 
						+ CPoint2D(g_hOffset, g_vOffset);
					startPoint = funArray.ScreenToMyQuadrant(startPoint) 
						+ CPoint2D(g_hOffset, g_vOffset);

					centerPoint *= g_fRate;
					m_fCircleRadius *= g_fRate;
					startPoint *= g_fRate;

					pt2Array = CPointArray(centerPoint, startPoint);

					/*qGraph = new CGraph(m_iDrawType,startPoint,centerPoint,
					clrGraph,m_fCircleRadius,FALSE,pt2Array);*/
					qGraph = new CGraph;
					qGraph->m_iDrawType = m_iDrawType;
					qGraph->m_ptOrigin = startPoint;
					qGraph->m_ptCenter = centerPoint;
					qGraph->clrREF = clrGraph;
					qGraph->m_fRadius = m_fCircleRadius;
					qGraph->pt2Array = pt2Array;
					qGraph->m_bFlags = FALSE;

					g_GraphArray.Add(qGraph);
					Invalidate();
					m_iDrawType = 0;
					m_CommandType = CT_NONE;

				}
				break;

			case 4:
				++m_iArcCount;
				if(m_iArcCount == 1)
				{
					//鼠标左键单击，设置可以开始绘制
					if(!m_bIsLink)
					{
						m_startPt = point;//记录开始点
						m_oldPt = point;//设置终点也为开始点
					}
					else
					{
						m_startPt = m_lastPt;
						m_oldPt = m_lastPt;
					}
				}
				else if(m_iArcCount == 2)
				{
					m_bFlag = TRUE;
					m_savePoint = point;
				}
				else if(m_iArcCount == 3)
				{
					m_bFlag = FALSE;
					m_iArcCount = 0;
					startX = m_startPt.x;
					startY = m_startPt.y;
					midX = m_savePoint.x;
					midY = m_savePoint.y;
					endX = point.x;
					endY = point.y;


					if(startY == midY && 
						startY == endY || startX == midX && 
						startX == endX)
						;

					if(startY == midY && startY != endY)
					{
						pt2.x = (startX + endX) / 2.0;
						pt2.y = (startY + endY) / 2.0;

						fOX = (startX + midX) / 2.0;
						k2 = (double)(startX - endX) / (double)(endY - startY);
						fOY = k2 * (fOX - pt2.x) + pt2.y;
						m_fCircleRadius = funArray.GetDistance(CPoint2D(fOX, fOY), 
							CPoint2D(startX, startY));//圆弧半径

						//圆心相对于起点的增量

						tempAngle = funArray.GetAngle(CPoint2D(startX, startY), 
							CPoint2D(endX, endY), m_fCircleRadius);

						funArray.GetRigthAngle(CPoint2D(startX, startY), CPoint2D(endX, endY), CPoint2D(fOX, fOY), !m_bIsOrder, &tempAngle);

						/*if((startY - fOY) * (endX - fOX) 
						- (endY - fOY) * (startX - fOX) < 0)
						{
						if(!m_bIsOrder)
						{
						tempAngle = 2 * PI - tempAngle;
						}
						}
						else
						{
						if(m_bIsOrder)
						{
						tempAngle = 2 * PI - tempAngle;
						}
						}*/
					}

					else if(startY == endY && startY != midY)
					{
						pt1.x = (startX + midX) / 2.0;
						pt1.y = (startY + midY) / 2.0;

						fOX = (startX + midX) / 2.0;
						k1 = (double)(startX - midX) / (double)(midY - startY);
						fOY = k1 * (fOX - pt1.x) + pt1.y;
						m_fCircleRadius = funArray.GetDistance(CPoint2D(fOX, fOY), 
							CPoint2D(startX, startY));//圆弧半径

						tempAngle = funArray.GetAngle(CPoint2D(startX, startY),
							CPoint2D(endX, endY), m_fCircleRadius);

						funArray.GetRigthAngle(CPoint2D(startX, startY), CPoint2D(endX, endY), CPoint2D(fOX, fOY), !m_bIsOrder, &tempAngle);

						/*if((startY - fOY) * (endX - fOX) 
						- (endY - fOY) * (startX - fOX) < 0)
						{
						if(!m_bIsOrder)
						{		
						tempAngle = 2 * PI - tempAngle;
						}
						}
						else
						{
						if(m_bIsOrder)
						{
						tempAngle = 2 * PI - tempAngle;
						}
						}*/
					}
					else
					{
						k1 = (double)(startX - midX) / (double)(midY - startY);
						k2 = (double)(startX - endX) / (double)(endY - startY);
						if(k1 == k2)
							;
						else
						{
							pt1.x = (startX + midX) / 2.0;
							pt1.y = (startY + midY) / 2.0;
							pt2.x = (startX + endX) / 2.0;
							pt2.y = (startY + endY) / 2.0;

							fOX = (pt2.y - pt1.y + k1 * pt1.x - k2 * pt2.x) / (k1 - k2);
							fOY = (k1 * k2 * (pt1.x - pt2.x) 
								+ pt2.y * k1 - pt1.y * k2) / (k1 - k2);
							m_fCircleRadius = funArray.GetDistance(CPoint2D(fOX, fOY)
								,CPoint2D(startX, startY));//圆弧半径

							tempAngle = funArray.GetAngle(CPoint2D(startX, startY)
								, CPoint2D(endX, endY), m_fCircleRadius);

							funArray.GetRigthAngle(CPoint2D(startX, startY), CPoint2D(endX, endY), CPoint2D(fOX, fOY), !m_bIsOrder, &tempAngle);

							/*if((startY - fOY) * (endX - fOX) 
							- (endY - fOY) * (startX - fOX) < 0)
							{
							if(!m_bIsOrder)	
							{
							tempAngle = 2 * PI - tempAngle;
							}
							}
							else
							{
							if(m_bIsOrder)
							{
							tempAngle = 2 * PI - tempAngle;
							}
							}*/
						}
					}

					m_lastPt = point;

					centerPoint = CPoint2D(fOX,fOY);

					startPoint = funArray.ScreenToMyQuadrant(m_startPt) 
						+ CPoint2D(g_hOffset, g_vOffset);
					midPoint = funArray.ScreenToMyQuadrant(m_savePoint) 
						+ CPoint2D(g_hOffset, g_vOffset);
					endPoint = funArray.ScreenToMyQuadrant(point) 
						+ CPoint2D(g_hOffset, g_vOffset);
					centerPoint = funArray.ScreenToMyQuadrant(centerPoint) 
						+ CPoint2D(g_hOffset, g_vOffset);

					startPoint *= g_fRate;
					midPoint *= g_fRate;
					endPoint *= g_fRate;
					centerPoint *= g_fRate;
					m_fCircleRadius *= g_fRate;

					pt2Array = CPointArray(centerPoint, endPoint);

					/*qGraph = new CGraph(m_iDrawType,startPoint,midPoint,endPoint,
					centerPoint,m_bIsOrder,clrGraph,m_fCircleRadius,tempAngle,pt2Array,FALSE);*/
					qGraph = new CGraph;
					qGraph->m_iDrawType = m_iDrawType;
					qGraph->m_ptOrigin = startPoint;
					qGraph->m_ptMiddle = midPoint;
					qGraph->m_ptEnd = endPoint;
					qGraph->m_ptCenter = centerPoint;
					qGraph->m_bDirection = m_bIsOrder;
					qGraph->clrREF = clrGraph;
					qGraph->m_fRadius = m_fCircleRadius;
					qGraph->m_fAngle = tempAngle;
					qGraph->pt2Array = pt2Array;

					g_GraphArray.Add(qGraph);

					Invalidate();
					m_iDrawType = 0;
					m_CommandType = CT_NONE;

				}
				break;

			case 5:
				++m_iRectCount;
				if(m_iRectCount == 1)
				{
					m_bFlag = TRUE;
					//鼠标左键单击，设置可以开始绘制
					rectStartPoint = point;//记录开始点
					m_oldPt = point;//设置终点也为开始点
				}
				else
				{
					m_bFlag = FALSE;
					m_iRectCount = 0;
					rectEndPoint = point;
					dc.Rectangle(CRect(rectStartPoint, rectEndPoint));

					startPoint = funArray.ScreenToMyQuadrant(rectStartPoint) 
						+ CPoint2D(g_hOffset, g_vOffset);
					endPoint = funArray.ScreenToMyQuadrant(rectEndPoint) 
						+ CPoint2D(g_hOffset, g_vOffset);
					if(rectStartPoint.x < rectEndPoint.x) 
					{
						tmp = rectStartPoint.x;
						rectStartPoint.x = rectEndPoint.x;
						rectEndPoint.x = tmp;
					}

					if(rectStartPoint.y < rectEndPoint.y) 
					{
						tmp = rectStartPoint.y;
						rectStartPoint.y = rectEndPoint.y;
						rectEndPoint.y = tmp;
					}

					startPoint *= g_fRate;
					endPoint *= g_fRate;


					qGraph = new CGraph;
					qGraph->m_iDrawType = m_iDrawType;
					qGraph->m_ptOrigin = startPoint;
					qGraph->m_ptEnd = endPoint;
					qGraph->m_rectWidth = endPoint.x - startPoint.x;
					qGraph->m_rectHeight = startPoint.y - endPoint.y;
					qGraph->clrREF = clrGraph;
					qGraph->pt2Array = CPointArray(CPoint2D(endPoint.x, startPoint.y), startPoint);
					g_GraphArray.Add(qGraph);

					Invalidate();
					m_iDrawType = 0;
					m_CommandType = CT_NONE;
				}

				break;

			case 6:
				++m_iEllipseCount;
				if(m_iEllipseCount == 1)//椭圆心
				{
					m_centerPoint = point;
					m_oldPt = point;
				}
				else if(m_iEllipseCount == 2)
				{
					m_bFlag = TRUE;
					m_firstPoint = point;
				}
				else if(m_iEllipseCount == 3)
				{
					m_bFlag = FALSE;
					m_iEllipseCount = 0;

					m_secondPoint = point; 

					centerPoint = CPoint2D(m_centerPoint.x, m_centerPoint.y);
					startPoint = CPoint2D(m_firstPoint.x, m_firstPoint.y);
					endPoint = CPoint2D(m_secondPoint.x, m_secondPoint.y);

					firstDis = funArray.GetDistance(centerPoint, startPoint);
					secondDis = funArray.GetDistance(centerPoint, endPoint);

					centerPoint = funArray.ScreenToMyQuadrant(centerPoint) 
						+ CPoint2D(g_hOffset, g_vOffset);
					startPoint = funArray.ScreenToMyQuadrant(startPoint) 
						+ CPoint2D(g_hOffset, g_vOffset);
					endPoint = funArray.ScreenToMyQuadrant(endPoint) 
						+ CPoint2D(g_hOffset, g_vOffset);

					if(firstDis >= secondDis)
					{
						m_fMajorLength = firstDis;
						m_fMinorLength = secondDis;
					}
					else
					{
						m_fMajorLength = secondDis;
						m_fMinorLength = firstDis;
					}
					centerPoint *= g_fRate;
					startPoint *= g_fRate;
					endPoint *= g_fRate;
					m_fMajorLength *= g_fRate;
					m_fMinorLength *= g_fRate;

					mx = startPoint.x - centerPoint.x;
					my = startPoint.y - centerPoint.y;

					if(mx == 0.0 && my != 0.0)
					{
						if(my < 0)
							degree = 3 * PI / 2;
						else
							degree = PI / 2;
					}
					else if(my == 0.0 && mx != 0.0)
					{
						if(mx > 0)
							degree = 0;
						else
							degree = PI;
					}
					else
					{
						degree = atan(my / mx);
						if(mx < 0 && my < 0)
							degree = PI + degree;
					}

					if(firstDis < secondDis)
						degree += PI / 2;

					qGraph = new CGraph;
					qGraph->m_iDrawType = m_iDrawType;
					qGraph->m_fStartAngle = 0;
					qGraph->m_fEndAngle = 2 * PI;
					qGraph->m_ptCenter = centerPoint;
					qGraph->m_fDegree = degree;
					qGraph->m_fMajor = m_fMajorLength;
					qGraph->m_fMinor = m_fMinorLength;

					qGraph->clrREF = clrGraph;
					//qGraph->pt2Array = CPointArray(CPoint2D(endPoint.x,startPoint.y),startPoint);

					g_GraphArray.Add(qGraph);

					Invalidate();
					m_iDrawType = 0;
					m_CommandType = CT_NONE;
				}
				else if(m_iEllipseCount == 4)
				{

				}
				else if(m_iEllipseCount == 5)
				{
					m_bFlag = FALSE;
					m_iEllipseCount = 0;
				}
				break;

			case 9://点
				CPoint2D sPoint;
				//fPoint = CPoint(fPoint.x * g_fRate,fPoint.y * g_fRate);
				m_lastPt = point;

				sPoint = funArray.ScreenToMyQuadrant(point) + CPoint2D(g_hOffset, g_vOffset);
				sPoint *= g_fRate;

				//qGraph = new CGraph(m_iDrawType,sPoint,clrGraph);
				qGraph = new CGraph;
				qGraph->m_iDrawType = m_iDrawType;
				qGraph->m_ptOrigin = sPoint;
				qGraph->m_ptEnd = sPoint;
				qGraph->clrREF = clrGraph;
				g_GraphArray.Add(qGraph);
				Invalidate();
				m_iDrawType = 0;
				m_CommandType = CT_NONE;
				m_bFlag = FALSE;
				break;
			}
		}

		else if (m_CommandType == CT_MOVE)//平移
		{
			if(g_OperationArray.IsEmpty())
			{
				return ; 
			}

			++m_iMoveCount;
			if(m_iMoveCount == 1)
			{
				m_bFlag = TRUE;
				m_downPoint = CPoint2D(point.x, point.y);
				m_oldMovePoint = m_downPoint;
				for(int i = 0; i < g_OperationArray.GetSize(); ++i)
					((CGraph*)g_OperationArray.GetAt(i))->m_bIsPitched = FALSE;

				Invalidate();
			}
			else
			{
				m_bFlag = FALSE;

				dx = (point.x - m_downPoint.x) * g_fRate;
				dy = (point.y - m_downPoint.y) * g_fRate;

				for(int i = 0; i < g_OperationArray.GetSize(); ++i)
				{
					switch(((CGraph*)g_OperationArray.GetAt(i))->m_iDrawType)
					{
					case 2:
						newStartPoint = CPoint2D(dx, -dy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptOrigin;
						newEndPoint = CPoint2D(dx, -dy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptEnd;

						((CGraph*)g_OperationArray.GetAt(i))->m_ptOrigin = newStartPoint;
						((CGraph*)g_OperationArray.GetAt(i))->m_ptEnd = newEndPoint;
						((CGraph*)g_OperationArray.GetAt(i))->pt2Array = CPointArray(newStartPoint,newEndPoint);
						break;

					case 3:
						newCenterPoint = CPoint2D(dx, -dy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptCenter;

						((CGraph*)g_OperationArray.GetAt(i))->m_ptCenter = newCenterPoint;
						newStartPoint.x = newCenterPoint.x - ((CGraph*)g_OperationArray.GetAt(i))->m_fRadius;
						newStartPoint.y = newCenterPoint.y;
						((CGraph*)g_OperationArray.GetAt(i))->m_ptOrigin = newStartPoint;
						((CGraph*)g_OperationArray.GetAt(i))->pt2Array = CPointArray(newCenterPoint,newStartPoint);

						break;

					case 4:
						newStartPoint = CPoint2D(dx, -dy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptOrigin;
						newEndPoint = CPoint2D(dx, -dy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptEnd;
						newMidPoint = CPoint2D(dx, -dy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptMiddle;
						newCenterPoint = CPoint2D(dx, -dy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptCenter;

						((CGraph*)g_OperationArray.GetAt(i))->m_ptOrigin = newStartPoint;
						((CGraph*)g_OperationArray.GetAt(i))->m_ptEnd = newEndPoint;
						((CGraph*)g_OperationArray.GetAt(i))->m_ptCenter = newCenterPoint;
						((CGraph*)g_OperationArray.GetAt(i))->m_ptMiddle = newMidPoint;

						((CGraph*)g_OperationArray.GetAt(i))->pt2Array = CPointArray(newCenterPoint,newEndPoint);
						break;

					case 5:
						newStartPoint = CPoint2D(dx, -dy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptOrigin;
						newEndPoint = CPoint2D(dx, -dy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptEnd;

						((CGraph*)g_OperationArray.GetAt(i))->m_ptOrigin = newStartPoint;
						((CGraph*)g_OperationArray.GetAt(i))->m_ptEnd = newEndPoint;
						pt2Array = ((CGraph*)g_OperationArray.GetAt(i))->pt2Array;
						pt2Array.p1.x += dx;
						pt2Array.p1.y -= dy;
						pt2Array.p2.x += dx;
						pt2Array.p2.y -= dy;
						((CGraph*)g_OperationArray.GetAt(i))->pt2Array = pt2Array;
						break;

					case 6:
						newStartPoint = CPoint2D(dx, -dy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptOrigin;
						newEndPoint = CPoint2D(dx, -dy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptEnd;
						newCenterPoint = CPoint2D(dx, -dy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptCenter;

						//pt2Array = CPointArray(CPoint2D(ptSecond.x,ptSecond.y),CPoint2D(ptEnd.x,ptEnd.y));
						((CGraph*)g_OperationArray.GetAt(i))->m_ptOrigin = newStartPoint;
						((CGraph*)g_OperationArray.GetAt(i))->m_ptEnd = newEndPoint;
						((CGraph*)g_OperationArray.GetAt(i))->m_ptCenter = newCenterPoint;
						break;

					case 7:

						break;

					case 8:
						NodeArray = vector<vec2>(((CGraph*)g_OperationArray.GetAt(i))->controlNode.begin(), 
							((CGraph*)g_OperationArray.GetAt(i))->controlNode.end());

						for(int j = 0; j < NodeArray.size(); ++j)
						{
							NodeArray[j][0] += dx;
							NodeArray[j][1] -= dy;
						}
						((CGraph*)g_OperationArray.GetAt(i))->controlNode = vector<vec2>(NodeArray.begin(),NodeArray.end());
						NodeArray.clear();

						break;

					case 9:
						newStartPoint = CPoint2D(dx, -dy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptOrigin;
						((CGraph*)g_OperationArray.GetAt(i))->m_ptOrigin = newStartPoint;
						break;
					}

				}

				//07-01修改Cf
				//Start
				g_OperationArray.RemoveAll();
				//End

				m_iMoveCount = 0;
				m_CommandType = CT_NONE;
				Invalidate();
			}
		}

		//06-27修改Cf
		//Start
		else if (m_CommandType == CT_COPY)
		{
			if(g_OperationArray.IsEmpty())
			{
				return ; 
			}

			++m_iMoveCount;
			if(m_iMoveCount == 1)
			{
				m_bFlag = TRUE;
				m_downPoint = CPoint2D(point.x, point.y);
				m_oldMovePoint = m_downPoint;
				for(int i = 0; i < g_OperationArray.GetSize(); ++i)
					((CGraph*)g_OperationArray.GetAt(i))->m_bIsPitched = FALSE;

				Invalidate();
			}
			else
			{
				m_bFlag = FALSE;

				dx = (point.x - m_downPoint.x) * g_fRate;
				dy = (point.y - m_downPoint.y) * g_fRate;

				for(int i = 0; i < g_OperationArray.GetSize(); ++i)
				{	
					switch(((CGraph*)g_OperationArray.GetAt(i))->m_iDrawType)
					{
					case 2:
						newStartPoint = CPoint2D(dx, -dy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptOrigin;
						newEndPoint = CPoint2D(dx, -dy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptEnd;

						CopyGraph = new CGraph;
						CopyGraph->AssignGraph((CGraph*)g_OperationArray.GetAt(i));
						CopyGraph->m_ptOrigin = newStartPoint;
						CopyGraph->m_ptEnd = newEndPoint;
						CopyGraph->pt2Array = CPointArray(newStartPoint,newEndPoint);
						g_GraphArray.Add(CopyGraph);
						break;

					case 3:
						newCenterPoint = CPoint2D(dx, -dy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptCenter;

						CopyGraph = new CGraph;
						CopyGraph->AssignGraph((CGraph*)g_OperationArray.GetAt(i));
						CopyGraph->m_ptCenter = newCenterPoint;

						newStartPoint.x = newCenterPoint.x - ((CGraph*)g_OperationArray.GetAt(i))->m_fRadius;
						newStartPoint.y = newCenterPoint.y;
						CopyGraph->m_ptOrigin = newStartPoint;
						CopyGraph->pt2Array = CPointArray(newCenterPoint, newStartPoint);
						g_GraphArray.Add(CopyGraph);
						break;

					case 4:
						newStartPoint = CPoint2D(dx, -dy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptOrigin;
						newEndPoint = CPoint2D(dx, -dy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptEnd;
						newMidPoint = CPoint2D(dx, -dy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptMiddle;
						newCenterPoint = CPoint2D(dx, -dy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptCenter;

						CopyGraph = new CGraph;
						CopyGraph->AssignGraph((CGraph*)g_OperationArray.GetAt(i));

						CopyGraph->m_ptOrigin = newStartPoint;
						CopyGraph->m_ptEnd = newEndPoint;
						CopyGraph->m_ptCenter = newCenterPoint;
						CopyGraph->m_ptMiddle = newMidPoint;

						CopyGraph->pt2Array = CPointArray(newCenterPoint,newEndPoint);
						g_GraphArray.Add(CopyGraph);
						break;

					case 5:
						newStartPoint = CPoint2D(dx, -dy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptOrigin;
						newEndPoint = CPoint2D(dx, -dy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptEnd;

						CopyGraph = new CGraph;
						CopyGraph->AssignGraph((CGraph*)g_OperationArray.GetAt(i));

						CopyGraph->m_ptOrigin = newStartPoint;
						CopyGraph->m_ptEnd = newEndPoint;
						pt2Array = ((CGraph*)g_OperationArray.GetAt(i))->pt2Array;
						pt2Array.p1.x += dx;
						pt2Array.p1.y -= dy;
						pt2Array.p2.x += dx;
						pt2Array.p2.y -= dy;
						CopyGraph->pt2Array = pt2Array;
						g_GraphArray.Add(CopyGraph);
						break;

					case 6:
						newStartPoint = CPoint2D(dx, -dy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptOrigin;
						newEndPoint = CPoint2D(dx, -dy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptEnd;
						newCenterPoint = CPoint2D(dx, -dy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptCenter;

						CopyGraph = new CGraph;
						CopyGraph->AssignGraph((CGraph*)g_OperationArray.GetAt(i));

						CopyGraph->m_ptOrigin = newStartPoint;
						CopyGraph->m_ptEnd = newEndPoint;
						CopyGraph->m_ptCenter = newCenterPoint;
						g_GraphArray.Add(CopyGraph);
						break;

					case 7:

						break;

					case 8:
						NodeArray = vector<vec2>(((CGraph*)g_OperationArray.GetAt(i))->controlNode.begin(), 
							((CGraph*)g_OperationArray.GetAt(i))->controlNode.end());

						for(int j = 0; j < NodeArray.size(); ++j)
						{
							NodeArray[j][0] += dx;
							NodeArray[j][1] -= dy;
						}

						CopyGraph = new CGraph;
						CopyGraph->AssignGraph((CGraph*)g_OperationArray.GetAt(i));

						CopyGraph->controlNode = vector<vec2>(NodeArray.begin(),NodeArray.end());
						NodeArray.clear();
						g_GraphArray.Add(CopyGraph);
						break;

					case 9:
						newStartPoint = CPoint2D(dx, -dy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptOrigin;
						CopyGraph = new CGraph;
						CopyGraph->AssignGraph((CGraph*)g_OperationArray.GetAt(i));

						CopyGraph->m_ptOrigin = newStartPoint;
						g_GraphArray.Add(CopyGraph);
						break;
					}

				}
				m_iMoveCount = 0;
				m_CommandType = CT_NONE;
				Invalidate();
			}
		}
		//End

		else if(m_CommandType == CT_ROTATE)//旋转
		{

		}
		else if(m_CommandType == CT_MIRROR)//镜像
		{

		}
		else if(m_CommandType == CT_ADJUSTLOCATION)//调整位置
		{
			curPoint = CPoint2D(point.x, point.y);
			curPoint = funArray.ScreenToMyQuadrant(curPoint) + CPoint2D(g_hOffset, g_vOffset);
			curPoint *= g_fRate;

			for(int i = 0; i < g_GraphArray.GetSize(); ++i)
			{
				CGraph *newGraph = (CGraph*)g_GraphArray.GetAt(i);
				if(!(newGraph->m_bIsPitched))
				{
					bSelected = funArray.PosInGraph(newGraph, curPoint);
					if(bSelected)
					{
						//newGraph->color = RGB(255,0,0);
						newGraph->m_bIsPitched = TRUE;
						g_OperationArray.Add(newGraph);
					}
				}
				else if(newGraph->m_bIsPitched)
				{
					bSelected = funArray.PosInGraph(newGraph, curPoint);
					if(bSelected)
					{
						newGraph->m_bIsPitched = FALSE;

						pos = funArray.GetIndexFromOperationArray(newGraph);
						g_OperationArray.RemoveAt(pos);
					}
				}
			}
			Invalidate();
		}
	}	

	//设置光标为十字光标  
	::SetCursor(m_HCross);
	CView::OnLButtonDown(nFlags, point);
}


/***********************************
** 鼠标左键移动触发函数
** 参数
** 时间：2013-5-10
***********************************/
void Cd2gView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	double fDistance = 0;
	double k1 = 0, k2 = 0;
	double fOX = 0, fOY = 0, fOR = 0;
	CPoint2D pt1, pt2;
	CClientDC dc(this), dc1(this);//获取设备句柄
	double oldDx = 0, oldDy = 0;
	double newDx = 0, newDy = 0;
	CPoint2D oldStartPoint, oldEndPoint, oldMidPoint, oldCenterPoint;
	CPoint2D newStartPoint, newEndPoint, newMidPoint, newCenterPoint;
	double tempMajor = m_fMajorLength, tempMinor = 0, tempLength = 0;
	double tempAngle = m_fAngle;
	CPoint2D movePoint;
	BOOL isLighting = FALSE;
	BOOL bDirection = FALSE;
	double fRadius = 0.0, fMajor = 0.0, fMinor = 0.0, degree = 0.0, startAngle = 0.0, endAngle = 0.0;
	CBspline bCurve;
	vector<vec2>NodeArray;
	NodeArray.clear();
	CPoint2D curMovePoint;
	double angle = 0.0;


	//关键
	dc1.SetROP2(R2_NOT);
	dc.SetROP2(R2_NOT);//逆转当前屏幕颜色来画线的绘图方式
	//所绘制的图形并没有消失，所以可以验证下面的连续两次颜色取反不是在一次相应中完成   
	//即：该函数用来定义绘制的颜色，而该参数则将颜色设置为原屏幕颜色的反色   
	//这样，如果连续绘制两次的话，就可以恢复原来屏幕的颜色了（如下）   
	//但是，这里的连续两次绘制却不是在一次消息响应中完成的   
	//而是在第一次拖动响应的绘制可以显示（也就是看到的），第二次拖动绘制实现擦出（也就看不到了）   


	CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	dc.SelectObject(pBrush);

	if(m_bFlag)
	{			
		if(m_CommandType == CT_DRAW)
		{
			switch(m_iDrawType)
			{	
			case 1:
				::SetCursor(m_HCross);
				dc.Rectangle(CRect(rectStartPoint, m_oldPt));
				dc.Rectangle(CRect(rectStartPoint, point));
				m_oldPt = point;

				break;
			case 2:
				if(m_iLineCount == 1)
				{
					::SetCursor(m_HCross);   

					dc.MoveTo(m_startPt);
					dc.LineTo(m_oldPt);

					dc.MoveTo(m_startPt);
					dc.LineTo(point);

					m_oldPt = point;
				}
				break;

			case 3:
				if(m_iCircleCount == 1)
				{
					::SetCursor(m_HCross);  
					fDistance = funArray.GetDistance(CPoint2D(m_startPt.x, m_startPt.y)
						, CPoint2D(m_oldPt.x, m_oldPt.y));
					dc.Ellipse(CRect(m_oldPt.x - fDistance, m_oldPt.y - fDistance
						, m_oldPt.x + fDistance, m_oldPt.y + fDistance));

					fDistance = funArray.GetDistance(CPoint2D(m_startPt.x, m_startPt.y)
						, CPoint2D(point.x, point.y));
					dc.Ellipse(CRect(point.x - fDistance, point.y - fDistance
						, point.x + fDistance, point.y + fDistance));

					m_oldPt = point; 
				}
				break;

			case 4:
				::SetCursor(m_HCross);

				if(1 == m_iArcCount)
				{
					dc.MoveTo(m_startPt);
					dc.LineTo(m_oldPt);

					dc.MoveTo(m_startPt);
					dc.LineTo(point);
				}

				else if(2 == m_iArcCount)
				{
					CClientDC tmpDc(this);
					CPen pen1(PS_SOLID, 1, RGB(255, 255, 255));
					tmpDc.SelectObject(&pen1);
					tmpDc.MoveTo(m_startPt);
					tmpDc.LineTo(m_savePoint);

					pt1 = CPoint2D((m_startPt.x + m_savePoint.x) / 2.0
						, (m_startPt.y + m_savePoint.y) / 2.0);

					if(m_startPt.y == m_savePoint.y)//此时画的为水平线
					{
						fOX = pt1.x;

						pt2 = CPoint2D((m_startPt.x + m_oldPt.x) / 2.0
							, (m_startPt.y + m_oldPt.y) / 2.0);	

						if(m_oldPt.y != m_startPt.y || m_startPt.y != point.y)
						{
							k2 = (double)(m_startPt.x - m_oldPt.x) / (double)(m_oldPt.y - m_startPt.y);
							fOY = k2 * (fOX - pt2.x) + pt2.y;
							fOR = funArray.GetDistance(CPoint2D(fOX, fOY)
								, CPoint2D(m_oldPt.x, m_oldPt.y));
							if(m_startPt.x < m_savePoint.x)
							{
								if(m_oldPt.y < m_savePoint.y)
									dc.Arc(CRect(fOX - fOR, fOY - fOR, fOX + fOR, fOY + fOR), m_startPt, m_oldPt);
								else
									dc.Arc(CRect(fOX - fOR, fOY - fOR, fOX + fOR, fOY + fOR), m_oldPt, m_startPt);
							}
							else
							{
								if(m_oldPt.y < m_savePoint.y)
									dc.Arc(CRect(fOX - fOR, fOY - fOR, fOX + fOR, fOY + fOR), m_oldPt, m_startPt);				
								else
									dc.Arc(CRect(fOX - fOR, fOY - fOR, fOX + fOR, fOY + fOR), m_startPt, m_oldPt);
							}

						}

						pt2 = CPoint2D((m_startPt.x + point.x) / 2.0,(m_startPt.y + point.y) / 2.0);
						k2 = (double)(m_startPt.x - point.x) / (double)(point.y - m_startPt.y);
						fOY = k2 * (fOX - pt2.x) + pt2.y;
						fOR = funArray.GetDistance(CPoint2D(fOX, fOY), CPoint2D(point.x, point.y));
						if(m_startPt.x < m_savePoint.x)
						{
							if(point.y < m_savePoint.y)
							{
								dc.Arc(CRect(fOX - fOR, fOY - fOR, fOX + fOR, fOY + fOR)
									, m_startPt, point);
								m_bIsOrder = FALSE;
							}
							else
							{
								dc.Arc(CRect(fOX - fOR, fOY - fOR, fOX + fOR, fOY + fOR)
									, point, m_startPt);
								m_bIsOrder = TRUE;	
							}
						}
						else
						{
							if(point.y < m_savePoint.y)
							{
								dc.Arc(CRect(fOX - fOR, fOY - fOR, fOX + fOR, fOY + fOR)
									, point, m_startPt);
								m_bIsOrder = TRUE;
							}
							else
							{
								dc.Arc(CRect(fOX - fOR, fOY - fOR, fOX + fOR, fOY + fOR)
									, m_startPt, point);
								m_bIsOrder = FALSE;
							}
						}
					}


					else if(point.y == m_startPt.y && m_startPt.y != m_savePoint.y)
					{
						return ;
					}


					//此时画的是斜线,包括垂直线和一般的斜线
					else if(m_savePoint.y != m_startPt.y && m_startPt.y != point.y && m_startPt.y != m_oldPt.y)
					{
						pt2 = CPoint2D((m_startPt.x + m_oldPt.x) / 2.0,(m_startPt.y + m_oldPt.y) / 2.0);
						k1 = (double)(m_startPt.x - m_savePoint.x) / (double)(m_savePoint.y - m_startPt.y);
						k2 = (double)(m_startPt.x - m_oldPt.x) / (double)(m_oldPt.y - m_startPt.y);
						fOX = (pt2.y - pt1.y + k1 * pt1.x - k2 * pt2.x) / (k1 - k2);
						fOY = (k1 * k2 * (pt1.x - pt2.x) + pt2.y * k1 - pt1.y * k2) / (k1 - k2);
						fOR = funArray.GetDistance(CPoint2D(fOX, fOY), CPoint2D(m_oldPt.x, m_oldPt.y));


						//如果画的是垂直线
						if(m_startPt.x == m_savePoint.x)
						{
							if(m_startPt.y < m_savePoint.y)
							{
								if(m_oldPt.x > m_startPt.x)
									dc.Arc(CRect(fOX - fOR, fOY - fOR, fOX + fOR, fOY + fOR), m_startPt, m_oldPt);
								else
									dc.Arc(CRect(fOX - fOR, fOY - fOR, fOX + fOR, fOY + fOR), m_oldPt, m_startPt);
							}
							else
							{
								if(m_oldPt.x > m_startPt.x)
									dc.Arc(CRect(fOX - fOR, fOY - fOR, fOX + fOR, fOY + fOR), m_oldPt, m_startPt);
								else
									dc.Arc(CRect(fOX - fOR, fOY - fOR, fOX + fOR, fOY + fOR), m_startPt, m_oldPt);

							}
						}
						//一般的斜线
						else
						{
							if(k1 < 0)
							{
								if(m_startPt.y < m_savePoint.y)
								{
									if(pt1.y - (m_oldPt.x - pt1.x) / k1 > m_oldPt.y)
										dc.Arc(CRect(fOX - fOR, fOY - fOR, fOX + fOR, fOY + fOR), m_startPt, m_oldPt);
									else
										dc.Arc(CRect(fOX - fOR, fOY - fOR, fOX + fOR, fOY + fOR), m_oldPt, m_startPt);
								}
								else
								{
									if(pt1.y - (m_oldPt.x - pt1.x) / k1 > m_oldPt.y)
										dc.Arc(CRect(fOX - fOR, fOY - fOR, fOX + fOR, fOY + fOR), m_oldPt, m_startPt);
									else
										dc.Arc(CRect(fOX - fOR, fOY - fOR, fOX + fOR, fOY + fOR), m_startPt, m_oldPt);
								}
							}
							else
							{
								if(m_startPt.y < m_savePoint.y)
								{
									if(pt1.y - (m_oldPt.x - pt1.x) / k1 > m_oldPt.y)
										dc.Arc(CRect(fOX - fOR, fOY - fOR, fOX + fOR, fOY + fOR), m_oldPt, m_startPt);
									else
										dc.Arc(CRect(fOX - fOR, fOY - fOR, fOX + fOR, fOY + fOR), m_startPt, m_oldPt);
								}
								else
								{
									if(pt1.y - (m_oldPt.x - pt1.x) / k1 > m_oldPt.y)
										dc.Arc(CRect(fOX - fOR, fOY - fOR, fOX + fOR, fOY + fOR), m_startPt, m_oldPt);
									else
										dc.Arc(CRect(fOX - fOR, fOY - fOR, fOX + fOR, fOY + fOR), m_oldPt, m_startPt);

								}

							}
						}

						pt2 = CPoint2D((m_startPt.x + point.x) / 2.0, (m_startPt.y + point.y) / 2.0);
						k2 = (double)(m_startPt.x - point.x) / (double)(point.y - m_startPt.y);
						fOX = (pt2.y - pt1.y + k1 * pt1.x - k2 * pt2.x) / (k1 - k2);
						fOY = (k1 * k2 * (pt1.x - pt2.x) + pt2.y * k1 - pt1.y * k2) / (k1 - k2);
						fOR = funArray.GetDistance(CPoint2D(fOX, fOY), CPoint2D(point.x, point.y));

						//如果画的是垂直线
						if(m_startPt.x == m_savePoint.x)
						{
							if(m_startPt.y < m_savePoint.y)
							{
								if(point.x > m_startPt.x)
								{
									dc.Arc(CRect(fOX - fOR, fOY - fOR, fOX + fOR, fOY + fOR), m_startPt, point);
									m_bIsOrder = FALSE;
								}
								else
								{
									dc.Arc(CRect(fOX - fOR, fOY - fOR, fOX + fOR, fOY + fOR), point, m_startPt);
									m_bIsOrder = TRUE;
								}
							}
							else
							{
								if(point.x > m_startPt.x)
								{
									dc.Arc(CRect(fOX - fOR, fOY - fOR, fOX + fOR, fOY + fOR), point, m_startPt);
									m_bIsOrder = TRUE;
								}
								else
								{
									dc.Arc(CRect(fOX - fOR, fOY - fOR, fOX + fOR, fOY + fOR), m_startPt, point);
									m_bIsOrder = FALSE;
								}

							}
						}

						//一般的斜线
						else
						{
							if(k1 < 0)
							{
								if(m_startPt.y < m_savePoint.y)
								{
									if(pt1.y - (point.x - pt1.x) / k1 > point.y)
									{
										dc.Arc(CRect(fOX - fOR, fOY - fOR, fOX + fOR, fOY + fOR), m_startPt, point);
										m_bIsOrder = FALSE;
									}
									else
									{
										dc.Arc(CRect(fOX - fOR, fOY - fOR, fOX + fOR, fOY + fOR), point, m_startPt);
										m_bIsOrder = TRUE;
									}
								}
								else
								{
									if(pt1.y - (point.x - pt1.x) / k1 > point.y)
									{
										dc.Arc(CRect(fOX - fOR, fOY - fOR, fOX + fOR, fOY + fOR), point, m_startPt);
										m_bIsOrder = TRUE;
									}
									else
									{
										dc.Arc(CRect(fOX - fOR, fOY - fOR, fOX + fOR, fOY + fOR), m_startPt, point);
										m_bIsOrder = FALSE;
									}
								}
							}
							else
							{
								if(m_startPt.y < m_savePoint.y)
								{
									if(pt1.y - (point.x - pt1.x) / k1 > point.y)
									{
										dc.Arc(CRect(fOX - fOR, fOY - fOR, fOX + fOR, fOY + fOR), point, m_startPt);
										m_bIsOrder = TRUE;
									}
									else
									{
										dc.Arc(CRect(fOX - fOR, fOY - fOR, fOX + fOR, fOY + fOR), m_startPt, point);
										m_bIsOrder = FALSE;
									}
								}
								else
								{
									if(pt1.y - (point.x - pt1.x) / k1 > point.y)
									{
										dc.Arc(CRect(fOX - fOR, fOY - fOR, fOX + fOR, fOY + fOR), m_startPt, point);
										m_bIsOrder = FALSE;
									}
									else
									{
										dc.Arc(CRect(fOX - fOR, fOY - fOR, fOX + fOR, fOY + fOR), point, m_startPt);
										m_bIsOrder = TRUE;
									}	
								}
							}
						}
					}
				}
				m_oldPt = point;

				break;

			case 5:
				::SetCursor(m_HCross);  
				if(m_iRectCount == 1)
				{
					dc.Rectangle(CRect(rectStartPoint, m_oldPt));
					dc.Rectangle(CRect(rectStartPoint, point));
					m_oldPt = point;
				}
				break;

			case 6:
				::SetCursor(m_HCross);  
				if(m_iEllipseCount == 1)
				{
					dc.MoveTo(m_centerPoint);
					dc.LineTo(m_oldPt);

					dc.MoveTo(m_centerPoint);
					dc.LineTo(point);
				}
				if(m_iEllipseCount == 2)
				{
					tempMajor = funArray.GetDistance(CPoint2D(m_centerPoint.x, m_centerPoint.y)
						, CPoint2D(m_firstPoint.x, m_firstPoint.y));

					angle = atan((double)(m_firstPoint.y - m_centerPoint.y) / (double)(m_firstPoint.x - m_centerPoint.x));
					tempMinor = funArray.GetDistance(CPoint2D(m_centerPoint.x, m_centerPoint.y)
						, CPoint2D(m_oldPt.x, m_oldPt.y));

					if(tempMajor <= tempMinor)
					{
						/*tempLength = tempMajor;
						tempMajor = tempMinor;
						tempMinor = tempLength;
						angle += PI / 2;*/
					}

					dc.MoveTo(CPoint(
						funArray.GetEllipsePoint(tempMajor, tempMinor, angle, CPoint2D(m_centerPoint.x, m_centerPoint.y), 0, FALSE).x, 
						funArray.GetEllipsePoint(tempMajor, tempMinor, angle, CPoint2D(m_centerPoint.x, m_centerPoint.y), 0, FALSE).y)
						);
					for(int i = 1;i < 360;++i)		
						dc.LineTo(CPoint(
						funArray.GetEllipsePoint(tempMajor, tempMinor, angle, CPoint2D(m_centerPoint.x, m_centerPoint.y), i, FALSE).x, 
						funArray.GetEllipsePoint(tempMajor, tempMinor, angle, CPoint2D(m_centerPoint.x, m_centerPoint.y), i, FALSE).y)
						);

					//dc.LineTo(funArray.getPoint(tempMajor,tempMinor,angle,m_centerPoint,0));


					//angle = tempAngle;
					tempMinor = funArray.GetDistance(CPoint2D(m_centerPoint.x, m_centerPoint.y), CPoint2D(point.x, point.y));

					if(tempMajor <= tempMinor)
					{
						/*tempLength = tempMajor;
						tempMajor = tempMinor;
						tempMinor = tempLength;
						angle += PI / 2;*/
					}
					dc.MoveTo(CPoint(
						funArray.GetEllipsePoint(tempMajor, tempMinor, angle, CPoint2D(m_centerPoint.x, m_centerPoint.y), 0, FALSE).x, 
						funArray.GetEllipsePoint(tempMajor, tempMinor, angle, CPoint2D(m_centerPoint.x, m_centerPoint.y), 0, FALSE).y)
						);
					for(int i = 1; i < 360; ++i)		
						dc.LineTo(CPoint(
						funArray.GetEllipsePoint(tempMajor, tempMinor, angle, CPoint2D(m_centerPoint.x, m_centerPoint.y), i, FALSE).x, 
						funArray.GetEllipsePoint(tempMajor, tempMinor, angle, CPoint2D(m_centerPoint.x, m_centerPoint.y), i, FALSE).y)
						);
				}
				else if(m_iEllipseCount == 3)
				{

				}
				else if(m_iEllipseCount == 4)
				{

				}
				m_oldPt = point;

				break;
			}
		}

		//06-27修改Cf
		//Start
		else if(m_CommandType == CT_MOVE || m_CommandType == CT_COPY)
		{
			if(g_OperationArray.IsEmpty())
				return;

			movePoint = CPoint2D(point.x, point.y);

			oldDx = m_oldMovePoint.x - m_downPoint.x;
			oldDy = m_oldMovePoint.y - m_downPoint.y;

			newDx = movePoint.x - m_downPoint.x;
			newDy = movePoint.y - m_downPoint.y;

			dc1.MoveTo(m_downPoint.x, m_downPoint.y);
			dc1.LineTo(m_oldMovePoint.x, m_oldMovePoint.y);

			dc1.MoveTo(m_downPoint.x, m_downPoint.y);
			dc1.LineTo(movePoint.x, movePoint.y);
			for(int i = 0;i < g_OperationArray.GetSize();++i)
			{
				bDirection = ((CGraph*)g_OperationArray.GetAt(i))->m_bDirection;
				fRadius = ((CGraph*)g_OperationArray.GetAt(i))->m_fRadius;
				fMajor = ((CGraph*)g_GraphArray.GetAt(i))->m_fMajor;
				fMinor = ((CGraph*)g_GraphArray.GetAt(i))->m_fMinor;
				degree = ((CGraph*)g_GraphArray.GetAt(i))->m_fDegree;

				startAngle = ((CGraph*)g_GraphArray.GetAt(i))->m_fStartAngle;
				endAngle = ((CGraph*)g_GraphArray.GetAt(i))->m_fEndAngle;

				switch(((CGraph*)g_OperationArray.GetAt(i))->m_iDrawType)
				{
				case 2://直线
					oldStartPoint = CPoint2D(oldDx, -oldDy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptOrigin / g_fRate;
					oldEndPoint = CPoint2D(oldDx, -oldDy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptEnd / g_fRate;

					oldStartPoint = funArray.MyQuadrantToScreen(oldStartPoint) + CPoint2D(-g_hOffset, g_vOffset);
					oldEndPoint = funArray.MyQuadrantToScreen(oldEndPoint) + CPoint2D(-g_hOffset, g_vOffset);

					dc.MoveTo(oldStartPoint.x, oldStartPoint.y);
					dc.LineTo(oldEndPoint.x, oldEndPoint.y);

					newStartPoint = CPoint2D(newDx, -newDy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptOrigin / g_fRate;
					newEndPoint = CPoint2D(newDx, -newDy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptEnd / g_fRate;

					newStartPoint = funArray.MyQuadrantToScreen(newStartPoint) + CPoint2D(-g_hOffset, g_vOffset);
					newEndPoint = funArray.MyQuadrantToScreen(newEndPoint) + CPoint2D(-g_hOffset, g_vOffset);

					dc.MoveTo(newStartPoint.x, newStartPoint.y);
					dc.LineTo(newEndPoint.x, newEndPoint.y);
					m_oldMovePoint = movePoint;
					break;

				case 3://整圆
					oldCenterPoint = CPoint2D(oldDx, -oldDy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptCenter / g_fRate;
					oldCenterPoint = funArray.MyQuadrantToScreen(oldCenterPoint) + CPoint2D(-g_hOffset, g_vOffset);
					dc.Ellipse(CRect(oldCenterPoint.x - fRadius / g_fRate, oldCenterPoint.y - fRadius / g_fRate, oldCenterPoint.x + fRadius / g_fRate, oldCenterPoint.y + fRadius / g_fRate));

					newCenterPoint = CPoint2D(newDx, -newDy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptCenter / g_fRate;
					newCenterPoint = funArray.MyQuadrantToScreen(newCenterPoint) + CPoint2D(-g_hOffset, g_vOffset);
					dc.Ellipse(CRect(newCenterPoint.x - fRadius / g_fRate, newCenterPoint.y - fRadius / g_fRate, newCenterPoint.x + fRadius / g_fRate, newCenterPoint.y + fRadius / g_fRate));
					m_oldMovePoint = movePoint;
					break;

				case 4://圆弧
					oldStartPoint = CPoint2D(oldDx, -oldDy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptOrigin / g_fRate;
					oldEndPoint = CPoint2D(oldDx, -oldDy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptEnd / g_fRate;
					oldCenterPoint = CPoint2D(oldDx, -oldDy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptCenter / g_fRate;

					oldStartPoint = funArray.MyQuadrantToScreen(oldStartPoint) + CPoint2D(-g_hOffset, g_vOffset);
					oldEndPoint = funArray.MyQuadrantToScreen(oldEndPoint) + CPoint2D(-g_hOffset, g_vOffset);
					oldCenterPoint = funArray.MyQuadrantToScreen(oldCenterPoint) + CPoint2D(-g_hOffset, g_vOffset);

					if(bDirection)
						dc.Arc(CRect(oldCenterPoint.x - fRadius / g_fRate, oldCenterPoint.y - fRadius / g_fRate, oldCenterPoint.x + fRadius / g_fRate, oldCenterPoint.y + fRadius / g_fRate), CPoint(oldEndPoint.x, oldEndPoint.y), CPoint(oldStartPoint.x, oldStartPoint.y));
					else
						dc.Arc(CRect(oldCenterPoint.x - fRadius / g_fRate, oldCenterPoint.y - fRadius / g_fRate, oldCenterPoint.x + fRadius / g_fRate, oldCenterPoint.y + fRadius / g_fRate), CPoint(oldStartPoint.x, oldStartPoint.y), CPoint(oldEndPoint.x, oldEndPoint.y));

					newStartPoint = CPoint2D(newDx, -newDy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptOrigin / g_fRate;
					newEndPoint = CPoint2D(newDx, -newDy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptEnd / g_fRate;
					newCenterPoint = CPoint2D(newDx, -newDy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptCenter / g_fRate;

					newStartPoint = funArray.MyQuadrantToScreen(newStartPoint) + CPoint2D(-g_hOffset, g_vOffset);
					newEndPoint = funArray.MyQuadrantToScreen(newEndPoint) + CPoint2D(-g_hOffset, g_vOffset);
					newCenterPoint = funArray.MyQuadrantToScreen(newCenterPoint) + CPoint2D(-g_hOffset, g_vOffset);

					if(bDirection)
						dc.Arc(CRect(newCenterPoint.x - fRadius / g_fRate, newCenterPoint.y - fRadius / g_fRate, newCenterPoint.x + fRadius / g_fRate, newCenterPoint.y + fRadius / g_fRate),CPoint(newEndPoint.x, newEndPoint.y), CPoint(newStartPoint.x, newStartPoint.y));
					else
						dc.Arc(CRect(newCenterPoint.x - fRadius / g_fRate, newCenterPoint.y - fRadius / g_fRate, newCenterPoint.x + fRadius / g_fRate, newCenterPoint.y + fRadius / g_fRate),CPoint(newStartPoint.x, newStartPoint.y), CPoint(newEndPoint.x, newEndPoint.y));

					m_oldMovePoint = movePoint;
					break;

				case 5://矩形
					oldStartPoint = CPoint2D(oldDx, -oldDy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptOrigin / g_fRate;
					oldEndPoint = CPoint2D(oldDx, -oldDy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptEnd / g_fRate;

					oldStartPoint = funArray.MyQuadrantToScreen(oldStartPoint) + CPoint2D(-g_hOffset, g_vOffset);
					oldEndPoint = funArray.MyQuadrantToScreen(oldEndPoint) + CPoint2D(-g_hOffset, g_vOffset);
					dc.Rectangle(oldStartPoint.x, oldStartPoint.y, oldEndPoint.x, oldEndPoint.y);

					newStartPoint = CPoint2D(newDx,-newDy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptOrigin / g_fRate;
					newEndPoint = CPoint2D(newDx,-newDy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptEnd / g_fRate;

					newStartPoint = funArray.MyQuadrantToScreen(newStartPoint) + CPoint2D(-g_hOffset, g_vOffset);
					newEndPoint = funArray.MyQuadrantToScreen(newEndPoint) + CPoint2D(-g_hOffset, g_vOffset);
					dc.Rectangle(newStartPoint.x, newStartPoint.y, newEndPoint.x, newEndPoint.y);
					m_oldMovePoint = movePoint;
					break;

				case 6://椭圆
					oldCenterPoint = ((CGraph*)g_OperationArray.GetAt(i))->m_ptCenter;
					oldStartPoint = funArray.GetEllipsePoint(fMajor, fMinor, degree, oldCenterPoint, 
						startAngle * 180 / PI, bDirection);

					oldCenterPoint = CPoint2D(oldDx, -oldDy) + oldCenterPoint / g_fRate;
					oldStartPoint = CPoint2D(oldDx, -oldDy) + oldStartPoint / g_fRate;

					dc.MoveTo(oldStartPoint.x + 20 + g_hSetOffset, 
						g_Rect.Height() - oldStartPoint.y - g_vSetOffset);

					for (int m = (int)(startAngle * 180 / PI); m <= (int)(endAngle * 180 / PI) + 1; m++)
						dc.LineTo(CPoint(funArray.GetEllipsePoint(fMajor, fMinor, degree, oldCenterPoint, 
						m,bDirection).x / g_fRate + 20 + g_hSetOffset,
						g_Rect.Height() - g_vSetOffset - funArray.GetEllipsePoint(fMajor, fMinor, degree, oldCenterPoint, m, bDirection).y / g_fRate));

					newCenterPoint = ((CGraph*)g_OperationArray.GetAt(i))->m_ptCenter;
					newStartPoint = funArray.GetEllipsePoint(fMajor, fMinor, degree, newCenterPoint, 
						startAngle * 180 / PI, bDirection);

					newCenterPoint = CPoint2D(newDx, -newDy) + newCenterPoint / g_fRate;
					newStartPoint = CPoint2D(newDx, -newDy) + newStartPoint / g_fRate;

					dc.MoveTo(newStartPoint.x + 20 + g_hSetOffset,
						g_Rect.Height() - newStartPoint.y - g_vSetOffset);

					for (int m = (int)(startAngle * 180 / PI); m <= (int)(endAngle * 180 / PI) + 1; m++)
						dc.LineTo(CPoint(funArray.GetEllipsePoint(fMajor, fMinor, degree, newCenterPoint, 
						m,bDirection).x / g_fRate + 20 + g_hSetOffset, 
						g_Rect.Height() - g_vSetOffset - funArray.GetEllipsePoint(fMajor, fMinor, degree, newCenterPoint, m, bDirection).y / g_fRate));

					m_oldMovePoint = movePoint;
					break;

				case 7://多段线

					break;

				case 8://样条
					NodeArray = vector<vec2>(((CGraph*)g_GraphArray.GetAt(i))->controlNode.begin(), 
						((CGraph*)g_GraphArray.GetAt(i))->controlNode.end());

					bCurve = CBspline(NodeArray);
					bCurve.DrawInterpolation();

					for(int j = 0; j < bCurve.m_interpolationNode.size() - 1; ++j)
					{
						oldStartPoint = CPoint2D(oldDx, -oldDy) + CPoint2D(bCurve.m_interpolationNode[j][0], bCurve.m_interpolationNode[j][1]);
						oldEndPoint = CPoint2D(oldDx, -oldDy) + CPoint2D(bCurve.m_interpolationNode[j + 1][0], bCurve.m_interpolationNode[j + 1][1]);
						dc.MoveTo(oldStartPoint.x / g_fRate + 20 + g_hSetOffset, 
							g_Rect.Height() - g_vSetOffset - oldStartPoint.y / g_fRate);
						dc.LineTo(oldEndPoint.x / g_fRate + 20 + g_hSetOffset, 
							g_Rect.Height() - g_vSetOffset - oldEndPoint.y / g_fRate);
					}

					for(int j = 0; j < bCurve.m_interpolationNode.size() - 1; ++j)
					{
						newStartPoint = CPoint2D(newDx, -newDy) + CPoint2D(bCurve.m_interpolationNode[j][0], bCurve.m_interpolationNode[j][1]);
						newEndPoint = CPoint2D(newDx, -newDy) + CPoint2D(bCurve.m_interpolationNode[j + 1][0], bCurve.m_interpolationNode[j + 1][1]);
						dc.MoveTo(newStartPoint.x / g_fRate + 20 + g_hSetOffset, 
							g_Rect.Height() - g_vSetOffset - newStartPoint.y / g_fRate);
						dc.LineTo(newEndPoint.x / g_fRate + 20 + g_hSetOffset, 
							g_Rect.Height() - g_vSetOffset - newEndPoint.y / g_fRate);
					}

					m_oldMovePoint = movePoint;
					break;

				case 9://点
					oldStartPoint = CPoint2D(oldDx, -oldDy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptOrigin / g_fRate;
					oldStartPoint = funArray.MyQuadrantToScreen(oldStartPoint) + CPoint2D(-g_hOffset, g_vOffset);
					DrawPoint(&dc, oldStartPoint);
					newStartPoint = CPoint2D(newDx, -newDy) + ((CGraph*)g_OperationArray.GetAt(i))->m_ptOrigin / g_fRate;
					newStartPoint = funArray.MyQuadrantToScreen(newStartPoint) + CPoint2D(-g_hOffset, g_vOffset);
					DrawPoint(&dc, newStartPoint);
					m_oldMovePoint = movePoint;
					break;
				}
				//UpdateData();
			}
		}
		//End

		/*else if(m_CommandType == CT_COPY)
		{

		}*/
		else if(m_CommandType == CT_SCREENMOVE)
		{
			g_hSetOffset = m_OldHSetOffset + (point.x - m_screenMoveStartPoint.x) / g_fRate;
			g_vSetOffset = m_OldVSetOffset + (m_screenMoveStartPoint.y - point.y) / g_fRate;

			Invalidate();
		}
	}

	CString MousePos;
	CPoint2D MousePoint;
	MousePoint = funArray.ScreenToMyQuadrant(point) + CPoint2D(g_hOffset, g_vOffset);

	//06-30修改Cf
	//Start
	CString strCurMachinePos, strCurWindowsPos;
	strCurMachinePos.Format(L"机床坐标：%.f, %.f", MousePoint.x * g_fRate, MousePoint.y * g_fRate);
	strCurWindowsPos.Format(L"窗口坐标：%d, %d", point.x, point.y);

	((CStatusBar*)GetParent()->GetDescendantWindow(AFX_IDW_STATUS_BAR))->SetPaneInfo(1, ID_MACHINE_POS, SBPS_NORMAL, 150);
	((CStatusBar*)GetParent()->GetDescendantWindow(AFX_IDW_STATUS_BAR))->SetPaneText(1, strCurMachinePos);

	((CStatusBar*)GetParent()->GetDescendantWindow(AFX_IDW_STATUS_BAR))->SetPaneInfo(2, ID_MACHINE_POS, SBPS_NORMAL, 150);
	((CStatusBar*)GetParent()->GetDescendantWindow(AFX_IDW_STATUS_BAR))->SetPaneText(2, strCurWindowsPos);

	//MousePos.Format(L"当前机床坐标：%.f, %.f, 当前窗口坐标：%d, %d", MousePoint.x * g_fRate, MousePoint.y * g_fRate, point.x, point.y);
	//GetParent()->GetDescendantWindow(AFX_IDW_STATUS_BAR)->SetWindowText(MousePos);
	//End

	CView::OnMouseMove(nFlags, point);
}


/***********************************
** 文件保存函数
** 参数
** 时间：2013-5-10
***********************************/
void Cd2gView::OnFileSave()
{
	// TODO: Add your command handler code here
	int k = 0 , j = 0, m = 0, h = 0, tmIndex = 0;

	m_bIsLink = FALSE;
	int bIsWithC = 0;//是否产生C角
	int saveAllOrSelected = 0, nOptimizeSelected = 0;

	double lastEndX = 0, lastEndY = 0, lastEndZ = 0, lastEndC = 0;//上一终点坐标及上一C角 by yuanlin 2013.5.20
	double fOX = 0, fOY = 0, fOR = 0;//圆心坐标和半径
	double angle = 0, startAngle = 0, endAngle = 0;

	COLORREF clrREF = 0;
	double lineC = 0, fStepX = 0, fStepY = 0;
	double fCurStartC = 0.0, fCurEndC = 0.0, fNextStartC = 0.0, fNextEndC = 0.0;
	double fOffsetC = 0.0, fPerOffsetC = 0.0;
	double fUnit = 0;
	UINT nCount1 = 0, nCount2 = 0, nCount = 0;
	double fLength = 0, fPolySumLen = 0.0;
	double fStartAngle = 0.0, fEndAngle = 0.0;
	double fStartC = 0.0, fEndC = 0.0;
	int iLineCount = 0, iArcCount = 0;

	CPoint2D pt1, pt2, centerPoint;;
	CString strTool = L"";
	double x = 0, y = 0, z = 0, c = 0, r = 0;
	BOOL bFlag = TRUE, gbTransition = FALSE, perbTransition = FALSE;
	//UINT uFlags;
	double LineStep = 0, con = 0, fArcStep = 0.0;
	double fMajor = 0, fMinor = 0;
	double startX = 0, startY = 0, startC = 0.0, endX = 0, endY = 0, nextX = 0,nextY = 0, lastX = 0,lastY = 0, lastZ = 0, lastC = 0.0, tmpX = 0.0, tmpY = 0.0;
	double nextStartX = 0.0, nextStartY = 0.0, nextEndX = 0.0, nextEndY = 0.0;
	double fPointPerDepth = 0.0, fPercentDepth = 0.0;
	int nPtNumber = 0, nextIndex = 0;
	CArray<CPoint2D> ptMember;
	double *pfPoint = NULL, *pfLength = NULL;
	double fFirstC = 0, fLastC = 0, fEllipseStep = 0, fDelte = 0;
	vector<vec2> NodeArray;
	UINT iDrawType/*, iNextDrawType*/;
	size_t nPolyVertexCount = 0/*端点个数*/, nTempCount = 0, size = 0, nPolySectionCount = 0/*段数*/;

	CBspline bCurve;
	BOOL isFormer = FALSE, bDirection;
	double fDistance = 0;
	CPoint2D pFirst, pNext, pSecond;
	int iMaxSize = 0, iGraphNum = 0, index = 0, iNum = 0, iIndex = 0, iGroupGraph = 0;
	int /*iFirstIndex,*/ iLastIndex;
	int iStartIndex/*, iEndIndex*/;
	INT_PTR nSize = 0;
	BOOL isPoint = FALSE, lastIsPoint = FALSE, isArc = FALSE, lastIsArc = FALSE;
	BOOL bTransition = FALSE, bSetNewStepFlags = FALSE;
	COLORREF gClr = 0;
	double fNewStep = 0.0, fSplineStep = 0.0, fCurveStep_Len = 0.0, fAsymptotic = 0.0, fCoincide_Len = 0.0;
	double fDelteL = 0.0, fDelteC = 0.0;;

	double fEllipse2PointLen = 0.0, fSplineLen = 0.0;
	CPoint2D lastPoint = CPoint2D(-1, -1);

	//vector<CInfomation> EachEdgeOfPolyLine;
	//vector<vector<CInfomation>> EachPolyLine;
	std::size_t /*EachCounts,*/ CurCounts = 0;
	std::vector<CInfomation> EachOfPoint;
	double fNeedAngle = 0.0;

	CInfomation ci, curInfo;
	int nCoinCount = 0;//重合步数
	double fUpDownStep = 0.0;
	double fPerStep = 0.0, fSumLength = 0.0;
	BOOL bConnection = FALSE;

	double basePoint_X = 0;
	double basePoint_Y = 0;
	CString strExePath = GetSavePath(m_savePath);
	FILE *SaveFile = NULL;
	double ToolRadius = 0.0, fTheoryHalfLenght = 0.0, fDepth = 0.0;
	int iFeedSpeed = 0;
	BOOL bSetFeedSpeed = FALSE;
	int ToolNum = 0;
	double ToolRadius1 = 0.0, ToolRadius2 = 0.0;
	double ToolRadius3 = 0.0, ToolRadius4 = 0.0;
	double ToolRadius5 = 0.0, ToolRadius6 = 0.0;
	BOOL bFirstToDepth = FALSE;
	double fPointDepth = 0.0;
	BOOL bSetPointDepth = FALSE;
	CGraph *pCurGraph = NULL;
	CGraph *pNextGraph = NULL;

	if(tmSelect.size() == 0)
	{
		MessageBox(L"请先从刀具管理中选择加工刀具，然后再保存!");
		return ;
	}
	else
	{
		CString strTool = L"", strCurTool = L"";
		for(int nSelectedIndex = 0; nSelectedIndex < tmSelect.size(); ++nSelectedIndex)	
		{
			strCurTool = tmVector.at(tmSelect.at(nSelectedIndex)).ToolNo;
			strTool += strCurTool + L",";
		}

		int nMessage = MessageBox(L"当前选择的刀具号为" + strTool + L"是否进行G代码保存？", L"提示" , MB_ICONWARNING | MB_OKCANCEL);
		if(nMessage != IDOK)
		{
			return ;
		}
	}

	USES_CONVERSION;
	LPSTR filePath = T2A(strExePath + _T("\\") + m_DefaultFileName);

	fopen_s(&SaveFile, filePath, "w+");
	if(! SaveFile)
	{
		TRACE0("Open failed...");
		return ;
	}

	try
	{
		if(m_iWorkStation == CARVE)//刻花机
		{
			g_SaveArray.RemoveAll();

			CPreSaveDlg PreSaveDlg;
			if(IDOK != PreSaveDlg.DoModal())
			{
				fclose(SaveFile);
				return;
			}
			basePoint_X = PreSaveDlg.m_basePoint_X;
			basePoint_Y = PreSaveDlg.m_basePoint_Y;
			bIsWithC = PreSaveDlg.m_SelectIndex;
			saveAllOrSelected = PreSaveDlg.m_saveAllOrSelected;
			nOptimizeSelected = PreSaveDlg.m_nOptimizeSelected;

			if(!saveAllOrSelected)
				g_SaveArray.Copy(g_GraphArray);
			else
				g_SaveArray.Copy(g_OperationArray);

			iMaxSize = g_SaveArray.GetSize();
			g_BetterPathPoint = new CPoint2D[iMaxSize * 2];

			funArray.DataMalloc(g_SaveArray);
			funArray.OptimizePath(g_SaveArray, CPoint2D(basePoint_X, basePoint_Y), !nOptimizeSelected);

			if(bAutoBreath)
				fprintf_s(SaveFile, "M28\n");

			OperatorGroup(g_SaveArray);

			//06-25修改Cf
			//Start
			/*while(tmIndex < tmSelect.size())
			{*/
			//End

			USES_CONVERSION;
			LPSTR tool = T2A(tmVector.at(tmSelect.at(tmIndex)).ToolNo);
			fprintf_s(SaveFile, "%s\nM8\nM3\nS%d\nM24\nM24\nG54\nG90\n", tool, tmVector.at(tmSelect.at(tmIndex)).RotationRate);

			ToolNum = tmSelect.at(tmIndex);
			switch(ToolNum)
			{
			case 0:
			case 1:
				ToolRadius1 = tmVector.at(0).ToolRadius;
				ToolRadius2 = tmVector.at(1).ToolRadius;
				ToolRadius = ToolRadius1 > ToolRadius2 ? ToolRadius1 : ToolRadius2;
				break;

			case 2:
			case 3:
				ToolRadius3 = tmVector.at(2).ToolRadius;
				ToolRadius4 = tmVector.at(3).ToolRadius;
				ToolRadius = ToolRadius3 > ToolRadius4 ? ToolRadius3 : ToolRadius4;
				break;

			case 4:
			case 5:
				ToolRadius5 = tmVector.at(4).ToolRadius;
				ToolRadius6 = tmVector.at(5).ToolRadius;
				ToolRadius = ToolRadius5 > ToolRadius6 ? ToolRadius5 : ToolRadius6;
				break;
			}

			//ToolRadius = tmVector.at(tmSelect.at(tmIndex)).ToolRadius;

			//理论上一半的长度
			fTheoryHalfLenght = sqrt(pow(ToolRadius, 2.0) - pow(ToolRadius + g_fDepth, 2.0));

			iGroupGraph = g_VectorGroupGraph.size();
			iGraphNum = 0;
			while(iGraphNum < iGroupGraph)
			{
				nSize = g_VectorGroupGraph[iGraphNum].m_GraphIndex.size();

				//一个组中只有一条线段，使用默认的渐近比例
				if(nSize == 1)
				{
					index = g_VectorGroupGraph[iGraphNum].m_GraphIndex.at(0);
					lastIsPoint = isPoint;
					isPoint = FALSE;
					lastIsArc = isArc;
					isArc = FALSE;

					iDrawType = ((CGraph*)g_SaveArray.GetAt(index))->m_iDrawType;
					startX = ((CGraph*)g_SaveArray.GetAt(index))->m_ptOrigin.x ;
					startY = ((CGraph*)g_SaveArray.GetAt(index))->m_ptOrigin.y ;
					endX = ((CGraph*)g_SaveArray.GetAt(index))->m_ptEnd.x ;
					endY = ((CGraph*)g_SaveArray.GetAt(index))->m_ptEnd.y ;

					clrREF = ((CGraph*)g_SaveArray.GetAt(index))->clrREF;

					bSetNewStepFlags = ((CGraph*)g_SaveArray.GetAt(index))->m_bSetPerStep;
					fNewStep = ((CGraph*)g_SaveArray.GetAt(index))->m_fPerStep;
					bSetFeedSpeed = ((CGraph*)g_SaveArray.GetAt(index))->m_bSetFeedSpeed;

					gbTransition = FALSE;
					fAsymptotic = g_fAsymptotic;

					iFeedSpeed = g_iFeedSpeed;
					bFirstToDepth = FALSE;

					switch(iDrawType)
					{
					case 2://直线
						if(bSetFeedSpeed)
							iFeedSpeed = ((CGraph*)g_SaveArray.GetAt(index))->m_iPerFeedSpeed;	

						fDepth = g_fDepth;
						//计算直线总长并计算段数
						fLength = funArray.GetDistance(startX, startY, endX, endY);

						if(!bSetNewStepFlags)
						{
							if(fLength < 50)
								LineStep = g_fLineStep_S;
							else
								LineStep = g_fLineStep_L;
						}
						else
							LineStep = fNewStep;

						nCount1 = fLength / LineStep;

						//当实际长度小于两倍的理论值时，修改切削深度
						if(fTheoryHalfLenght * 2 > fLength)
						{
							fDepth = -ToolRadius + sqrt(pow(ToolRadius, 2.0) - pow(fLength / 2.0, 2.0));
						}

						if(clrREF == RGB(0, 0, 0))
						{
							clrREF = RGB(255, 0, 0);
							fAsymptotic = sqrt(pow(ToolRadius, 2.0) - pow(ToolRadius + fDepth, 2.0)) / fLength;
						}

						//06-30修改Cf
						//Start
						upDownCount = nCount1 * fAsymptotic;
						//if(upDownCount == 0)
						//	throw L"直线步长过大";
						//End

						upDownStep = fabs(fDepth) / upDownCount;
						upDownStep = funArray.GetNewStep(fabs(fDepth), upDownStep);//修改每步的渐近高度

						fStepX = (endX - startX) / nCount1;//在X轴方向上的单步长度
						fStepY = (endY - startY) / nCount1;//在Y轴方向上的单步长度

						lineC = funArray.GetC(2, startX, startY, endX, endY);

						if(fabs(startX - lastEndX) < 1e-3 && fabs(startY - lastEndY) < 1e-3 
							&& fabs(lineC - lastEndC) < g_fDeltaC1 || fabs(lineC - lastEndC) > 2 * PI - g_fDeltaC1)
							gbTransition = TRUE;

						//将线段末尾抬刀移到下一段起点处判断是否需要抬刀
						//*********by yuanlin 2013.5.20******************
						if(!bIsWithC)//产生C角
						{
							/*if (gbTransition)
							{
							fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",iFeedSpeed,lastEndX,lastEndY,0.0,RadianToAngle(lineC));
							}
							else
							{*/
							//上一线段抬刀
							if (iGraphNum > 0 && !lastIsPoint)
							{
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iDownSpeed, lastEndX, lastEndY, 0.0, RadianToAngle(lastEndC));
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iUpSpeed, lastEndX, lastEndY, g_fSafeHeight, RadianToAngle(lastEndC));
							}
							//下刀
							fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iG00Speed, startX, startY, g_fSafeHeight, RadianToAngle(lineC));
							fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iDownSpeed, startX, startY, 0.0, RadianToAngle(lineC));
							//}
						}
						else//不产生C角
						{
							/*if (gbTransition)
							{   
							fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",iFeedSpeed,lastEndX,lastEndY,0.0);
							}
							else
							{*/
							//上一线段抬刀
							if (iGraphNum > 0 && !lastIsPoint)
							{
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iDownSpeed, lastEndX, lastEndY, 0.0);
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iUpSpeed, lastEndX, lastEndY, g_fSafeHeight);
							}
							//下刀
							fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iG00Speed, startX, startY, g_fSafeHeight);
							fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iDownSpeed, startX, startY, 0.0);			
							//}
						}

						for(k = 1, j = nCount1 - 1; k < nCount1; ++k, --j)
						{
							if(clrREF == RGB(255,0,0))
								z = funArray.SetDepth1(fDepth, nCount1, k, j);
							else if(clrREF == RGB(0,255,0))
								z = funArray.SetDownDepth(fDepth, k);
							else if(clrREF == RGB(0,0,255))
								z = funArray.SetUpDepth(fDepth, nCount1, k);
							else
								z = fDepth;

							if(z > fDepth)
								bFirstToDepth = FALSE;

							if(!bIsWithC)
							{
								if(g_SaveArray.GetSize() != 0 && gbTransition)
								{
									if(z >= fDepth && !bFirstToDepth)
										fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iDownSpeed, startX + fStepX * k, startY + fStepY * k, z, RadianToAngle(lineC));
									else
										fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", iFeedSpeed, startX + fStepX * k, startY + fStepY * k, z, RadianToAngle(lineC));
								}
								else
								{
									if(z >= fDepth && !bFirstToDepth)
										fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iDownSpeed, startX + fStepX * k, startY + fStepY * k, z, RadianToAngle(lineC));
									else
										fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", iFeedSpeed, startX + fStepX * k, startY + fStepY * k, z, RadianToAngle(lineC));
								}
							}
							else
							{
								if(g_SaveArray.GetSize() != 0 && gbTransition)
								{
									if(z >= fDepth && !bFirstToDepth)
										fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iDownSpeed, startX + fStepX * k, startY + fStepY * k, z);
									else
										fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", iFeedSpeed, startX + fStepX * k, startY + fStepY * k, z);
								}
								else
								{
									if(z >= fDepth && !bFirstToDepth)
										fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iDownSpeed, startX + fStepX * k, startY + fStepY * k, z);
									else
										fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", iFeedSpeed, startX + fStepX * k, startY + fStepY * k, z);
								}
							}

							if(z == fDepth)
								bFirstToDepth = TRUE;

						}
						//注释抬刀过程，将抬刀过程移到下一线段开始处 by yuanlin 2013.5.21
						/*fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iUpSpeed,endX,endY,0.0,lineC);
						fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iUpSpeed,endX,endY,g_fSafeHeight,lineC);
						*/
						lastEndX = endX;	//*********by yuanlin 2013.5.21保存当前终点坐标为上一终点坐标
						lastEndY = endY;	//*********by yuanlin 2013.5.21保存当前终点坐标为上一终点坐标
						lastEndC = lineC;	//*********by yuanlin 2013.5.21保存当前终点坐标为上一终点坐标
						lastEndZ = z;

						basePoint_X = lastEndX;
						basePoint_Y = lastEndY;
						break;

					case 3://整圆
						if(bSetFeedSpeed)
							iFeedSpeed = ((CGraph*)g_SaveArray.GetAt(index))->m_iPerFeedSpeed;

						g_CircleArray.RemoveAll();
						//保存整圆的终点坐标及圆心相对于起点坐标的增量
						fOX = ((CGraph*)g_SaveArray.GetAt(index))->m_ptCenter.x ;
						fOY = ((CGraph*)g_SaveArray.GetAt(index))->m_ptCenter.y ;
						fOR = ((CGraph*)g_SaveArray.GetAt(index))->m_fRadius ;
						bDirection = ((CGraph*)g_SaveArray.GetAt(index))->m_bDirection;
						clrREF = ((CGraph*)g_SaveArray.GetAt(index))->clrREF;

						fLength = 2 * PI * fOR;
						fDepth = g_fDepth;
						//判断半径大小，决定弧长步长 by yuanlin 2013.5.21

						if(!bSetNewStepFlags)
						{
							if (fOR <= 30)
							{
								fCurveStep_Len = g_fCurveStep_S;
								//g_fCoincide_Len = g_fCoincide_S;
							}
							else if (fOR <= 50)
							{
								fCurveStep_Len = g_fCurveStep_M;
								//g_fCoincide_Len = g_fCoincide_M;
							}
							else if(fOR <= 100)
							{
								fCurveStep_Len = g_fCurveStep_L;
								//g_fCoincide_Len = g_fCoincide_L;
							}
							else if(fOR > 100)
							{
								fCurveStep_Len = g_fCurveStep_XL;
								//g_fCoincide_Len = g_fCoincide_XL;
							}
						}
						else
						{
							fCurveStep_Len = fNewStep;
						}

						nCount1 = fLength / fCurveStep_Len;
						fCurveStep_Len = funArray.GetNewStep(fLength,fCurveStep_Len);
						fUnit = fCurveStep_Len / fOR;//每一小段的弧度大小

						c = funArray.GetC(3, fOX, fOY, startX, startY, bDirection);

						g_CircleArray.Add(CInfomation(startX,startY,c));
						if(fabs(g_CircleArray[0].x - lastEndX) < 1e-3 && fabs(g_CircleArray[0].y - lastEndY) < 1e-3 && fabs(g_CircleArray[0].c - lastEndC) < 1e-3)
							gbTransition = TRUE;

						fCoincide_Len = fLength * g_fCoincideRatio;
						nCount2 = fCoincide_Len / fCurveStep_Len;
						nCount = nCount1 + nCount2 * 2;

						//将线段末尾抬刀移到下一段起点处判断是否需要抬刀
						//*********by yuanlin 2013.5.21******************
						if(!bIsWithC)
						{
							if (iGraphNum > 0 && !gbTransition && !lastIsPoint)
							{
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iDownSpeed, lastEndX, lastEndY, 0.0, RadianToAngle(lastEndC));
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iUpSpeed, lastEndX, lastEndY, g_fSafeHeight, RadianToAngle(lastEndC));
							}

							for(k = 1; k < nCount1; ++k)
								funArray.GetElement(3, g_CircleArray[k - 1].x, g_CircleArray[k - 1].y, g_CircleArray[k - 1].c, fOX, fOY, fUnit, k, bDirection);

							//g_CircleArray[0].c = g_CircleArray[nCount1 - 1].c;

							if(!gbTransition)
							{
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iG00Speed, g_CircleArray[0].x, g_CircleArray[0].y, g_fSafeHeight, RadianToAngle(g_CircleArray[0].c));
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iDownSpeed, g_CircleArray[0].x, g_CircleArray[0].y, 0.0, RadianToAngle(g_CircleArray[0].c));
							}
						}
						else
						{
							if (iGraphNum > 0 && !gbTransition && !lastIsPoint)
							{
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iDownSpeed, lastEndX, lastEndY, 0.0);
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iUpSpeed, lastEndX, lastEndY, g_fSafeHeight);
							}

							for(k = 1; k < nCount1; ++k)
								funArray.GetElement(3, g_CircleArray[k - 1].x, g_CircleArray[k - 1].y, g_CircleArray[k - 1].c, fOX, fOY, fUnit, k, bDirection);

							g_CircleArray[0].c = g_CircleArray[nCount1 - 1].c;

							if(!gbTransition)
							{
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iG00Speed, g_CircleArray[0].x, g_CircleArray[0].y, g_fSafeHeight);
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iDownSpeed, g_CircleArray[0].x, g_CircleArray[0].y, 0.0);
							}
						}

						size = g_CircleArray.GetSize();
						for(j = 1; j < nCount; ++j)
						{
							k = j % size;
							h = j % nCount1;

							if(k <= nCount / 2)
								c = g_CircleArray[k].c;
							else if(k == nCount1)
								c = g_CircleArray[k].c;
							else
								c = g_CircleArray[k].c;

							z = funArray.SetDepth2(iDrawType, fCoincide_Len, fDepth, fCurveStep_Len, nCount,j,nCount - j);

							if(z > fDepth)
								bFirstToDepth = FALSE;

							if(!bIsWithC)
							{
								if(z >= fDepth && !bFirstToDepth)
									fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iCoincideSpeed, g_CircleArray[k].x, g_CircleArray[k].y, z, RadianToAngle(c));
								else
									fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", iFeedSpeed, g_CircleArray[k].x, g_CircleArray[k].y, z, RadianToAngle(c));
							}
							else
							{
								if(z >= fDepth && !bFirstToDepth)
									fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iCoincideSpeed, g_CircleArray[k].x, g_CircleArray[k].y, z);
								else
									fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", iFeedSpeed, g_CircleArray[k].x, g_CircleArray[k].y, z);
							}

							if(z == fDepth)
								bFirstToDepth = TRUE;
						}

						//注释抬刀过程，将抬刀过程移到下一线段开始处 by yuanlin 2013.5.21
						/*fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iUpSpeed,pArray[k - nCount1].x,pArray[k - nCount1].y,0.0,pArray[k - nCount1].c);
						fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iUpSpeed,pArray[k - nCount1].x,pArray[k - nCount1].y,g_fSafeHeight,pArray[k - nCount1].c);
						*/
						//*********by yuanlin 2013.5.14******************

						lastEndX = g_CircleArray[k + 1].x;	//*********by yuanlin 2013.5.21保存当前终点坐标为上一终点坐标
						lastEndY = g_CircleArray[k + 1].y;	//*********by yuanlin 2013.5.21保存当前终点坐标为上一终点坐标
						lastEndC = g_CircleArray[k + 1].c;	//*********by yuanlin 2013.5.21保存当前终点坐标为上一终点坐标
						lastEndZ = z;// - fCompensationFactor;					//*********by yuanlin 2013.5.21保存当前终点坐标为上一终点坐标

						basePoint_X = lastEndX;
						basePoint_Y = lastEndY;
						break;

					case 4://圆弧
						if(bSetFeedSpeed)
							iFeedSpeed = ((CGraph*)g_SaveArray.GetAt(index))->m_iPerFeedSpeed;

						//保存圆弧的终点坐标及圆心相对于起点坐标的增量
						g_ArcArray.RemoveAll();
						//bFlag = TRUE;
						fOX = ((CGraph*)g_SaveArray.GetAt(index))->m_ptCenter.x ;
						fOY = ((CGraph*)g_SaveArray.GetAt(index))->m_ptCenter.y ;
						fOR = ((CGraph*)g_SaveArray.GetAt(index))->m_fRadius ;
						bDirection = ((CGraph*)g_SaveArray.GetAt(index))->m_bDirection;//获取圆弧是顺时针还是逆时针
						angle = ((CGraph*)g_SaveArray.GetAt(index))->m_fAngle;
						clrREF = ((CGraph*)g_SaveArray.GetAt(index))->clrREF;

						fDepth = g_fDepth;
						fLength = fOR * angle;
						//判断半径大小，决定弧长步长 by yuanlin 2013.5.21

						if(!bSetNewStepFlags)
						{
							if (fOR <= 30)
								fCurveStep_Len = g_fCurveStep_S;
							else if (fOR <= 50)
								fCurveStep_Len = g_fCurveStep_M;
							else if(fOR <= 100)
								fCurveStep_Len = g_fCurveStep_L;
							else if(fOR > 100)
								fCurveStep_Len = g_fCurveStep_XL;
						}
						else
							fCurveStep_Len = fNewStep;

						if(fTheoryHalfLenght * 2 > fLength)
							fDepth = -ToolRadius + sqrt(pow(ToolRadius, 2.0) - pow(fLength / 2.0, 2.0));

						if(clrREF == RGB(0, 0, 0))
						{
							clrREF = RGB(255, 0 ,0);
							fAsymptotic = sqrt(pow(ToolRadius, 2.0) - pow(ToolRadius + fDepth, 2.0)) / fLength;
						}

						nCount1 = fLength / fCurveStep_Len;
						fCurveStep_Len = funArray.GetNewStep(fLength, fCurveStep_Len);
						fUnit = fCurveStep_Len / fOR;

						upDownCount = fLength * fAsymptotic / fCurveStep_Len;
						upDownStep = fabs(fDepth) / upDownCount;
						upDownStep = funArray.GetNewStep(fabs(fDepth), upDownStep);//修改每步的渐近高度

						c = funArray.GetC(4, fOX, fOY, startX, startY, bDirection);
						g_ArcArray.Add(CInfomation(startX, startY, c));

						if(lastIsArc)
						{	
							if(fabs(startX - lastEndX) < 1e-3 
								&& fabs(startY - lastEndY) < 1e-3 
								&& (fabs(g_ArcArray[0].c - lastEndC) < g_fDeltaC1 
								|| fabs(g_ArcArray[0].c - lastEndC) > 2 * PI - g_fDeltaC1))
								gbTransition = TRUE;
						}
						else
						{

						}

						//将线段末尾抬刀移到下一段起点处判断是否需要抬刀
						//*********by yuanlin 2013.5.20******************
						if(!bIsWithC)
						{
							/*if (lastEndZ < 0 && gbTransition)
							{   
							if(clrREF == RGB(255,0,0))
							fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",iFeedSpeed,g_ArcArray[0].x,g_ArcArray[0].y,0.0,RadianToAngle(g_ArcArray[0].c));
							}
							else
							{*/
							//上一线段抬刀
							if (iGraphNum > 0 && !lastIsPoint)
							{
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iDownSpeed, lastEndX, lastEndY, 0.0, RadianToAngle(lastEndC));
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iUpSpeed, lastEndX, lastEndY, g_fSafeHeight, RadianToAngle(lastEndC));
							}
							//下刀
							fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iG00Speed, g_ArcArray[0].x, g_ArcArray[0].y, g_fSafeHeight, RadianToAngle(g_ArcArray[0].c));
							fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iDownSpeed, g_ArcArray[0].x, g_ArcArray[0].y, 0.0, RadianToAngle(g_ArcArray[0].c));
							//}
						}
						else
						{
							/*if (lastEndZ < 0 && gbTransition)
							{   
							if(clrREF == RGB(255,0,0))
							fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",iFeedSpeed,g_ArcArray[0].x,g_ArcArray[0].y,0.0);
							}
							else
							{*/
							//上一线段抬刀
							if (iGraphNum > 0 && !lastIsPoint)
							{
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iDownSpeed, lastEndX, lastEndY, 0.0);
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iUpSpeed, lastEndX, lastEndY, g_fSafeHeight);
							}
							//下刀
							fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iG00Speed, g_ArcArray[0].x, g_ArcArray[0].y, g_fSafeHeight);
							fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iDownSpeed, g_ArcArray[0].x, g_ArcArray[0].y, 0.0);
							//}
						}

						for(m = 1, j = nCount1 - 1; m < nCount1; ++m, --j)
						{
							if(clrREF == RGB(255, 0, 0))
								z = funArray.SetDepth1(fDepth, nCount1, m, j);
							else if(clrREF == RGB(0, 255, 0))
								z = funArray.SetDownDepth(fDepth, m);
							else if(clrREF == RGB(0, 0, 255))
								z = funArray.SetUpDepth(fDepth, nCount1, m);
							else 
								z = g_fDepth;

							if(z > fDepth)
								bFirstToDepth = FALSE;

							if(!bIsWithC)
							{
								if(iGraphNum != 0 && gbTransition)
								{
									funArray.GetElement(4, g_ArcArray[m - 1].x, g_ArcArray[m - 1].y, g_ArcArray[m - 1].c, fOX, fOY, fUnit, m, bDirection);

									if(z >= fDepth && !bFirstToDepth)
										fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iDownSpeed, g_ArcArray[m].x, g_ArcArray[m].y, z, RadianToAngle(g_ArcArray[m].c));
									else
										fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", iFeedSpeed, g_ArcArray[m].x, g_ArcArray[m].y, z, RadianToAngle(g_ArcArray[m].c));
								}
								else
								{
									funArray.GetElement(4, g_ArcArray[m - 1].x, g_ArcArray[m - 1].y, g_ArcArray[m - 1].c, fOX, fOY, fUnit, m, bDirection);

									if(z >= fDepth && !bFirstToDepth)
										fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iDownSpeed, g_ArcArray[m].x, g_ArcArray[m].y, z, RadianToAngle(g_ArcArray[m].c));
									else
										fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", iFeedSpeed, g_ArcArray[m].x, g_ArcArray[m].y, z, RadianToAngle(g_ArcArray[m].c));
								}
							}
							else
							{
								if(iGraphNum != 0 && gbTransition)
								{
									funArray.GetElement(4, g_ArcArray[m - 1].x, g_ArcArray[m - 1].y, g_ArcArray[m - 1].c, fOX, fOY, fUnit, m, bDirection);

									if(z >= fDepth && !bFirstToDepth)
										fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iDownSpeed, g_ArcArray[m].x, g_ArcArray[m].y, z);
									else
										fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", iFeedSpeed, g_ArcArray[m].x, g_ArcArray[m].y, z);
								}
								else
								{
									funArray.GetElement(4, g_ArcArray[m - 1].x, g_ArcArray[m - 1].y, g_ArcArray[m - 1].c, fOX, fOY, fUnit, m, bDirection);

									if(z >= fDepth && !bFirstToDepth)
										fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iDownSpeed, g_ArcArray[m].x, g_ArcArray[m].y, z);
									else
										fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n", iFeedSpeed, g_ArcArray[m].x, g_ArcArray[m].y, z);
								}
							}

							if(z == fDepth)
								bFirstToDepth = TRUE;
						}

						c = funArray.GetC(4, fOX, fOY, endX, endY, bDirection);
						/*if(bDirection)
						c += PI;
						c = c - PI * 2 < 0.0 ? c : c - PI * 2;*/
						//注释抬刀过程，将抬刀过程移到下一线段开始处 by yuanlin 2013.5.21
						/*fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",iFeedSpeed,endX,endY,0.0,c);
						fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",iFeedSpeed,endX,endY,g_fSafeHeight,c);			
						*/
						lastEndX = endX;	//*********by yuanlin 2013.5.21保存当前终点坐标为上一终点坐标
						lastEndY = endY;	//*********by yuanlin 2013.5.21保存当前终点坐标为上一终点坐标
						lastEndC = c;		//*********by yuanlin 2013.5.21保存当前终点坐标为上一终点坐标
						lastEndZ = z;// - fCompensationFactor;		//*********by yuanlin 2013.5.21保存当前终点坐标为上一终点坐标

						isArc = TRUE;
						basePoint_X = lastEndX;
						basePoint_Y = lastEndY;
						break;

					case 6:
						if(bSetFeedSpeed)
							iFeedSpeed = ((CGraph*)g_SaveArray.GetAt(index))->m_iPerFeedSpeed;

						g_EllipseArray.RemoveAll();
						z = g_fDepth;
						nCount1 = 0;
						nCount2 = 0;
						nCount = 0;
						fEllipse2PointLen = 0.0;
						fAsymptotic = g_fAsymptotic;
						fDepth = g_fDepth;

						fMajor = ((CGraph*)g_SaveArray.GetAt(index))->m_fMajor;
						fMinor = ((CGraph*)g_SaveArray.GetAt(index))->m_fMinor;
						startAngle = ((CGraph*)g_SaveArray.GetAt(index))->m_fStartAngle;
						endAngle = ((CGraph*)g_SaveArray.GetAt(index))->m_fEndAngle;
						angle = ((CGraph*)g_SaveArray.GetAt(index))->m_fDegree;
						fDelte = endAngle - startAngle - 2 * PI;

						pt2 = ((CGraph*)g_SaveArray.GetAt(index))->m_ptCenter;//中心点
						bDirection = ((CGraph*)g_SaveArray.GetAt(index))->m_bDirection;

						if(fMinor <= 30)
							fEllipseStep = g_fEllipseStep_S;
						else if(fMinor <= 50)
							fEllipseStep = g_fEllipseStep_M;
						else if(fMinor <= 100)
							fEllipseStep = g_fEllipseStep_L;
						else if(fMinor > 100)
							fEllipseStep = g_fEllipseStep_XL;

						fEllipseStep = funArray.GetNewStep((endAngle - startAngle) * 180 / PI,fEllipseStep);

						//获取起点信息
						if(!bDirection)//逆时针
						{
							//pFirst代表前一个点坐标，pSecond代表起点
							pFirst = funArray.GetEllipsePoint(fMajor, fMinor, angle, pt2, startAngle * 180 / PI - fEllipseStep, bDirection);
							pSecond = funArray.GetEllipsePoint(fMajor, fMinor, angle, pt2, startAngle * 180 / PI, bDirection);
						}
						else//顺时针
						{
							pFirst = funArray.GetEllipsePoint(fMajor, fMinor, angle, pt2, endAngle * 180 / PI + fEllipseStep, bDirection);
							pSecond = funArray.GetEllipsePoint(fMajor, fMinor, angle, pt2, endAngle * 180 / PI, bDirection);
						}

						startX = pFirst.x;
						startY = pFirst.y;

						nextX = pSecond.x;
						nextY = pSecond.y;
						c = funArray.GetC(2, startX, startY, nextX, nextY);
						g_EllipseArray.Add(CInfomation(startX, startY, c));

						if(!bIsWithC)
						{
							if(iGraphNum > 0 && !lastIsPoint)
							{
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iDownSpeed, lastEndX, lastEndY,0.0, RadianToAngle(lastEndC));
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iUpSpeed, lastEndX, lastEndY, g_fSafeHeight, RadianToAngle(lastEndC));
							}
						}
						else
						{
							if(iGraphNum > 0 && !lastIsPoint)
							{
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iDownSpeed, lastEndX, lastEndY, 0.0);
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iUpSpeed, lastEndX, lastEndY, g_fSafeHeight);
							}
						}
						//fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iG00Speed,g_EllipseArray[0].x,g_EllipseArray[0].y,g_fSafeHeight,g_EllipseArray[0].c);
						//fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iDownSpeed,g_EllipseArray[0].x,g_EllipseArray[0].y,0.0,g_EllipseArray[0].c);

						if(fabs(fDelte) < 1e-6)//闭合椭圆
						{
							j = 1;
							nCount1 = 360 / fEllipseStep;

							if(!bSetNewStepFlags)
							{
								if(fMinor <= 30)
									fCoincide_Len = g_fCoincide_S;
								else if(fMinor <= 50)
									fCoincide_Len = g_fCoincide_M;
								else if(fMinor <= 100)
									fCoincide_Len = g_fCoincide_L;
								else
									fCoincide_Len = g_fCoincide_XL;
							}
							else
								fCoincide_Len = fNewStep;

							lastPoint = pFirst;
							if(!bDirection)//逆时针
							{
								for(int n = 1; n < nCount1; n++)
								{
									pNext = funArray.GetEllipsePoint(fMajor, fMinor, angle,pt2, n * fEllipseStep, bDirection);
									x = pNext.x;
									y = pNext.y;
									c = funArray.GetC(2, g_EllipseArray[j - 1].x, g_EllipseArray[j - 1].y, x, y);

									if(fEllipse2PointLen <= g_fCoincide_Len)
									{
										fEllipse2PointLen += funArray.GetDistance(lastPoint, pNext);
										nCount2++;
										lastPoint = pNext;
									}
									g_EllipseArray.Add(CInfomation(x, y, c));
									j++;
								}
							}
							else//顺时针
							{
								for(int n = nCount1 - 1;n > 0; n--)
								{
									pNext = funArray.GetEllipsePoint(fMajor, fMinor, angle, pt2, n * fEllipseStep, bDirection);
									x = pNext.x;
									y = pNext.y;
									c = funArray.GetC(2, g_EllipseArray[j - 1].x, g_EllipseArray[j - 1].y, x, y);
									if(fEllipse2PointLen <= g_fCoincide_Len)
									{
										fEllipse2PointLen += funArray.GetDistance(lastPoint, pNext);
										nCount2++;
										lastPoint = pNext;
									}

									g_EllipseArray.Add(CInfomation(x, y, c));
									j++;
									//fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",iFeedSpeed,g_EllipseArray[j - 1].x,g_EllipseArray[j - 1].y,z,g_EllipseArray[j - 1].c);
								}
							}

							nCount = nCount1 + nCount2 * 2;
							//g_EllipseArray[0].c = funArray.GetC(6,g_EllipseArray[nCount1 - 1].x,g_EllipseArray[nCount1 - 1].y,g_EllipseArray[0].x,g_EllipseArray[0].y);

							if(!bIsWithC)
							{
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iG00Speed, g_EllipseArray[0].x, g_EllipseArray[0].y, g_fSafeHeight, RadianToAngle(g_EllipseArray[0].c));
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iDownSpeed, g_EllipseArray[0].x, g_EllipseArray[0].y, 0.0, RadianToAngle(g_EllipseArray[0].c));
							}
							else
							{
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iG00Speed, g_EllipseArray[0].x, g_EllipseArray[0].y, g_fSafeHeight);
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iDownSpeed, g_EllipseArray[0].x, g_EllipseArray[0].y, 0.0);
							}

							size = g_EllipseArray.GetSize();
							upDownCount = nCount2;

							for(j = 1;j < nCount;++j)
							{
								k = j % size;
								//h = j % nCount1;

								z = funArray.SetEllipseDepth(nCount, upDownCount, j);

								c = g_EllipseArray[k].c;
								if(!bIsWithC)
								{
									if(j == 1)
										fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iDownSpeed, g_EllipseArray[k].x, g_EllipseArray[k].y, z, RadianToAngle(c));
									else
										fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", iFeedSpeed, g_EllipseArray[k].x, g_EllipseArray[k].y, z, RadianToAngle(c));
								}
								else
								{
									if(j == 1)
										fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iDownSpeed, g_EllipseArray[k].x, g_EllipseArray[k].y, z);
									else
										fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", iFeedSpeed, g_EllipseArray[k].x, g_EllipseArray[k].y, z);
								}


							}

							lastEndX = g_EllipseArray[(nCount - nCount1) % size].x;
							lastEndY = g_EllipseArray[(nCount - nCount1) % size].y;
							lastEndC = g_EllipseArray[(nCount - nCount1) % size].c;
						}

						else//椭圆弧
						{
							j = 1;
							nCount1 = (endAngle - startAngle) * 180 / PI / fEllipseStep;

							if(!bDirection)//逆时针
							{
								for(int n = 1; n < nCount1; n++)
								{
									pNext = funArray.GetEllipsePoint(fMajor, fMinor, angle, pt2, startAngle * 180 / PI + n * fEllipseStep, bDirection);
									x = pNext.x;
									y = pNext.y;
									c = funArray.GetC(2, g_EllipseArray[j - 1].x, g_EllipseArray[j - 1].y, x, y);
									g_EllipseArray.Add(CInfomation(x, y, c));
									j++;
								}
							} 
							else//顺时针
							{
								for(int n = 1; n < nCount1; n++)
								{
									pNext = funArray.GetEllipsePoint(fMajor, fMinor, angle, pt2, endAngle * 180 / PI - n * fEllipseStep, bDirection);
									x = pNext.x;
									y = pNext.y;
									c = funArray.GetC(2, g_EllipseArray[j - 1].x, g_EllipseArray[j - 1].y, x, y);
									g_EllipseArray.Add(CInfomation(x, y, c));
									j++;
								}
							}

							//g_EllipseArray[0].c = g_EllipseArray[nCount1].c;
							if(!bIsWithC)
							{
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iG00Speed, g_EllipseArray[0].x, g_EllipseArray[0].y, g_fSafeHeight, RadianToAngle(g_EllipseArray[0].c));
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iDownSpeed, g_EllipseArray[0].x, g_EllipseArray[0].y, 0.0, RadianToAngle(g_EllipseArray[0].c));
							}
							else
							{
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iG00Speed, g_EllipseArray[0].x, g_EllipseArray[0].y, g_fSafeHeight);
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iDownSpeed, g_EllipseArray[0].x, g_EllipseArray[0].y, 0.0);
							}

							nCount = (endAngle - startAngle) * 180 / PI / fEllipseStep;
							upDownCount = nCount * fAsymptotic;
							upDownStep = fabs(g_fDepth) / upDownCount;

							for(j = 0; j < nCount; ++j)
							{
								if(clrREF == RGB(255, 0, 0))
									z = funArray.SetDepth1(fDepth, nCount, j, nCount - j);
								else if(clrREF == RGB(0, 255, 0))
									z = funArray.SetDownDepth(fDepth, j);
								else if(clrREF == RGB(0, 0, 255))
									z = funArray.SetUpDepth(fDepth, nCount,j);
								else
									z = fDepth;

								c = g_EllipseArray[j].c;
								if(!bIsWithC)
								{
									if(j == 0)
										fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iDownSpeed, g_EllipseArray[j].x, g_EllipseArray[j].y, z, RadianToAngle(c));
									else
										fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", iFeedSpeed, g_EllipseArray[j].x, g_EllipseArray[j].y, z, RadianToAngle(c));
								}
								else
								{
									if(j == 0)
										fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iDownSpeed, g_EllipseArray[j].x, g_EllipseArray[j].y, z);
									else
										fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", iFeedSpeed, g_EllipseArray[j].x, g_EllipseArray[j].y, z);
								}
							}

							lastEndX = g_EllipseArray[j - 1].x;
							lastEndY = g_EllipseArray[j - 1].y;
							lastEndC = g_EllipseArray[j - 1].c;

						}
						basePoint_X = lastEndX;
						basePoint_Y = lastEndY;
						break;

						//无效代码
						//				case 7:
						//					if(bSetFeedSpeed)
						//						iFeedSpeed = ((CGraph*)g_SaveArray.GetAt(index))->m_iPerFeedSpeed;
						//
						//					fLength = 0;
						//					fPolySumLen = 0.0;
						//					nPolyVertexCount = 0;
						//					fAsymptotic = g_fAsymptotic;
						//					iNum = 0;
						//					bTransition = TRUE;
						//					gClr = ((CGraph*)g_SaveArray.GetAt(index))->clrREF;
						//					fDepth = g_fDepth;
						//
						//					for(int ax = 0;ax < EachPolyLine.size();++ax)
						//					{
						//						EachPolyLine[ax].clear();
						//					}
						//					EachPolyLine.clear();
						//
						//					m_PolyLineArray.RemoveAll();
						//					nPtNumber = ((CGraph*)g_SaveArray.GetAt(index))->m_iVertexNum;
						//					ptMember.Copy(((CGraph*)g_SaveArray.GetAt(index))->m_PeakGroup);
						//					uFlags = ((CGraph*)g_SaveArray.GetAt(index))->m_bFlags;
						//
						//					pfPoint = new double[3 * nPtNumber];
						//					for(j = 0;j < nPtNumber;++j)
						//					{
						//						pfPoint[j * 3] = ptMember[j].x;
						//						pfPoint[j * 3 + 1] = ptMember[j].y;
						//						pfPoint[j * 3 + 2] = ptMember[j].con;
						//					}
						//
						//					//获取第一个图形起点的C角和最后一个图形终点的C角
						//
						//					//求第一点的C角
						//					startX = pfPoint[0],startY = pfPoint[1];
						//					endX = pfPoint[3],endY = pfPoint[4];
						//					con = pfPoint[2];
						//					//起点C角
						//					if(con == 0.0)
						//					{
						//						fFirstC = funArray.GetC(2, startX, startY, endX, endY);
						//					}
						//					else
						//					{
						//						angle = 4 * atan(fabs(con));
						//						r = funArray.GetDistance(startX,startY,endX,endY) / (2 * sin(angle / 2));
						//
						//						if(con > 0)
						//							bDirection = FALSE;
						//						else
						//							bDirection = TRUE;
						//
						//						centerPoint = funArray.GetCenterPoint(startX,startY,endX,endY,r,angle,bDirection);
						//						fFirstC = funArray.GetC(4, centerPoint.x, centerPoint.y, startX, startY, bDirection);
						//					}
						//
						//					if(uFlags == 1)
						//					{
						//						//终点C角
						//						int nu = nPtNumber - 1;
						//						startX = pfPoint[nu * 3],startY = pfPoint[nu * 3 + 1];
						//						con = pfPoint[nu * 3 + 2];
						//						endX = pfPoint[0],endY = pfPoint[1];
						//						if(con == 0.0)
						//						{
						//							fLastC = funArray.GetC(2,startX,startY,endX,endY);
						//						}
						//						else
						//						{
						//							angle = 4 * atan(fabs(con));
						//							r = funArray.GetDistance(startX,startY,endX,endY) / (2 * sin(angle / 2));
						//
						//							if(con > 0)
						//								bDirection = FALSE;						
						//							else
						//								bDirection = TRUE;	
						//
						//							centerPoint = funArray.GetCenterPoint(startX,startY,endX,endY,r,angle,bDirection);
						//							fLastC = funArray.GetC(4, centerPoint.x, centerPoint.y, endX, endY, bDirection);
						//						}
						//					}
						//
						//					//把所有数据保存起来，便于输出
						//					for(j = 0;j < nPtNumber - 1;++j)
						//					{
						//						EachEdgeOfPolyLine.clear();
						//
						//						startX = pfPoint[j * 3], startY = pfPoint[j * 3 + 1];
						//						endX = pfPoint[(j + 1) * 3], endY = pfPoint[(j + 1) * 3 + 1];
						//
						//						if(pfPoint[j * 3 + 2] == 0.0)
						//						{
						//							fLength = funArray.GetDistance(startX, startY, endX, endY);
						//
						//							if(!bSetNewStepFlags)
						//							{
						//								if(fLength < 50)
						//									LineStep = g_fLineStep_S;
						//								else
						//									LineStep = g_fLineStep_L;
						//							}
						//							else
						//								LineStep = fNewStep;
						//
						//							nCount1 = fLength / LineStep;
						//							LineStep = funArray.GetNewStep(fLength, LineStep);
						//
						//							nPolyVertexCount += nCount1;
						//							fPolySumLen += fLength;
						//
						//							//----------------------------------------------------------------------------------
						//							fStepX = (endX - startX) / nCount1;
						//							fStepY = (endY - startY) / nCount1;
						//							lineC = funArray.GetC(2, startX, startY, endX, endY);
						//
						//							for(int ax = 0;ax <= nCount1;++ax)
						//							{
						//								ci.x = startX + fStepX * ax;
						//								ci.y = startY + fStepY * ax;
						//								ci.c = lineC;
						//								EachEdgeOfPolyLine.push_back(ci);
						//							}
						//							EachPolyLine.push_back(EachEdgeOfPolyLine);
						//						}
						//						else
						//						{
						//							g_PolyArray.RemoveAll();
						//
						//							angle = 4 * atan(fabs(pfPoint[j * 3 + 2]));
						//							fOR = funArray.GetDistance(startX,startY,endX,endY) / (2 * sin(angle / 2));
						//
						//							fLength = fOR * angle;
						//
						//							if(!bSetNewStepFlags)
						//							{
						//								if (fOR <= 30)
						//									fCurveStep_Len = g_fCurveStep_S;
						//								else if (fOR <= 50)
						//									fCurveStep_Len = g_fCurveStep_M;
						//								else if(fOR <= 100)
						//									fCurveStep_Len = g_fCurveStep_L;
						//								else if(fOR > 100)
						//									fCurveStep_Len = g_fCurveStep_XL;
						//							}
						//							else
						//								fCurveStep_Len = fNewStep;
						//
						//							nCount1 = fLength / fCurveStep_Len;
						//							fCurveStep_Len = funArray.GetNewStep(fLength,fCurveStep_Len);
						//
						//							if(pfPoint[j * 3 + 2] > 0)
						//								bDirection = FALSE;						
						//							else
						//								bDirection = TRUE;
						//
						//							nPolyVertexCount += nCount1;
						//							fPolySumLen += fLength;
						//							//------------------------------------------------------------------------------------
						//							fUnit = fCurveStep_Len / fOR;
						//
						//							centerPoint = funArray.GetCenterPoint(startX,startY,endX,endY,fOR,angle,bDirection);
						//							c = funArray.GetC(4, centerPoint.x, centerPoint.y, startX, startY, bDirection);
						//
						//							ci.x = startX;
						//							ci.y = startY;
						//							ci.c = c;
						//							g_PolyArray.Add(CInfomation(startX, startY, c));
						//							EachEdgeOfPolyLine.push_back(ci);
						//
						//							for(int ax = 1; ax < nCount1; ++ax)
						//							{
						//								funArray.GetElement(7, g_PolyArray[ax - 1].x, g_PolyArray[ax - 1].y, g_PolyArray[ax - 1].c, 
						//									centerPoint.x, centerPoint.y, fUnit, ax, bDirection);
						//
						//								ci.x = g_PolyArray[ax].x;
						//								ci.y = g_PolyArray[ax].y;
						//								ci.c = g_PolyArray[ax].c;
						//								EachEdgeOfPolyLine.push_back(ci);
						//							}
						//							c = funArray.GetC(4, centerPoint.x, centerPoint.y, endX, endY, bDirection);
						//							ci.x = endX;
						//							ci.y = endY;
						//							ci.c = c;
						//							EachEdgeOfPolyLine.push_back(ci);
						//
						//							EachPolyLine.push_back(EachEdgeOfPolyLine);
						//						}
						//					}
						//
						//					if(uFlags)
						//					{
						//						EachEdgeOfPolyLine.clear();
						//						startX = pfPoint[(nPtNumber - 1) * 3],startY = pfPoint[(nPtNumber - 1) * 3 + 1];
						//						endX = pfPoint[0],endY = pfPoint[1];
						//
						//						if(pfPoint[(nPtNumber - 1) * 3 + 2] == 0.0)
						//						{
						//							fLength = funArray.GetDistance(startX,startY,endX,endY);
						//
						//							if(!bSetNewStepFlags)
						//							{
						//								if(fLength < 50)
						//									LineStep = g_fLineStep_S;
						//								else
						//									LineStep = g_fLineStep_L;
						//							}
						//							else
						//								LineStep = fNewStep;
						//
						//							nCount1 = fLength / LineStep;
						//
						//							nPolyVertexCount += nCount1;
						//							fPolySumLen += fLength;
						//							//---------------------------------------------------------------------------
						//							fStepX = (endX - startX) / nCount1;
						//							fStepY = (endY - startY) / nCount1;
						//							lineC = funArray.GetC(2,startX,startY,endX,endY);
						//
						//							for(int ax = 0;ax <= nCount1;++ax)
						//							{
						//								ci.x = startX + fStepX * ax;
						//								ci.y = startY + fStepY * ax;
						//								ci.c = lineC;
						//								EachEdgeOfPolyLine.push_back(ci);
						//							}
						//							EachPolyLine.push_back(EachEdgeOfPolyLine);
						//
						//						}
						//						else
						//						{
						//							g_PolyArray.RemoveAll();
						//
						//							angle = 4 * atan(fabs(pfPoint[j * 3 + 2]));
						//							fOR = funArray.GetDistance(startX,startY,endX,endY) / (2 * sin(angle / 2));
						//
						//							fLength = fOR * angle;
						//
						//							if(!bSetNewStepFlags)
						//							{
						//								if (fOR <= 30)
						//									fCurveStep_Len = g_fCurveStep_S;
						//								else if (fOR <= 50)
						//									fCurveStep_Len = g_fCurveStep_M;
						//								else if(fOR <= 100)
						//									fCurveStep_Len = g_fCurveStep_L;
						//								else if(fOR > 100)
						//									fCurveStep_Len = g_fCurveStep_XL;
						//							}
						//							else
						//								fCurveStep_Len = fNewStep;
						//
						//							nCount1 = fLength / fCurveStep_Len;
						//							fCurveStep_Len = funArray.GetNewStep(fLength,fCurveStep_Len);
						//
						//							if(pfPoint[j * 3 + 2] > 0)
						//								bDirection = FALSE;						
						//							else
						//								bDirection = TRUE;
						//
						//							nPolyVertexCount += nCount1;
						//							fPolySumLen += fLength;
						//							//------------------------------------------------------------------------------------
						//							fUnit = fCurveStep_Len / fOR;
						//
						//							centerPoint = funArray.GetCenterPoint(startX,startY,endX,endY,fOR,angle,bDirection);
						//							c = funArray.GetC(4, centerPoint.x, centerPoint.y, startX, startY, bDirection);
						//
						//							ci.x = startX;
						//							ci.y = startY;
						//							ci.c = c;
						//							g_PolyArray.Add(CInfomation(startX,startY,c));
						//							EachEdgeOfPolyLine.push_back(ci);
						//
						//							for(int ax = 1;ax < nCount1;++ax)
						//							{
						//								funArray.GetElement(7,g_PolyArray[ax - 1].x,g_PolyArray[ax - 1].y,g_PolyArray[ax - 1].c,
						//									centerPoint.x,centerPoint.y,fUnit,ax,bDirection);
						//
						//								ci.x = g_PolyArray[ax].x;
						//								ci.y = g_PolyArray[ax].y;
						//								ci.c = g_PolyArray[ax].c;
						//								EachEdgeOfPolyLine.push_back(ci);
						//							}
						//
						//							c = funArray.GetC(4, centerPoint.x, centerPoint.y, endX, endY, bDirection);
						//							ci.x = endX;
						//							ci.y = endY;
						//							ci.c = c;
						//							EachEdgeOfPolyLine.push_back(ci);
						//							EachPolyLine.push_back(EachEdgeOfPolyLine);
						//						}
						//					}
						//
						//					//***********************************************************************
						//					gbTransition = FALSE;
						//					perbTransition = FALSE;
						//
						//					if(uFlags == 1)			
						//					{
						//						if(fabs(fFirstC - fLastC) < g_fDeltaC1 || fabs(fFirstC - fLastC) > 2 * PI - g_fDeltaC1)
						//						{
						//							//gbTransition:全局过渡标识符，即多段线的起点和终点是过渡的
						//							gbTransition = TRUE;
						//						}
						//					}
						//
						//					startX = pfPoint[0],startY = pfPoint[1];
						//
						//					nPolyVertexCount = 0;
						//					for(size_t ic = 0;ic < EachPolyLine.size();++ic)
						//						nPolyVertexCount += EachPolyLine[ic].size();
						//
						//					nPolySectionCount = nPolyVertexCount - (nPtNumber -= uFlags ? 0 : 1);
						//
						//					//------------------开始输出路径--------------------
						//					if(!bIsWithC)
						//					{
						//						if(iGraphNum > 1 && ((fabs(startX - lastEndX) > 1e-3 || fabs(startY - lastEndY) > 1e-3) 
						//							|| (fabs(fFirstC - lastEndC) > g_fDeltaC1 || fabs(fFirstC - lastEndC) < 2 * PI - g_fDeltaC1)))
						//						{
						//							fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iUpSpeed,lastEndX,lastEndY,0.0,RadianToAngle(lastEndC));
						//							fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iUpSpeed,lastEndX,lastEndY,g_fSafeHeight,RadianToAngle(lastEndC));
						//						}
						//						fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iG00Speed, (EachPolyLine.at(0)).at(0).x, (EachPolyLine.at(0)).at(0).y, g_fSafeHeight, RadianToAngle((EachPolyLine.at(0)).at(0).c));
						//						fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iDownSpeed, (EachPolyLine.at(0)).at(0).x, (EachPolyLine.at(0)).at(0).y, 0.0, RadianToAngle((EachPolyLine.at(0)).at(0).c));
						//					}
						//					else
						//					{
						//						if(iGraphNum > 1 && ((fabs(startX - lastEndX) > 1e-3 || fabs(startY - lastEndY) > 1e-3) 
						//							|| (fabs(fFirstC - lastEndC) > g_fDeltaC1 || fabs(fFirstC - lastEndC) < 2 * PI - g_fDeltaC1)))
						//						{
						//							fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iUpSpeed,lastEndX,lastEndY,0.0);
						//							fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iUpSpeed,lastEndX,lastEndY,g_fSafeHeight);
						//						}
						//						fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iG00Speed, (EachPolyLine.at(0)).at(0).x, (EachPolyLine.at(0)).at(0).y, g_fSafeHeight);
						//						fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iDownSpeed, (EachPolyLine.at(0)).at(0).x, (EachPolyLine.at(0)).at(0).y, 0.0);
						//					}
						//
						//					//Start Date:6月...日--------------------------------------------
						//					int EdgeIndex;
						//
						//					if(!gbTransition)//gbTransition=FALSE
						//					{
						//						if(clrREF == RGB(0, 0, 0))
						//						{
						//							for (EdgeIndex = 0; EdgeIndex < nPtNumber - 1; ++EdgeIndex)
						//							{
						//								fDepth = g_fDepth;
						//								EachCounts = EachPolyLine.at(EdgeIndex).size();
						//
						//								if(EdgeIndex == 0)
						//								{
						//									for(std::size_t ss = 0; ss < EachCounts; ++ss)
						//									{
						//
						//										lastX = EachPolyLine.at(EdgeIndex).at(ss).x;
						//										lastY = EachPolyLine.at(EdgeIndex).at(ss).y;
						//										lastC = EachPolyLine.at(EdgeIndex).at(ss).c;
						//
						//										if(!bIsWithC)
						//										{
						//											if(ss == 0)
						//												fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iDownSpeed, lastX, lastY, g_fDepth, RadianToAngle(lastC));
						//											else
						//												fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",iFeedSpeed, lastX, lastY, g_fDepth, RadianToAngle(lastC));
						//										}
						//										else
						//										{
						//											if(ss == 0)
						//												fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iDownSpeed, lastX, lastY, g_fDepth);
						//											else
						//												fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",iFeedSpeed, lastX, lastY, g_fDepth);
						//										}
						//									}
						//								}
						//								else
						//								{
						//									startX = EachPolyLine.at(EdgeIndex).at(0).x;
						//									startY = EachPolyLine.at(EdgeIndex).at(0).y;
						//									startC = EachPolyLine.at(EdgeIndex).at(0).c;
						//
						//									if(!bIsWithC)
						//									{
						//										if(fabs(lastC - startC) > g_fDeltaC1 || fabs(lastC - startC) > 2 * PI - g_fDeltaC1)//需要抬刀
						//										{
						//											bTransition = FALSE;
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iUpSpeed, lastX, lastY, 0.0, RadianToAngle(lastC));
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iUpSpeed, lastX, lastY, g_fSafeHeight, RadianToAngle(lastC));
						//
						//											//下刀
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iDownSpeed, startX, startY, g_fSafeHeight, RadianToAngle(startC));
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iDownSpeed, startX, startY, 0.0, RadianToAngle(startC));
						//										}
						//									}
						//									else
						//									{
						//										if(fabs(lastC - startC) > g_fDeltaC1 || fabs(lastC - startC) > 2 * PI - g_fDeltaC1)//需要抬刀
						//										{
						//											bTransition = FALSE;
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iUpSpeed, lastX, lastY, 0.0);
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iUpSpeed, lastX, lastY, g_fSafeHeight);
						//
						//											//下刀
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iDownSpeed, startX, startY, g_fSafeHeight);
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iDownSpeed, startX, startY, 0.0);
						//										}
						//									}
						//
						//									for(std::size_t ss = bTransition ? 1 : 0; ss < EachCounts; ++ss)
						//									{
						//										lastX = EachPolyLine.at(EdgeIndex).at(ss).x;
						//										lastY = EachPolyLine.at(EdgeIndex).at(ss).y;
						//										lastC = EachPolyLine.at(EdgeIndex).at(ss).c;
						//
						//										if(!bIsWithC)
						//										{
						//											if(ss == 0)
						//												fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iDownSpeed, lastX, lastY, g_fDepth, RadianToAngle(lastC));
						//											else
						//												fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",iFeedSpeed, lastX, lastY, g_fDepth, RadianToAngle(lastC));
						//										}
						//										else
						//										{
						//											if(ss == 0)
						//												fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iDownSpeed, lastX, lastY, g_fDepth);
						//											else
						//												fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",iFeedSpeed, lastX, lastY, g_fDepth);
						//										}
						//									}
						//								}
						//								lastEndX = lastX;
						//								lastEndY = lastY;
						//								lastEndC = lastC;
						//							}
						//
						//							if(uFlags)
						//							{
						//								EachCounts = EachPolyLine.at(EdgeIndex).size();
						//
						//								startX = EachPolyLine.at(EdgeIndex).at(0).x;
						//								startY = EachPolyLine.at(EdgeIndex).at(0).y;
						//								startC = EachPolyLine.at(EdgeIndex).at(0).c;
						//
						//								if(!bIsWithC)
						//								{
						//									if(fabs(lastC - startC) > g_fDeltaC1 || fabs(lastC - startC) > 2 * PI - g_fDeltaC1)//需要抬刀
						//									{
						//										bTransition = FALSE;
						//										fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iUpSpeed, lastX, lastY, 0.0, RadianToAngle(lastC));
						//										fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iUpSpeed, lastX, lastY, g_fSafeHeight, RadianToAngle(lastC));
						//
						//										//下刀
						//										fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iDownSpeed, startX, startY, g_fSafeHeight, RadianToAngle(startC));
						//										fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iDownSpeed, startX, startY, 0.0, RadianToAngle(startC));
						//									}
						//								}
						//								else
						//								{
						//									if(fabs(lastC - startC) > g_fDeltaC1 || fabs(lastC - startC) > 2 * PI - g_fDeltaC1)//需要抬刀
						//									{
						//										bTransition = FALSE;
						//										fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iUpSpeed, lastX, lastY, 0.0);
						//										fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iUpSpeed, lastX, lastY, g_fSafeHeight);
						//
						//										//下刀
						//										fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iDownSpeed, startX, startY, g_fSafeHeight);
						//										fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iDownSpeed, startX, startY, 0.0);
						//									}
						//								}
						//
						//								for(std::size_t ss = bTransition ? 1 : 0; ss < EachCounts; ++ss)
						//								{
						//									lastX = EachPolyLine.at(EdgeIndex).at(ss).x;
						//									lastY = EachPolyLine.at(EdgeIndex).at(ss).y;
						//									lastC = EachPolyLine.at(EdgeIndex).at(ss).c;
						//
						//									if(!bIsWithC)
						//									{
						//										if(ss == 0)
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iDownSpeed, lastX, lastY, g_fDepth, RadianToAngle(lastC));
						//										else
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",iFeedSpeed, lastX, lastY, g_fDepth, RadianToAngle(lastC));
						//									}
						//									else
						//									{
						//										if(ss == 0)
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iDownSpeed, lastX, lastY, g_fDepth);
						//										else
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",iFeedSpeed, lastX, lastY, g_fDepth);
						//									}
						//								}
						//
						//								lastEndX = lastX;
						//								lastEndY = lastY;
						//								lastEndC = lastC;
						//
						//							}
						//						}
						//						else
						//						{
						//							CurCounts = 1;
						//							upDownCount = nPolySectionCount * fAsymptotic;
						//							upDownStep = -g_fDepth / upDownCount;
						//
						//							for (EdgeIndex = 0; EdgeIndex < nPtNumber - 1; ++EdgeIndex)
						//							{
						//								EachCounts = EachPolyLine.at(EdgeIndex).size();
						//
						//								if(EdgeIndex == 0)
						//								{
						//									for(std::size_t ss = 1; ss < EachCounts; ++ss)
						//									{
						//										lastX = EachPolyLine.at(EdgeIndex).at(ss).x;
						//										lastY = EachPolyLine.at(EdgeIndex).at(ss).y;
						//										lastC = EachPolyLine.at(EdgeIndex).at(ss).c;
						//
						//										if(clrREF == RGB(255, 0, 0))
						//											z = funArray.SetPolyRedDepth(nPolySectionCount, CurCounts);
						//										else if(clrREF == RGB(0, 255, 0))
						//											z = funArray.SetPolyGreenDepth(nPolySectionCount, CurCounts);
						//										else if(clrREF == RGB(0, 0, 255))
						//											z = funArray.SetPolyBlueDepth(nPolySectionCount, CurCounts);
						//
						//										if(!bIsWithC)
						//										{
						//											if(ss == 1)
						//												fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iDownSpeed, lastX, lastY, z, RadianToAngle(lastC));
						//											else
						//												fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",iFeedSpeed, lastX, lastY, z, RadianToAngle(lastC));
						//										}
						//										else
						//										{
						//											if(ss == 1)
						//												fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iDownSpeed, lastX, lastY, z);
						//											else
						//												fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",iFeedSpeed, lastX, lastY, z);
						//										}
						//										CurCounts++;
						//									}
						//								}
						//								else
						//								{
						//									startX = EachPolyLine.at(EdgeIndex).at(0).x;
						//									startY = EachPolyLine.at(EdgeIndex).at(0).y;
						//									startC = EachPolyLine.at(EdgeIndex).at(0).c;
						//
						//									if(!bIsWithC)
						//									{
						//										if(fabs(lastC - startC) > g_fDeltaC1 || fabs(lastC - startC) > 2 * PI - g_fDeltaC1)//需要抬刀
						//										{
						//											bTransition = FALSE;
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iUpSpeed, lastX, lastY, 0.0, RadianToAngle(lastC));
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iUpSpeed, lastX, lastY, g_fSafeHeight, RadianToAngle(lastC));
						//
						//											//下刀
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iDownSpeed, startX, startY, g_fSafeHeight, RadianToAngle(startC));
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iDownSpeed, startX, startY, 0.0, RadianToAngle(startC));
						//										}
						//									}
						//									else
						//									{
						//										if(fabs(lastC - startC) > g_fDeltaC1 || fabs(lastC - startC) > 2 * PI - g_fDeltaC1)//需要抬刀
						//										{
						//											bTransition = FALSE;
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iUpSpeed, lastX, lastY, 0.0);
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iUpSpeed, lastX, lastY, g_fSafeHeight);
						//
						//											//下刀
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iDownSpeed, startX, startY, g_fSafeHeight);
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iDownSpeed, startX, startY, 0.0);
						//										}
						//									}
						//
						//									for(std::size_t ss = bTransition ? 1 : 0; ss < EachCounts; ++ss)
						//									{
						//										lastX = EachPolyLine.at(EdgeIndex).at(ss).x;
						//										lastY = EachPolyLine.at(EdgeIndex).at(ss).y;
						//										lastC = EachPolyLine.at(EdgeIndex).at(ss).c;
						//
						//										if(ss != 0)
						//										{
						//											if(clrREF == RGB(255, 0, 0))
						//												z = funArray.SetPolyRedDepth(nPolySectionCount, CurCounts);
						//											else if(clrREF == RGB(0, 255, 0))
						//												z = funArray.SetPolyGreenDepth(nPolySectionCount, CurCounts);
						//											else if(clrREF == RGB(0, 0, 255))
						//												z = funArray.SetPolyBlueDepth(nPolySectionCount, CurCounts);
						//										}
						//										else
						//											CurCounts--;//防止下面又++多出一个
						//
						//										if(!bIsWithC)
						//										{
						//											if(ss == 0)
						//												fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iDownSpeed, lastX, lastY, z, RadianToAngle(lastC));
						//											else
						//												fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",iFeedSpeed, lastX, lastY, z, RadianToAngle(lastC));
						//										}
						//										else
						//										{
						//											if(ss == 0)
						//												fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iDownSpeed, lastX, lastY, z);
						//											else
						//												fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",iFeedSpeed, lastX, lastY, z);
						//										}
						//										CurCounts++;
						//									}
						//								}
						//								lastEndX = lastX;
						//								lastEndY = lastY;
						//								lastEndC = lastC;
						//							}
						//							if(uFlags)
						//							{
						//								EachCounts = EachPolyLine.at(EdgeIndex).size();
						//
						//								startX = EachPolyLine.at(EdgeIndex).at(0).x;
						//								startY = EachPolyLine.at(EdgeIndex).at(0).y;
						//								startC = EachPolyLine.at(EdgeIndex).at(0).c;
						//
						//								if(!bIsWithC)
						//								{
						//									if(fabs(lastC - startC) > g_fDeltaC1 || fabs(lastC - startC) > 2 * PI - g_fDeltaC1)//需要抬刀
						//									{
						//										bTransition = FALSE;
						//										fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iUpSpeed, lastX, lastY, 0.0, RadianToAngle(lastC));
						//										fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iUpSpeed, lastX, lastY, g_fSafeHeight, RadianToAngle(lastC));
						//
						//										//下刀
						//										fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iDownSpeed, startX, startY, g_fSafeHeight, RadianToAngle(startC));
						//										fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iDownSpeed, startX, startY, 0.0, RadianToAngle(startC));
						//									}
						//								}
						//								else
						//								{
						//									if(fabs(lastC - startC) > g_fDeltaC1 || fabs(lastC - startC) > 2 * PI - g_fDeltaC1)//需要抬刀
						//									{
						//										bTransition = FALSE;
						//										fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iUpSpeed, lastX, lastY, 0.0);
						//										fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iUpSpeed, lastX, lastY, g_fSafeHeight);
						//
						//										//下刀
						//										fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iDownSpeed, startX, startY, g_fSafeHeight);
						//										fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iDownSpeed, startX, startY, 0.0);
						//									}
						//								}
						//
						//								for(std::size_t ss = bTransition ? 1 : 0; ss < EachCounts; ++ss)
						//								{
						//									lastX = EachPolyLine.at(EdgeIndex).at(ss).x;
						//									lastY = EachPolyLine.at(EdgeIndex).at(ss).y;
						//									lastC = EachPolyLine.at(EdgeIndex).at(ss).c;
						//
						//									if(ss != 0)
						//									{
						//										if(clrREF == RGB(255, 0, 0))
						//											z = funArray.SetPolyRedDepth(nPolySectionCount, CurCounts);
						//										else if(clrREF == RGB(0, 255, 0))
						//											z = funArray.SetPolyGreenDepth(nPolySectionCount, CurCounts);
						//										else if(clrREF == RGB(0, 0, 255))
						//											z = funArray.SetPolyBlueDepth(nPolySectionCount, CurCounts);
						//									}
						//									else
						//										CurCounts--;//防止下面又++多出一个
						//
						//									if(!bIsWithC)
						//									{
						//										if(ss == 0)
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iDownSpeed, lastX, lastY, z, RadianToAngle(lastC));
						//										else
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",iFeedSpeed, lastX, lastY, z, RadianToAngle(lastC));
						//									}
						//									else
						//									{
						//										if(ss == 0)
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iDownSpeed, lastX, lastY, z);
						//										else
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",iFeedSpeed, lastX, lastY, z);
						//									}
						//									CurCounts++;
						//								}
						//
						//								lastEndX = lastX;
						//								lastEndY = lastY;
						//								lastEndC = lastC;
						//							}
						//						}
						//					}
						//					else//gbTransition = TRUE，过渡
						//					{
						//						//以绿色的行进格式完成重合长度前的步骤
						//						CurCounts = 1;
						//						upDownCount = nPolySectionCount * fAsymptotic;
						//						upDownStep = -g_fDepth / upDownCount;
						//
						//						for (EdgeIndex = 0; EdgeIndex < nPtNumber - 1; ++EdgeIndex)
						//						{
						//							EachCounts = EachPolyLine.at(EdgeIndex).size();
						//
						//							if(EdgeIndex == 0)
						//							{
						//								for(std::size_t ss = 1; ss < EachCounts; ++ss)
						//								{
						//									lastX = EachPolyLine.at(EdgeIndex).at(ss).x;
						//									lastY = EachPolyLine.at(EdgeIndex).at(ss).y;
						//									lastC = EachPolyLine.at(EdgeIndex).at(ss).c;
						//
						//									z = funArray.SetPolyGreenDepth(nPolySectionCount, CurCounts);
						//
						//									if(!bIsWithC)
						//									{
						//										if(ss == 1)
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iDownSpeed, lastX, lastY, z, RadianToAngle(lastC));
						//										else
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",iFeedSpeed, lastX, lastY, z, RadianToAngle(lastC));
						//									}
						//									else
						//									{
						//										if(ss == 1)
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iDownSpeed, lastX, lastY, z);
						//										else
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",iFeedSpeed, lastX, lastY, z);
						//									}
						//									CurCounts++;
						//								}
						//							}
						//							else
						//							{
						//								startX = EachPolyLine.at(EdgeIndex).at(0).x;
						//								startY = EachPolyLine.at(EdgeIndex).at(0).y;
						//								startC = EachPolyLine.at(EdgeIndex).at(0).c;
						//
						//								if(!bIsWithC)
						//								{
						//									if(fabs(lastC - startC) > g_fDeltaC1 || fabs(lastC - startC) > 2 * PI - g_fDeltaC1)//需要抬刀
						//									{
						//										bTransition = FALSE;
						//										fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iUpSpeed, lastX, lastY, 0.0, RadianToAngle(lastC));
						//										fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iUpSpeed, lastX, lastY, g_fSafeHeight, RadianToAngle(lastC));
						//
						//										//下刀
						//										fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iDownSpeed, startX, startY, g_fSafeHeight, RadianToAngle(startC));
						//										fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iDownSpeed, startX, startY, 0.0, RadianToAngle(startC));
						//									}
						//								}
						//								else
						//								{
						//									if(fabs(lastC - startC) > g_fDeltaC1 || fabs(lastC - startC) > 2 * PI - g_fDeltaC1)//需要抬刀
						//									{
						//										bTransition = FALSE;
						//										fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iUpSpeed, lastX, lastY, 0.0);
						//										fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iUpSpeed, lastX, lastY, g_fSafeHeight);
						//
						//										//下刀
						//										fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iDownSpeed, startX, startY, g_fSafeHeight);
						//										fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iDownSpeed, startX, startY, 0.0);
						//									}
						//								}
						//
						//								for(std::size_t ss = bTransition ? 1 : 0; ss < EachCounts; ++ss)
						//								{
						//									lastX = EachPolyLine.at(EdgeIndex).at(ss).x;
						//									lastY = EachPolyLine.at(EdgeIndex).at(ss).y;
						//									lastC = EachPolyLine.at(EdgeIndex).at(ss).c;
						//
						//									if(ss != 0)
						//										z = funArray.SetPolyGreenDepth(nPolySectionCount, CurCounts);
						//									else
						//										CurCounts--;//防止下面又++多出一个
						//
						//									if(!bIsWithC)
						//									{
						//										if(ss == 0)
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iDownSpeed, lastX, lastY, z, RadianToAngle(lastC));
						//										else
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",iFeedSpeed, lastX, lastY, z, RadianToAngle(lastC));
						//									}
						//									else
						//									{
						//										if(ss == 0)
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iDownSpeed, lastX, lastY, z);
						//										else
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",iFeedSpeed, lastX, lastY, z);
						//									}
						//									CurCounts++;
						//								}
						//							}
						//							lastEndX = lastX;
						//							lastEndY = lastY;
						//							lastEndC = lastC;
						//						}
						//						if(uFlags)
						//						{
						//							EachCounts = EachPolyLine.at(EdgeIndex).size();
						//
						//							startX = EachPolyLine.at(EdgeIndex).at(0).x;
						//							startY = EachPolyLine.at(EdgeIndex).at(0).y;
						//							startC = EachPolyLine.at(EdgeIndex).at(0).c;
						//
						//							if(!bIsWithC)
						//							{
						//								if(fabs(lastC - startC) > g_fDeltaC1 || fabs(lastC - startC) > 2 * PI - g_fDeltaC1)//需要抬刀
						//								{
						//									bTransition = FALSE;
						//									fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iUpSpeed, lastX, lastY, 0.0, RadianToAngle(lastC));
						//									fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iUpSpeed, lastX, lastY, g_fSafeHeight, RadianToAngle(lastC));
						//
						//									//下刀
						//									fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iDownSpeed, startX, startY, g_fSafeHeight, RadianToAngle(startC));
						//									fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iDownSpeed, startX, startY, 0.0, RadianToAngle(startC));
						//								}
						//							}
						//							else
						//							{
						//								if(fabs(lastC - startC) > g_fDeltaC1 || fabs(lastC - startC) > 2 * PI - g_fDeltaC1)//需要抬刀
						//								{
						//									bTransition = FALSE;
						//									fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iUpSpeed, lastX, lastY, 0.0);
						//									fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iUpSpeed, lastX, lastY, g_fSafeHeight);
						//
						//									//下刀
						//									fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iDownSpeed, startX, startY, g_fSafeHeight);
						//									fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iDownSpeed, startX, startY, 0.0);
						//								}
						//							}
						//
						//							for(std::size_t ss = bTransition ? 1 : 0; ss < EachCounts; ++ss)
						//							{
						//								lastX = EachPolyLine.at(EdgeIndex).at(ss).x;
						//								lastY = EachPolyLine.at(EdgeIndex).at(ss).y;
						//								lastC = EachPolyLine.at(EdgeIndex).at(ss).c;
						//
						//								if(ss != 0)
						//									z = funArray.SetPolyGreenDepth(nPolySectionCount, CurCounts);
						//								else
						//									CurCounts--;//防止下面又++多出一个
						//
						//								if(!bIsWithC)
						//								{
						//									if(ss == 0)
						//										fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iDownSpeed, lastX, lastY, z, RadianToAngle(lastC));
						//									else
						//										fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",iFeedSpeed, lastX, lastY, z, RadianToAngle(lastC));
						//								}
						//								else
						//								{	
						//									if(ss == 0)
						//										fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iDownSpeed, lastX, lastY, z);
						//									else
						//										fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",iFeedSpeed, lastX, lastY, z);
						//								}
						//								CurCounts++;
						//							}
						//
						//							lastEndX = lastX;
						//							lastEndY = lastY;
						//							lastEndC = lastC;
						//						}
						//
						//						//从头开始，完成重合长度的步骤
						//						CurCounts = nPolySectionCount * fAsymptotic;
						//						CurCounts++;
						//
						//						for (EdgeIndex = 0; EdgeIndex < nPtNumber - 1; ++EdgeIndex)
						//						{
						//							EachCounts = EachPolyLine.at(EdgeIndex).size();
						//
						//							if(EdgeIndex == 0)
						//							{
						//								for(std::size_t ss = 1; ss < EachCounts; ++ss)
						//								{
						//									lastX = EachPolyLine.at(EdgeIndex).at(ss).x;
						//									lastY = EachPolyLine.at(EdgeIndex).at(ss).y;
						//									lastC = EachPolyLine.at(EdgeIndex).at(ss).c;
						//
						//									z = funArray.SetPolyBlueDepth(nPolySectionCount, CurCounts);
						//
						//									if(!bIsWithC)
						//									{
						//										if(ss == 1)
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iDownSpeed, lastX, lastY, z, RadianToAngle(lastC));
						//										else
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",iFeedSpeed, lastX, lastY, z, RadianToAngle(lastC));
						//									}
						//									else
						//									{
						//										if(ss == 1)
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iDownSpeed, lastX, lastY, z);
						//										else
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",iFeedSpeed, lastX, lastY, z);
						//									}
						//									CurCounts++;
						//
						//									if(z == 0.0)
						//									{
						//										lastEndX = lastX;
						//										lastEndY = lastY;
						//										lastEndC = lastC;
						//										lastEndZ = z;
						//										goto Label1;
						//									}
						//								}
						//							}
						//							else
						//							{
						//								startX = EachPolyLine.at(EdgeIndex).at(0).x;
						//								startY = EachPolyLine.at(EdgeIndex).at(0).y;
						//								startC = EachPolyLine.at(EdgeIndex).at(0).c;
						//
						//								if(!bIsWithC)
						//								{
						//									if(fabs(lastC - startC) > g_fDeltaC1 || fabs(lastC - startC) > 2 * PI - g_fDeltaC1)//需要抬刀
						//									{
						//										bTransition = FALSE;
						//										fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iUpSpeed, lastX, lastY, 0.0, RadianToAngle(lastC));
						//										fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iUpSpeed, lastX, lastY, g_fSafeHeight, RadianToAngle(lastC));
						//
						//										//下刀
						//										fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iDownSpeed, startX, startY, g_fSafeHeight, RadianToAngle(startC));
						//										fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iDownSpeed, startX, startY, 0.0, RadianToAngle(startC));
						//									}
						//								}
						//								else
						//								{
						//									if(fabs(lastC - startC) > g_fDeltaC1 || fabs(lastC - startC) > 2 * PI - g_fDeltaC1)//需要抬刀
						//									{
						//										bTransition = FALSE;
						//										fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iUpSpeed, lastX, lastY, 0.0);
						//										fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iUpSpeed, lastX, lastY, g_fSafeHeight);
						//
						//										//下刀
						//										fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iDownSpeed, startX, startY, g_fSafeHeight);
						//										fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iDownSpeed, startX, startY, 0.0);
						//									}
						//								}
						//
						//								for(std::size_t ss = bTransition ? 1 : 0; ss < EachCounts; ++ss)
						//								{
						//									lastX = EachPolyLine.at(EdgeIndex).at(ss).x;
						//									lastY = EachPolyLine.at(EdgeIndex).at(ss).y;
						//									lastC = EachPolyLine.at(EdgeIndex).at(ss).c;
						//
						//									if(ss != 0)
						//										z = funArray.SetPolyBlueDepth(nPolySectionCount, CurCounts);
						//									else
						//										CurCounts--;//防止下面又++多出一个
						//
						//									if(!bIsWithC)
						//									{
						//										if(ss == 0)
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iDownSpeed, lastX, lastY, z, RadianToAngle(lastC));
						//										else
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",iFeedSpeed, lastX, lastY, z, RadianToAngle(lastC));
						//									}
						//									else
						//									{
						//										if(ss == 0)
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",g_iDownSpeed, lastX, lastY, z);
						//										else
						//											fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f;\n",iFeedSpeed, lastX, lastY, z);
						//									}
						//									CurCounts++;
						//
						//									if(z == 0.0)
						//									{
						//										lastEndX = lastX;
						//										lastEndY = lastY;
						//										lastEndC = lastC;
						//										lastEndZ = z;
						//										goto Label1;
						//									}
						//								}
						//							}
						//							lastEndX = lastX;
						//							lastEndY = lastY;
						//							lastEndC = lastC;
						//						}
						//					}
						//					//End Date:6月...日--------------------------------------------
						//Label1:
						//					if(pfPoint)
						//					{
						//						delete []pfPoint;
						//						pfPoint = NULL;
						//					}
						//					if(pfLength)
						//					{
						//						delete []pfLength;
						//						pfLength = NULL;
						//					}
						//
						//					basePoint_X = ptMember[nPtNumber - 1].x;
						//					basePoint_Y = ptMember[nPtNumber - 1].y;
						//					lastEndZ = z;
						//					break;

					case 8:
						bFlag = FALSE;

						if(bSetFeedSpeed)
							iFeedSpeed = ((CGraph*)g_SaveArray.GetAt(index))->m_iPerFeedSpeed;

						g_SplineArray.RemoveAll();
						NodeArray.clear();
						isFormer = FALSE;
						fSplineStep = g_fSplineStep;
						nPtNumber = ((CGraph*)g_SaveArray.GetAt(index))->m_iVertexNum;
						clrREF = ((CGraph*)g_SaveArray.GetAt(index))->clrREF;

						fDepth = g_fDepth;
						NodeArray = vector<vec2>(((CGraph*)g_SaveArray.GetAt(index))->controlNode.begin(), 
							((CGraph*)g_SaveArray.GetAt(index))->controlNode.end());

						startX = ((CGraph*)g_SaveArray.GetAt(index))->controlNode[0][0];
						startY = ((CGraph*)g_SaveArray.GetAt(index))->controlNode[0][1];
						endX = ((CGraph*)g_SaveArray.GetAt(index))->controlNode[nPtNumber - 1][0];
						endY = ((CGraph*)g_SaveArray.GetAt(index))->controlNode[nPtNumber - 1][1];
						if(startX == endX && startY == endY)
							bFlag = TRUE;

						bCurve = CBspline(NodeArray);
						bCurve.ComputeInterpolation();

						//修改最后一点的坐标
						bCurve.m_interpolationNode[MAX_LENGTH - 1][0] = ((CGraph*)g_SaveArray.GetAt(index))->controlNode[nPtNumber - 1][0];
						bCurve.m_interpolationNode[MAX_LENGTH - 1][1] = ((CGraph*)g_SaveArray.GetAt(index))->controlNode[nPtNumber - 1][1];

						for(int t = 0; t < bCurve.m_interpolationNode.size() - 1; ++t)
						{
							lastX = bCurve.m_interpolationNode[t][0], lastY = bCurve.m_interpolationNode[t][1];
							nextX = bCurve.m_interpolationNode[t + 1][0], nextY = bCurve.m_interpolationNode[t + 1][1];
							fSplineLen += funArray.GetDistance(lastX, lastY, nextX, nextY);
						}

						if(!bIsWithC)
						{
							if(iGraphNum > 0 && !lastIsPoint)
							{
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iDownSpeed, lastEndX, lastEndY, 0.0, RadianToAngle(lastEndC));
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iUpSpeed, lastEndX, lastEndY, g_fSafeHeight, RadianToAngle(lastEndC));
							}
						}
						else
						{
							if(iGraphNum > 0 && !lastIsPoint)
							{
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iDownSpeed, lastEndX, lastEndY, 0.0);
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iUpSpeed, lastEndX, lastEndY, g_fSafeHeight);
							}
						}

						if(bSetNewStepFlags)
							fSplineStep = fNewStep;
						//循环计算点与点之间的距离
						g_SplineArray.RemoveAll();
						isFormer = FALSE;

						for(j = 0;j < bCurve.m_interpolationNode.size() - 1;++j)
						{
							//如果isFormer为TRUE，长度小于样条步长时，则起点不变，终点索引往后加1；如果isFormer为FALSE，长度大于样条步长时，则起点为最后一次的终点，此时终点为起点的下一个坐标，依次循环
							if(isFormer == FALSE)
								tmpX = lastX = bCurve.m_interpolationNode[j][0], tmpY = lastY = bCurve.m_interpolationNode[j][1];
							else
							{
								tmpX = x, tmpY = y;
							}

							nextX = bCurve.m_interpolationNode[j + 1][0],nextY = bCurve.m_interpolationNode[j + 1][1];
							fLength += funArray.GetDistance(tmpX, tmpY, nextX, nextY);

							//当某两点之间的距离小于样条步长时，isFormer为FALSE，并把起点的坐标保存下来，并返回到for循环；否则isFormer为FALSE，继续向下执行
							if(fLength - fSplineStep < 0.0)
							{
								isFormer = TRUE;
								x = nextX, y = nextY;

								if(j > bCurve.m_interpolationNode.size() - 20)
									c = 10;

								continue;
							}
							else
							{
								isFormer = FALSE;
								fLength = 0.0;
							}

							//计算新的步长，使其平分
							//tempStep = funArray.GetNewStep(fLength,tempStep);
							lineC = funArray.GetC(2,lastX, lastY, nextX, nextY);

							g_SplineArray.Add(CInfomation(lastX, lastY, lineC));
							lastX = nextX, lastY = nextY;

						}
						//
						if(j == bCurve.m_interpolationNode.size() - 1)
						{
							//if(!bFlag)
							//{
							if(lastX == nextX && lastY == nextY)
							{
								lastX = bCurve.m_interpolationNode[j - 1][0];
								lastY = bCurve.m_interpolationNode[j - 1][1];
							}
							nextX = bCurve.m_interpolationNode[j][0];
							nextY = bCurve.m_interpolationNode[j][1];
							//}

							lineC = funArray.GetC(2,lastX, lastY, nextX, nextY);

							g_SplineArray.Add(CInfomation(lastX, lastY, lineC));
						}

						if(!bIsWithC)
						{
							fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iG00Speed, g_SplineArray[0].x, g_SplineArray[0].y, g_fSafeHeight, RadianToAngle(g_SplineArray[0].c));
							fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iDownSpeed, g_SplineArray[0].x, g_SplineArray[0].y, 0.0, RadianToAngle(g_SplineArray[0].c));
						}
						else
						{ 
							fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iG00Speed, g_SplineArray[0].x, g_SplineArray[0].y, g_fSafeHeight);
							fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iDownSpeed, g_SplineArray[0].x, g_SplineArray[0].y, 0.0);			
						}
						lastEndX = g_SplineArray[0].x;
						lastEndY = g_SplineArray[0].y;
						lastEndC = g_SplineArray[0].c;

						nTempCount = g_SplineArray.GetSize();

						if(fTheoryHalfLenght * 2 > fSplineLen)
						{
							fDepth = -ToolRadius + sqrt(pow(ToolRadius, 2.0) - pow(fLength / 2.0, 2.0));
						}

						if(clrREF == RGB(0, 0, 0))
						{
							clrREF = RGB(255, 0, 0);
							fAsymptotic = sqrt(pow(ToolRadius, 2.0) - pow(ToolRadius + fDepth, 2.0)) / fSplineLen;
						}

						upDownCount = nTempCount * fAsymptotic;
						upDownStep = fabs(fDepth) / upDownCount;
						upDownStep = funArray.GetNewStep(fabs(fDepth), upDownStep);//修改每步的渐近高度

						int n;
						for(n = 1, j = nTempCount - 1; n < nTempCount; ++n, --j)
						{
							bTransition = FALSE;

							if(clrREF == RGB(255, 0, 0))
								z = funArray.SetDepth1(fDepth, nTempCount, n, j);
							else if(clrREF == RGB(0, 255, 0))
								z = funArray.SetDownDepth(fDepth, n);
							else if(clrREF == RGB(0, 0, 255))
								z = funArray.SetUpDepth(fDepth, nTempCount, n);
							else
								z = fDepth;

							if(z > fDepth)
								bFirstToDepth = FALSE;

							lastX = g_SplineArray[n].x;
							lastY = g_SplineArray[n].y;
							lastC = g_SplineArray[n].c;

							if(fabs(lastC - lastEndC) < g_fDeltaC1 || fabs(lastC - lastEndC) > 2 * PI - g_fDeltaC1)
								bTransition = TRUE;

							if(!bIsWithC)
							{
								if(!bTransition)//抬刀
								{
									fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iDownSpeed, lastEndX, lastEndY, 0.0, RadianToAngle(lastEndC));
									fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iUpSpeed, lastEndX, lastEndY, g_fSafeHeight, RadianToAngle(lastEndC));

									//下刀
									fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iDownSpeed, lastEndX, lastEndY, g_fSafeHeight, RadianToAngle(lastC));
									fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iDownSpeed, lastEndX, lastEndY, z, RadianToAngle(lastC));
								}
							}
							else
							{
								if(!bTransition)//抬刀
								{
									fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iDownSpeed, lastEndX, lastEndY, 0.0);
									fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iUpSpeed, lastEndX, lastEndY, g_fSafeHeight);

									//下刀
									fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iDownSpeed, lastEndX, lastEndY, g_fSafeHeight);
									fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iDownSpeed, lastEndX, lastEndY, z);
								}
							}

							if(!bIsWithC)
							{
								if(z >= fDepth && !bFirstToDepth)
									fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iDownSpeed, lastX, lastY, z, RadianToAngle(lastC));
								else
									fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", iFeedSpeed, lastX, lastY, z, RadianToAngle(lastC));
							}
							else
							{
								if(z >= fDepth && !bFirstToDepth)
									fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iDownSpeed,g_SplineArray[n].x,g_SplineArray[n].y,z);
								else
									fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", iFeedSpeed,g_SplineArray[n].x,g_SplineArray[n].y,z);
							}

							if(z == fDepth)
								bFirstToDepth = TRUE;

							lastEndX = lastX;
							lastEndY = lastY;
							lastEndZ = z;
							lastEndC = lastC;
						}

						//lastX = bCurve.m_interpolationNode[MAX_LENGTH - 2][0];
						//lastY = bCurve.m_interpolationNode[MAX_LENGTH - 2][1];
						/*lastEndX = bCurve.m_interpolationNode[MAX_LENGTH - 1][0];
						lastEndY = bCurve.m_interpolationNode[MAX_LENGTH - 1][1];
						lastEndC = funArray.GetC(2, lastX, lastY, lastEndX, lastEndY);*/

						if(!bIsWithC)
						{
							if(z == fDepth)
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", iFeedSpeed, lastEndX, lastEndY, z, RadianToAngle(lastEndC));
						}
						else
						{
							if(z == fDepth)
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", iFeedSpeed, lastEndX, lastEndY, z);
						}

						lastEndX = bCurve.m_interpolationNode[MAX_LENGTH - 1][0];
						lastEndY = bCurve.m_interpolationNode[MAX_LENGTH - 1][1];
						lastEndC = g_SplineArray[g_SplineArray.GetSize() - 1].c;

						basePoint_X = lastEndX;
						basePoint_Y = lastEndY;					
						break;

					case 9:
						//圆点坐标
						isPoint = TRUE;
						fDepth = g_fDepth;

						if(((CGraph*)g_SaveArray.GetAt(index))->m_bSetPointDepth)
							fDepth = ((CGraph*)g_SaveArray.GetAt(index))->m_fPointDepth;

						startX = ((CGraph*)g_SaveArray.GetAt(index))->m_ptOrigin.x;
						startY = ((CGraph*)g_SaveArray.GetAt(index))->m_ptOrigin.y;

						if(!bIsWithC)
						{
							if(iGraphNum > 0 && !isPoint)
							{
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.6f C%.3f;\n", g_iDownSpeed, lastEndX, lastEndY, 0.0, RadianToAngle(lastEndC));
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.6f C%.3f;\n", g_iUpSpeed, lastEndX, lastEndY, g_fSafeHeight, RadianToAngle(lastEndC));
							}

							fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iG00Speed, startX, startY, g_fSafeHeight, 0.0);
							fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iDownSpeed, startX, startY, 0.0, 0.0);
						}
						else
						{
							if(iGraphNum > 0 && !isPoint)
							{
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iDownSpeed, lastEndX, lastEndY, 0.0);
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iUpSpeed, lastEndX, lastEndY, g_fSafeHeight);
							}

							fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iG00Speed, startX, startY, g_fSafeHeight);
							fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iDownSpeed, startX, startY, 0.0);
						}

						fPointPerDepth = g_fPointPerDepth;
						nCount1 = 360 / PERCENT;//一圈多少段
						fPercentDepth = fPointPerDepth / nCount1;//每段的深度

						nCount = fabs(fDepth) / fPercentDepth;//总共多少段数
						int adIndex;

						if(!bIsWithC)
						{
							for(adIndex = 1; adIndex <= nCount; ++adIndex)
							{
								z = -fPercentDepth * adIndex;
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.6f C%.3f;\n", g_iDownSpeed, startX, startY, z, adIndex % 360 * 1.0);
							}

							for(int cc = 0; cc < nCount1; ++cc)
							{
								z = fDepth;
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.6f C%.3f;\n", g_iDownSpeed * 2, startX, startY, z, (adIndex + cc) % 360 * 1.0);
							}

							fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.6f C%.3f;\n", g_iDownSpeed, startX, startY, 0.0, (adIndex) % 360 * 1.0);
							fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.6f C%.3f;\n", g_iUpSpeed, startX, startY, g_fSafeHeight, (adIndex) % 360 * 1.0);
						}
						else
						{
							for(adIndex = 1; adIndex <= nCount; ++adIndex)
							{
								z = -fPercentDepth * adIndex;
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.6f;\n", g_iDownSpeed, startX, startY, z);
							}

							for(int cc = 0; cc < nCount1; ++cc)
							{
								z = fDepth;
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.6f;\n", g_iDownSpeed * 2, startX, startY, z);
							}

							fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.6f;\n", g_iDownSpeed, startX, startY, 0.0);
							fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.6f;\n", g_iUpSpeed, startX, startY, g_fSafeHeight);
						}

						lastEndX = startX;
						lastEndY = startY;
						lastEndC = (adIndex + 1) % 360;
						break;

					}
				}
				else
				{
					iFeedSpeed = g_iFeedSpeed;
					EachOfPoint.clear();
					//至少有两段相切段
					iStartIndex = g_VectorGroupGraph[iGraphNum].m_GraphIndex.at(0);
					//获取第一段起点坐标
					CGraph *pFirst = (CGraph*)g_SaveArray.GetAt(iStartIndex);
					startX = pFirst->m_ptOrigin.x;
					startY = pFirst->m_ptOrigin.y;

					//获取最后一段终点坐标
					iLastIndex = g_VectorGroupGraph[iGraphNum].m_GraphIndex.at(nSize - 1);
					CGraph *pLast = (CGraph*)g_SaveArray.GetAt(iLastIndex);
					endX = pLast->m_ptEnd.x;
					endY = pLast->m_ptEnd.y;

					if (fabs(startX - endX) < 1e-2 && fabs(startY - endY) < 1e-2)
					{
						if(pFirst->m_iDrawType == 2)
						{
							fFirstC = funArray.GetC(2, pFirst->m_ptOrigin.x, pFirst->m_ptOrigin.y, pFirst->m_ptEnd.x, pFirst->m_ptEnd.y);
						}
						else if(pFirst->m_iDrawType == 4)
						{
							fOR = pFirst->m_fRadius;
							angle = pFirst->m_fAngle;
							bDirection = pFirst->m_bDirection;

							centerPoint = funArray.GetCenterPoint(pFirst->m_ptOrigin.x, pFirst->m_ptOrigin.y, pFirst->m_ptEnd.x, pFirst->m_ptEnd.y, fOR, angle, bDirection);
							fFirstC = funArray.GetC(4, centerPoint.x, centerPoint.y, pFirst->m_ptOrigin.x, pFirst->m_ptOrigin.y, bDirection);
						}
						//Start 06-23
						else if(pFirst->m_iDrawType == 8)
						{
							NodeArray.clear();
							nPtNumber = pFirst->m_iVertexNum;
							NodeArray = vector<vec2>(pFirst->controlNode.begin(),pFirst->controlNode.end());

							bCurve = CBspline(NodeArray);
							bCurve.ComputeInterpolation();

							lastX = pFirst->m_ptOrigin.x, lastY = pFirst->m_ptOrigin.y;
							nextX = bCurve.m_interpolationNode[1][0], nextY = bCurve.m_interpolationNode[1][1];
							fFirstC = funArray.GetC(2, lastX, lastY, nextX, nextY);
						}

						if(pLast->m_iDrawType == 2)
						{
							fLastC = funArray.GetC(2, pLast->m_ptOrigin.x, pLast->m_ptOrigin.y, pLast->m_ptEnd.x, pLast->m_ptEnd.y);
						}
						else if(pLast->m_iDrawType == 4)
						{
							fOR = pLast->m_fRadius;
							angle = pLast->m_fAngle;
							bDirection = pLast->m_bDirection;

							centerPoint = funArray.GetCenterPoint(pLast->m_ptOrigin.x, pLast->m_ptOrigin.y, pLast->m_ptEnd.x, pLast->m_ptEnd.y, fOR, angle, bDirection);
							fLastC = funArray.GetC(4, centerPoint.x, centerPoint.y, pLast->m_ptEnd.x, pLast->m_ptEnd.y, bDirection);
						}
						//Start 06-23
						else if(pLast->m_iDrawType == 8)
						{
							NodeArray.clear();
							nPtNumber = pLast->m_iVertexNum;
							NodeArray = vector<vec2>(pLast->controlNode.begin(),pLast->controlNode.end());

							bCurve = CBspline(NodeArray);
							bCurve.ComputeInterpolation();

							size_t NodeCount = bCurve.m_interpolationNode.size();
							lastX = bCurve.m_interpolationNode[NodeCount - 2][0], lastY = bCurve.m_interpolationNode[NodeCount - 2][1];
							nextX = pLast->m_ptEnd.x, nextY = pLast->m_ptEnd.y;
							fLastC = funArray.GetC(2, lastX, lastY, nextX, nextY);
						}

						//表明重合
						if (fabs(fFirstC - fLastC) < g_fDeltaC1 || fabs(fFirstC - fLastC) > 2 * PI - g_fDeltaC1)
						{
							//需要计算重合过渡
							fSumLength = 0.0;
							fDepth = g_fDepth;

							for(int ax = 0; ax < nSize; ++ax)
							{
								g_ArcArray.RemoveAll();
								//取得当前段和下段的C轴角度
								if(ax != nSize - 1)
								{
									pCurGraph = (CGraph*)g_SaveArray.GetAt(g_VectorGroupGraph[iGraphNum].m_GraphIndex.at(ax));
									pNextGraph = (CGraph*)g_SaveArray.GetAt(g_VectorGroupGraph[iGraphNum].m_GraphIndex.at(ax + 1));
								}
								else
								{
									pCurGraph = (CGraph*)g_SaveArray.GetAt(g_VectorGroupGraph[iGraphNum].m_GraphIndex.at(ax));
									pNextGraph = (CGraph*)g_SaveArray.GetAt(g_VectorGroupGraph[iGraphNum].m_GraphIndex.at(0));
								}

								startX = pCurGraph->m_ptOrigin.x;
								startY = pCurGraph->m_ptOrigin.y;
								endX = pCurGraph->m_ptEnd.x;
								endY = pCurGraph->m_ptEnd.y;

								nextStartX = pNextGraph->m_ptOrigin.x;
								nextStartY = pNextGraph->m_ptOrigin.y;
								nextEndX = pNextGraph->m_ptEnd.x;
								nextEndY = pNextGraph->m_ptEnd.y;

								if(pCurGraph->m_iDrawType == 2)
								{
									fCurStartC = fCurEndC = funArray.GetC(2, startX, startY, endX, endY);
								}
								else if(pCurGraph->m_iDrawType == 4)
								{
									fOR = pCurGraph->m_fRadius;
									angle = pCurGraph->m_fAngle;
									bDirection = pCurGraph->m_bDirection;

									centerPoint = funArray.GetCenterPoint(startX, startY, endX, endY, fOR, angle, bDirection);
									fCurEndC = funArray.GetC(4, centerPoint.x, centerPoint.y, endX, endY, bDirection);
								}
								//Start 06-23
								else if(pCurGraph->m_iDrawType == 8)
								{
									NodeArray.clear();
									nPtNumber = pCurGraph->m_iVertexNum;
									NodeArray = vector<vec2>(pCurGraph->controlNode.begin(),pCurGraph->controlNode.end());

									bCurve = CBspline(NodeArray);
									bCurve.ComputeInterpolation();

									size_t NodeCount = bCurve.m_interpolationNode.size();
									lastX = bCurve.m_interpolationNode[NodeCount - 2][0], lastY = bCurve.m_interpolationNode[NodeCount - 2][1];
									nextX = pCurGraph->m_ptEnd.x, nextY = pCurGraph->m_ptEnd.y;
									fCurEndC = funArray.GetC(2, lastX, lastY, nextX, nextY);
								}

								if(pNextGraph->m_iDrawType == 2)
								{
									fNextStartC = fNextEndC = funArray.GetC(2, nextStartX, nextStartY, nextEndX, nextEndY);
								}
								else if(pNextGraph->m_iDrawType == 4)
								{
									fOR = pNextGraph->m_fRadius;
									angle = pNextGraph->m_fAngle;
									bDirection = pNextGraph->m_bDirection;

									centerPoint = funArray.GetCenterPoint(nextStartX, nextStartY, nextEndX, nextEndY, fOR, angle, bDirection);
									fNextStartC = funArray.GetC(4, centerPoint.x, centerPoint.y, nextStartX, nextStartY, bDirection);
								}

								//Start 06-23
								else if(pNextGraph->m_iDrawType == 8)
								{
									NodeArray.clear();
									nPtNumber = pNextGraph->m_iVertexNum;
									NodeArray = vector<vec2>(pNextGraph->controlNode.begin(),pNextGraph->controlNode.end());

									bCurve = CBspline(NodeArray);
									bCurve.ComputeInterpolation();

									lastX = pNextGraph->m_ptOrigin.x, lastY = pNextGraph->m_ptOrigin.y;
									nextX = bCurve.m_interpolationNode[1][0], nextY = bCurve.m_interpolationNode[1][1];
									fNextStartC = funArray.GetC(2, lastX, lastY, nextX, nextY);
								}

								//C轴相差度
								fOffsetC = fNextStartC - fCurEndC;
								if(fOffsetC >= PI)
									fOffsetC = fNextStartC - 2 * PI - fCurEndC;
								else if(fOffsetC <= -PI)
									fOffsetC = fNextStartC + 2 * PI - fCurEndC;

								if(pCurGraph->m_iDrawType == 2)
								{
									fCurStartC = fCurEndC = funArray.GetC(2, startX, startY, endX, endY);
									fLength = funArray.GetDistance(startX, startY, endX, endY);

									fSumLength += fLength;

									if(ax == 0)
									{
										if(pCurGraph->m_bSetPerStep)
										{
											fPerStep = pCurGraph->m_fPerStep;
										}
										else
										{
											if(fLength < 50)
												fPerStep = g_fLineStep_S;
											else
												fPerStep = g_fLineStep_L;
										}
									}
									else
									{
										//进行二分查找最优步长
										fPerStep = fDelteL;
										fPerStep = FindSuitablePerStep(0, fPerStep, -1, fOffsetC, fDelteL, fLength);
									}

									nCount1 = fLength / fPerStep;
									fPerStep = funArray.GetNewStep(fLength, fPerStep);
									fStepX = (endX - startX) / nCount1;
									fStepY = (endY - startY) / nCount1;
									fPerOffsetC = fOffsetC / nCount1;

									iLineCount = ax == 0 ? 0 : 1;
									for(; iLineCount <= nCount1; ++iLineCount)
									{
										ci.x = startX + fStepX * iLineCount;
										ci.y = startY + fStepY * iLineCount;
										ci.c = fCurStartC + fPerOffsetC * iLineCount;

										if(ci.c > 2 * PI)
											ci.c -= 2 * PI;
										if(ci.c < 0)
											ci.c += 2 * PI;

										g_ArcArray.Add(ci);

										EachOfPoint.push_back(ci);
									}

									INT_PTR nEachCount = g_ArcArray.GetSize();
									fDelteC = RadianToAngle(g_ArcArray[nEachCount - 1].c) - RadianToAngle(g_ArcArray[nEachCount - 2].c);
									if(fabs(fDelteC) > 180)
										fDelteC = 360 - fabs(fDelteC);

									fDelteL = sqrt(
										pow(g_ArcArray[nEachCount - 1].x - g_ArcArray[nEachCount - 2].x, 2.0)
										+ pow(g_ArcArray[nEachCount - 1].y - g_ArcArray[nEachCount - 2].y, 2.0)
										+ pow(fDelteC, 2.0)
										);
								}

								else if(pCurGraph->m_iDrawType == 4)
								{
									fOR = pCurGraph->m_fRadius;
									angle = pCurGraph->m_fAngle;
									bDirection = pCurGraph->m_bDirection;

									centerPoint = funArray.GetCenterPoint(startX, startY, endX, endY, fOR, angle, bDirection);

									fLength = fOR * angle;
									fSumLength += fLength;
									//判断半径大小，决定弧长步长 by yuanlin 2013.5.21

									if(ax == 0)
									{
										if(!pCurGraph->m_bSetPerStep)
										{
											if (fOR <= 30)
												fPerStep = g_fCurveStep_S;
											else if (fOR <= 50)
												fPerStep = g_fCurveStep_M;
											else if(fOR <= 100)
												fPerStep = g_fCurveStep_L;
											else if(fOR > 100)
												fPerStep = g_fCurveStep_XL;
										}
										else
											fPerStep = pCurGraph->m_fPerStep;
									}
									else
									{
										//进行二分查找最优步长
										fPerStep = fDelteL / sqrt(1 + pow(180 / (fOR * PI), 2.0));
										fPerStep = FindSuitablePerStep(0, fPerStep, fOR, fOffsetC, fDelteL, fLength);
									}

									nCount1 = fLength / fPerStep;
									fPerStep = funArray.GetNewStep(fLength, fPerStep);
									fUnit = fPerStep / fOR;

									fCurStartC = funArray.GetC(4, centerPoint.x, centerPoint.y, startX, startY, bDirection);
									ci.x = startX;
									ci.y = startY;
									ci.c = fCurStartC;
									g_ArcArray.Add(ci);

									if(ax == 0)
										EachOfPoint.push_back(ci);

									fPerOffsetC = fOffsetC / nCount1;

									for(iArcCount = 1; iArcCount <= nCount1; ++iArcCount)
									{
										funArray.GetElement(4, g_ArcArray[iArcCount - 1].x, g_ArcArray[iArcCount - 1].y, g_ArcArray[iArcCount - 1].c,
											centerPoint.x, centerPoint.y, fUnit, iArcCount, bDirection);
										ci.x = g_ArcArray[iArcCount].x;
										ci.y = g_ArcArray[iArcCount].y;
										ci.c = g_ArcArray[iArcCount].c + fPerOffsetC * iArcCount;

										if(ci.c > 2 * PI)
											ci.c -= 2 * PI;
										if(ci.c < 0)
											ci.c += 2 * PI;

										EachOfPoint.push_back(ci);
									}

									INT_PTR nEachCount = g_ArcArray.GetSize();

									fDelteC = RadianToAngle(g_ArcArray[nEachCount - 1].c) - RadianToAngle(g_ArcArray[nEachCount - 2].c);
									if(fabs(fDelteC) > 180)
										fDelteC = 360 - fabs(fDelteC);

									fDelteL = sqrt(
										pow(g_ArcArray[nEachCount - 1].x - g_ArcArray[nEachCount - 2].x, 2.0)
										+ pow(g_ArcArray[nEachCount - 1].y - g_ArcArray[nEachCount - 2].y, 2.0)
										+ pow(fDelteC, 2.0)
										);
								}


								//Start 06-23
								else if(pCurGraph->m_iDrawType == 8)
								{
									g_SplineArray.RemoveAll();
									NodeArray.clear();
									isFormer = FALSE;
									nPtNumber = pCurGraph->m_iVertexNum;
									clrREF = pCurGraph->clrREF;

									NodeArray = vector<vec2>(pCurGraph->controlNode.begin(), pCurGraph->controlNode.end());
									bCurve = CBspline(NodeArray);
									bCurve.ComputeInterpolation();
									//修改最后一点的坐标
									bCurve.m_interpolationNode[MAX_LENGTH - 1][0] = ((CGraph*)g_SaveArray.GetAt(index))->controlNode[nPtNumber - 1][0];
									bCurve.m_interpolationNode[MAX_LENGTH - 1][1] = ((CGraph*)g_SaveArray.GetAt(index))->controlNode[nPtNumber - 1][1];
									/*
									for(j = 0;j < bCurve.m_interpolationNode.size() - 1;++j)
									{
									//如果isFormer为TRUE，长度小于样条步长时，则起点不变，终点索引往后加1；如果isFormer为FALSE，长度大于样条步长时，则起点为最后一次的终点，此时终点为起点的下一个坐标，依次循环
									if(isFormer == FALSE)
									tmpX = lastX = bCurve.m_interpolationNode[j][0], tmpY = lastY = bCurve.m_interpolationNode[j][1];
									else
									{
									tmpX = x, tmpY = y;
									}

									nextX = bCurve.m_interpolationNode[j + 1][0],nextY = bCurve.m_interpolationNode[j + 1][1];
									fLength += funArray.GetDistance(tmpX, tmpY, nextX, nextY);

									//当某两点之间的距离小于样条步长时，isFormer为FALSE，并把起点的坐标保存下来，并返回到for循环；否则isFormer为FALSE，继续向下执行
									if(fLength - fSplineStep < 0.0)
									{
									isFormer = TRUE;
									x = nextX, y = nextY;

									if(j > bCurve.m_interpolationNode.size() - 20)
									c = 10;

									continue;
									}
									else
									{
									isFormer = FALSE;
									fLength = 0.0;
									}

									//计算新的步长，使其平分
									//tempStep = funArray.GetNewStep(fLength,tempStep);
									lineC = funArray.GetC(2,lastX, lastY, nextX, nextY);

									g_SplineArray.Add(CInfomation(lastX, lastY, lineC));
									lastX = nextX, lastY = nextY;

									}
									//
									if(j == bCurve.m_interpolationNode.size() - 1)
									{
									//if(!bFlag)
									//{
									if(lastX == nextX && lastY == nextY)
									{
									lastX = bCurve.m_interpolationNode[j - 1][0];
									lastY = bCurve.m_interpolationNode[j - 1][1];
									}
									nextX = bCurve.m_interpolationNode[j][0];
									nextY = bCurve.m_interpolationNode[j][1];
									//}

									lineC = funArray.GetC(2,lastX, lastY, nextX, nextY);

									g_SplineArray.Add(CInfomation(lastX, lastY, lineC));
									}

									*/
								}
							}

							INT_PTR nRealSection = EachOfPoint.size() - 1;
							INT_PTR nAreaSection = nRealSection * g_fCoincideRatio;
							INT_PTR nSumSection = nRealSection + nAreaSection * 2;
							fSumLength = fSumLength + fSumLength * g_fCoincideRatio * 2;

							upDownCount = nAreaSection;
							upDownStep = fabs(fDepth) / upDownCount;
							upDownStep = funArray.GetNewStep(fabs(fDepth), upDownStep);//修改每步的渐近高度

							startX = EachOfPoint[0].x;
							startY = EachOfPoint[0].y;
							c = EachOfPoint[0].c;

							if(!bIsWithC)
							{
								if (iGraphNum > 0 && !lastIsPoint)
								{
									fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iDownSpeed, lastEndX, lastEndY, 0.0, RadianToAngle(lastEndC));
									fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iUpSpeed, lastEndX, lastEndY, g_fSafeHeight, RadianToAngle(lastEndC));
								}
								//下刀
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iG00Speed, startX, startY, g_fSafeHeight, RadianToAngle(c));
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iDownSpeed, startX, startY, 0.0, RadianToAngle(c));
							}
							else
							{
								if (iGraphNum > 0 && !lastIsPoint)
								{
									fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iDownSpeed, lastEndX, lastEndY, 0.0);
									fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iUpSpeed, lastEndX, lastEndY, g_fSafeHeight);
								}
								//下刀
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iG00Speed, startX, startY, g_fSafeHeight);
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iDownSpeed, startX, startY, 0.0);
							}
							int az;
							int nIndex = 0;
							for(az = 1, nIndex = nSumSection - 1; az < nSumSection; ++az, --nIndex)
							{
								k = az % nRealSection;

								z = funArray.SetDepth1(fDepth, nSumSection, az, nIndex);

								x = EachOfPoint[k].x;
								y = EachOfPoint[k].y;
								c = EachOfPoint[k].c;

								if(fabs(z - fDepth) > 1e-4)
									bFirstToDepth = FALSE;

								if(!bIsWithC)
								{
									if(g_SaveArray.GetSize() != 0)
									{
										if(z >= fDepth && !bFirstToDepth)
											fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iCoincideSpeed, x, y, z, RadianToAngle(c));
										else
											fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", iFeedSpeed, x, y, z, RadianToAngle(c));
									}
									else
									{
										if(z >= fDepth && !bFirstToDepth)
											fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iCoincideSpeed, x, y, z, RadianToAngle(c));
										else
											fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", iFeedSpeed, x, y, z, RadianToAngle(c));
									}
								}
								else
								{
									if(g_SaveArray.GetSize() != 0)
									{
										if(z >= fDepth && !bFirstToDepth)
											fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iCoincideSpeed, x, y, z);
										else
											fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", iFeedSpeed, x, y, z);
									}
									else
									{
										if(z >= fDepth && !bFirstToDepth)
											fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iCoincideSpeed, x, y, z);
										else
											fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", iFeedSpeed, x, y, z);
									}
								}

								if(fabs(z - fDepth) <= 1e-4)
									bFirstToDepth = TRUE;
							}

							lastEndX = EachOfPoint[k + 1].x;
							lastEndY = EachOfPoint[k + 1].y;
							lastEndC = EachOfPoint[k + 1].c;
							lastEndZ = z;

						}
						else
						{
							//根据当前段终点C轴角度与下段起点C轴角度偏差进行单步补偿C角
							goto Label2;
						}
					}
					else
					{
Label2:
						fSumLength = 0.0;
						fDepth = g_fDepth;
						//不重合
						//根据当前段终点C轴角度与下段起点C轴角度偏差进行单步补偿C角
						for(int ax = 0; ax < nSize - 1; ++ax)
						{
							g_ArcArray.RemoveAll();
							//取得当前段和下段的C轴角度
							pCurGraph = (CGraph*)g_SaveArray.GetAt(g_VectorGroupGraph[iGraphNum].m_GraphIndex.at(ax));
							pNextGraph = (CGraph*)g_SaveArray.GetAt(g_VectorGroupGraph[iGraphNum].m_GraphIndex.at(ax + 1));

							startX = pCurGraph->m_ptOrigin.x;
							startY = pCurGraph->m_ptOrigin.y;
							endX = pCurGraph->m_ptEnd.x;
							endY = pCurGraph->m_ptEnd.y;

							nextStartX = pNextGraph->m_ptOrigin.x;
							nextStartY = pNextGraph->m_ptOrigin.y;
							nextEndX = pNextGraph->m_ptEnd.x;
							nextEndY = pNextGraph->m_ptEnd.y;

							if(pCurGraph->m_iDrawType == 2)
							{
								fCurStartC = fCurEndC = funArray.GetC(2, startX, startY, endX, endY);
							}
							else if(pCurGraph->m_iDrawType == 4)
							{
								fOR = pCurGraph->m_fRadius;
								angle = pCurGraph->m_fAngle;
								bDirection = pCurGraph->m_bDirection;

								centerPoint = funArray.GetCenterPoint(startX, startY, endX, endY, fOR, angle, bDirection);
								fCurEndC = funArray.GetC(4, centerPoint.x, centerPoint.y, endX, endY, bDirection);
							}

							//Start 06-23
							else if(pCurGraph->m_iDrawType == 8)
							{

							}

							if(pNextGraph->m_iDrawType == 2)
							{
								fNextStartC = fNextEndC = funArray.GetC(2, nextStartX, nextStartY, nextEndX, nextEndY);
							}
							else if(pNextGraph->m_iDrawType == 4)
							{
								fOR = pNextGraph->m_fRadius;
								angle = pNextGraph->m_fAngle;
								bDirection = pNextGraph->m_bDirection;

								centerPoint = funArray.GetCenterPoint(nextStartX, nextStartY, nextEndX, nextEndY, fOR, angle, bDirection);
								fNextStartC = funArray.GetC(4, centerPoint.x, centerPoint.y, nextStartX, nextStartY, bDirection);
							}

							//Start 06-23
							else if(pNextGraph->m_iDrawType == 8)
							{

							}

							//C轴相差度
							fOffsetC = fNextStartC - fCurEndC;
							if(fOffsetC >= PI)
								fOffsetC = fNextStartC - 2 * PI - fCurEndC;
							else if(fOffsetC <= -PI)
								fOffsetC = fNextStartC + 2 * PI - fCurEndC;

							/*if(fabs(fabs(fOffsetC) - 2 * PI) < 1e-3)
							fOffsetC = 0.0;*/

							if(pCurGraph->m_iDrawType == 2)
							{
								fCurStartC = fCurEndC = funArray.GetC(2, startX, startY, endX, endY);
								fLength = funArray.GetDistance(startX, startY, endX, endY);

								fSumLength += fLength;

								if(ax == 0)
								{
									if(pCurGraph->m_bSetPerStep)
									{
										fPerStep = pCurGraph->m_fPerStep;
									}
									else
									{
										if(fLength < 50)
											fPerStep = g_fLineStep_S;
										else
											fPerStep = g_fLineStep_L;
									}
								}
								else
								{
									//进行二分查找最优步长
									fPerStep = fDelteL;
									fPerStep = FindSuitablePerStep(0, fPerStep, -1, fOffsetC, fDelteL, fLength);
								}

								nCount1 = fLength / fPerStep;
								fPerStep = funArray.GetNewStep(fLength, fPerStep);
								fStepX = (endX - startX) / nCount1;
								fStepY = (endY - startY) / nCount1;
								fPerOffsetC = fOffsetC / nCount1;

								iLineCount = ax == 0 ? 0 : 1;
								for(; iLineCount <= nCount1; ++iLineCount)
								{
									ci.x = startX + fStepX * iLineCount;
									ci.y = startY + fStepY * iLineCount;
									ci.c = fCurStartC + fPerOffsetC * iLineCount;

									if(ci.c > 2 * PI)
										ci.c -= 2 * PI;
									if(ci.c < 0)
										ci.c += 2 * PI;

									g_ArcArray.Add(ci);

									EachOfPoint.push_back(ci);
								}

								INT_PTR nEachCount = g_ArcArray.GetSize();
								fDelteC = RadianToAngle(g_ArcArray[nEachCount - 1].c) - RadianToAngle(g_ArcArray[nEachCount - 2].c);
								if(fabs(fDelteC) > 180)
									fDelteC = 360 - fabs(fDelteC);

								fDelteL = sqrt(
									pow(g_ArcArray[nEachCount - 1].x - g_ArcArray[nEachCount - 2].x, 2.0)
									+ pow(g_ArcArray[nEachCount - 1].y - g_ArcArray[nEachCount - 2].y, 2.0)
									+ pow(fDelteC, 2.0)
									);
							}

							else if(pCurGraph->m_iDrawType == 4)
							{
								fOR = pCurGraph->m_fRadius;
								angle = pCurGraph->m_fAngle;
								bDirection = pCurGraph->m_bDirection;

								centerPoint = funArray.GetCenterPoint(startX, startY, endX, endY, fOR, angle, bDirection);

								fLength = fOR * angle;
								fSumLength += fLength;
								//判断半径大小，决定弧长步长 by yuanlin 2013.5.21

								if(ax == 0)
								{
									if(!pCurGraph->m_bSetPerStep)
									{
										if (fOR <= 30)
											fPerStep = g_fCurveStep_S;
										else if (fOR <= 50)
											fPerStep = g_fCurveStep_M;
										else if(fOR <= 100)
											fPerStep = g_fCurveStep_L;
										else if(fOR > 100)
											fPerStep = g_fCurveStep_XL;
									}
									else
										fPerStep = pCurGraph->m_fPerStep;
								}
								else
								{
									//进行二分查找最优步长
									fPerStep = fDelteL / sqrt(1 + pow(180 / (fOR * PI), 2.0));
									fPerStep = FindSuitablePerStep(0, fPerStep, fOR, fOffsetC, fDelteL, fLength);
								}

								nCount1 = fLength / fPerStep;
								fPerStep = funArray.GetNewStep(fLength, fPerStep);
								fUnit = fPerStep / fOR;

								fCurStartC = funArray.GetC(4, centerPoint.x, centerPoint.y, startX, startY, bDirection);
								ci.x = startX;
								ci.y = startY;
								ci.c = fCurStartC;
								g_ArcArray.Add(ci);

								if(ax == 0)
									EachOfPoint.push_back(ci);

								fPerOffsetC = fOffsetC / nCount1;

								for(iArcCount = 1; iArcCount <= nCount1; ++iArcCount)
								{
									funArray.GetElement(4, g_ArcArray[iArcCount - 1].x, g_ArcArray[iArcCount - 1].y, g_ArcArray[iArcCount - 1].c,
										centerPoint.x, centerPoint.y, fUnit, iArcCount, bDirection);
									ci.x = g_ArcArray[iArcCount].x;
									ci.y = g_ArcArray[iArcCount].y;
									ci.c = g_ArcArray[iArcCount].c + fPerOffsetC * iArcCount;

									if(ci.c > 2 * PI)
										ci.c -= 2 * PI;
									if(ci.c < 0)
										ci.c += 2 * PI;

									EachOfPoint.push_back(ci);
								}

								INT_PTR nEachCount = g_ArcArray.GetSize();
								fDelteC = RadianToAngle(g_ArcArray[nEachCount - 1].c) - RadianToAngle(g_ArcArray[nEachCount - 2].c);
								if(fabs(fDelteC) > 180)
									fDelteC = 360 - fabs(fDelteC);

								fDelteL = sqrt(
									pow(g_ArcArray[nEachCount - 1].x - g_ArcArray[nEachCount - 2].x, 2.0)
									+ pow(g_ArcArray[nEachCount - 1].y - g_ArcArray[nEachCount - 2].y, 2.0)
									+ pow(fDelteC, 2.0)
									);
							}

							//Start 06-23
							else if(pCurGraph->m_iDrawType == 8)
							{

							}
						}

						//最后一段单独处理
						g_ArcArray.RemoveAll();
						//取得当前段和下段的C轴角度
						CGraph *pCurGraph = (CGraph*)g_SaveArray.GetAt(g_VectorGroupGraph[iGraphNum].m_GraphIndex.at(nSize - 1));

						startX = pCurGraph->m_ptOrigin.x;
						startY = pCurGraph->m_ptOrigin.y;
						endX = pCurGraph->m_ptEnd.x;
						endY = pCurGraph->m_ptEnd.y;

						if(pCurGraph->m_iDrawType == 2)
						{
							fCurStartC = fCurEndC = funArray.GetC(2, startX, startY, endX, endY);
							fLength = funArray.GetDistance(startX, startY, endX, endY);

							fSumLength += fLength;

							fPerStep = fDelteL;
							fPerStep = FindSuitablePerStep(0, fPerStep, -1, fOffsetC, fDelteL, fLength);

							nCount1 = fLength / fPerStep;
							fPerStep = funArray.GetNewStep(fLength, fPerStep);
							fStepX = (endX - startX) / nCount1;
							fStepY = (endY - startY) / nCount1;

							for(iLineCount = 1; iLineCount <= nCount1; ++iLineCount)
							{
								ci.x = startX + fStepX * iLineCount;
								ci.y = startY + fStepY * iLineCount;
								ci.c = fCurStartC;
								EachOfPoint.push_back(ci);
							}
						}
						else if(pCurGraph->m_iDrawType == 4)
						{
							fOR = pCurGraph->m_fRadius;
							angle = pCurGraph->m_fAngle;
							bDirection = pCurGraph->m_bDirection;

							centerPoint = funArray.GetCenterPoint(startX, startY, endX, endY, fOR, angle, bDirection);

							fLength = fOR * angle;
							fSumLength += fLength;
							//判断半径大小，决定弧长步长 by yuanlin 2013.5.21

							fPerStep = fDelteL / sqrt(1 + pow(180 / (fOR * PI), 2.0));
							fPerStep = FindSuitablePerStep(0, fPerStep, fOR, fOffsetC, fDelteL, fLength);

							nCount1 = fLength / fPerStep;
							fPerStep = funArray.GetNewStep(fLength, fPerStep);
							fUnit = fPerStep / fOR;

							fCurStartC = funArray.GetC(4, centerPoint.x, centerPoint.y, startX, startY, bDirection);
							ci.x = startX;
							ci.y = startY;
							ci.c = fCurStartC;
							g_ArcArray.Add(ci);
							//EachOfPoint.push_back(ci);

							for(iArcCount = 1; iArcCount <= nCount1; ++iArcCount)
							{
								funArray.GetElement(4, g_ArcArray[iArcCount - 1].x, g_ArcArray[iArcCount - 1].y, g_ArcArray[iArcCount - 1].c,
									centerPoint.x, centerPoint.y, fUnit, iArcCount, bDirection);
								ci.x = g_ArcArray[iArcCount].x;
								ci.y = g_ArcArray[iArcCount].y;
								ci.c = g_ArcArray[iArcCount].c;
								EachOfPoint.push_back(ci);
							}
						}


						//Start 06-23
						else if(pCurGraph->m_iDrawType == 8)
						{

						}

						int nSection = EachOfPoint.size() - 1;
						int nIndex = 0;	
						fAsymptotic = sqrt(pow(ToolRadius, 2.0) - pow(ToolRadius + fDepth, 2.0)) / fSumLength;

						fDepth = g_fDepth;
						upDownCount = (nSection + 1) * fAsymptotic;
						upDownStep = fabs(fDepth) / upDownCount;
						upDownStep = funArray.GetNewStep(fabs(fDepth), upDownStep);//修改每步的渐近高度

						startX = EachOfPoint[0].x;
						startY = EachOfPoint[0].y;
						c = EachOfPoint[0].c;

						if(!bIsWithC)
						{
							if (iGraphNum > 0 && !lastIsPoint)
							{
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iDownSpeed, lastEndX, lastEndY, 0.0, RadianToAngle(lastEndC));
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iUpSpeed, lastEndX, lastEndY, g_fSafeHeight, RadianToAngle(lastEndC));
							}
							//下刀
							fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iG00Speed, startX, startY, g_fSafeHeight, RadianToAngle(c));
							fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iDownSpeed, startX, startY, 0.0, RadianToAngle(c));
						}
						else
						{
							if (iGraphNum > 0 && !lastIsPoint)
							{
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iDownSpeed, lastEndX, lastEndY, 0.0);
								fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iUpSpeed, lastEndX, lastEndY, g_fSafeHeight);
							}
							//下刀
							fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iG00Speed, startX, startY, g_fSafeHeight);
							fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iDownSpeed, startX, startY, 0.0);
						}

						int az;
						for(az = 1, nIndex = nSection - 1; az < nSection; ++az, --nIndex)
						{
							z = funArray.SetDepth1(fDepth, nSection, az, nIndex);

							x = EachOfPoint[az].x;
							y = EachOfPoint[az].y;
							c = EachOfPoint[az].c;

							if(z > fDepth)
								bFirstToDepth = FALSE;

							if(!bIsWithC)
							{
								if(g_SaveArray.GetSize() != 0)
								{
									if(z >= fDepth && !bFirstToDepth)
										fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iDownSpeed, x, y, z, RadianToAngle(c));
									else
										fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", iFeedSpeed, x, y, z, RadianToAngle(c));
								}
								else
								{
									if(z >= fDepth && !bFirstToDepth)
										fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iDownSpeed, x, y, z, RadianToAngle(c));
									else
										fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", iFeedSpeed, x, y, z, RadianToAngle(c));
								}
							}
							else
							{
								if(g_SaveArray.GetSize() != 0)
								{
									if(z >= fDepth && !bFirstToDepth)
										fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iDownSpeed, x, y, z);
									else
										fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", iFeedSpeed, x, y, z);
								}
								else
								{
									if(z >= fDepth && !bFirstToDepth)
										fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iDownSpeed, x, y, z);
									else
										fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", iFeedSpeed, x, y, z);
								}
							}

							if(z == fDepth)
								bFirstToDepth = TRUE;
						}

						lastEndX = EachOfPoint[az].x;
						lastEndY = EachOfPoint[az].y;
						lastEndC = EachOfPoint[az].c;
						lastEndZ = z;
						/*
						int FirstIndex = g_VectorGroupGraph[iGraphNum].m_GraphIndex[0];
						CGraph *pCur = (CGraph*)g_SaveArray.GetAt(FirstIndex);
						if(pCur->m_iDrawType == 2)
						{
						startX = pCur->m_ptOrigin.x;
						startY = pCur->m_ptOrigin.y;
						endX = pCur->m_ptEnd.x;
						endY = pCur->m_ptEnd.y;
						fLength = funArray.GetDistance(startX, startY, endX, endY);
						nCount1 = fLength / fMinStep;

						fAreaLength = fLength - nCount1 * fMinStep;
						}
						else if(pCur->m_iDrawType == 3)
						{

						}

						upDownCount = nCount1 * fAsymptotic;
						upDownStep = fabs(fDepth) / upDownCount;
						upDownStep = funArray.GetNewStep(fabs(fDepth), upDownStep);//修改每步的渐近高度
						*/
					}
					/*
					fDepth = g_fDepth;
					fMinStep = g_VectorGroupGraph[iGraphNum].m_fMinStep;
					bConnection = g_VectorGroupGraph[iGraphNum].m_bConnection;

					for(int ax = 0; ax < nSize; ++ax)
					{
					EachEdgeOfPolyLine.clear();

					index = g_VectorGroupGraph[iGraphNum].m_GraphIndex.at(ax);
					iDrawType = ((CGraph*)g_SaveArray.GetAt(index))->m_iDrawType;
					startX = ((CGraph*)g_SaveArray.GetAt(index))->m_ptOrigin.x;
					startY = ((CGraph*)g_SaveArray.GetAt(index))->m_ptOrigin.y;
					endX = ((CGraph*)g_SaveArray.GetAt(index))->m_ptEnd.x;
					endY = ((CGraph*)g_SaveArray.GetAt(index))->m_ptEnd.y;

					if (ax==0)
					{
					//对相切段的第一段进行划分
					} 
					else
					{
					}

					switch(iDrawType)
					{
					case 2:
					LineStep = fMinStep;
					fLength = funArray.GetDistance(startX, startY, endX, endY);
					lineC = funArray.GetC(2, startX, startY, endX, endY);

					nCount1 = fLength / LineStep;
					LineStep = funArray.GetNewStep(fLength, LineStep);
					fStepX = (endX - startX) / nCount1;
					fStepY = (endY - startY) / nCount1;

					for(int ay = 0; ay <= nCount1;++ay)
					{
					ci.x = startX + fStepX * ay;
					ci.y = startY + fStepY * ay;
					ci.c = lineC;
					EachEdgeOfPolyLine.push_back(ci);
					}
					EachPolyLine.push_back(EachEdgeOfPolyLine);
					break;

					case 4:
					g_ArcArray.RemoveAll();
					fArcStep = fMinStep;
					fOR = ((CGraph*)g_SaveArray.GetAt(index))->m_fRadius;
					angle = ((CGraph*)g_SaveArray.GetAt(index))->m_fAngle;
					bDirection = ((CGraph*)g_SaveArray.GetAt(index))->m_bDirection;
					fLength = angle * fOR;
					nCount1 = fLength / fArcStep;
					fArcStep = funArray.GetNewStep(fLength, fArcStep);

					fUnit = fArcStep / fOR;
					centerPoint = funArray.GetCenterPoint(startX, startY, endX, endY, fOR, angle, bDirection);
					c = funArray.GetC(4, centerPoint.x, centerPoint.y, startX, startY, bDirection);
					ci.x = startX;
					ci.y = startY;
					ci.c = c;
					g_PolyArray.Add(CInfomation(startX,startY,c));
					EachEdgeOfPolyLine.push_back(ci);

					for(int ay = 1; ay < nCount1; ++ay)
					{
					funArray.GetElement(4,g_ArcArray[ay - 1].x,g_ArcArray[ay - 1].y,g_ArcArray[ay - 1].c,
					centerPoint.x,centerPoint.y,fUnit,ay,bDirection);

					ci.x = g_ArcArray[ay].x;
					ci.y = g_ArcArray[ay].y;
					ci.c = g_ArcArray[ay].c;
					EachEdgeOfPolyLine.push_back(ci);
					}

					c = funArray.GetC(4, centerPoint.x, centerPoint.y, endX, endY, bDirection);
					ci.x = endX;
					ci.y = endY;
					ci.c = c;
					EachEdgeOfPolyLine.push_back(ci);
					EachPolyLine.push_back(EachEdgeOfPolyLine);
					break;
					}

					}
					nPolyVertexCount = 0;
					for(size_t ic = 0;ic < EachPolyLine.size();++ic)
					nPolyVertexCount += EachPolyLine[ic].size();

					nPolySectionCount = nPolyVertexCount - (nSize -= bConnection ? 0 : 1);

					for(int ax = 0; ax < EachPolyLine.size(); ++ax)
					{

					}
					//重合并渐近
					if(bConnection)
					{

					}
					else//非渐近或者非重合
					{

					}
					*/
				}
				iGraphNum++;			
			}
			//by yuanlin 2013.5.21增加最后抬刀过程
			if(!isPoint)
			{
				if(!bIsWithC)
				{
					if(lastEndZ != 0.0)
						fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iDownSpeed, lastEndX, lastEndY, 0.0, RadianToAngle(lastEndC));
					fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n", g_iUpSpeed, lastEndX, lastEndY, g_fSafeHeight, RadianToAngle(lastEndC));
				}
				else
				{
					if(lastEndZ != 0.0)
						fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iDownSpeed, lastEndX, lastEndY, 0.0);
					fprintf_s(SaveFile, "F%d G01 X%.3f Y%.3f Z%.3f;\n", g_iUpSpeed, lastEndX, lastEndY, g_fSafeHeight);
				}
			}

			fprintf_s(SaveFile, "M5\nM9\n");
			//	tmIndex++;
			//}
		}

		//fprintf_s(SaveFile, "T0\n");
		//fprintf_s(SaveFile, "F%d G53 G01 X%.3f Y%.3f Z%.3f C%.3f\n", g_iG00Speed, 0.0, 0.0, 0.0, 0.0);

		if(bAutoBreath)
			fprintf_s(SaveFile, "M24\nM29\n");

		fprintf_s(SaveFile, "M30");

	}
	catch(TCHAR *ErrMsg)
	{
		MessageBox(ErrMsg);
	}

	if(SaveFile)
	{
		fclose(SaveFile);
		SaveFile = NULL;
	}

}


/***********************************
** 参数设置函数
** 参数
** 时间：2013-5-10
***********************************/
void Cd2gView::OnSaveSetting()
{
	// TODO: Add your command handler code here
	m_bIsLink = FALSE;

	CPoint m_rectLeftTop;							//矩形的左上角坐标
	CPoint m_rectRightBottom;						//矩形的右下角坐标
	double rate1, rate2;
	CString s[37];

	CParameterDialog parameterDlg;
	//保存上一次的参数值
	parameterDlg.m_iFeedSpeed = g_iFeedSpeed;
	parameterDlg.m_fDepth = -g_fDepth;
	//parameterDlg.m_strToolSelect = g_strToolSelect;
	parameterDlg.m_LineStep_S = g_fLineStep_S;
	parameterDlg.m_LineStep_L = g_fLineStep_L;
	parameterDlg.m_CurveStep = g_fCurveStep_S;	//曲线步长by yuanlin 13.5.21
	parameterDlg.m_CurveStep_M = g_fCurveStep_M;	//曲线步长by yuanlin 13.5.21
	parameterDlg.m_CurveStep_L = g_fCurveStep_L;	//曲线步长by yuanlin 13.5.21
	parameterDlg.m_CurveStep_XL = g_fCurveStep_XL;//曲线步长by yuanlin 13.5.21
	parameterDlg.m_fSafeHeight = g_fSafeHeight;
	parameterDlg.m_iG00speed = g_iG00Speed;
	parameterDlg.m_iUpSpeed = g_iUpSpeed;
	parameterDlg.m_iDownSpeed = g_iDownSpeed;
	parameterDlg.m_clr = color;
	//parameterDlg.m_fStartEndStep = g_fStartEndStep;
	parameterDlg.m_iLength = g_iLength;
	parameterDlg.m_iWidth = g_iWidth;
	parameterDlg.m_EllipseStep_S = g_fEllipseStep_S;
	parameterDlg.m_ElipseStep_M = g_fEllipseStep_M;
	parameterDlg.m_EllipseStep_L = g_fEllipseStep_L;
	parameterDlg.m_EllipseStep_XL = g_fEllipseStep_XL;

	parameterDlg.m_fSplineStep = g_fSplineStep;
	//parameterDlg.m_iWorkStation = m_iWorkStation;
	//parameterDlg.m_iDownWay = g_iDownWay;
	//parameterDlg.m_fPointRadius = m_fPointRadius;
	parameterDlg.m_Asymptotic = g_fAsymptotic;
	parameterDlg.m_fCoincideRatio = g_fCoincideRatio;
	parameterDlg.m_iCoincideSpeed = g_iCoincideSpeed;
	parameterDlg.m_fPointPerDepth = g_fPointPerDepth;
	parameterDlg.m_iPointSpeed = g_iPointSpeed;

	//Start	Cf_Lsd_06_25
	parameterDlg.m_fDeltaAngle = g_fDeltaAngle;
	//End Cf_Lsd_06_25

	//Start Cf_Lsd_10_22
	parameterDlg.m_iCarveWay = g_iCarveWay;
	//End Cf_Lsd_10_22

	if(IDOK == parameterDlg.DoModal())
	{
		//Start Cf_Lsd_06_25
		g_fDeltaAngle = parameterDlg.m_fDeltaAngle;
		//End Cf_Lsd_06_25

		//Start Cf_Lsd_10_22
		g_iCarveWay = parameterDlg.m_iCarveWay;
		//End Cf_Lsd_10_22

		g_iPointSpeed = parameterDlg.m_iPointSpeed;
		g_fPointPerDepth = parameterDlg.m_fPointPerDepth;
		g_iCoincideSpeed = parameterDlg.m_iCoincideSpeed;
		g_fCoincideRatio = parameterDlg.m_fCoincideRatio;
		g_fAsymptotic = parameterDlg.m_Asymptotic;
		//m_fPointRadius = parameterDlg.m_fPointRadius;
		//g_iDownWay = parameterDlg.m_iDownWay;
		//m_iWorkStation = parameterDlg.m_iWorkStation;
		g_fSplineStep = parameterDlg.m_fSplineStep;
		g_fEllipseStep_S = parameterDlg.m_EllipseStep_S;
		g_fEllipseStep_M = parameterDlg.m_ElipseStep_M;
		g_fEllipseStep_L = parameterDlg.m_EllipseStep_L;
		g_fEllipseStep_XL = parameterDlg.m_EllipseStep_XL;

		g_iLength = parameterDlg.m_iLength;
		g_iWidth = parameterDlg.m_iWidth;
		//g_fStartEndStep = parameterDlg.m_fStartEndStep;
		color = parameterDlg.m_clr;
		g_iFeedSpeed = parameterDlg.m_iFeedSpeed;
		g_fDepth = -parameterDlg.m_fDepth;
		//g_strToolSelect = parameterDlg.m_strToolSelect;
		g_fLineStep_S = parameterDlg.m_LineStep_S;
		g_fLineStep_L = parameterDlg.m_LineStep_L;
		g_fCurveStep_S = parameterDlg.m_CurveStep;		//曲线步长by yuanlin 13.5.21
		g_fCurveStep_L = parameterDlg.m_CurveStep_L;		//曲线步长by yuanlin 13.5.21
		g_fCurveStep_M = parameterDlg.m_CurveStep_M;		//曲线步长by yuanlin 13.5.21
		g_fCurveStep_XL = parameterDlg.m_CurveStep_XL;	//曲线步长by yuanlin 13.5.21
		g_fSafeHeight = parameterDlg.m_fSafeHeight;
		g_iG00Speed = parameterDlg.m_iG00speed;
		g_iUpSpeed = parameterDlg.m_iUpSpeed;
		g_iDownSpeed = parameterDlg.m_iDownSpeed;

		rate1 = g_iLength * 1.0 / (1366 - g_hSetOffset);
		rate2 = g_iWidth * 1.0 / (644 - g_vSetOffset);
		g_fRate = rate1 >= rate2 ? rate1 : rate2;
		int count = g_fRate / 0.5;
		g_fRate = 0.5 * (count + 1);
		g_iSpacing += (g_fRate / 0.5 - 2);

		////画矩形框
		//CRect rectLarge;
		//GetClientRect(&rectLarge);
		//m_rectLeftTop.x = 0 + 20 + g_hSetOffset;
		//m_rectLeftTop.y = rectLarge.Height() - g_vSetOffset - g_iWidth;
		//m_rectRightBottom.x = g_iLength + 20 + g_hSetOffset;
		//m_rectRightBottom.y = rectLarge.Height() - g_vSetOffset;
		//g_RectNew.SetRect(m_rectLeftTop,m_rectRightBottom);
		Invalidate();

		//修改相切角的弧度
		g_fDeltaC1 = g_fDeltaAngle * PI / 180;

		s[0].Format(L"%d",	g_iFeedSpeed);
		s[1].Format(L"%f",	g_fDepth);
		s[2].Format(L"%s",	g_strToolSelect);
		s[3].Format(L"%f",	g_fLineStep_S);
		s[4].Format(L"%f",	g_fLineStep_L);
		s[5].Format(L"%f",	g_fOffsetDegree);
		s[6].Format(L"%f",	g_fSafeHeight);
		s[7].Format(L"%d",	g_iG00Speed);
		s[8].Format(L"%d",	g_iDownSpeed);
		s[9].Format(L"%d",	g_iUpSpeed);
		s[10].Format(L"%f", g_fCurveStep_S);
		s[11].Format(L"%s", color);
		s[12].Format(L"%f", g_fStartEndStep);
		s[13].Format(L"%d", g_iDownWay);
		s[14].Format(L"%f", g_fCurveStep_M);
		s[15].Format(L"%f", g_fCurveStep_L);
		s[16].Format(L"%f", g_fCurveStep_XL);
		s[17].Format(L"%d", g_iLength);
		s[18].Format(L"%d", g_iWidth);
		s[19].Format(L"%f", g_fEllipseStep_S);
		s[20].Format(L"%f", g_fEllipseStep_M);
		s[21].Format(L"%f", g_fEllipseStep_L);
		s[22].Format(L"%f", g_fEllipseStep_XL);

		s[23].Format(L"%f", g_fSplineStep);
		s[24].Format(L"%d", m_iWorkStation);
		s[25].Format(L"%f", m_fPointRadius);
		s[26].Format(L"%f", g_fCoincide_S);
		s[27].Format(L"%f", g_fCoincide_M);
		s[28].Format(L"%f", g_fCoincide_L);
		s[29].Format(L"%f", g_fCoincide_XL);
		s[30].Format(L"%f", g_fAsymptotic);
		s[31].Format(L"%f", g_fCoincideRatio);
		s[32].Format(L"%d", g_iCoincideSpeed);
		s[33].Format(L"%f", g_fPointPerDepth);
		s[34].Format(L"%d", g_iPointSpeed);

		//Start Cf_Lsd_06_25
		s[35].Format(L"%f", g_fDeltaAngle);
		//End Cf_Lsd_06_25
	
		//Start Cf_Lsd_10_22
		s[36].Format(L"%d", g_iCarveWay);
		//End Cf_Lsd_10_22


		RegisterInfomation regInfo[] = 
		{
			{L"IDC_FEEDSET",		s[0]},
			{L"IDC_Z_POSSET",		s[1]},
			{L"IDC_TOOLLIST",		s[2]},
			{L"IDC_LINESTEP_S",		s[3]},
			{L"IDC_LINESTEP_L",		s[4]},
			{L"IDC_OFFSET",			s[5]},
			{L"IDC_SAFEHEIGHT",		s[6]},
			{L"IDC_FEEDSPEED",		s[7]},
			{L"IDC_DOWNSPEED",		s[8]},
			{L"IDC_UPSPEED",		s[9]},
			{L"IDC_CURVESTEP",		s[10]},
			{L"IDC_COLORLIST",		s[11]},
			{L"IDC_UPDOWN",			s[12]},
			{L"IDC_DOWNWAY",		s[13]},
			{L"IDC_CURVESTEP2",		s[14]},
			{L"IDC_CURVESTEP3",		s[15]},
			{L"IDC_CURVESTEP4",		s[16]},
			{L"IDC_LENGTH",			s[17]},
			{L"IDC_WIDTH",			s[18]},
			{L"IDC_ELLIPSESTEP_S",	s[19]},
			{L"IDC_ELLIPSESTEP_M",	s[20]},
			{L"IDC_ELLIPSESTEP_L",	s[21]},
			{L"IDC_ELLIPSESTEP_XL",	s[22]},

			{L"IDC_SPLINESTEP",		s[23]},
			{L"IDC_WORKSTATION",	s[24]},
			{L"IDC_POINTRADIUS",	s[25]},
			{L"IDC_COINCIDE_S",		s[26]},
			{L"IDC_COINCIDE_M",		s[27]},
			{L"IDC_COINCIDE_L",		s[28]},
			{L"IDC_COINCIDE_X",		s[29]},
			{L"IDC_ASYMPTOTIC",		s[30]},
			{L"IDE_COINCIDE_RATIO",	s[31]},
			{L"IDE_COINCIDE_SPEED", s[32]},
			{L"IDE_POINTDEPTH_PER", s[33]},
			{L"IDE_POINT_SPEED",	s[34]},

			//Start	Cf_Lsd_06_25
			{L"IDE_DELTA_ANGLE",	s[35]},
			//End Cf_Lsd_06_25

			{L"IDE_CARVE_WAY",		s[36]}
		};

		for(int i = 0; i < 37; ++i)
			WritePrivateProfileString(L"Setting", regInfo[i].idString, regInfo[i].amount, m_strSaveSettingPath);
	}
}

//新建
void Cd2gView::OnFileNew()
{
	// TODO: Add your command handler code here
	if(g_GraphArray.GetSize())
	{
		if(IDOK == MessageBox(L"是否清除图形数据？", L"Warning", MB_OKCANCEL))
		{
			ClearData();
			Invalidate();
		}
	}

	if(g_ImitatePerPointDataVector.size() > 0)
	{
		if(IDOK == MessageBox(L"模拟进行中,是否强制结束?", L"Warning", MB_OKCANCEL))
		{
			::SendMessage(m_SpeedDlg.m_hWnd, WM_COMMAND, IDC_BTN_ENDDLG, (LPARAM)(m_SpeedDlg.GetDlgItem(IDC_BTN_ENDDLG))->m_hWnd);
			::SendMessage(m_hWnd, WM_ENDDLG, 0, 0);
			/*g_DrawVector.clear();
			g_ImitatePerPointDataVector.clear();
			SpeedDlg.m_ImitateStatus = PAUSE;
			m_bStartImitate = FALSE;
			CurrentImitateIndex = 0;
			::SendMessage(SpeedDlg.m_hWnd,WM_INITDIALOG,0,0);*/
		}
	}
}


/***********************************
** 文件打开函数
** 参数
** 时间：2013-5-10
***********************************/
void Cd2gView::OnFileOpen()
{
	if(g_ImitatePerPointDataVector.size() > 0)
	{
		if(IDOK == MessageBox(L"模拟进行中,是否强制结束?", L"Warning", MB_OKCANCEL))
		{
			//::SendMessage(SpeedDlg.m_hWnd,WM_COMMAND,IDC_BTN_ENDDLG,(LPARAM)(SpeedDlg.GetDlgItem(IDC_BTN_ENDDLG))->m_hWnd);
			m_SpeedDlg.DestroyWindow();
			KillTimer(TIMER_IMITATE);

			g_DrawVector.clear();
			g_ImitatePerPointDataVector.clear();
			m_SpeedDlg.m_ImitateStatus = PAUSE;
			m_bStartImitate = FALSE;
			CurrentImitateIndex = 0;
			Invalidate();

			//::SendMessage(m_hWnd,WM_ENDDLG,0,0);
			/*g_DrawVector.clear();
			g_ImitatePerPointDataVector.clear();
			SpeedDlg.m_ImitateStatus = PAUSE;
			m_bStartImitate = FALSE;
			CurrentImitateIndex = 0;
			::SendMessage(SpeedDlg.m_hWnd,WM_INITDIALOG,0,0);*/
			//KillTimer()
		}
	}

	Sleep(10);
	CString strExt;
	m_bIsLink = FALSE;
	// TODO: Add your command handler code here
	for(int j = 0; j < g_GraphArray.GetSize(); ++j)
		((CGraph*)g_GraphArray.GetAt(j))->m_bIsPitched = FALSE;

	CFileDialog fileDlg(TRUE);
	fileDlg.m_ofn.lpstrFilter = L"DXF Files(*.dxf)\0*.dxf\0TXT Files(*.txt)\0*.txt\0NC Files(*.NC)\0*.NC\0All Files(*.*)\0*.*\0\0";
	//fileDlg.m_ofn.lpstrDefExt = "dxf";
	if(IDOK != fileDlg.DoModal())
	{
		return ;
	}
	m_filePathName = fileDlg.GetPathName();
	m_fileName = fileDlg.GetFileName();
	AfxGetMainWnd()->SetWindowText(m_filePathName);

	strExt = m_filePathName.Mid(m_filePathName.ReverseFind('.') + 1);
	if(!strExt.CompareNoCase(L"dxf"))
	{
		try
		{
			DxfReading(m_filePathName);
			KillTimer(TIMER_IMITATE);
			m_CommandType = CT_NONE;
		}
		//07-06修改Cf
		//Start
		catch(TCHAR *ErrorMsg)
		{
			MessageBox(ErrorMsg);
			g_GraphArray.RemoveAll();
			return;
		}
		//End
	}
	else if(!strExt.CompareNoCase(L"txt") || !strExt.CompareNoCase(L"nc"))
	{
		LoadGCodeFromNCFile(m_filePathName);
	}
	Invalidate();
}


/*********************************
** 图形连接函数
** 参数
** 时间：2013-5-10
***********************************/
void Cd2gView::OnLink()
{
	// TODO: Add your command handler code here
	m_bIsLink = TRUE;
}


/***********************************
** 图形断开函数
** 参数
** 时间：2013-5-10
***********************************/
void Cd2gView::OnUnlink()
{
	// TODO: Add your command handler code here
	m_bIsLink = FALSE;
}


/***********************************
** 鼠标右键按下弹起菜单函数
** 参数
** 时间：2013-5-10
***********************************/
void Cd2gView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_OperationPoint = CPoint2D(point.x, point.y);
	CPoint2D rPoint = CPoint2D(point.x, point.y);

	UINT bControlSpeed = m_SpeedDlg.m_hWnd == NULL ? TRUE : FALSE;

	if(m_CommandType != CT_ADJUSTLOCATION 
		&& m_CommandType != CT_SCREENMOVE 
		&& m_CommandType != CT_SET_A_ANGLE)
	{
		menu.LoadMenu(IDR_POPMENU);
		pSubMenu = menu.GetSubMenu(0);

		if(g_OperationArray.GetSize() == 0)
		{
			//图形操作

			//06-27修改Cf
			//Start
			pSubMenu->EnableMenuItem(IDC_COPY, MF_GRAYED);
			pSubMenu->EnableMenuItem(IDC_CUT, MF_GRAYED);
			pSubMenu->EnableMenuItem(IDC_DELETE, MF_GRAYED);
			pSubMenu->EnableMenuItem(IDC_MOVE, MF_GRAYED);
			//End

			//渐近菜单
			pSubMenu->EnableMenuItem(IDC_BOTH_GROW, MF_GRAYED);
			pSubMenu->EnableMenuItem(IDC_FRONT_GROW, MF_GRAYED);
			pSubMenu->EnableMenuItem(IDC_ENDING_GROW, MF_GRAYED);

			//方向菜单
			pSubMenu->EnableMenuItem(IDC_SHOWDIRECTION, MF_GRAYED);
			pSubMenu->EnableMenuItem(IDC_RESERVEDIRECTION, MF_GRAYED);
			pSubMenu->EnableMenuItem(IDC_UNSHOWDIRECTION, MF_GRAYED);

			//步长菜单
			pSubMenu->EnableMenuItem(IDM_SET_NEWSTEP, MF_GRAYED);
			pSubMenu->EnableMenuItem(IDM_SET_NEWFEEDSPEED, MF_GRAYED);
			pSubMenu->EnableMenuItem(IDM_MODIFY_POINT_DEPTH, MF_GRAYED);
		}
	}
	else if(m_CommandType == CT_ADJUSTLOCATION 
		|| m_CommandType == CT_SCREENMOVE
		|| m_CommandType == CT_SET_A_ANGLE)
	{
		menu.LoadMenu(IDR_POPMENU1);
		pSubMenu = menu.GetSubMenu(0);
	}

	pSubMenu->EnableMenuItem(ID_CONTROL_SPEED, bControlSpeed);

	CPoint popDownPoint = point;
	ClientToScreen(&popDownPoint);
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, popDownPoint.x, popDownPoint.y, this);
	menu.Detach();

	CView::OnRButtonDown(nFlags, point);
}


/***********************************
** 复制操作函数
** 参数
** 时间：2013-5-10
***********************************/
void Cd2gView::OnCopy()
{
	m_bIsLink = FALSE;
	// TODO: Add your command handler code here
	m_iDrawType = 0;

	//06-27修改Cf
	//Start
	if(g_OperationArray.GetSize() > 0)
	{
		m_CommandType = CT_COPY;

	}
	//End

	Invalidate();

}


/***********************************
** 平移操作函数
** 参数
** 时间：2013-5-10
***********************************/
void Cd2gView::OnMove()
{
	// TODO: Add your command handler code here
	m_bIsLink = FALSE;
	m_CommandType = CT_MOVE;
}


/***********************************
** 删除操作函数
** 参数
** 时间：2013-5-10
***********************************/
void Cd2gView::OnDelete()
{
	// TODO: Add your command handler code here
	m_bIsLink = FALSE;
	BOOL bIsPitched;


	//将操作数组清空
	g_OperationArray.RemoveAll();
	CPoint2D tempS, tempE;

	for(int i = 0; i < g_GraphArray.GetSize(); ++i)
	{
		CPoint tempStartPoint = rectStartPoint, tempEndPoint = rectEndPoint;
		bIsPitched = ((CGraph*)g_GraphArray.GetAt(i))->m_bIsPitched;
		if(! bIsPitched)
		{
			funArray.toSureRectenglePosition(tempStartPoint, tempEndPoint);
			tempS = funArray.ScreenToMyQuadrant(tempStartPoint) + CPoint2D(g_hOffset, g_vOffset);
			tempE = funArray.ScreenToMyQuadrant(tempEndPoint) + CPoint2D(g_hOffset, g_vOffset);

			tempStartPoint = CPoint(tempS.x, tempS.y);
			tempEndPoint = CPoint(tempE.x, tempE.y);

			tempStartPoint = CPoint(tempStartPoint.x * g_fRate, tempStartPoint.y * g_fRate);
			tempEndPoint = CPoint(tempEndPoint.x * g_fRate, tempEndPoint.y * g_fRate);

			bIsPitched = funArray.isInRect(CRect(tempStartPoint, tempEndPoint), (CGraph*)g_GraphArray.GetAt(i));
		}

		if(bIsPitched)
		{
			//int returnMessage = MessageBox("Are you sure to delete this graph?","Warning",MB_OKCANCEL);
			//if(returnMessage == IDOK)
			//{
			g_GraphArray.RemoveAt(i);
			i -= 1;
			//}
		}	
		g_OperationArray.RemoveAll();
		g_iNumArray.RemoveAll();
		Invalidate();
	}
}

/***********************************
** 旋转操作函数
** 参数
** 时间：2013-5-10
***********************************/
void Cd2gView::OnRotate()
{
	// TODO: Add your command handler code here
	m_bIsLink = FALSE;
}


/***********************************
** View初始化函数
** 参数
** 时间：2013-5-10
***********************************/
int Cd2gView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	USES_CONVERSION;
	char* szSerial = T2A(funArray.GetMachineSerial());

	CString strExeFolderName = GetModuleFolderPath();

	//把机器码写入文件中
	CString strReadMeFilePath = strExeFolderName + _T("\\ReadMe.txt");
	CFile ReadMeFile(strReadMeFilePath, CFile::modeCreate | CFile::modeReadWrite);
	if(ReadMeFile.m_hFile)
	{
		ReadMeFile.Write(szSerial, strlen(szSerial));
		ReadMeFile.Close();
	}

	//经过两次md5运算
	md5 = new MD5(szSerial);
	string str = md5->toString();
	/*md5 = new MD5();
	md5->reset();
	md5->update(szSerial, strlen(szSerial));
	string str = md5->toString();
	md5->reset();
	md5->update(str.c_str(), strlen(str.c_str()));
	str = md5->toString();*/
	md5->update(str);
	str = md5->toString();

	/************************************************************************/
	CString strLicenseFile = GetModuleFolderPath() + _T("\\") + _T("License.ini");

	char buf[100] = {0};
	CFile LicenseFile;
	if(LicenseFile.Open(strLicenseFile, CFile::modeRead))
	{
		LicenseFile.Read(buf, 100);
		LicenseFile.Close();
	}
	else
	{
		MessageBox(L"序列号文件不存在...", L"启动失败", MB_ICONWARNING);
		return -1;
	}

	delete md5;
	//if(strcmp(str.c_str(), buf) != 0)
	//{
	//	MessageBox(L"序列号匹配失败,请联系开发商...", L"启动失败", MB_ICONWARNING);
	//	return -1;
	//}

	/*CTime time = CTime::GetCurrentTime();
	int year = time.GetYear();
	int month = time.GetMonth();
	int day = time.GetDay();

	if(year != 2015 || month != 5)
	return -1;*/


	CString s[37];
	if(PathFileExists(m_strSaveSettingPath))
	{
		::GetPrivateProfileString(L"Setting", L"IDC_FEEDSET", NULL, s[0].GetBuffer(10), 10, m_strSaveSettingPath);
		::GetPrivateProfileString(L"Setting", L"IDC_Z_POSSET", NULL, s[1].GetBuffer(10), 10, m_strSaveSettingPath);
		::GetPrivateProfileString(L"Setting", L"IDC_TOOLLIST", NULL, s[2].GetBuffer(10), 10, m_strSaveSettingPath);
		::GetPrivateProfileString(L"Setting", L"IDC_LINESTEP_S", NULL, s[3].GetBuffer(10), 10, m_strSaveSettingPath);
		::GetPrivateProfileString(L"Setting", L"IDC_LINESTEP_L", NULL, s[4].GetBuffer(10), 10, m_strSaveSettingPath);
		::GetPrivateProfileString(L"Setting", L"IDC_OFFSET", NULL, s[5].GetBuffer(10), 10, m_strSaveSettingPath);
		::GetPrivateProfileString(L"Setting", L"IDC_SAFEHEIGHT", NULL, s[6].GetBuffer(10), 10, m_strSaveSettingPath);
		::GetPrivateProfileString(L"Setting", L"IDC_FEEDSPEED", NULL, s[7].GetBuffer(10), 10, m_strSaveSettingPath);
		::GetPrivateProfileString(L"Setting", L"IDC_DOWNSPEED", NULL, s[8].GetBuffer(10), 10, m_strSaveSettingPath);
		::GetPrivateProfileString(L"Setting", L"IDC_UPSPEED", NULL, s[9].GetBuffer(10), 10, m_strSaveSettingPath);
		::GetPrivateProfileString(L"Setting", L"IDC_CURVESTEP", NULL, s[10].GetBuffer(10), 10, m_strSaveSettingPath);
		::GetPrivateProfileString(L"Setting", L"IDC_COLORLIST", NULL, s[11].GetBuffer(10), 10, m_strSaveSettingPath);
		::GetPrivateProfileString(L"Setting", L"IDC_UPDOWN", NULL, s[12].GetBuffer(10), 10, m_strSaveSettingPath);
		::GetPrivateProfileString(L"Setting", L"IDC_DOWNWAY", NULL, s[13].GetBuffer(10), 10, m_strSaveSettingPath);
		::GetPrivateProfileString(L"Setting", L"IDC_CURVESTEP2", NULL, s[14].GetBuffer(10), 10, m_strSaveSettingPath);
		::GetPrivateProfileString(L"Setting", L"IDC_CURVESTEP3", NULL, s[15].GetBuffer(10), 10, m_strSaveSettingPath);
		::GetPrivateProfileString(L"Setting", L"IDC_CURVESTEP4", NULL, s[16].GetBuffer(10), 10, m_strSaveSettingPath);
		::GetPrivateProfileString(L"Setting", L"IDC_LENGTH", NULL, s[17].GetBuffer(10), 10, m_strSaveSettingPath);
		::GetPrivateProfileString(L"Setting", L"IDC_WIDTH", NULL, s[18].GetBuffer(10), 10, m_strSaveSettingPath);
		::GetPrivateProfileString(L"Setting", L"IDC_ELLIPSESTEP_S", NULL, s[19].GetBuffer(10), 10, m_strSaveSettingPath);
		::GetPrivateProfileString(L"Setting", L"IDC_ELLIPSESTEP_M", NULL, s[20].GetBuffer(10), 10, m_strSaveSettingPath);
		::GetPrivateProfileString(L"Setting", L"IDC_ELLIPSESTEP_L", NULL, s[21].GetBuffer(10), 10, m_strSaveSettingPath);
		::GetPrivateProfileString(L"Setting", L"IDC_ELLIPSESTEP_XL", NULL, s[22].GetBuffer(10), 10, m_strSaveSettingPath);
		::GetPrivateProfileString(L"Setting", L"IDC_SPLINESTEP", NULL, s[23].GetBuffer(10), 10, m_strSaveSettingPath);
		::GetPrivateProfileString(L"Setting", L"IDC_WORKSTATION", NULL, s[24].GetBuffer(10), 10, m_strSaveSettingPath);
		::GetPrivateProfileString(L"Setting", L"IDC_POINTRADIUS", NULL, s[25].GetBuffer(10), 10, m_strSaveSettingPath);
		::GetPrivateProfileString(L"Setting", L"IDC_COINCIDE_S", NULL, s[26].GetBuffer(10), 10, m_strSaveSettingPath);
		::GetPrivateProfileString(L"Setting", L"IDC_COINCIDE_M", NULL, s[27].GetBuffer(10), 10, m_strSaveSettingPath);
		::GetPrivateProfileString(L"Setting", L"IDC_COINCIDE_L", NULL, s[28].GetBuffer(10), 10, m_strSaveSettingPath);
		::GetPrivateProfileString(L"Setting", L"IDC_COINCIDE_X", NULL, s[29].GetBuffer(10), 10, m_strSaveSettingPath);
		::GetPrivateProfileString(L"Setting", L"IDC_ASYMPTOTIC", NULL, s[30].GetBuffer(10), 10, m_strSaveSettingPath);
		::GetPrivateProfileString(L"Setting", L"IDE_COINCIDE_RATIO", NULL, s[31].GetBuffer(10), 10, m_strSaveSettingPath);
		::GetPrivateProfileString(L"Setting", L"IDE_COINCIDE_SPEED", NULL, s[32].GetBuffer(10), 10, m_strSaveSettingPath);
		::GetPrivateProfileString(L"Setting", L"IDE_POINTDEPTH_PER", NULL, s[33].GetBuffer(10), 10, m_strSaveSettingPath);
		::GetPrivateProfileString(L"Setting", L"IDE_POINT_SPEED", NULL, s[34].GetBuffer(10), 10, m_strSaveSettingPath);

		//Start	Cf_Lsd_06_25
		::GetPrivateProfileString(L"Setting", L"IDE_DELTA_ANGLE", NULL, s[35].GetBuffer(10), 10, m_strSaveSettingPath);
		//End Cf_Lsd_06_25

		//Start Cf_Lsd_10_22
		::GetPrivateProfileString(L"Setting", L"IDE_CARVE_WAY", NULL, s[36].GetBuffer(10), 10, m_strSaveSettingPath);
		//End Cf_Lsd_10_22
		

		g_iFeedSpeed = _ttoi(s[0]);
		g_fDepth = _wtof(s[1]);
		g_strToolSelect = s[2];
		g_fLineStep_S = _wtof(s[3]);
		g_fLineStep_L = _wtof(s[4]);
		g_fOffsetDegree = _wtof(s[5]);
		g_fSafeHeight = _wtof(s[6]);
		g_iG00Speed = _ttoi(s[7]);
		g_iDownSpeed = _ttoi(s[8]);
		g_iUpSpeed = _ttoi(s[9]);
		g_fCurveStep_S = _wtof(s[10]);
		color = s[11];
		g_fStartEndStep = _wtof(s[12]);
		g_iDownWay = _ttoi(s[13]);
		g_fCurveStep_M = _wtof(s[14]);
		g_fCurveStep_L = _wtof(s[15]);
		g_fCurveStep_XL = _wtof(s[16]);
		g_iLength = _ttoi(s[17]);
		g_iWidth = _ttoi(s[18]);
		g_fEllipseStep_S = _ttoi(s[19]);
		g_fEllipseStep_M = _ttoi(s[20]);
		g_fEllipseStep_L = _ttoi(s[21]);
		g_fEllipseStep_XL = _ttoi(s[22]);

		g_fSplineStep = _wtof(s[23]);
		m_iWorkStation = _ttoi(s[24]);
		m_fPointRadius = _wtof(s[25]);
		g_fCoincide_S = _wtof(s[26]);
		g_fCoincide_M = _wtof(s[27]);
		g_fCoincide_L = _wtof(s[28]);
		g_fCoincide_XL = _wtof(s[29]);
		g_fAsymptotic = _wtof(s[30]);
		g_fCoincideRatio = _wtof(s[31]);
		g_iCoincideSpeed = _wtoi(s[32]);
		g_fPointPerDepth = _wtof(s[33]);
		g_iPointSpeed = _wtoi(s[34]);

		//Start Cf_Lsd_06_25
		g_fDeltaAngle = _wtof(s[35]);
		g_fDeltaC1 = g_fDeltaAngle * PI / 180;
		//End Cf_Lsd_06_25

		//Start Cf_Lsd_10_22
		g_iCarveWay = _ttoi(s[36]);
		//End Cf_Lsd_10_22
	}
	else
	{
		g_iFeedSpeed = 6000;
		g_fDepth = -0.45;
		g_strToolSelect = L"T1";
		g_fLineStep_S = 1.0;
		g_fLineStep_L = 2.0;
		g_fOffsetDegree = 0.0;
		g_fSafeHeight = 5.0;
		g_iG00Speed = 10000;
		g_iDownSpeed = 500;
		g_iUpSpeed = 2000;
		g_fCurveStep_S = 2.0;
		color = L"Black";
		g_fStartEndStep = 0.4;
		g_iDownWay = 0;
		g_fCurveStep_M = 2.0;
		g_fCurveStep_L = 2.0;
		g_fCurveStep_XL = 2.0;
		g_iLength = 2200;
		g_iWidth = 1600;
		g_fEllipseStep_S = 2.0;
		g_fEllipseStep_M = 4.0;
		g_fEllipseStep_L = 6.0;
		g_fEllipseStep_XL = 8.0;

		g_fSplineStep = 1.0;
		m_iWorkStation = 0;
		m_fPointRadius = 0.0;
		g_fCoincide_S = 60.0;
		g_fCoincide_M = 75.0;
		g_fCoincide_L = 90.0;
		g_fCoincide_XL = 100.0;
		g_fAsymptotic = 0.2;
		g_fCoincideRatio = 0.25;
		g_iCoincideSpeed = 3000;
		g_fPointPerDepth = 0.4;
		g_iPointSpeed = 2000;

		//Start Cf_Lsd_06_25
		g_fDeltaAngle = 20.0;
		g_fDeltaC1 = g_fDeltaAngle * PI / 180;
		//End Cf_Lsd_06_25

		//Start Cf_Lsd_10_22
		g_iCarveWay = 0;
		//End Cf_Lsd_10_22
	}

	return 0;
}

//另存为事件
void Cd2gView::OnFileSaveAs()
{
	// TODO: Add your command handler code here
	CFileDialog fileDlg(FALSE);
	CString saveAsPath ;
	fileDlg.m_ofn.lpstrFilter = L"Text Files(*.txt)\0*.txt";
	fileDlg.m_ofn.lpstrDefExt = L"txt";
	if(IDOK == fileDlg.DoModal())
	{	
		m_filePathName = fileDlg.GetPathName();
		m_savePath = m_filePathName;
		m_DefaultFileName = fileDlg.GetFileName();
		OnFileSave();
	}
}

//设置保存路径
CString Cd2gView::GetSavePath(CString strExePath)
{
	if (strExePath.IsEmpty())
	{
		GetModuleFileName(NULL, strExePath.GetBuffer(MAX_PATH), MAX_PATH);
		strExePath.ReleaseBuffer();

		if(m_fileName.IsEmpty())
			m_DefaultFileName = L"程序.txt";
		else
		{
			m_fileName.MakeLower();
			m_fileName.Replace(L"dxf", L"txt");
			m_DefaultFileName = m_fileName;// + L".txt";
		}
	}

	strExePath = strExePath.Left(strExePath.ReverseFind(_T('\\')));	
	m_savePath = L"";
	return strExePath;
}

//显示方向
void Cd2gView::OnShowDirection()
{
	// TODO: Add your command handler code here
	//	g_OperationArray.RemoveAll();
	m_bIsLink = FALSE;

	for(int k = 0; k < g_OperationArray.GetSize(); ++k)
	{
		((CGraph*)g_OperationArray.GetAt(k))->m_bIsShowArrow = TRUE;
		((CGraph*)g_OperationArray.GetAt(k))->m_bIsPitched = FALSE;
	}
	g_OperationArray.RemoveAll();
	g_iNumArray.RemoveAll();
	Invalidate();
}

//刷新
void Cd2gView::OnRefresh()
{
	m_bIsLink = FALSE;
	// TODO: Add your command handler code here
	rectStartPoint = rectEndPoint = 0;
	g_OperationArray.RemoveAll();
	g_iNumArray.RemoveAll();

	for(int i = 0; i < g_GraphArray.GetSize(); ++i)
	{
		((CGraph*)g_GraphArray.GetAt(i))->m_bIsPitched = FALSE;
	}
	Invalidate();
	SetStatusText(L"取消");
}

//
void Cd2gView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if(nChar == VK_DELETE)
		OnDelete();
	else if(nChar == VK_F5)
		OnRefresh();
	else if(nChar == VK_ESCAPE)
	{
		m_iDrawType = 0;
		m_bFlag = FALSE;
		m_iVirtualRectCount = 0;
		m_iLineCount = 0;
		m_iCircleCount = 0;
		m_iArcCount = 0;
		g_OperationArray.RemoveAll();
		m_CommandType = CT_NONE;
		OnRefresh();
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

//
BOOL Cd2gView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	TCHAR nChar = (TCHAR)(pMsg->wParam);
	if(nChar == 'Z' && GetAsyncKeyState(VK_CONTROL))
		OnUndo();
	else if(nChar == 'C' && GetAsyncKeyState(VK_CONTROL))
		OnCopy();
	else if(nChar == 'A' && GetAsyncKeyState(VK_CONTROL))
		OnSelectAll();

	//06-27修改Cf
	//Start
	//else if(nChar == 'V' && GetAsyncKeyState(VK_CONTROL))
	//	OnStick();
	//End


	if (pMsg->message == WM_ENDDLG)
	{
		m_SpeedDlg.m_ImitateStatus = PAUSE;
		m_bStartImitate = FALSE;
		CurrentImitateIndex = 0;
		//::PostMessage(SpeedDlg.m_hWnd,WM_INITDIALOG,0,0);
		KillTimer(TIMER_IMITATE);
	}

	return CView::PreTranslateMessage(pMsg);
}

//撤销
void Cd2gView::OnUndo(void)
{
	int i = g_GraphArray.GetSize();
	if(i > 0)
	{
		g_GraphArray.RemoveAt(i - 1);
		Invalidate();
	}
}

//图形反向
void Cd2gView::OnReserveDirection()
{
	m_bIsLink = FALSE;
	// TODO: Add your command handler code here
	for(int j = 0; j < g_OperationArray.GetSize(); ++j)
	{
		DirectionReserve((CGraph*)g_OperationArray.GetAt(j));
		((CGraph*)g_OperationArray.GetAt(j))->m_bIsPitched = FALSE;
	}
	g_OperationArray.RemoveAll();
	g_iNumArray.RemoveAll();
	Invalidate();
}

//取消显示方向
void Cd2gView::OnUnShowDirection()
{
	m_bIsLink = FALSE;
	// TODO: Add your command handler code here
	for(int k = 0; k < g_OperationArray.GetSize(); ++k)
	{
		((CGraph*)g_OperationArray.GetAt(k))->m_bIsShowArrow = FALSE;
		((CGraph*)g_OperationArray.GetAt(k))->m_bIsPitched = FALSE;
	}
	g_OperationArray.RemoveAll();
	g_iNumArray.RemoveAll();
	Invalidate();
}

//鼠标滑轮事件
BOOL Cd2gView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default

	//zDelta > 0,滚轮向上，zDelta < 0,滚轮向下
	if(zDelta > 0 && g_iSpacing >= 1.1)//缩小
	{
		g_iSpacing -= 1;
		g_fRate += 0.1;
		//g_fRate *= 2;
	}
	if(zDelta < 0)//放大
	{
		if(g_fRate > 0.1)
		{
			g_iSpacing += 1;
			g_fRate -= 0.1;
		}
	}

	if(g_iSpacing > 0)
		Invalidate();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
	//return TRUE;
}

//选中图形后，画的轮廓
void Cd2gView::DrawFrame(CDC *pDC,CGraph *pGraph)
{
	m_bIsLink = FALSE;
	CClientDC dc(this);
	CPoint2D startPoint, endPoint, centerPoint, midPoint;
	double deltaX = 0, deltaY = 0, fRadius = 0;
	CPoint2D leftTop, leftBottom, rightTop, rightBottom;
	double fMajor = 0, fMinor = 0, degree = 0;
	BOOL bDirection;
	CArray<CPoint2D>m_PeakGroup;
	CPoint2D dotPoint;
	double startAngle = 0, endAngle = 0;
	int nu;
	double *fPoint = NULL;

	vector<vec2>controlPoint;
	double lastX = 0, lastY = 0, nextX = 0, nextY = 0;

	CBrush brush, *oldBrush = NULL, *pBrush = NULL, *oldPBrush = NULL;
	CPen *oldPen = NULL;

	CPen dcPen(PS_SOLID, 2, RGB(0, 0, 255)), pen(PS_DOT, 1, RGB(0, 0, 255));
	brush.CreateSolidBrush(RGB(0, 255, 255));
	pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));

	switch(pGraph->m_iDrawType)
	{
	case 2:
		startPoint = pGraph->m_ptOrigin;
		endPoint = pGraph->m_ptEnd;

		startPoint /= g_fRate;
		endPoint /= g_fRate;

		leftTop = funArray.MyQuadrantToScreen(startPoint) + CPoint2D(-g_hOffset, g_vOffset);
		rightBottom = funArray.MyQuadrantToScreen(endPoint) + CPoint2D(-g_hOffset, g_vOffset);
		centerPoint = CPoint2D((leftTop.x + rightBottom.x) / 2.0, (leftTop.y + rightBottom.y) / 2.0);

		pDC->SelectObject(&dcPen);
		pDC->SelectObject(&brush);
		DrawPoint(pDC, leftTop);
		DrawPoint(pDC, rightBottom);
		DrawPoint(pDC, centerPoint);

		oldPen = pDC->SelectObject(&pen);
		oldPBrush = pDC->SelectObject(pBrush);
		pDC->Rectangle(CRect(CPoint(leftTop.x, leftTop.y), CPoint(rightBottom.x, rightBottom.y)));
		break;

	case 3:
		centerPoint = pGraph->m_ptCenter;
		fRadius = pGraph->m_fRadius / g_fRate;

		centerPoint /= g_fRate;

		leftTop = funArray.MyQuadrantToScreen(centerPoint.x, centerPoint.y + fRadius) + CPoint2D(-g_hOffset, g_vOffset);//圆心上点
		leftBottom = funArray.MyQuadrantToScreen(centerPoint.x - fRadius, centerPoint.y) + CPoint2D(-g_hOffset, g_vOffset);//圆心左点
		rightTop = funArray.MyQuadrantToScreen(centerPoint.x, centerPoint.y - fRadius) + CPoint2D(-g_hOffset, g_vOffset);//圆心下点
		rightBottom = funArray.MyQuadrantToScreen(centerPoint.x + fRadius, centerPoint.y) + CPoint2D(-g_hOffset, g_vOffset);//圆心右点
		centerPoint = funArray.MyQuadrantToScreen(centerPoint.x, centerPoint.y) + CPoint2D(-g_hOffset, g_vOffset);

		pDC->SelectObject(&dcPen);
		pDC->SelectObject(&brush);
		DrawPoint(pDC, leftTop);
		DrawPoint(pDC, leftBottom);
		DrawPoint(pDC, rightTop);
		DrawPoint(pDC, rightBottom);
		DrawPoint(pDC, centerPoint);

		oldPen = pDC->SelectObject(&pen);
		oldPBrush = pDC->SelectObject(pBrush);
		pDC->MoveTo(leftTop.x, leftTop.y);
		pDC->LineTo(rightTop.x, rightTop.y);
		pDC->MoveTo(leftBottom.x, leftBottom.y);
		pDC->LineTo(rightBottom.x, rightBottom.y);
		break;

	case 4:
		centerPoint = pGraph->m_ptCenter;
		startPoint = pGraph->m_ptOrigin;
		midPoint = pGraph->m_ptMiddle;
		endPoint = pGraph->m_ptEnd;

		startPoint /= g_fRate;
		endPoint /= g_fRate;
		centerPoint /= g_fRate;
		midPoint /= g_fRate;

		leftTop = funArray.MyQuadrantToScreen(startPoint) + CPoint2D(-g_hOffset, g_vOffset);//起点
		leftBottom = funArray.MyQuadrantToScreen(midPoint) + CPoint2D(-g_hOffset, g_vOffset);//中点
		rightTop= funArray.MyQuadrantToScreen(endPoint) + CPoint2D(-g_hOffset, g_vOffset);//终点
		rightBottom = funArray.MyQuadrantToScreen(centerPoint) + CPoint2D(-g_hOffset, g_vOffset);//圆心

		pDC->SelectObject(&dcPen);
		pDC->SelectObject(&brush);
		DrawPoint(pDC, leftTop);
		DrawPoint(pDC, leftBottom);
		DrawPoint(pDC, rightTop);
		DrawPoint(pDC, rightBottom);

		oldPen = pDC->SelectObject(&pen);
		oldPBrush = pDC->SelectObject(pBrush);
		pDC->MoveTo(rightBottom.x, rightBottom.y);
		pDC->LineTo(leftTop.x, leftTop.y);
		pDC->MoveTo(rightBottom.x, rightBottom.y);
		pDC->LineTo(rightTop.x, rightTop.y);
		pDC->MoveTo(rightBottom.x, rightBottom.y);
		pDC->LineTo(leftBottom.x, leftBottom.y);

		break;

	case 5:
		startPoint = pGraph->m_ptOrigin;
		endPoint = pGraph->m_ptEnd;

		leftTop = startPoint;
		leftBottom = CPoint2D(startPoint.x, endPoint.y);
		rightTop = CPoint2D(endPoint.x, startPoint.y);
		rightBottom = endPoint;

		leftTop /= g_fRate;
		leftBottom /= g_fRate;
		rightTop /= g_fRate;
		rightBottom /= g_fRate;

		leftTop = funArray.MyQuadrantToScreen(leftTop) + CPoint2D(-g_hOffset, g_vOffset);
		leftBottom = funArray.MyQuadrantToScreen(leftBottom) + CPoint2D(-g_hOffset, g_vOffset);
		rightTop = funArray.MyQuadrantToScreen(rightTop) + CPoint2D(-g_hOffset, g_vOffset);
		rightBottom = funArray.MyQuadrantToScreen(rightBottom) + CPoint2D(-g_hOffset, g_vOffset);

		oldPen = pDC->SelectObject(&dcPen);
		oldPBrush = pDC->SelectObject(&brush);
		DrawPoint(pDC, leftTop);
		DrawPoint(pDC, leftBottom);
		DrawPoint(pDC, rightTop);
		DrawPoint(pDC, rightBottom);	
		break;

	case  6:
		centerPoint = pGraph->m_ptCenter;
		fMajor = pGraph->m_fMajor;
		fMinor = pGraph->m_fMinor;
		degree = pGraph->m_fDegree;
		bDirection = pGraph->m_bDirection;
		startAngle = pGraph->m_fStartAngle;
		endAngle = pGraph->m_fEndAngle;

		centerPoint /= g_fRate;
		fMajor /= g_fRate;
		fMinor /= g_fRate;

		startPoint = funArray.GetEllipsePoint(fMajor, fMinor, degree, centerPoint, startAngle * 180 / PI, bDirection);
		endPoint = funArray.GetEllipsePoint(fMajor, fMinor, degree, centerPoint, endAngle * 180 / PI, bDirection);

		if(fabs(startAngle - 0.0) < 1e-6 && fabs(endAngle - 2 * PI) < 1e-6)//闭合图形
		{
			leftTop = CPoint2D(funArray.GetEllipsePoint(fMajor, fMinor, degree, centerPoint, 0, bDirection).x, funArray.GetEllipsePoint(fMajor, fMinor, degree, centerPoint, 0, bDirection).y);
			leftBottom = CPoint2D(funArray.GetEllipsePoint(fMajor, fMinor, degree, centerPoint, 90, bDirection).x, funArray.GetEllipsePoint(fMajor, fMinor, degree, centerPoint, 90, bDirection).y);
			rightTop = CPoint2D(funArray.GetEllipsePoint(fMajor, fMinor, degree, centerPoint, 180, bDirection).x, funArray.GetEllipsePoint(fMajor, fMinor, degree, centerPoint, 180, bDirection).y);
			rightBottom = CPoint2D(funArray.GetEllipsePoint(fMajor, fMinor, degree, centerPoint, 270, bDirection).x, funArray.GetEllipsePoint(fMajor, fMinor, degree, centerPoint, 270, bDirection).y);
			leftTop = funArray.MyQuadrantToScreen(leftTop) + CPoint2D(-g_hOffset, g_vOffset);//右点
			leftBottom = funArray.MyQuadrantToScreen(leftBottom) + CPoint2D(-g_hOffset, g_vOffset);//上点
			rightTop = funArray.MyQuadrantToScreen(rightTop) + CPoint2D(-g_hOffset, g_vOffset);//左点
			rightBottom = funArray.MyQuadrantToScreen(rightBottom) + CPoint2D(-g_hOffset, g_vOffset);//下点
			centerPoint = funArray.MyQuadrantToScreen(centerPoint) + CPoint2D(-g_hOffset, g_vOffset);//圆心

			pDC->SelectObject(&dcPen);
			pDC->SelectObject(&brush);
			DrawPoint(pDC, centerPoint);
			DrawPoint(pDC, leftTop);
			DrawPoint(pDC, leftBottom);
			DrawPoint(pDC, rightTop);
			DrawPoint(&dc, rightBottom);		

			oldPen = pDC->SelectObject(&pen);
			oldPBrush = pDC->SelectObject(pBrush);
			pDC->MoveTo(leftTop.x, leftTop.y);
			pDC->LineTo(rightTop.x, rightTop.y);
			pDC->MoveTo(leftBottom.x, leftBottom.y);
			pDC->LineTo(rightBottom.x, rightBottom.y);
		}
		else
		{
			startPoint = funArray.MyQuadrantToScreen(startPoint) + CPoint2D(-g_hOffset, g_vOffset);//右点
			endPoint = funArray.MyQuadrantToScreen(endPoint) + CPoint2D(-g_hOffset, g_vOffset);//上点
			centerPoint = funArray.MyQuadrantToScreen(centerPoint) + CPoint2D(-g_hOffset, g_vOffset);//圆心

			pDC->SelectObject(&dcPen);
			pDC->SelectObject(&brush);
			DrawPoint(pDC, centerPoint);
			DrawPoint(pDC, startPoint);
			DrawPoint(pDC, endPoint);		

			oldPen = pDC->SelectObject(&pen);
			oldPBrush = pDC->SelectObject(pBrush);
			pDC->MoveTo(centerPoint.x, centerPoint.y);
			pDC->LineTo(startPoint.x, startPoint.y);
			pDC->MoveTo(centerPoint.x, centerPoint.y);
			pDC->LineTo(endPoint.x, endPoint.y);
		}
		break;

	case 7:
		m_PeakGroup.RemoveAll();
		m_PeakGroup.Copy(pGraph->m_PeakGroup);
		nu = pGraph->m_iVertexNum;
		fPoint = new double[nu * 3];

		for(int j = 0; j < nu; ++j)
		{
			fPoint[j * 3] = m_PeakGroup[j].x;
			fPoint[j * 3 + 1] = m_PeakGroup[j].y;
			//fPoint[j * 3 + 2] = m_PeakGroup[j].con;
		}

		oldPen = pDC->SelectObject(&dcPen);
		oldPBrush = pDC->SelectObject(&brush);
		for(int j = 0;j < nu;++j)
		{
			dotPoint = CPoint2D(fPoint[j * 3] / g_fRate, fPoint[j * 3 + 1] / g_fRate);
			dotPoint = funArray.MyQuadrantToScreen(dotPoint) + CPoint2D(-g_hOffset, g_vOffset);
			DrawPoint(pDC, dotPoint);
		}
		//	dc.SelectObject(oldBrush);
		break;

	case 8:
		controlPoint.clear();
		nu = pGraph->m_iVertexNum;
		controlPoint = vector<vec2>(pGraph->controlNode.begin(), pGraph->controlNode.end());

		pDC->SelectObject(&dcPen);
		pDC->SelectObject(&brush);
		for(int j = 0; j < nu; ++j)
		{
			dotPoint = CPoint2D(controlPoint[j][0] / g_fRate,controlPoint[j][1] / g_fRate);
			dotPoint = funArray.MyQuadrantToScreen(dotPoint) + CPoint2D(-g_hOffset, g_vOffset);
			DrawPoint(pDC, dotPoint);
		}

		oldPen = pDC->SelectObject(&pen);
		oldPBrush = pDC->SelectObject(pBrush);
		for(int j = 0; j < nu - 1; ++j)
		{
			lastX = controlPoint[j][0], lastY = controlPoint[j][1];
			nextX = controlPoint[j + 1][0], nextY = controlPoint[j + 1][1];
			pDC->MoveTo(lastX / g_fRate + 20 + g_hSetOffset, g_Rect.Height() - g_vSetOffset - lastY / g_fRate);
			pDC->LineTo(nextX / g_fRate + 20 + g_hSetOffset, g_Rect.Height() - g_vSetOffset - nextY / g_fRate);
		}
		break;

	case 9:
		startPoint = pGraph->m_ptOrigin;
		startPoint /= g_fRate;

		startPoint = funArray.MyQuadrantToScreen(startPoint) + CPoint2D(-g_hOffset, g_vOffset);

		oldPen = pDC->SelectObject(&dcPen);
		oldPBrush = pDC->SelectObject(&brush);
		DrawPoint(pDC, startPoint);
		break;
	}

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldPBrush);

	pBrush->DeleteObject();
	oldPBrush->DeleteObject();

	pBrush = NULL;
	oldPBrush = NULL;
	DeleteObject(brush);

	if(fPoint)
	{
		delete []fPoint;
		fPoint = NULL;
	}

}

//
void Cd2gView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	m_TotalSize.cx = 3000;
	m_TotalSize.cy = 1500;
	CRect rt;
	GetClientRect(&rt);
	g_Rect = rt;
}


//画节点
void Cd2gView::DrawPoint(CDC* pDC, const CPoint2D fPoint)
{
	m_bIsLink = FALSE;
	/*CBrush brush,*oldBrush;
	brush.CreateSolidBrush(RGB(0,0,0));
	oldBrush = pDC->SelectObject(&brush);

	CPen pen(PS_DOT,4,RGB(0,0,0)),*oldPen;
	CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	oldPen = pDC->SelectObject(&pen);
	CBrush *oldPBrush = pDC->SelectObject(pBrush);

	pDC->Ellipse(fPoint.x - 1,fPoint.y - 1,fPoint.x + 1,fPoint.y + 1);
	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldPBrush);*/

	//CPen pen(PS_DOT,2,RGB(0,0,255)),*oldPen;
	//CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	//oldPen = pDC->SelectObject(&pen);
	//CBrush *oldPBrush = pDC->SelectObject(pBrush);

	pDC->Ellipse(fPoint.x - 3, fPoint.y - 3, fPoint.x + 3, fPoint.y + 3);
	//pDC->SelectObject(oldPen);
	//pDC->SelectObject(oldPBrush);

}	

//
void Cd2gView::OnUpdateLink(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here

	if(!m_bIsLink)
	{
		pCmdUI->Enable();
		//pCmdUI->SetCheck(0);
	}
	else
	{
		pCmdUI->Enable(FALSE);
		//pCmdUI->SetCheck(1);
	}
}	

//
void Cd2gView::OnUpdateUnlink(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here

	if(!m_bIsLink)
	{
		pCmdUI->Enable(FALSE);
		//pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->Enable();
		//pCmdUI->SetCheck(0);
	}
}

//两端渐近
void Cd2gView::OnBothGrow()
{
	// TODO: Add your command handler code here
	m_bIsLink = FALSE;
	int SelectedNum;

	for(int i = 0;i < g_OperationArray.GetSize();++i)
	{
		//如果不是多段线，直接将颜色变成红色
		//如果是多段线，则将点选中的编号对应的m_PolyColorType进行修改
		if(((CGraph*)g_OperationArray.GetAt(i))->m_iDrawType != 7)
			((CGraph*)g_OperationArray.GetAt(i))->clrREF = RGB(255, 0, 0);
		else
		{
			SelectedNum = ((CGraph*)g_OperationArray.GetAt(i))->m_SelectedNum;
			((CGraph*)g_OperationArray.GetAt(i))->m_PolyColorType[SelectedNum] = RED;
		}
	}

	MessageBox(L"设置两边渐近成功!");
	Invalidate();
}

//头渐近
void Cd2gView::OnFrontGrow()
{
	// TODO: Add your command handler code here
	m_bIsLink = FALSE;
	int SelectedNum;

	for(int i = 0; i < g_OperationArray.GetSize(); ++i)
	{
		//如果不是多段线，直接将颜色变成绿色
		//如果是多段线，则将点选中的编号对应的m_PolyColorType进行修改
		if(((CGraph*)g_OperationArray.GetAt(i))->m_iDrawType != 7)
			((CGraph*)g_OperationArray.GetAt(i))->clrREF = RGB(0, 255, 0);
		else
		{
			SelectedNum = ((CGraph*)g_OperationArray.GetAt(i))->m_SelectedNum;
			((CGraph*)g_OperationArray.GetAt(i))->m_PolyColorType[SelectedNum] = GREEN;
		}
	}

	MessageBox(L"设置头渐近成功!");
	Invalidate();
}

//尾渐近
void Cd2gView::OnEndingGrow()
{
	// TODO: Add your command handler code here
	m_bIsLink = FALSE;
	int SelectedNum;

	for(int i = 0; i < g_OperationArray.GetSize(); ++i)
	{
		//如果不是多段线，直接将颜色变成蓝色
		//如果是多段线，则将点选中的编号对应的m_PolyColorType进行修改
		if(((CGraph*)g_OperationArray.GetAt(i))->m_iDrawType != 7)
			((CGraph*)g_OperationArray.GetAt(i))->clrREF = RGB(0, 0, 255);
		else
		{
			SelectedNum = ((CGraph*)g_OperationArray.GetAt(i))->m_SelectedNum;
			((CGraph*)g_OperationArray.GetAt(i))->m_PolyColorType[SelectedNum] = BLUE;
		}
	}

	MessageBox(L"设置尾渐近成功!");
	Invalidate();
}

//取消渐近
void Cd2gView::OnCancelGrow()
{
	// TODO: Add your command handler code here
	m_bIsLink = FALSE;
	int SelectedNum;

	for(int i = 0; i < g_OperationArray.GetSize(); ++i)
	{
		//如果不是多段线，直接将颜色变成黑色
		//如果是多段线，则将点选中的编号对应的m_PolyColorType进行修改
		if(((CGraph*)g_OperationArray.GetAt(i))->m_iDrawType != 7)
			((CGraph*)g_OperationArray.GetAt(i))->clrREF = RGB(0, 0, 0);
		else
		{
			SelectedNum = ((CGraph*)g_OperationArray.GetAt(i))->m_SelectedNum;
			((CGraph*)g_OperationArray.GetAt(i))->m_PolyColorType[SelectedNum] = EMPTY;
		}
	}

	MessageBox(L"取消渐近!");
	Invalidate();
}

//多段线合并
void Cd2gView::OnMerge()
{
	// TODO: Add your command handler code here
	int selectCount = g_OperationArray.GetSize();
	if(selectCount < 2)
	{
		MessageBox(L"请至少选择两个图形进行合并.");
		return ;
	}

	int type = 0, iVertex = 0;
	BOOL bFlags = FALSE;//闭合标记
	bool bDirection = FALSE, uFlags = FALSE;
	CPoint2D sLastPoint, eLastPoint, sCurPoint, eCurPoint, sPoint, ePoint;
	double fMajor = 0.0, fMinor = 0.0, degree = 0.0, angle = 0.0, con = 0.0;
	CGraph *newPolyGraph;
	CArray <CPoint2D> vArray;
	CArray <CPoint2D> pm1, pm2;
	vArray.RemoveAll();
	pm1.RemoveAll();
	int ptNum, k;

	CGraph *qpGraph = (CGraph*)g_OperationArray.GetAt(0);
	type = ((CGraph*)g_OperationArray.GetAt(0))->m_iDrawType;
	switch(type)
	{
	case LINE:
		sLastPoint = ((CGraph*)g_OperationArray.GetAt(0))->m_ptOrigin;
		eLastPoint = ((CGraph*)g_OperationArray.GetAt(0))->m_ptEnd;
		con = 0.0;

		sPoint = sLastPoint;
		vArray.Add(CPoint2D(sLastPoint.x, sLastPoint.y, con));
		iVertex++;
		break;

	case ARC:
		sLastPoint = ((CGraph*)g_OperationArray.GetAt(0))->m_ptOrigin;
		eLastPoint = ((CGraph*)g_OperationArray.GetAt(0))->m_ptEnd;
		angle = ((CGraph*)g_OperationArray.GetAt(0))->m_fAngle;
		bDirection = ((CGraph*)g_OperationArray.GetAt(0))->m_bDirection;
		con = tan(angle / 4.0);
		if(bDirection)
			con = -con;

		sPoint = sLastPoint;
		vArray.Add(CPoint2D(sLastPoint.x, sLastPoint.y, con));
		iVertex++;
		break;

	case POLYLINE:
		ptNum = ((CGraph*)g_OperationArray.GetAt(0))->m_iVertexNum;
		uFlags = ((CGraph*)g_OperationArray.GetAt(0))->m_bFlags;
		pm1.Copy(((CGraph*)g_OperationArray.GetAt(0))->m_PeakGroup);
		sLastPoint = CPoint2D(pm1[0].x, pm1[0].y);
		sPoint = sLastPoint;

		if(!uFlags)//非闭合
		{
			eLastPoint = CPoint2D(pm1[ptNum - 1].x, pm1[ptNum - 1].y);
			for(k = 0; k < ptNum - 1; ++k)
			{
				vArray.Add(CPoint2D(pm1[k].x, pm1[k].y, pm1[k].con));
				iVertex++;
			}
		}
		else//闭合
		{
			eLastPoint = CPoint2D(pm1[0].x, pm1[0].y);
			//vArray.Copy(pm1);
			for(k = 0; k < ptNum; ++k)
			{
				vArray.Add(CPoint2D(pm1[k].x, pm1[k].y, pm1[k].con));
				iVertex++;
			}
		}
		break;
	}

	for(int i = 1; i < selectCount; ++i)
	{
		type = ((CGraph*)g_OperationArray.GetAt(i))->m_iDrawType;

		pm2.RemoveAll();

		switch(type)
		{
		case LINE:
			sCurPoint = ((CGraph*)g_OperationArray.GetAt(i))->m_ptOrigin;
			eCurPoint = ((CGraph*)g_OperationArray.GetAt(i))->m_ptEnd;
			if(fabs(eLastPoint.x - sCurPoint.x) > 1e-3 || (fabs(eLastPoint.y - sCurPoint.y) > 1e-3))
			{
				MessageBox(L"头尾不相接，请查看线段是否相连或者方向不一致，合并失败!");
				return;
			}

			vArray.Add(CPoint2D(sCurPoint.x, sCurPoint.y, 0.0));
			iVertex++;
			break;

		case ARC:
			sCurPoint = ((CGraph*)g_OperationArray.GetAt(i))->m_ptOrigin;
			eCurPoint = ((CGraph*)g_OperationArray.GetAt(i))->m_ptEnd;
			if(fabs(eLastPoint.x - sCurPoint.x) > 1e-3 || (fabs(eLastPoint.y - sCurPoint.y) > 1e-3))
			{
				MessageBox(L"头尾不相接，请查看线段是否相连或者方向不一致，合并失败!");
				return;
			}

			angle = ((CGraph*)g_OperationArray.GetAt(i))->m_fAngle;
			bDirection = ((CGraph*)g_OperationArray.GetAt(i))->m_bDirection;
			con = tan(angle / 4.0);
			if(bDirection)
				con = -con;
			vArray.Add(CPoint2D(sCurPoint.x, sCurPoint.y, con));
			iVertex++;
			break;

		case POLYLINE:
			ptNum = ((CGraph*)g_OperationArray.GetAt(i))->m_iVertexNum;
			uFlags = ((CGraph*)g_OperationArray.GetAt(i))->m_bFlags;
			pm2.Copy(((CGraph*)g_OperationArray.GetAt(i))->m_PeakGroup);
			sCurPoint = CPoint2D(pm2[0].x, pm2[0].y);

			if(fabs(eLastPoint.x - sCurPoint.x) > 1e-3 || (fabs(eLastPoint.y - sCurPoint.y) > 1e-3))
			{
				MessageBox(L"头尾不相接，请查看线段是否相连或者方向不一致，合并失败!");
				return;
			}

			if(!uFlags)
			{
				eCurPoint = CPoint2D(pm2[ptNum - 1].x, pm2[ptNum - 1].y);
				for(k = 0; k < ptNum - 1; ++k)
				{
					vArray.Add(CPoint2D(pm2[k].x, pm2[k].y, pm2[k].con));
					iVertex++;
				}
			}
			else
			{
				eCurPoint = CPoint2D(pm2[0].x, pm2[0].y);
				for(k = 0; k < ptNum; ++k)
				{
					vArray.Add(CPoint2D(pm2[k].x, pm2[k].y, pm2[k].con));
					iVertex++;
				}
			}

			break;
		}
		//vArray.Add(CPoint2D(sCurPoint.x,sCurPoint.y,0.0));
		//iVertex++;
		sLastPoint = sCurPoint;
		eLastPoint = eCurPoint;
	}

	vArray.Add(CPoint2D(eCurPoint.x, eCurPoint.y, 0.0));//最后一点
	iVertex++;

	if(fabs(ePoint.x - sCurPoint.x) < 1e-3 && (fabs(sPoint.y - sCurPoint.y) < 1e-3))
		bFlags = TRUE;

	pt2Array = CPointArray(sCurPoint, eCurPoint);
	newPolyGraph = new CGraph;
	newPolyGraph->m_iDrawType = 7;
	newPolyGraph->clrREF = RGB(0, 0, 0);

	newPolyGraph->m_iVertexNum = iVertex;
	newPolyGraph->m_bFlags = bFlags;
	newPolyGraph->m_PeakGroup.Copy(vArray);
	newPolyGraph->m_ptOrigin = sPoint;
	newPolyGraph->pt2Array = pt2Array;

	int no;
	int *array = new int[selectCount];
	for(int k = 0; k < selectCount; ++k)
	{
		no = g_iNumArray[k];
		array[k] = g_iNumArray[k];
	}

	funArray.MySort(array, selectCount);

	for(int i = 0; i < selectCount; ++i)
		g_GraphArray.RemoveAt(array[i]);

	g_GraphArray.Add(newPolyGraph);
	//g_OperationArray.RemoveAll();
	Invalidate();
}

//放大
void Cd2gView::OnBigger()
{
	//m_bIsBigger = TRUE;
	m_iDrawType = 0;
	m_CommandType = CT_NONE;
	if(g_fRate > 0.11)
	{
		g_iSpacing += 1;
		g_fRate -= 0.1;
	}
	else
	{
		g_fRate = 0.1;
	}
	Invalidate();
	//g_hSetOffset -= 100;
	////g_vOffset = -100;
}

//缩小
void Cd2gView::OnSmaller()
{
	//m_bIsSmaller = TRUE;
	m_iDrawType = 0;
	m_CommandType = CT_NONE;
	if(g_iSpacing > 1)
	{
		g_fRate += 0.1;
		g_iSpacing -= 1;
	}
	Invalidate();
	//g_hSetOffset += 100;
}

//还原
void Cd2gView::OnReSet()
{
	g_fRate = 1.0;
	g_iSpacing = 5;
	g_vOffset = 0;
	g_hOffset = 0;
	g_vSetOffset = INIT_VSETOFFSET;
	g_hSetOffset = INIT_HSETOFFSET;
	m_OldHSetOffset = INIT_HSETOFFSET;
	m_OldVSetOffset = INIT_VSETOFFSET;
	Invalidate();
}

//寻找最短路径
void Cd2gView::OnBetterPath()
{
	// TODO: Add your command handler code here
	CPoint2D BasePoint;
	int bIsOptimizeAll = 0, m_nOptimizeSelected = 0;
	g_SaveArray.RemoveAll();

	if(g_GraphArray.GetSize() == 0)
		return;

	COptimizeDlg OptimizeDlg;
	if(IDOK != OptimizeDlg.DoModal())
	{
		return;
	}

	BasePoint.x = OptimizeDlg.m_BasePointX;
	BasePoint.y = OptimizeDlg.m_BasePointY;
	bIsOptimizeAll = OptimizeDlg.m_bIsOptimizeAll;
	m_nOptimizeSelected = OptimizeDlg.m_nOptimizeSelected;

	if(!bIsOptimizeAll)
		g_SaveArray.Copy(g_GraphArray);
	else
		g_SaveArray.Copy(g_OperationArray);

	funArray.DataMalloc(g_SaveArray);
	funArray.OptimizePath(g_SaveArray, BasePoint, !m_nOptimizeSelected);
	Invalidate();
}

//获取工具栏上点，直线等等控件的文本信息
BOOL Cd2gView::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
	UINT nID = pNMHDR->idFrom; //获取工具栏按钮ID
	CString strHint;

	CToolBar *pToolBar=(CToolBar *)AfxGetMainWnd()->GetDescendantWindow(AFX_IDW_TOOLBAR);
	int index = pToolBar->GetToolBarCtrl().CommandToIndex(nID);//根据ID获取按钮索引

	pToolBar->GetButtonText(index, strHint);//获取按钮文本
	_tcscpy_s(pTTT->szText, strHint.GetBuffer(strHint.GetLength()));//设置显示的提示信息
	pTTT->hinst = AfxGetResourceHandle();
	return(TRUE);
}

//设置点坐标
void Cd2gView::OnSetPoint()
{
	// TODO: Add your command handler code here
	CSetPointDlg dlg;
	CPoint2D pt;
	COLORREF clr;

	if(IDOK == dlg.DoModal())
	{
		pt.x = dlg.m_pointX;
		pt.y = dlg.m_pointY;
		clr = funArray.StringToRGB(dlg.m_pointColor);

		CGraph *qGraph = new CGraph;
		qGraph->m_iDrawType = SOLIDPOINT;
		qGraph->m_ptOrigin = pt;
		qGraph->clrREF = clr;

		g_GraphArray.Add(qGraph);
	}
	Invalidate();
	m_iDrawType = 0;
}

//设置直线的起点和终点坐标
void Cd2gView::OnSetLineStartAndEnd()
{
	// TODO: Add your command handler code here
	CSetLineDlg dlg;
	CPoint2D spt, ept;
	COLORREF clr;

	if(IDOK == dlg.DoModal())
	{
		spt.x = dlg.m_lineStartX;
		spt.y = dlg.m_lineStartY;
		ept.x = dlg.m_lineEndX;
		ept.y = dlg.m_lineEndY;

		clr = funArray.StringToRGB(dlg.m_lineColor);

		CGraph *qGraph = new CGraph;
		qGraph->m_iDrawType = LINE;
		qGraph->m_ptOrigin = spt;
		qGraph->m_ptEnd = ept;
		qGraph->clrREF = clr;
		qGraph->pt2Array = CPointArray(spt, ept);

		g_GraphArray.Add(qGraph);
	}
	Invalidate();
	m_iDrawType = 0;
}

//设置整圆圆心坐标和半径
void Cd2gView::OnSetCircleCenterAndRadius()
{
	// TODO: Add your command handler code here
	CSetCircleDlg dlg;
	CPoint2D opt, spt;
	double fRadius = 0.0;
	COLORREF clr;

	if(IDOK == dlg.DoModal())
	{
		opt.x = dlg.m_centerX;
		opt.y = dlg.m_centerY;
		fRadius = dlg.m_fCircleRadius;
		spt.x = opt.x - fRadius;
		spt.y = opt.y;

		clr = funArray.StringToRGB(dlg.m_circleColor);

		CGraph *qGraph = new CGraph;
		qGraph->m_iDrawType = CIRCLE;
		qGraph->m_ptCenter = opt;
		qGraph->m_ptOrigin = spt;
		qGraph->m_fRadius = fRadius;
		qGraph->pt2Array = CPointArray(opt, spt);
		qGraph->m_bFlags = TRUE;
		qGraph->clrREF = clr;

		g_GraphArray.Add(qGraph);
	}
	Invalidate();
	m_iDrawType = 0;
}

//设置圆弧的半径和角度
void Cd2gView::OnSetArcCenterRaidusAngle()
{
	// TODO: Add your command handler code here

	CSetArcDlg dlg;
	CPoint2D opt, spt, ept, mpt;
	double fRadius = 0.0;
	double startAngle = 0.0, endAngle = 0.0, tempAngle = 0.0, angle = 0.0, angleLength = 0.0;
	BOOL bDirection = FALSE;
	COLORREF clr;

	if(IDOK == dlg.DoModal())
	{
		opt.x = dlg.m_arcCenterX;
		opt.y = dlg.m_arcCenterY;
		bDirection = dlg.m_arcDirection ? TRUE: FALSE;
		fRadius = dlg.m_arcRadius;
		startAngle = dlg.m_arcStartAngle;
		endAngle = dlg.m_arcEndAngle;
		clr = funArray.StringToRGB(dlg.m_arcColor);

		spt.x = (opt.x + fRadius * cos(startAngle * PI / 180)) ;
		spt.y = (opt.y + fRadius * sin(startAngle * PI / 180)) ;

		ept.x = (opt.x + fRadius * cos(endAngle * PI / 180)) ;
		ept.y = (opt.y + fRadius * sin(endAngle * PI / 180)) ;

		angleLength = endAngle - startAngle;

		if(angleLength > 0)
			tempAngle = (startAngle + endAngle) * PI / 360;
		else
			tempAngle = (startAngle + endAngle - 360) * PI / 360;

		mpt.x = (opt.x + fRadius * cos(tempAngle));
		mpt.y = (opt.y + fRadius * sin(tempAngle));

		if(angleLength < 0)
			angleLength += 360;

		angle = angleLength * PI / 180;

		CGraph *qGraph = new CGraph;
		qGraph->m_iDrawType = ARC;
		qGraph->m_ptOrigin = spt;
		qGraph->m_ptEnd = ept;
		qGraph->m_ptMiddle = mpt;
		qGraph->m_ptCenter = opt;

		qGraph->m_bDirection = bDirection;
		qGraph->clrREF = clr;
		qGraph->m_fRadius = fRadius;
		qGraph->m_fAngle = angle;
		qGraph->pt2Array = CPointArray(opt, ept);
		g_GraphArray.Add(qGraph);
	}
	Invalidate();
	m_iDrawType = 0;
}

//设置矩形的左上角坐标和高，宽
void Cd2gView::OnSetRectangleLeftTopWidthAndHeight()
{
	// TODO: Add your command handler code here
	CSetRectangleDlg1 dlg;
	CPoint2D leftTop, rightBottom;
	double width = 0.0, height = 0.0;
	COLORREF clr;

	if(IDOK == dlg.DoModal())
	{
		leftTop.x = dlg.m_rectangleLeftTopX;
		leftTop.y = dlg.m_rectangleLeftTopY;

		width = dlg.m_rectangleWidth;
		height = dlg.m_rectangleHeight;

		rightBottom.x = leftTop.x + width;
		rightBottom.y = leftTop.y - height;
		clr = funArray.StringToRGB(dlg.m_rectangleColor);


		CGraph *qGraph = new CGraph;
		qGraph->m_iDrawType = RECTANGLE;
		qGraph->m_ptOrigin = leftTop;
		qGraph->m_ptEnd = rightBottom;

		qGraph->m_rectWidth = width;
		qGraph->m_rectHeight = height;
		qGraph->clrREF = clr;

		qGraph->pt2Array = CPointArray(CPoint2D(rightBottom.x, leftTop.y), leftTop);
		g_GraphArray.Add(qGraph);
	}
	Invalidate();
	m_iDrawType = 0;
}

//设置矩形的左上角和右下角坐标
void Cd2gView::OnSetRectnagleLeftTopAndRightBottom()
{
	// TODO: Add your command handler code here

	CSetRectangleDlg2 dlg;
	CPoint2D leftTop, rightBottom;
	COLORREF clr;

	if(IDOK == dlg.DoModal())
	{
		leftTop.x = dlg.m_rectangleLeftTopX;
		leftTop.y = dlg.m_rectangleLeftTopY;

		rightBottom.x = dlg.m_rectangleRightBottomX;
		rightBottom.y = dlg.m_rectangleRightBottomY;
		clr = funArray.StringToRGB(dlg.m_rectangleColor);

		CGraph *qGraph = new CGraph;
		qGraph->m_iDrawType = RECTANGLE;
		qGraph->m_ptOrigin = leftTop;
		qGraph->m_ptEnd = rightBottom;

		qGraph->m_rectWidth = rightBottom.x - leftTop.x;
		qGraph->m_rectHeight = leftTop.y - rightBottom.y;
		qGraph->clrREF = clr;

		qGraph->pt2Array = CPointArray(CPoint2D(rightBottom.x, leftTop.y), leftTop);
		g_GraphArray.Add(qGraph);
	}
	Invalidate();
	m_iDrawType = 0;
}

//反向
void Cd2gView::DirectionReserve(CGraph* pGraph)
{
	funArray.DirectionReserve(pGraph);
}

//全部图形进行修剪，把线段小的合并起来，拼接到上一段上
void Cd2gView::OnPrune()
{
	// TODO: Add your command handler code here
	funArray.Prune(g_GraphArray);
	Invalidate();
}

//全选
void Cd2gView::OnSelectAll()
{
	// TODO: Add your command handler code here
	//07-01修改Cf
	//Start
	g_OperationArray.RemoveAll();
	//End

	for (int i = 0; i < g_GraphArray.GetSize(); ++i)
	{
		((CGraph*)g_GraphArray.GetAt(i))->m_bIsPitched = TRUE;

		g_iNumArray.Add(i);
		g_OperationArray.Add((CGraph*)g_GraphArray.GetAt(i));
	}
	CString strMsg;
	strMsg.Format(L"选中了 %d 个图形", g_OperationArray.GetSize());
	SetStatusText(strMsg);
	Invalidate();
}

void Cd2gView::OnEndCommand()
{
	// TODO: Add your command handler code here
	if(CT_ADJUSTLOCATION == m_CommandType)
	{
		m_CommandType = CT_NONE;

		for(int i = 0;i < g_OperationArray.GetSize(); ++i)
			((CGraph*)g_OperationArray.GetAt(i))->m_bIsPitched = FALSE;

		m_OutPutArray[m_RealNcPathNum].Copy(g_OperationArray);

		funArray.GraphSort(m_OutPutArray[m_RealNcPathNum]);
		funArray.Prune(m_OutPutArray[m_RealNcPathNum]);
		m_RealNcPathNum++;
		g_OperationArray.RemoveAll();

		Invalidate();
	}

	else if(CT_SCREENMOVE == m_CommandType)
	{
		m_CommandType = CT_NONE;
		m_bFlag = FALSE;

		m_OldHSetOffset = g_hSetOffset;
		m_OldVSetOffset = g_vSetOffset;
		Invalidate();
	}
	SetStatusText(L"确定");
}

void Cd2gView::OnScreenMove()
{
	m_CommandType = CT_SCREENMOVE;
	SetStatusText(L"平移");
}

void Cd2gView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(m_CommandType == CT_SCREENMOVE)
	{
		//m_CommandType = CT_NONE;
		m_bFlag = FALSE;

		m_OldHSetOffset = g_hSetOffset;
		m_OldVSetOffset = g_vSetOffset;
		Invalidate();
	}

	CView::OnLButtonUp(nFlags, point);
}

void Cd2gView::OnUpdateAdjustLocation(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_iWorkStation != WATERJET)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable();
}

void Cd2gView::LoadGCodeFromNCFile(const CString strFileName)
{
	g_2PointDataVector.clear();
	g_ImitatePerPointDataVector.clear();

	m_bStartImitate = FALSE;

	CRect rt;

	LPTSTR lpsz = (LPTSTR)(LPCTSTR)strFileName;
	DWORD dwReadID = 0;
	HANDLE hReadThread = CreateThread(NULL, 0, ReadCodeThreadProc, (LPVOID)lpsz, 0, (LPDWORD)&dwReadID);

	ASSERT(hReadThread);
	CloseHandle(hReadThread);

	Sleep(100);
	EnterCriticalSection(&g_csObject);
	{
		if(g_2PointDataVector.size() > 0)
		{
			m_bStartImitate = TRUE;
			m_CommandType = CT_IMITATE;
			OnFileNew();//清除窗口上的数据
			g_DrawVector.clear();
			Invalidate();
			StartDrawBeforeOnImitate();

			g_DrawVector.assign(g_ImitatePerPointDataVector.begin(), g_ImitatePerPointDataVector.end());
			Invalidate();

			int idCommand = MessageBox(L"数据加载完毕,是否开启模拟？", L"完成", MB_OKCANCEL | MB_ICONWARNING);
			if(IDOK == idCommand)
			{
				if (m_SpeedDlg.m_hWnd)
				{
					m_SpeedDlg.SetParentWnd(this->m_hWnd);
					m_SpeedDlg.ShowWindow(SW_SHOW);
					m_SpeedDlg.GetWindowRect(&rt);
					m_SpeedDlg.MoveWindow(g_Rect.right - rt.Width() - 10, g_Rect.bottom - rt.Height() - 10, rt.Width(), rt.Height(), TRUE);
					g_DrawVector.clear();
					CurrentImitateIndex = 0;

					::SendMessage(m_SpeedDlg.m_hWnd, WM_UPDATE_PROGRESS, 0, 0);
				}
				else
				{
					m_SpeedDlg.Create(IDD_IMITATESPEED_DIALOG, this);
					m_SpeedDlg.SetParentWnd(this->m_hWnd);
					m_SpeedDlg.ShowWindow(SW_SHOW);
					m_SpeedDlg.GetWindowRect(&rt);
					m_SpeedDlg.MoveWindow(g_Rect.right - rt.Width() - 10, g_Rect.bottom - rt.Height() - 10, rt.Width(), rt.Height(), TRUE);
					g_DrawVector.clear();
					SetTimer(TIMER_IMITATE, 10, NULL);
					//m_bSetTimerFlag = TRUE;
					m_CommandType = CT_IMITATE;
				}
			}
		}
		else
		{
			MessageBox(L"数据加载失败，请查看文件是否有数据!", L"失败", MB_ICONWARNING);
		}
	}
	LeaveCriticalSection(&g_csObject);
}

void OperatorString(CString& str)
{
	str.Remove(' ');//去掉空格
	str.MakeUpper();//转成大写
}

DWORD Cd2gView::ReadCodeThreadProc(LPVOID lpParameter)
{
	EnterCriticalSection(&g_csObject);
	LPCTSTR strFileName = (LPCTSTR)lpParameter;

	CStdioFile gCodefile;
	CFileException fileExcept;
	if(! gCodefile.Open(strFileName, CFile::modeRead, &fileExcept))
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << fileExcept.m_cause << "\n";
		return 0;
#endif
	}

	g_2PointDataVector.clear();

	int iPosG, iPosX, iPosY, iPosZ;
	CPoint2D lastPoint;
	_2PointData pd;
	CString strBuffer;

	gCodefile.ReadString(strBuffer);//读取一行存入buffer
	OperatorString(strBuffer);

	while(strBuffer != "G90")
	{
		gCodefile.ReadString(strBuffer);
		OperatorString(strBuffer);
	}

	gCodefile.ReadString(strBuffer);
	OperatorString(strBuffer);
	//开始读取数据
	while(strBuffer != "M5")
	{
		iPosG = strBuffer.Find('G');
		iPosX = strBuffer.Find('X');
		iPosY = strBuffer.Find('Y');
		iPosZ = strBuffer.Find('Z');

		if(iPosX == -1 && iPosY != -1)//X未变
		{
			pd.ToPointWay = _ttoi(strBuffer.Mid(iPosG + 1, iPosY));
			pd.ePoint.x = lastPoint.x;
			pd.ePoint.y = _wtof(strBuffer.Mid(iPosY + 1, iPosZ));
		}
		else if(iPosX != -1 && iPosY == -1)
		{
			pd.ToPointWay = _ttoi(strBuffer.Mid(iPosG + 1, iPosX));
			pd.ePoint.x = _wtof(strBuffer.Mid(iPosX + 1, iPosZ));
			pd.ePoint.y = lastPoint.y;
		}
		else
		{
			pd.ToPointWay = _ttoi(strBuffer.Mid(iPosG + 1, iPosX));
			pd.ePoint.x = _wtof(strBuffer.Mid(iPosX + 1, iPosY));
			pd.ePoint.y = _wtof(strBuffer.Mid(iPosY + 1, iPosZ));
		}

		if(lastPoint != pd.ePoint)
		{
			g_2PointDataVector.push_back(pd);
			lastPoint = pd.ePoint;
		}

		gCodefile.ReadString(strBuffer);
		OperatorString(strBuffer);
	}

	gCodefile.Close();

	LeaveCriticalSection(&g_csObject);

	return 0;
}

void Cd2gView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	CRect rt;
	GetClientRect(&rt);
	g_Rect = rt;
}

void Cd2gView::StartDrawBeforeOnImitate()
{
	funArray.CalculateImitatePerPointData();
}

void Cd2gView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	_2PointData pd;

	if(m_SpeedDlg.m_ImitateStatus == START)
	{
		if(nIDEvent == TIMER_IMITATE)
		{
			for(int i = 0; i < m_SpeedDlg.m_SliderPosValue; ++i)
			{
				if(CurrentImitateIndex < g_ImitatePerPointDataVector.size())
				{	
					pd = g_ImitatePerPointDataVector[CurrentImitateIndex];
					//DrawSpindl(NULL,pd.ePoint);
					g_DrawVector.push_back(pd);
					CurrentImitateIndex++;
				}
				else if(CurrentImitateIndex == g_ImitatePerPointDataVector.size())
				{
					m_bStartImitate = FALSE;
					CurrentImitateIndex = 0;
					m_SpeedDlg.m_ImitateStatus = PAUSE;
					::SendMessage(m_SpeedDlg.m_hWnd, WM_INITDIALOG, 0, 0);
					break;
				} 

			}
			Invalidate();
		}
	}

	CView::OnTimer(nIDEvent);
}

void Cd2gView::DrawSpindl(CDC *pDC,const CPoint2D pt)
{
	POINT point[3];
	point[0].x = pt.x;
	point[0].y = pt.y;
	point[1].x = pt.x - 3;
	point[1].y = pt.y - 8;
	point[2].x = pt.x + 3;
	point[2].y = pt.y - 8;

	CClientDC dc(this);	
	CBrush brush(RGB(255, 0, 0));
	CBrush *pOldBrush;
	pOldBrush = dc.SelectObject(&brush);
	CPen pen(PS_SOLID, 1, RGB(255, 0, 0));
	CPen *pOldPen = dc.SelectObject(&pen);
	dc.Polygon(point, 3);
	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);
	brush.DeleteObject();
	pen.DeleteObject();
}


void Cd2gView::OnControlSpeed()
{
	// TODO: Add your command handler code here
	m_SpeedDlg.ShowWindow(SW_SHOWNORMAL);
}

LRESULT Cd2gView::OnLoadFile(WPARAM wParam,LPARAM lParam)
{
	LPTSTR szLoadFileName = (LPTSTR)wParam;
	CString strFile = szLoadFileName;

	LoadGCodeFromNCFile(strFile);
	return 0;
}

double Cd2gView::RadianToAngle(double Radian)
{
	double Angle;
	if(g_iCarveWay == 0)//从下至上
	{
		Angle = Radian * 180 / PI + 180;
		if(Angle >= 360)
			Angle -= 360;
	}
	else//从上至下
	{
		Angle = Radian * 180 / PI;
	}

	//Start Cf_05_30
	//return (360 - Angle);
	return Angle;
	//End Cf_05_30
}

double Cd2gView::AngleToRadian( double Angle )
{
	double Radian = Angle * PI / 180;
	return Radian;
}

void Cd2gView::OnToolManage()
{
	// TODO: Add your command handler code here
	if(tmVector.empty())
	{
		MessageBox(L"缺少刀具配置文件，请在EXE同目录下添加配置文件", L"警告", MB_ICONWARNING);
	}
	else
	{
		CToolManageDlg dlg;
		dlg.DoModal();
	}
}

void Cd2gView::OnScreenSuitable()
{
	double fRate1, fRate2;
	fRate1 = g_iLength * 1.0 / (1366 - g_hSetOffset);
	fRate2 = g_iWidth * 1.0 / (644 - g_vSetOffset);
	g_fRate = fRate1 >= fRate2 ? fRate1 : fRate2;
	int nCount = g_fRate / 0.5;
	g_fRate = 0.5 * (nCount + 1);

	//Start Cf_Lsd_2015_12_06
	//g_iSpacing += (g_fRate / 0.5 - 2);
	g_iSpacing = (g_fRate / 0.5 - 2);
	//End Cf_lsd_2015_12_06
	Invalidate();
}

void Cd2gView::OnSetNewStep()
{
	// TODO: Add your command handler code here
	CSetNewStepDlg dlg;
	double fNewStep = 0.0;
	if(IDOK == dlg.DoModal())
	{
		fNewStep = dlg.m_fNewStep;
		funArray.SetNewStepToGraph(fNewStep);
	}
}

void Cd2gView::OnCancelNewStep()
{
	// TODO: Add your command handler code here
	funArray.CancelNewStepToGraph();
}

void Cd2gView::OnSetNewFeedSpeed()
{
	// TODO: Add your command handler code here
	int iFeedSpeed;
	CSetFeedSpeedDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		iFeedSpeed = dlg.m_iNewFeedSpeed;
		funArray.SetNewFeedSpeed(iFeedSpeed);
	}
}

void Cd2gView::OnCancelNewFeedSpeed()
{
	// TODO: Add your command handler code here
	funArray.CancelFeedSpeed();
}

int Cd2gView::FetchGraphInRectToArray(CRect rect)
{
	//06-30修改Cf
	//Start
	for(int j = 0; j < g_GraphArray.GetSize(); ++j)
		((CGraph*)g_GraphArray.GetAt(j))->m_bIsPitched = FALSE;
	//End

	int iFetchGraphNum = 0;

	rect.NormalizeRect();
	CPoint2D topLeft = funArray.ScreenToMyQuadrant(rect.TopLeft()) + CPoint2D(g_hOffset, g_vOffset);;
	CPoint2D bottomRight = funArray.ScreenToMyQuadrant(rect.BottomRight()) + CPoint2D(g_hOffset, g_vOffset);;

	topLeft *= g_fRate;
	bottomRight *= g_fRate;

	CGraph *CurGraph = NULL;
	for(int i = 0; i < g_GraphArray.GetSize(); ++i)
	{
		CurGraph = (CGraph*)g_GraphArray.GetAt(i);
		if(funArray.isInRect(CRect(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y), CurGraph))
		{
			iFetchGraphNum++;
			CurGraph->m_bIsPitched = TRUE;
			g_iNumArray.Add(i);
			g_OperationArray.Add(CurGraph);
		}
	}
	Invalidate();
	return iFetchGraphNum;
}

void Cd2gView::OnShowBetterPath()
{
	// TODO: Add your command handler code here
	CClientDC dc(this);
	CPoint2D sp, ep;
	CPen pen(PS_DOT, 2, RGB(0, 255, 255)), *pOldPen;
	pOldPen = dc.SelectObject(&pen);

	INT_PTR nCount = g_SaveArray.GetSize() * 2;
	for(int i = 0; i < nCount; i += 2)
	{
		sp = g_BetterPathPoint[i];
		ep = g_BetterPathPoint[i + 1];

		sp /= g_fRate;
		ep /= g_fRate;
		sp = funArray.MyQuadrantToScreen(sp) + CPoint2D(-g_hOffset, g_vOffset);
		ep = funArray.MyQuadrantToScreen(ep) + CPoint2D(-g_hOffset, g_vOffset);
		dc.MoveTo(sp.x, sp.y);
		dc.LineTo(ep.x, ep.y);
	}

	dc.SelectObject(pOldPen);
}

void Cd2gView::OnModifyPointDepth()
{
	// TODO: Add your command handler code here
	CSetPointDepthDlg dlg;
	if(IDOK == dlg.DoModal())
	{
		for(int i = 0; i < g_OperationArray.GetSize(); ++i)
		{
			if(((CGraph*)g_OperationArray.GetAt(i))->m_iDrawType == 9)
			{
				((CGraph*)g_OperationArray.GetAt(i))->m_fPointDepth = dlg.m_fPointDepth;
				((CGraph*)g_OperationArray.GetAt(i))->m_bSetPointDepth = TRUE;
			}
		}
	}
}

void Cd2gView::OnCancelPointDepth()
{
	// TODO: Add your command handler code here
	for(int i = 0; i < g_OperationArray.GetSize(); ++i)
	{
		if(((CGraph*)g_OperationArray.GetAt(i))->m_iDrawType == 9)
		{
			((CGraph*)g_OperationArray.GetAt(i))->m_bSetPointDepth = FALSE;
			((CGraph*)g_OperationArray.GetAt(i))->m_bIsPitched = FALSE;
		}
	}
}

void Cd2gView::OperatorGroup(CPtrArray& arr)
{
	CGroupGraph groupGraph;
	g_VectorGroupGraph.clear();

	CPoint2D CurStartPt, CurEndPt, NextStartPt, NextEndPt, CurCenterPt, NextCenterPt;
	CPoint2D CurAfterStartPt, CurBeforeEndPt, NextAfterStartPt, NextBeforeEndPt;
	int iCurIndex, iNextIndex, CurDrawType, NextDrawType;
	double CurStartAngle = 0.0, CurEndAngle = 0.0, NextStartAngle = 0.0, NextEndAngle = 0.0;
	double fRadius = 0.0, fAngle = 0.0;
	BOOL bDirection, bConnection = FALSE;;
	CBspline Bspline;
	int nPtNumber;
	vector<vec2> NodeArray;
	INT_PTR iMaxSize = arr.GetSize();
	double SaveStartAngle = 0.0;

	if(iMaxSize == 1)
	{
		groupGraph.m_GraphIndex.push_back(g_idNo[0]);
		g_VectorGroupGraph.push_back(groupGraph);
		return;
	}

	for(int i = 0; i < iMaxSize - 1; )
	{
		bConnection = FALSE;
		for(int j = i + 1; j < iMaxSize; ++j)
		{
			iCurIndex = g_idNo[j - 1];
			CurDrawType = ((CGraph*)arr.GetAt(iCurIndex))->m_iDrawType;
			CurStartPt = ((CGraph*)arr.GetAt(iCurIndex))->m_ptOrigin;
			CurEndPt = ((CGraph*)arr.GetAt(iCurIndex))->m_ptEnd;

			switch(CurDrawType)
			{
			case 2:
				CurStartAngle = CurEndAngle = funArray.GetAngle(CurStartPt.x, CurStartPt.y,
					CurEndPt.x, CurEndPt.y);
				break;

			case 3:
				fRadius = ((CGraph*)arr.GetAt(iCurIndex))->m_fRadius;
				bDirection = ((CGraph*)arr.GetAt(iCurIndex))->m_bDirection;
				CurCenterPt = ((CGraph*)arr.GetAt(iCurIndex))->m_ptCenter;

				CurStartAngle = 3 * PI / 2.0;
				CurEndAngle = CurStartAngle;
				break;

			case 4:
				fRadius = ((CGraph*)arr.GetAt(iCurIndex))->m_fRadius;
				bDirection = ((CGraph*)arr.GetAt(iCurIndex))->m_bDirection;
				fAngle = ((CGraph*)arr.GetAt(iCurIndex))->m_fAngle;
				CurCenterPt = funArray.GetCenterPoint(CurStartPt.x, CurStartPt.y,
					CurEndPt.x, CurEndPt.y, fRadius, fAngle, bDirection);

				CurStartAngle = funArray.GetAngle(CurCenterPt.x, CurCenterPt.y, CurStartPt.x, CurStartPt.y);
				CurEndAngle = funArray.GetAngle(CurCenterPt.x, CurCenterPt.y, CurEndPt.x, CurEndPt.y);

				if(bDirection)
				{
					CurStartAngle -= PI / 2.0;
					CurEndAngle -= PI / 2.0;

					if(CurStartAngle < 0.0)
						CurStartAngle += PI * 2;
					if(CurEndAngle < 0.0)
						CurEndAngle += PI * 2;

				}
				else
				{
					CurStartAngle += PI / 2.0;
					CurEndAngle += PI / 2.0;

					if(CurStartAngle > 2 * PI)
						CurStartAngle -= PI * 2;
					if(CurEndAngle  > 2 * PI)
						CurEndAngle -= PI * 2;
				}
				break;

			case 8:
				NodeArray.clear();
				nPtNumber = ((CGraph*)g_SaveArray.GetAt(iCurIndex))->m_iVertexNum;
				NodeArray = vector<vec2>(((CGraph*)g_SaveArray.GetAt(iCurIndex))->controlNode.begin(), 
					((CGraph*)g_SaveArray.GetAt(iCurIndex))->controlNode.end());
				Bspline = CBspline(NodeArray);
				Bspline.ComputeInterpolation();
				CurAfterStartPt.x = Bspline.m_interpolationNode[1][0];
				CurAfterStartPt.y = Bspline.m_interpolationNode[1][1];
				CurBeforeEndPt.x = Bspline.m_interpolationNode[MAX_LENGTH - 2][0];
				CurBeforeEndPt.y = Bspline.m_interpolationNode[MAX_LENGTH - 2][1];

				//06-30修改Cf
				//Start
				//CurStartAngle = funArray.GetAngle(CurStartPt.x, CurStartPt.y, CurAfterStartPt.x, CurAfterStartPt.y);
				//CurEndAngle = funArray.GetAngle(CurBeforeEndPt.x, CurBeforeEndPt.y, CurEndPt.x, CurEndPt.y);
				//End
				break;

			case 9:
				break;
			}

			if(CurDrawType == 9 || CurDrawType == 8)
			{
				groupGraph.m_GraphIndex.push_back(iCurIndex);
				g_VectorGroupGraph.push_back(groupGraph);
				groupGraph.m_GraphIndex.clear();

				//groupGraph.m_GraphIndex.push_back(g_idNo[j]);
				i++;
				break;
			}
			else
			{
				if(!bConnection)
				{
					SaveStartAngle = CurStartAngle;
					groupGraph.m_GraphIndex.push_back(iCurIndex);
				}
				else
				{

				}
			}


			/************************************************************************/
			iNextIndex = g_idNo[j];
			NextDrawType = ((CGraph*)arr.GetAt(iNextIndex))->m_iDrawType;
			NextStartPt = ((CGraph*)arr.GetAt(iNextIndex))->m_ptOrigin;
			NextEndPt = ((CGraph*)arr.GetAt(iNextIndex))->m_ptEnd;

			switch(NextDrawType)
			{
			case 2:
				NextStartAngle = NextEndAngle = funArray.GetAngle(NextStartPt.x, NextStartPt.y,
					NextEndPt.x, NextEndPt.y);
				break;

			case 3:
				fRadius = ((CGraph*)arr.GetAt(iNextIndex))->m_fRadius;
				bDirection = ((CGraph*)arr.GetAt(iNextIndex))->m_bDirection;
				CurCenterPt = ((CGraph*)arr.GetAt(iNextIndex))->m_ptCenter;

				NextStartAngle = 3 * PI / 2.0;
				NextEndAngle = NextStartAngle;
				break;

			case 4:
				fRadius = ((CGraph*)arr.GetAt(iNextIndex))->m_fRadius;
				bDirection = ((CGraph*)arr.GetAt(iNextIndex))->m_bDirection;
				fAngle = ((CGraph*)arr.GetAt(iNextIndex))->m_fAngle;
				NextCenterPt = funArray.GetCenterPoint(NextStartPt.x, NextStartPt.y,
					NextEndPt.x, NextEndPt.y, fRadius, fAngle, bDirection);

				NextStartAngle = funArray.GetAngle(NextCenterPt.x, NextCenterPt.y, NextStartPt.x, NextStartPt.y);
				NextEndAngle = funArray.GetAngle(NextCenterPt.x, NextCenterPt.y, NextEndPt.x, NextEndPt.y);

				if(bDirection)
				{
					NextStartAngle -= PI / 2.0;
					NextEndAngle -= PI / 2.0;

					if(NextStartAngle < 0.0)
						NextStartAngle += PI * 2;
					if(NextEndAngle < 0.0)
						NextEndAngle += PI * 2;
				}
				else
				{
					NextStartAngle += PI / 2.0;
					NextEndAngle += PI / 2.0;

					if(NextStartAngle > 2 * PI)
						NextStartAngle -= PI * 2;
					if(NextEndAngle > 2 * PI)
						NextEndAngle -= PI * 2;
				}
				break;

			case 8:
				NodeArray.clear();
				nPtNumber = ((CGraph*)g_SaveArray.GetAt(iNextIndex))->m_iVertexNum;
				NodeArray = vector<vec2>(((CGraph*)g_SaveArray.GetAt(iNextIndex))->controlNode.begin(), 
					((CGraph*)g_SaveArray.GetAt(iNextIndex))->controlNode.end());
				Bspline = CBspline(NodeArray);
				Bspline.ComputeInterpolation();
				NextAfterStartPt.x = Bspline.m_interpolationNode[1][0];
				NextAfterStartPt.y = Bspline.m_interpolationNode[1][1];
				NextBeforeEndPt.x = Bspline.m_interpolationNode[MAX_LENGTH - 2][0];
				NextBeforeEndPt.y = Bspline.m_interpolationNode[MAX_LENGTH - 2][1];

				//06-30修改Cf
				//Start
				//NextStartAngle = funArray.GetAngle(NextStartPt.x, NextStartPt.y, NextAfterStartPt.x, NextAfterStartPt.y);
				//NextEndAngle = funArray.GetAngle(NextBeforeEndPt.x, NextBeforeEndPt.y, NextEndPt.x, NextEndPt.y);
				//End
				break;

			case 9:
				break;
			}

			if(NextDrawType == 9 || NextDrawType == 8)
			{
				groupGraph.m_GraphIndex.push_back(iCurIndex);
				g_VectorGroupGraph.push_back(groupGraph);
				groupGraph.m_GraphIndex.clear();
				groupGraph.m_GraphIndex.push_back(iNextIndex);
				g_VectorGroupGraph.push_back(groupGraph);
				groupGraph.m_GraphIndex.clear();
				++i;
				break;
			}
			else
			{
				if(fabs(CurEndPt.x - NextStartPt.x) < 1 && fabs(CurEndPt.y - NextStartPt.y) < 1)
				{
					if(fabs(CurEndAngle - NextStartAngle) < g_fDeltaC1 
						|| fabs(CurEndAngle - NextStartAngle) > 2 * PI - g_fDeltaC1)
					{
						if(j == iMaxSize - 1)
							goto Label1;

						bConnection = TRUE;
						groupGraph.m_GraphIndex.push_back(iNextIndex);
						continue;
					}
					else
					{
						g_VectorGroupGraph.push_back(groupGraph);
						groupGraph.m_GraphIndex.clear();

						i = j;
						break;
					}
				}
				else
				{
					//groupGraph.m_GraphIndex.push_back(iCurIndex);
					g_VectorGroupGraph.push_back(groupGraph);
					groupGraph.m_GraphIndex.clear();
					//groupGraph.m_GraphIndex.push_back(iNextIndex);

					i = j;
					break;
				}
			}
		}
	}

Label1:
	groupGraph.m_GraphIndex.push_back(g_idNo[iMaxSize - 1]);
	g_VectorGroupGraph.push_back(groupGraph);
	groupGraph.m_GraphIndex.clear();
}

double Cd2gView::FindSuitablePerStep( double lStep, double rStep, double fRadius, double fOffsetC, double fDelteL, double fGraphLength )
{
	double fTheoryL = 0.0;
	int nCount;
	double fMidStep;
	int n = 0;
	while(n < 100)
	{
		fMidStep = (lStep + rStep) / 2.0;
		nCount = floor(fGraphLength / fMidStep);
		if(fRadius != -1)
		{
			fTheoryL = sqrt(
				pow(180 * fMidStep / (fRadius * PI) + fOffsetC * 180 / (PI * nCount), 2.0)
				+	pow(fMidStep, 2.0)
				);
		}
		else
		{
			fTheoryL = sqrt(
				pow(fOffsetC * 180 / PI / nCount, 2.0)
				+	pow(fMidStep, 2.0)
				);
		}

		if(fabs(fTheoryL - fDelteL) < DEVIATION)
		{
			return fMidStep;
		}
		else if(fTheoryL - fDelteL >= DEVIATION)
		{
			rStep = fMidStep;
		}
		else
		{
			lStep = fMidStep;
		}
		n++;
	}

	return fMidStep;
}

void Cd2gView::SetStatusText( CString strMsg )
{
	GetParent()->GetDescendantWindow(AFX_IDW_STATUS_BAR)->SetWindowText(strMsg);
}