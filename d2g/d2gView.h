// d2gView.h : Cd2gView 类的接口
//


#pragma once

#include "atltypes.h"
#include "afxwin.h"
#include "afxext.h"
#include "afxcoll.h"
#include "Graph.h"
#include <GdiPlus.h>
#include <shlwapi.h>
#include "ParameterDialog.h"
#include "FunctionArray.h"
#include "md5.h"
#include "Ruler.h"
#include "Bspline.h"
#include "PreSaveDlg.h"
#include "SetPointDlg.h"
#include "SetLineDlg.h"
#include "SetCircleDlg.h"
#include "SetArcDlg.h"
#include "SetRectangleDlg1.h"
#include "SetRectangleDlg2.h"
#include <list>
#include "OptimizeDlg.h"
#include "ImitateSpeedDlg.h"
#include "ToolManageDlg.h"
#include "SetNewStepDlg.h"
#include "SetFeedSpeedDlg.h"
#include "SetPointDepthDlg.h"

#include "GetHDSerial.h"
#include "afxtempl.h"
#pragma comment(lib, "HDlib.lib")

#include "GlobalDataType.h"
//#include "tooltype.h"
//#include "Point2DType.h"
//#include "CutGraph.h"
//#include "PointArray.h"
//#include "Point2D.h"

#define		MAX_NC_LEN		1024
#define		MAX_NC_PATH		1024

#define		PERCENT			1
#define		DEVIATION		1e-2

struct RegisterInfomation
{
	CString idString;
	CString amount;
};

typedef struct  
{
	int NCDxfIndex[MAX_NC_LEN];
	int realGetNCNum;
}PathSearch;


class Cd2gView : public CView
{
protected: // 仅从序列化创建
	Cd2gView();
	DECLARE_DYNCREATE(Cd2gView)

	// 属性
public:
	Cd2gDoc* GetDocument() const;

	// 操作
public:


	// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);


	// 实现
public:
	virtual ~Cd2gView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);  //键盘事件
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);//滚轮事件
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnUpdateLink(CCmdUI *pCmdUI);
	afx_msg void OnUpdateUnlink(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAdjustLocation(CCmdUI *pCmdUI);

	afx_msg void OnFileNew();
	afx_msg void OnFileSave();
	afx_msg void OnFileOpen();						//打开文件
	afx_msg void OnFileSaveAs();					//另存为

	afx_msg void OnSelect();
	afx_msg void OnDrawLine();
	afx_msg void OnDrawCirCle();
	afx_msg void OnDrawArc();
	afx_msg void OnDrawSpline();
	afx_msg void OnDrawEllipse();
	afx_msg void OnDrawPoint();
	afx_msg void OnDrawRectangle();
	afx_msg void OnReSet();

	afx_msg void OnBigger();
	afx_msg void OnSmaller();
	afx_msg void OnScreenMove();
	afx_msg void OnScreenSuitable();
	afx_msg void OnRectangleSize();					//设置矩形大小

	afx_msg void OnRotate();						//旋转操作
	afx_msg void OnMove();							//平移操作
	afx_msg void OnDelete();						//删除操作
	afx_msg void OnLink();							//图形连接
	afx_msg void OnUnlink();						//图形断开
	afx_msg void OnCopy();							//复制操作
	afx_msg void OnSaveSetting();					//参数设置
	afx_msg void OnShowDirection();					//显示箭头
	afx_msg void OnReserveDirection();				//箭头方向
	afx_msg void OnUnShowDirection();						//取消显示箭头
	afx_msg void OnRefresh();
	
	afx_msg void OnBetterPath();

	afx_msg void OnSetPoint();
	afx_msg void OnSetLineStartAndEnd();
	afx_msg void OnSetCircleCenterAndRadius();
	afx_msg void OnSetArcCenterRaidusAngle();
	afx_msg void OnSetRectangleLeftTopWidthAndHeight();
	afx_msg void OnSetRectnagleLeftTopAndRightBottom();
	afx_msg void OnPrune();
	afx_msg void OnBothGrow();
	afx_msg void OnFrontGrow();
	afx_msg void OnEndingGrow();
	afx_msg void OnCancelGrow();
	afx_msg void OnMerge();
	afx_msg void OnToolManage();
	afx_msg void OnSelectAll();
	afx_msg void OnEndCommand();
	afx_msg void OnControlSpeed();
	afx_msg void OnSetNewStep();
	afx_msg void OnCancelNewStep();
	afx_msg void OnSetNewFeedSpeed();
	afx_msg void OnCancelNewFeedSpeed();
	afx_msg void OnShowBetterPath();
	afx_msg void OnModifyPointDepth();
	afx_msg void OnCancelPointDepth();

	//自定义消息函数
public:
	afx_msg LRESULT OnLoadFile(WPARAM wParam, LPARAM lParam);
	
private:
	HCURSOR m_HCross;								//鼠标形状
	int m_iDrawType;								//图形类型
	CPoint m_startPt;								//起点
	CPoint m_oldPt;									//画图形时的瞬时点
	BOOL m_bFlag;									//是否开始绘制
	CPoint m_savePoint;								//用来保存画圆弧时第一次弹起时的坐标
	CPoint m_lastPt;								//终点
										
	CString color;									//颜色
	double m_fCircleRadius;							//圆(弧)半径
	CString m_filePathName;							//文件路径
	CString m_fileName;								//文件名
	BOOL m_bIsOrder;									//
	BOOL m_bIsLink;									//是否图形是断开的
	CPoint rectStartPoint;							//画矩形框的起点
	CPoint rectEndPoint;							//画矩形框的终点
	CPoint2D m_OperationPoint;						//保存右击时的坐标
	BOOL m_bIsOnMove;								//是否处于平移操作
	CPoint2D m_movePoint;								//图形平移时的临时坐标
	BOOL m_bIsOnRotate;								//是否处于旋转操作
	CPoint downPoint;
	CString m_savePath;								//保存路径
	CString m_DefaultFileName;						//默认文件名
	CFunctionArray funArray;							//函数类
	CGraph *pGraph;									//图形类				
	CGraph *qGraph;	
	CGetHDSerial m_serial;							//序列号
	MD5 *md5;										//MD5算法实例
	CRuler m_Ruler;									//标尺
	CSize m_TotalSize;
	CPoint m_centerPoint;
	CPoint m_firstPoint;
	CPoint m_secondPoint;
	double m_fMajorLength;
	double m_fMinorLength;
	double m_fAngle;
	int m_iVirtualRectCount;
	int m_iRectCount;
	int m_iLineCount;
	int m_iCircleCount;
	UINT m_iArcCount;
	int m_iEllipseCount;
	CPointArray pt2Array;//两个元素的point数组
	CPointArray pt3Array;//三个元素的point数组
	CPtrArray m_PolyLineArray;
	int m_iMoveCount;
	int m_nVSize;
	int m_nHSize;
	int m_iWorkStation;
	CString m_strTool;
	CMenu menu, *pSubMenu;
	double m_fPointRadius;
	int m_polySelectNum;

	int *nuArray;
	enum CommandType m_CommandType;
	//double m_basePoint_X;
	//double m_basePoint_Y;
	CPoint2D m_downPoint;
	CPoint2D m_oldMovePoint;
	CPtrArray m_OutPutArray[MAX_NC_PATH];
	int m_RealNcPathNum;
	CPoint2D m_screenMoveStartPoint;

	CBrush *pAllBrush, *oldPBrush;
	CPen *oldPen, *pen1, *pen2, *selectPen;
	int m_OldHSetOffset;
	int m_OldVSetOffset;
	CString m_strSaveSettingPath;
	BOOL m_bStartImitate;//是否开始模拟
	CImitateSpeedDlg m_SpeedDlg;
	BOOL m_bSetTimerFlag;

	
private:
	CString GetSavePath(CString strExePath);
	void OnUndo(void);								//撤销
	void DrawPoint(CDC* pDC, const CPoint2D point);
	void DrawFrame(CDC *, CGraph *pGraph);

	void DirectionReserve(CGraph* pGraph);
	
	void Prune(CPtrArray&);	
	void LoadGCodeFromNCFile(const CString strFileName);
	void ClearData(void);

	void StartDrawBeforeOnImitate();
	void DrawSpindl(CDC *pDC, const CPoint2D);

	double RadianToAngle(double Radian);
	double AngleToRadian(double Angle);
	int FetchGraphInRectToArray(CRect rect);
	void OperatorGroup(CPtrArray &);
	double FindSuitablePerStep(double lStep, double rStep, double fRadius, double fOffsetC, double fDelteL, double fGraphLength);
	void SetStatusText(CString strMsg);

public:
	static DWORD WINAPI ReadCodeThreadProc(LPVOID lpParameter);	

	afx_msg void OnStick();
};


#ifndef _DEBUG  // d2gView.cpp 中的调试版本
inline Cd2gDoc* Cd2gView::GetDocument() const
{ return reinterpret_cast<Cd2gDoc*>(m_pDocument); }
#endif