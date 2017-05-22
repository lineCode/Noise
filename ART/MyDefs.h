#pragma once

#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <fstream>
using namespace std;

//���޸�TYPE
typedef float                   TYPE;

//һЩ�Զ���:���ڷ���
#define DIRECTION_TOP               1
#define DIRECTION_BOTTOM           -1
#define DIRECTION_LEFT              2
#define DIRECTION_RIGHT            -2
#define DIRECTION_LEFT_TOP          3
#define DIRECTION_LEFT_BOTTOM       4
#define DIRECTION_RIGHT_TOP        -4
#define DIRECTION_RIGHT_BOTTOM     -3
#define DIRECTION_TERMINATE         0

//������ֵ�Ĳ�С�ھ���ʱ����Ϊ�������
#define PRECISION                   1e-12

//����
#define PI                          3.141592653
#define THIS_REAL_MAX               1e+12