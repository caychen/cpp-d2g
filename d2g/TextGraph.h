#pragma once
#include "GlobalDataType.h"

class CTextGraph
{
public:
	CTextGraph(void)
	: m_TextStyle(0)
	, m_InsertPoint(0)
	, m_TextHeight(0.0)
	, m_TextWidth(0.0)
	, m_Angle(0.0)
	, m_Text("")
	, m_Style("")
	, m_AttachPoint(1)
	, m_DrawingDirection(5)
	{

	}
	virtual ~CTextGraph(void){}

public:
	int m_TextStyle;//单行：1，多行：2
	CPoint2D m_InsertPoint;	
	double m_TextHeight;
	double m_TextWidth;
	double m_Angle;
	CString m_Text;
	CString m_Style;//文字样式
	int m_AttachPoint;
	int m_DrawingDirection;
};
