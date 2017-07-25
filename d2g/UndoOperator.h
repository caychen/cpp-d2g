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
	CPoint2D m_OperatorPoint;//���ӡ�ɾ��ʱΪ0���ƶ�ʱΪ�ƶ�λ�õ�x��y��ɵ������ʾƫ��
};