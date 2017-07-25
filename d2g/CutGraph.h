#ifndef __CUTGRAPH_H_
#define __CUTGRAPH_H_

#include "Point2D.h"

//µ∂≤π¿‡
class CCutGraph
{
	public:
		CCutGraph(void);
		~CCutGraph();

		//÷±œﬂµ∂≤π¿‡
		CCutGraph(UINT m_iDrawType,CPoint2D m_ptOrigin,CPoint2D m_ptEnd);

		//‘≤ª°µ∂≤π¿‡
		CCutGraph(UINT m_iDrawType,CPoint2D m_ptOrigin,CPoint2D m_ptEnd,
			CPoint2D m_ptCenter,double m_radius,BOOL m_bDirection);

		UINT m_iDrawType;
		CPoint2D m_ptOrigin;
		CPoint2D m_ptEnd;
		CPoint2D m_ptCenter;
		BOOL m_bDirection;
		double m_radius;
};

#endif