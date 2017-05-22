
// NoiseDlg.h : ͷ�ļ�
//

#pragma once

#include "NoiseWnd.h"
#include "resource.h"

// CNoiseDlg �Ի���
class CNoiseDlg : public CDialogEx
{
// ����
public:
	CNoiseDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CNoiseDlg();

// �Ի�������
	enum { IDD = IDD_NOISE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:// ������Ա�뺯��
	CNoiseWnd*          m_pNoiseWnd;                    //�Ӵ��������

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
