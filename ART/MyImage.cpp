#include "stdafx.h"
#include "MyImage.h"

CMyImage::CMyImage()
{
	m_dWidth = 0;
	m_dHeight = 0;
	m_dWidthSpace = 0;
	m_dHeightSpace = 0;
	m_nWidthSplits = 0;
	m_nHeightSplits = 0;
	m_nQuantityOfPixel = 0;
	m_PixelArray = NULL;
	//m_Intersections
}

CMyImage::CMyImage(TYPE Width, TYPE Height, int WidthSplits, int HeightSplits)
{
	m_dWidth           = Width;                                    //x����
	m_dHeight          = Height;                                   //y����
	m_nWidthSplits     = WidthSplits;                              //x����
	m_nHeightSplits    = HeightSplits;                             //y����
	m_dWidthSpace      = m_dWidth/m_nWidthSplits;                  //x����
	m_dHeightSpace     = m_dHeight/m_nHeightSplits;                //y����
	m_nQuantityOfPixel = m_nWidthSplits*m_nHeightSplits;           //����
	m_PixelArray       = new CMyPixel[m_nQuantityOfPixel];         //����ռ�
	for (int i = 0; i<m_nHeightSplits; i++)                        //���ؿ鸳ֵ
	{
		for (int j = 0; j<m_nWidthSplits; j++)
		{
			m_PixelArray[j + i*m_nWidthSplits].m_nRow = i;         //y����
			m_PixelArray[j + i*m_nWidthSplits].m_nCol = j;         //x����
			m_PixelArray[j + i*m_nWidthSplits].m_dLeftBottom = CMyPoint(j*m_dWidthSpace, i*m_dHeightSpace);
			m_PixelArray[j + i*m_nWidthSplits].m_dRightTop = CMyPoint((j+1)*m_dWidthSpace, (i+1)*m_dHeightSpace);
			m_PixelArray[j + i*m_nWidthSplits].m_dValue = 0;
		}
	}
}

//���ɵ�ǰ����ĸ���ͼ��
void CMyImage::CreateImage(const CMyImage& image)
{
	m_dWidth           = image.m_dWidth;
	m_dHeight          = image.m_dHeight;
	m_nWidthSplits     = image.m_nWidthSplits;
	m_nHeightSplits    = image.m_nHeightSplits;
	m_dWidthSpace      = image.m_dWidthSpace;
	m_dHeightSpace     = image.m_dHeightSpace;
	m_nQuantityOfPixel = image.m_nQuantityOfPixel;
	m_PixelArray = new CMyPixel[m_nQuantityOfPixel];
	for (long i = 0; i<m_nQuantityOfPixel; i++)
	{
		m_PixelArray[i].m_nRow        = image.m_PixelArray[i].m_nRow;
		m_PixelArray[i].m_nCol        = image.m_PixelArray[i].m_nCol;
		m_PixelArray[i].m_dLeftBottom = image.m_PixelArray[i].m_dLeftBottom;
		m_PixelArray[i].m_dRightTop   = image.m_PixelArray[i].m_dRightTop;
		m_PixelArray[i].m_dValue      = image.m_PixelArray[i].m_dValue;
	}
	for (long i = 0; i<image.m_Intersections.size(); i++)
	{
		LINES* newLINE = new LINES;
		newLINE->m_dMeasured = image.m_Intersections[i]->m_dMeasured;
		m_Intersections.push_back(newLINE);
		for (int j = 0; j<image.m_Intersections[i]->m_Line.size(); j++)
		{
			CMyLine* newMyLine = new CMyLine;
			newMyLine->m_nRow    = image.m_Intersections[i]->m_Line[j]->m_nRow;
			newMyLine->m_nCol    = image.m_Intersections[i]->m_Line[j]->m_nCol;
			newMyLine->m_dLength = image.m_Intersections[i]->m_Line[j]->m_dLength;
			newLINE->m_Line.push_back(newMyLine);
		}
	}
}

//��ֵ���Ե�ǰ������и���
void CMyImage::Assign(const CMyImage& image)
{
	m_dWidth           = image.m_dWidth;
	m_dHeight          = image.m_dHeight;
	m_nWidthSplits     = image.m_nWidthSplits;
	m_nHeightSplits    = image.m_nHeightSplits;
	m_dWidthSpace      = image.m_dWidthSpace;
	m_dHeightSpace     = image.m_dHeightSpace;
	m_nQuantityOfPixel = image.m_nQuantityOfPixel;
	for (long i = 0; i<m_nQuantityOfPixel; i++)
	{
		m_PixelArray[i].m_nRow        = image.m_PixelArray[i].m_nRow;
		m_PixelArray[i].m_nCol        = image.m_PixelArray[i].m_nCol;
		m_PixelArray[i].m_dLeftBottom = image.m_PixelArray[i].m_dLeftBottom;
		m_PixelArray[i].m_dRightTop   = image.m_PixelArray[i].m_dRightTop;
		m_PixelArray[i].m_dValue      = image.m_PixelArray[i].m_dValue;
	}
	for (long i = 0; i<image.m_Intersections.size(); i++)
	{
		m_Intersections[i]->m_dMeasured = image.m_Intersections[i]->m_dMeasured;
		for (int j = 0; j<image.m_Intersections[i]->m_Line.size(); j++)
		{
			//�ر�ע��m_Line��ָ��
			*(m_Intersections[i]->m_Line[j]) = *(image.m_Intersections[i]->m_Line[j]);
		}
	}
}

CMyImage::~CMyImage()
{
	delete [] m_PixelArray;
	for (long i = 0; i<m_Intersections.size(); i++)
	{
		for (long j = 0; j<m_Intersections[i]->m_Line.size(); j++)
		{
			if(m_Intersections[i]->m_Line[j] != NULL)
				delete m_Intersections[i]->m_Line[j];
			m_Intersections[i]->m_Line[j] = NULL;
		}
		m_Intersections[i]->m_Line.~vector();
		delete m_Intersections[i];
	}
	m_Intersections.~vector();
}

//����ͼ��,����͹��캯���ظ�
void CMyImage::CreateImage(TYPE Width, TYPE Height, int WidthSplits, int HeightSplits)
{
	m_dWidth           = Width;                                    //x����
	m_dHeight          = Height;                                   //y����
	m_nWidthSplits     = WidthSplits;                              //x����
	m_nHeightSplits    = HeightSplits;                             //y����
	m_dWidthSpace      = m_dWidth/m_nWidthSplits;                  //x����
	m_dHeightSpace     = m_dHeight/m_nHeightSplits;                //y����
	m_nQuantityOfPixel = m_nWidthSplits*m_nHeightSplits;           //����
	m_PixelArray       = new CMyPixel[m_nQuantityOfPixel];         //����ռ�
	for (int i = 0; i<m_nHeightSplits; i++)                        //���ؿ鸳ֵ
	{
		for (int j = 0; j<m_nWidthSplits; j++)
		{
			m_PixelArray[j + i*m_nWidthSplits].m_nRow = i;         //y����
			m_PixelArray[j + i*m_nWidthSplits].m_nCol = j;         //x����
			m_PixelArray[j + i*m_nWidthSplits].m_dLeftBottom = CMyPoint(j*m_dWidthSpace, i*m_dHeightSpace);
			m_PixelArray[j + i*m_nWidthSplits].m_dRightTop = CMyPoint((j+1)*m_dWidthSpace, (i+1)*m_dHeightSpace);
			m_PixelArray[j + i*m_nWidthSplits].m_dValue = 0;
		}
	}
}

//ȡ��(Row, Col)���ڽ�����,WhichSectorָ�����������ȡʧ��,WhichSector = DIRECTION_TERMINATE.
CMyPixel CMyImage::GetSector(int Row, int Col, int* WhichSector)
{
	switch (*WhichSector)
	{
	case DIRECTION_TOP:
		if (Row <= m_nHeightSplits -2)
		{//������һ������û��Top
			return m_PixelArray[Col + (Row+1)*m_nWidthSplits];
		}
		break;
	case DIRECTION_BOTTOM:
		if (Row >= 1)
		{//������һ������û��Bottom
			return m_PixelArray[Col + (Row-1)*m_nWidthSplits];
		}
		break;
	case DIRECTION_LEFT:
		if (Col >= 1)
		{//�����һ��û��Left
			return m_PixelArray[(Col-1) + Row*m_nWidthSplits];
		}
		break;
	case DIRECTION_RIGHT:
		if (Col <= m_nWidthSplits-2)
		{//���ұ�һ��û��Right
			return m_PixelArray[(Col+1) + Row*m_nWidthSplits];
		}
		break;
	case DIRECTION_LEFT_TOP:
		if (Row <= m_nHeightSplits-2 && Col >= 1)
		{//�����һ�к�������һ��û��Left-Top
			return m_PixelArray[(Col-1) + (Row+1)*m_nWidthSplits];
		}
		break;
	case DIRECTION_LEFT_BOTTOM:
		if (Row >= 1 && Col >= 1)
		{//�����һ�к�������һ��û��Left-Bottom
			return m_PixelArray[(Col-1) + (Row-1)*m_nWidthSplits];
		}
		break;
	case DIRECTION_RIGHT_TOP:
		if (Row <= m_nHeightSplits-2 && Col <= m_nWidthSplits-2)
		{//���ұ�һ�к�������һ��û��Right-Top
			return m_PixelArray[(Col+1) + (Row+1)*m_nWidthSplits];
		}
		break;
	case DIRECTION_RIGHT_BOTTOM:
		if (Row >= 1 && Col <= m_nWidthSplits-2)
		{//���ұ�һ�к�������һ��û��Right-Bottom
				return m_PixelArray[(Col+1) + (Row-1)*m_nWidthSplits];
		}
		break;
	default:
		break;
	}
	*WhichSector = DIRECTION_TERMINATE;//����������µ�����ֹ���������ر���.
	return m_PixelArray[Col + Row*m_nWidthSplits];
}

//��֪Point��ֱ��Ray��Pixel��һ�����㣬�����һ��.Direction��ʾ����ķ�λ.
//�������������⽻�߳�:Pixel-��ʼ�����أ� Ray-���ߣ� Point-���ߴ������صĽ��㣬 Direction-���ߴӺδ�����
void CMyImage::Iterate(CMyPixel* Pixel, CMyRay* Ray, CMyPoint* Point, int* Direction)
{
	TYPE x, y;
	CMyPoint* TempPoint = new CMyPoint;
	if (Ray->GetSlope()>=0)
	{//б�ʷǸ�
		switch (*Direction)
		{
		case DIRECTION_TOP:
			x = Pixel->m_dLeftBottom.x;
			y = Ray->GetValueAxisY(x);
			if (y>Pixel->m_dLeftBottom.y)
			{//�����
				TempPoint->x = x;
				TempPoint->y = y;
				*Direction = DIRECTION_LEFT;
			}
			else if (fabs(y - Pixel->m_dLeftBottom.y)<PRECISION)
			{//���½�
				*TempPoint = Pixel->m_dLeftBottom;
				*Direction = DIRECTION_LEFT_BOTTOM;
			} 
			else
			{//���ױ�
				TempPoint->y = Pixel->m_dLeftBottom.y;
				TempPoint->x = Ray->GetValueAxisX(TempPoint->y);
				*Direction = DIRECTION_BOTTOM;
			}
			break;
		case DIRECTION_BOTTOM:
			x = Pixel->m_dRightTop.x;
			y = Ray->GetValueAxisY(x);
			if (y>Pixel->m_dRightTop.y)
			{//���ϱ�
				TempPoint->y = Pixel->m_dRightTop.y;
				TempPoint->x = Ray->GetValueAxisX(TempPoint->y);
				*Direction = DIRECTION_TOP;
			}
			else if (fabs(y - Pixel->m_dRightTop.y)<PRECISION)
			{//���Ͻ�
				*TempPoint = Pixel->m_dRightTop;
				*Direction = DIRECTION_RIGHT_TOP;
			} 
			else
			{//���ұ�
				TempPoint->x = x;
				TempPoint->y = y;
				*Direction = DIRECTION_RIGHT;
			}
			break;
		case DIRECTION_LEFT:
			y = Pixel->m_dRightTop.y;
			x = Ray->GetValueAxisX(y);
			if (x>Pixel->m_dRightTop.x)
			{//���ұ�
				TempPoint->x = Pixel->m_dRightTop.x;
				TempPoint->y = Ray->GetValueAxisY(TempPoint->x);
				*Direction = DIRECTION_RIGHT;
			}
			else if (fabs(x - Pixel->m_dRightTop.x)<PRECISION)
			{//���Ͻ�
				*TempPoint = Pixel->m_dRightTop;
				*Direction = DIRECTION_RIGHT_TOP;
			} 
			else
			{//������
				TempPoint->x = x;
				TempPoint->y = y;
				*Direction = DIRECTION_TOP;
			}
			break;
		case DIRECTION_RIGHT:
			y = Pixel->m_dLeftBottom.y;
			x = Ray->GetValueAxisX(y);
			if (x<Pixel->m_dLeftBottom.x)
			{//�����
				TempPoint->x = Pixel->m_dLeftBottom.x;
				TempPoint->y = Ray->GetValueAxisY(TempPoint->x);
				*Direction = DIRECTION_LEFT;
			}
			else if (fabs(x - Pixel->m_dLeftBottom.x)<PRECISION)
			{//���½�
				*TempPoint = Pixel->m_dLeftBottom;
				*Direction = DIRECTION_LEFT_BOTTOM;
			} 
			else
			{//���ױ�
				TempPoint->x = x;
				TempPoint->y = y;
				*Direction = DIRECTION_BOTTOM;
			}
			break;
		case DIRECTION_LEFT_TOP:/*�������������
			TempPoint->x = Pixel->m_dLeftBottom.x;
			TempPoint->y = Pixel->m_dRightTop.y;
			*Direction = DIRECTION_LEFT_TOP;*/
			break;
		case DIRECTION_LEFT_BOTTOM://��LEFTһ��
			y = Pixel->m_dRightTop.y;
			x = Ray->GetValueAxisX(y);
			if (x>Pixel->m_dRightTop.x)
			{//���ұ�
				TempPoint->x = Pixel->m_dRightTop.x;
				TempPoint->y = Ray->GetValueAxisY(TempPoint->x);
				*Direction = DIRECTION_RIGHT;
			}
			else if (fabs(x - Pixel->m_dRightTop.x)<PRECISION)
			{//���Ͻ�
				*TempPoint = Pixel->m_dRightTop;
				*Direction = DIRECTION_RIGHT_TOP;
			} 
			else
			{//������
				TempPoint->x = x;
				TempPoint->y = y;
				*Direction = DIRECTION_TOP;
			}
			break;
		case DIRECTION_RIGHT_TOP://��RIGHTһ��
			y = Pixel->m_dLeftBottom.y;
			x = Ray->GetValueAxisX(y);
			if (x<Pixel->m_dLeftBottom.x)
			{//�����
				TempPoint->x = Pixel->m_dLeftBottom.x;
				TempPoint->y = Ray->GetValueAxisY(TempPoint->x);
				*Direction = DIRECTION_LEFT;
			}
			else if (fabs(x - Pixel->m_dLeftBottom.x)<PRECISION)
			{//���½�
				*TempPoint = Pixel->m_dLeftBottom;
				*Direction = DIRECTION_LEFT_BOTTOM;
			} 
			else
			{//���ױ�
				TempPoint->x = x;
				TempPoint->y = y;
				*Direction = DIRECTION_BOTTOM;
			}
			break;
		case DIRECTION_RIGHT_BOTTOM:/*�������������
			TempPoint->x = Pixel->m_dRightTop.x;
			TempPoint->y = Pixel->m_dLeftBottom.y;
			*Direction = DIRECTION_RIGHT_BOTTOM;*/
			break;
		default:
			break;
		}
	}
	else//б��С��0
	{
		switch (*Direction)
		{
		case DIRECTION_TOP:
			x = Pixel->m_dRightTop.x;
			y = Ray->GetValueAxisY(x);
			if (y<Pixel->m_dLeftBottom.y)
			{//���ױ�
				TempPoint->y = Pixel->m_dLeftBottom.y;
				TempPoint->x = Ray->GetValueAxisX(TempPoint->y);
				*Direction = DIRECTION_BOTTOM;
			}
			else if (fabs(y - Pixel->m_dLeftBottom.y)<PRECISION)
			{//���½�
				TempPoint->x = Pixel->m_dRightTop.x;
				TempPoint->y = Pixel->m_dLeftBottom.y;
				*Direction = DIRECTION_RIGHT_BOTTOM;
			} 
			else
			{//���ұ�
				TempPoint->x = x;
				TempPoint->y = y;
				*Direction = DIRECTION_RIGHT;
			}
			break;
		case DIRECTION_BOTTOM:
			x = Pixel->m_dLeftBottom.x;
			y = Ray->GetValueAxisY(x);
			if (y>Pixel->m_dRightTop.y)
			{//���ϱ�
				TempPoint->y = Pixel->m_dRightTop.y;
				TempPoint->x = Ray->GetValueAxisX(TempPoint->y);
				*Direction = DIRECTION_TOP;
			}
			else if (fabs(y - Pixel->m_dRightTop.y)<PRECISION)
			{//���Ͻ�
				TempPoint->x = Pixel->m_dLeftBottom.x;
				TempPoint->y = Pixel->m_dRightTop.y;
				*Direction = DIRECTION_LEFT_TOP;
			} 
			else
			{//�����
				TempPoint->x = x;
				TempPoint->y = y;
				*Direction = DIRECTION_LEFT;
			}
			break;
		case DIRECTION_LEFT:
			y = Pixel->m_dLeftBottom.y;
			x = Ray->GetValueAxisX(y);
			if (x>Pixel->m_dRightTop.x)
			{//���ұ�
				TempPoint->x = Pixel->m_dRightTop.x;
				TempPoint->y = Ray->GetValueAxisY(TempPoint->x);
				*Direction = DIRECTION_RIGHT;
			}
			else if (fabs(x - Pixel->m_dRightTop.x)<PRECISION)
			{//���½�
				TempPoint->x = Pixel->m_dRightTop.x;
				TempPoint->y = Pixel->m_dLeftBottom.y;
				*Direction = DIRECTION_RIGHT_BOTTOM;
			} 
			else
			{//���ױ�
				TempPoint->x = x;
				TempPoint->y = y;
				*Direction = DIRECTION_BOTTOM;
			}
			break;
		case DIRECTION_RIGHT:
			y = Pixel->m_dRightTop.y;
			x = Ray->GetValueAxisX(y);
			if (x<Pixel->m_dLeftBottom.x)
			{//�����
				TempPoint->x = Pixel->m_dLeftBottom.x;
				TempPoint->y = Ray->GetValueAxisY(TempPoint->x);
				*Direction = DIRECTION_LEFT;
			}
			else if (fabs(x - Pixel->m_dLeftBottom.x)<PRECISION)
			{//���Ͻ�
				TempPoint->x = Pixel->m_dLeftBottom.x;
				TempPoint->y = Pixel->m_dRightTop.y;
				*Direction = DIRECTION_LEFT_TOP;
			} 
			else
			{//������
				TempPoint->x = x;
				TempPoint->y = y;
				*Direction = DIRECTION_TOP;
			}
			break;
		case DIRECTION_LEFT_TOP://��LEFTһ��
			y = Pixel->m_dLeftBottom.y;
			x = Ray->GetValueAxisX(y);
			if (x>Pixel->m_dRightTop.x)
			{//���ұ�
				TempPoint->x = Pixel->m_dRightTop.x;
				TempPoint->y = Ray->GetValueAxisY(TempPoint->x);
				*Direction = DIRECTION_RIGHT;
			}
			else if (fabs(x - Pixel->m_dRightTop.x)<PRECISION)
			{//���½�
				TempPoint->x = Pixel->m_dRightTop.x;
				TempPoint->y = Pixel->m_dLeftBottom.y;
				*Direction = DIRECTION_RIGHT_BOTTOM;
			} 
			else
			{//���ױ�
				TempPoint->x = x;
				TempPoint->y = y;
				*Direction = DIRECTION_BOTTOM;
			}
			break;
		case DIRECTION_LEFT_BOTTOM:/*�������������
			*TempPoint = Pixel->m_dLeftBottom;
			*Direction = DIRECTION_LEFT_BOTTOM;*/
			break;
		case DIRECTION_RIGHT_TOP:/*�������������
			*TempPoint = Pixel->m_dRightTop;
			*Direction = DIRECTION_RIGHT_TOP;*/
			break;
		case DIRECTION_RIGHT_BOTTOM://��RIGHTһ��
			y = Pixel->m_dRightTop.y;
			x = Ray->GetValueAxisX(y);
			if (x<Pixel->m_dLeftBottom.x)
			{//�����
				TempPoint->x = Pixel->m_dLeftBottom.x;
				TempPoint->y = Ray->GetValueAxisY(TempPoint->x);
				*Direction = DIRECTION_LEFT;
			}
			else if (fabs(x - Pixel->m_dLeftBottom.x)<PRECISION)
			{//���Ͻ�
				TempPoint->x = Pixel->m_dLeftBottom.x;
				TempPoint->y = Pixel->m_dRightTop.y;
				*Direction = DIRECTION_LEFT_TOP;
			} 
			else
			{//������
				TempPoint->x = x;
				TempPoint->y = y;
				*Direction = DIRECTION_TOP;
			}
			break;
		default:
			break;
		}
	}

	//���һ������
	CMyLine* pNewLine = new CMyLine;
	pNewLine->m_nRow = Pixel->m_nRow;
	pNewLine->m_nCol = Pixel->m_nCol;
	pNewLine->m_dLength = Distance(*Point, *TempPoint);
	m_Intersections.back()->m_Line.push_back(pNewLine);

	//���һЩ��Ϣ
    //cout<<"("<<pNewLine->m_nCol<<","<<pNewLine->m_nRow<<"): "<<pNewLine->m_dLength<<endl;
	//�޸���ز���
	*Pixel = GetSector(Pixel->m_nRow, Pixel->m_nCol, Direction);
	*Point = *TempPoint;
	*Direction = -*Direction;
	delete TempPoint;
}

//�������������⽻�߳�:Ray-����
void CMyImage::Solve(CMyRay* Ray)
{
	CMyPixel *Pixel = new CMyPixel;
	CMyPoint *Point = new CMyPoint;
	int *Direction = new int;
	TYPE x = Ray->GetMeetX();
	TYPE y = Ray->GetMeetY();
	int Index;

	if (x<0)
	{//��������x���ཻ������
		if (y>0 && y<m_dHeight)
		{//���ߴ�ͼ���������
			Point->y = y;
			Index = int(y/m_dHeightSpace);
			*Pixel = m_PixelArray[Index*m_nWidthSplits];
			*Direction = DIRECTION_LEFT;
		}
		else
		{
			delete Pixel;
			delete Point;
			delete Direction;
			return;
		}
	}
	else if (x>m_dWidth)
	{//���ߴ�ͼ���ұ�����
		TYPE Temp = Ray->GetValueAxisY(m_dWidth);
		if (Temp>0 && Temp<m_dHeight)
		{
			Point->x = m_dWidth;
			Point->y = Temp;
			Index = int(Temp/m_dHeightSpace);
			*Pixel = m_PixelArray[m_nWidthSplits-1 + Index*m_nWidthSplits];
			*Direction = DIRECTION_RIGHT;
		}
		else
		{
			delete Pixel;
			delete Point;
			delete Direction;
			return;
		}
	}
	else
	{//���ߴ�ͼ��ױ�����
		Point->x = x;
		Index = int(x/m_dHeightSpace);
		*Pixel = m_PixelArray[Index];
		*Direction = DIRECTION_BOTTOM;
	}

	//�½�һ��LINES�ṹ��Ž��ߺͶ�Ӧ�Ĳ���ֵ
	LINES* pNewStruct = new LINES;
	m_Intersections.push_back(pNewStruct);
	pNewStruct->m_dMeasured = Ray->m_dMeasured;

	while (*Direction != DIRECTION_TERMINATE)
	{//DIRECTION_TERMINATE �������Ƶ�������
		Iterate(Pixel, Ray, Point, Direction);
	}

	delete Pixel;
	delete Point;
	delete Direction;
}

//���һϵ�����������صĽ���
void CMyImage::SolveAll(CMyRay Lines[], long LinesQuantity)
{
	for (long i = 0; i<LinesQuantity; i++)
	{
		Solve(&Lines[i]);
	}
}

//ART������������
void CMyImage::ARTSingleLine(int nCurrentLine)
{
	long i, Row, Col;
	LINES* pLineStruct;
	TYPE Delta, LengthSquare;
	pLineStruct = m_Intersections[nCurrentLine];
	Delta = 0;
	LengthSquare = 0;
	for (i = 0; i<pLineStruct->m_Line.size(); i++)
	{
		Row = pLineStruct->m_Line.at(i)->m_nRow;
		Col = pLineStruct->m_Line.at(i)->m_nCol;
		Delta += pLineStruct->m_Line.at(i)->m_dLength * m_PixelArray[Col+Row*m_nWidthSplits].m_dValue;
		LengthSquare += pLineStruct->m_Line.at(i)->m_dLength * pLineStruct->m_Line.at(i)->m_dLength;//�˴��д��Ľ�
	}
	Delta = (pLineStruct->m_dMeasured - Delta);
	for (i = 0; i<pLineStruct->m_Line.size(); i++)
	{
		Row = pLineStruct->m_Line.at(i)->m_nRow;
		Col = pLineStruct->m_Line.at(i)->m_nCol;
		m_PixelArray[Col+Row*m_nWidthSplits].m_dValue += pLineStruct->m_Line.at(i)->m_dLength * Delta/LengthSquare;
	}
}

//ART������������
void CMyImage::ARTSingleStep()
{
	for (long j = 0; j<m_Intersections.size(); j++)
	{
		ARTSingleLine(j);
	}
}

//ART��������N��
void CMyImage::ARTSolution(int Steps)
{
	int k = 0;
	do 
	{
		ARTSingleStep();
		k++;
	} while (k<Steps);
	//SaveResult("D:\Output.txt");
}

//������
void CMyImage::SaveResult(char* filepath)
{
	//д���ļ�
	ofstream OutputFile(filepath);//���ļ�����д�����ļ������ھʹ�����
	if(!OutputFile)
		return;                           //���ļ�ʧ�����������
	for (int i = 0; i<m_nHeightSplits; i++)
	{
		for (int j = 0; j<m_nWidthSplits; j++)
		{                                 //ʹ�ò��������д�ļ�����
			OutputFile<<m_PixelArray[j + i*m_nWidthSplits].m_dValue<<",";
		}
		OutputFile<<endl;
	}
	OutputFile.close();
}

//�ҵ����ص����ֵ
TYPE CMyImage::FindMax()
{
	TYPE max = -1;
	for (int i = 0; i<m_nWidthSplits; i++)
	{
		for (int j = 0; j<m_nHeightSplits; j++)
		{
			if (m_PixelArray[j + i*m_nWidthSplits].m_dValue>max)
			{
				max = m_PixelArray[j + i*m_nWidthSplits].m_dValue;
			}
		}
	}
	return max;
}