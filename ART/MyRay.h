#pragma once

//�����ࣺһ������ռ��24

class CMyRay
{
public:
	TYPE                             m_dNormalAngle;  //����н�
	TYPE                                m_dDistance;  //��ԭ�����
	TYPE                                m_dMeasured;  //ʵ�ʲ���ֵ

public:
	CMyRay();
	CMyRay(TYPE NormalAngle, TYPE Distance);
	CMyRay(TYPE NormalAngle, TYPE Distance, TYPE Measured);
	~CMyRay();
	TYPE GetSlope();               //���б��
	TYPE GetMeetX();               //�����x�ύ��
	TYPE GetMeetY();               //�����y�ύ��
	TYPE GetValueAxisY(TYPE x);    //���x��Ӧ��yֵ
	TYPE GetValueAxisX(TYPE y);    //���y��Ӧ��xֵ
};