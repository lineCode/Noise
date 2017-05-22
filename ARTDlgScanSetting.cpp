// ARTDlgScanSetting.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Noise.h"
#include "ARTDlgScanSetting.h"
#include "afxdialogex.h"


// CARTDlgScanSetting �Ի���

IMPLEMENT_DYNAMIC(CARTDlgScanSetting, CDialogEx)

CARTDlgScanSetting::CARTDlgScanSetting(CWnd* pParent /*=NULL*/)	: CDialogEx(CARTDlgScanSetting::IDD, pParent)
{
	m_AngleBegin = PRECISION;                //��ʼ��
	m_AngleEnd = 2*PI-PRECISION;             //��ֹ��
	m_nAngleQuantity = 180;                  //�ǶȲ�����
	m_DistanceBegin = PRECISION;             //��ʼ����
	m_DistanceEnd = 768;                     //��ֹ����
	m_nDistanceQuantity = 180;               //���������
}

CARTDlgScanSetting::~CARTDlgScanSetting()
{
}

void CARTDlgScanSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ARTDLG_ANGLE_BEGIN, m_AngleBegin);
	DDX_Text(pDX, IDC_ARTDLG_ANGLE_END, m_AngleEnd);
	DDX_Text(pDX, IDC_ARTDLG_ANGLE_QUANTITY, m_nAngleQuantity);
	DDV_MinMaxInt(pDX, m_nAngleQuantity, 8, 1024);
	DDX_Text(pDX, IDC_ARTDLG_DISTANCE_BEGIN, m_DistanceBegin);
	DDX_Text(pDX, IDC_ARTDLG_DISTANCE_END, m_DistanceEnd);
	DDX_Text(pDX, IDC_ARTDLG_DISTANCE_QUANTITY, m_nDistanceQuantity);
	DDV_MinMaxInt(pDX, m_nDistanceQuantity, 8, 1024);
}


BEGIN_MESSAGE_MAP(CARTDlgScanSetting, CDialogEx)
END_MESSAGE_MAP()


// CARTDlgScanSetting ��Ϣ�������
