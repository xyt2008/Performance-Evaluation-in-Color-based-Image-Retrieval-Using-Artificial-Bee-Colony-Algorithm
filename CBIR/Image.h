/******************************************************************
�ļ�����		Image.h
�����ˣ�		����ΰ���ż�漣��������������
��ע���ڣ�	2013-4-5
������		��Ҫͼ�����ʵ�֡�������ʵ���˵���ͼ������Ӧ�Ĵ��������������ͼ��ļ��ء�����ֵ����ȡ��
������ֵ��ͼ��ƥ��ȡ�
******************************************************************/
#ifndef __IMAGEPARSER_H__
#define __IMAGEPARSER_H__
#include <GL/glew.h>

struct RGBValue
{
	unsigned char r , g , b;

	RGBValue()
	{
		r = 0;
		g = 0; 
		b = 0;
	}

	RGBValue(unsigned char aa , unsigned char bb , unsigned char cc)
	{
		r = aa;
		g = bb;
		b = cc;
	}
};

struct HSVValue
{
	float h , s , v;

	HSVValue()
	{
		h = 0.0f;
		s = 0.0f;
		v = 0.0f;
	}

	HSVValue(float a , float b , float c)
	{
		h = a;
		s = b;
		v = c;
	}
};

class Image
{
private:
	char mPath[MAX_PATH];	// ͼ������·��
	char mName[MAX_PATH];	// ͼ����������
	unsigned char* mpData;	// ͼ�����ľ������ݿռ�
	int mWidth;				// ͼ�����Ŀ�͸�
	int mHeight;	
	int mIndex;				// ͼ���������Ӧ�����ݿ��е�����

	GLuint mGLIndex;		// ͼ���������Ӧ��OpenGL��ͼ����������Ⱦʱʹ��

	double mHistogram[256];	// ֱ��ͼ���������ռ�
	double mEntropy[3];		// ��ɫ�����������ռ�
	double mMoment[9];		// ��ɫ�����������ռ�

	double mWeight[3];		// ��������ֵ�����յ��ۺ�����ֵ����ռ�ı��أ�Ĭ�������Ϊ��0.3, 0.3 , 0.4

	double mDistance;		// ��ǰͼ�����������ͼ�����ƥ�����֮��Ļ�õľ��루Ҳ�������ƶȣ�
	bool mIsFlagged;		// ��ǵ�ǰͼ���Ƿ񱻲���

private:
	/*
	��������	RGB2HSV
	���ܣ�	������ֵ��RGB�ռ�ת����HSV�ռ�
	���룺	����1: RGB����ֵ
	�����	HSVValue��HSV����ֵ
	*/
	HSVValue RGB2HSV(RGBValue& rgb);
	/*
	��������	ExtractHistogram
	���ܣ�	��ȡͼ���ֱ��ͼ����ֵ
	���룺	��
	�����	��
	*/
	void ExtractHistogram();

	/*
	��������	ExtractEntropy
	���ܣ�	��ȡͼ�����ɫ������
	���룺	��
	�����	��
	*/
	void ExtractEntropy();

	/*
	��������	ExtractMoment
	���ܣ�	��ȡͼ�����ɫ������
	���룺	��
	�����	��
	*/
	void ExtractMoment();

	/*
	�������� Normalize
	���ܣ�	��ͼ�����������ֵ���й�һ����λ������
	���룺	��
	�����	��
	*/
	void Normalize();

	/*
	��������	DebugOutputDescriptor
	���ܣ�	��������ʹ�ã����ͼ�������ֵ
	���룺	��
	�����	��
	*/
	void DebugOutputDescriptor();

public:
	/*
	��������	LoadImage
	���ܣ�	���ص�ǰͼ������ݣ��ڲ�Ŀ¼��
	�����	��
	���룺	��
	*/
	void LoadImage();

	/*
	��������	LoadImage
	���ܣ�	���ص�ǰͼ������ݣ��ⲿĿ¼��
	�����	��
	���룺	��
	*/
	void LoadOutImage();

	/*
	��������	GetWidth, GetHeight
	���ܣ�	�õ���ǰͼ��Ŀ�͸�
	���룺	��
	�����	ͼ��Ŀ�͸�
	*/
	int GetWidth();
	int GetHeight();

	/*
	��������	InitImage
	���ܣ�	��ʼ��ͼ�������
	���룺	����1: ͼ������ƣ�����2��ͼ������ݿ�����
	�����	��
	*/
	void InitImage(const char* pName , int index);

	/*
	��������	GetGLIndex
	���ܣ�	���ͼ���OpenGL����
	���룺	��
	�����	OpenGL�е��豸����
	*/
	GLuint GetGLIndex();

	/*
	��������	ExtractDescriptor
	���ܣ�	��ȡͼ�������ֵ�����У�
	���룺	��
	�����	��
	*/
	void ExtractDescriptor();

	/*
	��������	Match
	���ܣ�	����ǰͼ��������һͼ�����ƥ�䣬�������Ա�����б������ƶ�
	���룺	����1�� ��һͼ��Ķ���ָ��
	�����	��
	*/
	void Match(Image* pImage);

	/*
	��������	GetDistance
	���ܣ�	��ȡ��ǰͼ����һ��ƥ��֮������ƶ�ֵ
	���룺	��
	�����	��һ��ƥ��ʱ�õ������ƶ�
	*/
	double GetDistance();

	/*
	��������	SetFlagged
	���ܣ�	�Ե�ǰͼ����б�ǣ��Է�ֹ�����ظ�����
	���룺	��
	�����	��
	*/
	void SetFlagged();

	/*
	��������	IsFlagged
	���ܣ�	��ѯ��ǰͼ���Ƿ��Ѿ�����ǣ��������
	���룺	��
	�����	bool���Ƿ��ѱ����
	*/
	bool IsFlagged();

	/*
	��������	GetImageIndex
	���ܣ�	��ȡ��ǰͼ�������ݿ�������Ӧ������
	���룺	��
	�����	int�����ݿ��е�������Ӧֵ
	*/
	int GetImageIndex();

	/*
	��������	GetHistogramValue
	���ܣ�	�õ�ֱ��ͼ����������ĳ��ά���ϵľ���ֵ���ڽ�������ֵ����ʱʹ�ã�
	���룺	����1������������ά������
	�����	float������ֵ
	*/
	float GetHistogramValue(int index);

	/*
	��������	GetEntropyValue
	���ܣ�	�õ���ɫ������������ĳ��ά���ϵľ���ֵ���ڽ�������ֵ����ʱʹ�ã�
	���룺	����1������������ά������
	�����	float������ֵ
	*/
	float GetEntropyValue(int index);

	/*
	��������	GetMomentValue
	���ܣ�	�õ���ɫ������������ĳ��ά���ϵľ���ֵ���ڽ�������ֵ����ʱʹ�ã�
	���룺	����1������������ά������
	�����	float������ֵ
	*/
	float GetMomentValue(int index);

public:
	Image();
	~Image();

};
#endif