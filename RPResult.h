/******************************************************************
�ļ�����		RPResult.h
�����ˣ�		zhaoweiwang
��ע���ڣ�	2013-4-5
������		�������ʵ���ࡣ����Ҫ������ڼ�������б����R-P���ߵķ�������㣬��
��ΪRPResultDlg����ĳ�Ա����RP���߿��ӻ����Ƶ������С�
******************************************************************/

#ifndef __RPRESULT_H__
#define __RPRESULT_H__

#include "DataStruct.h"
#include "Image.h"
#include <fstream>

using namespace std;
/*
R-Pֵ����
*/
struct RPData
{
	float recall;
	float precision;
};

class RPResult
{
public:
	List<RPData> mRPDataList;	// RPֵ�����б�

	float mUsedTime;			// ������ʱ
		
	bool mbIsValid;				// �Ϸ����

public:
	/*
	��������	ComputeRP
	���ܣ�	�Ե�ǰ�Ľ���б��������Ӧ��RP����
	���룺	����1��ԴͼƬ����������������ͼ������Ӧ����𣩣�����2����������б�
	�����	��
	*/
	void ComputeRP(int srcIndex , List<Image*>& resultList);

	/*
	��������	OutputResult
	���ܣ�	�����ǰ�洢�ļ���������ⲿ�ļ���
	���룺	�ⲿĿ���ļ�������
	�����	��
	*/
	void OutputResult(ofstream& outFile);

public:
	RPResult();

};

#endif