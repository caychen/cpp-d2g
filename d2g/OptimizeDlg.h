#pragma once


// COptimizeDlg dialog

class COptimizeDlg : public CDialog
{
	DECLARE_DYNAMIC(COptimizeDlg)

public:
	COptimizeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COptimizeDlg();

// Dialog Data
	enum { IDD = IDD_OPTIMIZE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	double m_BasePointX;
	double m_BasePointY;
	int m_bIsOptimizeAll;
	int m_nOptimizeSelected;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioNo();
	afx_msg void OnBnClickedRadioYes();

public:
	void EnableControl(UINT nID, BOOL bEnabled = TRUE);
};
