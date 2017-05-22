#pragma once

// CCurveWnd

#include "NoiseDlg.h"
#include "NoiseWnd.h"
#include "resource.h"

#include <vector>
using namespace std;

#define LINE_PATTERN_SPLINE 0
#define LINE_PATTERN_LINEAR 1
#define CHANNEL_RGB         0
#define CHANNEL_RED         1
#define CHANNEL_GREEN       2
#define CHANNEL_BLUE        3


#define IDB_GROUP_BOX                  1000               //Group Box
#define IDC_COMBOBOX_RGB               1001               //ͨ��:RGB
#define IDB_CHOOSE_CURVE               1002               //ѡ������
#define IDB_CHOOSE_LINE                1003               //ѡ��ֱ��
#define IDB_CURVEWND_RESET             1004               //����
#define IDB_CURVEWND_INVERSE           1005               //��ת
#define IDB_IMPORT_CURVE               1006               //��������
#define IDB_EXPORT_CURVE               1007               //��������
#define IDE_CURVEWND_XPOSITION         1008               //X����
#define IDE_CURVEWND_YPOSITION         1009               //Y����
#define IDS_CURVEWND_XLABEL            1010               //X��ǩ
#define IDS_CURVEWND_YLABEL            1011               //Y��ǩ
#define IDB_CURVE_OK                   1012               //ȷ��
#define IDB_CURVE_CANCEL               1013               //ȡ��

struct peg
{
	CPoint*                           m_pt;               //�����
	double                       m_dRatioX;               //���Rectλ��X
	double                       m_dRatioY;               //���Rextλ��Y
};
typedef CList<peg, peg>               PEGS;               //���peg�����ݽṹ
typedef vector<int>              TRANSFORM;               //��ŻҶȱ任������

class CCurveWnd : public CWnd
{
	DECLARE_DYNAMIC(CCurveWnd)

public:
	CCurveWnd();
	virtual ~CCurveWnd();

	CNoiseWnd* GetDocument();                             /*��ȡCNoiseWnd*/

	CBitmap* GetBitmap();                                 //ȡ�õ�ǰͼ��
	BOOL GetHistogram();                                  //ͨ�����㣬ȡ��ֱ��ͼ
	BOOL PegIsExist(CPoint& point);                       //��peg�Ƿ����
	POSITION AddNewPeg(CPoint& point);                    //�����µ�peg
	BOOL RePlacePeg(CPoint& point);                       //�ı�ĳ��peg
	BOOL UserWillDrag(CPoint& point);                     //�������û�������һ��peg
	void SplineFunc();                                    //������ֵ����
	void PieceWiseFunc();                                 //�ֶ����Ժ���
	BOOL CursorIsInPaintRect(CPoint& point);              //����ڻ�ͼ����֮��
	CPoint TransPositionToDialog(CPoint& point);          //ת�����Ի���Ĭ�ϵ�����ϵ(���Ͻ������)
	CPoint TransPositionToRectangle(CPoint& point);       //ת������Rect���½�Ϊԭ�������ϵ
	BOOL GrayTransform();                                 //�����������Ҷȱ任
	void Threshold();                                     //�Գ���[0,255]�ĻҶȽ��д���
	int* PreTransform();                                  //��m_pnTransformԤ����
	void InverseTransform();                              //ͼ��ת
	void SaveCurrentState();                              //���浱ǰ״̬
	void ImshowImmediately();                             //����ˢ��ͼ��
	void ShowGrayTransform();                             //��ֵ����ʾ�Ҷȱ任����

	void ReShapeButtons(int nItemID, int nButtonPosX1, int nButtonPosX2, double Space, int Number);
	void ReShapePegs();
	void ReshapeTransform();

public:
	//��ť
	CComboBox*                         m_pComboBoxRGB;    //RGBͨ��:4
	CButton*                            m_pBnGroupBox;    //Group Box
	CMFCButton*                      m_pBnChooseCurve;    //����ģʽ
	CMFCButton*                       m_pBnChooseLine;    //ֱ��ģʽ
	CMFCButton*                          m_pBnInverse;    //��ת
	CMFCButton*                            m_pBnReset;    //����
	CMFCButton*                      m_pBnImportCurve;    //��������
	CMFCButton*                      m_pBnExportCurve;    //��������
	CMFCButton*                               m_pBnOK;    //ȷ��
	CMFCButton*                           m_pBnCancel;    //ȡ��
	CEdit*                               m_pXPosition;    //X����
	CEdit*                               m_pYPosition;    //Y����
	CStatic*                                m_pXlabel;    //X��ǩ
	CStatic*                                m_pYlabel;    //Y��ǩ

	//����
	double*                           m_pfHistogram[4];    //ֱ��ͼ:4*256
	TRANSFORM                        m_V4Transform[4];    //�Ҷȱ任:4*m_nWidth
	PEGS                               m_pPegsList[4];    //ͼ��ѡ�ĵ�:4*m_nWidth
	CRect*                             m_pOnPaintRect;    //��ͼ����
	BYTE*                                   m_dataSrc;    //ͼ��Դ����
	BYTE*                                   m_curData;    //��ǰͼ������

	int                            m_nChannelSelected;    //��ǰͨ��
	BOOL                              m_bUserWillDrag;    //����peg
	int                                  m_nRectWidth;    //��ͼ�����
	int                                 m_nRectHeight;    //��ͼ���߶�
	double                               m_fWidthRatio;    //������ű�
	double                              m_fHeightRatio;    //�߶����ű�
	POSITION                          m_posCurrentPeg;    //��ǰק����peg
	int                                m_nLinePattern;    //����
	BOOL                                   m_bInverse;    //��ת

	//ͼ����Ϣ
	long                                m_nImageWidth;    //ͼ����
	long                               m_nImageHeight;    //ͼ��߶�
	long                             m_nPixelQuantity;    //���ظ���
	long                                m_nbytesWidth;    //ͼ��λ��
	int                                    m_nChannel;    //ͼ��ͨ��
	DWORD                                  m_nlenData;    //ͼ�񳤶�

	//OnSize
	CRect*                                 m_pWndSize;    //��ǰWnd��С

	//����
	CFont*                                 m_pAppFont;    //����
	int                                   m_nFontSize;    //�����С
	CString                                m_FontName;    //������

	//*** Bitmap ***//
	CBitmap*                                m_pBitmap;    //CBitmapָ��

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnChooseCurve();
	afx_msg void OnChooseLine();
	afx_msg void OnComboboxRgb();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnIdbCurvewndReset();
	afx_msg void OnIdbCurvewndInverse();
	afx_msg void OnIdbImportCurve();
	afx_msg void OnIdbExportCurve();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnIdbCurveOk();
	afx_msg void OnIdbCurveCancel();
};

//
// *** ����������ֵ���� ***
//

typedef vector<double> VECTOR;

VECTOR Spline(VECTOR& Node, VECTOR& Value, VECTOR& x0, double c1, double c2, int CASE = 2);
void Chase(VECTOR& a, VECTOR& b, VECTOR& c, VECTOR& f, VECTOR& x, int n);
COLORREF ColorSetting(int Channel, int Strength, int WhereUse = 0);