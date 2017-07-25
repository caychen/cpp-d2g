// ParameterDialog.cpp : implementation file
//

#include "stdafx.h"
#include "d2g.h"
#include "ParameterDialog.h"


// CParameterDialog dialog

IMPLEMENT_DYNAMIC(CParameterDialog, CDialog)

CParameterDialog::CParameterDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CParameterDialog::IDD, pParent)
	, pToolComboBox(NULL)
	, m_iFeedSpeed(0)
	, m_fDepth(0)
	, m_strToolSelect(_T(""))
	, m_LineStep_S(0)
	, m_LineStep_L(0)
	, m_fSafeHeight(0)
	, m_iG00speed(0)
	, m_iDownSpeed(0)
	, m_iUpSpeed(0)
	, m_CurveStep(0)
	, m_clr(_T(""))
	, m_CurveStep_M(0)//曲线步长by yuanlin 13.5.22
	, m_CurveStep_L(0)//曲线步长by yuanlin 13.5.22
	, m_CurveStep_XL(0)//曲线步长by yuanlin 13.5.22
	, m_iLength(0)
	, m_iWidth(0)
	, m_fSplineStep(0)
	, m_iWorkStation(0)
	//, m_fPointRadius(0)
	, m_EllipseStep_S(0)
	, m_ElipseStep_M(0)
	, m_EllipseStep_L(0)
	, m_EllipseStep_XL(0)
	, m_Asymptotic(0)
	, m_fCoincideRatio(0)
	, m_iCoincideSpeed(0)
	, m_iPointSpeed(0)
	, m_fDeltaAngle(0)
	, m_iCarveWay(0)
{

}

CParameterDialog::~CParameterDialog()
{
}

void CParameterDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_FEEDSET, m_iFeedSpeed);
	DDX_Text(pDX, IDC_Z_POSSET, m_fDepth);
	//DDX_CBString(pDX, IDC_TOOLLIST, m_strToolSelect);
	DDX_Text(pDX, IDC_LINESTEP_S, m_LineStep_S);
	DDX_Text(pDX, IDC_LINESTEP_L, m_LineStep_L);
	DDX_Text(pDX, IDC_SAFEHEIGHT, m_fSafeHeight);
	DDX_Text(pDX, IDC_FEEDSPEED, m_iG00speed);
	DDX_Text(pDX, IDC_DOWNSPEED, m_iDownSpeed);
	DDX_Text(pDX, IDC_UPSPEED, m_iUpSpeed);
	DDX_Text(pDX, IDC_CURVESTEP, m_CurveStep);
	DDX_CBString(pDX, IDC_COLORLIST, m_clr);
	DDX_Text(pDX, IDC_CURVESTEP2, m_CurveStep_M);
	DDX_Text(pDX, IDC_CURVESTEP3, m_CurveStep_L);
	DDX_Text(pDX, IDC_CURVESTEP4, m_CurveStep_XL);
	DDX_Text(pDX, IDC_LENGTH, m_iLength);
	DDX_Text(pDX, IDC_WIDTH, m_iWidth);
	DDX_Text(pDX, IDC_ELLIPSESTEP_S, m_EllipseStep_S);
	DDX_Text(pDX, IDC_ELLIPSESTEP_M, m_ElipseStep_M);
	DDX_Text(pDX, IDC_ELLIPSESTEP_L, m_EllipseStep_L);
	DDX_Text(pDX, IDC_ELLIPSESTEP_XL, m_EllipseStep_XL);

	DDX_Text(pDX, IDC_SPLINESTEP, m_fSplineStep);
	//DDX_Radio(pDX, IDC_RADIO9, m_iWorkStation);
	//DDX_Radio(pDX, IDC_RADIO1, m_iDownWay);
	//DDX_Text(pDX, IDC_POINTRADIUS, m_fPointRadius);
	//DDX_Text(pDX, IDC_LINESTEP_S, g_fLineStep_S);
	DDX_Text(pDX, IDC_ASYMPTOTIC, m_Asymptotic);
	DDX_Text(pDX, IDE_COINCIDE_RATIO, m_fCoincideRatio);
	DDX_Text(pDX, IDE_COINCIDE_SPEED, m_iCoincideSpeed);
	DDX_Text(pDX, IDE_POINTDEPTH_PER, m_fPointPerDepth);
	DDX_Text(pDX, IDE_POINT_SPEED, m_iPointSpeed);
	DDX_Text(pDX, IDE_DELTA_ANGLE, m_fDeltaAngle);
	DDX_Radio(pDX, IDC_RADIO1, m_iCarveWay);
}


BEGIN_MESSAGE_MAP(CParameterDialog, CDialog)
	
END_MESSAGE_MAP()


// CParameterDialog message handlers

BOOL CParameterDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	
	/*CWnd *pWnd = GetDlgItem(IDD_PARAMETERSET);
	pToolComboBox = (CComboBox*)GetDlgItem(IDC_TOOLLIST);
	pToolComboBox->InsertString(0,_TEXT("T1"));
	pToolComboBox->InsertString(1,_TEXT("T2"));
	pToolComboBox->InsertString(2,_TEXT("T3"));
	pToolComboBox->InsertString(3,_TEXT("T4"));
	pToolComboBox->InsertString(4,_TEXT("T5"));
	pToolComboBox->InsertString(5,_TEXT("T6"));
	pToolComboBox->InsertString(6,_TEXT("T7"));
	pToolComboBox->InsertString(7,_TEXT("T8"));
	pToolComboBox->InsertString(8,_TEXT("T9"));
	pToolComboBox->InsertString(9,_TEXT("T10"));*/

	pColorComboBox = (CComboBox*)GetDlgItem(IDC_COLORLIST);
	pColorComboBox->InsertString(0, _TEXT("Black"));
	pColorComboBox->InsertString(1, _TEXT("Red"));
	pColorComboBox->InsertString(2, _TEXT("Green"));
	pColorComboBox->InsertString(3, _TEXT("Blue"));


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

