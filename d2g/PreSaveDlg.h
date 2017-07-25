#pragma once


// CPreSaveDlg dialog

class CPreSaveDlg : public CDialog
{
	DECLARE_DYNAMIC(CPreSaveDlg)

public:
	CPreSaveDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPreSaveDlg();

// Dialog Data
	enum { IDD = IDD_PRESAVE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:
	double m_basePoint_X;
	double m_basePoint_Y;
	int m_SelectIndex;
	int m_saveAllOrSelected;
	int m_nOptimizeSelected;
};
