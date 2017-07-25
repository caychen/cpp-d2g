#pragma once
#include "atltypes.h"
#include "Vec.h"
#include <vector>

#include "GlobalDataType.h"
//#include "tooltype.h"
//#include "PointArray.h"
//#include "Point2D.h"
#include <map>

class CGraph
{
public:
	CGraph(void)
		: m_iDrawType(0)
		, m_ptOrigin(0,0)
		, m_ptMiddle(0,0)
		, m_ptEnd(0,0)
		, m_ptCenter(0,0)
		, m_bDirection(FALSE)
		, m_fRadius(0)
		, m_fDegree(0)
		, m_fAngle(0)
		, m_fMajor(0)
		, m_fMinor(0)
		, m_fStartAngle(0)
		, m_fEndAngle(0)
		, m_bIsPitched(FALSE)
		, m_bIsShowArrow(FALSE)
		, m_bFlags(FALSE)
		, m_iVertexNum(0)
		, clrREF(RGB(0,0,0))
		, m_rectWidth(0)
		, m_rectHeight(0)
		, m_PolyColorType(NULL)
		, m_SelectedNum(-1)
		, m_fPerStep(0.0)
		, m_bSetPerStep(FALSE)
		, m_iPerFeedSpeed(0)
		, m_bSetFeedSpeed(FALSE)
		, m_fPointDepth(0.0)
		, m_bSetPointDepth(FALSE)
		, m_bIsPolyLine(FALSE)
		, m_IndexOfPolyLine(-1)
		, m_IndexPerPolyLine(-1)
		, m_nCount(0)
	{
		
	}

	////点9
	//CGraph(UINT m_iDrawType,CPoint2D m_ptOrigin,COLORREF g_clr,BOOL m_bIsPitched = FALSE,
	//	UINT toolNum = 0,Tooltype* Tt = NULL);
	//
	////直线2
	//CGraph(UINT m_iDrawType,CPoint2D m_ptOrigin,CPoint2D m_ptEnd,COLORREF color,
	//	CPointArray pt2Array,BOOL m_bIsPitched = FALSE,BOOL m_bIsShowArrow = FALSE,
	//	int graphSpeed = 0,int graphDownWay = 0,double graphControlC = 0.0,int ColorType = 0,
	//	UINT toolNum = 0,Tooltype* Tt = NULL);

	////整圆3
	//CGraph(UINT m_iDrawType,CPoint2D m_ptOrigin,CPoint2D m_ptEnd,COLORREF color,double m_fRadius,BOOL m_bDirection,
	//	CPointArray pt2Array,BOOL m_bIsPitched = FALSE,BOOL m_bIsShowArrow = FALSE,
	//	int graphSpeed = 0,int graphDownWay = 0,double graphControlC = 0.0,int ColorType = 0,
	//	UINT toolNum = 0,Tooltype* Tt = NULL);

	////圆弧4
	//CGraph(UINT m_iDrawType,CPoint2D m_ptOrigin,CPoint2D m_ptMiddle,CPoint2D m_ptEnd,CPoint2D m_ptCenter,
	//	BOOL m_bDirection,COLORREF color,double m_fRadius,double m_fAngle,CPointArray pt2Array,
	//	BOOL m_bIsPitched = FALSE,BOOL m_bIsShowArrow = FALSE,
	//	int graphSpeed = 0,int graphDownWay = 0,double graphControlC = 0.0,int ColorType = 0,
	//	UINT toolNum = 0,Tooltype* Tt = NULL);

	////椭圆6
	//CGraph(UINT m_iDrawType,double m_fMajor,double m_fMinor,double m_fDegree,CPoint2D m_ptCenter,CPoint2D m_ptOrigin,
	//	double m_fStartAngle,double m_fEndAngle,COLORREF color,BOOL m_bDirection,CPointArray pt2Array,
	//	BOOL m_bIsPitched = FALSE,BOOL m_bIsShowArrow = FALSE,
	//	int graphSpeed = 0,int graphDownWay = 0,double graphControlC = 0.0,int ColorType = 0,
	//	UINT toolNum = 0,Tooltype* Tt = NULL);

	////多段线7
	//CGraph(UINT m_iDrawType,UINT m_iVertexNum,COLORREF g_clr,UINT m_bFlags,CArray<CPoint2D>pm,CPoint2D m_ptOrigin,CPointArray pt2Array,
	//	BOOL m_bIsPitched = FALSE,BOOL m_bIsShowArrow = FALSE,
	//	int graphSpeed = 0,int graphDownWay = 0,double graphControlC = 0.0,int *pPolyColorType = 0,
	//	UINT toolNum = 0,Tooltype* Tt = NULL);

	////样条曲线8
	//CGraph(UINT m_iDrawType,UINT m_iVertexNum,COLORREF g_clr,CPoint2D m_ptOrigin,
	//	vector<vec2>controlNode,CPointArray pt2Array,
	//	BOOL m_bIsPitched = FALSE,BOOL m_bIsShowArrow = FALSE,
	//	int graphSpeed = 0,int graphDownWay = 0,double graphControlC = 0.0,int ColorType = 0,
	//	UINT toolNum = 0,Tooltype* Tt = NULL);
public:
	virtual ~CGraph(void){}

	void AssignGraph(CGraph *SrcGraph)
	{
		this->m_iDrawType = SrcGraph->m_iDrawType;
		this->m_ptOrigin = SrcGraph->m_ptOrigin;
		this->m_ptMiddle = SrcGraph->m_ptMiddle;
		this->m_ptEnd = SrcGraph->m_ptEnd;
		this->m_ptCenter = SrcGraph->m_ptCenter;
		this->m_bDirection = SrcGraph->m_bDirection;
		this->m_fRadius = SrcGraph->m_fRadius;
		this->m_fDegree = SrcGraph->m_fDegree;
		this->clrREF = SrcGraph->clrREF;
		this->m_rectWidth = SrcGraph->m_rectWidth;
		this->m_rectHeight = SrcGraph->m_rectHeight;
		this->m_fAngle = SrcGraph->m_fAngle;
		this->m_fMajor = SrcGraph->m_fMajor;
		this->m_fMinor = SrcGraph->m_fMinor;
		this->pt2Array = SrcGraph->pt2Array;
		this->m_fStartAngle = SrcGraph->m_fStartAngle;
		this->m_fEndAngle = SrcGraph->m_fEndAngle;
		this->m_bIsShowArrow = SrcGraph->m_bIsShowArrow;
		this->m_bFlags = SrcGraph->m_bFlags;
		this->m_iVertexNum = SrcGraph->m_iVertexNum;
		this->m_bIsPitched = FALSE;//SrcGraph->m_bIsPitched;
		this->m_PeakGroup.Copy(SrcGraph->m_PeakGroup);
		this->controlNode = SrcGraph->controlNode;

		this->m_fPerStep = SrcGraph->m_fPerStep;
		this->m_bSetPerStep = SrcGraph->m_bSetPerStep;
		this->m_iPerFeedSpeed = SrcGraph->m_iPerFeedSpeed;
		this->m_bSetFeedSpeed = SrcGraph->m_bSetFeedSpeed;
		this->m_fPointDepth = SrcGraph->m_fPointDepth;
		this->m_bSetPointDepth = SrcGraph->m_bSetPointDepth;
		this->m_bIsPolyLine = SrcGraph->m_bIsPolyLine;
		this->m_IndexOfPolyLine = SrcGraph->m_IndexOfPolyLine;
		this->m_IndexPerPolyLine = SrcGraph->m_IndexPerPolyLine;
		this->m_nCount = SrcGraph->m_nCount;
		this->m_SelectedNum = SrcGraph->m_SelectedNum;
		this->m_PolyColorType = SrcGraph->m_PolyColorType;
	}

	UINT m_iDrawType;//类型
	CPoint2D m_ptOrigin;//起点
	CPoint2D m_ptMiddle;//圆弧过渡中点
	CPoint2D m_ptEnd;//终点
	CPoint2D m_ptCenter;//圆心坐标
	BOOL m_bDirection;//方向
	double m_fRadius;//半径
	double m_fDegree;//弧度
	COLORREF clrREF;//颜色
	double m_rectWidth;//矩形宽
	double m_rectHeight;//矩形高
	double m_fAngle;//
	double m_fMajor;//长轴
	double m_fMinor;//短轴
	CPointArray pt2Array;//终点前箭头两点
	double m_fStartAngle;//椭圆弧起点弧度
	double m_fEndAngle;//椭圆弧终点弧度
	
	BOOL m_bIsShowArrow;//是否显示箭头
	UINT m_bFlags;//多段线的闭合标记
	UINT m_iVertexNum;//多段线的顶点数，样条的控制点数
	BOOL m_bIsPitched;//是否被选中
	CArray<CPoint2D> m_PeakGroup;//多段线顶点坐标
	vector<vec2> controlNode;//样条控制点坐标数组

	/************************************************************************/
	double m_fPerStep;
	BOOL m_bSetPerStep;
	int m_iPerFeedSpeed;
	BOOL m_bSetFeedSpeed;

	double m_fPointDepth;
	BOOL m_bSetPointDepth;

	BOOL m_bIsPolyLine;//是否为多段线
	int m_IndexOfPolyLine;//第n个多段线
	int m_IndexPerPolyLine;//第n个多段线中的第m条线段
	int m_nCount;
	/************************************************************************/
	
	int m_SelectedNum;
	int *m_PolyColorType;//多段线每段颜色的值（RED=1,GREEN=2,BLUE=3）
};