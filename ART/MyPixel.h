#pragma once

//�����ࣺһ������ռ��48

class CMyPixel
{
public:
	int                                   m_nRow;  //����������(y����)
	int                                   m_nCol;  //����������(x����)
	CMyPoint                       m_dLeftBottom;  //���½�����
	CMyPoint                         m_dRightTop;  //���Ͻ�����
	TYPE                                m_dValue;  //������ֵ

public:
	CMyPixel();
	CMyPixel(CMyPoint LeftBottom, CMyPoint RightTop);
	CMyPixel(int Row, int Col, CMyPoint LeftBottom, CMyPoint RightTop);
	~CMyPixel();
};