// ARTDlgProjectSetting.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Noise.h"
#include "ARTDlgProjectSetting.h"
#include "afxdialogex.h"


// CARTDlgProjectSetting �Ի���

IMPLEMENT_DYNAMIC(CARTDlgProjectSetting, CDialogEx)

CARTDlgProjectSetting::CARTDlgProjectSetting(CWnd* pParent /*=NULL*/) : CDialogEx(CARTDlgProjectSetting::IDD, pParent)
{
	m_nWidth = 256;                          //ͼ����
	m_nHeight = 256;                         //ͼ��߶�

	m_BigRadius = 128;                       //��Բ�뾶
	m_BigDecay = 0.3;                        //��Բ˥��ϵ��
	m_BigCenterX = 128;
	m_BigCenterY = 128;
	m_SmallRadius = 64;                      //СԲ�뾶
	m_SmallDecay = 0.7;                      //СԲ˥��ϵ��
	m_SmallCenterX = 192;
	m_SmallCenterY = 128;
}

CARTDlgProjectSetting::~CARTDlgProjectSetting()
{
}

void CARTDlgProjectSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ARTDLG_WIDTH, m_CtrlWidth);
	DDX_Control(pDX, IDC_ARTDLG_HEIGHT, m_CtrlHeight);
	DDX_Control(pDX, IDC_ARTDLG_BIG_RADIUS, m_CtrlBigRadius);
	DDX_Control(pDX, IDC_ARTDLG_BIG_DECAY, m_CtrlBigDecay);
	DDX_Control(pDX, IDC_ARTDLG_BIG_CENTERX, m_CtrlBigCenterX);
	DDX_Control(pDX, IDC_ARTDLG_BIG_CENTERY, m_CtrlBigCenterY);
	DDX_Control(pDX, IDC_ARTDLG_SMALL_RADIUS, m_CtrlSmallRadius);
	DDX_Control(pDX, IDC_ARTDLG_SMALL_DECAY, m_CtrlSmallDecay);
	DDX_Control(pDX, IDC_ARTDLG_SMALL_CENTERX, m_CtrlSmallCenterX);
	DDX_Control(pDX, IDC_ARTDLG_SMALL_CENTERY, m_CtrlSmallCenterY);
	DDX_Text(pDX, IDC_ARTDLG_WIDTH, m_nWidth);
	DDV_MinMaxInt(pDX, m_nWidth, 8, 1024);
	DDX_Text(pDX, IDC_ARTDLG_HEIGHT, m_nHeight);
	DDV_MinMaxInt(pDX, m_nHeight, 8, 1024);
	DDX_Text(pDX, IDC_ARTDLG_BIG_RADIUS, m_BigRadius);
	DDX_Text(pDX, IDC_ARTDLG_BIG_DECAY, m_BigDecay);
	DDX_Text(pDX, IDC_ARTDLG_BIG_CENTERX, m_BigCenterX);
	DDX_Text(pDX, IDC_ARTDLG_BIG_CENTERY, m_BigCenterY);
	DDX_Text(pDX, IDC_ARTDLG_SMALL_RADIUS, m_SmallRadius);
	DDX_Text(pDX, IDC_ARTDLG_SMALL_DECAY, m_SmallDecay);
	DDX_Text(pDX, IDC_ARTDLG_SMALL_CENTERX, m_SmallCenterX);
	DDX_Text(pDX, IDC_ARTDLG_SMALL_CENTERY, m_SmallCenterY);
}


BEGIN_MESSAGE_MAP(CARTDlgProjectSetting, CDialogEx)
END_MESSAGE_MAP()


// CARTDlgProjectSetting ��Ϣ�������
