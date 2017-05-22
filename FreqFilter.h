#pragma once

// �Ի���Ƶ�����˲�

// CFreqFilter �Ի���

#include "FFTDlg.h"
#include "afxwin.h"

class CFreqFilter : public CDialogEx
{
	DECLARE_DYNAMIC(CFreqFilter)

public:
	CFreqFilter(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFreqFilter();

// �Ի�������
	enum { IDD = IDD_FREQFILTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public://������Ա�뺯��
	int                            m_nMethod;       //�˲�����
	float                           m_fPower;       //��
	CEdit                        m_EditPower;       //Edit�ؼ�
	CFFTDlg*                       m_pFFTDlg;       //��һ��Ի���ָ��
	CBitmap*                       m_pBitmap;       //��һ��Ի���λͼ
	CFFTDlg* GetDocument();                         //��ȡ��һ��Ի���ָ��
	CBitmap* GetBitmap();                           //��ȡ��һ��Ի���λͼ
	virtual BOOL OnInitDialog();                    //�Ի����ʼ��
	afx_msg void OnFilterButterworthl();
	afx_msg void OnFilterButterworthh();
	afx_msg void OnFilterRetrorse();
	afx_msg void OnFilterWiener();
	afx_msg void OnFilterPse();
	afx_msg void OnFilterMinpower();
};