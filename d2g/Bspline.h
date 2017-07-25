#pragma once

#include "Vec.h"
#include<vector>

class CBspline
{
public:
	CBspline(void);
	virtual ~CBspline();


	void Hartley(void);
	CBspline(vector<vec2>pt);
	void ComputeInterpolation(void);//�����ֵ(����)
	void DrawInterpolation(void);//�����ֵ(��ͼ)
	void ComputeNode(void);//����ڵ�ʸ��
	vector<vec2>m_interpolationNode;
private:
	unsigned int m_count, m_controlNum;//���������ƶ������
	vector<vec2> m_controlPoint;//���ƶ���
	vector<float>m_node;//�ڵ�ʸ��
	
	
};
