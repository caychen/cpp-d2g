// d2gDoc.h : Cd2gDoc ��Ľӿ�
//


#pragma once


class Cd2gDoc : public CDocument
{
protected: // �������л�����
	Cd2gDoc();
	DECLARE_DYNCREATE(Cd2gDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~Cd2gDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


