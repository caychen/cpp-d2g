#pragma once
#include "afxwin.h"
#include "GlobalDataType.h"
#include <vector>

// CToolManageDlg dialog

class CToolManageDlg : public CDialog
{
	DECLARE_DYNAMIC(CToolManageDlg)

public:
	CToolManageDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CToolManageDlg();

// Dialog Data
	enum { IDD = IDD_TOOLMANAGE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnCbnSelchangeToolNumberList();
	afx_msg void OnBnClickedBtnModify();
	afx_msg void OnBnClickedOk();

private:
	void SaveDataToConfigureFile();

private:
	BOOL m_bModifyFlag;
};
