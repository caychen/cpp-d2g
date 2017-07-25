#include "StdAfx.h"
#include "Bspline.h"

CBspline::CBspline(void)
{
}

CBspline::~CBspline(void)
{
}

CBspline::CBspline(std::vector<vec2> pt)
{
	m_controlNum = pt.size() - 1;
	if (pt.size() < 4)
	{
		m_count = pt.size() - 1;
	}
	else 
		m_count = 3;
	m_controlPoint.resize(pt.size());

	memcpy(&m_controlPoint[0], &pt[0], pt.size() * sizeof(vec2));
	m_node.resize(m_controlNum + m_count + 2);
}

void CBspline::ComputeInterpolation()
{
	m_interpolationNode.clear();
	ComputeNode();

	int m,n;
	float interpolationNumber;
	vector<vec2> relevantPoint;
	vector<float>relevantU;
	relevantPoint.resize(m_count + 1);
	relevantU.resize(2 * m_count);

	float u = 0;
	int i = m_count;//m表示u值所在的节点,则控制顶点为m_controlpoint[i-k]\m_controlpoint[i]
	float a;
	for(interpolationNumber = 0.0; interpolationNumber < MAX_LENGTH; interpolationNumber++)
	{
		n = 0;
		m = 0;
		u = interpolationNumber / MAX_LENGTH;
		if(u > m_node[i + 1])
			i++;

		for(m = i - m_count; m <= i; m++)
		{   
			relevantPoint[n] = m_controlPoint[m];
			n++;
		}

		//相关节点矢量拷贝
		n = 0;
		for(m = i - m_count + 1; m <= i + m_count; m++)
		{   
			relevantU[n] = m_node[m];
			n++;
		}

		//开始插值计
		for (n = 1; n < relevantPoint.size(); n++)
		{
			for(m = 0; m < relevantPoint.size() - n; m++)
			{   
				a = relevantU[m_count + m] - relevantU[m + n - 1];
				if (fabs(a) < 1e-5)
					a = 0.0;
				else  
					a = (u - relevantU[m + n - 1]) / a;

				relevantPoint[m] = (1 - a) * relevantPoint[m] + a * relevantPoint[m + 1];
			}
		}
		m_interpolationNode.push_back(relevantPoint[0]);
	}
}


void CBspline::DrawInterpolation(void)
{
	m_interpolationNode.clear();
	ComputeNode();

	int m,n;
	float interpolationNumber;
	vector<vec2> relevantPoint;
	vector<float>relevantU;
	relevantPoint.resize(m_count + 1);
	relevantU.resize(2 * m_count);

	float u = 0;
	int i = m_count;//m表示u值所在的节点,则控制顶点为m_controlpoint[i-k]\m_controlpoint[i]
	float a;
	for(interpolationNumber = 0.0; interpolationNumber < 3000; interpolationNumber++)
	{
		n = 0;
		m = 0;
		u = interpolationNumber / 3000;
		if(u > m_node[i + 1])
			i++;

		for(m = i - m_count; m <= i; m++)
		{   
			relevantPoint[n] = m_controlPoint[m];
			n++;
		}

		//相关节点矢量拷贝
		n = 0;
		for(m = i - m_count + 1; m <= i + m_count; m++)
		{   
			relevantU[n] = m_node[m];
			n++;	
		}

		//开始插值计算
		for (n = 1; n < relevantPoint.size(); n++)
		{
			for(m = 0; m < relevantPoint.size() - n; m++)
			{   
				a = relevantU[m_count + m] - relevantU[m + n - 1];
				if (fabs(a) < 1e-5)
					a = 0.0;
				else  
					a = (u - relevantU[m + n - 1]) / a;

				relevantPoint[m] = (1 - a) * relevantPoint[m] + a * relevantPoint[m + 1];
			}
		}

		m_interpolationNode.push_back(relevantPoint[0]);
	}
}

void CBspline::ComputeNode(void)
{ 
	float u = 0;
	for (int i = 0; i <= m_count; i++)
		m_node[i] = 0;

	for(int i = 1; i <= m_count + 1; i++)
		m_node[i + m_controlNum] = 1;

	Hartley();
}

void CBspline::Hartley(void)
{
	int i,j;
	float u = 0;
	float sum = 0;
	float s = 0;
	vector<float>length(m_controlNum);
	vector<float>ll;
	ll.push_back(0.0);
	vec2 pq;

	for(i = 0; i < m_controlNum; i++)
	{
		pq = m_controlPoint[i + 1] - m_controlPoint[i];
		length[i] = len(pq);
	}

	for (i = 0; i <= length.size() - m_count; i++)
	{   
		s = 0;
		for(j = 0; j < m_count; j++)
		{
			s += length[i + j];
		}
		sum += s;
		ll.push_back(s);
	}

	for(i = 1; i < ll.size(); i++)
	{   
		ll[i] = ll[i - 1] + ll[i];
		m_node[i + m_count] = ll[i] / sum;
	}
}