#pragma once
#include "afxwin.h"

// CParameterDialog dialog

class CParameterDialog : public CDialog
{
	DECLARE_DYNAMIC(CParameterDialog)

public:
	CParameterDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CParameterDialog();

// Dialog Data
	enum { IDD = IDD_PARAMETERSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

public:
	CComboBox* pToolComboBox;
	CComboBox* pColorComboBox;
public:
	int m_iFeedSpeed;
	double m_fDepth;
	CString m_strToolSelect;
	double m_LineStep_L;
	double m_LineStep_S;
	int m_fSafeHeight;
	int m_iG00speed;
	int m_iDownSpeed;
	int m_iUpSpeed;
	double m_CurveStep;
	CString m_clr;
	double m_CurveStep_M;//曲线步长by yuanlin 13.5.21
	double m_CurveStep_L;//曲线步长by yuanlin 13.5.21
	double m_CurveStep_XL;//曲线步长by yuanlin 13.5.21
	int m_iLength;
	int m_iWidth;
	double m_EllipseStep_S;
	double m_ElipseStep_M;
	double m_EllipseStep_L;
	double m_EllipseStep_XL;
	double m_fSplineStep;
	int m_iWorkStation;
	//double m_fPointRadius;
	double m_Asymptotic;
	double m_fCoincideRatio;
	int m_iCoincideSpeed;
	double m_fPointPerDepth;
	int m_iPointSpeed;
	double m_fDeltaAngle;
public:
	int m_iCarveWay;
};