// CurveDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Noise.h"
#include "CurveDlg.h"
#include "afxdialogex.h"


// CCurveDlg �Ի���

IMPLEMENT_DYNAMIC(CCurveDlg, CDialogEx)

CCurveDlg::CCurveDlg(CWnd* pParent /*=NULL*/) : CDialogEx(CCurveDlg::IDD, pParent)
{
	m_pWndCurve = NULL;
	m_pBkgndBrush = NULL;
}

CCurveDlg::~CCurveDlg()
{
	if(m_pWndCurve != NULL)       delete m_pWndCurve;
	if (m_pBkgndBrush != NULL)    delete m_pBkgndBrush;
}

void CCurveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCurveDlg, CDialogEx)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CCurveDlg ��Ϣ�������


int CCurveDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	//���ߴ�
	CRect rect;
	GetClientRect(&rect);
	m_pWndCurve = new CCurveWnd;
	m_pWndCurve->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rect, this, IDW_CURVE_WINDOW);

	//����ˢ
	m_pBkgndBrush = new CBrush;
	m_pBkgndBrush->CreateSolidBrush(RGB(230, 240, 250));

	return 0;
}


void CCurveDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	CWnd *hWnd = GetDlgItem(IDW_CURVE_WINDOW);
	if (hWnd != NULL)
	{
		hWnd->MoveWindow(0, 0, cx, cy);
	}
}


BOOL CCurveDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//WindowSize
	int nDlgWidth = 365+18;
	int nDlgHeight = 375+36;
	::SetWindowPos(this->m_hWnd, HWND_TOP, 100, 100, nDlgWidth, nDlgHeight, SWP_SHOWWINDOW);
	//CenterWindow();

	return TRUE;
}

//����EraseBackground������������ʱ��ʲôҲ������2014.4.24
//��ͼ����Ի���Ĵ�С���ı�ʱ�����������⡣
//true��ʾ�Ѵ�����ˢ�£�false��ʾ��Ҫ��OnPaint�ﴦ��
BOOL CCurveDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CRect rect;
	GetClientRect(&rect);
	pDC->FillRect(&rect, m_pBkgndBrush);
	return TRUE;
	return CDialogEx::OnEraseBkgnd(pDC);
}

void CCurveDlg::OnPaint()
{
	CPaintDC dc(this);
}