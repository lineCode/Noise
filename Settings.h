#pragma once
#include "afxwin.h"


// CSettings �Ի���

class CSettings : public CDialogEx
{
	DECLARE_DYNAMIC(CSettings)

public:
	CSettings(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSettings();

public://������Ա�뺯��

// �Ի�������
	enum { IDD = IDD_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEdit                    m_CtrlGaussMean;
	CEdit                     m_CtrlGaussStd;
	CEdit                       m_CtrlPepper;
	CEdit                         m_CtrlSalt;
	CEdit                   m_CtrlFilterSize;
	CEdit                m_CtrlMaxFilterSize;
	double                            m_Mean;
	double                             m_Std;
	double                          m_Pepper;
	double                            m_Salt;
	int                        m_nFilterSize;
	int                     m_nMaxFilterSize;
};
