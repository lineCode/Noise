#pragma once
#include "afxwin.h"


// CARTDlgProjectSetting �Ի���

class CARTDlgProjectSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CARTDlgProjectSetting)

public:
	CARTDlgProjectSetting(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CARTDlgProjectSetting();

// �Ի�������
	enum { IDD = IDD_ARTDLG_PROJECT_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_CtrlWidth;
	CEdit m_CtrlHeight;
	CEdit m_CtrlBigRadius;
	CEdit m_CtrlBigDecay;
	CEdit m_CtrlBigCenterX;
	CEdit m_CtrlBigCenterY;
	CEdit m_CtrlSmallRadius;
	CEdit m_CtrlSmallDecay;
	CEdit m_CtrlSmallCenterX;
	CEdit m_CtrlSmallCenterY;
	int m_nWidth;
	int m_nHeight;
	double m_BigRadius;
	double m_BigDecay;
	double m_BigCenterX;
	double m_BigCenterY;
	double m_SmallRadius;
	double m_SmallDecay;
	double m_SmallCenterX;
	double m_SmallCenterY;
};