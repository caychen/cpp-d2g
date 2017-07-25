#include "StdAfx.h"
#include "FunctionArray.h"
#include <math.h>

#include "GlobalDataType.h"
//#include "Infomation.h"
//#include "d2gView.h"

#define  MAX_SIZE 10000
#define  ERRORCODE -1
#define  MAXRANGE	5

extern CPtrArray g_GraphArray;
extern CPtrArray g_OperationArray;
extern CArray <CInfomation> pArray;
extern CArray <CInfomation> g_CircleArray;
extern CArray <CInfomation> g_ArcArray;
extern CArray <CInfomation> g_EllipseArray;
extern CArray <CInfomation> g_PolyArray;
extern CArray <CInfomation> g_SplineArray;
extern CArray <CPoint2D> g_NodeArray;
extern CArray <CLineInfo> g_LineInfoArray;
extern CArray<int> g_iNumArray;

extern CRect g_Rect;

//extern double m_LineStep;
extern double g_fOffsetDegree;
extern double g_fDepth;
extern double g_fSafeHeight;
extern int g_iWidth;
extern int g_ilength; 
extern COLORREF g_clr;
extern double g_fStartEndStep;

extern int g_vSetOffset;
extern int g_hSetOffset;
extern double upDownStep;
extern int upDownCount;
extern double g_fRate;

extern double g_fCurveStep_Len;
extern double g_fCoincide_Len;

extern double g_fLineStep_S;
extern double g_fLineStep_L;
extern double g_fLineStep_S;

extern double g_fCurveStep_S;
extern double g_fCurveStep_M;
extern double g_fCurveStep_L;
extern double g_fCurveStep_XL;

extern double g_fEllipseStep_S;
extern double g_fEllipseStep_M;
extern double g_fEllipseStep_L;
extern double g_fEllipseStep_XL;

extern double g_fSplineStep;

extern CWnd* pClientWnd;
extern CPoint2D* g_BetterPathPoint;
extern int *g_idNo;
extern vector<_2PointData> g_2PointDataVector;
extern vector<_2PointData> g_ImitatePerPointDataVector;


CFunctionArray::CFunctionArray(void)
{
}

CFunctionArray::~CFunctionArray(void)
{
}


/***********************************
** 获取圆弧的圆心角函数
** 参数：	
**			first	起点坐标
**			end		终点坐标
**			radius	圆弧半径
** 时间：2013-5-10
***********************************/
double CFunctionArray::GetAngle(CPoint2D first, CPoint2D end, double radius)
{
	double a = GetDistance(first, end) / 2.0 / radius;
	if(a > 1) a = 1;
	return asin(a) * 2;
}


/***********************************
** 获取两点之间的长度函数
** 参数：	first	起点坐标
**			end		终点坐标
** 时间：2013-5-10
***********************************/
double CFunctionArray::GetDistance(CPoint2D first, CPoint2D end)
{
	return GetDistance(first.x, first.y, end.x, end.y);
}

double CFunctionArray::GetDistance(double x1, double y1, double x2, double y2)
{
	return sqrt(pow(x1 - x2, 2.0) + pow(y1 - y2, 2.0));
}

/***********************************
** 获取图形上某两点间的斜率函数
** 参数：	
**			startX	起点X坐标
**			startY	起点Y坐标
**			endX	终点X坐标
**			endY	终点Y坐标
**			注意：	对于直线，起点和终点对于直线的两个端点
对于圆弧，起点为圆心，终点为边上某一点
** 时间：2013-5-10
***********************************/
double CFunctionArray::GetC(int type, double startX, double startY, 
							double endX, double endY, BOOL direction)
{
	double c = 0.0;
	if(type == 2)
	{
		if(startX == endX)
		{
			if(startY > endY)
				c = 3 * PI / 2.0;
			else
				c = PI / 2.0;
			c += 3 * PI / 2.0;
		}

		else if(startY == endY)
		{
			if(startX < endX)
				c = 0;
			else
				c = PI;
			c += 3 * PI / 2.0;
		}
		else 
		{
			c = atan((double)(endY - startY) / (double)(endX - startX));
			if(endY - startY > 0.0 && endX - startX > 0.0)//第一象限
				c += 3 * PI / 2.0;
			if(endY - startY > 0.0 && endX - startX < 0.0)//第二象限
				c += PI / 2.0;
			if(endY - startY < 0.0 && endX - startX > 0.0)//第四象限
				c += 3 * PI / 2.0;
			if(endY - startY < 0.0 && endX - startX < 0.0)//第三象限
				c += PI / 2.0;
		}
	}
	else if(type == 3 || type == 4)
	{
		if(direction)//顺时针
		{
			if(startX == endX)
			{
				if(startY > endY)
					c = PI;
				else
					c = 0;
				c -= PI / 2.0;
			}

			else if(startY == endY)
			{
				if(startX > endX)
					c = PI / 2.0;
				else
					c = 3 * PI / 2.0;
				c -= PI / 2.0;
			}
			else 
			{
				c = atan((double)(endY - startY) / (double)(endX - startX));
				if(endY - startY > 0.0 && endX - startX > 0.0)//第一象限
					;
				if(endY - startY > 0.0 && endX - startX < 0.0)//第二象限
					c += PI;
				if(endY - startY < 0.0 && endX - startX > 0.0)//第四象限
					c += PI * 2;
				if(endY - startY < 0.0 && endX - startX < 0.0)//第三象限
					c += PI;
				c -= PI;
			}
		}
		else//逆时针
		{
			if(startX == endX)
			{
				if(startY > endY)
					c = 0;
				else
					c = PI;
				c -= PI / 2.0;
			}
			else if(startY == endY)
			{
				if(startX > endX)
					c = 3 * PI / 2.0;
				else
					c = PI / 2.0;
				c -= PI / 2.0;
			}
			else
			{
				c = atan((double)(endY - startY) / (double)(endX - startX));
				if(endY - startY > 0.0 && endX - startX > 0.0)//第一象限
					;
				if(endY - startY > 0.0 && endX - startX < 0.0)//第二象限
					c += PI;
				if(endY - startY < 0.0 && endX - startX > 0.0)//第四象限
					c += PI * 2;
				if(endY - startY < 0.0 && endX - startX < 0.0)//第三象限
					c += PI;
			}
		}
	}
	else if(type == 6)
	{

	}

	c += g_fOffsetDegree * PI / 180;
	if(c < 0.0)
		c += 2 * PI;
	c = c - 2 * PI < 0 ? c : c - 2 * PI;

	return c;
}


/***********************************
** 将颜色字符串转换为相对应的RGB函数
** 参数：
**			colorString	颜色字符串
** 时间：2013-5-10
***********************************/
COLORREF CFunctionArray::StringToRGB(CString colorString)
{
	if(colorString == "Red")
		g_clr = RGB(255, 0, 0);
	else if(colorString == "Green")
		g_clr = RGB(0, 255, 0);
	else if(colorString == "Blue")
		g_clr = RGB(0, 0, 255);
	else
		g_clr = RGB(0, 0, 0);
	return g_clr;
}

/***************************有关渐近高度的函数***************************/
double CFunctionArray::SetDepth1(double fDepth, size_t count, int i, int j)//非闭合
{
	double temZ = 0, z = 0;
	double step = upDownStep;
	int m_upDownCount = upDownCount;

	if(i < count - m_upDownCount)
	{
		temZ = - step * i;
		z = temZ > fDepth ? temZ : fDepth;
	}
	else
	{
		z = fDepth;
		z = -step * j;
		z = z < 0.0 ? z : 0.0;
	}
	return z;
}

/***********************************
** 刀具在移动过程中的切削深度函数
** 参数：
**			count	图形分成的段数
**			i，j	循环次数中的某一个		
** 时间：2013-5-10
***********************************/
double CFunctionArray::SetDepth2(UINT iDrawType, double fCoincide_Len, double fDepth, double fCurveStep_Len, int nCount, int i, int j)//闭合
{
	double temZ = 0, z = 0;
	double step = 0.0;
	int nUpDownCount = 0;

	//if(iDrawType == 6)
	//{
	//	upDownCount = g_fCoincide_Len / g_fCurveStep_Len;
	//	step = fabs(g_fDepth) / upDownCount;
	//}
	//else
	//{
	nUpDownCount = fCoincide_Len / fCurveStep_Len;
	step = fabs(fDepth) / nUpDownCount;
	step = GetNewStep(fabs(fDepth), step);
	//	}

	if(i <= nCount - nUpDownCount)
	{
		temZ = - step * i;
		z = temZ > fDepth ? temZ : fDepth;
	}
	else
	{
		z = fDepth;
		z = -step * j;
		z = z < 0.0 ? z : 0.0;
	}
	return z;
}

double CFunctionArray::SetDownDepth(double fDepth, size_t i)
{
	double z = 0;
	z = -upDownStep * i;
	z = z > fDepth ? z : fDepth;
	return z;
}

double CFunctionArray::SetDownDepth1(size_t count, int i)
{
	double z = 0, step = 0;
	int m_upDownCount = 0;

	step = g_fCurveStep_Len * fabs(g_fDepth) / g_fCoincide_Len;
	m_upDownCount = (fabs(g_fDepth) + 10e-4) / step;
	step = GetNewStep(fabs(g_fDepth), step);

	z = -step * i;
	z = z > g_fDepth ? z : g_fDepth;

	return z;
}

double CFunctionArray::SetEllipseDepth(int maxCount, int nUpOrDown, int curCount)
{
	double z,tmpZ = 0.0;

	double step = fabs(g_fDepth) / nUpOrDown;
	step = GetNewStep(fabs(g_fDepth), step);

	if(curCount <= maxCount - nUpOrDown)
	{
		tmpZ = -step * curCount;
		z = tmpZ > g_fDepth ? tmpZ : g_fDepth;
	}
	else
	{
		z = -step * (maxCount - curCount);
		z = z < 0.0 ? z : 0.0;
	}
	return z;
}

double CFunctionArray::SetUpDepth(double fDepth, size_t count, int i)
{
	double z = g_fDepth;
	int m_upDownCount = upDownCount;
	if(i >= count - m_upDownCount)
	{
		z = -upDownStep * (count - i);
		z = z < 0.0 ? z : 0.0;
	}
	return z;
}

double CFunctionArray::SetUpDepth1(size_t count, int i)
{
	double z = g_fDepth, step = 0;
	int m_upCount = 0;

	step = g_fCurveStep_Len * fabs(g_fDepth) / g_fCoincide_Len;
	m_upCount = (fabs(g_fDepth) + 10e-4) / step;
	step = GetNewStep(fabs(g_fDepth), step);

	if(i >= count - m_upCount)
	{
		z = -step * (count - i);
		//z = z < 0.0 ? z : 0.0;
	}

	return z;
}

double CFunctionArray::SetPolyRedDepth(size_t count, int i)
{
	double z = g_fDepth;
	double step = upDownStep;
	int upCount = upDownCount;

	if(i <= upCount)
	{
		z = -step * i;
		z = z < g_fDepth ? g_fDepth : z;
	}
	else if(i >= count - upCount)
	{
		z = -step * (count - i);
		z = z > 0 ? 0 : z;
	}

	return z;

}

double CFunctionArray::SetPolyGreenDepth(size_t count, int i)
{
	double z = g_fDepth, step = upDownStep;
	int upCount = upDownCount;

	//step = g_fCurveStep_Len * fabs(g_fDepth) / g_fCoincide_Len;
	//upCount = (fabs(g_fDepth) + 10e-4) / step;

	if(i <= upCount)
	{
		z = -step * i;
	}
	return z;
}

double CFunctionArray::SetPolyBlueDepth(size_t count, int i)
{
	double z = g_fDepth, step = upDownStep;
	int upCount = upDownCount;

	if(i >= count - upCount)
	{
		z = -step * (count - i);
	}
	return z;

}

double CFunctionArray::SetPolyDownDepth(size_t i)
{
	double z = 0;
	z = -g_fStartEndStep * i;
	z = z > g_fDepth ? z : g_fDepth;
	return z;
}

double CFunctionArray::SetPolyDownDepth1(size_t i)
{
	double z = 0, step = 0;
	int m_upDownCount = 0;

	m_upDownCount = g_fCoincide_Len / g_fCurveStep_Len;
	step = fabs(g_fDepth) / m_upDownCount;
	step = GetNewStep(fabs(g_fDepth), step);
	//m_upDownCount = (fabs(g_fDepth) + 10e-4) / step;

	z = -step * i;
	z = z > g_fDepth ? z : g_fDepth;
	return z;
}

double CFunctionArray::SetPolyUpDepth(size_t count, int i)
{
	double z = g_fDepth;

	int m_upDownCount = (fabs(g_fDepth) + 1e-3) / g_fStartEndStep;
	if(i >= count - m_upDownCount)
	{
		z = -g_fStartEndStep * (count - i);
		z = z < 0.0 ? z : 0.0;
	}
	return z;
}

double CFunctionArray::SetPolyUpDepth1(size_t count, int i)
{
	double z = g_fDepth, step = 0;
	int m_upCount = 0;

	step = g_fCurveStep_Len * fabs(g_fDepth) / g_fCoincide_Len;
	m_upCount = (fabs(g_fDepth) + 10e-4) / step;

	if(i >= count - m_upCount)
	{
		z = -step * (count - i);
		z = z < 0.0 ? z : 0.0;
	}

	return z;
}


/***********************************
** 在拖动鼠标产生矩形框的时候将起点和终点设置为左上角和右下角函数
** 参数：	
**			first	拖动鼠标前按下时的坐标
**			end		弹起鼠标时的坐标
** 时间：2013-5-10
***********************************/
void CFunctionArray::toSureRectenglePosition(CPoint2D& first, CPoint2D& end)
{
	double temX = 0, temY = 0;

	if(first.x > end.x)
	{
		temX = first.x;
		first.x = end.x;
		end.x = temX;
	}

	if(first.y > end.y)
	{
		temY = first.y;
		first.y = end.y;
		end.y = temY;
	}
}

void CFunctionArray::toSureRectenglePosition(CPoint &first, CPoint &end)
{
	return toSureRectenglePosition(CPoint2D(first.x, first.y), CPoint2D(end.x, end.y));
}

/***********************************
** 通过上一点的坐标信息获取下一点坐标信息函数
** 参数：
**			type	图形类型
**			temX	上一点的X坐标
**			temY	上一点的Y坐标
**			temC	上一点的斜率
**			ox		圆心X坐标
**			oy		圆心Y坐标
**			unit	每一小段的弧度
**			i		循环次数中的一个
**			direction	画圆弧时的顺序(顺或逆)
** 时间：2013-5-10
***********************************/
void CFunctionArray:: GetElement(int type, double temX, double temY, double temC, 
								 double ox, double oy, double unit, int i, BOOL direction)
{
	double x, y, c;

	if(direction)
	{
		x = (temX - ox) * cos(unit) + (temY - oy) * sin(unit) + ox;
		y = (temY - oy) * cos(unit) - (temX - ox) * sin(unit) + oy;
	}	
	else
	{
		x = (temX - ox) * cos(unit) - (temY - oy) * sin(unit) + ox;
		y = (temY - oy) * cos(unit) + (temX - ox) * sin(unit) + oy;
	}

	if(type == 3)
	{
		//g_CircleArray.SetAt(i - 1,CInfomation(temX,temY,temC));
		c = GetC(type, ox, oy, x, y, direction);
		g_CircleArray.Add(CInfomation(x, y, c));

	}
	else if(type == 4)
	{
		//g_ArcArray.SetAt(i - 1,CInfomation(temX,temY,temC));
		c = GetC(type, ox, oy, x, y,direction);
		g_ArcArray.Add(CInfomation(x, y, c));

	}
	else if(type == 7)
	{		
		c = GetC(3, ox, oy, x, y, direction);
		g_PolyArray.Add(CInfomation(x, y, c));
	}

}

void CFunctionArray::GetElement1(int type, double temX, double temY, 
								 double ox, double oy, double unit, int i, BOOL direction)
{
	double x = 0, y = 0;
	if(direction)
	{
		x = (temX - ox) * cos(unit) + (temY - oy) * sin(unit) + ox;
		y = (temY - oy) * cos(unit) - (temX - ox) * sin(unit) + oy;
	}	
	else
	{
		x = (temX - ox) * cos(unit) - (temY - oy) * sin(unit) + ox;
		y = (temY - oy) * cos(unit) + (temX - ox) * sin(unit) + oy;
	}

	if(type == 3)
	{
		g_CircleArray.SetAt(i - 1, CInfomation(temX, temY));
		g_CircleArray.Add(CInfomation(x, y));

	}
	else if(type == 4)
	{
		g_ArcArray.SetAt(i - 1, CInfomation(temX, temY));
		g_ArcArray.Add(CInfomation(x, y));

	}
	else if(type == 7)
	{
		g_PolyArray.Add(CInfomation(x, y));

	}

}


/***********************************
** 确认图形是否在所拖动的矩形框内函数
** 参数：
**			rect	矩形框
**			pGraph	图形类
** 时间：2013-5-10
***********************************/
BOOL CFunctionArray::isInRect(CRect rect, CGraph *pGraph)
{
	//int i = g_GraphArray.GetSize();
	CPoint2D startPoint, endPoint, centerPoint;
	double m_r = 0;
	BOOL bDirection = FALSE;
	double startX = 0, startY = 0, midX = 0, midY = 0, endX = 0, endY = 0;
	CRect rectSmall;
	CPoint2D pt1, pt2;
	double fOX = 0, fOY = 0, tempAngle = 0, k1 = 0, k2 = 0;
	double x = 0, y = 0, temX = 0, temY = 0;
	double Length = 0, unit = 0, count;
	double major = 0, minor = 0;
	double lastX = 0, lastY = 0, nextX = 0, nextY = 0;

	CArray<CPoint2D> ptMember;
	double *point;
	double angle = 0;
	int ptNumber = 0;
	BOOL flags = FALSE;
	double firstAngle = 0, endAngle = 0;
	CPoint2D ptTemp;
	CBspline bCurve;

	vector<vec2> controlPt;

	switch(pGraph->m_iDrawType)
	{
	case 2:
		startPoint = pGraph->m_ptOrigin;
		endPoint = pGraph->m_ptEnd;

		if(startPoint.x >= rect.left && startPoint.x <= rect.right  
			&& startPoint.y <= rect.top  && startPoint.y >= rect.bottom 
			&& endPoint.x >= rect.left && endPoint.x <= rect.right  
			&& endPoint.y <= rect.top  && endPoint.y >= rect.bottom )
			return TRUE;
		else	
			return FALSE;

		break;

	case 3:
		m_r = pGraph->m_fRadius ;
		centerPoint = pGraph->m_ptCenter;
		rectSmall.top = rect.top + m_r;
		rectSmall.left = rect.left + m_r;
		rectSmall.bottom = rect.bottom - m_r;
		rectSmall.right = rect.right - m_r;
		if(centerPoint.x >= rectSmall.left  && centerPoint.x <= rectSmall.right  
			&& centerPoint.y <= rectSmall.top  && centerPoint.y >= rectSmall.bottom )
			return TRUE;
		else
			return FALSE;

		break;

	case 4:
		startX = pGraph->m_ptOrigin.x;
		startY = pGraph->m_ptOrigin.y ;
		midX = pGraph->m_ptMiddle.x;
		midY = pGraph->m_ptMiddle.y ;
		endX = pGraph->m_ptEnd.x;
		endY = pGraph->m_ptEnd.y ;
		fOX = pGraph->m_ptCenter.x;
		fOY = pGraph->m_ptCenter.y ;
		bDirection = pGraph->m_bDirection;//获取圆弧是顺时针还是逆时针
		tempAngle = pGraph->m_fAngle;
		m_r = pGraph->m_fRadius ;

		if(startY == midY && startY != endY)
		{
			Length = m_r * tempAngle;
			count = Length / 0.1;
			unit = tempAngle / count;
			x = startX,y = startY;

			if(startY < endY)
			{
				if(bDirection)
				{
					for(int i = 0; i < count; ++i)
					{
						temX = x, temY = y;
						if(x < rect.left || x > rect.right || y > rect.top || y < rect.bottom)
							return FALSE;
						x = (temX - fOX) * cos(unit) + (temY - fOY) * sin(unit) + fOX;
						y = (temY - fOY) * cos(unit) - (temX - fOX) * sin(unit) + fOY;
					}
				}
				else
				{
					for(int i = 0; i < count; ++i)
					{
						temX = x, temY = y;
						if(x < rect.left || x > rect.right || y > rect.top || y < rect.bottom)
							return FALSE;
						x = (temX - fOX) * cos(unit) - (temY - fOY) * sin(unit) + fOX;
						y = (temY - fOY) * cos(unit) + (temX - fOX) * sin(unit) + fOY;
					}
				}
			}
			else
			{
				if(bDirection)
				{
					for(int i = 0; i < count; ++i)
					{
						temX = x, temY = y;
						if(x < rect.left || x > rect.right || y > rect.top || y < rect.bottom)
							return FALSE;
						x = (temX - fOX) * cos(unit) + (temY - fOY) * sin(unit) + fOX;
						y = (temY - fOY) * cos(unit) - (temX - fOX) * sin(unit) + fOY;
					}
				}
				else
				{
					for(int i = 0; i < count; ++i)
					{
						temX = x, temY = y;
						if(x < rect.left || x > rect.right || y > rect.top || y < rect.bottom)
							return FALSE;
						x = (temX - fOX) * cos(unit) - (temY - fOY) * sin(unit) + fOX;
						y = (temY - fOY) * cos(unit) + (temX - fOX) * sin(unit) + fOY;
					}
				}
			}

		}
		else if(startY == endY && startY != midY)
		{
			Length = m_r * tempAngle;
			count = Length / 0.1;
			unit = tempAngle / count;
			x = startX, y = startY;

			if(startY < midY)
			{
				if(bDirection)
				{
					for(int i = 0; i < count; ++i)
					{
						temX = x, temY = y;
						if(x < rect.left || x > rect.right || y > rect.top || y < rect.bottom)
							return FALSE;
						x = (temX - fOX) * cos(unit) + (temY - fOY) * sin(unit) + fOX;
						y = (temY - fOY) * cos(unit) - (temX - fOX) * sin(unit) + fOY;
					}
				}
				else
				{
					for(int i = 0; i < count; ++i)
					{
						temX = x, temY = y;
						if(x < rect.left || x > rect.right || y > rect.top || y < rect.bottom)
							return FALSE;
						x = (temX - fOX) * cos(unit) - (temY - fOY) * sin(unit) + fOX;
						y = (temY - fOY) * cos(unit) + (temX - fOX) * sin(unit) + fOY;
					}
				}
			}
			else
			{
				if(bDirection)
				{
					for(int i = 0; i < count; ++i)
					{
						temX = x, temY = y;
						if(x < rect.left || x > rect.right || y > rect.top || y < rect.bottom)
							return FALSE;
						x = (temX - fOX) * cos(unit) + (temY - fOY) * sin(unit) + fOX;
						y = (temY - fOY) * cos(unit) - (temX - fOX) * sin(unit) + fOY;
					}
				}
				else
				{
					for(int i = 0; i < count; ++i)
					{
						temX = x, temY = y;
						if(x < rect.left || x > rect.right || y > rect.top || y < rect.bottom)
							return FALSE;
						x = (temX - fOX) * cos(unit) - (temY - fOY) * sin(unit) + fOX;
						y = (temY - fOY) * cos(unit) + (temX - fOX) * sin(unit) + fOY;
					}
				}
			}

		}
		else
		{
			k1 = (double)(startX - midX) / (double)(midY - startY);
			k2 = (double)(startX - endX) / (double)(endY - startY);
			if(k1 == k2)
				return FALSE;

			else
			{
				Length = m_r * tempAngle;
				count = Length / 0.1;
				unit = tempAngle / count;
				x = startX, y = startY;

				if(startY - (endX - startX) / k1 < endY)
				{
					if(bDirection)
					{
						for(int i = 0; i < count; ++i)
						{
							temX = x, temY = y;
							if(x < rect.left || x > rect.right || y > rect.top || y < rect.bottom)
								return FALSE;
							x = (temX - fOX) * cos(unit) + (temY - fOY) * sin(unit) + fOX;
							y = (temY - fOY) * cos(unit) - (temX - fOX) * sin(unit) + fOY;
						}
					}
					else
					{
						for(int i = 0; i < count; ++i)
						{
							temX = x, temY = y;
							if(x < rect.left || x > rect.right || y > rect.top || y < rect.bottom)
								return FALSE;
							x = (temX - fOX) * cos(unit) - (temY - fOY) * sin(unit) + fOX;
							y = (temY - fOY) * cos(unit) + (temX - fOX) * sin(unit) + fOY;
						}
					}
				}
				else
				{
					if(bDirection)
					{
						for(int i = 0; i < count; ++i)
						{
							temX = x, temY = y;
							if(x < rect.left || x > rect.right || y > rect.top || y < rect.bottom)
								return FALSE;
							x = (temX - fOX) * cos(unit) + (temY - fOY) * sin(unit) + fOX;
							y = (temY - fOY) * cos(unit) - (temX - fOX) * sin(unit) + fOY;
						}
					}
					else
					{
						for(int i = 0; i < count; ++i)
						{
							temX = x, temY = y;
							if(x < rect.left || x > rect.right || y > rect.top || y < rect.bottom)
								return FALSE;
							x = (temX - fOX) * cos(unit) - (temY - fOY) * sin(unit) + fOX;
							y = (temY - fOY) * cos(unit) + (temX - fOX) * sin(unit) + fOY;
						}
					}
				}

			}
		}
		return TRUE;
		break;

	case 5:
		startPoint = pGraph->m_ptOrigin;
		endPoint = pGraph->m_ptEnd;

		if(startPoint.x >= rect.left && startPoint.x <= rect.right  
			&& startPoint.y <= rect.top  && startPoint.y >= rect.bottom 
			&& endPoint.x >= rect.left && endPoint.x <= rect.right  
			&& endPoint.y <= rect.top  && endPoint.y >= rect.bottom )
			return TRUE;
		else	
			return FALSE;

		break;


	case 6:
		major = pGraph->m_fMajor;
		minor = pGraph->m_fMinor;
		tempAngle = pGraph->m_fDegree;
		firstAngle = pGraph->m_fStartAngle;
		endAngle = pGraph->m_fEndAngle;
		centerPoint = pGraph->m_ptCenter;
		bDirection = pGraph->m_bDirection;


		for(int i = firstAngle * 180 / PI; i < endAngle * 180 / PI; ++i)
		{
			ptTemp = GetEllipsePoint(major, minor, tempAngle, centerPoint, i, bDirection);
			//x = GetEllipsePoint(major,minor,tempAngle,centerPoint,i,m_bDirection);
			//y = GetEllipsePoint(major,minor,tempAngle,centerPoint,i,m_bDirection);
			x = ptTemp.x, y = ptTemp.y;
			if(x < rect.left || x > rect.right || y > rect.top || y < rect.bottom)
				return FALSE;
		}
		return TRUE;
		break;

	case 7:
		ptNumber = pGraph->m_iVertexNum;
		ptMember.Copy(pGraph->m_PeakGroup);
		flags = pGraph->m_bFlags;


		point = new double[3 * ptNumber];
		for(int i = 0; i < ptNumber; ++i)
		{
			point[i * 3] = ptMember[i].x;
			point[i * 3 + 1] = ptMember[i].y;
			point[i * 3 + 2] = ptMember[i].con;
		}

		for(int j = 0; j < ptNumber - 1; ++j)
		{
			startX = point[j * 3], startY = point[j * 3 + 1];
			tempAngle = point[j * 3 + 2];
			endX = point[(j + 1) * 3], endY = point[(j + 1) * 3 + 1];

			if(tempAngle == 0.0)
			{
				if(startX < rect.left || startX > rect.right  || startY > rect.top  
					|| startY < rect.bottom || endX < rect.left || endX > rect.right
					|| endY > rect.top || endY < rect.bottom )
					return FALSE;
			}
			else
			{
				angle = 4 * atan(fabs(tempAngle));
				m_r = GetDistance(startX, startY, endX, endY) / (2 * sin(angle / 2));

				if(tempAngle > 0)
					bDirection = FALSE;						
				else
					bDirection = TRUE;
				centerPoint = GetCenterPoint(startX, startY, endX, endY, m_r, 
					angle, bDirection);

				Length = m_r * angle;
				count = Length / 0.1;
				unit = 0.1 / m_r;

				if(centerPoint.x == startX)
					firstAngle = PI / 2.0;
				if(centerPoint.x == endX)
					endAngle = 3 * PI / 2.0;
				else
				{
					firstAngle = GetAngle(centerPoint.x, centerPoint.y, startX, startY);
					endAngle = GetAngle(centerPoint.x, centerPoint.y, endX, endY);
					if(firstAngle < endAngle)
					{
						if(!bDirection)
							;
						else
						{
							SwapElement(firstAngle, endAngle);
							firstAngle = firstAngle - 2 * PI;
						}
					}
					else
					{
						if(!bDirection)
							firstAngle = firstAngle - 2 * PI;
						else
							SwapElement(firstAngle, endAngle);
					}

					for(double i = firstAngle; i <= endAngle; ++i)
					{
						x = m_r * cos(firstAngle) + centerPoint.x;
						y = m_r * sin(firstAngle) + centerPoint.y;
						if(x < rect.left || x > rect.right || y < rect.bottom || y > rect.top)
							return FALSE;
					}
				}
			}
		}

		if(flags)
		{
			startX = point[(ptNumber - 1) * 3], startY = point[(ptNumber - 1) * 3 + 1];
			tempAngle = point[(ptNumber - 1) * 3 + 2];
			endX = point[0], endY = point[1];

			if(tempAngle == 0.0)
			{
				if(startX < rect.left || startX > rect.right  || startY > rect.top  
					|| startY < rect.bottom || endX < rect.left 
					|| endX > rect.right || endY > rect.top || endY < rect.bottom )
					return FALSE;
			}
			else
			{
				angle = 4 * atan(fabs(tempAngle));
				m_r = GetDistance(startX, startY, endX, endY) / (2 * sin(angle / 2));

				if(tempAngle > 0)
					bDirection = FALSE;						
				else
					bDirection = TRUE;
				centerPoint = GetCenterPoint(startX, startY, endX, endY, m_r, 
					angle, bDirection);

				Length = m_r * angle;
				count = Length / 0.1;
				unit = 0.1 / m_r;

				if(centerPoint.x == startX)
					firstAngle = PI / 2.0;
				if(centerPoint.x == endX)
					endAngle = 3 * PI / 2.0;
				else
				{
					firstAngle = GetAngle(centerPoint.x, centerPoint.y, startX, startY);
					endAngle = GetAngle(centerPoint.x, centerPoint.y, endX, endY);
					if(firstAngle < endAngle)
					{
						if(!bDirection)
							;
						else
						{
							SwapElement(firstAngle, endAngle);
							firstAngle = firstAngle - 2 * PI;
						}
					}
					else
					{
						if(!bDirection)
							firstAngle = firstAngle - 2 * PI;
						else
							SwapElement(firstAngle, endAngle);
					}

					for(double i = firstAngle; i <= endAngle; ++i)
					{
						x = m_r * cos(firstAngle) + centerPoint.x;
						y = m_r * sin(firstAngle) + centerPoint.y;
						if(x < rect.left || x > rect.right || y < rect.bottom || y > rect.top)
							return FALSE;
					}
				}
			}
		}
		return TRUE;
		break;

	case 8:

		controlPt = vector<vec2>(pGraph->controlNode.begin(), pGraph->controlNode.end());
		bCurve = CBspline(controlPt);
		bCurve.ComputeInterpolation();

		for(int j = 0; j < bCurve.m_interpolationNode.size() - 1; ++j)
		{
			lastX = bCurve.m_interpolationNode[j][0], lastY = bCurve.m_interpolationNode[j][1];
			//nextX = bCurve.m_interpolationNode[j + 1][0],nextY = bCurve.m_interpolationNode[j + 1][1];
			if(lastX < rect.left || lastX > rect.right || lastY < rect.bottom || lastY > rect.top)
				return FALSE;
		}

		return TRUE;
		break;

	case 9:
		startX = pGraph->m_ptOrigin.x;
		startY = pGraph->m_ptOrigin.y;

		if(startX >= rect.left && startX <= rect.right && startY >= rect.bottom && startY <= rect.top)
			return TRUE;
		break;
	}
	return FALSE;
}


/***********************************
** 获取椭圆在某一角度是的坐标
** 参数：
**			major	长轴
**			minor	短轴
**			alpha	长轴与短轴之间的倾斜角（弧度）
**			center	椭圆中心坐标
**			degree	以长轴之间的夹角（度）
**			direction 椭圆方向
** 时间：2013-10-23
***********************************/
CPoint2D CFunctionArray::GetEllipsePoint(double major, double minor, double alpha, 
										 CPoint2D center, double degree, BOOL direction)
{
	CPoint2D pM;
	degree = degree * PI / 180;
	if(!direction)
	{
		pM.x = major * cos(alpha) * cos(degree) - minor * sin(alpha) * sin(degree) + center.x;
		pM.y = major * sin(alpha) * cos(degree) + minor * cos(alpha) * sin(degree) + center.y;
		//x = major * cos(alpha) * cos(degree) - minor * sin(alpha) * sin(degree) + center.x;
	}
	else
	{
		pM.x = major * cos(alpha) * cos(degree) - minor * sin(alpha) * sin(degree) + center.x;
		pM.y = major * sin(alpha) * cos(degree) + minor * cos(alpha) * sin(degree) + center.y;
	}
	return pM;
}

/***********************************
** 将坐标进行交换
** 参数：
**			p1	坐标1
**			p2	坐标2
** 时间：2013-10-23
***********************************/
void CFunctionArray::SwapPoint(CPoint2D& p1, CPoint2D& p2)
{
	CPoint2D temp;
	temp = p1;
	p1 = p2;
	p2 = temp;
}

/***********************************
** 获取组成箭头的三点坐标
** 参数：
**			type	图形类型
**			pt1		最后一点的前一点（对于直线为起点）
**			pt2		最后一点
**			flag	方向，默认为FALSE
** 时间：2013-10-23
***********************************/
CPointArray CFunctionArray::GetArrowPointArray(int type, CPoint2D pt1, 
											   CPoint2D pt2, BOOL flag)
{
	double slpoy = 0.0, cosy = 0.0, siny = 0.0;
	double arrowLength = 10.0;
	CPoint2D ptStart1, ptStart2, ptEnd, ptStart;
	double angle1 = 0, angle2 = 0;
	double c = 0.0;

	CPoint2D p1, p2;
	double tCos = 0.0, tSin = 0.0;


	switch(type)
	{
	case 2:
	case 5:
		ptEnd = pt2;
		if(pt1.x == pt2.x)
		{
			if(pt1.y < pt2.y)
				slpoy = PI * 3 / 2;
			else
				slpoy = PI / 2;
		}
		else
			slpoy = atan((double)(pt2.y - pt1.y) / (double)(pt2.x - pt1.x));
		cosy = cos(slpoy);
		siny = sin(slpoy);
		if (pt2.x - pt1.x > 0)
		{
			ptStart1 = CPoint2D(pt2.x - int( arrowLength * cosy - arrowLength / 2.0 * siny), 
				pt2.y - int(arrowLength * siny + arrowLength / 2.0 * cosy));
			ptStart2 = CPoint2D(pt2.x - int( arrowLength * cosy + arrowLength / 2.0 * siny), 
				pt2.y + int(arrowLength / 2.0 * cosy - arrowLength * siny));
		}
		else
		{
			ptStart1 = CPoint2D(pt2.x + int( arrowLength * cosy - arrowLength / 2.0 * siny), 
				pt2.y + int(arrowLength * siny + arrowLength / 2.0 * cosy));
			ptStart2 = CPoint2D(pt2.x + int( arrowLength * cosy + arrowLength / 2.0 * siny), 
				pt2.y - int(arrowLength / 2.0 * cosy - arrowLength * siny));
		}
		break;

	case 3:
		ptEnd = pt2;
		c = GetAngle(pt1.x, pt1.y, pt2.x, pt2.y);
		if(fabs(c) == 0.0)
		{
			if(flag)
			{

				ptStart1 = CPoint2D(ptEnd.x - arrowLength * sin(PI / 6), 
					ptEnd.y + arrowLength * cos(PI / 6));
				ptStart2 = CPoint2D(ptEnd.x + arrowLength * sin(PI / 6), 
					ptEnd.y + arrowLength * cos(PI / 6));
			}
			else
			{
				c += PI / 2.0;
				ptStart1 = CPoint2D(ptEnd.x - arrowLength * sin(PI / 6), 
					ptEnd.y - arrowLength * cos(PI / 6));	
				ptStart2 = CPoint2D(ptEnd.x + arrowLength * sin(PI / 6), 
					ptEnd.y - arrowLength * cos(PI / 6));

			}
		}else

		{
			if(flag)
			{
				c -= 3 * PI / 2.0;
				tCos = cos(c + PI / 6);
				tSin = sin(c + PI / 6);

				p1.x = ptEnd.x - arrowLength * cos(c + PI / 6);
				p1.y = ptEnd.y - arrowLength * sin(c + PI / 6);

				tCos = cos(c - PI / 6);
				tSin = sin(c - PI / 6);
				p2.x = ptEnd.x - arrowLength * cos(c - PI / 6);
				p2.y = ptEnd.y - arrowLength * sin(c - PI / 6);

				ptStart1 = p1;
				ptStart2 = p2;
			}
			else
			{
				c -= PI / 2.0;

				tCos = cos(c + PI / 6);
				tSin = sin(c + PI / 6);
				p1.x = ptEnd.x - arrowLength * cos(c + PI / 6);
				p1.y = ptEnd.y - arrowLength * sin(c + PI / 6);

				tCos = cos(c - PI / 6);
				tSin = sin(c - PI / 6);
				p2.x = ptEnd.x - arrowLength * cos(c - PI / 6);
				p2.y = ptEnd.y - arrowLength * sin(c - PI / 6);

				ptStart1 = p1;	
				ptStart2 = p2;

			}
		}
		break;

	case 4:
		ptEnd = pt2;
		if(flag)
		{
			ptStart.x = (ptEnd.x - pt1.x) * cos(PI / 90) + (ptEnd.y - pt1.y) * sin(PI / 90) + pt1.x;
			ptStart.y = (ptEnd.y - pt1.y) * cos(PI / 90) - (ptEnd.x - pt1.x) * sin(PI / 90) + pt1.y;
		}
		else
		{
			ptStart.x = (ptEnd.x - pt1.x) * cos(PI / 90) - (ptEnd.y - pt1.y) * sin(PI / 90) + pt1.x;
			ptStart.y = (ptEnd.y - pt1.y) * cos(PI / 90) + (ptEnd.x - pt1.x) * sin(PI / 90) + pt1.y;
		}

		if(ptEnd.x == ptStart.x)
			ptStart = CPoint2D(ptStart.x + 1, ptStart.y + 1);

		slpoy = atan((double)(ptEnd.y - ptStart.y) / (double)(ptEnd.x - ptStart.x));
		cosy = cos(slpoy);
		siny = sin(slpoy);

		if (ptEnd.x - ptStart.x >= 0)
		{
			ptStart1 = CPoint2D(ptEnd.x - int( arrowLength * cosy - arrowLength / 2.0 * siny), 
				ptEnd.y - int(arrowLength * siny + arrowLength / 2.0 * cosy));
			ptStart2 = CPoint2D(ptEnd.x - int( arrowLength * cosy + arrowLength / 2.0 * siny), 
				ptEnd.y + int(arrowLength / 2.0 * cosy - arrowLength * siny));
		}
		else
		{
			ptStart1 = CPoint2D(ptEnd.x + int( arrowLength * cosy - arrowLength / 2.0 * siny), 
				ptEnd.y + int(arrowLength * siny + arrowLength / 2.0 * cosy));
			ptStart2 = CPoint2D(ptEnd.x + int( arrowLength * cosy + arrowLength / 2.0 * siny), 
				ptEnd.y - int(arrowLength / 2.0 * cosy - arrowLength * siny));
		}
		break;

	}
	return CPointArray(ptEnd, ptStart1, ptStart2);
}

/***********************************
** 获取椭圆的箭头的三点坐标
** 参数：
**			major	长轴
**			minor	短轴
**			degree	长轴与短轴的夹角（弧度）
**			pt1		最后一点的前一点
**			pt2		最后一点
**			isClosed	椭圆是否是封闭的
**			flag	椭圆方向
** 时间：2013-10-23
***********************************/
CPointArray CFunctionArray::GetEllipsePointArray(double major, double minor, double degree, 
												 CPoint2D pt1, CPoint2D pt2, BOOL isClosed, BOOL flag /* = FALSE */)
{
	double arrowLength = 10.0;
	double length = 0;
	CPoint2D ptStart1, ptStart2;
	CPoint2D prePoint;
	double angle1 = 0, angle2 = 0;
	double k = 0;//终点的切线斜率

	if(isClosed)
	{
		if(pt1.y == pt2.y)
		{
			if(flag)
			{
				ptStart1 = CPoint2D(pt2.x - arrowLength * sin(PI / 5), 
					pt2.y - arrowLength * cos(PI / 5));
				ptStart2 = CPoint2D(pt2.x + arrowLength * sin(PI / 5), 
					pt2.y - arrowLength * cos(PI / 5));
			}
			else
			{
				ptStart1 = CPoint2D(pt2.x - arrowLength * sin(PI / 5), 
					pt2.y + arrowLength * cos(PI / 5));	
				ptStart2 = CPoint2D(pt2.x + arrowLength * sin(PI / 5), 
					pt2.y + arrowLength * cos(PI / 5));
			}
		}
		else if(pt1.x == pt2.x)
		{
			if (flag)
			{
				ptStart1 = CPoint2D(pt2.x + arrowLength * cos(PI / 5), 
					pt2.y + arrowLength * sin(PI / 5));
				ptStart2 = CPoint2D(pt2.x + arrowLength * cos(PI / 5), 
					pt2.y - arrowLength * sin(PI / 5));
			}
			else
			{
				ptStart1 = CPoint2D(pt2.x - arrowLength * cos(PI / 5), 
					pt2.y + arrowLength * sin(PI / 5));
				ptStart2 = CPoint2D(pt2.x - arrowLength * cos(PI / 5), 
					pt2.y - arrowLength * sin(PI / 5));

			}
		}
		else
		{
			angle1 = atan((double)(pt2.y - pt1.y) / (double)(pt2.x - pt1.x)) + PI / 5 - PI / 2;
			angle2 = angle1 + 2 * PI / 5;
			if(flag)
			{
				ptStart1 = CPoint2D(pt2.x + arrowLength * cos(angle1), 
					pt2.y - arrowLength * sin(angle1));
				ptStart2 = CPoint2D(pt2.x + arrowLength * cos(angle2), 
					pt2.y - arrowLength * sin(angle2));
			}
			else
			{
				ptStart1 = CPoint2D(pt2.x - arrowLength * cos(angle1), 
					pt2.y + arrowLength * sin(angle1));
				ptStart2 = CPoint2D(pt2.x - arrowLength * cos(angle2), 
					pt2.y + arrowLength * sin(angle2));
			}
		}
	}
	else
	{
		if(degree == 0 || degree == PI)
		{
			if(pt2.y == pt1.y)
				k = PI / 2.0;	
			else if(pt2.x == pt1.x)
				k = PI;
			else
			{
				k = atan(-((pt2.x - pt1.x) / pow(major,2.0)) / ((pt2.y - pt1.y) / pow(minor,2.0)));
				if(pt1.y > pt2.y)
					k += PI;
			}

			angle1 = k - PI / 5, angle2 = k + PI / 5;
			if(flag)
			{
				ptStart1 = CPoint2D(pt2.x + arrowLength * cos(angle1), 
					pt2.y - arrowLength * sin(angle1));
				ptStart2 = CPoint2D(pt2.x + arrowLength * cos(angle2), 
					pt2.y - arrowLength * sin(angle2));
			}
			else
			{
				if(k == PI / 2)
				{
					ptStart1 = CPoint2D(pt2.x - arrowLength * cos(angle1), 
						pt2.y - arrowLength * sin(angle1));
					ptStart2 = CPoint2D(pt2.x - arrowLength * cos(angle2), 
						pt2.y - arrowLength * sin(angle2));
				}
				else
				{
					ptStart1 = CPoint2D(pt2.x - arrowLength * cos(angle1), 
						pt2.y + arrowLength * sin(angle1));
					ptStart2 = CPoint2D(pt2.x - arrowLength * cos(angle2), 
						pt2.y + arrowLength * sin(angle2));
				}
			}

		}
		else if(degree == 3 * PI / 2 || degree == PI / 2)
		{
			if(pt1.x == pt2.x)
				k = PI;
			else if(pt1.y == pt2.y)
				k = PI / 2;
			else 
			{
				k = atan(-((pt2.x - pt1.x) / pow(minor,2.0)) / ((pt2.y - pt1.y) / pow(major,2.0)));
				if(pt1.y > pt2.y)
					k += PI;
			}

			angle1 = k - PI / 5, angle2 = k + PI / 5;
			if(flag)
			{
				ptStart1 = CPoint2D(pt2.x + arrowLength * cos(angle1), 
					pt2.y - arrowLength * sin(angle1));
				ptStart2 = CPoint2D(pt2.x + arrowLength * cos(angle2), 
					pt2.y - arrowLength * sin(angle2));
			}
			else
			{
				if(k == PI)
				{
					ptStart1 = CPoint2D(pt2.x - arrowLength * cos(angle1), 
						pt2.y - arrowLength * sin(angle1));
					ptStart2 = CPoint2D(pt2.x - arrowLength * cos(angle2), 
						pt2.y - arrowLength * sin(angle2));
				}
				else
				{
					ptStart1 = CPoint2D(pt2.x + arrowLength * cos(angle1), 
						pt2.y - arrowLength * sin(angle1));
					ptStart2 = CPoint2D(pt2.x + arrowLength * cos(angle2), 
						pt2.y - arrowLength * sin(angle2));
				}
			}

		}
		else
		{
			length = GetDistance(pt1.x, pt1.y, pt2.x, pt2.y);
			k = atan((double)(pt2.y - pt1.y) / (double)(pt2.x - pt1.x));
			k -= degree;
			prePoint.x = length * cos(k) + pt1.x;
			prePoint.y = length * sin(k) + pt1.y;

			k = atan(-((prePoint.x - pt1.x) / pow(major,2.0)) / ((prePoint.y - pt1.y) / pow(minor,2.0)));
			k += degree;

			if(flag)
			{
				ptStart1 = CPoint2D(pt2.x + arrowLength * cos(angle1), 
					pt2.y - arrowLength * sin(angle1));
				ptStart2 = CPoint2D(pt2.x + arrowLength * cos(angle2), 
					pt2.y - arrowLength * sin(angle2));
			}
			else
			{
				if(k == PI)
				{
					ptStart1 = CPoint2D(pt2.x - arrowLength * cos(angle1), 
						pt2.y - arrowLength * sin(angle1));
					ptStart2 = CPoint2D(pt2.x - arrowLength * cos(angle2), 
						pt2.y - arrowLength * sin(angle2));
				}
				else
				{
					ptStart1 = CPoint2D(pt2.x + arrowLength * cos(angle1), 
						pt2.y - arrowLength * sin(angle1));
					ptStart2 = CPoint2D(pt2.x + arrowLength * cos(angle2), 
						pt2.y - arrowLength * sin(angle2));
				}
			}
		}
	}
	return CPointArray(pt2, ptStart1, ptStart2);
}

/***********************************
** 判断点是否在图形上
** 参数：
**			pGraph	图形
**			point	点坐标
** 时间：2013-10-23
***********************************/
BOOL CFunctionArray::PosInGraph(CGraph* pGraph, CPoint2D point)
{
	BOOL direction;
	double delta = 0, radius = 0, angle1 = 0, angle2 = 0, degree = 0, con = 0;
	CPoint2D startPoint, endPoint, centerPoint;
	double major = 0, minor = 0, Xpoint = 0, Ypoint = 0;
	double lastX, lastY;
	double startAngle = 0.0, endAngle = 0.0;
	double startX, startY, endX, endY;
	CArray<CPoint2D>pm;
	vector<CPoint2D> ellipsePoint;
	int nu, flags;
	double *pt;
	CPoint2D pTemp;
	double k = 0.0;
	CPoint2D lStartPoint, lEndPoint, rStartPoint, rEndPoint;
	double minX = 0.0, maxX = 0.0, minY = 0.0, maxY = 0.0;
	CRect largeRect, smallRect;

	vector<vec2>g_NodeArray;
	CBspline bCurve;

	UINT type = pGraph->m_iDrawType;

	switch(type)
	{
	case  2:
		startPoint = pGraph->m_ptOrigin;
		endPoint = pGraph->m_ptEnd;
		//

		if(startPoint.x == endPoint.x)
		{
			toSureRectenglePosition(startPoint, endPoint);
			if(abs(point.x - startPoint.x) <= MAXRANGE * g_fRate 
				&& point.y >= startPoint.y - MAXRANGE * g_fRate 
				&& point.y <= endPoint.y + MAXRANGE * g_fRate )
				return TRUE;
		}

		else if(startPoint.y == endPoint.y)
		{
			toSureRectenglePosition(startPoint, endPoint);
			if(abs(point.y - startPoint.y) <= MAXRANGE * g_fRate 
				&& point.x >= startPoint.x - MAXRANGE * g_fRate 
				&& point.x <= endPoint.x + MAXRANGE * g_fRate)
				return TRUE;
		}
		else 
		{	
			k = (double)(endPoint.y - startPoint.y) / (double)(endPoint.x - startPoint.x);
			delta = fabs(k * (point.x - startPoint.x) + startPoint.y - point.y) / sqrt(1 + pow(k, 2));

			lStartPoint = CalStartPoint(LINE, startPoint, endPoint, MAXRANGE, L"左刀补");
			lEndPoint = CalEndPoint(LINE, startPoint, endPoint, MAXRANGE, L"左刀补");
			rStartPoint = CalStartPoint(LINE, startPoint, endPoint, MAXRANGE, L"右刀补");
			rEndPoint = CalEndPoint(LINE, startPoint, endPoint, MAXRANGE, L"右刀补");

			minX = RetValue(FALSE, lStartPoint.x, lEndPoint.x, rStartPoint.x, rEndPoint.x);
			maxX = RetValue(TRUE, lStartPoint.x, lEndPoint.x, rStartPoint.x, rEndPoint.x);
			minY = RetValue(FALSE, lStartPoint.y, lEndPoint.y, rStartPoint.y, rEndPoint.y);
			maxY = RetValue(TRUE, lStartPoint.y, lEndPoint.y, rStartPoint.y, rEndPoint.y);

			if(fabs(delta) < MAXRANGE * g_fRate && point.x >= minX && point.x <= maxX 
				&& point.y >= minY && point.y <= maxY)
				return TRUE;
		}
		break;

	case  3:
		centerPoint = pGraph->m_ptCenter;
		radius = pGraph->m_fRadius;
		delta = sqrt(pow((double)(point.x - centerPoint.x), 2) 
			+ pow((double)(point.y - centerPoint.y), 2)) - radius;
		if (fabs(delta) < MAXRANGE * g_fRate)
			return TRUE;
		break;

	case 4:
		startPoint = pGraph->m_ptOrigin;
		endPoint = pGraph->m_ptEnd;
		centerPoint = pGraph->m_ptCenter;
		radius = pGraph->m_fRadius;
		direction = pGraph->m_bDirection;
		angle1 = pGraph->m_fAngle;
		delta = sqrt(pow((double)(point.x - centerPoint.x), 2) 
			+ pow((double)(point.y - centerPoint.y), 2)) - radius;
		if (fabs(delta) < MAXRANGE * g_fRate)
		{
			angle2 = GetAngle(startPoint, point, radius);

			if((startPoint.y - centerPoint.y) * (point.x - centerPoint.x) 
				- (point.y - centerPoint.y) * (startPoint.x - centerPoint.x) < 0)
			{
				if(direction)
				{		
					angle2 = 2 * PI - angle2;
				}
			}
			else
			{
				if(!direction)
				{
					angle2 = 2 * PI - angle2;
				}
			}

			if(angle1 > angle2)
				return TRUE;
		}
		break;

	case 5:
		startPoint = pGraph->m_ptOrigin;
		endPoint = pGraph->m_ptEnd;
		largeRect.left = startPoint.x - MAXRANGE;
		largeRect.top = startPoint.y + MAXRANGE;
		largeRect.right = endPoint.x + MAXRANGE;
		largeRect.bottom = endPoint.y - MAXRANGE;

		smallRect.left = startPoint.x + MAXRANGE;
		smallRect.top = startPoint.y - MAXRANGE;
		smallRect.right = endPoint.x - MAXRANGE;
		smallRect.bottom = endPoint.y + MAXRANGE;

		if((point.x >= largeRect.left && point.x <= largeRect.right && ((point.y >= smallRect.top && point.y <= largeRect.top) || (point.y >= largeRect.bottom && point.y <= smallRect.bottom))) 
			|| (point.y >= largeRect.bottom && point.y <= largeRect.top && ((point.x >= largeRect.left && point.x <= smallRect.left) || (point.x >= smallRect.right && point.x <= largeRect.right))))
			return TRUE;
		break;

	case  6:
		centerPoint = pGraph->m_ptCenter;
		degree = pGraph->m_fDegree;
		major = pGraph->m_fMajor;
		minor = pGraph->m_fMinor;
		direction = pGraph->m_bDirection;
		startAngle = pGraph->m_fStartAngle;
		endAngle = pGraph->m_fEndAngle;

		for(int i = startAngle * 180 / PI; i < endAngle * 180 / PI; ++i)	
		{
			pTemp = GetEllipsePoint(major, minor, degree, centerPoint, i, direction);
			//Xpoint = GetEllipsePoint(major,minor,degree,centerPoint,i,direction);
			//Ypoint = GetEllipsePoint(major,minor,degree,centerPoint,i,direction);
			Xpoint = pTemp.x;
			Ypoint = pTemp.y;
			delta = sqrt(pow(Xpoint - point.x, 2) + pow(Ypoint - point.y, 2));
			if(fabs(delta) < MAXRANGE * g_fRate)
				return TRUE;
		}
		break;

	case 7:
		nu = pGraph->m_iVertexNum;
		pm.Copy(pGraph->m_PeakGroup);
		flags = pGraph->m_bFlags;
		pt = new double[nu * 3];

		for(int j = 0; j < nu; ++j)
		{
			pt[j * 3] = pm[j].x;
			pt[j * 3 + 1] = pm[j].y;
			pt[j * 3 + 2] = pm[j].con;
		}

		int j;
		for(j = 0; j < nu - 1; ++j)
		{
			startX = pt[j * 3], startY = pt[j * 3 + 1];
			endX = pt[(j + 1) * 3], endY = pt[(j + 1) * 3 + 1];
			con = pt[j * 3 + 2];

			if(con == 0.0)
			{
				if(startX == endX)
				{
					if(startY > endY)
						SwapElement(startY, endY);
					if(abs(point.x - startX) <= MAXRANGE * g_fRate && point.y >= startY && point.y <= endY)
					{
						pGraph->m_SelectedNum = j;
						return TRUE;
					}
				}

				else if(startY == endY)
				{
					if(startX > endX)
						SwapElement(startX, endX);
					if(abs(point.y - startY) <= MAXRANGE * g_fRate && point.x >= startX && point.x <= endX)
					{
						pGraph->m_SelectedNum = j;
						return TRUE;
					}
				}
				else 
				{	
					k = (double)(endY - startY) / (double)(endX - startX);
					delta = fabs(k * (point.x - startX) + startY - point.y) / sqrt(1 + pow(k, 2));

					if(startX > endX)
						SwapElement(startX, endX);
					if(startY > endY)
						SwapElement(startY, endY);
					if(fabs(delta) < MAXRANGE * g_fRate && point.x >= startX && point.x <= endX && point.y >= startY && point.y <= endY)
					{
						pGraph->m_SelectedNum = j;
						return TRUE;
					}
				}
			}
			else
			{
				angle1 = 4 * atan(fabs(con));
				radius = GetDistance(startX, startY, endX, endY) / (2 * sin(angle1 / 2));

				if(con > 0)
					direction = FALSE;						
				else
					direction = TRUE;

				centerPoint = GetCenterPoint(startX, startY, endX, endY, radius, angle1, direction);

				delta = sqrt(pow((double)(point.x - centerPoint.x), 2) 
					+ pow((double)(point.y - centerPoint.y), 2)) - radius;
				if (fabs(delta) < MAXRANGE * g_fRate)
				{
					angle2 = GetAngle(CPoint2D(startX, startY), point, radius);

					if((startY - centerPoint.y) * (point.x - centerPoint.x) 
						- (point.y - centerPoint.y) * (startX - centerPoint.x) < 0)
					{
						if(direction)
						{		
							angle2 = 2 * PI - angle2;
						}
					}
					else
					{
						if(!direction)
						{
							angle2 = 2 * PI - angle2;
						}
					}

					if(angle1 > angle2)
					{
						pGraph->m_SelectedNum = j;
						return TRUE;
					}
				}

			}
		}

		if(flags)
		{
			startX = pt[(nu - 1) * 3], startY = pt[(nu - 1) * 3 + 1];
			endX = pt[0], endY = pt[1]; 
			con = pt[(nu - 1) * 3 + 2];

			if(con == 0.0)
			{
				if(startX == endX)
				{
					if(startY > endY)
						SwapElement(startY, endY);
					if(abs(point.x - startX) <= MAXRANGE * g_fRate && point.y >= startY && point.y <= endY)
					{
						pGraph->m_SelectedNum = j;
						return TRUE;
					}
				}

				else if(startY == endY)
				{
					if(startX > endX)
						SwapElement(startX, endX);
					if(abs(point.y - startY) <= MAXRANGE * g_fRate && point.x >= startX && point.x <= endX)
					{
						pGraph->m_SelectedNum = j;
						return TRUE;
					}
				}
				else 
				{	
					k = (double)(endY - startY) / (double)(endX - startX);
					delta = fabs(k * (point.x - startX) + startY - point.y) / sqrt(1 + pow(k, 2));

					if(startX > endX)
						SwapElement(startX, endX);
					if(startY > endY)
						SwapElement(startY, endY);

					if(fabs(delta) < MAXRANGE * g_fRate && point.x >= startX && point.x <= endX && point.y >= startY && point.y <= endY)
					{
						pGraph->m_SelectedNum = j;
						return TRUE;
					}
				}

			}
			else
			{
				angle1 = 4 * atan(fabs(con));
				radius = GetDistance(startX, startY, endX, endY) / (2 * sin(angle1 / 2));

				if(con > 0)
					direction = FALSE;						
				else
					direction = TRUE;

				centerPoint = GetCenterPoint(startX, startY, endX, endY, radius, angle1, direction);

				delta = sqrt(pow((double)(point.x - centerPoint.x), 2) 
					+ pow((double)(point.y - centerPoint.y), 2)) - radius;
				if (fabs(delta) < MAXRANGE * g_fRate)
				{
					angle2 = GetAngle(CPoint2D(startX, startY), point, radius);

					if((startY - centerPoint.y) * (point.x - centerPoint.x) 
						- (point.y - centerPoint.y) * (startX - centerPoint.x) < 0)
					{
						if(direction)
						{
							angle2 = 2 * PI - angle2;
						}
					}
					else
					{
						if(!direction)
						{
							angle2 = 2 * PI - angle2;
						}
					}

					if(angle1 > angle2)
					{
						pGraph->m_SelectedNum = j;
						return TRUE;
					}
				}
			}

		}
		break;

	case 8:
		g_NodeArray = vector<vec2>(pGraph->controlNode.begin(), pGraph->controlNode.end());
		bCurve = CBspline(g_NodeArray);

		bCurve.DrawInterpolation();

		for(int j = 0; j < bCurve.m_interpolationNode.size(); ++j)
		{
			lastX = bCurve.m_interpolationNode[j][0], lastY = bCurve.m_interpolationNode[j][1];
			delta = GetDistance(lastX, lastY, point.x, point.y);
			if(fabs(delta) < MAXRANGE * g_fRate)
				return TRUE;
		}
		break;

	case 9:
		startX = pGraph->m_ptOrigin.x;
		startY = pGraph->m_ptOrigin.y;

		if(fabs(point.x - startX) < 5 && fabs(point.y - startY) < 5)
			return TRUE;
	}
	return FALSE;
}

/***********************************
** 获取圆弧的中心坐标
** 参数：
**			x1	起点的y坐标
**			y1	起点的y坐标
**			x2	终点的x坐标
**			y2	终点的y坐标
**			r		最后一点
**			angle	圆弧起点和终点的圆弧角
**			direction	圆弧方向
** 时间：2013-10-23
***********************************/
CPoint2D CFunctionArray::GetCenterPoint(double x1, double y1, double x2, double y2, 
										double r, double angle, BOOL direction)
{
	CPoint2D center;
	double con = 0, height = 0;;
	double k = 0;
	double midX = (x1 + x2) / 2.0;
	double midY = (y1 + y2) / 2.0;

	double length = r * cos(angle / 2);

	if(y1 == y2)
	{
		center.x = midX;
		con = angle / 2.0;
		height = fabs(x2 - x1) / (2 * tan(con));
		if(direction)
		{
			if(x1 <= x2)
				center.y = y1 - height;
			else
				center.y = y1 + height;
		}
		else
		{
			if(x1 <= x2)
				center.y = y1 + height;
			else
				center.y = y1 - height;
		}
	}
	else
	{
		k = (x1 - x2) / (y2 - y1);
		if(direction)
		{
			if(y1 < y2)
			{
				center.x = midX + length * cos(atan(k));
				center.y = midY + length * sin(atan(k));
			}
			else
			{
				center.x = midX - length * cos(atan(k));
				center.y = midY - length * sin(atan(k));
			}
		}
		else
		{
			if(y1 < y2)
			{
				center.x = midX - length * cos(atan(k));
				center.y = midY - length * sin(atan(k));
			}
			else
			{
				center.x = midX + length * cos(atan(k));
				center.y = midY + length * sin(atan(k));
			}
		}
	}
	return center;
}

/***********************************
** 获取两点之间的斜率
** 参数：
**			firstX	起点的x坐标
**			firstX	起点的y坐标
**			endX	终点的x坐标
**			endX	终点的y坐标
** 时间：2013-10-23
***********************************/
double CFunctionArray::GetAngle(double firstX, double firstY, double endX, double endY)
{
	double c;
	if(firstX == endX)
	{
		if(firstY < endY)
			c = PI / 2.0;
		else
			c = 3 * PI / 2.0;
	}
	else if(firstY == endY)
	{
		if(firstX < endX)
			c = 0;
		else
			c = PI;
	}
	else
	{
		c = atan((endY - firstY) / (endX - firstX));
		if(endY - firstY > 0.0 && endX - firstX > 0.0)//第一象限
			;
		if(endY - firstY > 0.0 && endX - firstX < 0.0)//第二象限
			c += PI;
		if(endY - firstY < 0.0 && endX - firstX > 0.0)//第四象限
			c += PI * 2;
		if(endY - firstY < 0.0 && endX - firstX < 0.0)//第三象限
			c += PI;
	}
	return c;
}

//与GetAngle(double firstX,double firstY,double endX,double endY);配套使用
void CFunctionArray::GetRigthAngle(CPoint2D start, CPoint2D end, CPoint2D center, BOOL direction, double* angle)
{
	if((start.y - center.y) * (end.x - center.x) 
		- (end.y - center.y) * (start.x - center.x) < 0)
	{
		if(direction)
		{
			*angle = 2 * PI - (*angle);
		}
	}
	else
	{
		if(!direction)
		{
			*angle = 2 * PI - (*angle);
		}
	}
}

//交换两个数值
void CFunctionArray::SwapElement(double &a1, double &a2)
{
	double tempA;
	tempA = a1;
	a1 = a2;
	a2 = tempA;
}


CPointArray CFunctionArray::GetPt2Array(double startX, double startY, double endX, double endY, 
										double con)
{
	CPointArray pt2Array;
	double angle, r;
	BOOL direction;
	CPoint2D centerPoint;

	angle = 4 * atan(fabs(con));
	r = GetDistance(startX, startY, endX, endY) / (2 * sin(angle / 2));

	if(con > 0)
		direction = FALSE;						
	else
		direction = TRUE;

	centerPoint = GetCenterPoint(startX, startY, endX, endY, r, angle, direction);
	pt2Array = CPointArray(centerPoint, CPoint2D(endX, endY));
	return pt2Array;
}


double CFunctionArray::GetNewStep(double length, double oldStep)
{
	int count = length / oldStep;
	double sub = length - count * oldStep;
	double subStep = sub / count;
	double newStep = oldStep + subStep;
	return newStep;
}

/***********************************
** 将屏幕坐标转换为第一象限坐标函数
** 参数：
**			point	屏幕坐标
** 时间：2013-5-10
***********************************/
CPoint2D CFunctionArray::ScreenToMyQuadrant(CPoint2D point)
{
	point.x = point.x - 20 - g_hSetOffset;
	point.y = g_Rect.Height() - point.y - g_vSetOffset;
	return point;
}

//重载ScreenToMyQuadrant函数以（double，double）作为参数
CPoint2D CFunctionArray::ScreenToMyQuadrant(double p1, double p2)
{
	return ScreenToMyQuadrant(CPoint2D(p1, p2));
}

//重载ScreenToMyQuadrant函数，以CPoint作为参数
CPoint2D CFunctionArray::ScreenToMyQuadrant(CPoint point)
{
	return ScreenToMyQuadrant(CPoint2D(point.x, point.y));
}


/***********************************
** 将转换后的点恢复到窗口坐标
** 参数：
**			point	需要转换的点坐标
** 时间：2013-10-23
***********************************/
CPoint2D CFunctionArray::MyQuadrantToScreen(CPoint2D point)
{
	point.x += 20 + g_hSetOffset;
	point.y = g_Rect.Height() - point.y - g_vSetOffset;
	return point;
}

void CFunctionArray::MyQuadrantToScreen1(CPoint2D& point)
{
	point.x += 20 + g_hSetOffset;
	point.y = g_Rect.Height() - point.y - g_vSetOffset;
	//return point;
}

//重载
CPoint2D CFunctionArray::MyQuadrantToScreen(int x, int y)
{
	return MyQuadrantToScreen(CPoint2D(x, y));
}


double CFunctionArray::GetNewDepth(double oldDepth)
{
	int old = -oldDepth * 10 + 1;
	if(fabs(old + oldDepth) < 1e-6)
		return old / 10;
	else
		return oldDepth;
}

CPoint2D CFunctionArray::CalStartPoint(UINT graphType, CPoint2D startPoint, CPoint2D endPoint/*centerPoint*/, 
									   double toolRadius, CString strToolWays, BOOL direction /*= FALSE*/)
{
	double lineC = 0, startAngle = 0, arcRadius = 0;
	switch(graphType)
	{
	case LINE:
		lineC = GetAngle(startPoint.x, startPoint.y, endPoint.x, endPoint.y);

		if(strToolWays == L"左刀补")
		{
			startPoint += CPoint2D(-toolRadius * sin(lineC), toolRadius * cos(lineC));
		}
		else if(strToolWays == L"右刀补")
		{
			startPoint += CPoint2D(toolRadius * sin(lineC), -toolRadius * cos(lineC));
		}

		break;

	case ARC:
		startAngle = GetAngle(endPoint.x, endPoint.y, startPoint.x, startPoint.y);
		arcRadius = GetDistance(endPoint.x, endPoint.y, startPoint.x, startPoint.y);

		if(direction)
		{	
			if(strToolWays == L"左刀补")
			{
				arcRadius += toolRadius;
			}
			else
			{
				arcRadius -= toolRadius;
			}
		}
		else
		{
			if(strToolWays == L"左刀补")
			{
				arcRadius -= toolRadius;
			}
			else
			{
				arcRadius += toolRadius;
			}
		}
		startPoint = CPoint2D(endPoint.x + arcRadius * cos(startAngle), 
			endPoint.y + arcRadius * sin(startAngle));

		break;
	}
	return startPoint;
}

CPoint2D CFunctionArray::CalEndPoint(UINT graphType, CPoint2D startPoint/*endPoint*/, CPoint2D endPoint/*centerPoint*/, 
									 double toolRadius, CString strToolWays, BOOL direction /*= FALSE*/)
{
	double lineC = 0, endLine = 0, arcRadius = 0;
	switch(graphType)
	{
	case LINE:
		lineC = GetAngle(startPoint.x, startPoint.y, endPoint.x, endPoint.y);

		if(strToolWays == L"左刀补")
		{
			endPoint += CPoint2D(-toolRadius * sin(lineC), toolRadius * cos(lineC));
		}
		else if(strToolWays == L"右刀补")
		{
			endPoint += CPoint2D(toolRadius * sin(lineC), -toolRadius * cos(lineC));
		}

		break;

	case ARC:
		endLine = GetAngle(endPoint.x, endPoint.y, startPoint.x, startPoint.y);
		arcRadius = GetDistance(endPoint.x, endPoint.y, startPoint.x, startPoint.y);

		if(direction)
		{	
			if(strToolWays == L"左刀补")
			{
				arcRadius += toolRadius;
			}
			else
			{
				arcRadius -= toolRadius;
			}
		}
		else
		{
			if(strToolWays == L"左刀补")
			{
				arcRadius -= toolRadius;
			}
			else
			{
				arcRadius += toolRadius;
			}
		}
		endPoint = CPoint2D(endPoint.x + arcRadius * cos(endLine), 
			endPoint.y + arcRadius * sin(endLine));

		break;
	}
	return endPoint;
}

BOOL CFunctionArray::Compare2Points(CPoint2D pm1, CPoint2D pm2)
{
	if(fabs(pm1.x - pm2.x) < 1e-3 && fabs(pm1.y - pm2.y) < 1e-3)
		return TRUE;

	return FALSE;
}


int CFunctionArray::RetPos(CGraph* pGraph, CPoint2D point)
{
	BOOL direction;
	double delta = 0, radius = 0, angle1 = 0, angle2 = 0, degree = 0, con = 0;
	CPoint2D startPoint, endPoint, centerPoint;
	double startX, startY, endX, endY;
	CArray<CPoint2D>pm;
	int nu, flags;
	double *pt;

	nu = pGraph->m_iVertexNum;
	pm.Copy(pGraph->m_PeakGroup);
	flags = pGraph->m_bFlags;
	pt = new double[nu * 3];

	for(int j = 0; j < nu; ++j)
	{
		pt[j * 3] = pm[j].x;
		pt[j * 3 + 1] = pm[j].y;
		pt[j * 3 + 2] = pm[j].con;
	}

	for(int j = 0; j < nu - 1; ++j)
	{
		startX = pt[j * 3], startY = pt[j * 3 + 1];
		endX = pt[(j + 1) * 3], endY = pt[(j + 1) * 3 + 1];
		con = pt[j * 3 + 2];

		if(con == 0.0)
		{
			if(startX > endX)	SwapElement(startX, endX);
			if(startY > endY)	SwapElement(startY, endY);

			if(startX == endX)
			{
				if(abs(point.x - startX) <= MAXRANGE * g_fRate)
					return j;
			}

			else if(startY == endY)
			{
				if(abs(point.y - startY) <= MAXRANGE * g_fRate)
					return j;
			}
			else 
			{	
				delta = fabs((double)(endY - startY) / (double)(endX - startX) * (point.x - startX) + startY - point.y) 
					/ sqrt(1 + pow((double)(endY - startY) / (double)(endX - startX), 2));
				if(fabs(delta) < MAXRANGE * g_fRate)
					return j;
			}

		}
		else
		{
			angle1 = 4 * atan(fabs(con));
			radius = GetDistance(startX, startY, endX, endY) / (2 * sin(angle1 / 2));

			if(con > 0)
				direction = FALSE;						
			else
				direction = TRUE;

			centerPoint = GetCenterPoint(startX, startY, endX, endY, radius, angle1, direction);

			delta = sqrt(pow((double)(point.x - centerPoint.x), 2) 
				+ pow((double)(point.y - centerPoint.y), 2)) - radius;
			if (fabs(delta) < MAXRANGE * g_fRate)
			{
				angle2 = GetAngle(CPoint2D(startX, startY), point, radius);

				if((startY - centerPoint.y) * (point.x - centerPoint.x) 
					- (point.y - centerPoint.y) * (startX - centerPoint.x) < 0)
				{
					if(direction)
					{		
						angle2 = 2 * PI - angle2;
					}
				}
				else
				{
					if(!direction)
					{
						angle2 = 2 * PI - angle2;
					}
				}

				if(angle1 > angle2)
					return j;
			}

		}

	}
	if(flags)
	{
		startX = pt[(nu - 1) * 3], startY = pt[(nu - 1) * 3 + 1];
		endX = pt[0], endY = pt[1];
		con = pt[(nu - 1) * 3 + 2];

		if(con == 0.0)
		{
			if(startX > endX)	SwapElement(startX, endX);
			if(startY > endY)	SwapElement(startY, endY);

			if(startX == endX)
			{
				if(abs(point.x - startX) <= MAXRANGE * g_fRate)
					return (nu - 1);
			}

			else if(startY == endY)
			{
				if(abs(point.y - startY) <= MAXRANGE * g_fRate)
					return (nu - 1);
			}
			else 
			{	
				delta = fabs((double)(endY - startY) / (double)(endX - startX) * (point.x - startX) + startY - point.y) 
					/ sqrt(1 + pow((double)(endY - startY) / (double)(endX - startX), 2));
				if(fabs(delta) < MAXRANGE * g_fRate)
					return (nu - 1);
			}

		}
		else
		{
			angle1 = 4 * atan(fabs(con));
			radius = GetDistance(startX, startY, endX, endY) / (2 * sin(angle1 / 2));

			if(con > 0)
				direction = FALSE;						
			else
				direction = TRUE;

			centerPoint = GetCenterPoint(startX, startY, endX, endY, radius, angle1, direction);

			delta = sqrt(pow((double)(point.x - centerPoint.x), 2) 
				+ pow((double)(point.y - centerPoint.y), 2)) - radius;
			if (fabs(delta) < MAXRANGE * g_fRate)
			{
				angle2 = GetAngle(CPoint2D(startX, startY), point, radius);

				if((startY - centerPoint.y) * (point.x - centerPoint.x) 
					- (point.y - centerPoint.y) * (startX - centerPoint.x) < 0)
				{
					if(direction)
					{		
						angle2 = 2 * PI - angle2;
					}
				}
				else
				{
					if(!direction)
					{
						angle2 = 2 * PI - angle2;
					}
				}

				if(angle1 > angle2)
					return (nu - 1);
			}

		}
	}
	delete pt;
	return ERRORCODE;

}

BOOL CFunctionArray::PosInPoly(CGraph* pGraph, CPoint2D point)
{
	BOOL direction;
	double delta = 0, radius = 0, angle1 = 0, angle2 = 0, degree = 0, con = 0, k = 0.0;
	CPoint2D startPoint, endPoint, centerPoint;
	double startX, startY, endX, endY;
	CArray<CPoint2D>pm;
	int nu, flags;
	double *pt;

	if(pGraph->m_iDrawType != 7)
		return FALSE;

	nu = pGraph->m_iVertexNum;
	pm.Copy(pGraph->m_PeakGroup);
	flags = pGraph->m_bFlags;
	pt = new double[nu * 3];

	for(int j = 0; j < nu; ++j)
	{
		pt[j * 3] = pm[j].x;
		pt[j * 3 + 1] = pm[j].y;
		pt[j * 3 + 2] = pm[j].con;
	}

	for(int j = 0; j < nu - 1; ++j)
	{
		startX = pt[j * 3], startY = pt[j * 3 + 1];
		endX = pt[(j + 1) * 3], endY = pt[(j + 1) * 3 + 1];
		con = pt[j * 3 + 2];

		if(con == 0.0)
		{
			if(startX == endX)
			{
				if(startX > endX)	SwapElement(startX, endX);
				if(startY > endY)	SwapElement(startY, endY);

				if(abs(point.x - startX) <= MAXRANGE * g_fRate)
					return TRUE;
			}

			else if(startY == endY)
			{
				if(startX > endX)	SwapElement(startX, endX);
				if(startY > endY)	SwapElement(startY, endY);

				if(abs(point.y - startY) <= MAXRANGE * g_fRate)
					return TRUE;
			}
			else 
			{	
				k = (double)(endY - startY) / (double)(endX - startX);
				delta = fabs(k * (point.x - startX) + startY - point.y) / sqrt(1 + pow(k, 2));

				if(startX > endX)	SwapElement(startX, endX);
				if(startY > endY)	SwapElement(startY, endY);

				if(fabs(delta) < MAXRANGE * g_fRate)
					return TRUE;
			}

		}
		else
		{
			angle1 = 4 * atan(fabs(con));
			radius = GetDistance(startX, startY, endX, endY) / (2 * sin(angle1 / 2));

			if(con > 0)
				direction = FALSE;						
			else
				direction = TRUE;

			centerPoint = GetCenterPoint(startX, startY, endX, endY, radius, angle1, direction);

			delta = sqrt(pow((double)(point.x - centerPoint.x), 2) 
				+ pow((double)(point.y - centerPoint.y), 2)) - radius;
			if (fabs(delta) < MAXRANGE * g_fRate)
			{
				angle2 = GetAngle(CPoint2D(startX, startY), point, radius);

				if((startY - centerPoint.y) * (point.x - centerPoint.x) 
					- (point.y - centerPoint.y) * (startX - centerPoint.x) < 0)
				{
					if(direction)
					{		
						angle2 = 2 * PI - angle2;
					}
				}
				else
				{
					if(!direction)
					{
						angle2 = 2 * PI - angle2;
					}
				}

				if(angle1 > angle2)
					return TRUE;
			}

		}

		if(flags)
		{
			startX = pt[(nu - 1) * 3], startY = pt[(nu - 1) * 3 + 1];
			endX = pt[0], endY = pt[1];
			con = pt[(nu - 1) * 3 + 2];

			if(con == 0.0)
			{
				if(startX == endX)
				{
					if(startX > endX)	SwapElement(startX, endX);
					if(startY > endY)	SwapElement(startY, endY);

					if(abs(point.x - startX) <= MAXRANGE * g_fRate)
						return TRUE;
				}

				else if(startY == endY)
				{
					if(startX > endX)	SwapElement(startX, endX);
					if(startY > endY)	SwapElement(startY, endY);

					if(abs(point.y - startY) <= MAXRANGE * g_fRate)
						return TRUE;
				}
				else 
				{	
					k = (double)(endY - startY) / (double)(endX - startX);
					delta = fabs(k * (point.x - startX) + startY - point.y) / sqrt(1 + pow(k, 2));
					if(startX > endX)	SwapElement(startX, endX);
					if(startY > endY)	SwapElement(startY, endY);

					if(fabs(delta) < MAXRANGE * g_fRate)
						return TRUE;
				}

			}
			else
			{
				angle1 = 4 * atan(fabs(con));
				radius = GetDistance(startX, startY, endX, endY) / (2 * sin(angle1 / 2));

				if(con > 0)
					direction = FALSE;						
				else
					direction = TRUE;

				centerPoint = GetCenterPoint(startX, startY, endX, endY, radius, angle1, direction);

				delta = sqrt(pow((double)(point.x - centerPoint.x), 2) 
					+ pow((double)(point.y - centerPoint.y), 2)) - radius;
				if (fabs(delta) < MAXRANGE * g_fRate)
				{
					angle2 = GetAngle(CPoint2D(startX, startY), point, radius);

					if((startY - centerPoint.y) * (point.x - centerPoint.x) 
						- (point.y - centerPoint.y) * (startX - centerPoint.x) < 0)
					{
						if(direction)
						{
							angle2 = 2 * PI - angle2;
						}
					}
					else
					{
						if(!direction)
						{
							angle2 = 2 * PI - angle2;
						}
					}

					if(angle1 > angle2)
						return TRUE;
				}

			}
		}
	}
	delete pt;
	return FALSE;

}
double CFunctionArray::RetValue(bool maxOrMin, double ls, double le, double rs, double re)
{
	double retV = 0.0;
	double t1 = 0.0, t2 = 0.0;//中间临时变量

	if(maxOrMin)//求最大值
	{
		t1 = ls > le ? ls : le;
		t2 = rs > re ? rs : re;
		retV = t1 > t2 ? t1 : t2;
	}
	else//求最小值
	{
		t1 = ls < le ? ls : le;
		t2 = rs < re ? rs : re;
		retV = t1 < t2 ? t1 : t2;
	}
	return retV;
}

CPoint2D CFunctionArray::GetCirclePoint(CPoint2D centerPoint, double radius, int degree)
{
	CPoint2D pm;
	pm.x = radius * cos(degree * PI / 180) + centerPoint.x;
	pm.y = radius * sin(degree * PI / 180) + centerPoint.y;

	return pm;
}

CPoint2D CFunctionArray::GetNextPoint(int type, double temX, double temY, double ox, double oy, double unit, 
									  int i, BOOL direction)
{
	double x = 0, y = 0;
	if(direction)
	{
		x = (temX - ox) * cos(unit) + (temY - oy) * sin(unit) + ox;
		y = (temY - oy) * cos(unit) - (temX - ox) * sin(unit) + oy;
	}	
	else
	{
		x = (temX - ox) * cos(unit) - (temY - oy) * sin(unit) + ox;
		y = (temY - oy) * cos(unit) + (temX - ox) * sin(unit) + oy;
	}

	return CPoint2D(x, y);
}

CPoint2D CFunctionArray::CalculateNewCenterPoint(CPoint2D startPoint, CPoint2D endPoint, double radius, BOOL dir, BOOL isOverPI)
{
	double Minim = 1e-6;
	double I1, I2, K1, K2, r, k, d, p, q, thita, cthita, sthita, dcthita, dsthita;
	double DelX, DelZ, zs, ze, xs, xe, a[2], b[2], center[2];    
	double DelSX, DelSY, S = 0.0, L = 0.0;


	a[0] = startPoint.x;
	a[1] = startPoint.y;		
	b[0] = endPoint.x;
	b[1] = endPoint.y;				


	DelSX = b[0] - a[0];
	DelSY = b[1] - a[1];

	L = sqrt(1.0 * (DelSX) * (DelSX) + (DelSY) * (DelSY));

	r = radius;

	p = (b[1] + a[1]) / 2.0;
	q = (b[0] + a[0]) / 2.0;
	S = L / 2.0;	

	d = sqrt(1.0 * r * r - S * S);

	DelX = b[0] - a[0];
	DelZ = b[1] - a[1];

	if (fabs(DelX) < Minim)
	{//k无穷大
		I1 = q - d; 
		I2 = q + d;
		K1 = p;
		K2 = p;
	}
	else if (fabs(DelZ) < Minim) 
	{//k=0
		K1 = p - d;
		K2 = p + d;
		I1 = q;
		I2 = q;
	}
	else
	{
		k = (-1) * (DelZ) / (DelX);
		if (k > 0)			
			thita = atan(k);			
		else//k<0		    
			thita = PI + atan(k);           

		cthita = cos(thita);
		sthita = cthita * k;

		dcthita = d * cthita;
		dsthita = d * sthita;	   

		K1 = p + dcthita;
		K2 = p - dcthita;

		I1 = q + dsthita;
		I2 = q - dsthita;		
	}

	zs = a[1] - K1;
	xs = a[0] - I1;
	ze = b[1] - K1;
	xe = b[0] - I1;   

	if ((zs * xe - xs * ze) < 0) 
	{
		if ((!dir && !isOverPI) || (dir && isOverPI)) 
		{
			center[0] = I1;
			center[1] = K1;
		} 
		else
		{
			center[0] = I2;
			center[1] = K2;
		}
	} 
	else 
	{
		if ((!dir && !isOverPI) || (dir && isOverPI))
		{
			center[0] = I2;
			center[1] = K2;
		} 
		else
		{
			center[0] = I1;
			center[1] = K1;
		}
	}

	return CPoint2D(center[0], center[1]);	
}

//返回下一个最短路径的索引
int CFunctionArray::ReturnIndexOfMinDistance(CPtrArray &arr, const CPoint2D basePoint)
{
	CPoint2D ptCurS, ptCurE, lastS, sp0, ep0;
	int index, pos, nSection = 0;
	INT_PTR count;
	double len = 0.0, minLen = 0.0, fLen1 = 0.0, fLen2 = 0.0;
	CGraph *FindGraph = NULL;
	BOOL bIsPolyLine = FALSE, bCurPolyLine = FALSE;
	int iIndex;

	lastS = basePoint;
	count = g_LineInfoArray.GetCount();

	if(count != 1)
	{
		index = (g_LineInfoArray.GetAt(0)).addrNo;
		FindGraph = (CGraph*)arr.GetAt(index);
		bIsPolyLine = FindGraph->m_bIsPolyLine;
		iIndex = FindGraph->m_IndexOfPolyLine;

		sp0 = (g_LineInfoArray.GetAt(0)).sP;
		ep0 = (g_LineInfoArray.GetAt(0)).eP;

		fLen1 = GetDistance(lastS, sp0);
		fLen2 = GetDistance(lastS, ep0);

		if(!bIsPolyLine)
		{
			if(fLen1 <= fLen2)
			{
				minLen = fLen1;
			}
			else
			{
				minLen = fLen2;
				SwapPoint((g_LineInfoArray.GetAt(0)).sP, (g_LineInfoArray.GetAt(0)).eP);
				DirectionReserve((CGraph*)arr.GetAt(index));
			}
		}
		else
		{
			nSection = FindGraph->m_nCount;
			minLen = fLen1;
		}

		//index = (g_LineInfoArray.GetAt(0)).addrNo;
		pos = 0;

		for(int i = 1; i < count; )
		{
			//如果第一段是多段线，跳过之后的n段
			if(!bIsPolyLine)
			{
				if(!(bCurPolyLine = ((CGraph*)arr.GetAt(g_LineInfoArray.GetAt(i).addrNo))->m_bIsPolyLine))
				{
					ptCurS = (g_LineInfoArray.GetAt(i)).sP;
					ptCurE = (g_LineInfoArray.GetAt(i)).eP;

					fLen1 = GetDistance(lastS, ptCurS);
					fLen2 = GetDistance(lastS, ptCurE);

					if(fLen1 <= fLen2)
					{
						len = fLen1;
					}
					else
					{
						len = fLen2;
						SwapPoint((g_LineInfoArray.GetAt(i)).sP, (g_LineInfoArray.GetAt(i)).eP);
						DirectionReserve((CGraph*)arr.GetAt((g_LineInfoArray.GetAt(i)).addrNo));
					}				

					if(minLen > len)
					{
						minLen = len;
						index = (g_LineInfoArray.GetAt(i)).addrNo;
						pos = i;
						//iIndex = ((CGraph*)arr.GetAt(index))->m_IndexOfPolyLine;
					}
					++i;
				}
				else
				{
					ptCurS = (g_LineInfoArray.GetAt(i)).sP;
					len = fLen1 = GetDistance(lastS, ptCurS);

					if(minLen > len)
					{
						minLen = len;
						index = (g_LineInfoArray.GetAt(i)).addrNo;
						pos = i;
						//iIndex = ((CGraph*)arr.GetAt(index))->m_IndexOfPolyLine;
					}

					nSection = ((CGraph*)arr.GetAt(g_LineInfoArray.GetAt(i).addrNo))->m_nCount;
					i += nSection;
				}
			}
			else
			{
				i += nSection - 1;
				bIsPolyLine = FALSE;
				continue;
			}
		}
	}
	else
	{
		sp0 = (g_LineInfoArray.GetAt(0)).sP;
		ep0 = (g_LineInfoArray.GetAt(0)).eP;

		fLen1 = GetDistance(lastS, sp0);
		fLen2 = GetDistance(lastS, ep0);

		if(fLen1 <= fLen2)
		{
			minLen = fLen1;
		}
		else
		{
			minLen = fLen2;
			SwapPoint((g_LineInfoArray.GetAt(0)).sP, (g_LineInfoArray.GetAt(0)).eP);
			DirectionReserve((CGraph*)arr.GetAt((g_LineInfoArray.GetAt(0)).addrNo));
		}

		index = (g_LineInfoArray.GetAt(0)).addrNo;
		pos = 0;
	}
	
	CGraph *pp = (CGraph*)arr.GetAt((g_LineInfoArray.GetAt(pos)).addrNo);
	if(pp->m_bIsPolyLine)
	{
		nSection = pp->m_nCount;
		while(nSection--)
			g_LineInfoArray.RemoveAt(pos);
	}
	else
	{
		g_LineInfoArray.RemoveAt(pos);
	}
	return index;
}

int CFunctionArray::ReturnIndexOfMinDistance(const CPoint2D fPoint, __out double* dis, __out BOOL* isReserve)
{
	CPoint2D spt, ept;
	CPoint2D ptCurS, curE, lastS = fPoint;
	int index;
	double len1 = 0.0, len2 = 0.0, minLen = 0.0, len = 0.0;

	INT_PTR count = g_LineInfoArray.GetSize();
	if(count > 1)
	{
		ptCurS = g_LineInfoArray.GetAt(0).sP;
		curE = g_LineInfoArray.GetAt(0).eP;

		len1 = GetDistance(fPoint, ptCurS);
		len2 = GetDistance(fPoint, curE);

		minLen = len1 < len2 ? len1 : len2;
		if(len1 < len2)
			*isReserve = FALSE;
		else
			*isReserve = TRUE;

		index = (g_LineInfoArray.GetAt(0)).addrNo;;

		for(int i = 1; i < count; ++i)
		{
			ptCurS = g_LineInfoArray.GetAt(i).sP;
			curE = g_LineInfoArray.GetAt(i).eP;

			len1 = GetDistance(fPoint, ptCurS);
			len2 = GetDistance(fPoint, curE);

			len = len1 < len2 ? len1 : len2;
			if(minLen > len)
			{
				minLen = len;

				if(len1 < len2)
					*isReserve = FALSE;
				else
					*isReserve = TRUE;

				index = (g_LineInfoArray.GetAt(i)).addrNo;
			}

		}
	}
	else
	{
		ptCurS = g_LineInfoArray.GetAt(0).sP;
		curE = g_LineInfoArray.GetAt(0).eP;

		len1 = GetDistance(fPoint, ptCurS);
		len2 = GetDistance(fPoint, curE);

		minLen = len1 < len2 ? len1 : len2;
		if(len1 < len2)
			*isReserve = FALSE;
		else
			*isReserve = TRUE;

		index = (g_LineInfoArray.GetAt(0)).addrNo;;
	}

	//g_LineInfoArray.RemoveAt(pos);
	return index;
}

//优化路径
void CFunctionArray::OptimizePath(CPtrArray& arr, CPoint2D basePoint, BOOL bOptimize /*= TRUE*/)
{
	CPoint2D startPoint, endPoint, nextBasePoint;
	int iBetterNum = 0, iGraphNum = 0;
	int iIndex = -1;
	int nSection = 0;

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

	INT_PTR iMaxSize = arr.GetSize();
	if(iMaxSize > 0)
	{
		g_idNo = new int[iMaxSize];
		memset(g_idNo, -1, sizeof(g_idNo));

		g_BetterPathPoint = new CPoint2D[iMaxSize * 2];
		memset(g_BetterPathPoint, 0, sizeof(g_BetterPathPoint));
	}

	nextBasePoint = basePoint;

	if (bOptimize)
	{
		while(iGraphNum < arr.GetSize())
		{
			g_BetterPathPoint[iBetterNum++] = nextBasePoint;
			iIndex = ReturnIndexOfMinDistance(arr, nextBasePoint);

			if(((CGraph*)arr.GetAt(iIndex))->m_bIsPolyLine)
			{
				nSection = ((CGraph*)arr.GetAt(iIndex))->m_nCount;
				startPoint = ((CGraph*)arr.GetAt(iIndex))->m_ptOrigin;
				endPoint = ((CGraph*)arr.GetAt(iIndex + nSection - 1))->m_ptEnd;

				for (int i = 0; i < nSection; ++i)
				{
					if(i != 0)
						g_BetterPathPoint[iBetterNum++] = nextBasePoint;

					g_BetterPathPoint[iBetterNum++] = ((CGraph*)arr.GetAt(iIndex + i))->m_ptOrigin;
					nextBasePoint = ((CGraph*)arr.GetAt(iIndex + i))->m_ptEnd;
					g_idNo[iGraphNum++] = iIndex + i;
				}
			}
			else
			{
				startPoint = ((CGraph*)arr.GetAt(iIndex))->m_ptOrigin;
				endPoint = ((CGraph*)arr.GetAt(iIndex))->m_ptEnd;
				g_BetterPathPoint[iBetterNum++] = startPoint;
				g_idNo[iGraphNum++] = iIndex;
			}
			nextBasePoint = endPoint;
		}
	}
	else
	{
		while(iGraphNum < arr.GetSize())
		{
			g_idNo[iGraphNum++] = ++iIndex;
		}
	}
}

//数据分配
void CFunctionArray::DataMalloc(const CPtrArray& m)//将线段数据分配到指定的数据结构数组中
{
	int count = m.GetCount();
	//LineInfor *pLineInfo = new LineInfor[count];
	CLineInfo LI;
	g_LineInfoArray.RemoveAll();

	CPoint2D sp, ep, op;
	int lineType = 0;
	double fMajor = 0.0, fMinor = 0.0, sAngle = 0.0, eAngle = 0.0, angle = 0.0;
	BOOL bDirection = FALSE, uFlags = FALSE;
	CArray<CPoint2D> ptMember;
	int ptNum;	

	//先把线段信息保存到结构体数组中
	for(int i = 0; i < count; ++i)
	{
		lineType = ((CGraph*)m.GetAt(i))->m_iDrawType;

		switch(lineType)
		{
		case 2:
			sp = ((CGraph*)m.GetAt(i))->m_ptOrigin;
			ep = ((CGraph*)m.GetAt(i))->m_ptEnd;
			break;

		case 3:
			sp = ((CGraph*)m.GetAt(i))->m_ptOrigin;
			ep = sp;
			break;

		case 4:
			sp = ((CGraph*)m.GetAt(i))->m_ptOrigin;
			ep = ((CGraph*)m.GetAt(i))->m_ptEnd;
			break;

		case 6:
			fMajor = ((CGraph*)m.GetAt(i))->m_fMajor;
			fMinor = ((CGraph*)m.GetAt(i))->m_fMinor;
			sAngle = ((CGraph*)m.GetAt(i))->m_fStartAngle;
			eAngle = ((CGraph*)m.GetAt(i))->m_fEndAngle;
			angle = ((CGraph*)m.GetAt(i))->m_fDegree;

			op = ((CGraph*)m.GetAt(i))->m_ptCenter;//中心点
			bDirection = ((CGraph*)m.GetAt(i))->m_bDirection;

			sp = GetEllipsePoint(fMajor, fMinor, angle, op, sAngle * 180 / PI, bDirection);
			ep = GetEllipsePoint(fMajor, fMinor, angle, op, eAngle * 180 / PI, bDirection);
			break;

		case 7:
			uFlags = ((CGraph*)m.GetAt(i))->m_bFlags;
			ptNum = ((CGraph*)m.GetAt(i))->m_iVertexNum;
			ptMember.Copy(((CGraph*)m.GetAt(i))->m_PeakGroup);

			if(uFlags)
			{
				sp = CPoint2D(ptMember[0].x, ptMember[0].y);
				ep = sp;
			}
			else
			{
				sp = CPoint2D(ptMember[0].x, ptMember[0].y);
				ep = CPoint2D(ptMember[ptNum - 1].x, ptMember[ptNum - 1].y);
			}
			break;

		case 8:
			ptNum = ((CGraph*)m.GetAt(i))->m_iVertexNum;
			sp = CPoint2D(((CGraph*)m.GetAt(i))->controlNode[0][0], 
				((CGraph*)m.GetAt(i))->controlNode[0][1]);
			ep = CPoint2D(((CGraph*)m.GetAt(i))->controlNode[ptNum - 1][0], 
				((CGraph*)m.GetAt(i))->controlNode[ptNum - 1][1]);
			break;

		case 9:
			sp = ((CGraph*)m.GetAt(i))->m_ptOrigin;
			ep = sp;
			break;
		}

		LI.addrNo = i;
		LI.sP = sp;
		LI.eP = ep;
		g_LineInfoArray.Add(LI);
	}
}

//计算节点
void CFunctionArray::CalculateNode(void)
{
	g_NodeArray.RemoveAll();

	int k,j;
	UINT gType;
	CPoint2D ptCenter;
	double sx = 0.0, sy = 0.0, ex = 0.0, ey = 0.0, cx = 0.0, cy = 0.0;
	double fRadius = 0.0;
	double length = 0.0;
	double lineStep = 0.0, stepX = 0, stepY = 0;
	double circleStep = 0.0, unit = 0.0, angle = 0.0, arcStep = 0;
	BOOL bDirection = FALSE;

	double fMajor = 0.0, fMinor = 0.0, startAngle = 0, endAngle = 0, fEllipseStep = 0.0, fDelte = 0.0;
	CPoint2D pStart, pNext;

	int nPtNumber;
	CArray<CPoint2D> ptMember;
	BOOL uFlags = FALSE;
	double *fPoint;

	vector<vec2> controlNodeArray;
	double tempStep = 0.0, tempX = 0.0, tempY = 0.0;
	CBspline bCurve;
	BOOL isFormer = FALSE;

	CPoint2D tempPoint;
	int count;

	for(int i = 0; i < g_GraphArray.GetSize(); ++i)
	{
		gType = ((CGraph*)g_GraphArray.GetAt(i))->m_iDrawType;

		sx = ((CGraph*)g_GraphArray.GetAt(i))->m_ptOrigin.x;
		sy = ((CGraph*)g_GraphArray.GetAt(i))->m_ptOrigin.y;

		switch(gType)
		{
		case 2:	
			ex = ((CGraph*)g_GraphArray.GetAt(i))->m_ptEnd.x ;
			ey = ((CGraph*)g_GraphArray.GetAt(i))->m_ptEnd.y ;

			//计算直线总长并计算段数
			length = GetDistance(sx, sy, ex, ey);

			if(length < 50)
				lineStep = g_fLineStep_S;
			else
				lineStep = g_fLineStep_L;

			count = length / lineStep;
			stepX = (ex - sx) / count;
			stepY = (ey - sy) / count;


			for(k = 0; k <= count; ++k)
			{
				tempPoint.x = sx + stepX * k;
				tempPoint.y = sy + stepY * k;
				g_NodeArray.Add(tempPoint);
			}
			break;

		case 3:
			g_CircleArray.RemoveAll();
			cx = ((CGraph*)g_GraphArray.GetAt(i))->m_ptCenter.x;
			cy = ((CGraph*)g_GraphArray.GetAt(i))->m_ptCenter.y;
			fRadius = ((CGraph*)g_GraphArray.GetAt(i))->m_fRadius;
			bDirection = ((CGraph*)g_GraphArray.GetAt(i))->m_bDirection;

			length = 2 * PI * fRadius;

			if (fRadius <= 30)
				circleStep = g_fCurveStep_S;
			else if (fRadius <= 50)
				circleStep = g_fCurveStep_M;
			else if(fRadius <= 100)
				circleStep = g_fCurveStep_L;
			else if(fRadius > 100)
				circleStep = g_fCurveStep_XL;

			count = length / circleStep;
			circleStep = GetNewStep(length, circleStep);
			unit = circleStep / fRadius;//每一小段的弧度大小

			g_CircleArray.Add(CInfomation(sx, sy));
			g_NodeArray.Add(CPoint2D(sx,sy));

			for(k = 1; k <= count; ++k)
			{
				GetElement1(3, g_CircleArray[k - 1].x, g_CircleArray[k - 1].y, cx, cy, unit, k, bDirection);
				g_NodeArray.Add(CPoint2D(g_CircleArray[k].x, g_CircleArray[k].y));
			}
			break;

		case 4:
			g_ArcArray.RemoveAll();
			cx = ((CGraph*)g_GraphArray.GetAt(i))->m_ptCenter.x;
			cy = ((CGraph*)g_GraphArray.GetAt(i))->m_ptCenter.y;
			fRadius = ((CGraph*)g_GraphArray.GetAt(i))->m_fRadius;
			bDirection = ((CGraph*)g_GraphArray.GetAt(i))->m_bDirection;
			angle = ((CGraph*)g_GraphArray.GetAt(i))->m_fAngle;

			length = fRadius * angle;
			//判断半径大小，决定弧长步长 by yuanlin 2013.5.21
			if (fRadius <= 30)
				arcStep = g_fCurveStep_S;
			else if (fRadius <= 50)
				arcStep = g_fCurveStep_M;
			else if(fRadius <= 100)
				arcStep = g_fCurveStep_L;
			else if(fRadius > 100)
				arcStep = g_fCurveStep_XL;

			count = length / arcStep;
			arcStep = GetNewStep(length,arcStep);
			unit = arcStep / fRadius;

			g_ArcArray.Add(CInfomation(sx, sy));
			g_NodeArray.Add(CPoint2D(sx, sy));
			for(k = 1; k <= count; ++k)
			{
				GetElement1(4, g_ArcArray[k - 1].x, g_ArcArray[k - 1].y, cx, cy, unit, k, bDirection);
				g_NodeArray.Add(CPoint2D(g_ArcArray[k].x, g_ArcArray[k].y));
			}
			break;

		case 6:
			g_EllipseArray.RemoveAll();
			fMajor = ((CGraph*)g_GraphArray.GetAt(i))->m_fMajor;
			fMinor = ((CGraph*)g_GraphArray.GetAt(i))->m_fMinor;
			startAngle = ((CGraph*)g_GraphArray.GetAt(i))->m_fStartAngle;
			endAngle = ((CGraph*)g_GraphArray.GetAt(i))->m_fEndAngle;
			angle = ((CGraph*)g_GraphArray.GetAt(i))->m_fDegree;
			fDelte = endAngle - startAngle - 2 * PI;

			ptCenter = ((CGraph*)g_GraphArray.GetAt(i))->m_ptCenter;
			bDirection = ((CGraph*)g_GraphArray.GetAt(i))->m_bDirection;

			if(fMinor <= 30)
				fEllipseStep = g_fEllipseStep_S;
			else if(fMinor <= 50)
				fEllipseStep = g_fEllipseStep_M;
			else if(fMinor <= 100)
				fEllipseStep = g_fEllipseStep_L;
			else if(fMinor > 100)
				fEllipseStep = g_fEllipseStep_XL;

			if(!bDirection)
				pStart = GetEllipsePoint(fMajor, fMinor, angle, ptCenter, startAngle * 180 / PI, bDirection);
			else
				pStart = GetEllipsePoint(fMajor, fMinor, angle, ptCenter, endAngle * 180 / PI, bDirection);

			g_EllipseArray.Add(CInfomation(pStart.x, pStart.y));
			g_NodeArray.Add(pStart);

			if(fabs(fDelte) < 1e-6)
			{
				count = 360 / fEllipseStep;
				if(!bDirection)
				{
					for(int n = fEllipseStep; n <= 360; n += fEllipseStep)
					{
						pNext = GetEllipsePoint(fMajor, fMinor, angle, ptCenter, n, bDirection);
						g_NodeArray.Add(pNext);
					}
				}
				else
				{
					for(int n = 360 - fEllipseStep; n >= 0; n -= fEllipseStep)
					{
						pNext = GetEllipsePoint(fMajor, fMinor, angle, ptCenter, n, bDirection);
						g_NodeArray.Add(pNext);
					}
				}
			}
			else
			{
				count = 360 / fEllipseStep;
				if(!bDirection)
				{
					for(int n = fEllipseStep; n <= 360; n += fEllipseStep)
					{
						pNext = GetEllipsePoint(fMajor, fMinor,angle, ptCenter, startAngle * 180 / PI + n, bDirection);
						g_NodeArray.Add(pNext);
					}
				}
				else
				{
					for(int n = fEllipseStep; n <= 360; n += fEllipseStep)
					{
						pNext = GetEllipsePoint(fMajor, fMinor, angle, ptCenter, endAngle * 180 / PI - n, bDirection);
						g_NodeArray.Add(pNext);
					}
				}
			}
			break;

		case 7:
			nPtNumber = ((CGraph*)g_GraphArray.GetAt(i))->m_iVertexNum;
			ptMember.Copy(((CGraph*)g_GraphArray.GetAt(i))->m_PeakGroup);
			uFlags = ((CGraph*)g_GraphArray.GetAt(i))->m_bFlags;

			fPoint = new double[3 * nPtNumber];
			for(j = 0; j < nPtNumber; ++j)
			{
				fPoint[j * 3] = ptMember[j].x;
				fPoint[j * 3 + 1] = ptMember[j].y;
				fPoint[j * 3 + 2] = ptMember[j].con;
			}

			for(j = 0; j < nPtNumber - 1; ++j)
			{
				sx = fPoint[j * 3], sy = fPoint[j * 3 + 1];
				ex = fPoint[(j + 1) * 3], ey = fPoint[(j + 1) * 3 + 1];

				//直线处理
				if(fPoint[j * 3 + 2] == 0.0)
				{
					length = GetDistance(sx, sy, ex, ey);

					if(length < 50)
						lineStep = g_fLineStep_S;
					else
						lineStep = g_fLineStep_L;

					count = length / lineStep;
					stepX = (ex - sx) / count;
					stepY = (ey - sy) / count;

					if(j == 0)
						g_NodeArray.Add(tempPoint);

					for(k = 1; k <= count; ++k)
					{
						tempPoint.x = sx + stepX * k;
						tempPoint.y = sy + stepY * k;
						g_NodeArray.Add(tempPoint);
					}	
				}			
				//圆弧处理
				else
				{
					g_PolyArray.RemoveAll();

					angle = 4 * atan(fabs(fPoint[j * 3 + 2]));
					fRadius = GetDistance(sx, sy, ex, ey) / (2 * sin(angle / 2.0));

					length = fRadius * angle;

					if (fRadius <= 30)
						arcStep = g_fCurveStep_S;
					else if (fRadius <= 50)
						arcStep = g_fCurveStep_M;
					else if(fRadius <= 100)
						arcStep = g_fCurveStep_L;
					else if(fRadius > 100)
						arcStep = g_fCurveStep_XL;

					count = length / arcStep;
					arcStep = GetNewStep(length, arcStep);
					unit = arcStep / fRadius;

					if(fPoint[j * 3 + 2] > 0)
						bDirection = FALSE;						
					else
						bDirection = TRUE;
					ptCenter = GetCenterPoint(sx, sy, ex, ey, fRadius, angle,  bDirection);

					g_PolyArray.Add(CInfomation(sx, sy));
					if(j == 0)
						g_NodeArray.Add(CPoint2D(sx, sy));

					for(k = 1; k <= count; ++k)
					{ 
						GetElement1(7, g_PolyArray[k - 1].x, g_PolyArray[k - 1].y, ptCenter.x, ptCenter.y, unit, k, bDirection);
						g_NodeArray.Add(CPoint2D(g_PolyArray[k].x, g_PolyArray[k].y));
					}
				}
			}

			if(uFlags)
			{
				sx = fPoint[(nPtNumber - 1) * 3], sy = fPoint[(nPtNumber - 1) * 3 + 1];
				ex = fPoint[0], ey = fPoint[1];

				//直线处理
				if(fPoint[(nPtNumber - 1) * 3 + 2] == 0.0)
				{
					length = GetDistance(sx, sy, ex, ey);

					if(length < 50)
						lineStep = g_fLineStep_S;
					else
						lineStep = g_fLineStep_L;

					count = length / lineStep;
					stepX = (ex - sx) / count;
					stepY = (ey - sy) / count;

					for(k = 1;k < count;++k)
					{
						tempPoint.x = sx + stepX * k;
						tempPoint.y = sy + stepY * k;
						g_NodeArray.Add(tempPoint);
					}	
				}
				//圆弧处理
				else
				{
					g_PolyArray.RemoveAll();

					angle = 4 * atan(fabs(fPoint[j * 3 + 2]));
					fRadius = GetDistance(sx, sy, ex, ey) / (2 * sin(angle / 2.0));

					length = fRadius * angle;

					if (fRadius <= 30)
						arcStep = g_fCurveStep_S;
					else if (fRadius <= 50)
						arcStep = g_fCurveStep_M;
					else if(fRadius <= 100)
						arcStep = g_fCurveStep_L;
					else if(fRadius > 100)
						arcStep = g_fCurveStep_XL;

					count = length / arcStep;
					arcStep = GetNewStep(length, arcStep);
					unit = arcStep / fRadius;

					if(fPoint[j * 3 + 2] > 0)
						bDirection = FALSE;						
					else
						bDirection = TRUE;
					ptCenter = GetCenterPoint(sx, sy, ex, ey, fRadius, angle, bDirection);

					g_PolyArray.Add(CInfomation(sx, sy));

					for(k = 1;k < count; ++k)
					{ 
						GetElement1(7, g_PolyArray[k - 1].x, g_PolyArray[k - 1].y, ptCenter.x, ptCenter.y, unit, k, bDirection);
						g_NodeArray.Add(CPoint2D(g_PolyArray[k].x, g_PolyArray[k].y));
					}
				}
			}
			break;

		case 8:
			g_SplineArray.RemoveAll();
			isFormer = FALSE;
			nPtNumber = ((CGraph*)g_GraphArray.GetAt(i))->m_iVertexNum;

			tempStep = g_fSplineStep;
			controlNodeArray = vector<vec2>(((CGraph*)g_GraphArray.GetAt(i))->controlNode.begin(),
				((CGraph*)g_GraphArray.GetAt(i))->controlNode.end());

			bCurve = CBspline(controlNodeArray);
			bCurve.ComputeInterpolation();

			//修改最后一点的坐标
			bCurve.m_interpolationNode[MAX_LENGTH - 1][0] = ((CGraph*)g_GraphArray.GetAt(i))->controlNode[nPtNumber - 1][0];
			bCurve.m_interpolationNode[MAX_LENGTH - 1][1] = ((CGraph*)g_GraphArray.GetAt(i))->controlNode[nPtNumber - 1][1];

			for(int t = 0; t < bCurve.m_interpolationNode.size() - 1; ++t)
			{
				sx = bCurve.m_interpolationNode[t][0], sy = bCurve.m_interpolationNode[t][1];
				ex = bCurve.m_interpolationNode[t + 1][0], ey = bCurve.m_interpolationNode[t + 1][1];
				//m_splineLen += GetDistance(sx,sy,ex,ey);
			}

			//循环计算点与点之间的距离
			//for(n = 0;n < 20;++n)
			//{
			//	g_SplineArray.RemoveAll();
			//	tempStep = g_fSplineStep - 0.1 * n;

			//	isFormer = FALSE;
			//	count = (int)(m_splineLen / tempStep + 1);//样条总共分成的段数
			//	tempStep = m_splineLen / count;//每段中多少个小段

			for(j = 0; j < bCurve.m_interpolationNode.size() - 1; ++j)
			{	
				//如果isFormer为TRUE，长度小于样条步长时，则起点不变，终点索引往后加1；如果isFormer为FALSE，长度大于样条步长时，则起点为最后一次的终点，此时终点为起点的下一个坐标，依次循环
				if(isFormer == FALSE)
					sx = bCurve.m_interpolationNode[j][0], sy = bCurve.m_interpolationNode[j][1];
				else
				{
					sx = tempX, sy = tempY;
				}

				ex = bCurve.m_interpolationNode[j + 1][0], ey = bCurve.m_interpolationNode[j + 1][1];
				length = GetDistance(sx, sy, ex, ey);

				//当某两点之间的距离小于样条步长时，isFormer为FALSE，并把起点的坐标保存下来，并返回到for循环；否则isFormer为FALSE，继续向下执行
				if(tempStep - length > 0.6)
				{
					isFormer = TRUE;
					tempX = sx, tempY = sy;
					continue;
				}
				else
					isFormer = FALSE;
				g_NodeArray.Add(CPoint2D(sx, sy));
			}


			ex = ((CGraph*)g_GraphArray.GetAt(i))->controlNode[nPtNumber - 1][0];
			ey = ((CGraph*)g_GraphArray.GetAt(i))->controlNode[nPtNumber - 1][1];
			g_NodeArray.Add(CPoint2D(ex, ey));

			break;
		}
	}
}

void CFunctionArray::MySort(int *array, const int len)
{
	int p, q;
	int i, j, tmp;
	for(i = 0; i < len; i++)
	{
		p = i;
		q = array[i];

		for(j = i + 1; j < len ; j++)
		{
			if(q < array[j])
			{ 
				p = j;
				q = array[j];
			}

			if(i != p)
			{
				tmp = array[i]; 
				array[i] = array[p];
				array[p] = tmp;
			}
		}
	}
}

void CFunctionArray::DirectionReserve(CGraph* pGraph)
{
	UINT type = pGraph->m_iDrawType;
	COLORREF color = pGraph->clrREF;
	BOOL m_bIsShowArrow = pGraph->m_bIsShowArrow;
	pGraph->m_bIsPitched = FALSE;

	switch(type)
	{
	case 2:
		LineReserve(pGraph);
		break;

	case 3:
		CirCleReserve(pGraph);
		break;

	case 4:
		ArcReserve(pGraph);
		break;

	case 5:
		RectangleReserve(pGraph);
		break;

	case 6:
		EllipseReserve(pGraph);
		break;

	case  7:
		PolyLineReserve(pGraph);
		break;

	case 8:
		SplineReserve(pGraph);
		break;
	}
}

//直线反向
void CFunctionArray::LineReserve(CGraph* pGraph)
{
	CPoint2D startPoint, endPoint;
	CPointArray pt2Array;

	startPoint = pGraph->m_ptOrigin;
	endPoint = pGraph->m_ptEnd;
	SwapPoint(startPoint, endPoint);
	pt2Array = CPointArray(startPoint, endPoint);

	pGraph->m_ptOrigin = startPoint;
	pGraph->m_ptEnd = endPoint;
	pGraph->pt2Array = pt2Array;
}

//整圆反向
void CFunctionArray::CirCleReserve(CGraph* pGraph)
{
	CPoint2D startPoint, centerPoint;
	CPointArray pt2Array;
	BOOL bDirection = FALSE;

	startPoint = pGraph->m_ptOrigin;
	centerPoint = pGraph->m_ptCenter;
	bDirection = pGraph->m_bDirection;
	bDirection = !bDirection;
	pt2Array = CPointArray(centerPoint, startPoint);
	pGraph->m_bDirection = bDirection;
	pGraph->pt2Array = pt2Array;
}

//圆弧反向
void CFunctionArray::ArcReserve(CGraph* pGraph)
{
	CPoint2D startPoint, endPoint, centerPoint;
	CPointArray pt2Array;
	BOOL bDirection = FALSE;

	startPoint = pGraph->m_ptOrigin;
	endPoint = pGraph->m_ptEnd;
	bDirection = pGraph->m_bDirection;
	centerPoint = pGraph->m_ptCenter;
	SwapPoint(startPoint, endPoint);
	bDirection = ! bDirection;

	pt2Array = CPointArray(centerPoint, endPoint);
	pGraph->m_ptOrigin = startPoint;
	pGraph->m_ptEnd = endPoint;
	pGraph->m_bDirection = bDirection;
	pGraph->pt2Array = pt2Array;
}

//矩形反向
void CFunctionArray::RectangleReserve(CGraph* pGraph)
{
	CPoint2D startPoint, endPoint;
	CPointArray pt2Array;

	startPoint = pGraph->m_ptOrigin;
	endPoint = pGraph->m_ptEnd;

	pt2Array = CPointArray(CPoint2D(startPoint.x, endPoint.y), startPoint);

	pGraph->pt2Array = pt2Array;
}

//椭圆(弧)反向
void CFunctionArray::EllipseReserve(CGraph* pGraph)
{
	CPoint2D startPoint, endPoint, centerPoint, ptFirst,ptEnd, ptSecond;
	CPointArray pt2Array;
	BOOL bDirection = FALSE;
	double fMajor = 0.0, fMinor = 0.0, degree = 0.0, startAngle = 0.0, endAngle = 0.0;

	fMajor = pGraph->m_fMajor;
	fMinor = pGraph->m_fMinor;
	degree = pGraph->m_fDegree;
	centerPoint = pGraph->m_ptCenter;
	bDirection = pGraph->m_bDirection;
	startAngle = pGraph->m_fStartAngle;
	endAngle = pGraph->m_fEndAngle;

	ptFirst = GetEllipsePoint(fMajor, fMinor, degree, centerPoint, startAngle * 180 / PI, FALSE);
	ptEnd = GetEllipsePoint(fMajor, fMinor, degree, centerPoint, endAngle * 180 / PI, FALSE);

	if(!bDirection)
	{
		SwapElement(ptFirst.x, ptEnd.x);
		SwapElement(ptFirst.y, ptEnd.y);
		ptSecond = GetEllipsePoint(fMajor, fMinor, degree, centerPoint, startAngle * 180 / PI + 10, FALSE);
	}
	else
	{
		ptSecond = GetEllipsePoint(fMajor, fMinor, degree, centerPoint, endAngle * 180 / PI - 10, FALSE);
	}

	pt2Array = CPointArray(CPoint2D(ptSecond.x, ptSecond.y), CPoint2D(ptEnd.x, ptEnd.y));

	pGraph->m_ptOrigin = CPoint2D(ptFirst.x, ptFirst.y);
	pGraph->pt2Array = pt2Array;
	pGraph->m_bDirection = !bDirection;
	pGraph->m_ptEnd = endPoint;

}

//多段线反向
void CFunctionArray::PolyLineReserve(CGraph* pGraph)
{
	CArray<CPoint2D> m_PeakGroup;
	CPoint2D tempPm;
	CPoint2D startPoint;
	CPointArray pt2Array;

	double startX, startY, endX, endY, con;
	double x, y, c;
	BOOL uFlags = pGraph->m_bFlags;
	UINT VertexNum = pGraph->m_iVertexNum;
	m_PeakGroup.Copy(pGraph->m_PeakGroup);

	double *fPoint = new double[VertexNum * 3];

	for(UINT j = 0; j < VertexNum; ++j)
		m_PeakGroup[j].con = -m_PeakGroup[j].con;

	for(UINT z = 0; z < VertexNum; ++z)
	{
		x = m_PeakGroup[z].x;
		y = m_PeakGroup[z].y;
		c = m_PeakGroup[z].con;
	}


	if(!uFlags)
	{
		for(UINT j = 0; j < VertexNum / 2; ++j)
		{
			SwapElement(m_PeakGroup[j].x, m_PeakGroup[VertexNum - j - 1].x);
			SwapElement(m_PeakGroup[j].y, m_PeakGroup[VertexNum - j - 1].y);
			SwapElement(m_PeakGroup[j].con, m_PeakGroup[VertexNum - j - 2].con);
		}
	}
	else
	{
		SwapElement(m_PeakGroup[0].con, m_PeakGroup[VertexNum - 1].con);

		for (UINT j = 1; j < VertexNum / 2; ++j)
		{
			SwapElement(m_PeakGroup[j].x, m_PeakGroup[VertexNum - j].x);
			SwapElement(m_PeakGroup[j].y, m_PeakGroup[VertexNum - j].y);
			SwapElement(m_PeakGroup[j].con, m_PeakGroup[VertexNum - j - 1].con);
		}
	}

	if(uFlags)
	{
		startX = m_PeakGroup[VertexNum - 1].x;
		startY = m_PeakGroup[VertexNum - 1].y;
		con = m_PeakGroup[VertexNum - 1].con;
		endX = m_PeakGroup[0].x;
		endY = m_PeakGroup[0].y;
	}
	else
	{
		startX = m_PeakGroup[VertexNum - 2].x;
		startY = m_PeakGroup[VertexNum - 2].y;
		con = m_PeakGroup[VertexNum - 2].con;
		endX = m_PeakGroup[VertexNum - 1].x;
		endY = m_PeakGroup[VertexNum - 1].y;
	}

	if(con == 0.0)
	{
		pt2Array = CPointArray(CPoint2D(startX, startY), CPoint2D(endX, endY));
	}
	else
	{
		pt2Array = GetPt2Array(startX, startY, endX, endY, con);
	}

	for(UINT z = 0; z < VertexNum; ++z)
	{
		x = m_PeakGroup[z].x;
		y = m_PeakGroup[z].y;
		c = m_PeakGroup[z].con;
	}

	startPoint = CPoint2D(m_PeakGroup[0].x, m_PeakGroup[0].y);
	pGraph->m_PeakGroup.Copy(m_PeakGroup);
	pGraph->pt2Array = pt2Array;
	pGraph->m_ptOrigin = startPoint;
}

//样条曲线反向
void CFunctionArray::SplineReserve(CGraph* pGraph)
{
	vector<vec2> g_NodeArray;
	CPointArray pt2Array;

	g_NodeArray = vector<vec2>(pGraph->controlNode.begin(), pGraph->controlNode.end());
	size_t size = g_NodeArray.size();
	for(size_t j = 0; j < size / 2; ++j)
	{
		SwapElement((double&)g_NodeArray[j], (double&)g_NodeArray[size - 1 - j]);
		//SwapElement((double&)g_NodeArray[j],(double&)g_NodeArray[size - 1 - j]);
	}

	pt2Array = CPointArray(CPoint2D(g_NodeArray[size - 2][0], g_NodeArray[size - 2][1]),CPoint2D(g_NodeArray[size - 1][0], g_NodeArray[size - 1][1]));
	pGraph->controlNode = vector<vec2>(g_NodeArray.begin(), g_NodeArray.end());
	pGraph->m_ptOrigin = CPoint2D(pGraph->controlNode[0][0], pGraph->controlNode[0][1]);
	pGraph->m_ptEnd = CPoint2D(pGraph->controlNode[size - 1][0], pGraph->controlNode[size - 1][1]);
	pGraph->pt2Array = pt2Array;
}

void CFunctionArray::Prune(CPtrArray& m)
{
	CPoint2D basePoint = CPoint2D(0,0), lastPoint = CPoint2D(-1,-1);
	CPoint2D startPoint, endPoint, middlePoint, centerPoint, centerPoint1, centerPoint2;
	CPointArray pt2Array;
	CArray<CPoint2D> ptMember;
	UINT nPtNumber;
	vector<vec2> controlNode;
	double fMajor = 0.0, fMinor = 0.0, endAngle = 0.0, degree = 0.0, fRadius = 0.0, angle = 0.0, angle1 = 0.0, angle2 = 0.0, radius1 = 0.0, radius2 = 0.0;
	BOOL bFlag = FALSE, bDirection = FALSE, direction1 = FALSE, direction2 = FALSE;
	CGraph *lastLinkGraph = NULL, *lastMergeGraph = NULL;
	CPoint2D lastStartPoint;
	double GraphLen = 0.0;
	BOOL isLink = FALSE;

	////先找到基点
	//for(int i = 0;i < m.GetSize();++i)
	//{
	//	if(9 == ((CGraph*)m.GetAt(i))->m_iDrawType)
	//	{
	//		basePoint = ((CGraph*)m.GetAt(i))->m_ptOrigin;
	//		m.RemoveAt(i);
	//		break;
	//	}
	//}

	for(int i = 0; i < m.GetSize(); ++i)
	{
		//isLink = FALSE;
		switch(((CGraph*)m.GetAt(i))->m_iDrawType)
		{
		case 2:
			startPoint = ((CGraph*)m.GetAt(i))->m_ptOrigin - basePoint;
			endPoint = ((CGraph*)m.GetAt(i))->m_ptEnd - basePoint;
			GraphLen = GetDistance(startPoint,endPoint);

			if(lastLinkGraph != NULL && !isLink)
			{
				lastStartPoint = lastLinkGraph->m_ptOrigin;

				if(GetDistance(lastStartPoint, startPoint) < POINTMAXDIS)
				{
					if(lastLinkGraph->m_iDrawType == LINE)
						LineReserve(lastLinkGraph);
					else
						ArcReserve(lastLinkGraph);
					lastPoint = lastLinkGraph->m_ptEnd;
				}
				else if(GetDistance(lastStartPoint, endPoint) < POINTMAXDIS)
				{
					if(lastLinkGraph->m_iDrawType == LINE)
						LineReserve(lastLinkGraph);
					else
						ArcReserve(lastLinkGraph);
					lastPoint = lastLinkGraph->m_ptEnd;
				}

			}

			if(GetDistance(lastPoint, startPoint) < POINTMAXDIS)
			{
				isLink = TRUE;
				startPoint = lastPoint;
				pt2Array = CPointArray(startPoint, endPoint);

				((CGraph*)m.GetAt(i))->m_ptOrigin = startPoint;
				((CGraph*)m.GetAt(i))->m_ptEnd = endPoint;
				((CGraph*)m.GetAt(i))->pt2Array = pt2Array;

				if(isLink && lastMergeGraph != NULL)
				{
					MergeWithNextGraph((CGraph*)m.GetAt(i - 1), (CGraph*)m.GetAt(i), i - 1);
					i--;
					lastMergeGraph = NULL;
				}
				else
				{
					GraphLen = GetDistance(startPoint, endPoint);
					if(GraphLen < GRAPHMAXLEN)
					{
						MergeWithFormerGraph((CGraph*)m.GetAt(i - 1), (CGraph*)m.GetAt(i), i);
						i--;
					}
				}
			}
			else if(GetDistance(lastPoint,endPoint) < POINTMAXDIS)
			{
				isLink = TRUE;
				endPoint = lastPoint;
				SwapPoint(startPoint, endPoint);
				pt2Array = CPointArray(startPoint, endPoint);

				((CGraph*)m.GetAt(i))->m_ptOrigin = startPoint;
				((CGraph*)m.GetAt(i))->m_ptEnd = endPoint;
				((CGraph*)m.GetAt(i))->pt2Array = pt2Array;

				if(isLink && lastMergeGraph != NULL)
				{
					MergeWithNextGraph((CGraph*)m.GetAt(i - 1), (CGraph*)m.GetAt(i), i - 1);
					i--;
					lastMergeGraph = NULL;
				}
				else
				{
					GraphLen = GetDistance(startPoint, endPoint);
					if(GraphLen < GRAPHMAXLEN)
					{
						MergeWithFormerGraph((CGraph*)m.GetAt(i - 1), (CGraph*)m.GetAt(i), i);
						i--;
					}
				}
			}
			else
			{
				isLink = FALSE;
				lastLinkGraph = NULL;
				lastMergeGraph = NULL;

				if(!isLink && lastLinkGraph == NULL)
				{
					((CGraph*)m.GetAt(i))->m_ptOrigin = startPoint;
					((CGraph*)m.GetAt(i))->m_ptEnd = endPoint;

					pt2Array = ((CGraph*)m.GetAt(i))->pt2Array;

					pt2Array.p1 -= basePoint;
					pt2Array.p2 -= basePoint;
					((CGraph*)m.GetAt(i))->pt2Array = pt2Array;

					lastLinkGraph = (CGraph*)m.GetAt(i);

					lastPoint = ((CGraph*)m.GetAt(i))->m_ptEnd;
					continue;
				}

				if(!isLink && lastMergeGraph == NULL)
				{	
					if(GraphLen < GRAPHMAXLEN)
					{
						pt2Array = ((CGraph*)m.GetAt(i))->pt2Array;

						pt2Array.p1 -= basePoint;
						pt2Array.p2 -= basePoint;

						((CGraph*)m.GetAt(i))->m_ptOrigin = startPoint;
						((CGraph*)m.GetAt(i))->m_ptEnd = endPoint;
						((CGraph*)m.GetAt(i))->pt2Array = pt2Array;

						lastPoint = ((CGraph*)m.GetAt(i))->m_ptEnd;
						lastMergeGraph = (CGraph*)m.GetAt(i);
						continue;
					}
				}

				pt2Array = ((CGraph*)m.GetAt(i))->pt2Array;

				pt2Array.p1 -= basePoint;
				pt2Array.p2 -= basePoint;

				((CGraph*)m.GetAt(i))->m_ptOrigin = startPoint;
				((CGraph*)m.GetAt(i))->m_ptEnd = endPoint;
				((CGraph*)m.GetAt(i))->pt2Array = pt2Array;

			}			
			lastPoint = ((CGraph*)m.GetAt(i))->m_ptEnd;
			break;

		case 3:
			centerPoint = ((CGraph*)m.GetAt(i))->m_ptCenter - basePoint;
			startPoint = ((CGraph*)m.GetAt(i))->m_ptOrigin - basePoint;
			fRadius = ((CGraph*)m.GetAt(i))->m_fRadius;

			if(lastLinkGraph != NULL && !isLink)
			{
				lastStartPoint = lastLinkGraph->m_ptOrigin;
				if(PosInGraph((CGraph*)m.GetAt(i), lastStartPoint))
				{
					if(lastLinkGraph->m_iDrawType == LINE)
						LineReserve(lastLinkGraph);
					else
						ArcReserve(lastLinkGraph);
					lastPoint = lastLinkGraph->m_ptEnd;
				}
			}

			if(PosInGraph(((CGraph*)m.GetAt(i)), lastPoint))
			{
				startPoint = lastPoint;
			}

			if(GetDistance(lastPoint, startPoint) < POINTMAXDIS)
			{
				isLink = TRUE;
				startPoint = lastPoint;

				fRadius = GetDistance(startPoint, centerPoint);
				pt2Array = CPointArray(centerPoint, startPoint);
			}
			else
			{
				isLink = FALSE;
				lastLinkGraph = NULL;
				fRadius = ((CGraph*)m.GetAt(i))->m_fRadius;
				pt2Array = ((CGraph*)m.GetAt(i))->pt2Array;

				pt2Array.p1 -= basePoint;
				pt2Array.p2 -= basePoint;
			}

			((CGraph*)m.GetAt(i))->m_ptOrigin = startPoint;
			((CGraph*)m.GetAt(i))->m_ptCenter = centerPoint;
			((CGraph*)m.GetAt(i))->m_fRadius = fRadius;
			((CGraph*)m.GetAt(i))->pt2Array = pt2Array;
			lastPoint = ((CGraph*)m.GetAt(i))->m_ptOrigin;
			break;

		case 4:
			startPoint = ((CGraph*)m.GetAt(i))->m_ptOrigin - basePoint;
			endPoint = ((CGraph*)m.GetAt(i))->m_ptEnd - basePoint;
			centerPoint = ((CGraph*)m.GetAt(i))->m_ptCenter - basePoint;
			middlePoint = ((CGraph*)m.GetAt(i))->m_ptMiddle - basePoint;
			bDirection = ((CGraph*)m.GetAt(i))->m_bDirection;
			fRadius = ((CGraph*)m.GetAt(i))->m_fRadius;
			angle = ((CGraph*)m.GetAt(i))->m_fAngle;
			GraphLen = fRadius * angle;

			if(lastLinkGraph != NULL && !isLink)
			{
				lastStartPoint = lastLinkGraph->m_ptOrigin;
				if(GetDistance(lastStartPoint, endPoint) < POINTMAXDIS)
				{
					if(lastLinkGraph->m_iDrawType == LINE)
						LineReserve(lastLinkGraph);
					else
						ArcReserve(lastLinkGraph);
					lastPoint = lastLinkGraph->m_ptEnd;
				}
				else if(GetDistance(lastStartPoint, startPoint) < POINTMAXDIS)
				{
					if(lastLinkGraph->m_iDrawType == LINE)
						LineReserve(lastLinkGraph);
					else
						ArcReserve(lastLinkGraph);
					lastPoint = lastLinkGraph->m_ptEnd;
				}
			}

			if(GetDistance(lastPoint, startPoint) < POINTMAXDIS)
			{
				isLink = TRUE;
				startPoint = lastPoint;
				fRadius = (GetDistance(startPoint, centerPoint) + fRadius) / 2.0;
				angle = GetAngle(startPoint, endPoint, fRadius);
				GetRigthAngle(startPoint, endPoint, centerPoint, bDirection, &angle);

				centerPoint = CalculateNewCenterPoint(startPoint, endPoint, fRadius, bDirection, (angle > PI) ? TRUE : FALSE);
				pt2Array = CPointArray(centerPoint, endPoint);

				((CGraph*)m.GetAt(i))->m_ptCenter = centerPoint;
				((CGraph*)m.GetAt(i))->m_ptOrigin = startPoint;
				((CGraph*)m.GetAt(i))->m_ptEnd = endPoint;
				((CGraph*)m.GetAt(i))->m_ptMiddle = middlePoint;
				((CGraph*)m.GetAt(i))->pt2Array = pt2Array;
				((CGraph*)m.GetAt(i))->m_fRadius = fRadius;
				((CGraph*)m.GetAt(i))->m_fAngle = angle;
				((CGraph*)m.GetAt(i))->m_bDirection = bDirection;

				if(isLink && lastMergeGraph != NULL)
				{
					MergeWithNextGraph((CGraph*)m.GetAt(i - 1), (CGraph*)m.GetAt(i), i - 1);
					i--;
					lastMergeGraph = NULL;
				}
				else
				{
					GraphLen = fRadius * angle;
					if(GraphLen < GRAPHMAXLEN)
					{
						MergeWithFormerGraph((CGraph*)m.GetAt(i - 1), (CGraph*)m.GetAt(i), i);
						i--;
					}
				}
			}
			else if(GetDistance(lastPoint, endPoint) < POINTMAXDIS)
			{
				isLink = TRUE;
				endPoint = lastPoint;
				SwapPoint(startPoint, endPoint);
				bDirection = !bDirection;
				fRadius = (GetDistance(startPoint, centerPoint) + fRadius) / 2.0;

				angle = GetAngle(startPoint, endPoint, fRadius);
				GetRigthAngle(startPoint, endPoint, centerPoint, bDirection, &angle);

				centerPoint = CalculateNewCenterPoint(startPoint, endPoint, fRadius, bDirection, (angle > PI) ? TRUE : FALSE);
				pt2Array = CPointArray(centerPoint, endPoint);

				((CGraph*)m.GetAt(i))->m_ptCenter = centerPoint;
				((CGraph*)m.GetAt(i))->m_ptOrigin = startPoint;
				((CGraph*)m.GetAt(i))->m_ptEnd = endPoint;
				((CGraph*)m.GetAt(i))->m_ptMiddle = middlePoint;
				((CGraph*)m.GetAt(i))->pt2Array = pt2Array;
				((CGraph*)m.GetAt(i))->m_fRadius = fRadius;
				((CGraph*)m.GetAt(i))->m_fAngle = angle;
				((CGraph*)m.GetAt(i))->m_bDirection = bDirection;

				if(isLink && lastMergeGraph != NULL)
				{
					MergeWithNextGraph((CGraph*)m.GetAt(i - 1), (CGraph*)m.GetAt(i), i - 1);
					i--;
					lastMergeGraph = NULL;
				}
				else
				{
					GraphLen = fRadius * angle;
					if(GraphLen < GRAPHMAXLEN)
					{
						MergeWithFormerGraph((CGraph*)m.GetAt(i - 1), (CGraph*)m.GetAt(i), i);
						i--;
					}
				}
			}
			else
			{
				lastLinkGraph = NULL;
				isLink = FALSE;
				lastMergeGraph = NULL;

				if(!isLink && lastLinkGraph == NULL)
				{
					((CGraph*)m.GetAt(i))->m_ptCenter = centerPoint;
					((CGraph*)m.GetAt(i))->m_ptOrigin = startPoint;
					((CGraph*)m.GetAt(i))->m_ptEnd = endPoint;
					((CGraph*)m.GetAt(i))->m_ptMiddle = middlePoint;

					pt2Array = ((CGraph*)m.GetAt(i))->pt2Array;

					pt2Array.p1 -= basePoint;
					pt2Array.p2 -= basePoint;
					((CGraph*)m.GetAt(i))->pt2Array = pt2Array;

					lastLinkGraph = (CGraph*)m.GetAt(i);
					lastPoint = ((CGraph*)m.GetAt(i))->m_ptEnd;
					continue;
				}

				if(!isLink && lastMergeGraph == NULL)
				{
					if(GraphLen < GRAPHMAXLEN)
					{
						pt2Array = ((CGraph*)m.GetAt(i))->pt2Array;

						pt2Array.p1 -= basePoint;
						pt2Array.p2 -= basePoint;

						((CGraph*)m.GetAt(i))->m_ptCenter = centerPoint;
						((CGraph*)m.GetAt(i))->m_ptOrigin = startPoint;
						((CGraph*)m.GetAt(i))->m_ptEnd = endPoint;
						((CGraph*)m.GetAt(i))->m_ptMiddle = middlePoint;
						((CGraph*)m.GetAt(i))->pt2Array = pt2Array;
						((CGraph*)m.GetAt(i))->m_fRadius = fRadius;
						((CGraph*)m.GetAt(i))->m_fAngle = angle;
						((CGraph*)m.GetAt(i))->m_bDirection = bDirection;

						lastMergeGraph = (CGraph*)m.GetAt(i);
						lastPoint = ((CGraph*)m.GetAt(i))->m_ptEnd;
						continue;
					}
				}

				pt2Array = ((CGraph*)m.GetAt(i))->pt2Array;

				pt2Array.p1 -= basePoint;
				pt2Array.p2 -= basePoint;

				((CGraph*)m.GetAt(i))->m_ptCenter = centerPoint;
				((CGraph*)m.GetAt(i))->m_ptOrigin = startPoint;
				((CGraph*)m.GetAt(i))->m_ptEnd = endPoint;
				((CGraph*)m.GetAt(i))->m_ptMiddle = middlePoint;
				((CGraph*)m.GetAt(i))->pt2Array = pt2Array;
				((CGraph*)m.GetAt(i))->m_fRadius = fRadius;
				((CGraph*)m.GetAt(i))->m_fAngle = angle;
				((CGraph*)m.GetAt(i))->m_bDirection = bDirection;
			}
			lastPoint = ((CGraph*)m.GetAt(i))->m_ptEnd;
			break;

		case 6:				
			fMajor = ((CGraph*)m.GetAt(i))->m_fMajor;
			fMinor = ((CGraph*)m.GetAt(i))->m_fMinor;
			endAngle = ((CGraph*)m.GetAt(i))->m_fEndAngle;
			degree = ((CGraph*)m.GetAt(i))->m_fDegree;

			((CGraph*)m.GetAt(i))->m_ptOrigin -= basePoint;
			((CGraph*)m.GetAt(i))->m_ptCenter -= basePoint;
			pt2Array = ((CGraph*)m.GetAt(i))->pt2Array;

			pt2Array.p1 -= basePoint;
			pt2Array.p2 -= basePoint;
			((CGraph*)m.GetAt(i))->pt2Array = pt2Array;
			lastPoint = GetEllipsePoint(fMajor, fMinor, degree, 
				((CGraph*)m.GetAt(i))->m_ptCenter, endAngle);
			break;

		case 7:
			bFlag = ((CGraph*)m.GetAt(i))->m_bFlags;
			nPtNumber = ((CGraph*)m.GetAt(i))->m_iVertexNum;
			ptMember.Copy(((CGraph*)m.GetAt(i))->m_PeakGroup);
			pt2Array = ((CGraph*)m.GetAt(i))->pt2Array;

			for(UINT j = 0; j < nPtNumber; ++j)
			{
				ptMember[j] -= basePoint;
			}

			if(lastLinkGraph != NULL && !isLink)
			{
				lastStartPoint = lastLinkGraph->m_ptOrigin;
				if(GetDistance(lastStartPoint, ptMember[0]) < POINTMAXDIS)
				{
					if(lastLinkGraph->m_iDrawType == LINE)
						LineReserve(lastLinkGraph);
					else
						ArcReserve(lastLinkGraph);
					lastPoint = lastLinkGraph->m_ptEnd;
				}
			}

			if(GetDistance(lastPoint, ptMember[0]) < POINTMAXDIS)
			{
				isLink = TRUE;

				if(ptMember[0].con != 0.0)
				{
					if(ptMember[0].con > 0.0)
						direction1 = FALSE;
					else
						direction1 = TRUE;

					angle1 = 4 * atan(fabs(ptMember[0].con));
					radius1 = GetDistance(ptMember[0].x, ptMember[0].y, ptMember[1].x, ptMember[1].y) / (2 * sin(angle1 / 2));
					centerPoint1 = GetCenterPoint(ptMember[0].x, ptMember[0].y, ptMember[1].x, ptMember[1].y, 
						radius1, angle1, direction1);
				}
				if(bFlag)
				{
					if(ptMember[nPtNumber - 1].con != 0.0)
					{
						if(ptMember[nPtNumber - 1].con > 0.0)
							direction2 = FALSE;
						else
							direction2 = TRUE;

						angle2 = 4 * atan(fabs(ptMember[nPtNumber - 1].con));
						radius2 = GetDistance(ptMember[nPtNumber - 1].x, ptMember[nPtNumber - 1].y, ptMember[0].x, ptMember[0].y) / (2 * sin(angle2 / 2));
						centerPoint2 = GetCenterPoint(ptMember[nPtNumber - 1].x, ptMember[nPtNumber - 1].y, ptMember[0].x, ptMember[0].y, 
							radius2, angle2, direction2);
					}
				}

				ptMember[0].x = lastPoint.x;
				ptMember[0].y = lastPoint.y;

				//圆弧con
				//angle = 4 * atan(fabs(fPoint[j * 3 + 2]));

				if(ptMember[0].con != 0.0)//直线保持0.0，只需要修改圆弧的con
				{
					radius1 = (GetDistance(ptMember[0].x, ptMember[0].y, ptMember[1].x,ptMember[1].y) / (2 * sin(angle1 / 2)) + radius1) / 2.0;
					angle1 = GetAngle(CPoint2D(ptMember[0].x, ptMember[0].y), CPoint2D(ptMember[1].x, ptMember[1].y), radius1);
					ptMember[0].con = tan(angle1 / 4.0) * (direction1 ? -1 : 1);
				}

				if(bFlag)
				{
					if(ptMember[nPtNumber - 1].con != 0.0)//直线保持0.0，只需要修改圆弧的con
					{
						radius2 = (GetDistance(ptMember[nPtNumber - 1].x, ptMember[nPtNumber - 1].y, ptMember[0].x, ptMember[0].y) / (2 * sin(angle2 / 2)) + radius2) / 2.0;
						angle2 = GetAngle(CPoint2D(ptMember[nPtNumber - 1].x, ptMember[nPtNumber - 1].y), CPoint2D(ptMember[0].x, ptMember[0].y), radius2);
						ptMember[nPtNumber - 1].con = tan(angle2 / 4.0) * (direction2 ? -1 : 1);
					}
				}
			}
			else
			{
				isLink = FALSE;
				lastLinkGraph = NULL;
			}

			((CGraph*)m.GetAt(i))->m_PeakGroup.Copy(ptMember);
			pt2Array.p1 -= basePoint;
			pt2Array.p2 -= basePoint;

			((CGraph*)m.GetAt(i))->pt2Array = pt2Array;
			((CGraph*)m.GetAt(i))->m_ptOrigin = ptMember[0];
			lastPoint = bFlag ? ptMember[0] : ptMember[nPtNumber - 1];
			break;

		case 8:
			nPtNumber = ((CGraph*)m.GetAt(i))->m_iVertexNum;
			controlNode = vector<vec2>(((CGraph*)m.GetAt(i))->controlNode.begin(), 
				((CGraph*)m.GetAt(i))->controlNode.end());
			pt2Array = ((CGraph*)m.GetAt(i))->pt2Array;

			for(UINT j = 0; j < nPtNumber; ++j)
			{
				controlNode[j][0] -= basePoint.x;
				controlNode[j][1] -= basePoint.y;
			}

			((CGraph*)m.GetAt(i))->controlNode = vector<vec2>(controlNode.begin(), controlNode.end());
			pt2Array.p1 -= basePoint;
			pt2Array.p2 -= basePoint;
			((CGraph*)m.GetAt(i))->pt2Array = pt2Array;
			lastPoint = CPoint2D(controlNode[nPtNumber - 1][0], controlNode[nPtNumber - 1][1]);
			break;
		}
	}		
}

void CFunctionArray::MergeWithFormerGraph(CGraph* firstGraph, CGraph* secondGraph, const int index)
{
	UINT drawType = firstGraph->m_iDrawType;
	double r1 = 0.0, r2 = 0.0, angle = 0.0;
	CPoint2D oCenter;
	BOOL bDirection = FALSE;

	if(drawType == LINE)
	{
		firstGraph->m_ptEnd = secondGraph->m_ptEnd;
		firstGraph->pt2Array = CPointArray(firstGraph->m_ptOrigin, firstGraph->m_ptEnd);
	}
	else
	{
		r1 = firstGraph->m_fRadius;
		oCenter = firstGraph->m_ptCenter;
		bDirection = firstGraph->m_bDirection;
		r2 = (GetDistance(oCenter, secondGraph->m_ptEnd) + r1) / 2.0;
		firstGraph->m_ptEnd = secondGraph->m_ptEnd;

		angle = GetAngle(firstGraph->m_ptOrigin, firstGraph->m_ptEnd, r2);
		oCenter = CalculateNewCenterPoint(firstGraph->m_ptOrigin, firstGraph->m_ptEnd, r2, bDirection, (angle > PI) ? TRUE : FALSE);
		firstGraph->pt2Array = CPointArray(oCenter, firstGraph->m_ptEnd);

		firstGraph->m_fRadius = r2;
		firstGraph->m_fAngle = angle;
		firstGraph->m_ptCenter = oCenter;
	}
	g_GraphArray.RemoveAt(index);
}


void CFunctionArray::MergeWithNextGraph(CGraph* firstGraph, CGraph* secondGraph, const int index)
{
	UINT drawType = secondGraph->m_iDrawType;
	double r1 = 0.0, r2 = 0.0, angle = 0.0;
	CPoint2D oCenter;
	BOOL bDirection = FALSE;

	if(drawType == LINE)
	{
		secondGraph->m_ptOrigin = firstGraph->m_ptOrigin;
		secondGraph->pt2Array = CPointArray(secondGraph->m_ptOrigin, secondGraph->m_ptEnd);
	}
	else
	{
		r1 = secondGraph->m_fRadius;
		oCenter = secondGraph->m_ptCenter;
		bDirection = secondGraph->m_bDirection;

		r2 = (GetDistance(oCenter, firstGraph->m_ptOrigin) + r1) / 2.0;
		secondGraph->m_ptOrigin = firstGraph->m_ptOrigin;

		angle = GetAngle(secondGraph->m_ptOrigin, secondGraph->m_ptEnd, r2);
		oCenter = CalculateNewCenterPoint(secondGraph->m_ptOrigin, secondGraph->m_ptEnd, r2, bDirection, (angle > PI) ? TRUE : FALSE);
		secondGraph->pt2Array = CPointArray(oCenter, secondGraph->m_ptEnd);

		secondGraph->m_fRadius = r2;
		secondGraph->m_fAngle = angle;
		secondGraph->m_ptCenter = oCenter;
	}
	g_GraphArray.RemoveAt(index);
}

//将选中的图形重新排列
void CFunctionArray::GraphSort(CPtrArray& SrcGraph)
{
	CPoint2D sPoint, ePoint;
	CPtrArray DestGraph;
	double len1 = 0.0;
	int index1 = -1;
	DestGraph.RemoveAll();
	BOOL isReserve1 = FALSE;
	int pos;

	INT_PTR count = 0;

	DataMalloc(SrcGraph);

	DestGraph.Add((CGraph*)SrcGraph.GetAt(0));

	ePoint = g_LineInfoArray.GetAt(0).eP;
	g_LineInfoArray.RemoveAt(0);

	count = g_LineInfoArray.GetSize();

	while(count > 0)
	{
		index1 = ReturnIndexOfMinDistance(ePoint, &len1, &isReserve1);

		pos = GetIndexFromLineInfoArray(index1);
		if(isReserve1)
		{
			DirectionReserve((CGraph*)SrcGraph.GetAt(index1));
			ModifyLineInfo((CGraph*)SrcGraph.GetAt(index1), pos);
		}
		DestGraph.Add((CGraph*)SrcGraph.GetAt(index1));

		ePoint = g_LineInfoArray.GetAt(pos).eP;
		g_LineInfoArray.RemoveAt(pos);

		count = g_LineInfoArray.GetSize();
	}

	SrcGraph.Copy(DestGraph);
	DestGraph.RemoveAll();
}

int CFunctionArray::GetIndexFromLineInfoArray(const int index)
{
	for(int i = 0; i < g_LineInfoArray.GetSize(); ++i)	
	{
		if(index == g_LineInfoArray.GetAt(i).addrNo)
			return i;
	}
	return 0;
}

void CFunctionArray::ModifyLineInfo(CGraph* pGraph, const int index)
{
	CLineInfo LI;
	LI.addrNo = index;
	LI.sP = pGraph->m_ptOrigin;
	LI.eP = pGraph->m_ptEnd;

	g_LineInfoArray.SetAt(index, LI);
}

int CFunctionArray::GetIndexFromNumberArray(const int index)
{
	for(int i = 0; i < g_iNumArray.GetSize(); ++i)
		if(g_iNumArray.GetAt(i) == index)
			return i;

	return 0;
}

int CFunctionArray::GetIndexFromOperationArray(const CGraph* pGraph)
{
	for (int i = 0; i < g_OperationArray.GetSize(); ++i)
	{
		if((CGraph*)g_OperationArray.GetAt(i) == pGraph)
			return i;
	}
	return 0;
}

void CFunctionArray::UpperCaseString(CString &str)
{
	int length = str.GetLength();
	for(int i = 0; i < length; ++i)
	{
		if(isalpha(str[i]))
		{
			if(islower(str[i]))
				toupper(str[i]);
		}
	}
}

void CFunctionArray::CalculateImitatePerPointData()
{
	CPoint2D RealPoint, lastPoint;
	lastPoint = CPoint2D(0,0);
	_2PointData pd;
	int i, j;
	int ToPointWay;
	int iSpeed = 10;
	double _2PointLen = 0.0;
	double xPerLen = 0.0, yPerLen = 0.0;
	double x, y;
	_2PointData _2PerPointData;

	CPoint2D orgPoint = g_2PointDataVector[0].ePoint;
	if(orgPoint != lastPoint)
	{
		_2PerPointData.ToPointWay = 0;
		_2PerPointData.ePoint = lastPoint;
		g_ImitatePerPointDataVector.push_back(_2PerPointData);
	}

	for(i = 0; i < g_2PointDataVector.size(); ++i)
	{
		iSpeed = 10;
		pd = g_2PointDataVector[i];
		ToPointWay = pd.ToPointWay;
		RealPoint = pd.ePoint;

		_2PointLen = GetDistance(lastPoint, RealPoint);
		if(ToPointWay == 0)
		{
			iSpeed /= 2;
		}
		xPerLen = (RealPoint.x - lastPoint.x) / iSpeed;
		yPerLen = (RealPoint.y - lastPoint.y) / iSpeed;

		for(j = 1; j <= iSpeed; ++j)
		{
			x = lastPoint.x + xPerLen * j;
			y = lastPoint.y + yPerLen * j;
			_2PerPointData.ToPointWay = ToPointWay;
			_2PerPointData.ePoint = CPoint2D(x, y);

			g_ImitatePerPointDataVector.push_back(_2PerPointData);
		}
		lastPoint = RealPoint;
	}
}

void CFunctionArray::SetNewStepToGraph( double fNewStep )
{
	for(int i = 0; i < g_OperationArray.GetSize(); ++i)
	{
		((CGraph*)g_OperationArray.GetAt(i))->m_fPerStep = fNewStep;
		((CGraph*)g_OperationArray.GetAt(i))->m_bSetPerStep = TRUE;

		((CGraph*)g_OperationArray.GetAt(i))->m_bIsPitched = FALSE;
	}
}

void CFunctionArray::CancelNewStepToGraph()
{
	for(int i = 0; i < g_OperationArray.GetSize(); ++i)
	{
		((CGraph*)g_OperationArray.GetAt(i))->m_bSetPerStep = FALSE;

		((CGraph*)g_OperationArray.GetAt(i))->m_bIsPitched = FALSE;
	}
}

void CFunctionArray::SetNewFeedSpeed( int iFeedSpeed )
{
	for(int i = 0; i < g_OperationArray.GetSize(); ++i)
	{
		((CGraph*)g_OperationArray.GetAt(i))->m_iPerFeedSpeed = iFeedSpeed;
		((CGraph*)g_OperationArray.GetAt(i))->m_bSetFeedSpeed = TRUE;

		((CGraph*)g_OperationArray.GetAt(i))->m_bIsPitched = FALSE;
	}
}

void CFunctionArray::CancelFeedSpeed()
{
	for(int i = 0; i < g_OperationArray.GetSize(); ++i)
	{
		((CGraph*)g_OperationArray.GetAt(i))->m_bSetFeedSpeed = FALSE;
		((CGraph*)g_OperationArray.GetAt(i))->m_bIsPitched = FALSE;
	}
}

double CFunctionArray::GetTangent( int DrawType, CPoint2D StartPt, CPoint2D EndPt, BOOL bDir /*= FALSE*/ )
{
	double c = 0.0;

	if(DrawType == 2)
	{
		c = GetAngle(StartPt.x, StartPt.y, EndPt.x, EndPt.y);
	}
	else if(DrawType == 4)
	{
		c = GetAngle(StartPt.x, StartPt.y, EndPt.x, EndPt.y);
		if(bDir)
			c -= PI / 2.0;
		else 
			c += PI / 2.0;

		if(c > 2 * PI)
			c -= 2 * PI;
		if(c < 0)
			c += 2 * PI ;
	}
	return c;
}

CString CFunctionArray::GetMachineSerial(void)
{
	// TODO: Add your control notification handler code here
	unsigned long s1,s2,s3;
	unsigned char vendor_id[] = "------------";//CPU提供商ID

	// 以下为获得CPU ID的汇编语言指令
	//   _asm    // 得到CPU提供商信息 
	//   {  
	//       xor eax,eax   // 将eax清0
	//       cpuid         // 获取CPUID的指令
	//       mov dword ptr vendor_id,ebx
	//       mov dword ptr vendor_id[+4],edx
	//       mov dword ptr vendor_id[+8],ecx  
	//   }
	//   str1.Format("%s",vendor_id);

	_asm    // 得到CPU ID的高32位 
	{ 
		mov eax,01h    
		xor edx,edx
		cpuid
		mov s1,eax
	}
	//str2.Format("%08X-",s1);

	_asm    // 得到CPU ID的低64位
	{ 
		mov eax,03h
		xor ecx,ecx
		xor edx,edx
		cpuid
		mov s2,edx
		mov s3,ecx
	}

	//str3.Format("%08X-%08X\n",s2,s3);

	char   mac[200] = {0};   
	NCB ncb;     
	typedef struct _ASTAT_     
	{
		ADAPTER_STATUS   adapt;  
		NAME_BUFFER   NameBuff   [30];     
	}ASTAT,   *   PASTAT;     
	ASTAT Adapter;     

	typedef struct _LANA_ENUM     
	{  
		UCHAR   length;  
		UCHAR   lana[MAX_LANA];     
	}LANA_ENUM   ;     
	LANA_ENUM lana_enum;     

	UCHAR uRetCode;     
	memset(&ncb, 0, sizeof(ncb));     
	memset(&lana_enum, 0, sizeof(lana_enum));     

	ncb.ncb_command = NCBENUM;     
	ncb.ncb_buffer = (unsigned char *)&lana_enum;     
	ncb.ncb_length = sizeof(LANA_ENUM);     
	uRetCode = Netbios(&ncb);     


	for(int lana=0; lana<lana_enum.length; lana++)     
	{
		ncb.ncb_command = NCBRESET; 
		ncb.ncb_lana_num = lana_enum.lana[lana]; 
		uRetCode = Netbios(&ncb);   
		if(uRetCode == NRC_GOODRET)  
			break; 
	} 


	memset(&ncb, 0, sizeof(ncb));  
	ncb.ncb_command = NCBASTAT;  
	ncb.ncb_lana_num = lana_enum.lana[0];
	strcpy((char*)ncb.ncb_callname, "*"); 
	ncb.ncb_buffer = (unsigned char *)&Adapter;
	ncb.ncb_length = sizeof(Adapter);
	uRetCode = Netbios(&ncb);  

	sprintf(mac,"%02X%02X%02X%02X%02X%02X",     
		Adapter.adapt.adapter_address[0],     
		Adapter.adapt.adapter_address[1],     
		Adapter.adapt.adapter_address[2],     
		Adapter.adapt.adapter_address[3],     
		Adapter.adapt.adapter_address[4],     
		Adapter.adapt.adapter_address[5]  
	);   
	/*CString m_str1;
	CString str;
	m_str1 = mac;

	str2=str2+str3;
	str = m_str1 + str2;*/

	char s[100] = {0};
	sprintf_s(s, 100, "%s%08X-%08X-%08X\n",mac, s1, s2,s3);
	CString ss ;
	USES_CONVERSION;
	ss = A2T(s);
	return ss;
}
