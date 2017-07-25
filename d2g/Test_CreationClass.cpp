/*
* @file test_creationclass.cpp
*/

/*****************************************************************************
**  $Id: test_creationclass.cpp 163 2003-07-01 15:51:48Z andrew $
**
**  This is part of the dxflib library
**  Copyright (C) 2001 Andrew Mustun
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU Library General Public License as
**  published by the Free Software Foundation.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU Library General Public License for more details.
**
**  You should have received a copy of the GNU Library General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
******************************************************************************/
#include "stdafx.h"
#include "Test_CreationClass.h"
#include "DL_Dxf.h"
#include "GlobalDataType.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

int DxfColorToFeed[18][2]=
{
	{black,500},
	{red,1000},
	{l_red,1500},
	{brown,2000},
	{yellow,2500},
	{cyan,3000},
	{l_cyan,3500},
	{magenta,4000},
	{l_magenta,4500},
	{blue,5000},
	{l_blue,5500},
	{green,6000},
	{l_green,6500},
	{gray,7000},
	{l_gray,7500},
	{white,8000},
	{bylayer,8500},
	{byblock,9000}
};

int DxfLineTypeToModel[4][2]=
{
	{0,500},
	{1,1000},
	{2,1500},
	{3,2000}
};

double DxfToolSafeHeight = 10;

extern CPtrArray g_GraphArray;
extern CPtrArray g_TextArray;

extern CString g_strToolSelect;
extern double g_fLineStep_S;
extern double g_fLineStep_L;
extern double g_fCurveStep_S;
extern double g_fCurveStep_M;
extern double g_fCurveStep_L;
extern double g_fCurveStep_XL;
extern int g_iFeedSpeed;
extern double g_fOffsetDegree;
extern double g_fDepth;
extern double g_fSafeHeight;
extern int g_iWidth;
extern int g_iLength; 
extern int g_iG00Speed;
extern int g_iUpSpeed;
extern int g_iDownSpeed;
extern CRect g_RectNew;
extern double g_fCurveStep_Len;
extern int g_iSpacing;
extern double g_fDeltaC1;
extern stack<CUndoOperator> g_UndoOperatorStack;


CString DxfReading(CString DxfFileName) 
{
	// Load DXF file into memory
	CString SaveFileName;
	SaveFileName = DxfFileName;
	int nPointPos = SaveFileName.ReverseFind('.');

	if(nPointPos != -1)
	{
		SaveFileName = SaveFileName.Left(nPointPos);
	}

	SaveFileName += _T(".txt");

	Test_CreationClass* creationClass = new Test_CreationClass(SaveFileName);
	DL_Dxf* dxf = new DL_Dxf();
	if (!dxf->in(DxfFileName, creationClass)) 
	{ // if file open failed
		return NULL;
	}
	//creationClass->OutputPathCreate();
	delete dxf;
	delete creationClass;
	return SaveFileName;
}

Test_CreationClass::Test_CreationClass(CString FileName)
{
	mFileName = FileName;
	SaveFile = NULL;
	RealLinkLineNodeNum = 0;
	RealLinkCircleNodeNum = 0;
	RealLinkFastNodeNum = 0;
	m_IndexPolyLine = 0;

	memset(NcLinkLineNode,0,sizeof(NcLinkLineNode));
	memset(NcLinkCircleNode,0,sizeof(NcLinkCircleNode));
	memset(NcLinkFastNode,0,sizeof(NcLinkFastNode));
	memset(&OutputPath,0,sizeof(OutputPath));
	memset(&ncDxfSearchPath,0,sizeof(ncDxfSearchPath));
	memset(&NcLinkLinePath,0,sizeof(NcLinkLinePath));
	memset(&NcLinkCirclePath,0,sizeof(NcLinkCirclePath));
	memset(&NcLinkFastPath,0,sizeof(NcLinkFastPath));
}

Test_CreationClass::~Test_CreationClass(void)
{
	int i;

	for(i=0;i<RealLinkCircleNodeNum;i++)
	{
		delete NcLinkCircleNode[i];
	}
	for(i=0;i<RealLinkLineNodeNum;i++)
	{
		delete NcLinkLineNode[i];
	}
	for(i=0;i<RealLinkFastNodeNum;i++)
	{
		delete NcLinkFastNode[i];
	}
	if(SaveFile)
		fclose(SaveFile);
}

/**
* Sample implementation of the method which handles layers.
*/
void Test_CreationClass::addLayer(const DL_LayerData& data) 
{
}

/**
* Sample implementation of the method which handles point entities.
*/
void Test_CreationClass::addPoint(const DL_PointData& data) 
{
	int color = attributes.getColor();
	//Start cf，06-19
	//修改线段颜色，全部设为0
	color = 0;
	//End

	BaseNcElement CurNcElement;
	memset(&CurNcElement, 0, sizeof(CurNcElement));
	CurNcElement.color = color;
	CurNcElement.type = NC_POINT;
	CurNcElement.LineType = NC_REAL_LINE;
	CurNcElement.StartPoint[0] = data.x;
	CurNcElement.StartPoint[1] = data.y;

	g_clr = IntToRGB(color);
	//	pGraph = new CGraph(9,CPoint2D(CurNcElement.StartPoint[0],CurNcElement.StartPoint[1]),g_clr);
	pGraph = new CGraph;
	pGraph->m_iDrawType = 9;
	pGraph->m_ptOrigin = CPoint2D(CurNcElement.StartPoint[0], CurNcElement.StartPoint[1]);
	pGraph->m_ptEnd = pGraph->m_ptOrigin;
	pGraph->clrREF = g_clr;

	g_GraphArray.Add(pGraph);
}

/**
* Sample implementation of the method which handles line entities.
*/
void Test_CreationClass::addLine(const DL_LineData& data) 
{
	int color = attributes.getColor();
	//Start cf，06-19
	//修改线段颜色，全部设为0
	color = 0;
	//End

	BaseNcElement CurNcElement;
	memset(&CurNcElement, 0, sizeof(CurNcElement));
	CurNcElement.color = color;
	CurNcElement.type = NC_LINE;
	CurNcElement.LineType = NC_REAL_LINE;
	CurNcElement.StartPoint[0] = data.x1 ;
	CurNcElement.StartPoint[1] = data.y1 ;
	CurNcElement.StartPoint[2] = data.z1 ;
	CurNcElement.EndPoint[0] = data.x2;
	CurNcElement.EndPoint[1] = data.y2 ;
	CurNcElement.EndPoint[2] = data.z2 ;
	//PushLinkNode(&CurNcElement);

	double fLength = funArray.GetDistance(CurNcElement.StartPoint[0], CurNcElement.StartPoint[1], 
		CurNcElement.EndPoint[0], CurNcElement.EndPoint[1]);
	if(fLength < 1)
		throw L"存在小于1mm的直线，请仔细检查图形！";

	g_clr = IntToRGB(color);

	pt2Array = CPointArray(CPoint2D(CurNcElement.StartPoint[0], CurNcElement.StartPoint[1]), 
		CPoint2D(CurNcElement.EndPoint[0], CurNcElement.EndPoint[1]));
	
	pGraph = new CGraph;
	pGraph->m_iDrawType = 2;
	pGraph->clrREF = g_clr;
	pGraph->m_ptOrigin = CPoint2D(CurNcElement.StartPoint[0], CurNcElement.StartPoint[1]);
	pGraph->m_ptEnd = CPoint2D(CurNcElement.EndPoint[0], CurNcElement.EndPoint[1]);
	pGraph->pt2Array = pt2Array;
	g_GraphArray.Add(pGraph);

	undoOperator.m_OperatorName = L"ADD";

}

/**
* Sample implementation of the method which handles arc entities.
*/
void Test_CreationClass::addArc(const DL_ArcData& data)
{
	double StartPoint[3],EndPoint[3],MidPoint[3];
	double Pi = 3.141592653589793;
	double tempAngle,AngleLength,angle;

	AngleLength = data.angle2 - data.angle1;
	tempAngle = data.angle1 * Pi / 180;
	StartPoint[0] = (data.cx + data.radius * cos(tempAngle)) ;
	StartPoint[1] = (data.cy + data.radius * sin(tempAngle)) ;
	StartPoint[2] = data.cz ;

	tempAngle = data.angle2 * Pi / 180;
	EndPoint[0] = (data.cx + data.radius * cos(tempAngle));
	EndPoint[1] = (data.cy + data.radius * sin(tempAngle));
	EndPoint[2] = data.cz ;

	if(AngleLength > 0)
		tempAngle = (data.angle1 + data.angle2) * Pi / 360;
	else
		tempAngle = (data.angle1 + data.angle2 - 360) * Pi / 360;

	MidPoint[0] = (data.cx + data.radius * cos(tempAngle));
	MidPoint[1] = (data.cy + data.radius * sin(tempAngle));
	MidPoint[2] = data.cz;

	if(AngleLength < 0)
	{
		AngleLength = AngleLength + 360;
	}

	angle = AngleLength * Pi / 180;

	BaseNcElement CurNcElement;
	memset(&CurNcElement, 0, sizeof(CurNcElement));
	int color = attributes.getColor();
	
	//Start cf，06-19
	//修改线段颜色，全部设为0
	color = 0;
	//End

	CurNcElement.color = color;
	CurNcElement.LineType = NC_REAL_LINE;
	CurNcElement.Para = data.radius ;	

	CurNcElement.StartPoint[0] = StartPoint[0];
	CurNcElement.StartPoint[1] = StartPoint[1];
	CurNcElement.StartPoint[2] = StartPoint[2];
	CurNcElement.EndPoint[0] = EndPoint[0];
	CurNcElement.EndPoint[1] = EndPoint[1];
	CurNcElement.EndPoint[2] = EndPoint[2];
	CurNcElement.CentralPoint[0] = data.cx ;
	CurNcElement.CentralPoint[1] = data.cy ;
	CurNcElement.CentralPoint[2] = data.cz ;
	//PushLinkNode(&CurNcElement);


	g_clr = IntToRGB(color);

	pt2Array = CPointArray(CPoint2D(CurNcElement.CentralPoint[0], CurNcElement.CentralPoint[1]), 
		CPoint2D(CurNcElement.EndPoint[0], CurNcElement.EndPoint[1]));


	/*pGraph = new CGraph(4,CPoint2D(CurNcElement.StartPoint[0],CurNcElement.StartPoint[1]),
	CPoint2D(MidPoint[0],MidPoint[1]),
	CPoint2D(CurNcElement.EndPoint[0],CurNcElement.EndPoint[1]),
	CPoint2D(CurNcElement.CentralPoint[0],CurNcElement.CentralPoint[1]),
	FALSE,g_clr,CurNcElement.Para,angle,pt2Array,FALSE);*/

	pGraph = new CGraph;
	pGraph->m_iDrawType = 4;
	pGraph->m_ptOrigin = CPoint2D(CurNcElement.StartPoint[0], CurNcElement.StartPoint[1]);
	pGraph->m_ptMiddle = CPoint2D(MidPoint[0], MidPoint[1]);
	pGraph->m_ptEnd = CPoint2D(CurNcElement.EndPoint[0], CurNcElement.EndPoint[1]);
	pGraph->m_ptCenter = CPoint2D(CurNcElement.CentralPoint[0], CurNcElement.CentralPoint[1]);
	pGraph->clrREF = g_clr;
	pGraph->m_fRadius = CurNcElement.Para;
	pGraph->m_fAngle = angle;
	pGraph->pt2Array = pt2Array;

	g_GraphArray.Add(pGraph);
}

/**
* Sample implementation of the method which handles circle entities.
*/
void Test_CreationClass::addCircle(const DL_CircleData& data) 
{
	int color = attributes.getColor();
	//Start cf，06-19
	//修改线段颜色，全部设为0
	color = 0;
	//End

	BaseNcElement CurNcElement;
	memset(&CurNcElement, 0, sizeof(CurNcElement));
	CurNcElement.color = color;
	CurNcElement.type = NC_CIRCLE;
	CurNcElement.LineType = NC_REAL_LINE;
	CurNcElement.Para = data.radius ;
	CurNcElement.StartPoint[0] = (data.cx - data.radius);
	CurNcElement.StartPoint[1] = data.cy ;
	CurNcElement.StartPoint[2] = data.cz ;
	CurNcElement.EndPoint[0] = data.cx;
	CurNcElement.EndPoint[1] = (data.cy - data.radius) ;
	CurNcElement.EndPoint[2] = data.cz ;
	CurNcElement.CentralPoint[0] = data.cx ;
	CurNcElement.CentralPoint[1] = data.cy ;
	CurNcElement.CentralPoint[2] = data.cz ;
	//PushLinkNode(&CurNcElement);

	g_clr = IntToRGB(color);

	pt2Array = CPointArray(CPoint2D(CurNcElement.CentralPoint[0], CurNcElement.CentralPoint[1]), 
		CPoint2D(CurNcElement.StartPoint[0], CurNcElement.StartPoint[1]));

	/*pGraph = new CGraph(3,CPoint2D(CurNcElement.StartPoint[0],CurNcElement.StartPoint[1]),
	CPoint2D(CurNcElement.CentralPoint[0],CurNcElement.CentralPoint[1]),g_clr,CurNcElement.Para,FALSE,pt2Array);*/
	pGraph = new CGraph;
	pGraph->m_iDrawType = 3;
	pGraph->m_ptOrigin = CPoint2D(CurNcElement.StartPoint[0], CurNcElement.StartPoint[1]);
	pGraph->m_ptCenter = CPoint2D(CurNcElement.CentralPoint[0], CurNcElement.CentralPoint[1]);
	pGraph->m_ptEnd = pGraph->m_ptOrigin;
	pGraph->clrREF = g_clr;
	pGraph->m_fRadius = CurNcElement.Para;
	pGraph->pt2Array = pt2Array;
	pGraph->m_bFlags = TRUE;

	g_GraphArray.Add(pGraph);
}


/**
* Sample implementation of the method which handles spline entities.
*/
void Test_CreationClass::addSpline(const DL_SplineData& data)
{
	colorNum = attributes.getColor();
	//Start cf，06-19
	//修改线段颜色，全部设为0
	colorNum = 0;
	//End

	BaseNcElement CurNcElement;
	memset(&CurNcElement, 0, sizeof(CurNcElement));
	CurNcElement.color = colorNum;
	CurNcElement.type = NC_SPLINE;
	CurNcElement.LineType = NC_REAL_LINE;

	degree = data.degree;//阶数
	controlNum = data.nControl;//控制点数
	knotNum = data.nKnots;//节点数
	flags = data.flags;//标记
}

void Test_CreationClass::addControlPoint(const DL_ControlPointData& data)
{
	double point[2];
	point[0] = data.x;
	point[1] = data.y;
	vec2 pt(point[0], point[1]);
	controlPtArray.push_back(pt);

	if(controlPtArray.size() == controlNum)
	{
		g_clr = IntToRGB(colorNum);

		pGraph = new CGraph;
		pGraph->m_iDrawType = 8;
		pGraph->clrREF = g_clr;
		pGraph->m_iVertexNum = controlNum;
		pGraph->pt2Array = CPointArray(CPoint2D(controlPtArray[controlNum - 2][0], controlPtArray[controlNum - 2][1]), 
			CPoint2D(controlPtArray[controlNum - 1][0], controlPtArray[controlNum - 1][1]));
		pGraph->m_ptOrigin = CPoint2D(controlPtArray[0][0], controlPtArray[0][1]);
		pGraph->m_ptEnd = CPoint2D(controlPtArray[controlNum - 1][0], controlPtArray[controlNum - 1][1]);
		pGraph->controlNode = vector<vec2>(controlPtArray.begin(), controlPtArray.end());

		g_GraphArray.Add(pGraph);
		controlPtArray.clear();
	}
}

void Test_CreationClass::addKnot(const DL_KnotData& data)
{

}


void Test_CreationClass::addEllipse(const DL_EllipseData& data)
{
	double major = 0,minor = 0,degree = 0;
	CPoint2D ptFirst,ptEnd,ptSecond;
	int color = attributes.getColor();
	//Start cf，06-19
	//修改线段颜色，全部设为0
	color = 0;
	//End

	BaseNcElement CurNcElement;
	memset(&CurNcElement, 0, sizeof(CurNcElement));
	CurNcElement.color = color;
	CurNcElement.type = NC_ELLIPSE;
	CurNcElement.LineType = NC_REAL_LINE;
	//椭圆中心坐标
	CurNcElement.CentralPoint[0] = data.cx;
	CurNcElement.CentralPoint[1] = data.cy;
	CurNcElement.CentralPoint[2] = data.cz;
	double g_fRate = data.ratio;//短轴相对于长轴的比例
	double mx = data.mx;//相对于中心的长轴X坐标
	double my = data.my;//相对于中心的长轴Y坐标
	double startAngle = data.angle1;
	double endAngle = data.angle2;
	double angle = endAngle - startAngle;
	double delte = angle - 2 * PI;

	if(mx == 0.0 && my != 0.0)
	{
		major = fabs(my);
		minor = major * g_fRate;

		if(my < 0)
			degree = 3 * PI / 2;
		else
			degree = PI / 2;

		/*
		if(fabs(delte) < 10e-6)
		{
		CurNcElement.StartPoint[0] = CurNcElement.CentralPoint[0];
		CurNcElement.StartPoint[1] = CurNcElement.CentralPoint[1] + my;
		CurNcElement.EndPoint[0] = CurNcElement.StartPoint[0];
		CurNcElement.EndPoint[1] = CurNcElement.StartPoint[1];
		}
		else
		{
		//CurNcElement.StartPoint[0] = funArray.GetEllipsePoint(major,minor,degree,CPoint(CurNcElement.CentralPoint[0],CurNcElement.CentralPoint[1]),startAngle * 180 / PI,FALSE);
		//CurNcElement.StartPoint[1] = funArray.GetEllipsePoint(major,minor,degree,CPoint(CurNcElement.CentralPoint[0],CurNcElement.CentralPoint[1]),startAngle * 180 / PI,FALSE);
		//CurNcElement.EndPoint[0] = funArray.GetEllipsePoint(major,minor,degree,CPoint(CurNcElement.CentralPoint[0],CurNcElement.CentralPoint[1]),endAngle * 180 / PI,FALSE);
		//CurNcElement.EndPoint[1] = funArray.GetEllipsePoint(major,minor,degree,CPoint(CurNcElement.CentralPoint[0],CurNcElement.CentralPoint[1]),endAngle * 180 / PI,FALSE);

		}*/
	}
	else if(my == 0.0 && mx != 0.0)
	{
		major = fabs(mx);
		minor = major * g_fRate;
		if(mx > 0)
			degree = 0;
		else
			degree = PI;

		/*		if(fabs(delte) < 10e-6)
		{
		CurNcElement.StartPoint[0] = CurNcElement.CentralPoint[0] + mx;
		CurNcElement.StartPoint[1] = CurNcElement.CentralPoint[1];
		CurNcElement.EndPoint[0] = CurNcElement.StartPoint[0];
		CurNcElement.EndPoint[1] = CurNcElement.StartPoint[1];
		}
		else
		{
		ptFirst = funArray.GetEllipsePoint(major,minor,degree,CPoint2D(CurNcElement.CentralPoint[0],CurNcElement.CentralPoint[1]),startAngle * 180 / PI,FALSE);
		ptEnd = funArray.GetEllipsePoint(major,minor,degree,CPoint2D(CurNcElement.CentralPoint[0],CurNcElement.CentralPoint[1]),endAngle * 180 / PI,FALSE);

		//CurNcElement.StartPoint[0] = funArray.GetEllipsePoint(major,minor,degree,CPoint(CurNcElement.CentralPoint[0],CurNcElement.CentralPoint[1]),startAngle * 180 / PI,FALSE);
		//CurNcElement.StartPoint[1] = funArray.GetEllipsePoint(major,minor,degree,CPoint(CurNcElement.CentralPoint[0],CurNcElement.CentralPoint[1]),startAngle * 180 / PI,FALSE);
		//CurNcElement.EndPoint[0] = funArray.GetEllipsePoint(major,minor,degree,CPoint(CurNcElement.CentralPoint[0],CurNcElement.CentralPoint[1]),endAngle * 180 / PI,FALSE);
		//CurNcElement.EndPoint[1] = funArray.GetEllipsePoint(major,minor,degree,CPoint(CurNcElement.CentralPoint[0],CurNcElement.CentralPoint[1]),endAngle * 180 / PI,FALSE);
		CurNcElement.StartPoint[0] = ptFirst.x;
		CurNcElement.StartPoint[1] = ptFirst.y;
		CurNcElement.EndPoint[0] = ptEnd.x;
		CurNcElement.EndPoint[1] = ptEnd.y;
		}*/
	}
	else
	{
		major = sqrt(mx * mx + my * my);
		minor = major * g_fRate;

		degree = atan(my / mx);
		if(mx < 0 && my < 0)
			degree = PI + degree;

		/*		if(fabs(delte) < 10e-6)
		{
		CurNcElement.StartPoint[0] = CurNcElement.CentralPoint[0] + mx;
		CurNcElement.StartPoint[1] = CurNcElement.CentralPoint[1] + my;
		CurNcElement.EndPoint[0] = CurNcElement.StartPoint[0];
		CurNcElement.EndPoint[1] = CurNcElement.StartPoint[1];
		}
		else
		{
		ptFirst = funArray.GetEllipsePoint(major,minor,degree,CPoint2D(CurNcElement.CentralPoint[0],CurNcElement.CentralPoint[1]),startAngle * 180 / PI,FALSE);
		ptEnd = funArray.GetEllipsePoint(major,minor,degree,CPoint2D(CurNcElement.CentralPoint[0],CurNcElement.CentralPoint[1]),endAngle * 180 / PI,FALSE);
		//CurNcElement.StartPoint[0] = funArray.GetEllipsePoint(major,minor,degree,CPoint(CurNcElement.CentralPoint[0],CurNcElement.CentralPoint[1]),startAngle * 180 / PI,FALSE);
		//CurNcElement.StartPoint[1] = funArray.GetEllipsePoint(major,minor,degree,CPoint(CurNcElement.CentralPoint[0],CurNcElement.CentralPoint[1]),startAngle * 180 / PI,FALSE);
		//CurNcElement.EndPoint[0] = funArray.GetEllipsePoint(major,minor,degree,CPoint(CurNcElement.CentralPoint[0],CurNcElement.CentralPoint[1]),endAngle * 180 / PI,FALSE);
		//CurNcElement.EndPoint[1] = funArray.GetEllipsePoint(major,minor,degree,CPoint(CurNcElement.CentralPoint[0],CurNcElement.CentralPoint[1]),endAngle * 180 / PI,FALSE);
		CurNcElement.StartPoint[0] = ptFirst.x;
		CurNcElement.StartPoint[1] = ptFirst.y;
		CurNcElement.EndPoint[0] = ptEnd.x;
		CurNcElement.EndPoint[1] = ptEnd.y;
		}*/
	}

	if(fabs(delte) < 10e-6)
	{
		ptFirst = funArray.GetEllipsePoint(major, minor, degree, CPoint2D(CurNcElement.CentralPoint[0], CurNcElement.CentralPoint[1]), 0, FALSE);
		ptEnd = funArray.GetEllipsePoint(major, minor, degree, CPoint2D(CurNcElement.CentralPoint[0], CurNcElement.CentralPoint[1]), 2 * PI, FALSE);
		startAngle = 0;
		endAngle = 2 * PI;
	}
	else
	{	
		ptFirst = funArray.GetEllipsePoint(major, minor, degree, CPoint2D(CurNcElement.CentralPoint[0], CurNcElement.CentralPoint[1]), startAngle * 180 / PI, FALSE);
		ptEnd = funArray.GetEllipsePoint(major, minor, degree, CPoint2D(CurNcElement.CentralPoint[0], CurNcElement.CentralPoint[1]), endAngle * 180 / PI, FALSE);
	}

	CurNcElement.StartPoint[0] = ptFirst.x;
	CurNcElement.StartPoint[1] = ptFirst.y;
	CurNcElement.EndPoint[0] = ptEnd.x;
	CurNcElement.EndPoint[1] = ptEnd.y;

	if(fabs(delte) < 10e-6)
	{
		ptSecond = funArray.GetEllipsePoint(major, minor, degree, CPoint2D(CurNcElement.CentralPoint[0], CurNcElement.CentralPoint[1]), endAngle * 180 / PI - 10, FALSE);
		ptEnd = CPoint2D(CurNcElement.EndPoint[0], CurNcElement.EndPoint[1]);
	}
	else
		ptSecond = funArray.GetEllipsePoint(major, minor, degree, CPoint2D(CurNcElement.CentralPoint[0],CurNcElement.CentralPoint[1]),endAngle * 180 / PI - 10,FALSE);

	g_clr = IntToRGB(color);
	//pt2Array = CPointArray(CPoint(CurNcElement.CentralPoint[0],CurNcElement.CentralPoint[1]),CPoint(CurNcElement.EndPoint[0],CurNcElement.EndPoint[1]));
	pt2Array = CPointArray(CPoint2D(ptSecond.x, ptSecond.y), 
		CPoint2D(ptEnd.x, ptEnd.y));

	//	pGraph = new CGraph(6,major,minor,degree,CPoint2D(CurNcElement.CentralPoint[0],CurNcElement.CentralPoint[1]),CPoint2D(CurNcElement.StartPoint[0],CurNcElement.StartPoint[1]),startAngle,endAngle,g_clr,FALSE,pt2Array,FALSE);

	pGraph = new CGraph;
	pGraph->m_iDrawType = 6;
	pGraph->m_fMajor = major;
	pGraph->m_fMinor = minor;
	pGraph->m_fDegree = degree;
	pGraph->m_ptCenter = CPoint2D(CurNcElement.CentralPoint[0], CurNcElement.CentralPoint[1]);
	pGraph->m_ptOrigin = CPoint2D(CurNcElement.StartPoint[0], CurNcElement.StartPoint[1]);
	pGraph->m_ptEnd = CPoint2D(CurNcElement.EndPoint[0], CurNcElement.EndPoint[1]);
	pGraph->m_fStartAngle = startAngle;
	pGraph->m_fEndAngle = endAngle;
	pGraph->clrREF = g_clr;
	pGraph->pt2Array = pt2Array;

	g_GraphArray.Add(pGraph);
}

/**
* Sample implementation of the method which handles polyline entities.
*/
void Test_CreationClass::addPolyline(const DL_PolylineData& data) 
{
	colorNum = attributes.getColor();
	//Start cf，06-19
	//修改线段颜色，全部设为0
	colorNum = 0;
	//End

	BaseNcElement CurNcElement;
	memset(&CurNcElement,0,sizeof(CurNcElement));
	CurNcElement.color = colorNum;
	CurNcElement.type = NC_POLYLINE;
	CurNcElement.LineType = NC_REAL_LINE;
	VertexNum = data.number;//多段线段数
	flags = data.flags;//图形标记，是否闭合，闭合为1，默认为0
}

/**
* Sample implementation of the method which handles vertices.
*/
void Test_CreationClass::addVertex(const DL_VertexData& data) 
{
	double startX = 0, startY = 0, endX = 0, endY = 0, con = 0;
	double r = 0;
	double point[3] = {0.0};
	point[0] = data.x;
	point[1] = data.y;
	point[2] = data.bulge;
	VertexArray.Add(CPoint2D(point[0], point[1], point[2]));	
	g_clr = IntToRGB(colorNum);

	/*if(VertexArray.GetSize() == VertexNum)
	{
		if(flags != 1)
		{
			if(fabs(VertexArray[0].x - data.x) < 10e-3 && fabs(VertexArray[0].y - data.y) < 10e-3)
				flags = 1;
			else
				flags = 0;
		}

		if(flags == 1)
		{
			startX = VertexArray[VertexNum - 1].x;
			startY = VertexArray[VertexNum - 1].y;
			con = VertexArray[VertexNum - 1].con;
			endX = VertexArray[0].x;
			endY = VertexArray[0].y;
		}
		else
		{
			startX = VertexArray[VertexNum - 2].x;
			startY = VertexArray[VertexNum - 2].y;
			con = VertexArray[VertexNum - 2].con;
			endX = VertexArray[VertexNum - 1].x;
			endY = VertexArray[VertexNum - 1].y;
		}

		if(con == 0.0)
		{
			pt2Array = CPointArray(CPoint2D(startX,startY),CPoint2D(endX,endY));
		}
		else
		{
			pt2Array = funArray.GetPt2Array(startX,startY,endX,endY,con);
		}

		pGraph = new CGraph;
		pGraph->m_iDrawType = 7;
		pGraph->m_iVertexNum = VertexNum;
		pGraph->m_bFlags = flags;
		pGraph->clrREF = g_clr;

		pGraph->m_PeakGroup.Copy(VertexArray);
		pGraph->m_ptOrigin = CPoint2D(VertexArray[0].x,VertexArray[0].y);
		if(flags)
		{
			pGraph->m_ptEnd = pGraph->m_ptOrigin;
		}
		else
		{
			pGraph->m_ptEnd = CPoint2D(VertexArray[VertexNum - 1].x,VertexArray[VertexNum - 1].y);
		}

		int *PolyColorType = new int[VertexNum - (flags ? 0 : 1)];
		memset(PolyColorType, 0, sizeof(PolyColorType));
		pGraph->m_PolyColorType = PolyColorType;
		pGraph->pt2Array = pt2Array;

		g_GraphArray.Add(pGraph);
		VertexArray.RemoveAll();
	}*/

	if(VertexArray.GetSize() == VertexNum)
	{
		m_IndexPolyLine++;

		CPoint2D StartPt, EndPt, MidPt, CenterPt;
		double fAngle = 0.0, fRadius = 0.0;
		BOOL bDirection = FALSE;
		double fStartAngle = 0.0, fEndAngle = 0.0, fTmpAngle = 0.0;

		CPoint2D LastStartPt, LastEndPt, CurStartPt, CurEndPt, LastCenterPt, CurCenterPt;
		double fLastRadius = 0.0, fCurRadius = 0.0;
		double fLastCon = 0.0, fCurCon = 0.0;
		double fLastAngle = 0.0, fCurAngle = 0.0;
		double fLastEndC = 0.0, fCurStartC = 0.0;
		BOOL bLastDirection = FALSE, bCurDirection = FALSE;
		CArray<CPoint2D> tmp;
		tmp.RemoveAll();

		//判断最后一点是否跟起点重合
		//06-16修改
		//Start
		if(flags != 1)
		{
			if(fabs(VertexArray[0].x - data.x) < 10e-3 && fabs(VertexArray[0].y - data.y) < 10e-3)
			{
				flags = 1;
				VertexArray.RemoveAt(VertexArray.GetSize() - 1);
				VertexNum--;
			}
			else
				flags = 0;
		}

		int offset = 0;
		if(flags)
		{
			//查找起点和终点不相切的点的编号
			for (int j = 0; j < VertexNum; ++j)
			{
				//上一段
				if(j == 0)//判断原始起点和终点的切线方向
				{
					//最后一段
					LastStartPt.x = VertexArray.GetAt(VertexNum - 1).x;
					LastStartPt.y = VertexArray.GetAt(VertexNum - 1).y;
					fLastCon = VertexArray.GetAt(VertexNum - 1).con;
					LastEndPt.x = VertexArray.GetAt(0).x;
					LastEndPt.y = VertexArray.GetAt(0).y;
				}
				else if(j == VertexNum - 1)
				{
					LastStartPt.x = VertexArray.GetAt(j - 1).x;
					LastStartPt.y = VertexArray.GetAt(j - 1).y;
					fLastCon = VertexArray.GetAt(j - 1).con;
					LastEndPt.x = VertexArray.GetAt(j).x;
					LastEndPt.y = VertexArray.GetAt(j).y;
				}
				else
				{
					//其中某一段，非第一段
					LastStartPt.x = VertexArray.GetAt(j - 1).x;
					LastStartPt.y = VertexArray.GetAt(j - 1).y;
					fLastCon = VertexArray.GetAt(j - 1).con;
					LastEndPt.x = VertexArray.GetAt(j).x;
					LastEndPt.y = VertexArray.GetAt(j).y;
				}
				if(fLastCon == 0.0)//直线
				{
					fLastEndC = funArray.GetTangent(2, LastStartPt, LastEndPt);
				}
				else//圆弧
				{
					fLastAngle = 4 * atan(fabs(fLastCon));
					if(fLastCon > 0.0)
						bLastDirection = FALSE;
					else
						bLastDirection = TRUE;

					fLastRadius = funArray.GetDistance(LastStartPt, LastEndPt) / (2 * sin(fLastAngle / 2.0));
					LastCenterPt = funArray.GetCenterPoint(LastStartPt.x, LastStartPt.y, LastEndPt.x, LastEndPt.y, fLastRadius, fLastAngle, bLastDirection);

					fLastEndC = funArray.GetTangent(4, LastCenterPt, LastEndPt, bLastDirection);
				}


				//下一段
				if(j == 0)
				{
					CurStartPt.x = VertexArray.GetAt(0).x;
					CurStartPt.y = VertexArray.GetAt(0).y;
					fCurCon = VertexArray.GetAt(0).con;
					CurEndPt.x = VertexArray.GetAt(1).x;
					CurEndPt.y = VertexArray.GetAt(1).y;
				}
				else if(j == VertexNum - 1)
				{
					CurStartPt.x = VertexArray.GetAt(j).x;
					CurStartPt.y = VertexArray.GetAt(j).y;
					fCurCon = VertexArray.GetAt(j).con;
					CurEndPt.x = VertexArray.GetAt(0).x;
					CurEndPt.y = VertexArray.GetAt(0).y;
				}
				else
				{
					CurStartPt.x = VertexArray.GetAt(j).x;
					CurStartPt.y = VertexArray.GetAt(j).y;
					fCurCon = VertexArray.GetAt(j).con;
					CurEndPt.x = VertexArray.GetAt(j + 1).x;
					CurEndPt.y = VertexArray.GetAt(j + 1).y;
				}

				if(fCurCon == 0.0)//直线
				{
					fCurStartC = funArray.GetTangent(2, CurStartPt, CurEndPt);
				}
				else//圆弧
				{
					fCurAngle = 4 * atan(fabs(fCurCon));
					if(fCurCon > 0.0)
						bCurDirection = FALSE;
					else
						bCurDirection = TRUE;

					fCurRadius = funArray.GetDistance(CurStartPt, CurEndPt) / (2 * sin(fCurAngle / 2.0));
					CurCenterPt = funArray.GetCenterPoint(CurStartPt.x, CurStartPt.y, CurEndPt.x, CurEndPt.y, fCurRadius, fCurAngle, bCurDirection);
					fCurStartC = funArray.GetTangent(4, CurCenterPt, CurStartPt, bCurDirection);
				}

				if(fabs(fLastEndC - fCurStartC) > g_fDeltaC1 && fabs(fLastEndC - fCurStartC) < 2 * PI - g_fDeltaC1)
				{
					offset = j;
					break;
				}
			}
		}

		//进行重新定位起点
		if(offset == VertexNum - 1)
		{

		}
		else if(offset == 0)
		{
			;
		}
		else
		{
			tmp.Copy(VertexArray);
			
			for(int k = offset; k < VertexNum; ++k)
			{
				tmp[k - offset].x = VertexArray[k].x;
				tmp[k - offset].y = VertexArray[k].y;
				tmp[k - offset].con = VertexArray[k].con;
			}
			for(int k = 0; k < offset; ++k)
			{
				tmp[k + VertexNum - offset].x = VertexArray[k].x;
				tmp[k + VertexNum - offset].y = VertexArray[k].y;
				tmp[k + VertexNum - offset].con = VertexArray[k].con;				
			}
			VertexArray.Copy(tmp);
		}
		//End
		
		int i;
		for(i = 0; i < VertexArray.GetSize() - 1; ++i)
		{
			pGraph = new CGraph;
			pGraph->clrREF = g_clr;

			StartPt.x = VertexArray.GetAt(i).x;
			StartPt.y = VertexArray.GetAt(i).y;

			EndPt.x = VertexArray.GetAt(i + 1).x;
			EndPt.y = VertexArray.GetAt(i + 1).y;

			pGraph->m_ptOrigin = StartPt;
			pGraph->m_ptEnd = EndPt;

			if(VertexArray.GetAt(i).con == 0.0)
			{
				pGraph->m_iDrawType = 2;				
				pt2Array = CPointArray(StartPt, EndPt);				
				pGraph->pt2Array = pt2Array;
			}
			else
			{
				fAngle = 4 * atan(fabs(VertexArray.GetAt(i).con));
				if(VertexArray.GetAt(i).con > 0.0)
					bDirection = FALSE;
				else
					bDirection = TRUE;

				fRadius = funArray.GetDistance(StartPt, EndPt) / (2 * sin(fAngle / 2.0));
				CenterPt = funArray.GetCenterPoint(StartPt.x, StartPt.y, EndPt.x, EndPt.y, fRadius, fAngle, bDirection);
				pt2Array = CPointArray(CenterPt, EndPt);

				//中间点
				fStartAngle = funArray.GetAngle(CenterPt.x, CenterPt.y, StartPt.x, StartPt.y);
				fEndAngle = funArray.GetAngle(CenterPt.x, CenterPt.y, EndPt.x, EndPt.y);
				
				if(bDirection)
					fTmpAngle = fEndAngle + (fAngle / 2.0);
				else
					fTmpAngle = fEndAngle - (fAngle / 2.0);

				MidPt.x = (CenterPt.x + fRadius * cos(fTmpAngle));
				MidPt.y = (CenterPt.y + fRadius * sin(fTmpAngle));

				pGraph->m_iDrawType = 4;
				pGraph->m_fRadius = fRadius;
				pGraph->m_ptMiddle = MidPt;
				pGraph->m_ptCenter = CenterPt;
				pGraph->m_fAngle = fAngle;
				pGraph->m_bDirection = bDirection;
				pGraph->pt2Array = pt2Array;
			}
			pGraph->m_bIsPolyLine = TRUE;
			pGraph->m_IndexOfPolyLine = m_IndexPolyLine;
			pGraph->m_IndexPerPolyLine = i;
			pGraph->m_nCount = flags ? VertexNum : VertexNum - 1;
			g_GraphArray.Add(pGraph);
		}

		if(flags)
		{
			pGraph = new CGraph;
			pGraph->clrREF = g_clr;

			StartPt.x = VertexArray.GetAt(i).x;
			StartPt.y = VertexArray.GetAt(i).y;

			EndPt.x = VertexArray.GetAt(0).x;
			EndPt.y = VertexArray.GetAt(0).y;

			pGraph->m_ptOrigin = StartPt;
			pGraph->m_ptEnd = EndPt;

			if(VertexArray.GetAt(i).con == 0.0)
			{
				pGraph->m_iDrawType = 2;				
				pt2Array = CPointArray(StartPt, EndPt);				
				pGraph->pt2Array = pt2Array;

			}
			else
			{
				fAngle = 4 * atan(fabs(VertexArray.GetAt(i).con));
				if(VertexArray.GetAt(i).con > 0.0)
					bDirection = FALSE;
				else
					bDirection = TRUE;

				fRadius = funArray.GetDistance(StartPt, EndPt) / (2 * sin(fAngle / 2.0));
				CenterPt = funArray.GetCenterPoint(StartPt.x, StartPt.y, EndPt.x, EndPt.y, fRadius, fAngle, bDirection);
				pt2Array = CPointArray(CenterPt, EndPt);

				//中间点
				fStartAngle = funArray.GetAngle(CenterPt.x, CenterPt.y, StartPt.x, StartPt.y);
				fEndAngle = funArray.GetAngle(CenterPt.x, CenterPt.y, EndPt.x, EndPt.y);

				if(bDirection)
					fTmpAngle = fEndAngle + (fAngle / 2.0);
				else
					fTmpAngle = fEndAngle - (fAngle / 2.0);

				MidPt.x = (CenterPt.x + fRadius * cos(fTmpAngle));
				MidPt.y = (CenterPt.y + fRadius * sin(fTmpAngle));

				pGraph->m_iDrawType = 4;
				pGraph->m_fRadius = fRadius;
				pGraph->m_ptMiddle = MidPt;
				pGraph->m_ptCenter = CenterPt;
				pGraph->m_fAngle = fAngle;
				pGraph->m_bDirection = bDirection;
				pGraph->pt2Array = pt2Array;
			}

			pGraph->m_bIsPolyLine = TRUE;
			pGraph->m_IndexOfPolyLine = m_IndexPolyLine;
			pGraph->m_IndexPerPolyLine = i;
			pGraph->m_nCount = VertexNum;

			g_GraphArray.Add(pGraph);
		}
		VertexArray.RemoveAll();		
	}
}

void Test_CreationClass::addText(const DL_TextData& data)
{
	CTextGraph *pTextGraph = new CTextGraph;
	pTextGraph->m_TextStyle = 1;
	pTextGraph->m_InsertPoint = CPoint2D(data.ipx, data.ipy);
	pTextGraph->m_TextHeight = data.height;
	pTextGraph->m_Angle = data.angle;

	USES_CONVERSION;
	pTextGraph->m_Style = A2T(data.style.c_str());
	CString strText = A2T(data.text.c_str());
	if(strText[strText.GetLength() - 1] == L'}')
	{
		int LastSemicolonPos = strText.ReverseFind(L';');
		strText = strText.Mid(LastSemicolonPos + 1, strText.GetLength() - 2 - LastSemicolonPos);
	}
	pTextGraph->m_Text = strText;
	g_TextArray.Add(pTextGraph);
}

void Test_CreationClass::addMText(const DL_MTextData& data)
{
	CTextGraph *pTextGraph = new CTextGraph;
	pTextGraph->m_TextStyle = 2;
	pTextGraph->m_InsertPoint = CPoint2D(data.ipx, data.ipy);
	pTextGraph->m_TextHeight = data.height;
	pTextGraph->m_TextWidth = data.width;
	pTextGraph->m_AttachPoint = data.attachmentPoint;
	pTextGraph->m_DrawingDirection = data.drawingDirection;
	pTextGraph->m_Angle = data.angle;

	USES_CONVERSION;
	pTextGraph->m_Style = A2T(data.style.c_str());
	CString strText = A2T(data.text.c_str());
	if(strText[strText.GetLength() - 1] == L'}')
	{
		int LastSemicolonPos = strText.ReverseFind(L';');
		strText = strText.Mid(LastSemicolonPos + 1, strText.GetLength() - 2 - LastSemicolonPos);
	}
	pTextGraph->m_Text = strText;
	g_TextArray.Add(pTextGraph);
}

void Test_CreationClass::printAttributes() 
{

	return;
}

void Test_CreationClass::PushLinkNode(BaseNcElement *pNcElement)
{
	NcElementLink *CurLinkNode = new (NcElementLink);
	memcpy(&CurLinkNode->CurElement, pNcElement,sizeof(BaseNcElement));
	CurLinkNode->pLastNode = NULL;
	CurLinkNode->pNextNode = NULL;
	if(pNcElement->type == NC_FAST)
	{
		NcLinkFastNode[RealLinkFastNodeNum] = CurLinkNode;
		CurLinkNode->RealLinkNo = RealLinkFastNodeNum;
		RealLinkFastNodeNum++;
		PushBackLinkPathNode(CurLinkNode,&NcLinkFastPath);
	}	
	else if(pNcElement->type == NC_CIRCLE)
	{
		NcLinkCircleNode[RealLinkCircleNodeNum] = CurLinkNode;
		CurLinkNode->RealLinkNo = RealLinkCircleNodeNum;
		RealLinkCircleNodeNum++;
		PushBackLinkPathNode(CurLinkNode,&NcLinkCirclePath);
	}
	else
	{
		NcLinkLineNode[RealLinkLineNodeNum] = CurLinkNode;
		CurLinkNode->RealLinkNo = RealLinkLineNodeNum;
		RealLinkLineNodeNum++;
		PushBackLinkPathNode(CurLinkNode,&NcLinkLinePath);
	}
}


void Test_CreationClass::PushBackLinkPathNode(NcElementLink *pNcElementLink, NcPathLink *pCurPath)
{
	if(pCurPath->pHeadNode == NULL)
	{
		pCurPath->pHeadNode = pNcElementLink;
	}
	else if(pCurPath->pHeadNode->pNextNode == NULL)
	{
		pCurPath->pHeadNode->pNextNode = pNcElementLink;
		pNcElementLink->pLastNode = pCurPath->pHeadNode;
	}
	if(pCurPath->pEndNode)
	{
		pCurPath->pEndNode->pNextNode = pNcElementLink;
		pNcElementLink->pLastNode = pCurPath->pEndNode;
	}
	pCurPath->pEndNode = pNcElementLink;
	pCurPath->pEndNode->pNextNode = NULL;
	pCurPath->RealGetNum++;
}


NcElementLink *Test_CreationClass::PopLinkPathNode(NcPathLink *pCurPath)
{
	NcElementLink *pNcElementLink;
	if(pCurPath->RealGetNum == 0 || pCurPath->pHeadNode == NULL)
	{
		pNcElementLink = NULL;
		return NULL;
	}
	pNcElementLink = pCurPath->pHeadNode;
	pCurPath->pHeadNode = pCurPath->pHeadNode->pNextNode;
	if(pCurPath->pHeadNode)
		pCurPath->pHeadNode->pLastNode = NULL;
	pCurPath->RealGetNum--;
	if(pCurPath->RealGetNum == 0)
	{
		pCurPath->pHeadNode = NULL;
		pCurPath->pEndNode = NULL;
	}
	return pNcElementLink;
}


void Test_CreationClass::PathSearch(NcPathLink *pInsertPath)
{
	int i = NcLinkLinePath.RealGetNum - 1,j = -1;
	NcElementLink *CurWorkNode = NULL;
	double tempPos[3];

	pInsertPath->RealGetNum = 1;
	CurWorkNode = PopLinkPathNode(&NcLinkLinePath);
	if(CurWorkNode == NULL)
		return;

	pInsertPath->pHeadNode = CurWorkNode;
	pInsertPath->pEndNode = CurWorkNode;

	while(i > 0)
	{
		//当可以重复匹配的机会数全部用完时证明不需要再尝试了；
		//或者所有的结点均被有效匹配，那表示路径搜索完毕。
		CurWorkNode = PopLinkPathNode(&NcLinkLinePath);
		i--;
		if(CurWorkNode == NULL)
			break;
		pInsertPath->RealGetNum++;
		if(fabs(pInsertPath->pHeadNode->CurElement.StartPoint[0] - CurWorkNode->CurElement.StartPoint[0]) < MIN_F
			&&fabs(pInsertPath->pHeadNode->CurElement.StartPoint[1] - CurWorkNode->CurElement.StartPoint[1]) < MIN_F
			&&fabs(pInsertPath->pHeadNode->CurElement.StartPoint[2] - CurWorkNode->CurElement.StartPoint[2]) < MIN_F)
		{//新的结点插到起始端，而且需要调换新结点的首末位置。
			CurWorkNode->pNextNode = pInsertPath->pHeadNode;
			pInsertPath->pHeadNode->pLastNode = CurWorkNode;
			memcpy(tempPos,CurWorkNode->CurElement.StartPoint,sizeof(tempPos));
			memcpy(CurWorkNode->CurElement.StartPoint,CurWorkNode->CurElement.EndPoint,sizeof(tempPos));
			memcpy(CurWorkNode->CurElement.EndPoint,tempPos,sizeof(tempPos));
			pInsertPath->pHeadNode = CurWorkNode;
			j = -1;
		}
		else if(fabs(pInsertPath->pHeadNode->CurElement.StartPoint[0] - CurWorkNode->CurElement.EndPoint[0]) < MIN_F
			&&fabs(pInsertPath->pHeadNode->CurElement.StartPoint[1] - CurWorkNode->CurElement.EndPoint[1]) < MIN_F
			&&fabs(pInsertPath->pHeadNode->CurElement.StartPoint[2] - CurWorkNode->CurElement.EndPoint[2]) < MIN_F)
		{//新的结点直接插到起始端。
			CurWorkNode->pNextNode = pInsertPath->pHeadNode;
			pInsertPath->pHeadNode->pLastNode = CurWorkNode;
			pInsertPath->pHeadNode = CurWorkNode;
			j = -1;
		}
		else if(fabs(pInsertPath->pEndNode->CurElement.EndPoint[0] - CurWorkNode->CurElement.EndPoint[0]) < MIN_F
			&&fabs(pInsertPath->pEndNode->CurElement.EndPoint[1] - CurWorkNode->CurElement.EndPoint[1]) < MIN_F
			&&fabs(pInsertPath->pEndNode->CurElement.EndPoint[2] - CurWorkNode->CurElement.EndPoint[2]) < MIN_F)
		{//新的结点插到结束端，而且需要调换新结点的首末位置。
			CurWorkNode->pLastNode  = pInsertPath->pEndNode;
			pInsertPath->pEndNode->pNextNode = CurWorkNode;
			memcpy(tempPos,CurWorkNode->CurElement.StartPoint,sizeof(tempPos));
			memcpy(CurWorkNode->CurElement.StartPoint,CurWorkNode->CurElement.EndPoint,sizeof(tempPos));
			memcpy(CurWorkNode->CurElement.EndPoint,tempPos,sizeof(tempPos));
			pInsertPath->pEndNode = CurWorkNode;
			j = -1;
		}
		else if(fabs(pInsertPath->pEndNode->CurElement.EndPoint[0] - CurWorkNode->CurElement.StartPoint[0]) < MIN_F
			&&fabs(pInsertPath->pEndNode->CurElement.EndPoint[1] - CurWorkNode->CurElement.StartPoint[1]) < MIN_F
			&&fabs(pInsertPath->pEndNode->CurElement.EndPoint[2] - CurWorkNode->CurElement.StartPoint[2]) < MIN_F)
		{//新的结点直接插到结束端。
			CurWorkNode->pLastNode  = pInsertPath->pEndNode;
			pInsertPath->pEndNode->pNextNode = CurWorkNode;
			pInsertPath->pEndNode = CurWorkNode;
			j = -1;
		}
		else
		{
			if(j == -1)
			{//如果首次发生不匹配则记住可以重复匹配的机会数等于余下的所有结点数
				j = NcLinkLinePath.RealGetNum;
				i = j;
			}
			pInsertPath->RealGetNum--;
			PushBackLinkPathNode(CurWorkNode,&NcLinkLinePath);
		}
	}
}


void Test_CreationClass::OutputPathCreate(void)
{
	int i,j,Feed;
	double m_Length = 0;
	double m_count,m_unit;
	double m_fRadius;
	double x = 0,y = 0,z = g_fDepth,c;
	double fOX,fOY;
	double offset = g_fOffsetDegree;
	double temX,temY;
	double temAngle;
	double LengthStart,LengtEnd;
	double temp[3];
	NcElementLink *CurWorkNode = NULL,*CurTempNode = NULL,*CurTempNode1 = NULL,*CurTempNode2 = NULL,*CurTempNode3 = NULL;

	double m_LineStep = 0;
	double xOld = 0.0, yOld = 0.0;
	BOOL skipFlag = TRUE;
	SearchedPathNum = 0;
	memset(ncDxfSearchPath, 0, sizeof(ncDxfSearchPath));
	/*
	char *str = (char*)mFileName.GetBuffer();
	size_t len = strlen(str) + 1;
	size_t converted = 0;
	wchar_t *Wchar;
	Wchar = (wchar_t*)malloc(len * sizeof(wchar_t));
	mbstowcs_s(&converted,Wchar,len,str,_TRUNCATE);

	SaveFile = _wfopen(Wchar, L"wt");
	*/

	FILE *pFile;
	fopen_s(&pFile,"C:\\WINDOWS\\system32\\NTSL.txt","w+");
	if(!pFile) return;
	fprintf_s(pFile,"M02");
	fclose(pFile);

	USES_CONVERSION;
	LPSTR fileName = T2A(mFileName);

	fopen_s(&SaveFile,fileName,"w+");

	if(SaveFile == NULL)
		return;

	fprintf_s(SaveFile,"L%d.000	 W%d.000\n",g_iLength,g_iWidth);
	fprintf_s(SaveFile,"%s\n",g_strToolSelect);
	fprintf_s(SaveFile,"M03\n");
	while(NcLinkLinePath.RealGetNum > 0 || NcLinkCirclePath.RealGetNum > 0)
	{
		if(NcLinkLinePath.RealGetNum > 0)//每次搜索能够连接的所有直线和圆弧
			PathSearch(&ncDxfSearchPath[SearchedPathNum]);
		if(NcLinkCirclePath.RealGetNum > 0)//如果存在整圆结点，则尝试与整圆结点连接
			PathCircleProcess(&ncDxfSearchPath[SearchedPathNum]);
		SearchedPathNum++;
	}
	//==========================方案一=====================================//
	//               在相邻路径之间插入一段定位线段衔接两个路径称为一体     //
	memcpy(&OutputPath,ncDxfSearchPath,sizeof(NcPathLink));
	BaseNcElement CurNcElement;
	CurNcElement.color = NC_CLOER_FAST;
	CurNcElement.type = NC_FAST;
	CurNcElement.LineType = NC_BROKEN_LINE;

	for(i = 1; i < SearchedPathNum; i++)
	{
		OutputPath.RealGetNum++;
		LengthStart = (OutputPath.pEndNode->CurElement.EndPoint[0] - ncDxfSearchPath[i].pHeadNode->CurElement.StartPoint[0]) * (OutputPath.pEndNode->CurElement.EndPoint[0] - ncDxfSearchPath[i].pHeadNode->CurElement.StartPoint[0])
			+ (OutputPath.pEndNode->CurElement.EndPoint[1] - ncDxfSearchPath[i].pHeadNode->CurElement.StartPoint[1]) * (OutputPath.pEndNode->CurElement.EndPoint[1] - ncDxfSearchPath[i].pHeadNode->CurElement.StartPoint[1]);
		LengtEnd = (OutputPath.pEndNode->CurElement.EndPoint[0] - ncDxfSearchPath[i].pEndNode->CurElement.EndPoint[0]) * (OutputPath.pEndNode->CurElement.EndPoint[0] - ncDxfSearchPath[i].pEndNode->CurElement.EndPoint[0])
			+ (OutputPath.pEndNode->CurElement.EndPoint[1] - ncDxfSearchPath[i].pEndNode->CurElement.EndPoint[1]) * (OutputPath.pEndNode->CurElement.EndPoint[1] - ncDxfSearchPath[i].pEndNode->CurElement.EndPoint[1]);
		if(LengthStart < LengtEnd)//下一个路径的起点与上一个路径的终点的距离短于下一个路径的终点与上一个路径的终点的距离
		{
			//插入一个抬刀的移动过程
			InsertG00PathCreate(&OutputPath,ncDxfSearchPath[i].pHeadNode->CurElement.StartPoint);
			//把获取的新路径接在末端点					
			ncDxfSearchPath[i].pHeadNode->pLastNode  = OutputPath.pEndNode;
			OutputPath.pEndNode->pNextNode = ncDxfSearchPath[i].pHeadNode;
			OutputPath.pEndNode = ncDxfSearchPath[i].pEndNode;
		}
		else
		{
			//插入一个抬刀的移动过程
			InsertG00PathCreate(&OutputPath,ncDxfSearchPath[i].pEndNode->CurElement.EndPoint);
			//把获取的新路径始末端点反转
			CurWorkNode = ncDxfSearchPath[i].pHeadNode;
			j=ncDxfSearchPath[i].RealGetNum;
			while(j > 0)
			{
				temp[0] = CurWorkNode->CurElement.StartPoint[0];
				temp[1] = CurWorkNode->CurElement.StartPoint[1];
				temp[2] = CurWorkNode->CurElement.StartPoint[2];
				CurWorkNode->CurElement.StartPoint[0] = CurWorkNode->CurElement.EndPoint[0];
				CurWorkNode->CurElement.StartPoint[1] = CurWorkNode->CurElement.EndPoint[1];
				CurWorkNode->CurElement.StartPoint[2] = CurWorkNode->CurElement.EndPoint[2];
				CurWorkNode->CurElement.EndPoint[0] = temp[0];
				CurWorkNode->CurElement.EndPoint[1] = temp[1];
				CurWorkNode->CurElement.EndPoint[2] = temp[2];
				if(CurWorkNode->CurElement.type == NC_ARC)
					CurWorkNode->CurElement.type = NC_G03_ARC;
				else if(CurWorkNode->CurElement.type == NC_G03_ARC)
					CurWorkNode->CurElement.type = NC_ARC;
				CurWorkNode = CurWorkNode->pNextNode;
				j--;
			}
			//把获取的新路径连接顺序反转		
			CurTempNode = ncDxfSearchPath[i].pEndNode->pLastNode;
			CurWorkNode = ncDxfSearchPath[i].pEndNode;
			CurTempNode2 = ncDxfSearchPath[i].pHeadNode;
			CurTempNode3 = ncDxfSearchPath[i].pEndNode;
			while(CurWorkNode&&CurTempNode)
			{
				CurTempNode1 = CurTempNode->pLastNode;
				CurWorkNode->pNextNode = CurTempNode;
				CurTempNode->pLastNode = CurWorkNode;
				CurWorkNode = CurTempNode;
				CurTempNode = CurTempNode1;
			}

			OutputPath.pEndNode->pNextNode = CurTempNode3;
			CurTempNode3->pLastNode = OutputPath.pEndNode;
			OutputPath.pEndNode = CurTempNode2;
			OutputPath.pEndNode->pNextNode=NULL;
		}
		OutputPath.RealGetNum += ncDxfSearchPath[i].RealGetNum;
	}
	if(SaveFile)
	{
		CurWorkNode = OutputPath.pHeadNode;
		i = OutputPath.RealGetNum;
		while(CurWorkNode && i > -1)
		{			
			if(CurWorkNode->CurElement.type == NC_FAST)
			{	
				if (skipFlag)
				{
					CurWorkNode = CurWorkNode->pNextNode->pNextNode;
					i--;
					skipFlag = FALSE;
					continue;
				}
			}
			else
			{
				for(j = 0,Feed = 1000; j < 18; j++)
				{
					if(DxfColorToFeed[j][0] == CurWorkNode->CurElement.color)
					{
						Feed = DxfColorToFeed[j][1];
						break;
					}
				}

				if(CurWorkNode->CurElement.type == NC_LINE)
				{
					m_Length = GetDistance(CPoint2D(CurWorkNode->CurElement.StartPoint[0],CurWorkNode->CurElement.StartPoint[1]),CPoint2D(CurWorkNode->CurElement.EndPoint[0], CurWorkNode->CurElement.EndPoint[1]));

					if(m_Length < 50)
						m_LineStep = g_fLineStep_S;
					else
						m_LineStep = g_fLineStep_L;

					m_count = g_iLength / m_LineStep;
					double m_stepX = (CurWorkNode->CurElement.EndPoint[0] - CurWorkNode->CurElement.StartPoint[0]) / m_count;
					double m_stepY = (CurWorkNode->CurElement.EndPoint[1] - CurWorkNode->CurElement.StartPoint[1]) / m_count;

					if(m_stepX == 0.0)
					{	
						if(CurWorkNode->CurElement.EndPoint[1] > CurWorkNode->CurElement.StartPoint[1])
							c = PI / 2 + offset * PI / 180;
						else
							c = 3 * PI / 2 + offset * PI / 180;
					}
					else
					{
						c = atan((double)(CurWorkNode->CurElement.EndPoint[1] - CurWorkNode->CurElement.StartPoint[1]) / (double)(CurWorkNode->CurElement.EndPoint[0] - CurWorkNode->CurElement.StartPoint[0]));
						c = GetC(CurWorkNode->CurElement.StartPoint[0],CurWorkNode->CurElement.StartPoint[1],CurWorkNode->CurElement.EndPoint[0],CurWorkNode->CurElement.EndPoint[1],c) + g_fOffsetDegree * PI / 180;
					}

					fprintf_s(SaveFile,"F%d G00 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iG00Speed,CurWorkNode->CurElement.StartPoint[0], CurWorkNode->CurElement.StartPoint[1],g_fSafeHeight,c);

					fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iDownSpeed,CurWorkNode->CurElement.StartPoint[0],CurWorkNode->CurElement.StartPoint[1],0.0,c);
					fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iDownSpeed,CurWorkNode->CurElement.StartPoint[0],CurWorkNode->CurElement.StartPoint[1],z,c);

					for(int i = 1;i < m_count;++i)
					{
						fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iFeedSpeed,CurWorkNode->CurElement.StartPoint[0] + m_stepX * i,CurWorkNode->CurElement.StartPoint[1] + m_stepY * i,z,c);
					}
					fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iUpSpeed,CurWorkNode->CurElement.EndPoint[0],CurWorkNode->CurElement.EndPoint[1],0.0,c);
					fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iUpSpeed,CurWorkNode->CurElement.EndPoint[0],CurWorkNode->CurElement.EndPoint[1],g_fSafeHeight,c);//yuanlin

				}
				else if(CurWorkNode->CurElement.type == NC_ARC)
				{
					fOX = CurWorkNode->CurElement.CentralPoint[0];
					fOY = CurWorkNode->CurElement.CentralPoint[1];
					m_fRadius = CurWorkNode->CurElement.Para;
					temAngle = GetAngle(CPoint2D(CurWorkNode->CurElement.StartPoint[0],CurWorkNode->CurElement.StartPoint[1]),CPoint2D(CurWorkNode->CurElement.EndPoint[0],CurWorkNode->CurElement.EndPoint[1]),m_fRadius);

					if((CurWorkNode->CurElement.StartPoint[1] - fOY) * (CurWorkNode->CurElement.EndPoint[0] - fOX) - (CurWorkNode->CurElement.EndPoint[1] - fOY) * (CurWorkNode->CurElement.StartPoint[0] - fOX) > 0)
						temAngle = 2 * PI - temAngle;
					m_Length = m_fRadius * temAngle;
					//*****************start************************
					//判断半径大小，决定弧长步长 by yuanlin 2013.5.21
					if (m_fRadius<=30)
						g_fCurveStep_Len=g_fCurveStep_S;
					else if (m_fRadius<=50)
						g_fCurveStep_Len=g_fCurveStep_M;
					else if(m_fRadius<=100)
						g_fCurveStep_Len=g_fCurveStep_L;
					else if(m_fRadius<=100)
						g_fCurveStep_Len=g_fCurveStep_XL;
					m_count = m_Length / g_fCurveStep_Len;
					//*****************end************************
					m_unit = temAngle / m_count;
					x = CurWorkNode->CurElement.StartPoint[0];
					y = CurWorkNode->CurElement.StartPoint[1];

					if(CurWorkNode->CurElement.EndPoint[0] > 999999 || CurWorkNode->CurElement.EndPoint[0] < -999999)
						CurWorkNode->CurElement.EndPoint[0] = 0;
					if(CurWorkNode->CurElement.EndPoint[1] > 999999 || CurWorkNode->CurElement.EndPoint[1] < -999999)
						CurWorkNode->CurElement.EndPoint[1] = 0;
					for(int i = 0;i < m_count;++i)
					{
						temX = x,temY = y;
						x = (temX - fOX) * cos(m_unit) - (temY - fOY) * sin(m_unit) + fOX;
						y = (temY - fOY) * cos(m_unit) + (temX - fOX) * sin(m_unit) + fOY;
						//z += 0.1;
						c = atan((y - temY) / (x - temX));
						c = GetC(temX,temY,x,y,c) + g_fOffsetDegree * PI / 180;
						fprintf_s(SaveFile,"F%d G02 X%.3f Y%.3f Z%.3f I%f J%.3f K%.3f C%.3f;\n",g_iFeedSpeed, x, y,z, fOX - temX,fOY - temY,z,c);
					}
					fprintf_s(SaveFile,"F%d G02 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iUpSpeed,CurWorkNode->CurElement.EndPoint[0],CurWorkNode->CurElement.EndPoint[1],z,c);//yuanlin
					fprintf_s(SaveFile,"F%d G02 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iUpSpeed,CurWorkNode->CurElement.EndPoint[0],CurWorkNode->CurElement.EndPoint[1],g_fSafeHeight,c);//yuanlin
				}
				else if(CurWorkNode->CurElement.type == NC_G03_ARC)
				{
					fOX = CurWorkNode->CurElement.CentralPoint[0];
					fOY = CurWorkNode->CurElement.CentralPoint[1];
					m_fRadius = CurWorkNode->CurElement.Para;
					temAngle = GetAngle(CPoint2D(CurWorkNode->CurElement.StartPoint[0],CurWorkNode->CurElement.StartPoint[1]),CPoint2D(CurWorkNode->CurElement.EndPoint[0],CurWorkNode->CurElement.EndPoint[1]),m_fRadius);
					if((CurWorkNode->CurElement.StartPoint[1] - fOY) * (CurWorkNode->CurElement.EndPoint[0] - fOX) - (CurWorkNode->CurElement.EndPoint[1] - fOY) * (CurWorkNode->CurElement.StartPoint[0] - fOX) < 0)
						temAngle = 2 * PI - temAngle;
					m_Length = m_fRadius * temAngle;
					//*****************start************************
					//判断半径大小，决定弧长步长 by yuanlin 2013.5.21
					if (m_fRadius<=30)
						g_fCurveStep_Len=g_fCurveStep_S;
					else if (m_fRadius<=50)
						g_fCurveStep_Len=g_fCurveStep_M;
					else if(m_fRadius<=100)
						g_fCurveStep_Len=g_fCurveStep_L;
					else if(m_fRadius<=100)
						g_fCurveStep_Len=g_fCurveStep_XL;
					m_count = m_Length / g_fCurveStep_Len;
					//*****************end************************
					m_unit = temAngle / m_count;
					x = CurWorkNode->CurElement.StartPoint[0];
					y = CurWorkNode->CurElement.StartPoint[1];
					for(int i = 0;i < m_count;++i)
					{
						temX = x,temY = y;
						x = (temX - fOX) * cos(m_unit) - (temY - fOY) * sin(m_unit) + fOX;
						y = (temY - fOY) * cos(m_unit) + (temX - fOX) * sin(m_unit) + fOY;
						//z 6+= 0.1;
						c = atan((y - temY) / (x - temX));
						c = GetC(temX,temY,x,y,c) + g_fOffsetDegree * PI / 180;
						fprintf_s(SaveFile,"F%d G03 X%.3f Y%.3f Z%.3f I%.3f J%.3f K%.3f C%.3f;\n",g_iFeedSpeed, x, y,z, fOX - temX,fOY - temY,z,c);
					}
					fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iUpSpeed,CurWorkNode->CurElement.EndPoint[0],CurWorkNode->CurElement.EndPoint[1],z,c);//yuanlin
					fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iUpSpeed,CurWorkNode->CurElement.EndPoint[0],CurWorkNode->CurElement.EndPoint[1],g_fSafeHeight,c);//yuanlin
				}
				else if(CurWorkNode->CurElement.type == NC_CIRCLE)
				{
					m_fRadius = CurWorkNode->CurElement.Para;
					m_Length = 2 * PI * m_fRadius;
					//*****************start************************
					//判断半径大小，决定弧长步长 by yuanlin 2013.5.21
					if (m_fRadius<=30)
						g_fCurveStep_Len=g_fCurveStep_S;
					else if (m_fRadius<=50)
						g_fCurveStep_Len=g_fCurveStep_M;
					else if(m_fRadius<=100)
						g_fCurveStep_Len=g_fCurveStep_L;
					else if(m_fRadius<=100)
						g_fCurveStep_Len=g_fCurveStep_XL;
					m_count = m_Length / g_fCurveStep_Len;
					//*****************end************************
					m_unit = 2 * PI / m_count;
					fOX = CurWorkNode->CurElement.CentralPoint[0];
					fOY = CurWorkNode->CurElement.CentralPoint[1];
					x = CurWorkNode->CurElement.EndPoint[0];
					y = CurWorkNode->CurElement.EndPoint[1];
					for(int i = 1;i < m_count;++i)
					{
						temX = x,temY = y;
						x = (temX - fOX) * cos(m_unit) - (temY - fOY) * sin(m_unit) + fOX;
						y = (temY - fOY) * cos(m_unit) + (temX - fOX) * sin(m_unit) + fOY;
						//z += 0.1;
						c = atan((y - temY) / (x - temX));
						c = GetC(temX,temY,x,y,c) + g_fOffsetDegree * PI / 180;
						fprintf_s(SaveFile,"F%d G02 X%.3f Y%.3f Z%.3f R%.3f C%.3f;\n",  
							g_iFeedSpeed,x, y, z,CurWorkNode->CurElement.Para,c);
					}
					fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iUpSpeed,CurWorkNode->CurElement.EndPoint[0],CurWorkNode->CurElement.EndPoint[1],z,c);//yuanlin
					fprintf_s(SaveFile,"F%d G01 X%.3f Y%.3f Z%.3f C%.3f;\n",g_iUpSpeed,CurWorkNode->CurElement.EndPoint[0],CurWorkNode->CurElement.EndPoint[1],g_fSafeHeight,c);//yuanlin
				}


				xOld = CurWorkNode->CurElement.EndPoint[0];
				yOld = CurWorkNode->CurElement.EndPoint[1];

				//fprintf_s(SaveFile,"F%ld	G01	 X%lf	Y%lf	Z%lf;\n",g_iUpSpeed,xOld,yOld,0.0);
				//fprintf_s(SaveFile,"F%ld	G01	 X%lf	Y%lf	Z%lf;\n",g_iUpSpeed,xOld,yOld,g_fSafeHeight);

			}
			CurWorkNode = CurWorkNode->pNextNode;
			i--;
			skipFlag = TRUE;
		}

		//fprintf_s(SaveFile,"F%ld	 G01	X%lf	Y%lf	Z%lf;\n",g_iUpSpeed,xOld,yOld,0.0);
		//fprintf_s(SaveFile,"F%ld	 G01	X%lf	Y%lf	Z%lf;\n",g_iUpSpeed,xOld,yOld,safeHeight);//yuanlin
		fprintf_s(SaveFile,"M02\n");
	}
}


void Test_CreationClass::InsertG00PathCreate(NcPathLink *pInsertPath,double EndPoint[3])
{
	NcElementLink *CurFastNode;
	BaseNcElement CurNcElement;
	CurNcElement.color = NC_CLOER_FAST;
	CurNcElement.type = NC_FAST;
	CurNcElement.LineType = NC_BROKEN_LINE;
	//需要插入一个从当前整圆定位到新获取整圆的定位新结点
	CurNcElement.StartPoint[0] = pInsertPath->pEndNode->CurElement.EndPoint[0];
	CurNcElement.StartPoint[1] = pInsertPath->pEndNode->CurElement.EndPoint[1];
	CurNcElement.StartPoint[2] = pInsertPath->pEndNode->CurElement.EndPoint[2];
	CurNcElement.EndPoint[0] = pInsertPath->pEndNode->CurElement.EndPoint[0];
	CurNcElement.EndPoint[1] = pInsertPath->pEndNode->CurElement.EndPoint[1];
	CurNcElement.EndPoint[2] = DxfToolSafeHeight;
	PushLinkNode(&CurNcElement);
	CurFastNode = PopLinkPathNode(&NcLinkFastPath);
	CurFastNode->pLastNode  = pInsertPath->pEndNode;
	pInsertPath->pEndNode->pNextNode = CurFastNode;
	pInsertPath->pEndNode = CurFastNode;
	pInsertPath->RealGetNum++;
	//需要插入一个从当前整圆定位到新获取整圆的定位新结点
	CurNcElement.StartPoint[0] = pInsertPath->pEndNode->CurElement.EndPoint[0];
	CurNcElement.StartPoint[1] = pInsertPath->pEndNode->CurElement.EndPoint[1];
	CurNcElement.StartPoint[2] = DxfToolSafeHeight;
	CurNcElement.EndPoint[0] = EndPoint[0];
	CurNcElement.EndPoint[1] = EndPoint[1];
	CurNcElement.EndPoint[2] = DxfToolSafeHeight;
	PushLinkNode(&CurNcElement);
	CurFastNode = PopLinkPathNode(&NcLinkFastPath);
	CurFastNode->pLastNode  = pInsertPath->pEndNode;
	pInsertPath->pEndNode->pNextNode = CurFastNode;
	pInsertPath->pEndNode = CurFastNode;
	pInsertPath->RealGetNum++;
	//需要插入一个从当前整圆定位到新获取整圆的定位新结点
	CurNcElement.StartPoint[0] = EndPoint[0];
	CurNcElement.StartPoint[1] = EndPoint[1];
	CurNcElement.StartPoint[2] = DxfToolSafeHeight;
	CurNcElement.EndPoint[0] = EndPoint[0];
	CurNcElement.EndPoint[1] = EndPoint[1];
	CurNcElement.EndPoint[2] = EndPoint[2];
	PushLinkNode(&CurNcElement);
	CurFastNode = PopLinkPathNode(&NcLinkFastPath);
	CurFastNode->pLastNode  = pInsertPath->pEndNode;
	pInsertPath->pEndNode->pNextNode = CurFastNode;
	pInsertPath->pEndNode = CurFastNode;
	pInsertPath->RealGetNum++;
}


void Test_CreationClass::PathCircleProcess(NcPathLink *pInsertPath)
{
	int i = NcLinkCirclePath.RealGetNum - 1,j = -1;
	NcElementLink *CurWorkNode = NULL,*CurFastNode = NULL;
	double Length[3],AbsLength[2];
	int NoLinkSign = 0;

	BaseNcElement CurNcElement;
	CurNcElement.color = NC_CLOER_FAST;
	CurNcElement.type = NC_FAST;
	CurNcElement.LineType = NC_BROKEN_LINE;

	if(pInsertPath->RealGetNum == 0)
	{//如果没有有效的直线或者圆弧前置结点则把整圆当作新的路径的起点
		CurWorkNode = PopLinkPathNode(&NcLinkCirclePath);
		if(CurWorkNode == NULL)
			return;
		pInsertPath->pHeadNode = CurWorkNode;
		pInsertPath->pEndNode = CurWorkNode;
		pInsertPath->RealGetNum++;

		CurNcElement.StartPoint[0] = 0;
		CurNcElement.StartPoint[1] = 0;
		CurNcElement.StartPoint[2] = 0;
		CurNcElement.EndPoint[0] = pInsertPath->pEndNode->CurElement.StartPoint[0];
		CurNcElement.EndPoint[1] = pInsertPath->pEndNode->CurElement.StartPoint[1];
		CurNcElement.EndPoint[2] = pInsertPath->pEndNode->CurElement.StartPoint[2];
		PushLinkNode(&CurNcElement);
		CurFastNode = PopLinkPathNode(&NcLinkFastPath);
		CurFastNode->pNextNode  = pInsertPath->pHeadNode;
		pInsertPath->pHeadNode->pLastNode = CurFastNode;
		pInsertPath->pHeadNode = CurFastNode;
		pInsertPath->RealGetNum++;
	}

	while(i > 0)
	{
		if(pInsertPath->RealGetNum <= 0)
			NoLinkSign = 0;
		//当可以重复匹配的机会数全部用完时证明不需要再尝试了；
		//或者所有的结点均被有效匹配，那表示路径搜索完毕。
		CurWorkNode = PopLinkPathNode(&NcLinkCirclePath);
		i--;
		if(CurWorkNode == NULL)
			break;
		pInsertPath->RealGetNum++;

		if(pInsertPath->pHeadNode->CurElement.type != NC_CIRCLE)//整圆接直线或者圆弧的处理
		{
			Length[0] = (pInsertPath->pHeadNode->CurElement.StartPoint[0] - CurWorkNode->CurElement.CentralPoint[0]) * (pInsertPath->pHeadNode->CurElement.StartPoint[0] - CurWorkNode->CurElement.CentralPoint[0]);
			Length[1] = (pInsertPath->pHeadNode->CurElement.StartPoint[1] - CurWorkNode->CurElement.CentralPoint[1]) * (pInsertPath->pHeadNode->CurElement.StartPoint[1] - CurWorkNode->CurElement.CentralPoint[1]);
			AbsLength[0] = sqrt(Length[0] + Length[1]);
			if(fabs(AbsLength[0] - CurWorkNode->CurElement.Para) < MIN_F)//直接连接起始端
			{//新的结点插到起始端
				CurWorkNode->pNextNode = pInsertPath->pHeadNode;
				pInsertPath->pHeadNode->pLastNode = CurWorkNode;
				CurWorkNode->CurElement.StartPoint[0] = pInsertPath->pHeadNode->CurElement.StartPoint[0];
				CurWorkNode->CurElement.StartPoint[1] = pInsertPath->pHeadNode->CurElement.StartPoint[1];
				CurWorkNode->CurElement.StartPoint[2] = pInsertPath->pHeadNode->CurElement.StartPoint[2];
				CurWorkNode->CurElement.EndPoint[0] = pInsertPath->pHeadNode->CurElement.StartPoint[0];
				CurWorkNode->CurElement.EndPoint[1] = pInsertPath->pHeadNode->CurElement.StartPoint[1];
				CurWorkNode->CurElement.EndPoint[2] = pInsertPath->pHeadNode->CurElement.StartPoint[2];
				pInsertPath->pHeadNode = CurWorkNode;
				j =- 1;
				NoLinkSign = 0;
				continue;
			}
			else if(pInsertPath->pEndNode->CurElement.type != NC_CIRCLE)
			{
				if(j == -1)
				{//如果首次发生不匹配则记住可以重复匹配的机会数等于余下的所有结点数
					j = NcLinkCirclePath.RealGetNum;
					i = j;
				}
				NoLinkSign = 1;
			}
		}
		if(pInsertPath->pEndNode->CurElement.type != NC_CIRCLE)//整圆接直线或者圆弧的处理
		{
			Length[0] = (pInsertPath->pEndNode->CurElement.EndPoint[0] - CurWorkNode->CurElement.CentralPoint[0])*(pInsertPath->pEndNode->CurElement.EndPoint[0] - CurWorkNode->CurElement.CentralPoint[0]);
			Length[1] = (pInsertPath->pEndNode->CurElement.EndPoint[1] - CurWorkNode->CurElement.CentralPoint[1])*(pInsertPath->pEndNode->CurElement.EndPoint[1] - CurWorkNode->CurElement.CentralPoint[1]);
			AbsLength[1] = sqrt(Length[0] + Length[1]);
			if(fabs(AbsLength[1] - CurWorkNode->CurElement.Para) < MIN_F)//直接连接结束端
			{//新的结点插到结束端
				CurWorkNode->pLastNode  = pInsertPath->pEndNode;
				pInsertPath->pEndNode->pNextNode = CurWorkNode;
				CurWorkNode->CurElement.StartPoint[0] = pInsertPath->pEndNode->CurElement.EndPoint[0];
				CurWorkNode->CurElement.StartPoint[1] = pInsertPath->pEndNode->CurElement.EndPoint[1];
				CurWorkNode->CurElement.StartPoint[2] = pInsertPath->pEndNode->CurElement.EndPoint[2];
				CurWorkNode->CurElement.EndPoint[0] = pInsertPath->pEndNode->CurElement.EndPoint[0];
				CurWorkNode->CurElement.EndPoint[1] = pInsertPath->pEndNode->CurElement.EndPoint[1];
				CurWorkNode->CurElement.EndPoint[2] = pInsertPath->pEndNode->CurElement.EndPoint[2];
				pInsertPath->pEndNode = CurWorkNode;
				j = -1;
				NoLinkSign = 0;
				continue;
			}
			else if(pInsertPath->pHeadNode->CurElement.type != NC_CIRCLE)
			{
				if(j == -1)
				{//如果首次发生不匹配则记住可以重复匹配的机会数等于余下的所有结点数
					j = NcLinkCirclePath.RealGetNum;
					i = j;
				}
				NoLinkSign = 1;
			}
		}
		else if(pInsertPath->pEndNode->CurElement.type == NC_CIRCLE)//整圆接整圆的处理
		{
			//插入一个抬刀的移动过程
			InsertG00PathCreate(pInsertPath,CurWorkNode->CurElement.StartPoint);
			//接在末端点
			CurWorkNode->pLastNode  = pInsertPath->pEndNode;
			pInsertPath->pEndNode->pNextNode = CurWorkNode;
			pInsertPath->pEndNode = CurWorkNode;
			j = -1;
			NoLinkSign = 0;
		}
		if(NoLinkSign)
		{
			NoLinkSign = 0;
			pInsertPath->RealGetNum--;
			PushBackLinkPathNode(CurWorkNode,&NcLinkCirclePath);
		}
	}
}


double Test_CreationClass::GetDistance(CPoint2D first,CPoint2D end)
{
	return sqrt((double)((first.x - end.x) * (first.x - end.x) + (first.y - end.y) * (first.y - end.y)));
}

double Test_CreationClass::GetAngle(CPoint2D first,CPoint2D end,double m_fRadius)
{
	return asin(GetDistance(first,end) / 2.0 / m_fRadius) * 2;
}


double Test_CreationClass::GetC(double startX, double startY, double endX, double endY, double c)
{
	if(endY - startY > 0.0 && endX - startX > 0.0)
		;
	if(endY - startY >= 0.0 && endX - startX < 0.0)
		c += PI;
	if(endY - startY < 0.0 && endX - startX >= 0.0)
		c += PI * 2;
	if(endY - startY < 0.0 && endX - startX < 0.0)
		c += PI;
	return c;
}

/*
**从DXF文件中获取颜色相对应的整数值来转换成相应的RGB**
*/
COLORREF Test_CreationClass::IntToRGB(int colorNum)
{
	if(colorNum == 1)
		g_clr = RGB(255, 0, 0);
	else if(colorNum == 2)
		g_clr = RGB(255, 255, 0);
	else if(colorNum == 3)
		g_clr = RGB(0, 255, 0);
	else if(colorNum == 5)
		g_clr = RGB(0, 0, 255);
	else if(colorNum == 256 || colorNum == 0)
		g_clr = RGB(0, 0, 0);
	return g_clr;
}
