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
	int GetCursorPosition(POINT point);//����ָ������Ա�ߵ�λ��
	void Show(CWnd* pWnd,BOOL visible = TRUE);//��ʾ���
	void ShowVernier(CDC* pDC,POINT point);//��ʾ�̶�
	void HideVernier(CDC* pDC);//���ؿ̶�
public:
	CFont vfont;
	CFont hfont;
	CRect vrect;//��ֱ�����ϵľ�����
	CRect hrect;//ˮƽ�����ϵľ�����
	POINT vernier;
	int width;//��߿��
	//int spacing;//��߿̶ȼ��
	SIZE ClientSize;//�ͻ�����С
protected:

};