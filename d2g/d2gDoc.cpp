// d2gDoc.cpp : Cd2gDoc ���ʵ��
//

#include "stdafx.h"
#include "d2g.h"

#include "d2gDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// Cd2gDoc

IMPLEMENT_DYNCREATE(Cd2gDoc, CDocument)

BEGIN_MESSAGE_MAP(Cd2gDoc, CDocument)
END_MESSAGE_MAP()


// Cd2gDoc ����/����

Cd2gDoc::Cd2gDoc()
{
	// TODO: �ڴ����һ���Թ������

}

Cd2gDoc::~Cd2gDoc()
{
}

BOOL Cd2gDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	//SetTitle((LPCTSTR)"���ݲ���ͼ��תTXTӦ�ó���");

	
	return TRUE;
}




// Cd2gDoc ���л�

void Cd2gDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// Cd2gDoc ���

#ifdef _DEBUG
void Cd2gDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void Cd2gDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Cd2gDoc ����
