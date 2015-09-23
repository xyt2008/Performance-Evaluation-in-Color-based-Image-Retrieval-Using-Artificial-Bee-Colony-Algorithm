/******************************************************************
�ļ�����		RPResult.cpp
�����ˣ�		zhaoweiwang
��ע���ڣ�	2013-4-5
������		�������ʵ���ࡣ����Ҫ������ڼ�������б����R-P���ߵķ�������㣬��
��ΪRPResultDlg����ĳ�Ա����RP���߿��ӻ����Ƶ������С�
******************************************************************/

#include "stdafx.h"
#include "RPResult.h"

/************************************************************************************
�ж�����ָ��������ͼ���Ƿ�����ͬһ��
************************************************************************************/
inline bool IsSameClass(int srcClass , int dstIndex)
{
	return srcClass == (dstIndex / 100);
}

/************************************************************************************
����RP���߲���
���ݷ��صļ���������㱾�μ�����RP���ߡ���Ҫʵ��ͨ����ǰ�����б�����ͳ��������Ŀ��ͼ�������ͼ��
����ͬһ���ͼ�񣩵�������������������Ӧ��RPֵ������RP�����б��ɡ�
Recall = ��ǰ������֮ǰ��Ŀ��ͼ�������ͼ������� / ���ݿ�����Ŀ��ͼ�������ͼ�������
Precision = ��ǰ������֮ǰ��Ŀ��ͼ�������ͼ������� / ��ǰ������ǰ��ͼƬ����
************************************************************************************/
void RPResult::ComputeRP(int srcIndex , List<Image*>& resultList)
{
	mRPDataList.EmptyList();

	RPData rpData;

	int srcClass = srcIndex / 100;

	float relevantImageNums = 0.0f;

	mbIsValid = false;

	for (int i = 0 ; i < resultList.GetListSize() ; ++i)
	{
		rpData.recall = relevantImageNums * 0.01f;

		Image* pImage = *resultList.GetItemPtr(i);
		if (IsSameClass(srcClass , pImage->GetImageIndex()))
		{
			relevantImageNums += 1.0f;
		}

		rpData.precision = relevantImageNums / (float)(i + 1);

		mRPDataList.AddItem(rpData);

		if (rpData.precision > 0.0f)
		{
			mbIsValid = true;
		}
	}
}

/************************************************************************************
�����ǰ����������ⲿ�ļ���
************************************************************************************/
void RPResult::OutputResult(ofstream& outFile)
{
	if (mbIsValid == false)
	{
		return;
	}

	outFile<<"Time: "<<mUsedTime<<endl;
	

	int xDim = 100;
	float* valueList = new float[xDim];
	float xStep = 0.01f;
	float xValue = 0.0f;

	for(int i = 0 ; i < xDim ; ++i)
	{
		// �ڽ�����������е�ǰX���괦�µ�λ��
		int intX0 = 0;
		for(int j = mRPDataList.GetListSize() - 1 ; j >= 0 ; --j)
		{
			if (xValue >= mRPDataList.GetItemPtr(j)->recall)
			{
				intX0 = j;
				break;
			}
		}

		// ʹ��intX0��intX0+1����ֵ����ǰ����λ�ã�ע�⴦�����һ��ֵ�����
		if(intX0 >= mRPDataList.GetListSize() - 1)
		{
			valueList[i] = mRPDataList.GetItemPtr(intX0)->precision;
		}
		else
		{
			float epsilon = (xValue - mRPDataList.GetItemPtr(intX0)->recall) / (mRPDataList.GetItemPtr(intX0 + 1)->recall - mRPDataList.GetItemPtr(intX0)->recall);
			valueList[i] = (1.0f - epsilon) * mRPDataList.GetItemPtr(intX0)->precision + epsilon * mRPDataList.GetItemPtr(intX0 + 1)->precision;
		}

		xValue += xStep;
	}

	outFile<<"RP: "<<xDim<<endl;
	for(int i = 0 ; i < xDim ; ++i)
	{
		outFile<<valueList[i]<<" ";
	}

	outFile<<endl;

	delete []valueList;
}

RPResult::RPResult()
{
	mUsedTime = 0;
}