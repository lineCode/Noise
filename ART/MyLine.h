#pragma once

//�����ࣺһ������ռ��16

class CMyLine
{
public:
	int                                   m_nRow;  //�������±�(y����)
	int                                   m_nCol;  //�������±�(x����)
	TYPE                               m_dLength;  //���߳���

public:
	CMyLine();
	CMyLine(int row, int col);
	~CMyLine();
};