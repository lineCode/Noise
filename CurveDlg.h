#pragma once

#include "resource.h"
#include "CurveWnd.h"

#define IDW_CURVE_WINDOW 999

// CCurveDlg �Ի���

class CCurveDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCurveDlg)

public:
	CCurveDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCurveDlg();

public:
	CCurveWnd*           m_pWndCurve;  //���ߴ�
	CBrush*            m_pBkgndBrush;  //����ˢ

// �Ի�������
	enum { IDD = IDD_CURVE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};