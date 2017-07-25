#pragma once
//#include "Graph.h"

class CUndoOperator
{
public:
	CUndoOperator(void)
	: m_OperatorName(L"")
	, m_OperatorPoint()
	{
		m_OperatorGraph.clear();
		m_OperatorIndex.clear();
	}
	virtual ~CUndoOperator(void)
	{
		m_OperatorGraph.clear();
		m_OperatorIndex.clear();
	}

public:
	CString m_OperatorName;//ADD, DELETE, MOVE,
	std::vector<CGraph*> m_OperatorGraph;
	std::vector<int> m_OperatorIndex;
	CPoint2D m_OperatorPoint;//增加、删除时为0，移动时为移动位置的x和y组成的坐标表示偏移
};