// d2gView.h : Cd2gView ��Ľӿ�
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
protected: // �������л�����
	Cd2gView();
	DECLARE_DYNCREATE(Cd2gView)

	// ����
public:
	Cd2gDoc* GetDocument() const;

	// ����
public:


	// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);


	// ʵ��
public:
	virtual ~Cd2gView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);  //�����¼�
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);//�����¼�
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnUpdateLink(CCmdUI *pCmdUI);
	afx_msg void OnUpdateUnlink(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAdjustLocation(CCmdUI *pCmdUI);

	afx_msg void OnFileNew();
	afx_msg void OnFileSave();
	afx_msg void OnFileOpen();						//���ļ�
	afx_msg void OnFileSaveAs();					//���Ϊ

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
	afx_msg void OnRectangleSize();					//���þ��δ�С

	afx_msg void OnRotate();						//��ת����
	afx_msg void OnMove();							//ƽ�Ʋ���
	afx_msg void OnDelete();						//ɾ������
	afx_msg void OnLink();							//ͼ������
	afx_msg void OnUnlink();						//ͼ�ζϿ�
	afx_msg void OnCopy();							//���Ʋ���
	afx_msg void OnSaveSetting();					//��������
	afx_msg void OnShowDirection();					//��ʾ��ͷ
	afx_msg void OnReserveDirection();				//��ͷ����
	afx_msg void OnUnShowDirection();						//ȡ����ʾ��ͷ
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

	//�Զ�����Ϣ����
public:
	afx_msg LRESULT OnLoadFile(WPARAM wParam, LPARAM lParam);
	
private:
	HCURSOR m_HCross;								//�����״
	int m_iDrawType;								//ͼ������
	CPoint m_startPt;								//���
	CPoint m_oldPt;									//��ͼ��ʱ��˲ʱ��
	BOOL m_bFlag;									//�Ƿ�ʼ����
	CPoint m_savePoint;								//�������滭Բ��ʱ��һ�ε���ʱ������
	CPoint m_lastPt;								//�յ�
										
	CString color;									//��ɫ
	double m_fCircleRadius;							//Բ(��)�뾶
	CString m_filePathName;							//�ļ�·��
	CString m_fileName;								//�ļ���
	BOOL m_bIsOrder;									//
	BOOL m_bIsLink;									//�Ƿ�ͼ���ǶϿ���
	CPoint rectStartPoint;							//�����ο�����
	CPoint rectEndPoint;							//�����ο���յ�
	CPoint2D m_OperationPoint;						//�����һ�ʱ������
	BOOL m_bIsOnMove;								//�Ƿ���ƽ�Ʋ���
	CPoint2D m_movePoint;								//ͼ��ƽ��ʱ����ʱ����
	BOOL m_bIsOnRotate;								//�Ƿ�����ת����
	CPoint downPoint;
	CString m_savePath;								//����·��
	CString m_DefaultFileName;						//Ĭ���ļ���
	CFunctionArray funArray;							//������
	CGraph *pGraph;									//ͼ����				
	CGraph *qGraph;	
	CGetHDSerial m_serial;							//���к�
	MD5 *md5;										//MD5�㷨ʵ��
	CRuler m_Ruler;									//���
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
	CPointArray pt2Array;//����Ԫ�ص�point����
	CPointArray pt3Array;//����Ԫ�ص�point����
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
	BOOL m_bStartImitate;//�Ƿ�ʼģ��
	CImitateSpeedDlg m_SpeedDlg;
	BOOL m_bSetTimerFlag;

	
private:
	CString GetSavePath(CString strExePath);
	void OnUndo(void);								//����
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


#ifndef _DEBUG  // d2gView.cpp �еĵ��԰汾
inline Cd2gDoc* Cd2gView::GetDocument() const
{ return reinterpret_cast<Cd2gDoc*>(m_pDocument); }
#endif