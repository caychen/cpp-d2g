//Ruler.h

#pragma once

//define CursorPosition
#define CP_OUTER		0	
#define CP_HORIZONTAL	1
#define CP_VERTICAL		2
#define CP_CORNER		3
#define	CP_CLIENT		4

class CRuler : public CObject
{
public:
	CRuler();
	virtual ~CRuler();
	int GetCursorPosition(POINT point);//返回指定点相对标尺的位置
	void Show(CWnd* pWnd,BOOL visible = TRUE);//显示标尺
	void ShowVernier(CDC* pDC,POINT point);//显示刻度
	void HideVernier(CDC* pDC);//隐藏刻度
public:
	CFont vfont;
	CFont hfont;
	CRect vrect;//垂直方向上的矩形条
	CRect hrect;//水平方向上的矩形条
	POINT vernier;
	int width;//标尺宽度
	//int spacing;//标尺刻度间距
	SIZE ClientSize;//客户区大小
protected:

};