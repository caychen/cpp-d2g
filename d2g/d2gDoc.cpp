// d2gDoc.cpp : Cd2gDoc 类的实现
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


// Cd2gDoc 构造/析构

Cd2gDoc::Cd2gDoc()
{
	// TODO: 在此添加一次性构造代码

}

Cd2gDoc::~Cd2gDoc()
{
}

BOOL Cd2gDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	//SetTitle((LPCTSTR)"杭州玻邦图形转TXT应用程序");

	
	return TRUE;
}




// Cd2gDoc 序列化

void Cd2gDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// Cd2gDoc 诊断

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


// Cd2gDoc 命令
