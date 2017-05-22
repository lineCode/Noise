#include "stdafx.h"
#include "MyFuncs.h"
//
//ʵ��
//

//��������֮��ľ���.
TYPE Distance(CMyPoint& PointOne, CMyPoint& PointTwo)
{
	return sqrt((PointOne.x-PointTwo.x)*(PointOne.x-PointTwo.x)+(PointOne.y-PointTwo.y)*(PointOne.y-PointTwo.y));
}

//����һ�������
TYPE random(TYPE start, TYPE end)
{
	return start+(end-start)*rand()/(RAND_MAX + 1.0);
}

//�������߶�Ӧ�Ĳ���ֵ��Ray-����,Big-��Բ,Small-СԲ.
void Measured(CMyRay* Ray, CMyCircle Big, CMyCircle Small)
{
	CMyPoint *PtrBig, *PtrSmall;
	PtrBig = InterSect(*Ray, Big);
	PtrSmall = InterSect(*Ray, Small);
	TYPE DisBig, DisSmall;
	DisBig = Distance(PtrBig[0], PtrBig[1]);
	DisSmall = Distance(PtrSmall[0], PtrSmall[1]);
	delete [] PtrBig;
	delete [] PtrSmall;
	Ray->m_dMeasured = DisBig*Big.m_dDecay+DisSmall*Small.m_dDecay;
}

//����ֱ�ߺ�Բ�Ľ���
CMyPoint* InterSect(CMyRay& Ray, CMyCircle Circle)
{//(1+k*k)x*x+((2c-2b)k-2a)x+(c-b)(c-b)+a*a-r*r = 0
	CMyPoint* Point = new CMyPoint[2];//��������
	TYPE x0, x1, x2, y1, y2, a, b, c, k, r, Delta;
	a = Circle.m_Center.x;
	b = Circle.m_Center.y;
	r = Circle.m_Radius;
	if (fabs(sin(Ray.m_dNormalAngle))<PRECISION)
	{
		x0 = Ray.m_dDistance;
		Delta = r*r-(x0-a)*(x0-a);
		if (Delta>=0)
		{
			y1 = b+sqrt(Delta);
			y2 = b-sqrt(Delta);
			Point[0] = CMyPoint(x0, y1);
			Point[1] = CMyPoint(x0, y2);
		}
	}
	else
	{//���Ǵ�ֱ����
		c = Ray.GetMeetY();
		k = Ray.GetSlope();
		Delta = ((2*c-2*b)*k-2*a)*((2*c-2*b)*k-2*a)-4*(1+k*k)*((c-b)*(c-b)+a*a-r*r);
		if (Delta>=0)
		{
			x1 = (-((2*c-2*b)*k-2*a) + sqrt(Delta))/(2*(1+k*k));
			x2 = (-((2*c-2*b)*k-2*a) - sqrt(Delta))/(2*(1+k*k));
			Point[0] = CMyPoint(x1, Ray.GetValueAxisY(x1));
			Point[1] = CMyPoint(x2, Ray.GetValueAxisY(x2));
		}
	}
	return Point;
}

//����һϵ�����ߣ��Ƕȷ�Χ���� AngleBegin, AngleEnd�����뷶Χ���� DistanceBegin, DistanceEnd��
CMyRay* Demo(TYPE AngleBegin, TYPE AngleEnd, int AngleQuantity, TYPE DistanceBegin, TYPE DistanceEnd, int DistanceQuantity, CMyCircle& Big, CMyCircle& Small)
{
	TYPE AngleSpace = (AngleEnd - AngleBegin)/AngleQuantity;
	TYPE DistanceSpace = (DistanceEnd - DistanceBegin)/DistanceQuantity;
	CMyRay* pRayArray = new CMyRay[AngleQuantity * DistanceQuantity];
	for (int i = 0; i<AngleQuantity; i++)
	{
		for (int j = 0; j<DistanceQuantity; j++)
		{//��΢�Ķ��������i+1,j+1��Ϊi,j���޷��ؽ�
			pRayArray[j + i*DistanceQuantity].m_dNormalAngle = AngleBegin + i*AngleSpace;
			pRayArray[j + i*DistanceQuantity].m_dDistance = DistanceBegin + j*DistanceSpace;
			Measured(&pRayArray[j + i*DistanceQuantity], Big, Small);
		}
	}

	//д���ļ�
	//SaveDemoData(pRayArray, AngleQuantity, DistanceQuantity, "D:\Input.txt");
	return pRayArray;
}

//��ģ�����������д���ı��ĵ�
void SaveDemoData(CMyRay* pRayArray, int AngleQuantity, int DistanceQuantity, char* filepath)
{
	ofstream InputFile(filepath);
	if(InputFile)
	{
		for (int i = 0; i<AngleQuantity; i++)
		{
			for (int j = 0; j<DistanceQuantity; j++)
			{
				InputFile<<pRayArray[j + i*DistanceQuantity].m_dMeasured<<",";
			}
			InputFile<<endl;
		}
		InputFile.close();
	}
}