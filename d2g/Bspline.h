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
	void ComputeInterpolation(void);//计算插值(保存)
	void DrawInterpolation(void);//计算插值(画图)
	void ComputeNode(void);//计算节点矢量
	vector<vec2>m_interpolationNode;
private:
	unsigned int m_count, m_controlNum;//次数、控制顶点个数
	vector<vec2> m_controlPoint;//控制顶点
	vector<float>m_node;//节点矢量
	
	
};
