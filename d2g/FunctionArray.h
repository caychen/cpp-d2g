#pragma once

#include "vec.h"
#include <vector>
#include "Bspline.h"
#include "Graph.h"

#include "GlobalDataType.h"

//#include   <httpext.h>   
#include   <windef.h>   
#include   <Nb30.h>

#pragma comment(lib, "Netapi32.lib")



#define		POINTMAXDIS		0.1
#define		GRAPHMAXLEN		0.3

class CFunctionArray
{
public:
	CFunctionArray(void);
public:
	virtual ~CFunctionArray(void);

	double GetAngle(CPoint2D first, CPoint2D end, double radius);
	double GetAngle(double firstX, double firstY, double endX, double endY);

	double GetDistance(CPoint2D first, CPoint2D end);
	double GetDistance(double x1, double y1, double x2, double y2);
		
	double GetC(int type, double startX, double startY, double endX, double endY, 
		BOOL direction = FALSE);
	
	COLORREF StringToRGB(CString colorString);
	double SetPolyDownDepth(size_t);
	double SetPolyDownDepth1(size_t);
	double SetPolyUpDepth(size_t, int);
	double SetPolyUpDepth1(size_t, int);

	//单独曲线的起点与终点高度
	double SetDepth1(double fDepth, size_t count, int i, int j);
	double SetDepth2(UINT iDrawType, double fCoincide_Len, double fDepth, double fCurveStep_Len, int count, int i, int j);
	double SetEllipseDepth(int maxCount, int nUpOrDown, int curCount);

	//闭合曲线的起点与终点高度
	double SetDownDepth(double fDepth, size_t i);//下降高度
	double SetDownDepth1(size_t count, int i);
	
	double SetUpDepth(double fDepth, size_t count, int i);
	double SetUpDepth1(size_t count, int i);
	
	double SetPolyRedDepth(size_t count, int i);//多段线上升高度
	double SetPolyGreenDepth(size_t count, int i);
	double SetPolyBlueDepth(size_t count, int i);
	
	void toSureRectenglePosition(CPoint& first, CPoint& end);
	void toSureRectenglePosition(CPoint2D &p1, CPoint2D &p2);
	
	void GetElement(int type, double temX, double temY, double temC, double ox, double oy, 
		double unit, int i, BOOL direction);
	void GetElement1(int type, double temX, double temY, double ox, double oy, double unit, 
		int i,BOOL direction);

	CPoint2D GetNextPoint(int type, double temX, double temY, double ox, double oy, double unit, 
		int i, BOOL direction);
	
	BOOL isInRect(CRect rect, CGraph *pGraph);
	
	CPoint2D ScreenToMyQuadrant(CPoint2D);
	CPoint2D ScreenToMyQuadrant(double, double);
	CPoint2D ScreenToMyQuadrant(CPoint);
	void MyQuadrantToScreen1(CPoint2D& point);
	
	CPoint2D GetEllipsePoint(double major, double minor, double alpha, CPoint2D center, 
		double degree, BOOL direction = FALSE);
	
	void SwapPoint(CPoint2D& p1, CPoint2D& p2);	
	void SwapElement(double &a1, double &a2);
	BOOL Compare2Points(CPoint2D pm1, CPoint2D pm2);

	
	virtual CPointArray GetArrowPointArray(int type, CPoint2D pt1, CPoint2D pt2,
		BOOL flag = FALSE);
	
	virtual CPointArray GetEllipsePointArray(double major, double minor, double degree, 
		CPoint2D pt1, CPoint2D pt2, BOOL isClosed, BOOL flag = FALSE);

	BOOL PosInGraph(CGraph* pGraph, CPoint2D point);
	BOOL PosInPoly(CGraph* pGraph, CPoint2D point);
	int RetPos(CGraph* pGraph, CPoint2D point);
	
	CPoint2D GetCenterPoint(double x1, double y1, double x2, double y2, double r, 
		double angle, BOOL direction);
	
	CPointArray GetPt2Array(double startX, double startY, double endX, double endY, double con);
	
	double GetNewStep(double length, double oldStep);
	
	CPoint2D MyQuadrantToScreen(CPoint2D point);
	CPoint2D MyQuadrantToScreen(int , int);
	
	double GetNewDepth(double oldDepth);
	
	CPoint2D CalStartPoint(UINT graphType, CPoint2D startPoint, 
		CPoint2D endPoint/*centerPoint*/, double toolRadius, CString strToolWays, BOOL direction = FALSE);
	CPoint2D CalEndPoint(UINT graphType, CPoint2D startPoint, 
		CPoint2D endPoint/*centerPoint*/, double toolRadius, CString strToolWays, BOOL direction = FALSE);

	double RetValue(bool maxOrMin, double ls, double le, double rs, double re);

	CPoint2D GetCirclePoint(CPoint2D centerPoint, double radius, int degree);

	CPoint2D CalculateNewCenterPoint(CPoint2D startPoint, CPoint2D endPoint, double radius, BOOL dir, BOOL isOverPI);

	void GetRigthAngle(CPoint2D start, CPoint2D end, CPoint2D center, BOOL direction, double* angle);

	//返回下一个最短路径的索引
	int ReturnIndexOfMinDistance(CPtrArray&, const CPoint2D basePoint);
	int ReturnIndexOfMinDistance(const CPoint2D, __out double*, __out BOOL*);

	void OptimizePath(CPtrArray& arr, CPoint2D basePoint, BOOL bOptimize = TRUE);//优化路径

	void DataMalloc(const CPtrArray& m);//数据分配

	void CalculateNode(void);//计算节点

	void MySort(int *array, const int len);

	void LineReserve(CGraph* pGraph);
	void CirCleReserve(CGraph* pGraph);
	void ArcReserve(CGraph* pGraph);
	void RectangleReserve(CGraph* pGraph);
	void EllipseReserve(CGraph* pGraph);
	void PolyLineReserve(CGraph* pGraph);
	void SplineReserve(CGraph* pGraph);
	void DirectionReserve(CGraph* pGraph);

	void Prune(CPtrArray& m);

	void MergeWithNextGraph(CGraph* firstGraph, CGraph* secondGraph, const int index);
	void MergeWithFormerGraph(CGraph* firstGraph, CGraph* secondGraph, const int index);

	void GraphSort(CPtrArray& SrcGraph);
	int GetIndexFromLineInfoArray(const int index);
	void ModifyLineInfo(CGraph* pGraph, const int index);
	int GetIndexFromNumberArray(const int index);
	int GetIndexFromOperationArray(const CGraph* pGraph);

	void UpperCaseString(CString &);

	void CalculateImitatePerPointData(void);

	/************************************************************************/
	void SetNewStepToGraph(double fNewStep);
	void CancelNewStepToGraph();

	void SetNewFeedSpeed(int iFeedSpeed);
	void CancelFeedSpeed();
	/************************************************************************/
	double GetTangent(int DrawType, CPoint2D StartPt, CPoint2D EndPt, BOOL bDir = FALSE);
	CString GetMachineSerial(void);
};
