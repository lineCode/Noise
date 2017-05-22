
#pragma once


// CFFT �Ի���

#include "NoiseWnd.h"
#include <cmath>
#include <complex>
using namespace std;


#define HDIB                         HANDLE 
#define PI                           3.141592653
#define WIDTHBYTES(bits)             (((bits) + 31) / 32 * 4)

//��ť��Դ
#define IDB_FFTDLG_OK                201
#define IDB_FFTDLG_CANCEL            202
#define IDB_FFTDLG_FFT               203
#define IDB_FFTDLG_FILTER            204
#define IDB_FFTDLG_INVFFT            205

#define FILTER_BUTTERWORTHL 1    // - ������˹��ͨ�˲���
#define FILTER_BUTTERWORTHH 2    // - ������˹��ͨ�˲���
#define FILTER_RESTRORSE    3    // - ������˹��ͨ�˲��������˲�
#define FILTER_WIENER       4    // - ��Լ���ָ���ά���˲�
#define FILTER_PSE          5    // - ��Լ���ָ��Ĺ����׾����˲�
#define FILTER_MINPOWER     6    // - ��Լ���ָ�����Сƽ���˲�

class CFFTDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFFTDlg)

public:
	CFFTDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFFTDlg();

// �Ի�������
	enum { IDD = IDD_FFT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

public://������Ա�뺯��
	CNoiseWnd*                        m_pNoiseWnd;  //ָ��
	CBitmap*                            m_pBitmap;  //ָ��
	CBitmap*                              m_pFreq;  //FFTƵ��
	CRect*                           m_pPaintRect;  //��ͼ��
	BYTE*                               m_dataSrc;  //����Դ
	BYTE*                             m_data24Bit;  //����alphaͨ��ʣ�µ�����
	int                              m_bytesPixel;  //ͨ��
	int                             m_bmBitsPixel;  //λ��

	//��ǰͼ����Ϣ
	int                               m_nWidthCur;  //��
	int                              m_nHeightCur;  //��
	LONG                             m_nRowlenCur;  //λ��
	LONG                                m_nlenCur;  //����

	//Դͼ����Ϣ
	int                               m_nWidthSrc;  //��
	int                              m_nHeightSrc;  //��
	LONG                             m_nRowlenSrc;  //λ��
	LONG                                m_nlenSrc;  //����

	//24λͼ����Ϣ
	int                             m_nWidth24Bit;  //��
	int                            m_nHeight24Bit;  //��
	LONG                           m_nRowlen24Bit;  //λ��
	LONG                              m_nlen24Bit;  //����

	//��ť
	CMFCButton*                           m_pBnOK;  //ȷ��
	CMFCButton*                       m_pBnCancel;  //ȡ��
	CMFCButton*                          m_pBnFFT;  //����Ҷ�任
	CMFCButton*                       m_pBnFilter;  //�˲�

	//�˲���������
	int                                 m_nMethod;
	float                                m_fPower;
	
	CNoiseWnd* GetDocument();                       //��ȡ���ĵ�
	CBitmap* GetBitmap();                           //��ȡλͼָ��
	void GetBitmapInfo();                           //��ȡԴλͼ��Ϣ
	void GetBitmapData();                           //��ȡԴλͼ����
	void Get24BitData();                            //��ȡԴλͼ24λ����
	void Create32Bitmap(BYTE* dataSrc, int channel, int widthnew, int heightnew);
	                                                //����32λͼ��
	void ZoomBitmap(int widthnew, int heightnew);   //����
	virtual BOOL OnInitDialog();                    //��ʼ��

	void Reshape(int nItemID, double Space, int nNumber);

	/*** �任���� ***/
	void DIBIDWT(HDIB hDIB);
	void IDWT(LPBYTE lpData, int nX, int nY, int nXSize, int nYSize);
	void DIBDWT(HDIB hDIB);
	int a(int nX, int nXSize);
	void DWT(LPBYTE lpData, int nX, int nY, int nXSize, int nYSize);
	BOOL IFreqWALSH(double* f, double* F, LONG lWidth, LONG lHeight);
	BOOL FreqWALSH(double* f, double* F, LONG lWidth, LONG lHeight);
	BOOL DIBWalsh(HDIB hDIB);
	void IWALSH(double* F, double* f, int r);
	BOOL IFreqDCT(double* f, double* F, LONG lWidth, LONG lHeight);
	void WALSH(double* f, double* F, int r);
	BOOL FreqDCT(double* f, double* F, LONG lWidth, LONG lHeight);
	BOOL DIBDCT(HDIB hDIB);
	void IDCT(double* F,double* f, int r);
	void DCT(double* f, double* F, int r);
	BOOL IFourier(LPBYTE TD, LONG lWidth, LONG lHeight, complex<double> * FD);
	BOOL Fourier(complex<double> * TD, LONG lWidth, LONG lHeight, complex<double> * FD);
	BOOL DIBFourier(HDIB hDIB);
	void IFFT(complex<double> * FD, complex<double> * TD, int r);
	void FFT(complex<double> * TD, complex<double> * FD, int r);

	/*** Ƶ���˲� ***/
	void MinPower(HDIB hDIB, float fD);
	void PSE_Filter(HDIB hDIB, float fD);
	void WienerFilter(HDIB hDIB, float fD);
	void RetrorseFilter(HDIB hDIB,float fD);
	void MutualFilter(HDIB hDIB, CRect* pRect, int nSum);
	void ButterworthH(HDIB hDIB, float fD);
	void ButterworthL(HDIB hDIB, float fD);

	//��Ϣӳ��
	afx_msg void OnIdbFftdlgOk();
	afx_msg void OnIdbFftdlgCancel();
	afx_msg void OnIdbFftdlgFft();
	afx_msg void OnIdbFftdlgFilter();
};