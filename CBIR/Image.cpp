/******************************************************************
�ļ�����		Image.cpp
�����ˣ�		����ΰ���ż�漣��������������
��ע���ڣ�	2013-4-5
������		��Ҫͼ�����ʵ�֡�������ʵ���˵���ͼ������Ӧ�Ĵ��������������ͼ��ļ��ء�����ֵ����ȡ��
������ֵ��ͼ��ƥ��ȡ�
******************************************************************/
#include "stdafx.h"
#include "Image.h"
#include "SOIL.h"
#include "FuncsLib.h"
#include "iostream"

/************************************************************************************
����ͼ��
ͨ����ǰͼƬ���������õ����ԣ������ơ���ַ�ȼ�����Ӧ��ͼ������
1. ʹ��SOIL��������ͼ���ڴ��У��������еľ������ݴ洢��mpData���Թ�������ȡʹ�ã�
2. ʹ��SOIL��������ͼ���Դ��У�����¼��Ӧ��OpenGL�������Ա��ڽ��л���ʱʹ�ã�
************************************************************************************/
void Image::LoadImage()
{
	if (strlen(mName) <= 0)
	{
		return;
	}

	char imageFullName[MAX_PATH];
	sprintf_s(imageFullName , sizeof(char) * MAX_PATH , "%s/%s" , mPath , mName);

	int imageChannel;
	mpData = SOIL_load_image(imageFullName , &mWidth , &mHeight , &imageChannel , SOIL_LOAD_RGB);
	if(mpData == NULL)
	{
		MessageBoxA(NULL , "Fail to load the image!" , "Error" , MB_OK);
	}

	mGLIndex = SOIL_load_OGL_texture(imageFullName , SOIL_LOAD_AUTO , SOIL_CREATE_NEW_ID , 
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	if(mGLIndex == 0)
	{
		MessageBoxA(NULL , "Fail to load the image!" , "Error" , MB_OK);
	}
}

/************************************************************************************
����ͼ�����Ϻ�����ͬ��ֻ�������ﲻ���õ�ǰӦ�ó�������·�����������ⲿ��ȫ�ֵľ���·����
ͨ����ǰͼƬ���������õ����ԣ������ơ���ַ�ȼ�����Ӧ��ͼ������
1. ʹ��SOIL��������ͼ���ڴ��У��������еľ������ݴ洢��mpData���Թ�������ȡʹ�ã�
2. ʹ��SOIL��������ͼ���Դ��У�����¼��Ӧ��OpenGL�������Ա��ڽ��л���ʱʹ�ã�
************************************************************************************/
void Image::LoadOutImage()
{
	if (strlen(mName) <= 0)
	{
		return;
	}

	char imageFullName[MAX_PATH];
	sprintf_s(imageFullName , sizeof(char) * MAX_PATH , "%s" , mName);

	int imageChannel;
	mpData = SOIL_load_image(imageFullName , &mWidth , &mHeight , &imageChannel , SOIL_LOAD_RGB);
	if(mpData == NULL)
	{
		MessageBoxA(NULL , "Fail to load the image!" , "Error" , MB_OK);
	}

	mGLIndex = SOIL_load_OGL_texture(imageFullName , SOIL_LOAD_AUTO , SOIL_CREATE_NEW_ID , 
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	if(mGLIndex == 0)
	{
		MessageBoxA(NULL , "Fail to load the image!" , "Error" , MB_OK);
	}
}

/************************************************************************************
ͼ�������ֵ��ȡ��
��Ҫ����������Ҫ������
1. ��ȡ��ɫֱ��ͼ����ֵ
2. ��ȡ��ɫ������ֵ
3. ��ȡ��ɫ������ֵ
4. ��λ�������ֶ���������ֵ�������ݸ��Ե�Ȩ�ض��õ����յ��ۺ�����ֵ
5. ���������Ϣ��release�汾�ޣ�
************************************************************************************/
void Image::ExtractDescriptor()
{
	if (mpData)
	{
		// ��ȡͼƬ�ĻҶ�ֱ��ͼ����ֵ
		ExtractHistogram();

		// ��ȡͼƬ����ɫ������ֵ
		ExtractEntropy();

		// ��ȡͼƬ����ɫ������ֵ
		ExtractMoment();

		// ���ݲ�ͬ����ֵ����Ӧ��Ȩ������λ������������ֵ
		Normalize();

		// ���ͼƬ������ֵ������Ϣ
		DebugOutputDescriptor();
	}
}

/************************************************************************************
������ֵ��RGB�ռ�ת����HSV�ռ�
�������ʵ�ֿ��Բο���https://zh.wikipedia.org/wiki/HSL%E5%92%8CHSV%E8%89%B2%E5%BD%A9%E7%A9%BA%E9%97%B4#.E4.BB.8E_RGB_.E5.88.B0_HSL_.E6.88.96_HSV_.E7.9A.84.E8.BD.AC.E6.8D.A2
************************************************************************************/
HSVValue Image::RGB2HSV(RGBValue& rgb)
{
	HSVValue hsv;
	float r , g , b;
	r = (float)rgb.r / 255.0f;
	g = (float)rgb.g / 255.0f;
	b = (float)rgb.b / 255.0f;

	float fMin , fMax;
	fMin = min(min(r , g) , b);
	fMax = max(max(r , g) , b);

	// �������е�H
	if (r == g && r == b)	// max == min
	{
		hsv.h = 0.0f;
	}
	else
	{
		if (r > g && r > b)	// max == r
		{
			if (g >= b)		// max == r && g >= b
			{
				hsv.h = 60.0f * (g - b) / (fMax - fMin);
			}
			else			// max == r && g < b
			{
				hsv.h = 60.0f * (g - b) / (fMax - fMin) + 360.0f;
			}
		}
		else
		{
			if (g > r && g > b)		// max == g
			{
				hsv.h = 60.0f * (b - r) / (fMax - fMin) + 120.0f;
			}
			else					// max == b
			{
				hsv.h = 60.0f * (r - g) * (fMax - fMin) + 240.0f;
			}
		}
	}

	// �������е�S
	hsv.s = (fMax <= 0.0f) ? 0.0f : 1.0f - fMin / fMax;

	// �������е�V
	hsv.v = fMax;

	return hsv;
	
}

/************************************************************************************
��ȡͼ���ֱ��ͼ������
����256ά�ȵ���ɫֱ��ͼ��������洢��Image�ĳ�Ա����mHistogram���Ա�ʹ�á�
����Ҫ����ԭ��
����ͼ���е�ÿ�����أ��õ����Ӧ������ͨ���ϵ���ɫֵ��ͨ��RGB�ռ��е���ɫֵ�����Ӧ����ɫ�ף�
������Ӧ����ɫ�����顣�������ɫ�������е��������ɵõ����յ�ֱ��ͼ����ֵ��
************************************************************************************/
void Image::ExtractHistogram()
{
	ZeroMemory(mHistogram , sizeof(double) * 256);

	unsigned char r, g, b;

	int Lumi = 0; 

	for (int i = 0 ; i < mHeight ; ++i)
	{
		for (int j = 0 ; j < mWidth ; ++j)
		{
			int baseIndex = (i * mWidth + j) * 3;
			r = mpData[baseIndex + 0];
			g = mpData[baseIndex + 1];
			b = mpData[baseIndex + 2];

			// �õ���ǰ������HSV�ռ��µ�ֵ
			HSVValue hsv = RGB2HSV(RGBValue(r , g , b));
			int gradeH = 0 , gradeS = 0 , gradeV = 0;

			// ����Hͨ��������������Ϊ16��
			if(hsv.h <= 15.0f || hsv.h > 345.0f)
			{
				gradeH = 0;
			}
			if(hsv.h <= 25.0f && hsv.h > 15.0f)
			{
				gradeH = 1;
			}
			if(hsv.h <= 45.0f && hsv.h > 25.0f)
			{
				gradeH = 2;
			}
			if(hsv.h <= 55.0f && hsv.h > 45.0f)
			{
				gradeH = 3;
			}
			if(hsv.h <= 80.0f && hsv.h > 55.0f)
			{
				gradeH = 4;
			}
			if(hsv.h <= 108.0f && hsv.h > 80.0f)
			{
				gradeH = 5;
			}
			if(hsv.h <= 140.0f && hsv.h > 108.0f)
			{
				gradeH = 6;
			}
			if(hsv.h <= 165.0f && hsv.h > 140.0f)
			{
				gradeH = 7;
			}
			if(hsv.h <= 190.0f && hsv.h > 165.0f)
			{
				gradeH = 8;
			}
			if(hsv.h <= 220.0f && hsv.h > 190.0f)
			{
				gradeH = 9;
			}
			if(hsv.h <= 255.0f && hsv.h > 220.0f)
			{
				gradeH = 10;
			}
			if(hsv.h <= 275.0f && hsv.h > 255.0f)
			{
				gradeH = 11;
			}
			if(hsv.h <= 290.0f && hsv.h > 275.0f)
			{
				gradeH = 12;
			}
			if(hsv.h <= 316.0f && hsv.h > 290.0f)
			{
				gradeH = 13;
			}
			if(hsv.h <= 330.0f && hsv.h > 316.0f)
			{
				gradeH = 14;
			}
			if(hsv.h <= 345.0f && hsv.h > 330.0f)
			{
				gradeH = 15;
			}

			// ����Sͨ��������������Ϊ4��
			if(hsv.s <= 0.15 && hsv.s > 0.0f)
			{
				gradeS = 0;
			}
			if(hsv.s <= 0.4 && hsv.s > 0.15f)
			{
				gradeS = 1;
			}
			if(hsv.s <= 0.75 && hsv.s > 0.4f)
			{
				gradeS = 2;
			}
			if(hsv.s <= 1.0 && hsv.s > 0.75f)
			{
				gradeS = 3;
			}

			// ����Vͨ��������������Ϊ4��
			if(hsv.v <= 0.15 && hsv.v > 0.0f)
			{
				gradeV = 0;
			}
			if(hsv.v <= 0.4 && hsv.v > 0.15f)
			{
				gradeV = 1;
			}
			if(hsv.v <= 0.75 && hsv.v > 0.4f)
			{
				gradeV = 2;
			}
			if(hsv.v <= 1.0 && hsv.v > 0.75f)
			{
				gradeV = 3;
			}

			// ��HSV��һ��
			Lumi = gradeH * 16 + gradeS * 4 + gradeV;

			mHistogram[min(Lumi , 255)] += 1.0f;
		}
	}
}

/************************************************************************************
��ȡͼ�����ɫ��������
����3ά�ȵ���ɫ��ֵ��������洢��Image�ĳ�Ա����mEntropy���Ա�ʹ��
����ÿ�������ϵľ��������Ҫ���������Ե��ر仯��
��������ݿ��Բμ���Ӧ�����ġ�
************************************************************************************/
void Image::ExtractEntropy()
{
	unsigned char r, g, b;

	long rStat[256] = {0}, gStat[256] = {0}, bStat[256] = {0};
	double tempr=0,tempg=0,tempb=0;
	for (int i = 0 ; i < mHeight ; ++i)
	{
		for (int j = 0 ; j < mWidth ; ++j)
		{
			int baseIndex = (i * mWidth + j) * 3;
			r = mpData[baseIndex + 0];
			g = mpData[baseIndex + 1];
			b = mpData[baseIndex + 2];

			rStat[r]++;
			gStat[g]++;
			bStat[b]++;
		}
	}

	for (int i = 0; i < 3 ; ++i)
	{
		mEntropy[i] = 0;
	}

	double sizeEpsilon = 1.0f / (float)(mWidth * mHeight);
	double entropyEpsilon = 1.0f / log(256.0f);
	for (int i = 0 ; i < 256 ; ++i)
	{
		tempr = rStat[i] * sizeEpsilon;
		if (tempr > 0)
		{
			mEntropy[0] += -tempr * (log(tempr) * entropyEpsilon);
		}

		tempg = gStat[i] * sizeEpsilon;
		if (tempg > 0)
		{
			mEntropy[1] += -tempg * (log(tempg) * entropyEpsilon);
		}

		tempb = bStat[i] * sizeEpsilon;
		if (tempb > 0)
		{
			mEntropy[2] += -tempb * (log(tempb) * entropyEpsilon);
		}
	}
}

/************************************************************************************
��ȡͼ�����ɫ������ֵ��
����9ά�ȵ���ɫ�أ�������洢��Image�ĳ�Ա����mEntropy���Ա�ʹ��
��������ݿ��Բμ���Ӧ�����ġ�
************************************************************************************/
void Image::ExtractMoment()
{
	long cr = 0 , cg = 0 , cb = 0;
	unsigned char r , g , b;
	double rMean , gMean , bMean;

	for (int i = 0 ; i < mHeight ; ++i)
	{
		for (int j = 0 ; j < mWidth ; ++j)
		{
			int baseIndex = (i * mWidth + j) * 3;
			r = mpData[baseIndex + 0];
			g = mpData[baseIndex + 1];
			b = mpData[baseIndex + 2];

			cr += (int)r;
			cg += (int)g;
			cb += (int)b;
		}
	}

	double sizeEpsilon = 1.0 / (mWidth * mHeight);
	mMoment[0] = rMean = cr * sizeEpsilon;
	mMoment[1] = gMean = cg * sizeEpsilon;
	mMoment[2] = bMean = cb * sizeEpsilon;

	double cr2 = 0 , cg2 = 0 , cb2 = 0 , cr3 = 0 , cg3 = 0 , cb3 = 0;
	for (int i = 0 ; i < mHeight ; ++i)
	{
		for (int j = 0 ; j < mWidth ; ++j)
		{
			int baseIndex = (i * mWidth + j) * 3;
			r = mpData[baseIndex + 0];
			g = mpData[baseIndex + 1];
			b = mpData[baseIndex + 2];

			cr2 += ((double)r - (double)rMean) * ((double)r - (double)rMean);
			cr3 += fabs(((double)r - (double)rMean)) * ((double)r - (double)rMean) * ((double)r - (double)rMean);
			cg2 += ((double)g - (double)gMean) * ((double)g - (double)gMean);
			cg3 += fabs(((double)g - (double)gMean)) * ((double)g - (double)gMean) * ((double)g - (double)gMean);
			cb2 += ((double)b - (double)bMean) * ((double)b - (double)bMean);
			cb3 += fabs(((double)b - (double)bMean)) * ((double)b - (double)bMean) * ((double)b - (double)bMean);
		}
	}
	mMoment[3] = sqrt(cr2 * sizeEpsilon);
	mMoment[4] = sqrt(cg2 * sizeEpsilon);
	mMoment[5] = sqrt(cb2 * sizeEpsilon);
	mMoment[6] = pow(cr3 * sizeEpsilon , 0.33333333333333333);
	mMoment[7] = pow(cg3 * sizeEpsilon , 0.33333333333333333);
	mMoment[8] = pow(cb3 * sizeEpsilon , 0.33333333333333333);
}

/************************************************************************************
��λ����������
�Դ�����������������䵥λ����һ���ض���Ȩ��ֵ�ϡ�
��Ҫ������
1. �������е�����ֵ�����
2. ���ݵõ�������ֵ�������䵥λ��
3. ����λ���������������ŵ�Ȩ�ؿռ��ϼ��� 
************************************************************************************/
inline void NormalizeDescriptor(int dim , double* descValue , double weight)
{
	double total = 0;
	for (int i = 0 ; i < dim ; ++i)
	{
		total += descValue[i];
	}

	double scaleEpsilon = weight / total;
	for (int i = 0 ; i < dim ; ++i)
	{
		descValue[i] *= scaleEpsilon;
	}
}

/************************************************************************************
��λ�������������������������õ����յ��ۺ�����������
����Ȩ��ֵ�������������������е�λ������
************************************************************************************/
void Image::Normalize()
{
	NormalizeDescriptor(256 , mHistogram , mWeight[0]);
	NormalizeDescriptor(3 , mEntropy , mWeight[1]);
	NormalizeDescriptor(9 , mMoment , mWeight[2]);
}

/************************************************************************************
������Ϣ�����
************************************************************************************/
void Image::DebugOutputDescriptor()
{
	// �����Ӧ��ֱ��ͼ��Ϣ
	TRACE("Histogram: ");
	for( int i = 0 ; i < 256 ; ++i)
	{
		TRACE("%f " , mHistogram[i]);
	}
	TRACE("\n");

	// �����Ӧ����ɫ��ֵ��Ϣ
	TRACE("Entropy: ");
	for( int i = 0 ; i < 3 ; ++i)
	{
		TRACE("%f " , mEntropy[i]);
	}
	TRACE("\n");

	// �����Ӧ����ɫ����Ϣ
	TRACE("Moment: ");
	for( int i = 0 ; i < 9 ; ++i)
	{
		TRACE("%f " , mMoment[i]);
	}
	TRACE("\n");
}

/************************************************************************************
��ȡ��ǰͼ��Ĵ�С��Ϣ
************************************************************************************/
int Image::GetWidth()
{
	return mWidth;
}

int Image::GetHeight()
{
	return mHeight;
}

/************************************************************************************
��ʼ��ͼ��
��Ҫͨ����������ʼ��ͼ������ơ����ݿ�����
************************************************************************************/
void Image::InitImage(const char* pName , int index)
{
	strcpy_s(mName , pName);
	mIndex = index;
}

/************************************************************************************
OpenGL�����Ļ�ȡ
���ص�ǰͼ����OpenGL�豸�е�����
************************************************************************************/
GLuint Image::GetGLIndex()
{
	return mGLIndex;
}

/************************************************************************************
ͼ��ƥ�䡣
����ǰ��ͼ�����ⲿ�����ͼ����о���ƥ�䣬����Ҫ����ͼ������Ѿ��������������й���λ�������Ļ�����
���С����㷽��Ҳ��Ϊ�򵥣�
ֱ�Ӽ��㲢�ۼ�����ͼ���������������ά���ϵ�ŷ�Ͼ��뼴�ɣ������Դ˾�����Ϊ���յĲ�ͬ��(��
1.0 - distance����ȡ���ƶȣ����˴�����õ������ƶ�����һ���ļ�������������Ļ�����
************************************************************************************/
void Image::Match(Image* pImage)
{
	mIsFlagged = false;
	mDistance = 0;

	// �ۻ��Ҷ�ͼ���ֵ����ƶ�
	for(int i = 0 ; i < 256 ; ++i)
	{
		mDistance += pow(mHistogram[i] - pImage->mHistogram[i] , 2.0);
	}

	// �ۻ���ɫ�ز��ֵ����ƶ�
	for (int i = 0 ; i < 3 ; ++i)
	{
		mDistance += pow(mEntropy[i] - pImage->mEntropy[i] , 2.0);
	}

	// �ۻ���ɫ�ز��ֵ����ƶ�
	for (int i = 0 ; i < 9 ; ++i)
	{
		mDistance += pow(mMoment[i] - pImage->mMoment[i] , 2.0);
	}

	if (mDistance > 0)
	{
		mDistance = sqrt(mDistance);
	}
}

/************************************************************************************
��ȡͼ����롣
������һ��ƥ��������õ�ͼƬ֮��ľ��루�����ƶȣ�
************************************************************************************/
double Image::GetDistance()
{
	return mDistance;
}

/************************************************************************************
����ͼ��Ĳ������
************************************************************************************/
void Image::SetFlagged()
{
	mIsFlagged = true;
}

/************************************************************************************
��ȡͼ��Ĳ������
************************************************************************************/
bool Image::IsFlagged()
{
	return mIsFlagged;
}

/************************************************************************************
��ȡͼ������ݿ�����
************************************************************************************/
int Image::GetImageIndex()
{
	return mIndex;
}

/************************************************************************************
��ȡͼ��ָ��ά���ϵ�ֱ��ͼ����ֵ
************************************************************************************/
float Image::GetHistogramValue(int index)
{
	return mHistogram[index];
}

/************************************************************************************
��ȡͼ��ָ��ά���ϵ���ɫ��ֵ
************************************************************************************/
float Image::GetEntropyValue(int index)
{
	return mEntropy[index];
}

/************************************************************************************
��ȡͼ��ָ��ά���ϵ���ɫ��ֵ
************************************************************************************/
float Image::GetMomentValue(int index)
{
	return mMoment[index];
}

Image::Image()
{
	mpData = NULL;
	sprintf_s(mPath , sizeof(char) * MAX_PATH , "data/image.orig");

	mWeight[0] = 0.3;
	mWeight[1] = 0.3;
	mWeight[2] = 1.0 - mWeight[0] - mWeight[1];

	mDistance = 0;
	mIsFlagged = true;

	mIndex = 0;
}

Image::~Image()
{
	SAFE_DELETE_ARRAY(mpData);
}