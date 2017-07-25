#include "stdafx.h"
#include "CutGraph.h"

CCutGraph::CCutGraph(void)
{

}

CCutGraph::~CCutGraph()
{

}


CCutGraph::CCutGraph(UINT m_DrawType, CPointMember m_ptOrigin, CPointMember m_ptEnd)
{
	this->m_DrawType = m_DrawType;
	this->m_ptOrigin = m_ptOrigin;
	this->m_ptEnd = m_ptEnd;
}

CCutGraph::CCutGraph(UINT m_drawType, CPointMember m_ptOrigin, CPointMember m_ptEnd,
					 CPointMember m_ptCenter, double m_radius, BOOL m_direction)
{
	this->m_DrawType = m_DrawType;
	this->m_ptOrigin = m_ptOrigin;
	this->m_ptEnd = m_ptEnd;
	this->m_ptCenter = m_ptCenter;
	this->m_radius = m_radius;
	this->m_direction = m_direction;
}