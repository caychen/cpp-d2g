#pragma once
#include "SliderCtrlEx.h"
#include "GlobalDataType.h"
#include <vector>
#include "buttonex.h"
#include "afxext.h"
#include "afxcmn.h"
using namespace std;

// CImitateSpeedDlg dialog

class CImitateSpeedDlg : public CDialog
{
	DECLARE_DYNAMIC(CImitateSpeedDlg)

public:
	CImitateSpeedDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CImitateSpeedDlg();

// Dialog Data
	enum { IDD = IDD_IMITATESPEED_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual void PostNcDestroy();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	CSliderCtrlEx m_SliderCtrl;
	CToolTipCtrl m_ToolTipCtrl;
	HWND hParentWnd;
	CProgressCtrl m_PrgCtrl;

//	HBITMAP hFastBmp;

	HICON hStartIcon;
	HICON hPauseIcon;
	HICON hFasterIcon;
	HICON hSlowerIcon;
	HICON hStopIcon;
	HICON hOpenIcon;

public:
	enum IMITATETYPE m_ImitateStatus;
	int m_SliderPosValue;	

public:
	afx_msg void OnNMCustomdrawSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnSlower();
	afx_msg void OnBnClickedBtnFaster();
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnPause();
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnEndDlg();
	afx_msg void OnPaint();
	afx_msg LRESULT OnUpdatePrg(WPARAM wParam, LPARAM lParam);

public:	
	void SetParentWnd(HWND hWnd);
	HWND GetParentWnd(void);
	
	
public:
	afx_msg void OnBnClickedBtnOpen();
};