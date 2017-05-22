#pragma once
#include "MyImage.h"


// CARTDlg �Ի���

#define IDB_ARTDLG_CREATE_IMAGE                301
#define IDB_ARTDLG_PROJECT                     302
#define IDB_ARTDLG_SCANING                     303
#define IDB_ARTDLG_PAUSE                       304
#define IDB_ARTDLG_SINGLE_LINE                 305
#define IDB_ARTDLG_SINGLE_STEP                 306
#define IDB_ARTDLG_RANDOM                      307
#define IDB_ARTDLG_SOLVE                       308
#define IDB_ARTDLG_OK                          309
#define IDB_ARTDLG_CANCEL                      310
#define IDB_ARTDLG_PROJECT_SETTING             311
#define IDB_ARTDLG_SCAN_SETTING                312

#define ORIGIN                                 0
#define PROJECT                                1
#define BACK_PROJECT                           2

class CARTDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CARTDlg)

public:
	CARTDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CARTDlg();

// �Ի�������
	enum { IDD = IDD_ARTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public://������Ա�뺯��
	//����
	CMyCircle*                                 m_pBig;  //��Բ
	CMyCircle*                               m_pSmall;  //СԲ
	CMyImage*                              m_pMyImage;  //���ؽ�ͼ��
	CMyImage*                          m_pMyImageCopy;  //���ؽ�ͼ�񸱱�
	CMyRay*                                   m_pRays;  //����

	int                                      m_nWidth;  //��
	int                                     m_nHeight;  //��
	LONG                             m_nPixelQuantity;  //������

	//����
	TYPE                                  m_BigRadius;  //��Բ�뾶
	TYPE                                m_SmallRadius;  //СԲ�뾶
	CMyPoint                              m_BigCenter;  //��ԲԲ��
	CMyPoint                            m_SmallCenter;  //СԲԲ��
	TYPE                                   m_BigDecay;  //��Բ˥��ϵ��
	TYPE                                 m_SmallDecay;  //СԲ˥��ϵ��

	//����
	TYPE                                 m_AngleBegin;  //��ʼ��
	TYPE                                   m_AngleEnd;  //��ֹ��
	int                              m_nAngleQuantity;  //�ǶȲ�����
	TYPE                              m_DistanceBegin;  //��ʼ����
	TYPE                                m_DistanceEnd;  //��ֹ����
	int                           m_nDistanceQuantity;  //���������

	//��ť
	CMFCButton*                      m_pBnCreateImage;  //����ͼ��
	CMFCButton*                          m_pBnProject;  //ͶӰ
	CMFCButton*                          m_pBnScaning;  //ɨ��
	CMFCButton*                            m_pBnPause;  //��ͣ
	CMFCButton*                       m_pBnSingleLine;  //����
	CMFCButton*                       m_pBnSingleStep;  //��
	CMFCButton*                           m_pBnRandom;  //���
	CMFCButton*                         m_pBnARTSolve;  //����

	CMFCButton*                   m_pBnProjectSetting;  //ͶӰ����
	CMFCButton*                      m_pBnScanSetting;  //ɨ������
	CMFCButton*                               m_pBnOK;  //ȷ��
	CMFCButton*                           m_pBnCancel;  //ȡ��
	
	//λͼ
	CRect*                               m_pPaintRect;  //��ͼ��
	CBitmap*                             m_pBmpOrigin;  //ԭͼ
	CBitmap*                            m_pBmpProject;  //ͶӰͼ
	CBitmap*                        m_pBmpBackProject;  //��ͶӰ

	//����
	BOOL                              m_bImageCreated;  //�Ѿ�����Դͼ��
	BOOL                                 m_bProjected;  //���ͶӰ
	BOOL                                m_bHaveScaned;  //�Ѿ�ɨ��
	BOOL                                 m_bARTPaused;  //������ͣ
	BOOL                               m_bHaveRebuild;  //�Ѿ��ؽ�
	BOOL                               m_bIterateLByL;  //���ߵ���
	
	int                               m_nCurImageFlag;  //��ǰ��ʾ��ͼ
	int                                m_nIterateLine;  //��ǰ����ֱ��
	int                                m_nIterateStep;  //��ǰ��������
	int                                    m_nLineGap;  //ÿ�ߵ������
	int                                    m_nStepGap;  //ÿ���������

	virtual BOOL OnInitDialog();
	void Reshape(int nItemID, double Space, int nNumber, int Col = 1);  //�ػ�ؼ�
	BOOL CreateOriginImage();                                           //����һ��ͼ��
	BOOL CreateTargetImage(CBitmap* Target);                            //�ؽ�������ͼ��

	void InformParent();                                                //֪ͨ������

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();

	afx_msg void OnIdbCreateImage();
	afx_msg void OnIdbProject();
	afx_msg void OnIdbScaning();
	afx_msg void OnIdbPause();
	afx_msg void OnIdbSingleLine();
	afx_msg void OnIdbSingleStep();
	afx_msg void OnIdbRandom();
	afx_msg void OnIdbSolve();
	afx_msg void OnIdbOK();
	afx_msg void OnIdbCancel();
	afx_msg void OnIdbProjectSetting();
	afx_msg void OnIdbScanSetting();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

//����
void Create32Bitmap(CBitmap* pBmp, TYPE* pType, int width, int height);
void CreateBitmapUsingMatrix(CBitmap* pBmp, TYPE* pType, int width, int height);
void CreateBitmapUsingRays(CBitmap*pBmp, CMyRay* pRays, TYPE MAX, int DistanceQuantity, int AngleQuantity);