#ifndef __GLOBAL_H_
#define __GLOBAL_H_

#include "stdafx.h"
#include "Graph.h"
#include <vector>
#include "GlobalDataType.h"

#define  MAX_SIZE 10000

#define  ERRORCODE	-1
#define  EMPTY	0
#define  RED	1
#define  GREEN	2	
#define  BLUE	3


#define  LINE		2
#define  CIRCLE		3
#define  ARC		4
#define  RECTANGLE	5
#define  ELLIPSE	6
#define  POLYLINE	7
#define  SPLINE		8
#define  SOLIDPOINT		9

#define  INIT_HSETOFFSET	200
#define  INIT_VSETOFFSET	100

#define		NULL_CUT		0
#define		LEFT_CUT		1
#define		RIGHT_CUT		2
#define		OFFSET_CUT		4

#define		CARVE			0
#define		MACHINECENTER	1
#define		WATERJET		2

#define		DRAW_ON_NORMAL	0
#define		DRAW_ON_IMITATE	1

#define		TIMER_IMITATE	1

CRect g_Rect;

//共享数据区
CString g_strToolSelect = _T("T1");				//刀具号
double g_fLineStep_S = 10;						//直线短步长
double g_fLineStep_L = 15;						//直线长步长
double g_fSplineStep = 0.1;					//样条曲线步长
double g_fCurveStep_S = 5;					//曲线步长by yuanlin 13.5.21
double g_fCurveStep_M = 8;					//曲线步长by yuanlin 13.5.21
double g_fCurveStep_L = 10;					//曲线步长by yuanlin 13.5.21
double g_fCurveStep_XL = 15;					//曲线步长by yuanlin 13.5.21

double g_fCoincide_S = 5;
double g_fCoincide_M = 8;
double g_fCoincide_L = 10;
double g_fCoincide_XL = 15;
double g_fCoincideRatio = 0.25;

double g_fEllipseStep_S = 2;
double g_fEllipseStep_M = 5;
double g_fEllipseStep_L = 8;
double g_fEllipseStep_XL = 10;

int g_iUpSpeed = 10;						//抬刀速度
int g_iDownSpeed = 10;						//下刀速度
int g_iFeedSpeed = 30;						//进给速度
int g_iCoincideSpeed = 1000;
int g_iPointSpeed = 1000;
double g_fOffsetDegree = 270;				//C轴偏置量更改默认值0->270by yuanlin 13.5.21
double g_fDepth = -0.8;						//切削深度
int g_iLength = 1326;						//矩形框长度
int g_iWidth = 608;							//矩形框宽度
double g_fSafeHeight = 2.0;					//安全高度

int g_iSpacing = 5;							//标尺间距
double g_fRate = 1;								//缩放倍率
int g_vOffset = 0;
int g_hOffset = 0;
int g_vSetOffset = INIT_VSETOFFSET;
int g_hSetOffset = INIT_HSETOFFSET;
double g_fAsymptotic = 0.33;//渐近比例

double g_fCurveStep_Len = 0.5;
double g_fCoincide_Len = 0.5; 

double upDownStep = 0.0;
int	upDownCount = 0;

int g_iG00Speed = 100;						//空走速度
double g_fStartEndStep = 0.1;				//抬刀/下刀时的步长
COLORREF g_clr;
CRect g_RectNew = CRect(0);					//
int g_iDownWay = 1;							//
double g_fDeltaAngle = 20.0;
double g_fDeltaC1 = g_fDeltaAngle * PI / 180;					//C角偏差1by yuanlin 13.5.21

double g_fInterRadius = 10;
double g_fOuterRadius = 10;

double g_fPointPerDepth = 0.1;

//Start Cf_Lsd_10_22
int g_iCarveWay = 0;
//End Cf_Lsd_10_22


CArray <CInfomation> g_CircleArray;
CArray <CInfomation> g_ArcArray;
CArray <CInfomation> g_EllipseArray;
CArray <CInfomation> g_PolyArray;//多段线中的一段
CArray <CInfomation> g_SplineArray;
CArray <CPoint2D> g_NodeArray;
CArray <CLineInfo> g_LineInfoArray;
CArray <int> g_iNumArray;

CPtrArray g_TextArray;
CPtrArray g_GraphArray;//用于保存原dxf图形数据的数组
CPtrArray g_OperationArray;//用于图形操作的数组(复制，剪贴...)
CPtrArray g_SaveArray;
CPtrArray g_VirtualGraphArray;

CPoint2D* g_BetterPathPoint = NULL;
int *g_idNo = NULL;
std::vector<CGroupGraph> g_VectorGroupGraph;

vector<_2PointData> g_2PointDataVector;
vector<_2PointData> g_ImitatePerPointDataVector;
vector<_2PointData> g_DrawVector;
CRITICAL_SECTION g_csObject;

#endif