#pragma once


// CNoiseWnd

class CCurveDlg;

#define GAUSS_METHOD     "BoxMuller"

class CNoiseWnd : public CWnd
{
	DECLARE_DYNAMIC(CNoiseWnd)

public:
	CNoiseWnd();
	virtual ~CNoiseWnd();

public:// ������Ա�뺯��
	CBitmap*                    m_pBitmap;  //Դλͼ
	CBitmap*                   m_pBmpShow;  //��ʾλͼ
	CRect*                   m_pPaintRect;  //��ͼ��
	BOOL                 m_bHaveLoadImage;  //�Ѽ���ͼ��

	//����
	CFont*                     m_pAppFont;  //��������
	CString                    m_FontName;  //��������
	int                       m_nFontSize;  //�����С

	//��ť
	CMFCButton*            m_pBnOpenImage;  //��ͼ��
	CMFCButton*            m_pBnSaveImage;  //����ͼ��
	CMFCButton*                m_pBnGauss;  //��˹����
	CMFCButton*               m_pBnPepper;  //��������
	CMFCButton*         m_pBnMedianFilter;  //��ֵ�˲���
	CMFCButton*       m_pBnSAMedianFilter;  //����Ӧ��ֵ�˲���
	CMFCButton*                m_pBnCurve;  //����
	CMFCButton*              m_pBnFourier;  //FFT
	CMFCButton*              m_pBnARTDemo;  //ģ��CTͼ���ؽ�
	CMFCButton*             m_pBnSettings;  //����

	//ͼ�������Ϣ
	CString                  m_FolderPath;  //ͼ��·��
	CString                    m_FileName;  //ͼ������
	BITMAP                      m_bmpInfo;  //ͼ����Ϣ
	int                      m_bytesPixel;  //ͨ����
	double                  m_AspectRatio;  //��߱�
	
	//m_pBitmap
	long                     m_nRowlenSrc;  //λ��
	long                        m_nlenSrc;  //��λ��
	int                       m_nWidthSrc;  //ͼ����
	int                      m_nHeightSrc;  //ͼ��߶�
	
	//m_pBmpShow
	long                     m_nRowlenCur;  //λ��
	long                        m_nlenCur;  //��λ��
	int                       m_nWidthCur;  //ͼ����
	int                      m_nHeightCur;  //ͼ��߶�

	//Gauss Noise
	double                         m_Mean;  //��ֵ
	double                          m_Std;  //����

	//Pepper & Salt Noise
	double                       m_Pepper;  //��������
	double                         m_Salt;  //��������

	//�˲���
	int                     m_nFilterSize;  //�˲�����С
	int                  m_nMaxFilterSize;  //�˲���������

	//��ģʽ�Ի���
	CCurveDlg*                m_pCurveDlg;  //���߶Ի���

	void Reshape(int nItemID, double Space, int nNumber);             //�ػ�ؼ�
	BOOL GaussDistribution();                                          //��Ӹ�˹����
	BOOL PepperDistribution();                                         //��ӽ�������
	BOOL MedianFilter();                                               //��ֵ�˲���
	BOOL SelfAdaptionMedianFilter();                                   //����Ӧ��ֵ�˲�
	int FilterProcessA(BYTE* dataSrc, int curChannel, int y, int x, int FilterSize);
	                                                                   //����A
	int FilterProcessB(int Zmin, int Zmax, int Zmed, int Zxy);         //����B
	void EnableButtons(BOOL ButtonIsEnable = TRUE);                    //��ť�Ƿ���Ч


	CImage* CreateCImageObj();                                         //����CImage�����
	void ZoomBitmap(int widthnew, int heightnew);                      //ͼ��Ŵ�
	void Create32Bitmap(BYTE* dataSrc, int widthnew, int heightnew);   //����32λλͼ
	void Gray2Rgb(BYTE* dataSrc, int widthnew, int heightnew);         //�Ҷ�ͼתRGB
	void Synchronous();                                                //����λͼͬ��
	void FitScreen();                                                  //ʹλͼ��Ӧ��Ļ

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnIdbOpenImage();
	afx_msg void OnIdbSaveImage();
	afx_msg void OnIdbGaussDistribution();
	afx_msg void OnIdbPepperDistribution();
	afx_msg void OnIdbSelfAdaptionMedianFilter();
	afx_msg void OnIddCurveDlg();
	afx_msg void OnIdbMedianFilter();
	afx_msg void OnIdbSettings();
	afx_msg void OnIdbFourier();
	afx_msg void OnIdbArtdemo();
};

double GaussNoise(double nMean = 0, double nStd = 1, char* Method = "BoxMuller");                          //������˹����
int PepperNoise(double Probability = 0.05, char* PepperOrSalt = "Pepper");                                 //������������