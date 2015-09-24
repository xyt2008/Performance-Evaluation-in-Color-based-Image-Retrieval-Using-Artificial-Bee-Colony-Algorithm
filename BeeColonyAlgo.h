/******************************************************************
�ļ�����		BeeColonyAlgo.h
�����ˣ�		����ΰ���ż�漣��������������
��ע���ڣ�	2013-4-5
������		ʵ���˷�Ⱥ�㷨�����ࡣ���а���ʹ�÷�Ⱥ�㷨�����ݿ�ͼ���б���м��������ض�Ӧ�Ľ����
******************************************************************/

#ifndef __BEECOLONYALGO_H__
#define __BEECOLONYALGO_H__

#include "Image.h"
#include "DataStruct.h"

class BeeColonyAlgo
{
private:
	int mColonySize;			// ��Ⱥ�Ĺ�ģ
	int mDomainSize;			// ����������Ӧ������Ĵ�С
	float* mInitBeesDistance;	// ��ʼ����Ⱥ�ľ����б�

	float mBestBeeThreshold;	// ���ŷ�����Ӧ�����ƶ���ֵ��������С�ڴ�ֵ˵���õ��˽Ϻõķ���壩
	int* mInitBeesPosition;		// ��ʼ��Ⱥ����Ӧ��ȫ��λ���б�
	
public:
	/*
	��������	PerformRetrieval
	���ܣ�	ʵ�ֶ�ָ����Ŀ��ͼƬ�ڸ�����ͼ����б��Ͻ��м�������������������ص���Ӧ�������б���
	���룺	����1��ָ����Ŀ��ͼƬ��ָ�룻����2��Ŀ��ͼ�����б�����3�����ؽ����ͼ��ָ���б�
	�����	��
	*/
	void PerformRetrieval(Image* pSrcImage , List<Image>& srcImageList , List<Image*>& sortedImageList);

public:
	BeeColonyAlgo(int colonySize = 50 , int domainSize = 200 , float bestBeeThreshold = 0.1f);
	~BeeColonyAlgo();
};

#endif