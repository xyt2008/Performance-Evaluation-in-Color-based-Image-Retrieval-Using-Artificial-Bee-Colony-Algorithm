/******************************************************************
�ļ�����		BeeColonyAlgo.cpp
�����ˣ�		����ΰ���ż�漣��������������
��ע���ڣ�	2013-4-5
������		ʵ���˷�Ⱥ�㷨�����ࡣ���а���ʹ�÷�Ⱥ�㷨�����ݿ�ͼ���б���м��������ض�Ӧ�Ľ����
******************************************************************/

#include "stdafx.h"
#include "BeeColonyAlgo.h"

int mColonySize;
int mDomainSize;

/************************************************************************************
���ڷ�Ⱥ�㷨��ͼ�������ʵ�֣�
1. �ڸ����ķ�Ⱥ��ģ�����������Ӧ�ĳ�ʼ����Ⱥ����Ҫ�Ǹ��ַ�����λ�ã�
2. �����ʼ����Ⱥ�е�ÿ��������Ŀ��ͼ��֮������ƶ�
3. ���ǳ�ʼ��Ⱥ�к��з������������ŷ䣬��õ��˺Ϸ��ĳ�ʼ��Ⱥ������5����
4. ���ǳ�ʼ��Ⱥ�в����з������������ŷ䣬�򷵻�1�������ɷ�Ⱥ��ִ�к�������
5. ȡ���Ⱥ�е����ŷ䣬��������Χ�������������������������ƶ�
6. �������ŷ�������������
************************************************************************************/
void BeeColonyAlgo::PerformRetrieval(Image* pSrcImage , List<Image>& srcImageList , List<Image*>& sortedImageList)
{
	// �����ǰĿ��ͼƬΪ��ʱ����ֱ���˳�
	if (pSrcImage == NULL)
	{
		return;
	}

	// �õ�ͼ���Ĵ�С
	int imageNums = srcImageList.GetListSize();

	// ���λ��������¼��ǰ�ķ�Ⱥ�Ƿ�Ϸ�
	bool isValidColony = false;

	// ��ȫ�ֽ����������õ��Ϸ��ĳ�ʼ��Ⱥ
	int bestBeePosition = 0;
	while(isValidColony == false)
	{
		// ʹ���������������ʼ����Ⱥ��λ��
		for(int i = 0 ; i < mColonySize ; ++i)
		{
			mInitBeesPosition[i] = rand() % srcImageList.GetListSize();
		}

		// �����ʼ��Ⱥ�����ƶ�
		float minDistance = 99999.0f;
		for(int i = 0 ; i < mColonySize ; ++i)
		{
			pSrcImage->Match(srcImageList.GetItemPtr(mInitBeesPosition[i]));
			mInitBeesDistance[i] = pSrcImage->GetDistance();

			if (minDistance > mInitBeesDistance[i])
			{
				minDistance = mInitBeesDistance[i];
				bestBeePosition = mInitBeesPosition[i];
			}
		}

		// ͨ������ֵ���бȽ����жϵ�ǰ�ķ�Ⱥ�Ƿ�Ϸ�
		if (minDistance < mBestBeeThreshold)
		{
			isValidColony = true;
		}
	}

	// ȡ�÷�Ⱥ�е����Ÿ��壬���������Ϸ���������Ҫ�Ǵ������ݿ���βʱ�����������
	int startIndex;
	startIndex = max(0 , bestBeePosition - mDomainSize / 2);
	if (startIndex + mDomainSize > srcImageList.GetListSize())
	{
		startIndex = srcImageList.GetListSize() - mDomainSize;
	}

	// �ڵ�ǰ�������ڽ��н������
	for(int i = startIndex ; i < startIndex + mDomainSize ; ++i)
	{
		srcImageList.GetItemPtr(i)->Match(pSrcImage);
	}

	// �������ڵ����������������
	sortedImageList.EmptyList();
	double minDistance;
	int minImageIndex;
	for (int i = 0 ; i < mDomainSize ; ++i)
	{
		minDistance = 99999.0f;
		minImageIndex = 0;

		// ������������ǰ��ƥ�����ߣ�������С����ͼƬ����������б���Ա����ظ�����
		for (int j = startIndex ; j < startIndex + mDomainSize ; ++j)
		{
			if(srcImageList.GetItemPtr(j)->IsFlagged() == false)
			{
				if (srcImageList.GetItemPtr(j)->GetDistance() < minDistance)
				{
					minImageIndex = j;
					minDistance = srcImageList.GetItemPtr(j)->GetDistance();
				}
			}
		}

		// ����ǰ�������ͼƬ���뵽�б��У����޸�����ֵ����ֵ����ÿ��Match����ʱ���ã�
		Image* pImage = srcImageList.GetItemPtr(minImageIndex);
		sortedImageList.AddItem(pImage);
		srcImageList.GetItemPtr(minImageIndex)->SetFlagged();
	}
}

/************************************************************************************
���캯����ʵ��
************************************************************************************/
BeeColonyAlgo::BeeColonyAlgo(int colonySize, int domainSize , float bestBeeThreshold)
{
	mColonySize = colonySize;
	mDomainSize = domainSize;
	mBestBeeThreshold = bestBeeThreshold;

	mInitBeesPosition = new int[mColonySize];
	mInitBeesDistance = new float[mColonySize];
}

/************************************************************************************
����������ʵ��
************************************************************************************/
BeeColonyAlgo::~BeeColonyAlgo()
{
	SAFE_DELETE_ARRAY(mInitBeesPosition);
	SAFE_DELETE_ARRAY(mInitBeesDistance);
}