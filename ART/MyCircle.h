#pragma once

//Բ�࣬��������ģ������

class CMyCircle
{
public:
	CMyPoint                      m_Center;  //Բ��
	TYPE                          m_Radius;  //�뾶
	TYPE                          m_dDecay;  //˥��ϵ��

public:
	CMyCircle();
	CMyCircle(TYPE Radius);
	CMyCircle(CMyPoint Center, TYPE Radius);
	CMyCircle(CMyPoint Center, TYPE Radius, TYPE Decay);
	~CMyCircle();

	void Create(CMyPoint Center, TYPE Radius, TYPE Decay);
};