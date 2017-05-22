#pragma once

//ͼ����

#include "MyLine.h"
#include "MyRay.h"
#include "MyPixel.h"
#include "MyPoint.h"
#include <vector>

//���ߵĶ��壺һ������ռ��16*m_Line.Size()+8
typedef struct MyLines 
{
	vector<CMyLine*>              m_Line;  //����һ��һ�εĽ���
	TYPE                     m_dMeasured;  //��Ӧ����ʵ�ʲ���ֵ
}LINES;

class CMyImage
{
public:
	TYPE                                   m_dWidth;  //ͼ����
	TYPE                                  m_dHeight;  //ͼ��߶�
	TYPE                              m_dWidthSpace;  //�з�����(x)
	TYPE                             m_dHeightSpace;  //�з�����(y)
	int                              m_nWidthSplits;  //�з�������
	int                             m_nHeightSplits;  //�з�������
	int                          m_nQuantityOfPixel;  //���ظ���
	CMyPixel*                          m_PixelArray;  //��������(B)��48*m_nWidthSplits*m_nHeightSplits
	vector<LINES*>                  m_Intersections;  //��������(B)��(16*CMyIntersections::m_Line.Size()+8)*m_Intersections.size()

public:
	CMyImage();
	CMyImage(TYPE Width, TYPE Height, int WidthSplits, int HeightSplits);
	~CMyImage();

public:
	void CreateImage(const CMyImage& image);
	void Assign(const CMyImage& image);
	void CreateImage(TYPE Width, TYPE Height, int WidthSplits, int HeightSplits);
	CMyPixel GetSector(int Row, int Col, int* WhichSector);
	void Iterate(CMyPixel* Pixel, CMyRay* Ray, CMyPoint* Point, int* Direction);
	void Solve(CMyRay* Ray);
	void SolveAll(CMyRay Lines[], long LinesQuantity = 1);
	void ARTSingleLine(int nCurrentLine);
	void ARTSingleStep();
	void ARTSolution(int Steps = 16);
	void SaveResult(char* filepath);
	TYPE FindMax();
};