#ifndef __GOLBALDATATYPE_H_
#define __GOLBALDATATYPE_H_

#include <math.h>
#include <vector>
#include <stack>

class CPoint2D
{
public:
	double x;//�˵�X����
	double y;//�˵�Y����
	double con;//͹��

public:
	CPoint2D(double x = 0, double y = 0)
	{
		this->x = x;
		this->y = y;
	}

	CPoint2D(double x, double y, double con)
	{
		this->x = x;
		this->y = y;
		this->con = con;
	}

	virtual ~CPoint2D(void){}

	CPoint2D operator+(CPoint2D &p)
	{
		this->x += p.x;
		this->y += p.y;
		return *this;
	}

	CPoint2D operator-(CPoint2D &p)
	{
		this->x -= p.x;
		this->y -= p.y;
		return *this;
	}

	CPoint2D operator+=(CPoint2D &p)
	{
		this->x += p.x;
		this->y += p.y;
		return *this;
	}

	CPoint2D operator-=(CPoint2D &p)
	{
		this->x -= p.x;
		this->y -= p.y;
		return *this;
	}

	CPoint2D operator/(double r)
	{
		return CPoint2D(this->x / r, this->y / r);
	}

	CPoint2D& operator/=(double r)
	{
		this->x /= r;
		this->y /= r;
		return *this;
	}

	CPoint2D& operator*=(double r)
	{
		this->x *= r;
		this->y *= r;
		return *this;
	}

	BOOL operator==(CPoint2D &p)
	{
		if(fabs(this->x - p.x) < 10e-3 && fabs(this->y - p.y) < 10e-3)
			return TRUE;
		else
			return FALSE;
	}

	BOOL operator!=(CPoint2D &p)
	{
		/*if(fabs(this->x - p.x) < 10e-3 && fabs(this->y - p.y) < 10e-3)
			return TRUE;
		else
			return FALSE;*/
		if(*this == p)
			return FALSE;
		else
			return TRUE;
	}
};


class CInfomation
{
public:
	double x;
	double y;
	double c;

public:
	CInfomation(void){}
	virtual ~CInfomation(void){}

	CInfomation(double x, double y)
	{
		this->x = x;
		this->y = y;
	}

	CInfomation(double x, double y, double c)
	{
		this->x = x;
		this->y = y;
		this->c = c;
	}
};

//�߶���
class CLineInfo
{
public:
	int addrNo;
	CPoint2D sP;
	CPoint2D eP;

public:
	CLineInfo(void){}
	virtual ~CLineInfo(void){}
};

class CPointArray
{
public:
	CPoint2D p1, p2, p3;

public:
	//CPointArray(void){}
	CPointArray(CPoint2D p1 = 0, CPoint2D p2 = 0)
	{
		this->p1 = p1;
		this->p2 = p2;
	}

	CPointArray(CPoint2D p1, CPoint2D p2, CPoint2D p3)
	{
		this->p1 = p1;
		this->p2 = p2;
		this->p3 = p3;
	}
	virtual ~CPointArray(void){}
};

typedef struct tagToPointData
{
	int ToPointWay;//G00--->0,G01--->1
	CPoint2D ePoint;//�յ�����
}_2PointData;

enum IMITATETYPE
{
	START = 0,
	PAUSE,
	PREVIEW
};

enum CommandType	// ������
{
	CT_NONE = -1,
	CT_DRAW = 0,			// �滭
	CT_MOVE = 1,			// �ƶ�
	CT_ROTATE = 2,			// ��ת
	CT_MIRROR = 3,			// ����
	CT_ADJUSTLOCATION = 4,	// ����λ��
	CT_SCREENMOVE = 5,		// ��Ļ�ƶ�
	CT_IMITATE,				// ģ��
	CT_SET_A_ANGLE,			// �����¿ڽ�
	CT_COPY					// ���ƣ�06-27�޸�Cf��
};

typedef struct tagToolManage
{
	TCHAR ToolNo[10];
	TCHAR ToolName[10];
	double ToolRadius;
	int RotationRate;
}ToolManage;


class CGroupGraph
{
public:
	CGroupGraph()
	: m_bConnection(FALSE)
	{
		m_GraphIndex.clear();
	}

	virtual ~CGroupGraph(){}

public:
	std::vector<int> m_GraphIndex;
	BOOL m_bConnection;
};

#endif