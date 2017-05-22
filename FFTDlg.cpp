// FFTDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Noise.h"
#include "FFTDlg.h"
#include "afxdialogex.h"

#include "FreqFilter.h"

// CFFT �Ի���

IMPLEMENT_DYNAMIC(CFFTDlg, CDialogEx)

CFFTDlg::CFFTDlg(CWnd* pParent /*=NULL*/)	: CDialogEx(CFFTDlg::IDD, pParent)
{
	m_pFreq = NULL;
	m_pPaintRect = NULL;

	m_pBnOK = NULL;
	m_pBnCancel = NULL;
	m_pBnFFT = NULL;
	m_pBnFilter = NULL;

	m_nMethod = 0;
	m_fPower = 200;
}

CFFTDlg::~CFFTDlg()
{
	if (m_pFreq != NULL)		{  delete m_pFreq;}
	if (m_pPaintRect != NULL)   {  delete m_pPaintRect;}
	if (m_dataSrc != NULL)      {  delete [] m_dataSrc;}
	if (m_data24Bit != NULL)    {  delete [] m_data24Bit;}

	if (m_pBnOK != NULL)        {  delete m_pBnOK;}
	if (m_pBnCancel != NULL)    {  delete m_pBnCancel;}
	if (m_pBnFFT != NULL)       {  delete m_pBnFFT;}
	if (m_pBnFilter != NULL)    {  delete m_pBnFilter;}
}

void CFFTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFFTDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_COMMAND(IDB_FFTDLG_OK, &CFFTDlg::OnIdbFftdlgOk)
	ON_COMMAND(IDB_FFTDLG_CANCEL, &CFFTDlg::OnIdbFftdlgCancel)
	ON_COMMAND(IDB_FFTDLG_FFT, &CFFTDlg::OnIdbFftdlgFft)
	ON_COMMAND(IDB_FFTDLG_FILTER, &CFFTDlg::OnIdbFftdlgFilter)
END_MESSAGE_MAP()


// CFFT ��Ϣ�������

CNoiseWnd* CFFTDlg::GetDocument()
{
	CWnd* pWnd = GetParent();
	CNoiseWnd *pNoiseWnd = (CNoiseWnd*)pWnd->GetDlgItem(ID_NOISE_WND);
	return pNoiseWnd;
}

CBitmap* CFFTDlg::GetBitmap()
{
	return GetDocument()->m_pBitmap;
}

void CFFTDlg::GetBitmapInfo()
{
	//��ȡλͼ��Ϣ
	BITMAP bmp = m_pNoiseWnd->m_bmpInfo;
	m_bytesPixel = m_pNoiseWnd->m_bytesPixel;
	m_bmBitsPixel = bmp.bmBitsPixel;

	m_nWidth24Bit  = m_nWidthCur  = m_nWidthSrc  = m_pNoiseWnd->m_nWidthSrc;
	m_nHeight24Bit = m_nHeightCur = m_nHeightSrc = m_pNoiseWnd->m_nHeightSrc;
	m_nRowlenCur   = m_nRowlenSrc = m_pNoiseWnd->m_nRowlenSrc;
	m_nlenCur      = m_nlenSrc    = m_pNoiseWnd->m_nlenSrc;

	m_nRowlen24Bit = 3*m_nWidthSrc;
	m_nlen24Bit    = m_nRowlen24Bit*m_nHeightSrc;
}

void CFFTDlg::GetBitmapData()
{
	LONG lenData = m_pNoiseWnd->m_nlenSrc;
	m_dataSrc = new BYTE[lenData];
	m_pBitmap->GetBitmapBits(lenData, m_dataSrc);
}

//��ȡ�޳�alphaͨ��֮���24λ����
void CFFTDlg::Get24BitData()
{
	//����ռ����ڱ���24λλͼ����
	DWORD len24 = 3 * m_nWidthSrc*m_nHeightSrc;
	m_data24Bit = new BYTE[len24];
	//����λͼ����
	BYTE* pSrcData = m_dataSrc;
	BYTE* pDesData = m_data24Bit;
	for(int r = 0; r < m_nHeightSrc; r++)
	{
		pSrcData = m_dataSrc + r * m_nRowlenSrc;
		for(int c = 0; c <m_nWidthSrc; c++)
		{
			memcpy(pDesData, pSrcData, 3);
			pSrcData += m_bytesPixel;
			pDesData += 3;
		}
	}
}

void CFFTDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CRect WndSize;
	GetClientRect(&WndSize);
	*m_pPaintRect = CRect(WndSize.left+90, WndSize.top+10, WndSize.right-10, WndSize.bottom-10);

	int nWindowHeight = WndSize.Height();
	double Space = nWindowHeight/5.0;
	Reshape(IDB_FFTDLG_OK, Space, 1);
	Reshape(IDB_FFTDLG_CANCEL, Space, 2);
	Reshape(IDB_FFTDLG_FFT, Space, 3);
	Reshape(IDB_FFTDLG_FILTER, Space, 4);
}

void CFFTDlg::Reshape(int nItemID, double Space, int nNumber)
{
	CWnd *hWnd = GetDlgItem(nItemID);
	if (hWnd != NULL)
		hWnd->MoveWindow(25, int(nNumber*Space-12), 50, 25);
}

void CFFTDlg::OnPaint()
{
	CPaintDC dc(this);
	CDC MemDC;
	CRect WndSize;
	GetClientRect(&WndSize);
	MemDC.CreateCompatibleDC(&dc);
	MemDC.SelectObject(m_pFreq);
	dc.BitBlt(m_pPaintRect->left, m_pPaintRect->top, m_nWidthCur, m_nHeightCur, &MemDC, 0, 0, SRCCOPY);
	MemDC.DeleteDC();
}


int CFFTDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pNoiseWnd = GetDocument();
	m_pBitmap = GetBitmap();

	//��ͼ��
	CRect WndSize;
	GetClientRect(&WndSize);
	m_pPaintRect = new CRect(WndSize.left+90, WndSize.top+10, WndSize.right-10, WndSize.bottom-10);

	//��ȡλͼ����
	GetBitmapData();
	//��ȡλͼ��Ϣ
	GetBitmapInfo();
	//��ȡ24λλͼ����
	Get24BitData();

	//�Ȱ�ԭͼ������m_pFreq
	m_pFreq = new CBitmap;
	Create32Bitmap(m_dataSrc, m_bytesPixel, m_nWidthCur, m_nHeightCur);

	int nScreenWidth = int(0.8*GetSystemMetrics(SM_CXSCREEN));
	int nScreenHeight = int(0.8*GetSystemMetrics(SM_CYSCREEN));
	int x = nScreenWidth-m_nWidthCur-100;
	int y = nScreenHeight-m_nHeightCur-20;
	float Rate = 1.0f*m_nWidthCur/m_nHeightCur;
	if(x<0 && y<0)
	{
		ZoomBitmap(nScreenWidth, nScreenHeight);
	}
	else if(x>=0 && y<0)
	{
		ZoomBitmap(nScreenHeight*Rate, nScreenHeight);
	}
	else if(x<0 && y>=0)
	{
		ZoomBitmap(nScreenWidth, nScreenWidth/Rate);
	}

	int nWindowHeight = WndSize.Height();
	double Space = nWindowHeight/5.0;

	//ȷ��
	m_pBnOK = new CMFCButton;
	m_pBnOK->Create(_T("ȷ��"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(25, int(Space-12), 75, int(Space+25)), this, IDB_FFTDLG_OK);
	m_pBnOK->SetTooltip(_T("ȷ�����в���"));
	m_pBnOK->SetMouseCursorHand();

	m_pBnCancel = new CMFCButton;
	m_pBnCancel->Create(_T("ȡ��"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(25, int(2*Space-12), 75, int(2*Space+25)), this, IDB_FFTDLG_CANCEL);
	m_pBnCancel->SetTooltip(_T("�������в���"));
	m_pBnCancel->SetMouseCursorHand();

	m_pBnFFT = new CMFCButton;
	m_pBnFFT->Create(_T("Ƶ��"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(25, int(3*Space-12), 75, int(3*Space+25)), this, IDB_FFTDLG_FFT);
	m_pBnFFT->SetTooltip(_T("���ٸ���Ҷ�任"));
	m_pBnFFT->SetMouseCursorHand();

	m_pBnFilter = new CMFCButton;
	m_pBnFilter->Create(_T("�˲�"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(25, int(4*Space-12), 75, int(4*Space+25)), this, IDB_FFTDLG_FILTER);
	m_pBnFilter->SetTooltip(_T("Ƶ�����˲�"));
	m_pBnFilter->SetMouseCursorHand();

	return 0;
}

//����32λλͼ:dataSrc - ����Դ,widthnew,height - �µĿ����
void CFFTDlg::Create32Bitmap(BYTE* dataSrc, int channel, int widthnew, int heightnew)
{
	//����ռ����ڱ���32λλͼ����
	DWORD lennew = 4 * widthnew * heightnew;
	LONG rowlen = widthnew*channel;
	BYTE* datanew = new BYTE[lennew];
	//����λͼ����
	BYTE* pSrcData = dataSrc;
	BYTE* pDesData = datanew;
	for(int r = 0; r < heightnew; r++)
	{
		pSrcData = dataSrc + r * rowlen;
		for(int c = 0; c <widthnew; c++)
		{
			memcpy(pDesData, pSrcData, channel);
			pSrcData += channel;
			pDesData += 4;
		}
	}
	//����32λλͼ
	m_pFreq->DeleteObject();
	m_pFreq->CreateBitmap(widthnew, heightnew, 1, 32, datanew);

	delete[] datanew;
}

void CFFTDlg::ZoomBitmap(int widthnew, int heightnew)
{
	//��ȡͼ��ߴ�����
	long width = m_nWidthCur;
	long height = m_nHeightCur;
	long byteswidth = m_nRowlenCur;

	//�������ź��ͼ��ߴ�����
	m_nWidthCur = widthnew;
	m_nHeightCur = heightnew;
	m_nRowlenCur = m_nWidthCur * m_bytesPixel;
	LONG m_nlenCur = m_nHeightCur * m_nRowlenCur;
	//�������ű���
	float ratewidth = 1.0f * m_nWidthCur / width;
	float rateheight = 1.0f * m_nHeightCur/ height;
	//����ռ����ڱ���Ŀ��ͼ�����ݼ��м���
	BYTE* dataStep1 = new BYTE[height * m_nRowlenCur];
	BYTE* dataDes = new BYTE[m_nlenCur];

	//˫���β�ֵ��
	int r = 0, c = 0, k = 0;
	//�������ֵ
	for(r = 0; r < height; r++)
	{
		BYTE* pDes = dataStep1 + r * m_nRowlenCur;
		for(c = 0; c < m_nWidthCur; c++)
		{
			int col = (int)(c / ratewidth);
			float fRateX = c / ratewidth - col;
			BYTE* pSrc = m_dataSrc + r * byteswidth + col * m_bytesPixel;
			if (col < width-1)
			{
				for (k = 0; k<m_bytesPixel; k++)
				{
					*(pDes+k) = (BYTE)(*(pSrc+k) * (1-fRateX) + *(pSrc+m_bytesPixel+k) * fRateX);
				}
			}
			else
			{
				for (k = 0; k<m_bytesPixel; k++)
				{
					*(pDes+k) = (BYTE)*(pSrc+k);
				}
			}
			pDes += m_bytesPixel;
		}
	}
	//�������ֵ
	for (c = 0; c < m_nWidthCur; c++)
	{
		BYTE* pDes = dataDes + c * m_bytesPixel;
		for (r = 0; r < m_nHeightCur; r++)
		{
			int row = (int)(r / rateheight);
			float fRateY = r / rateheight - row;
			BYTE* pSrc = dataStep1 + row * m_nRowlenCur + c * m_bytesPixel;
			if(row < height-1)
			{
				for (k = 0; k<m_bytesPixel; k++)
				{
					*(pDes+k) = (BYTE)(*(pSrc+k) * (1-fRateY) + *(pSrc+m_nRowlenCur+k) * fRateY);
				}
			}
			else
			{
				for (k = 0; k<m_bytesPixel; k++)
				{
					*(pDes+k) = (BYTE)*(pSrc+k);
				}
			}
			pDes += m_nRowlenCur;
		}
	}
	Create32Bitmap(dataDes, m_bytesPixel, widthnew, heightnew);

	delete [] dataStep1;
	delete [] dataDes;
}

BOOL CFFTDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���öԻ����С
	int nScreenWidth = int(GetSystemMetrics(SM_CXSCREEN));
	int nScreenHeight = int(GetSystemMetrics(SM_CYSCREEN));
	int x = nScreenWidth-m_nWidthCur-100;
	int y = nScreenHeight-m_nHeightCur-20;
	if (x>=0 && y>=0)
	{
		::SetWindowPos(this->m_hWnd, HWND_TOP, 0, 0, m_nWidthCur+100+18, m_nHeightCur+20+36, SWP_SHOWWINDOW);
	}
	else
	{
		::SetWindowPos(this->m_hWnd, HWND_TOP, 0, 0, nScreenWidth, nScreenHeight, SWP_SHOWWINDOW);
	}
	CenterWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
}

//
// *** ���� ***
//

// ************************************************************************
//  �ļ�����FFTDlg.cpp
//
//  ͼ�������任�����⣺
//
//	FFT()				- һά���ٸ���Ҷ�任
//  IFFT()				- һά���ٸ���Ҷ��任
//  Fourier()			- ��ά���ٸ���Ҷ�任
//  IFourier()			- ��ά���ٸ���Ҷ��任
//  DCT()				- һά������ɢ���ұ任
//  IDCT()				- һά������ɢ������任
//	FreqDCT()			- ��ά������ɢ���ұ任
//  IFreqDCT()			- ��ά������ɢ������任
//  WALSH()				- һά�ֶ�ʲ��������任
//  IWALSH()			- һά�ֶ�ʲ����������任
//	FreqWALSH()			- ��ά�ֶ�ʲ��������任
//	IFreqWALSH()		- ��ά�ֶ�ʲ����������任
//	DWT()				- ��ά�����С���ֽ�
//	IDWT()				- ��ά�����С���ع�
//	
//  DIBFourier()		- ͼ��ĸ���Ҷ�任
//  DIBDCT()			- ͼ�����ɢ���ұ任
//  DIBWalsh()			- ͼ����ֶ�ʲ��������任
//  DIBDWT()			- ͼ��Ķ�ά��ɢС���任
//
//*************************************************************************


/*************************************************************************
 *
 * �������ƣ�
 *   FFT()
 *
 * ����:
 *   complex<double> * TD	- ָ��ʱ�������ָ��
 *   complex<double> * FD	- ָ��Ƶ�������ָ��
 *   r						��2������������������
 *
 * ����ֵ:
 *   �ޡ�
 *
 * ˵��:
 *   �ú�������ʵ�ֿ��ٸ���Ҷ�任��
 *
 ************************************************************************/

void CFFTDlg::FFT(complex<double> * TD, complex<double> * FD, int r)
{
	// ѭ������
	LONG	i;
	LONG	j;
	LONG	k;
	
	// �м����
	int		p;
	
	// �Ƕ�
	double	angle;
	
	complex<double> *W,*X1,*X2,*X;
	
	// ���㸵��Ҷ�任����
	LONG N = 1 << r;
	
	// ������������洢��
	W  = new complex<double>[N / 2];
	X1 = new complex<double>[N];
	X2 = new complex<double>[N];
	
	// �����Ȩϵ��
	for(i = 0; i < N / 2; i++)
	{
		angle = -i * PI * 2 / N;
		W[i] = complex<double> (cos(angle), sin(angle));
	}
	
	// ��ʱ���д��X1
	memcpy(X1, TD, sizeof(complex<double>) * N);
	
	// ���õ����㷨���п��ٸ���Ҷ�任
	for(k = 0; k < r; k++)
	{
		for(j = 0; j < 1 << k; j++)
		{
			for(i = 0; i < 1<<(r - k -1); i++)
			{
				p = j * (1<<(r - k));
				X2[i + p] = X1[i + p] + X1[i + p + (int)(1<<(r - k -1))];
				X2[i + p + (int)(1<<(r - k -1))] = (X1[i + p] - X1[i + p + (int)(1<<(r - k -1))]) * W[i * (1<<k)];
			}
		}
		X  = X1;
		X1 = X2;
		X2 = X;
	}
	
	// ��������
	for(j = 0; j < N; j++)
	{
		p = 0;
		for(i = 0; i < r; i++)
		{
			if (j&(1<<i))
			{
				p+=1<<(r - i - 1);
			}
		}
		FD[j] = X1[p];
	}
	
	// �ͷ��ڴ�
	delete W;
	delete X1;
	delete X2;
}


/*************************************************************************
 *
 * �������ƣ�
 *   IFFT()
 *
 * ����:
 *   complex<double> * FD	- ָ��Ƶ��ֵ��ָ��
 *   complex<double> * TD	- ָ��ʱ��ֵ��ָ��
 *   r						��2������
 *
 * ����ֵ:
 *   �ޡ�
 *
 * ˵��:
 *   �ú�������ʵ�ֿ��ٸ���Ҷ��任��
 *
 ************************************************************************/

void CFFTDlg::IFFT(complex<double> * FD, complex<double> * TD, int r)
{
	// ѭ������
	int		i;
	
	complex<double> *X;
	
	// ���㸵��Ҷ�任����
	LONG N = 1<<r;
	
	// ������������洢��
	X = new complex<double>[N];
	
	// ��Ƶ���д��X
	memcpy(X, FD, sizeof(complex<double>) * N);
	
	// ����
	for (i = 0; i < N; i++)
	{
		X[i] = complex<double> (X[i].real(), -X[i].imag());
	}
	
	// ���ÿ��ٸ���Ҷ�任
	FFT(X, TD, r);
	
	// ��ʱ���Ĺ���
	for (i = 0; i < N; i++)
	{
		TD[i] = complex<double> (TD[i].real() / N, -TD[i].imag() / N);
	}
	
	// �ͷ��ڴ�
	delete X;
}


/*************************************************************************
 *
 * �������ƣ�
 *   Fourier()
 *
 * ����:
 *   complex* TD		- �����ʱ������
 *	 LONG lWidth		- ͼ����
 *	 LONG lHeight		- ͼ��߶�
 *	 complex* FD		- �����Ƶ������
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú������ж�ά���ٸ���Ҷ�任��
 *
 ************************************************************************/

BOOL CFFTDlg::Fourier(complex<double> * TD, LONG lWidth, LONG lHeight, complex<double> * FD)
{
	// ѭ������
	LONG	i;
	LONG	j;
	LONG    k;

	// ���Ĺ����״
	BeginWaitCursor();
	
	// ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG w = 1;
	LONG h = 1;
	int wp = 0;
	int hp = 0;
	
	// ������и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	while (w < lWidth)
	{
		w *= 2;
		wp++;
	}
	         
	while (h < lHeight)
	{
		h *= 2;
		hp++;
	}
		
	// �����ڴ�
	complex<double> *TempT, *TempF;
	TempT = new complex<double>[h];
	TempF = new complex<double>[h];
	
	// ��y������п��ٸ���Ҷ�任
	for (i = 0; i < w * 3; i++)
	{
		// ��ȡ����
		for (j = 0; j < h; j++)
			TempT[j] = TD[j * w * 3 + i];
		
		// һά���ٸ���Ҷ�任
		FFT(TempT, TempF, hp);

		// ����任���
		for (j = 0; j < h; j++)
			TD[j * w * 3 + i] = TempF[j];
	}
	
	// �ͷ��ڴ�
	delete TempT;
	delete TempF;

	// �����ڴ�
	TempT = new complex<double>[w];
	TempF = new complex<double>[w];
	
	// ��x������п��ٸ���Ҷ�任
	for (i = 0; i < h; i++)
	{
		for (k = 0; k < 3; k++)
		{
			// ��ȡ����
			for (j = 0; j < w; j++)
				TempT[j] = TD[i * w * 3 + j * 3 + k];

			// һά���ٸ���Ҷ�任
			FFT(TempT, TempF, wp);

			// ����任���
			for (j = 0; j < w; j++)
				FD[i * w * 3 + j * 3 + k] = TempF[j];
		}
	}

	// �ͷ��ڴ�
	delete TempT;
	delete TempF;

	return TRUE;
}


/*************************************************************************
 *
 * �������ƣ�
 *   IFourier()
 *
 * ����:
 *   LPBYTE TD			- ���صĿ�������
 *   LONG lWidth		- ����ͼ��Ŀ��
 *	 LONG lHeight		- ����ͼ��ĸ߶�
 *	 complex* FD		- �����Ƶ������
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú������ж�ά���ٸ���Ҷ��任��
 *
 ************************************************************************/

BOOL CFFTDlg::IFourier(LPBYTE TD, LONG lWidth, LONG lHeight, complex<double> * FD)
{
	// ѭ������
	LONG	i;
	LONG	j;
	LONG    k;

	// ���Ĺ����״
	BeginWaitCursor();
	
	// ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG w = 1;
	LONG h = 1;
	int wp = 0;
	int hp = 0;
	
	// ������и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	while(w < lWidth)
	{
		w *= 2;
		wp++;
	}
	
	while(h < lHeight)
	{
		h *= 2;
		hp++;
	}

	// ����ͼ��ÿ�е��ֽ���
	LONG lLineBytes = m_nRowlen24Bit;

	// �����ڴ�
	complex<double> *TempT, *TempF;
	TempT = new complex<double>[w];
	TempF = new complex<double>[w];
	
	// ��x������п��ٸ���Ҷ�任
	for (i = 0; i < h; i++)
	{
		for (k = 0; k < 3; k++)
		{
			// ��ȡ����
			for (j = 0; j < w; j++)
				TempF[j] = FD[i * w * 3 + j * 3 + k];

			// һά���ٸ���Ҷ�任
			IFFT(TempF, TempT, wp);

			// ����任���
			for (j = 0; j < w; j++)
				FD[i * w * 3 + j * 3 + k] = TempT[j];
		}
	}

	// �ͷ��ڴ�
	delete TempT;
	delete TempF;
	
	TempT = new complex<double>[h];
	TempF = new complex<double>[h];

	// ��y������п��ٸ���Ҷ�任
	for (i = 0; i < w * 3; i++)
	{
		// ��ȡ����
		for (j = 0; j < h; j++)
			TempF[j] = FD[j * w * 3 + i];
		
		// һά���ٸ���Ҷ�任
		IFFT(TempF, TempT, hp);

		// ����任���
		for (j = 0; j < h; j++)
			FD[j * w * 3 + i] = TempT[j];
	}
	
	// �ͷ��ڴ�
	delete TempT;
	delete TempF;

	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w * 3; j++)
		{
			if (i < lHeight && j < lLineBytes)
				*(TD + i * lLineBytes + j) = FD[i * w * 3 + j].real();
		}
	}

	return TRUE;
}


/*************************************************************************
 *
 * �������ƣ�
 *   DIBFourier()
 *
 * ����:
 *   HDIB	hDIB		- �������DIB
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú���������ͼ����и���Ҷ�任��
 *
 ************************************************************************/

BOOL CFFTDlg::DIBFourier(HDIB hDIB)
{
	// ָ��Դͼ���ָ��
	BYTE*	lpSrc;

	// �м����
	double	dTemp;
	LONG TI,TJ;
	
	// ѭ������
	LONG	i;
	LONG	j;
	
	// ָ��DIB��ָ��
	LPBYTE	lpDIB;
	
	// ָ��DIB����ָ��
	LPBYTE    lpDIBBits = m_data24Bit;
	
	// ����DIB
	lpDIB = (LPBYTE) ::GlobalLock((HGLOBAL) hDIB);

	// ���Ĺ����״
	BeginWaitCursor();
	
	// DIB�Ŀ��
	LONG lWidth = m_nWidth24Bit;
	
	// DIB�ĸ߶�
	LONG lHeight = m_nHeight24Bit;

	// ����ͼ��ÿ�е��ֽ���
	LONG lLineBytes = m_nRowlen24Bit;

	// ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG w = 1;
	LONG h = 1;
	int wp = 0;
	int hp = 0;
	
	// ������и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	while(w < lWidth)
	{
		w *= 2;
		wp++;
	}
	
	while(h < lHeight)
	{
		h *= 2;
		hp++;
	}

	// �����ڴ�
	complex<double> *FD, *TD, *TempD;
	FD = new complex<double>[w * h * 3];
	TD = new complex<double>[w * h * 3];
	TempD =  new complex<double>[w * h * 3];
	
	// ��
	for(i = 0; i < h; i++)
	{
		// ��
		for(j = 0; j < 3 * w; j++)
		{
			if(i < lHeight && j < lLineBytes)
			{
				// ��ȡʱ����ֵ
				BYTE Value = *((BYTE *)lpDIBBits + lLineBytes * i + j);
			
				// ʱ��ֵ
				TD[w * i * 3 + j] = complex<double>(Value, 0.0f);
			}
			else
			{
				// ������
				TD[w * i * 3 + j] = complex<double>(0.0f, 0.0f);
			}
		}
	}

	// ����Ƶ�׷���
	if (Fourier(TD, lWidth, lHeight, FD) == FALSE)
		return FALSE;

	// �ͷ��ڴ�
	delete []TD;

	// ��ԭ�������ͼ������λ��
	for(i = 0; i < h; i++)
	{
		for(j = 0; j < 3 * w; j++)
		{
			if (i < h / 2)
				TI = i + h / 2;
			else
				TI = i - h / 2;

			if (j < w * 3 /2)
				TJ = j + 3 * w / 2;
			else 
				TJ = j - 3 * w / 2;

			// ����ת�����Ƶ��ͼ��
			TempD[i * w * 3 + j] = FD[TI * w * 3 + TJ];
		}
	}

	// ��
	for(i = (int)(h - lHeight) / 2; i < (int)(h + lHeight) / 2; i++)
	{
		// ��
		for(j = (int)(w * 3 - lLineBytes) / 2; j < (int)(w * 3 + lLineBytes) / 2; j += 3)
		{
			// ����Ƶ��
			dTemp = sqrt(TempD[w * 3 * i + j].real() * TempD[w * 3 * i + j].real() + 
				         TempD[w * 3 * i + j].imag() * TempD[w * 3 * i + j].imag()) / 100;

			// �ж��Ƿ񳬹�255
			if (dTemp > 255)
			{
				// ���ڳ����ģ�ֱ������Ϊ255
				dTemp = 255;
			}

			// ����Ϊԭͼ��С��Χ
			TI = i - (h - lHeight) / 2;
			TJ = j / 3 - (w - lWidth) / 2;
			
			// ��Ӧ����ָ��
			lpSrc = (BYTE*)lpDIBBits + lLineBytes * TI + TJ * 3;

			// ����Դͼ��
			* (lpSrc) = (BYTE) (dTemp);
			* (lpSrc + 1) = (BYTE) (dTemp);
			* (lpSrc + 2) = (BYTE) (dTemp);
		}
	}

	// �������
	::GlobalUnlock(hDIB);

	// ɾ����ʱ����
	delete []FD;
	delete []TempD;
	
	// �ָ����
	EndWaitCursor();
	
	// ����
	return TRUE;
}


/*************************************************************************
 *
 * �������ƣ�
 *   DCT()
 *
 * ����:
 *   double * f				- ָ��ʱ��ֵ��ָ��
 *   double * F				- ָ��Ƶ��ֵ��ָ��
 *   r						��2������
 *
 * ����ֵ:
 *   �ޡ�
 *
 * ˵��:
 *   �ú�������ʵ��һά������ɢ���ұ任
 *
 ************************************************************************/

void CFFTDlg::DCT(double *f, double *F, int r)
{
	// ѭ������
	int		i;
	
	// �м����
	double	dTemp;
	
	// ������ɢ���ұ任����
	LONG N = 1<<r;
	
	// ���벢�����ڴ�
	complex<double> *XIn;
	complex<double> *XOut;
	XIn = new complex<double>[N * 2];
	XOut = new complex<double>[N * 2];
	
	// ����ֵΪ0
	memset(XIn, 0, sizeof(complex<double>) * N * 2);
	memset(XOut, 0, sizeof(complex<double>) * N * 2);
	
	// ��ʱ���д������X
	for (i = 0; i < N; i++)
		XIn[i] = complex<double>(*(f + i), 0);
	
	// ���ÿ��ٸ���Ҷ�任
	FFT(XIn, XOut, r + 1);
	
	// ����ϵ��
	dTemp = 1 / sqrt(N);
	
	// ��F[0]
	F[0] = XOut[0].real() * dTemp;
	
	dTemp *= sqrt(2);
	
	// ��F[u]
	for (i = 1; i < N; i++)
		*(F + i) = (XOut[i].real() * cos(i * PI / (N * 2)) + XOut[i].imag() * sin(i * PI / (N * 2))) * dTemp;
	
	// �ͷ��ڴ�
	delete[] XIn;
	delete[] XOut;
}


/*************************************************************************
 *
 * �������ƣ�
 *   IDCT()
 *
 * ����:
 *   double * F				- ָ��Ƶ��ֵ��ָ��
 *   double * f				- ָ��ʱ��ֵ��ָ��
 *   r						��2������
 *
 * ����ֵ:
 *   �ޡ�
 *
 * ˵��:
 *   �ú���ʵ��һά������ɢ������任
 *
 ************************************************************************/

void CFFTDlg::IDCT(double *F, double *f, int r)
{
	// ѭ������
	int		i;
	
	// �м����
	double	dTemp, d0;
		
	// ������ɢ���ұ任����
	LONG N = 1<<r;
	
	// �����ڴ�
	complex<double> *XIn;
	complex<double> *XOut;
	XIn = new complex<double>[N * 2];
	XOut = new complex<double>[N * 2];

	// ����ֵΪ0
	memset(XIn, 0, sizeof(complex<double>) * N * 2);
	memset(XOut, 0, sizeof(complex<double>) * N * 2);
	
	// ��Ƶ��任���д������X
	for (i = 0; i < N; i++)
		XIn[i] = complex<double>(F[i] * cos(i * PI / (N * 2)), F[i] * sin(i * PI / (N * 2)));
	
	// ���ÿ��ٸ���Ҷ���任
	IFFT(XIn, XOut, r + 1);
	
	// ����ϵ��
	dTemp = sqrt(2.0 / N);
	d0 = (sqrt(1.0 / N) - dTemp) * F[0];
	
	// ����f(x)
	for (i = 0; i < N; i++)
		f[i] = d0 + XOut[i].real()* dTemp * 2 * N;
	
	// �ͷ��ڴ�
	delete[] XIn;
	delete[] XOut;
}


/*************************************************************************
 *
 * �������ƣ�
 *   FreqDCT()
 *
 * ����:
 *   double* f			- �����ʱ������
 *   double* F			- �����Ƶ������
 *	 LONG lWidth		- ͼ����
 *	 LONG lHeight		- ͼ��߶�
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú������ж�ά������ɢ���ұ任��
 *
 ************************************************************************/

BOOL CFFTDlg::FreqDCT(double *f, double *F, LONG lWidth, LONG lHeight)
{
	// ѭ������
	LONG	i;
	LONG	j;
	LONG    k;

	// ���Ĺ����״
	BeginWaitCursor();
	
	// ������ɢ���ұ任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG w = 1;
	LONG h = 1;
	int wp = 0;
	int hp = 0;
	
	// ���������ɢ���ұ任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	while (w < lWidth)
	{
		w *= 2;
		wp++;
	}
	
	while (h < lHeight)
	{
		h *= 2;
		hp++;
	}
		
	// �����ڴ�
	double* TempIn = new double[h];
	double* TempOut = new double[h];
	
	// ��y���������ɢ���ұ任
	for (i = 0; i < w * 3; i++)
	{
		// ��ȡ����
		for (j = 0; j < h; j++)
			TempIn[j] = f[j * w * 3 + i];
		
		// һά������ɢ���ұ任
		DCT(TempIn, TempOut, hp);

		// ����任���
		for (j = 0; j < h; j++)
			f[j * w * 3 + i] = TempOut[j];
	}
	
	// �ͷ��ڴ�
	delete TempIn;
	delete TempOut;

	// �����ڴ�
	TempIn = new double[w];
	TempOut = new double[w];
	
	// ��x������п�����ɢ���ұ任
	for (i = 0; i < h; i++)
	{
		for (k = 0; k < 3; k++)
		{
			// ��ȡ����
			for (j = 0; j < w; j++)
				TempIn[j] = f[i * w * 3 + j * 3 + k];

			// һά������ɢ���ұ任
			DCT(TempIn, TempOut, wp);

			// ����任���
			for (j = 0; j < w; j++)
				F[i * w * 3 + j * 3 + k] = TempOut[j];
		}
	}

	// �ͷ��ڴ�
	delete TempIn;
	delete TempOut;

	return TRUE;
}


/*************************************************************************
 *
 * �������ƣ�
 *   IFreqDCT()
 *
 * ����:
 *   double* f			- �����ʱ������
 *   double* F			- �����Ƶ������
 *	 LONG lWidth		- ͼ����
 *	 LONG lHeight		- ͼ��߶�
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú������ж�ά������ɢ������任��
 *
 ************************************************************************/

BOOL CFFTDlg::IFreqDCT(double *f, double *F, LONG lWidth, LONG lHeight)
{
	// ѭ������
	LONG	i;
	LONG	j;
	LONG    k;

	// ���Ĺ����״
	BeginWaitCursor();
	
	// ������ɢ���ұ任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG w = 1;
	LONG h = 1;
	int wp = 0;
	int hp = 0;
	
	// ������и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	while (w < lWidth)
	{
		w *= 2;
		wp++;
	}
	
	while (h < lHeight)
	{
		h *= 2;
		hp++;
	}

	// ����ͼ��ÿ�е��ֽ���
	LONG lLineBytes = m_nRowlen24Bit;

	// �����ڴ�
	double* TempIn = new double[w];
	double* TempOut = new double[w];
	
	// ��x������п��ٸ���Ҷ�任
	for (i = 0; i < h; i++)
	{
		for (k = 0; k < 3; k++)
		{
			// ��ȡ����
			for (j = 0; j < w; j++)
				TempIn[j] = F[i * w * 3 + j * 3 + k];

			// һά���ٸ���Ҷ�任
			IDCT(TempIn, TempOut, wp);

			// ����任���
			for (j = 0; j < w; j++)
				F[i * w * 3 + j * 3 + k] = TempOut[j];
		}
	}

	// �ͷ��ڴ�
	delete TempIn;
	delete TempOut;
	
	TempIn = new double[h];
	TempOut = new double[h];

	// ��y������п��ٸ���Ҷ�任
	for (i = 0; i < w * 3; i++)
	{
		// ��ȡ����
		for (j = 0; j < h; j++)
			TempIn[j] = F[j * w * 3 + i];
		
		// һά���ٸ���Ҷ�任
		IDCT(TempIn, TempOut, hp);

		// ����任���
		for (j = 0; j < h; j++)
			F[j * w * 3 + i] = TempOut[j];
	}
	
	// �ͷ��ڴ�
	delete TempIn;
	delete TempOut;
	
	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w * 3; j++)
		{
			if (i < lHeight && j < lLineBytes)
				*(f + i * lLineBytes + j) = F[i * w * 3 + j];
		}
	}

	return TRUE;
}

/*************************************************************************
 *
 * �������ƣ�
 *   DIBDCT()
 *
 * ����:
 *   HDIB  hDIB		    - �������DIB
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú���������ͼ�������ɢ���ұ任��
 *
 ************************************************************************/

BOOL CFFTDlg::DIBDCT(HDIB hDIB)
{
	// ������ɢ���ұ任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG	i;
	LONG	j;
	
	// ָ��DIB��ָ��
	LPBYTE	lpDIB;
	
	// ָ��DIB����ָ��
	LPBYTE  lpDIBBits = m_data24Bit;
	
	// ����DIB
	lpDIB = (LPBYTE) ::GlobalLock((HGLOBAL) hDIB);

	// DIB�Ŀ��
	LONG lWidth = m_nWidth24Bit;
	
	// DIB�ĸ߶�
	LONG lHeight = m_nHeight24Bit;
	
	// ����ͼ��ÿ�е��ֽ���
	LONG lLineBytes = m_nRowlen24Bit;
	
	// ������ɢ���ұ任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG w = 1;
	LONG h = 1;
	int wp = 0;
	int hp = 0;
	
	// ���������ɢ���ұ任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	while (w < lWidth)
	{
		w *= 2;
		wp++;
	}
	
	while (h < lHeight)
	{
		h *= 2;
		hp++;
	}

	// �����ڴ�
	double *f = new double[w * h * 3];
	double *F = new double[w * h * 3];

	// ��ʱ��ֵ������
	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w * 3; j++)
		{
			if (i < lHeight && j < lLineBytes)
				f[i * w * 3 + j] = *(BYTE*)(lpDIBBits + lLineBytes * i + j);
			else
				f[w * i * 3 + j] = 0.0f;
		}
	}

	// ����Ƶ�׷���
	if (FreqDCT(f, F,lWidth, lHeight) == FALSE)
		return FALSE;

	// ������������
	for (i = 0; i < lHeight; i++)
	{
		for (j = 0; j < lLineBytes; j++)
		{
			// �ж��Ƿ񳬹�255
			if (fabs(F[i * w * 3 + j]) > 255)
			{
				// ���ڳ����ģ�ֱ������Ϊ255
				*(BYTE*)(lpDIBBits + lLineBytes * (lHeight - 1 - i) + j) = 255;
			}
			else
			{
				// ���û�г�������ʵ�ʼ�������ֵ
				*(BYTE*)(lpDIBBits + lLineBytes * (lHeight - 1 - i) + j) = fabs(F[i * w * 3 + j]);
			}
		}
	}

	// �������
	::GlobalUnlock(hDIB);
	
	// �ͷ��ڴ�
	delete []f;
	delete[] F;

	// ����
	return TRUE;
}


/*************************************************************************
 *
 * �������ƣ�
 *   WALSH()
 *
 * ����:
 *   double* f			- �����ʱ������
 *   double* F			- �����Ƶ������
 *   int r				- 2������		
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú�������һά�����ֶ�ʲ����������任��
 *
 ************************************************************************/

void CFFTDlg::WALSH(double *f, double *F, int r)
{
	// ѭ������
	LONG	i;
	LONG	j;
	LONG	k;
	
	// �м����
	int		p;	
	double* X;
	
	// ��������ֶ�ʲ�任����
	LONG N = 1 << r;
	
	// �����������������
	double* X1 = new double[N];
	double* X2 = new double[N];
	
	// ��ʱ���д������X1
	memcpy(X1, f, sizeof(double) * N);
	
	// ��������
	for (k = 0; k < r; k++)
	{
		for (j = 0; j < 1<<k; j++)
		{
			for (i = 0; i < 1<<(r - k - 1); i++)
			{
				p = j * (1<<(r - k));
				X2[i + p] = X1[i + p] + X1[i + p + (int)(1<<(r - k - 1))];
				X2[i + p + (int)(1<<(r - k - 1))] = X1[i + p] - X1[i + p + (int)(1<<(r - k - 1))];
			}
		}
		
		// ����X1��X2  
		X = X1;
		X1 = X2;
		X2 = X;
	}
	
	// ����ϵ��
	for (j = 0; j < N; j++)
	{
		p = 0;
		for (i = 0; i < r; i++)
		{
			if (j & (1<<i))
			{
				p += 1<<(r - i - 1);
			}
		}

		F[j] = X1[p] / N;
	}
	
	// �ͷ��ڴ�
	delete X1;
	delete X2;
}


/*************************************************************************
 *
 * �������ƣ�
 *   IWALSH()
 *
 * ����:
 *   double* f			- �����ʱ������
 *   double* F			- �����Ƶ������
 *   int r				- 2������		
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú�������һά�����ֶ�ʲ������������任��
 *
 ************************************************************************/

void CFFTDlg::IWALSH(double *F, double *f, int r)
{
	// ѭ������
	int		i;
	
	// ����任����
	LONG N = 1 << r;
	
	// ���ÿ����ֶ�ʲ��������任���з��任
	WALSH(F, f, r);
	
	// ����ϵ��
	for (i = 0; i < N; i++)
		f[i] *= N;
}


/*************************************************************************
 *
 * �������ƣ�
 *   FreqWALSH()
 *
 * ����:
 *   double* f			- �����ʱ������
 *   double* F			- �����Ƶ������
 *	 LONG lWidth		- ͼ����
 *	 LONG lHeight		- ͼ��߶�
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú������ж�ά�����ֶ�ʲ����������任��
 *
 ************************************************************************/

BOOL CFFTDlg::FreqWALSH(double *f, double *F, LONG lWidth, LONG lHeight)
{
	// ѭ������
	LONG	i;
	LONG	j;
	LONG    k;

	// ���Ĺ����״
	BeginWaitCursor();
	
	// ������ɢ���ұ任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG w = 1;
	LONG h = 1;
	int wp = 0;
	int hp = 0;
	
	// ���������ɢ���ұ任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	while (w < lWidth)
	{
		w *= 2;
		wp++;
	}
	
	while (h < lHeight)
	{
		h *= 2;
		hp++;
	}
		
	// �����ڴ�
	double* TempIn = new double[h];
	double* TempOut = new double[h];
	
	// ��y���������ɢ���ұ任
	for (i = 0; i < w * 3; i++)
	{
		// ��ȡ����
		for (j = 0; j < h; j++)
			TempIn[j] = f[j * w * 3 + i];
		
		// һά������ɢ���ұ任
		WALSH(TempIn, TempOut, hp);

		// ����任���
		for (j = 0; j < h; j++)
			f[j * w * 3 + i] = TempOut[j];
	}
	
	// �ͷ��ڴ�
	delete TempIn;
	delete TempOut;

	// �����ڴ�
	TempIn = new double[w];
	TempOut = new double[w];
	
	// ��x������п�����ɢ���ұ任
	for (i = 0; i < h; i++)
	{
		for (k = 0; k < 3; k++)
		{
			// ��ȡ����
			for (j = 0; j < w; j++)
				TempIn[j] = f[i * w * 3 + j * 3 + k];

			// һά������ɢ���ұ任
			WALSH(TempIn, TempOut, wp);

			// ����任���
			for (j = 0; j < w; j++)
				F[i * w * 3 + j * 3 + k] = TempOut[j];
		}
	}

	// �ͷ��ڴ�
	delete TempIn;
	delete TempOut;

	return TRUE;
}



/*************************************************************************
 *
 * �������ƣ�
 *   IFreqWALSH()
 *
 * ����:
 *   double* f			- �����ʱ������
 *   double* F			- �����Ƶ������
 *	 LONG lWidth		- ͼ����
 *	 LONG lHeight		- ͼ��߶�
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú������ж�ά�����ֶ�ʲ������������任��
 *
 ************************************************************************/

BOOL CFFTDlg::IFreqWALSH(double *f, double *F, LONG lWidth, LONG lHeight)
{
	// ѭ������
	LONG	i;
	LONG	j;
	LONG    k;

	// ���Ĺ����״
	BeginWaitCursor();
	
	// ����ֵ
	LONG w = 1;
	LONG h = 1;
	int wp = 0;
	int hp = 0;
	
	// ������и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	while (w < lWidth)
	{
		w *= 2;
		wp++;
	}
	
	while (h < lHeight)
	{
		h *= 2;
		hp++;
	}

	// ����ͼ��ÿ�е��ֽ���
	LONG lLineBytes = m_nRowlen24Bit;

	// �����ڴ�
	double* TempIn = new double[w];
	double* TempOut = new double[w];
	
	// ��x������п��ٸ���Ҷ�任
	for (i = 0; i < h; i++)
	{
		for (k = 0; k < 3; k++)
		{
			// ��ȡ����
			for (j = 0; j < w; j++)
				TempIn[j] = F[i * w * 3 + j * 3 + k];

			// һά���ٸ���Ҷ�任
			IWALSH(TempIn, TempOut, wp);

			// ����任���
			for (j = 0; j < w; j++)
				F[i * w * 3 + j * 3 + k] = TempOut[j];
		}
	}

	// �ͷ��ڴ�
	delete TempIn;
	delete TempOut;
	
	TempIn = new double[h];
	TempOut = new double[h];

	// ��y������п��ٸ���Ҷ�任
	for (i = 0; i < w * 3; i++)
	{
		// ��ȡ����
		for (j = 0; j < h; j++)
			TempIn[j] = F[j * w * 3 + i];
		
		// һά���ٸ���Ҷ�任
		IWALSH(TempIn, TempOut, hp);

		// ����任���
		for (j = 0; j < h; j++)
			F[j * w * 3 + i] = TempOut[j];
	}
	
	// �ͷ��ڴ�
	delete TempIn;
	delete TempOut;

	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w * 3; j++)
		{
			if (i < lHeight && j < lLineBytes)
				*(f + i * lLineBytes + j) = F[i * w * 3 + j];
		}
	}

	return TRUE;
}

/*************************************************************************
 *
 * �������ƣ�
 *   DIBWalsh()
 *
 * ����:
 *   HDIB	hDIB		- �������DIB
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú�����ͼ����ж�ά�����ֶ�ʲ����������任��
 *
 ************************************************************************/

BOOL CFFTDlg::DIBWalsh(HDIB hDIB)
{
	// ѭ������
	LONG	i;
	LONG	j;
	
	// ָ��DIB��ָ��
	LPBYTE	lpDIB;
	
	// ָ��DIB����ָ��
	LPBYTE  lpDIBBits = m_data24Bit;
	
	// ����DIB
	lpDIB = (LPBYTE) ::GlobalLock((HGLOBAL) hDIB);

	// DIB�Ŀ��
	LONG lWidth = m_nWidth24Bit;
	
	// DIB�ĸ߶�
	LONG lHeight = m_nHeight24Bit;

	// ����ͼ��ÿ�е��ֽ���
	LONG lLineBytes = m_nRowlen24Bit;
	
	// �����ֶ�ʲ����������任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG w = 1;
	LONG h = 1;
	int wp = 0;
	int hp = 0;
	
	// ���������ɢ���ұ任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	while (w < lWidth)
	{
		w *= 2;
		wp++;
	}
	
	while (h < lHeight)
	{
		h *= 2;
		hp++;
	}
	
	// �����ڴ�
	double *f = new double[w * h * 3];
	double *F = new double[w * h * 3];
	
	// ��ʱ��ֵ������
	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w * 3; j++)
		{
			if (i < lHeight && j < lLineBytes)
				f[i * w * 3 + j] = *(BYTE*)(lpDIBBits + lLineBytes * i + j);
			else
				f[w * i * 3 + j] = 0.0f;
		}
	}

	// ����Ƶ�׷���
	if (FreqWALSH(f, F,lWidth, lHeight) == FALSE)
		return FALSE;
	
	// ������������
	for (i = 0; i < lHeight; i++)
	{
		for (j = 0; j < lLineBytes; j++)
		{
			// �ж��Ƿ񳬹�255
			if (fabs(F[i * w * 3 + j] * 1000) > 255)
			{
				// ���ڳ����ģ�ֱ������Ϊ255
				*(BYTE*)(lpDIBBits + lLineBytes * (lHeight - 1 - i) + j) = 255;
			}
			else
			{
				// ���û�г�������ʵ�ʼ�������ֵ
				*(BYTE*)(lpDIBBits + lLineBytes * (lHeight - 1 - i) + j) = fabs(F[i * w * 3 + j] * 1000);
			}
		}
	}

	// �������
	::GlobalUnlock(hDIB);

	//�ͷ��ڴ�
	delete[] f;
	delete[] F;

	// ����
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////
//																	     //
//							С���任									 //
//																		 //
///////////////////////////////////////////////////////////////////////////

#define DD 13
float h[DD] = { -0.00332761f,0.00569794f,	0.0196637f,	-0.0482603f,	-0.0485391f,
				0.292562f,	0.564406f,	0.292562f,	-0.0485391f,	-0.0482602f,
				-0.0196637f,	0.00569794f,	-0.0033276f};
float g[DD] = {0.00332761f,  0.00569794f,	-0.0196637f,	-0.0482603f,	0.0485391f,
			   0.292562f,	-0.564406f,	0.292562f,	0.0485391f,	-0.0482602f,
			   0.0196637f,	0.00569794f,	0.0033276f};
float hi[DD];
float gi[DD];

int CFFTDlg::a(int nX, int nXSize)
{
	if (nX < 0)
		nX = -nX;
	if (nX >= nXSize)
		nX = nXSize * 2 - nX - 2;
	return nX;
}


/*************************************************************************
 *
 * �������ƣ�
 *   DWT()
 *
 * ����:
 *   HDIB	hDIB		- �������DIB
 *
 * ����ֵ:
 *   void	            - �޷���ֵ
 *
 * ˵��:
 *   �ú����Զ�ά���������ɢС���任��
 *
 ************************************************************************/

void CFFTDlg::DWT(LPBYTE lpData, int nX, int nY, int nXSize, int nYSize)
{
	// ѭ������
	int i, j, k, n;

	// �м����
	float temp1, temp2;
	float* BufferX;
	float* BufferY;

	BufferX = new float[nXSize];
	BufferY = new float[nYSize];

	// ˮƽ����
	for (n = 0; n < nY; n++)
	{
		for (i = 0; i < nX; i += 2)
		{
			temp1 = temp2 = 0;
			for (j = -(DD - 1) / 2; j <= (DD - 1) / 2; j++)
				temp1 = temp1 + *(lpData + n * nX + a(i + j, nX)) * h[j + (DD - 1) / 2];
			for (j = -(DD - 1) / 2 + 1; j <= (DD - 1) / 2 + 1; j++)
				temp2 = temp2 + *(lpData + n * nX + a(i + j, nX)) * g[j + (DD - 1) / 2 - 1];
			BufferX[i / 2] = temp1;
			BufferX[i / 2 + nX / 2] = temp2;
		}

		// �ش�����
		for (k = 0; k < nX; k++)
			*(lpData + n * nX + k) = BufferX[k];
	}

	// ��ֱ����
	for (n = 0; n < nX; n++)
	{
		for (i = 0; i < nY; i += 2)
		{
			temp1 = temp2 = 0;
			for (j = -(DD - 1) / 2; j <= (DD - 1) / 2; j++)
				temp1 = temp1 + *(lpData + a(i + j, nY) * nX + n) * h[j + (DD - 1) / 2];
			for (j = -(DD - 1) / 2 + 1; j <= (DD - 1) / 2 + 1; j++)
				temp2 = temp2 + *(lpData + a(i + j, nY) * nX + n) * g[j + (DD - 1) / 2 - 1];
			BufferY[i / 2] = temp2;
			BufferY[i / 2 + nY / 2] = temp1;
		}

		// �ش�����
		for (k = 0; k < nY; k++)
			*(lpData + k * nX + n) = BufferY[k];
	}
	
	delete[] BufferX;
	delete[] BufferY;
}


/*************************************************************************
 *
 * �������ƣ�
 *   IDWT()
 *
 * ����:
 *   HDIB	hDIB		- �������DIB
 *
 * ����ֵ:
 *   void	            - �޷���ֵ
 *
 * ˵��:
 *   �ú����Զ�ά�������С���ع���
 *
 ************************************************************************/

void CFFTDlg::IDWT(LPBYTE lpData, int nX, int nY, int nXSize, int nYSize)
{
	// ѭ������
	int i, j, k, n;

	// �м����
	float temp1, temp2;
	float* Buffer1;
	float* Buffer2;

	Buffer1 = new float[nXSize];
	Buffer2 = new float[nYSize];

	for (i = 0; i < DD; i++)
	{
		hi[i] = h[DD - 1 - i];
		gi[i] = g[DD - 1 - i];
	}

	// ��ֱ����
	for (n = 0; n < nX; n++)
	{
		for (k = 0; k < nY / 2; k++)
		{
			Buffer1[k * 2] = *(lpData + k * nX + n);
			Buffer1[k * 2 + 1] = 0;
		}
		for (i = 0; i <  nY; i++)
		{
			temp1 = 0;
			for (j = -(DD - 1) / 2; j <= (DD - 1) / 2; j++)
			{
				temp1 = temp1 + Buffer1[a(i + j, nY)] * hi[j + (DD - 1) / 2];
				Buffer2[i] = temp1;
			}
		}
		for (k = 0; k < nY / 2; k++)
		{
			Buffer1[k * 2] = *(lpData + (k + nY / 2) * nX + n);
			Buffer1[k * 2 + 1] = 0;
		}
		for (i = 0; i < nY; i++)
		{
			temp1 = 0;
			for (j = -(DD - 1) / 2 - 1; j <= (DD - 1) / 2 - 1; j++)
				temp1 = temp1 + Buffer1[a(i + j, nY)] * gi[j + (DD - 1) / 2 + 1];
			temp2 = temp1 + Buffer2[i];
			Buffer2[i] = temp2;
		}

		// �ش�����
		for (k = 0; k < nY; k++)
			*(lpData + k * nX + n) = Buffer2[k];
	}

	// ˮƽ����
	for (n = 0; n < nY; n++)
	{
		for (k = 0; k < nX / 2; k++)
		{
			Buffer1[k * 2] = *(lpData + n * nX + k);
			Buffer1[k * 2 + 1] = 0;
		}
		
		for (i = 0; i < nX; i++)
		{
			temp1 = 0;
			for (j = -(DD - 1) / 2; j <= (DD - 1) / 2; j++)
				temp1 = temp1 + Buffer1[a(i + j, nX)] * hi[j + (DD - 1) / 2];
			Buffer2[i] = temp1;
		}
		
		for (k = 0; k < nX / 2; k++)
		{
			Buffer1[k * 2] = *(lpData + n * nX + k + nX / 2);
			Buffer1[k * 2 + 1] = 0;
		}

		for (i = 0; i < nX; i++)
		{
			temp1 = 0;
			for (j = -(DD - 1) / 2 - 1; j <= (DD - 1) / 2 - 1; j++)
				temp1 = temp1 + gi[j + (DD - 1) / 2 + 1] * Buffer1[a(i + j, nX)];
			temp2 = temp1 + Buffer2[i];
			Buffer2[i] = temp2;
		}

		// �ش�����
		for (k = 0; k < nX; k++)
			*(lpData + n * nX + k) = Buffer2[k] * 4;
	}

	delete[] Buffer1;
	delete[] Buffer2;
}


/*************************************************************************
 *
 * �������ƣ�
 *   DIBDWT()
 *
 * ����:
 *   HDIB	hDIB		- �������DIB
 *
 * ����ֵ:
 *   void	            - �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ����ж�ά��ɢС���任��
 *
 ************************************************************************/

void CFFTDlg::DIBDWT(HDIB hDIB)
{
	// ָ��DIB��ָ��
	LPBYTE lpDIB;
	
	// ָ��DIB����ָ��
	LPBYTE lpDIBBits = m_data24Bit;
	
	// ����DIB
	lpDIB = (LPBYTE) ::GlobalLock((HGLOBAL) hDIB);
	
	// �ж��Ƿ���24-bppλͼ
	if (m_bmBitsPixel != 8)
	{
		// ��ʾ�û�
		MessageBox(L"���Ƚ���ת��Ϊ8λɫλͼ���ٽ��д���", L"ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
		
		// �������
		::GlobalUnlock((HGLOBAL) hDIB);
		
		// ����
		return;
	}
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// DIB�Ŀ��
	LONG lWidth = m_nWidth24Bit;
	
	// DIB�ĸ߶�
	LONG lHeight = m_nHeight24Bit;

	// ����С���ֽ�
	DWT(lpDIBBits, lWidth, lHeight, lWidth, lHeight);

	// �������
	::GlobalUnlock((HGLOBAL) hDIB);

	// �ָ����
	EndWaitCursor();
}


/*************************************************************************
 *
 * �������ƣ�
 *   DIBIDWT()
 *
 * ����:
 *   HDIB	hDIB		- �������DIB
 *
 * ����ֵ:
 *   void	            - �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ����ж�άС���ع���
 *
 ************************************************************************/

void CFFTDlg::DIBIDWT(HDIB hDIB)
{
	// ָ��DIB��ָ��
	LPBYTE lpDIB;
	
	// ָ��DIB����ָ��
	LPBYTE lpDIBBits = m_data24Bit;
	
	// ����DIB
	lpDIB = (LPBYTE) ::GlobalLock((HGLOBAL) hDIB);
	
	// �ж��Ƿ���24-bppλͼ
	if (m_bmBitsPixel != 8)
	{
		// ��ʾ�û�
		MessageBox(L"���Ƚ���ת��Ϊ8λɫλͼ���ٽ��д���", L"ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
		
		// �������
		::GlobalUnlock((HGLOBAL) hDIB);
		
		// ����
		return;
	}
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// DIB�Ŀ��
	LONG lWidth = m_nWidth24Bit;
	
	// DIB�ĸ߶�
	LONG lHeight = m_nHeight24Bit;

	// ����С���ع�	
	IDWT(lpDIBBits, lWidth, lHeight, lWidth, lHeight);

	// �������
	::GlobalUnlock((HGLOBAL) hDIB);

	// �ָ����
	EndWaitCursor();
}

//
//*** Ƶ���˲� ***
//
// ************************************************************************
//
//  ͼ��Ƶ���˲������⣺
//
//	ButterworthL()		- ������˹��ͨ�˲���
//	ButterworthH()		- ������˹��ͨ�˲���
//	MutualFilter()		- ����ʽ�����˲���
//	RetrorseFilter()	- ������˹��ͨ�˲��������˲�
//	WienerFilter()		- ��Լ���ָ���ά���˲�
//	PSE_Filter()		- ��Լ���ָ��Ĺ����׾����˲�
//	MinPower()			- ��Լ���ָ�����Сƽ���˲�
//
//*************************************************************************

/*************************************************************************
 *
 * �������ƣ�
 *   ButterworthL()
 *
 * ����:
 *   HDIB	hDIB		- �������DIB
 *	 float  fD			- ��ͨ�˲���ֵ
 *
 * ����ֵ:
 *   void			    - �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ����а�����˹��ͨ�˲�
 *
 ************************************************************************/

void CFFTDlg::ButterworthL(HDIB hDIB, float fD)
{
	// ��ʱ����
	LONG	i;
	LONG	j;

	// ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG	w;
	LONG	h;
	
	int		wp;
	int		hp;

	// ָ��DIB��ָ��
	LPBYTE lpDIB;
	
	// ָ��DIB����ָ��
	LPBYTE lpDIBBits;
	
	// ����DIB
	lpDIB = (LPBYTE) ::GlobalLock((HGLOBAL) hDIB);

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = m_data24Bit;
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// DIB�Ŀ��
	LONG lWidth = m_nWidth24Bit;
	
	// DIB�ĸ߶�
	LONG lHeight = m_nHeight24Bit;

	// ����ͼ��ÿ�е��ֽ���
	LONG lLineBytes = WIDTHBYTES(lWidth * 24);

	// ����ֵ
	w = 1;
	h = 1;
	wp = 0;
	hp = 0;
	
	// ������и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	while(w < lWidth)
	{
		w *= 2;
		wp++;
	}
	
	while(h < lHeight)
	{
		h *= 2;
		hp++;
	}

	// ����Ƶ������
	complex<double> *FD, *TD;
	FD = new complex<double>[w * h * 3];
	TD = new complex<double>[w * h * 3];

	// ��
	for(i = 0; i < h; i++)
	{
		// ��
		for(j = 0; j < 3 * w; j++)
		{
			if(i < lHeight && j < lLineBytes)
			{
				// ��ȡʱ����ֵ
				BYTE Value = *((BYTE *)lpDIBBits + lLineBytes * i + j);
			
				// ʱ��ֵ
				TD[w * i * 3 + j] = complex<double>(Value, 0.0f);
			}
			else
			{
				// ������
				TD[w * i * 3 + j] = complex<double>(0.0f, 0.0f);
			}
		}
	}

	// ����Ƶ�׷��� ʱ��->Ƶ��
	if (Fourier(TD, lWidth, lHeight, FD) == FALSE)
		return;

	// �ͷ��ڴ�
	delete[] TD;

	// ��ǰƵ��
	float fDN;

	// ��ͼ����е�ͨ�˲�
	for(i = 0; i < h; i++)
	{
		for(j = 0; j < w * 3; j++)
		{
			// �������
			int k = (int)(j / 3);
			fDN = (float)sqrt( i * i + k * k);		

			// ���������˹��ͨ�˲������˲�
			FD[i * 3 * w + j] *= complex<double>(1 / (1 + 0.414 * (fDN / fD) * (fDN / fD)), 0.0f);
		}
	}

	// ����Ƶ�׷��� Ƶ��->ʱ��
	if (IFourier(lpDIBBits, lWidth, lHeight, FD) == FALSE)
		return;

	// �������
	::GlobalUnlock((HGLOBAL) hDIB);

	// �ͷ��ڴ�
	delete[] FD;

	// �ָ����
	EndWaitCursor();
}


/*************************************************************************
 *
 * �������ƣ�
 *   ButterworthH()
 *
 * ����:
 *   HDIB	hDIB		- �������DIB
 *	 float  fD			- ��ͨ�˲���ֵ
 *
 * ����ֵ:
 *   void			    - �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ����а�����˹��ͨ�˲�
 *
 ************************************************************************/

void CFFTDlg::ButterworthH(HDIB hDIB, float fD)
{
	// ��ʱ����
	LONG	i;
	LONG	j;

	// ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG	w;
	LONG	h;
	
	int		wp;
	int		hp;

	// ָ��DIB��ָ��
	LPBYTE lpDIB;
	
	// ָ��DIB����ָ��
	LPBYTE lpDIBBits;
	
	// ����DIB
	lpDIB = (LPBYTE) ::GlobalLock((HGLOBAL) hDIB);

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = m_data24Bit;
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// DIB�Ŀ��
	LONG lWidth = m_nWidth24Bit;
	
	// DIB�ĸ߶�
	LONG lHeight = m_nHeight24Bit;

	// ����ͼ��ÿ�е��ֽ���
	LONG lLineBytes = WIDTHBYTES(lWidth * 24);

	// ����ֵ
	w = 1;
	h = 1;
	wp = 0;
	hp = 0;
	
	// ������и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	while(w < lWidth)
	{
		w *= 2;
		wp++;
	}
	
	while(h < lHeight)
	{
		h *= 2;
		hp++;
	}

	// ����Ƶ������
	complex<double> *FD, *TD;
	FD = new complex<double>[w * h * 3];
	TD = new complex<double>[w * h * 3];

	// ��
	for(i = 0; i < h; i++)
	{
		// ��
		for(j = 0; j < 3 * w; j++)
		{
			if(i < lHeight && j < lLineBytes)
			{
				// ��ȡʱ����ֵ
				BYTE Value = *((BYTE *)lpDIBBits + lLineBytes * i + j);
			
				// ʱ��ֵ
				TD[w * i * 3 + j] = complex<double>(Value, 0.0f);
			}
			else
			{
				// ������
				TD[w * i * 3 + j] = complex<double>(0.0f, 0.0f);
			}
		}
	}

	// ����Ƶ�׷��� ʱ��->Ƶ��
	if (Fourier(TD, lWidth, lHeight, FD) == FALSE)
		return;

	// �ͷ��ڴ�
	delete[] TD;

	// ��ǰƵ��
	float fDN;

	// ��ͼ����и�ͨ�˲�
	for(i = 0; i < h; i++)
	{
		for(j = 0; j < w * 3; j++)
		{
			// �������
			int k = (int)(j / 3);
			fDN = (float)sqrt( i * i + k * k);		

			// ���������˹��ͨ�˲������˲�
			FD[i * 3 * w + j] *= complex<double>(1.0 / (1.0 + 0.414 * (fD / fDN) * (fD / fDN) + 0.5), 0.0f);
		}
	}

	// ����Ƶ�׷��� Ƶ��->ʱ��
	if (IFourier(lpDIBBits, lWidth, lHeight, FD) == FALSE)
		return;

	// �������
	::GlobalUnlock((HGLOBAL) hDIB);

	// �ͷ��ڴ�
	delete[] FD;

	// �ָ����
	EndWaitCursor();
}


/*************************************************************************
 *
 * �������ƣ�
 *   MutualFilter()
 *
 * ����:
 *   HDIB	hDIB		- �������DIB
 *	 CRect* pRect		- Ƶ�������������
 *	 int	nSum		- ����������Ŀ
 *
 * ����ֵ:
 *   void			    - �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ����н���ʽ�����˲�
 *
 ************************************************************************/

void CFFTDlg::MutualFilter(HDIB hDIB, CRect* pRect, int nSum)
{
	// ��ʱ����
	LONG	i;
	LONG	j;
	LONG	k;

	// ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG	w;
	LONG	h;
	
	int		wp;
	int		hp;

	// ָ��DIB��ָ��
	LPBYTE lpDIB;
	
	// ָ��DIB����ָ��
	LPBYTE lpDIBBits;
	
	// ����DIB
	lpDIB = (LPBYTE) ::GlobalLock((HGLOBAL) hDIB);

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = m_data24Bit;
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// DIB�Ŀ��
	LONG lWidth = m_nWidth24Bit;
	
	// DIB�ĸ߶�
	LONG lHeight = m_nHeight24Bit;

	// ����ͼ��ÿ�е��ֽ���
	LONG lLineBytes = WIDTHBYTES(lWidth * 24);

	// ����ֵ
	w = 1;
	h = 1;
	wp = 0;
	hp = 0;
	
	// ������и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	while(w < lWidth)
	{
		w *= 2;
		wp++;
	}
	
	while(h < lHeight)
	{
		h *= 2;
		hp++;
	}

	// ����Ƶ������
	complex<double> *FD, *TD;
	FD = new complex<double>[w * h * 3];
	TD = new complex<double>[w * h * 3];

	// ��
	for(i = 0; i < h; i++)
	{
		// ��
		for(j = 0; j < 3 * w; j++)
		{
			if(i < lHeight && j < lLineBytes)
			{
				// ��ȡʱ����ֵ
				BYTE Value = *((BYTE *)lpDIBBits + lLineBytes * i + j);
			
				// ʱ��ֵ
				TD[w * i * 3 + j] = complex<double>(Value, 0.0f);
			}
			else
			{
				// ������
				TD[w * i * 3 + j] = complex<double>(0.0f, 0.0f);
			}
		}
	}

	// ����Ƶ�׷��� ʱ��->Ƶ��
	if (Fourier(TD, lWidth, lHeight, FD) == FALSE)
		return;

	// �ͷ��ڴ�
	delete[] TD;

	// ����ѡ����ʵʩ�����˲�
	for(k = 0; k < nSum; k++)
	{
		// ������Ƶ������ƽ���ڵ�����
		pRect[k].top += (h - lHeight) / 2;
		pRect[k].bottom += (h - lHeight) / 2;
		pRect[k].left *= 3;
		pRect[k].right *= 3;
		pRect[k].left += (w - lWidth) * 3 / 2;
		pRect[k].right += (w - lWidth) * 3 / 2;

		// �ָ����任ǰ������λ��
		if (pRect[k].top < h / 2)
			pRect[k].top += h / 2;
		else
			pRect[k].top -= h / 2;

		if (pRect[k].bottom < h / 2)
			pRect[k].bottom += h / 2;
		else
			pRect[k].bottom -= h / 2;

		if (pRect[k].left < w * 3 /2)
			pRect[k].left += 3 * w / 2;
		else 
			pRect[k].left -= 3 * w / 2;

		if (pRect[k].right < w * 3 /2)
			pRect[k].right += 3 * w / 2;
		else 
			pRect[k].right -= 3 * w / 2;
	
		// �����ѡ������ͬһ����
		if (pRect[k].top < pRect[k].bottom && pRect[k].left < pRect[k].right)
		{
			// ��������д����˲�
			for (i = h - pRect[k].bottom; i < h - pRect[k].top; i++)
			{
				for (j = pRect[k].left * 3; j < pRect[k].right * 3; j++)
					FD[i * w * 3 + j] = complex<double>(0, 0);
			}
		}

		// �����ѡ����Ϊһ����������������
		if (pRect[k].top < pRect[k].bottom && pRect[k].left > pRect[k].right)
		{
			// ��������д����˲�
			for (i = h - pRect[k].bottom; i < h - pRect[k].top; i++)
			{
				// �Զ��������޽����˲�
				for (j = 0; j < pRect[k].right * 3; j++)
					FD[i * w * 3 + j] = complex<double>(0, 0);

				// ��һ�������޽����˲�
				for (j = pRect[k].left * 3; j < h; j++)
					FD[i * w * 3 + j] = complex<double>(0, 0);
			}
		}

		// �����ѡ����Ϊһ���Ļ����������
		if (pRect[k].top > pRect[k].bottom && pRect[k].left < pRect[k].right)
		{
			// ��������д����˲�
			// �����������޽����˲�
			for (i = 0; i < h - pRect[k].top; i++)
			{
				for (j = pRect[k].left * 3; j < pRect[k].right * 3; j++)
					FD[i * w * 3 + j] = complex<double>(0, 0);
			}
			// ��һ������޽����˲�
			for (i = h - pRect[k].bottom; i < h; i++)
			{
				for (j = pRect[k].left * 3; j < pRect[k].right * 3; j++)
					FD[i * w * 3 + j] = complex<double>(0, 0);
			}
		}

		// �����ѡ����Ϊһ����������������
		if (pRect[k].top > pRect[k].bottom && pRect[k].left > pRect[k].right)
		{
			// ��������д����˲�
			for (i = 0; i < h - pRect[k].top; i++)
			{
				// �������޽����˲�
				for (j = 0; j < pRect[k].right * 3; j++)
					FD[i * w * 3 + j] = complex<double>(0, 0);
				
				// �������޽����˲�
				for (j = pRect[k].left * 3; j < h; j++)
					FD[i * w * 3 + j] = complex<double>(0, 0);
			}
			for (i = h - pRect[k].bottom; i < h; i++)
			{
				// �Զ����޽����˲�
				for (j = 0; j < pRect[k].right * 3; j++)
					FD[i * w * 3 + j] = complex<double>(0, 0);

				// ��һ���޽����˲�
				for (j = pRect[k].left * 3; j < h; j++)
					FD[i * w * 3 + j] = complex<double>(0, 0);
			}
		}
	}

	// ����Ƶ�׷��� Ƶ��->ʱ��
	if (IFourier(lpDIBBits, lWidth, lHeight, FD) == FALSE)
		return;

	// �������
	::GlobalUnlock((HGLOBAL) hDIB);

	// �ͷ��ڴ�
	delete[] FD;

	// �ָ����
	EndWaitCursor();
}


/*************************************************************************
 *
 * �������ƣ�
 *   RetrorseFilter()
 *
 * ����:
 *   HDIB	hDIB		- �������DIB
 *	 float  fD			- ��ͨ�˲���ֵ
 *
 * ����ֵ:
 *   void			    - �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ����а�����˹��ͨ�˲������˲�����
 *
 ************************************************************************/

void CFFTDlg::RetrorseFilter(HDIB hDIB, float fD)
{
	// ��ʱ����
	LONG	i;
	LONG	j;

	// ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG	w;
	LONG	h;
	
	int		wp;
	int		hp;

	// ָ��DIB��ָ��
	LPBYTE lpDIB;
	
	// ָ��DIB����ָ��
	LPBYTE lpDIBBits;
	
	// ����DIB
	lpDIB = (LPBYTE) ::GlobalLock((HGLOBAL) hDIB);

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = m_data24Bit;
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// DIB�Ŀ��
	LONG lWidth = m_nWidth24Bit;
	
	// DIB�ĸ߶�
	LONG lHeight = m_nHeight24Bit;

	// ����ͼ��ÿ�е��ֽ���
	LONG lLineBytes = WIDTHBYTES(lWidth * 24);

	// ����ֵ
	w = 1;
	h = 1;
	wp = 0;
	hp = 0;
	
	// ������и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	while(w < lWidth)
	{
		w *= 2;
		wp++;
	}
	
	while(h < lHeight)
	{
		h *= 2;
		hp++;
	}

	// ����Ƶ������
	complex<double> *FD, *TD;
	FD = new complex<double>[w * h * 3];
	TD = new complex<double>[w * h * 3];

	// ��
	for(i = 0; i < h; i++)
	{
		// ��
		for(j = 0; j < 3 * w; j++)
		{
			if(i < lHeight && j < lLineBytes)
			{
				// ��ȡʱ����ֵ
				BYTE Value = *((BYTE *)lpDIBBits + lLineBytes * i + j);
			
				// ʱ��ֵ
				TD[w * i * 3 + j] = complex<double>(Value, 0.0f);
			}
			else
			{
				// ������
				TD[w * i * 3 + j] = complex<double>(0.0f, 0.0f);
			}
		}
	}

	// ����Ƶ�׷��� ʱ��->Ƶ��
	if (Fourier(TD, lWidth, lHeight, FD) == FALSE)
		return;

	// �ͷ��ڴ�
	delete[] TD;

	// ��ǰƵ��
	float fDN;

	// ��ͼ����е�ͨ�˲�
	for(i = 0; i < h; i++)
	{
		for(j = 0; j < w * 3; j++)
		{
			// �������
			int k = (int)(j / 3);
			fDN = (float)sqrt( i * i + k * k);		
			
			float fH = 1 / (1 + 0.414 * (fDN / fD) * (fDN / fD));

			// ���˲��ָ�
			if(fH > 0.25f)
				FD[i * 3 * w + j] /= complex<double>(fH, 0.0f);
			else
				FD[i * 3 * w + j] *= complex<double>(0.2f, 0.0f);
		}
	}

	// ����Ƶ�׷��� Ƶ��->ʱ��
	if (IFourier(lpDIBBits, lWidth, lHeight, FD) == FALSE)
		return;

	// �������
	::GlobalUnlock((HGLOBAL) hDIB);

	// �ͷ��ڴ�
	delete[] FD;

	// �ָ����
	EndWaitCursor();
}


/*************************************************************************
 *
 * �������ƣ�
 *   WienerFilter()
 *
 * ����:
 *   HDIB	hDIB		- �������DIB
 *	 float  fD			- ��ͨ�˲���ֵ
 *
 * ����ֵ:
 *   void			    - �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ�������Լ���ָ���ά���˲�����
 *
 ************************************************************************/

void CFFTDlg::WienerFilter(HDIB hDIB, float fD)
{
	// ��ʱ����
	LONG	i;
	LONG	j;

	// ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG	w;
	LONG	h;
	
	int		wp;
	int		hp;

	// ָ��DIB��ָ��
	LPBYTE lpDIB;
	
	// ָ��DIB����ָ��
	LPBYTE lpDIBBits;
	
	// ����DIB
	lpDIB = (LPBYTE) ::GlobalLock((HGLOBAL) hDIB);

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = m_data24Bit;
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// DIB�Ŀ��
	LONG lWidth = m_nWidth24Bit;
	
	// DIB�ĸ߶�
	LONG lHeight = m_nHeight24Bit;

	// ����ͼ��ÿ�е��ֽ���
	LONG lLineBytes = WIDTHBYTES(lWidth * 24);

	// ����ֵ
	w = 1;
	h = 1;
	wp = 0;
	hp = 0;
	
	// ������и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	while(w < lWidth)
	{
		w *= 2;
		wp++;
	}
	
	while(h < lHeight)
	{
		h *= 2;
		hp++;
	}

	// ����Ƶ������
	complex<double> *FD, *TD;
	FD = new complex<double>[w * h * 3];
	TD = new complex<double>[w * h * 3];

	// ��
	for(i = 0; i < h; i++)
	{
		// ��
		for(j = 0; j < 3 * w; j++)
		{
			if(i < lHeight && j < lLineBytes)
			{
				// ��ȡʱ����ֵ
				BYTE Value = *((BYTE *)lpDIBBits + lLineBytes * i + j);
			
				// ʱ��ֵ
				TD[w * i * 3 + j] = complex<double>(Value, 0.0f);
			}
			else
			{
				// ������
				TD[w * i * 3 + j] = complex<double>(0.0f, 0.0f);
			}
		}
	}

	// ����Ƶ�׷��� ʱ��->Ƶ��
	if (Fourier(TD, lWidth, lHeight, FD) == FALSE)
		return;

	// �ͷ��ڴ�
	delete[] TD;

	// ��ǰƵ��
	float fDN;

	// Ԥ���趨�ľ��鳣��
	float K = 0.05f;

	// ��ͼ����е�ͨ�˲�
	for(i = 0; i < h; i++)
	{
		for(j = 0; j < w * 3; j++)
		{
			// �������
			int k = (int)(j / 3);
			fDN = (float)sqrt( i * i + k * k);		

			float fH = 1 / (1 + 0.414 * (fDN / fD) * (fDN / fD));

			// ά���˲��ָ�
			FD[i * 3 * w + j] *= complex<double>(fH / (fH * fH + K), 0.0f);
		}
	}

	// ����Ƶ�׷��� Ƶ��->ʱ��
	if (IFourier(lpDIBBits, lWidth, lHeight, FD) == FALSE)
		return;

	// �������
	::GlobalUnlock((HGLOBAL) hDIB);

	// �ͷ��ڴ�
	delete[] FD;

	// �ָ����
	EndWaitCursor();
}


/*************************************************************************
 *
 * �������ƣ�
 *   PSE_Filter()
 *
 * ����:
 *   HDIB	hDIB		- �������DIB
 *	 float  fD			- ��ͨ�˲���ֵ
 *
 * ����ֵ:
 *   void			    - �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ����й����׾����˲�
 *
 ************************************************************************/

void CFFTDlg::PSE_Filter(HDIB hDIB, float fD)
{
	// ��ʱ����
	LONG	i;
	LONG	j;

	// ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG	w;
	LONG	h;
	
	int		wp;
	int		hp;

	// ָ��DIB��ָ��
	LPBYTE lpDIB;
	
	// ָ��DIB����ָ��
	LPBYTE lpDIBBits;
	
	// ����DIB
	lpDIB = (LPBYTE) ::GlobalLock((HGLOBAL) hDIB);

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = m_data24Bit;
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// DIB�Ŀ��
	LONG lWidth = m_nWidth24Bit;
	
	// DIB�ĸ߶�
	LONG lHeight = m_nHeight24Bit;

	// ����ͼ��ÿ�е��ֽ���
	LONG lLineBytes = WIDTHBYTES(lWidth * 24);

	// ����ֵ
	w = 1;
	h = 1;
	wp = 0;
	hp = 0;
	
	// ������и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	while(w < lWidth)
	{
		w *= 2;
		wp++;
	}
	
	while(h < lHeight)
	{
		h *= 2;
		hp++;
	}

	// ����Ƶ������
	complex<double> *FD, *TD;
	FD = new complex<double>[w * h * 3];
	TD = new complex<double>[w * h * 3];

	// ��
	for(i = 0; i < h; i++)
	{
		// ��
		for(j = 0; j < 3 * w; j++)
		{
			if(i < lHeight && j < lLineBytes)
			{
				// ��ȡʱ����ֵ
				BYTE Value = *((BYTE *)lpDIBBits + lLineBytes * i + j);
			
				// ʱ��ֵ
				TD[w * i * 3 + j] = complex<double>(Value, 0.0f);
			}
			else
			{
				// ������
				TD[w * i * 3 + j] = complex<double>(0.0f, 0.0f);
			}
		}
	}

	// ����Ƶ�׷��� ʱ��->Ƶ��
	if (Fourier(TD, lWidth, lHeight, FD) == FALSE)
		return;

	// �ͷ��ڴ�
	delete[] TD;

	// ��ǰƵ��
	float fDN;

	// Ԥ���趨�ľ��鳣��
	float K=0.05f;

	// ��ͼ����е�ͨ�˲�
	for(i = 0; i < h; i++)
	{
		for(j = 0; j < w * 3; j++)
		{
			// �������
			int k = (int)(j / 3);
			fDN = (float)sqrt( i * i + k * k);		
			
			float fH = 1 / (1 + 0.414 * (fDN / fD) * (fDN / fD));

			// �����׾����˲�
			FD[i * 3 * w + j] *= complex<double>(1 / sqrt(fH * fH + K), 0.0f);
		}
	}

	// ����Ƶ�׷��� Ƶ��->ʱ��
	if (IFourier(lpDIBBits, lWidth, lHeight, FD) == FALSE)
		return;

	// �������
	::GlobalUnlock((HGLOBAL) hDIB);

	// �ͷ��ڴ�
	delete[] FD;

	// �ָ����
	EndWaitCursor();
}


/*************************************************************************
 *
 * �������ƣ�
 *   MinPower()
 *
 * ����:
 *   HDIB	hDIB		- �������DIB
 *	 float  fD			- ��ͨ�˲���ֵ
 *
 * ����ֵ:
 *   void			    - �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ�������Լ����Сƽ���˲��˲�
 *
 ************************************************************************/

void CFFTDlg::MinPower(HDIB hDIB, float fD)
{
	// ��ʱ����
	LONG	i;
	LONG	j;

	// ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG	w;
	LONG	h;
	
	int		wp;
	int		hp;

	// ָ��DIB��ָ��
	LPBYTE lpDIB;
	
	// ָ��DIB����ָ��
	LPBYTE lpDIBBits;
	
	// ����DIB
	lpDIB = (LPBYTE) ::GlobalLock((HGLOBAL) hDIB);

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = m_data24Bit;
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// DIB�Ŀ��
	LONG lWidth = m_nWidth24Bit;
	
	// DIB�ĸ߶�
	LONG lHeight = m_nHeight24Bit;

	// ����ͼ��ÿ�е��ֽ���
	LONG lLineBytes = WIDTHBYTES(lWidth * 24);

	// ����ֵ
	w = 1;
	h = 1;
	wp = 0;
	hp = 0;
	
	// ������и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	while(w < lWidth)
	{
		w *= 2;
		wp++;
	}
	
	while(h < lHeight)
	{
		h *= 2;
		hp++;
	}

	// ����Ƶ������
	complex<double> *FD, *TD;
	FD = new complex<double>[w * h * 3];
	TD = new complex<double>[w * h * 3];

	// ��
	for(i = 0; i < h; i++)
	{
		// ��
		for(j = 0; j < 3 * w; j++)
		{
			if(i < lHeight && j < lLineBytes)
			{
				// ��ȡʱ����ֵ
				BYTE Value = *((BYTE *)lpDIBBits + lLineBytes * i + j);
			
				// ʱ��ֵ
				TD[w * i * 3 + j] = complex<double>(Value, 0.0f);
			}
			else
			{
				// ������
				TD[w * i * 3 + j] = complex<double>(0.0f, 0.0f);
			}
		}
	}

	// ����Ƶ�׷��� ʱ��->Ƶ��
	if (Fourier(TD, lWidth, lHeight, FD) == FALSE)
	{
		delete [] TD;
		delete [] FD;
		return;
	}

	// �ͷ��ڴ�
	delete[] TD;

	// ��ǰƵ��
	float fDN;

	// Ԥ���趨�ľ��鳣��
	float S = 0.00003f;

	// ��ͼ����е�ͨ�˲�
	for(i = 0; i < h; i++)
	{
		for(j = 0; j < w * 3; j++)
		{
			// �������
			int k = (int)(j / 3);
			fDN = (float)sqrt( i * i + k * k);		

			// ������Qʵ�ֵĸ�ͨ�˲�����ת�ƺ���P(u,v)
			float P = 2 * PI * fDN;
			
			// ����H(u,v)
			float fH = 1 / (1 + 0.414 * (fDN / fD) * (fDN / fD));

			// ��Сƽ���˲��ָ�
			FD[i * 3 * w + j] *= complex<double>(fH / (fH * fH + S * P), 0.0f);
		}
	}

	// ����Ƶ�׷��� Ƶ��->ʱ��
	if (IFourier(lpDIBBits, lWidth, lHeight, FD) == FALSE)
	{
		delete [] FD;
		return;
	}

	// ������� 
	::GlobalUnlock((HGLOBAL) hDIB);

	// �ͷ��ڴ�
	delete[] FD;

	// �ָ����
	EndWaitCursor();
}

//
//*** ��Ϣ��Ӧ ***
//

//ȷ��
void CFFTDlg::OnIdbFftdlgOk()
{
	// TODO: �ڴ���������������
}

//ȡ��
void CFFTDlg::OnIdbFftdlgCancel()
{
	// TODO: �ڴ���������������
}

//FFT
void CFFTDlg::OnIdbFftdlgFft()
{
	// ����DIBFourier()�������и���Ҷ�任
	if (DIBFourier(m_pBitmap) == FALSE)	return;

	//Ƶ��ͼ
	Create32Bitmap(m_data24Bit, 3, m_nWidthSrc, m_nHeightSrc);
	InvalidateRect(m_pPaintRect);
}

// ��ͼ��ʵʩ�˲�
void CFFTDlg::OnIdbFftdlgFilter()
{
	CFreqFilter dlg;
	dlg.m_nMethod = m_nMethod;
	dlg.m_fPower = m_fPower;
	if (dlg.DoModal() == IDOK)
	{
		m_nMethod = dlg.m_nMethod;
		m_fPower = dlg.m_fPower;
	}
	else return;
	if(m_nMethod == 0) return;
	switch (m_nMethod)
	{
	case FILTER_BUTTERWORTHL : ButterworthL(m_data24Bit, m_fPower);     break;
	case FILTER_BUTTERWORTHH : ButterworthH(m_data24Bit, m_fPower);     break;
	case FILTER_RESTRORSE    : RetrorseFilter(m_data24Bit, m_fPower);   break;
	case FILTER_WIENER       : WienerFilter(m_data24Bit, m_fPower);     break;
	case FILTER_PSE          : PSE_Filter(m_data24Bit, m_fPower);       break;
	case FILTER_MINPOWER     : MinPower(m_data24Bit, m_fPower);         break;
	default:break;
	}
	Create32Bitmap(m_data24Bit, 3, m_nWidthSrc, m_nHeightSrc);
	InvalidateRect(m_pPaintRect);
}