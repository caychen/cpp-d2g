#pragma once


/**
* @file test_creationclass.h
*/

/*****************************************************************************
**  $Id: test_creationclass.h 219 2004-01-07 23:08:40Z andrew $
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

#ifndef TEST_CREATIONCLASS_H
#define TEST_CREATIONCLASS_H

#include "Windows.h"
#include "Graph.h"
#include "DL_CreationAdapter.h"
#include "FunctionArray.h"
#include "Vec.h"
#include <vector>

#include "GlobalDataType.h"
#include "TextGraph.h"
#include "UndoOperator.h"

#define  MAXNUM 1000

#define MIN_F           0.000001
#define MIN_SF         0.0001

#define  MAX_NC_SEG        65536
#define  MAX_NC_CIRCLE     5000
#define  MAX_PATH_NUM      10000

#define  NC_FAST                0
#define  NC_LINE                 1
#define  NC_ARC                  2
#define  NC_G03_ARC          3
#define  NC_CIRCLE             4
#define  NC_G03_CIRCLE     5
#define  NC_SPLINE			6
#define  NC_ELLIPSE			7
#define  NC_POINT			8
#define  NC_POLYLINE		9

#define  NC_CLOER_FAST       5

#define  NC_REAL_LINE         1
#define  NC_BROKEN_LINE     2
#define  NC_BORDER_LINE      3

enum colorDxf 
{
	black = 250,
	green = 3,
	red = 1,
	brown = 15,
	yellow = 2,
	cyan = 4,
	magenta = 6,
	gray = 8,
	blue = 5,
	l_blue = 163,
	l_green = 121,
	l_cyan = 131,
	l_red = 23,
	l_magenta = 221,
	l_gray = 252,
	white = 7,
	bylayer = 256,
	byblock = 0
};

typedef struct BaseNcElement
{
	int type;          //  1--ֱ��   2--Բ��  3--��Բ
	int color;        //   ��ͬ��ɫ��Ӧ��ͬ�Ľ���ֵ
	int LineType;   //  1--ʵ��  2--����  3--�߽���
	double StartPoint[3];
	double EndPoint[3];
	double CentralPoint[3];
	double Para;         // Ʃ��Բ��/��Բ�İ뾶
	//int controlNumber;	//�������ߵĿ��Ƶ���Ŀ
	//int realNumber;		//
	//int degree;
}BaseNcElement;

typedef struct NcElementLink
{
	int RealLinkNo;
	BaseNcElement CurElement;
	NcElementLink *pLastNode;//��һ���ڵ�
	NcElementLink *pNextNode;//��һ���ڵ�
}NcElementLink;

typedef struct NcPathLink
{
	int RealGetNum;
	NcElementLink *pHeadNode;
	NcElementLink *pEndNode;
}NcPathLink;

CString  DxfReading(CString DxfFileName); 

/**
* This class takes care of the entities read from the file.
* Usually such a class would probably store the entities.
* this one just prints some information about them to stdout.
*
* @author Andrew Mustun
*/

class Test_CreationClass :public DL_CreationAdapter
{
public:
	Test_CreationClass(CString FileName);
	~Test_CreationClass(void);

	virtual void addLayer(const DL_LayerData& data);

	//��
	virtual void addPoint(const DL_PointData& data);

	//ֱ��
	virtual void addLine(const DL_LineData& data);

	//Բ��
	virtual void addArc(const DL_ArcData& data);

	//��Բ
	virtual void addCircle(const DL_CircleData& data);

	//�����
	virtual void addPolyline(const DL_PolylineData& data);
	virtual void addVertex(const DL_VertexData& data);

	//��Բ
	virtual void addEllipse(const DL_EllipseData& data);

	//��������
	virtual void addSpline(const DL_SplineData& data);
	virtual void addControlPoint(const DL_ControlPointData& data);
	virtual void addKnot(const DL_KnotData& data);

	virtual void addText(const DL_TextData& data);
	virtual void addMText(const DL_MTextData& data);

	void printAttributes();

	void PushLinkNode(BaseNcElement *pNcElement);
	void PushBackLinkPathNode(NcElementLink *pNcElementLink, NcPathLink *pCurPath);
	NcElementLink *PopLinkPathNode(NcPathLink *pCurPath);
	void PathSearch(NcPathLink *pInsertPath);
	void InsertG00PathCreate(NcPathLink *pInsertPath,double EndPoint[3]);
	void OutputPathCreate(void);
	void PathCircleProcess(NcPathLink *pInsertPath);
	double GetDistance(CPoint2D first,CPoint2D end);
	double GetAngle(CPoint2D first,CPoint2D end,double m_fRadius);
	double GetC(double startX, double startY, double endX, double endY, double c);
	COLORREF IntToRGB(int colorNum);


private:
	CString mFileName;
	FILE *SaveFile;
	NcElementLink *NcLinkLineNode[MAX_NC_SEG - MAX_NC_CIRCLE - MAX_PATH_NUM];
	int RealLinkLineNodeNum;
	NcPathLink NcLinkLinePath;

	NcElementLink *NcLinkCircleNode[MAX_NC_CIRCLE];
	int RealLinkCircleNodeNum;
	NcPathLink NcLinkCirclePath;

	NcElementLink *NcLinkFastNode[MAX_PATH_NUM];
	int RealLinkFastNodeNum;
	NcPathLink NcLinkFastPath;

	NcPathLink OutputPath;
	int SearchedPathNum;
	NcPathLink ncDxfSearchPath[MAX_PATH_NUM];

	CGraph * pGraph;
	CGraph * qGraph;
	COLORREF g_clr;
	CFunctionArray funArray;
	CPointArray pt2Array;

	//�����
	int colorNum,flags,VertexNum;
	CArray <CPoint2D> VertexArray;

	//��������
	int degree;//����
	int controlNum;//���Ƶ���
	int knotNum;//�ڵ���
	double *controlPoint;
	double *knotPoint;
	vector<vec2> controlPtArray;
	CUndoOperator undoOperator;
	int m_IndexPolyLine;
};

#endif
