#ifndef SPACE_FILTER_H
#define SPACE_FILTER_H

//�ռ��˲�����

class CSpaceFilter
{
public:
	CSpaceFilter();
	~CSpaceFilter();

protected://������Ա
	int                                     m_nSize;          //�˲���S�Ĵ�С
	int                                      m_nRow;          //�˲�������y����
	int                                      m_nCol;          //�˲�������x����
	int                                   m_nLength;          //�˲�������
	int                               m_nSizeIsEven;          //����-0��ż��-1
	int*                                   m_Sector;          //�˲���

public://��������
	BOOL Create(BYTE* dataSrc, BITMAP bmpInfo, int curChannel, int row, int col, int size);
	                                                          //����һ���˲���
	int FindMax();                                            //S�е����Ҷ�ֵ
	int FindMin();                                            //S�е���С�Ҷ�ֵ
	int FindMed();                                            //S�еĻҶ�ֵ��ֵ
	int Visit(int x, int y);                                  //����x��,y�д�ֵ

private:

};

#endif