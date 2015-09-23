/******************************************************************
�ļ�����		Canvas.cpp
�����ˣ�		zhaoweiwang
��ע���ڣ�	2013-4-5
������		ͼ���ڻ���MFC��ͼ��OpenGL�����л��Ƶ�ʵ���ࡣ�䱾�ʵĻ�������������ͶӰ�µ�OpenGL��ͼ��ʵ�֡�
******************************************************************/
#include "stdafx.h"
#include "Canvas.h"

/************************************************************************************
ʹ��OpenGL�е���ͼģʽ�������Ҳ�Ĺ�����
************************************************************************************/
void Canvas::DrawScrollBar()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();

	// Base bar
	//*
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.8f , 0.0f , 0.0f);
	glTexCoord2f(0.0 , 1.0);
	glVertex3f((GLfloat)mWidth - mBarWidth , (float)mHeight , 0.0f);
	glTexCoord2f(1.0 , 1.0);
	glVertex3f((GLfloat)mWidth, (float)mHeight , 0.0f);
	glTexCoord2f(1.0 , 0.0);
	glVertex3f((GLfloat)mWidth , 0 , 0.0f);
	glTexCoord2f(0.0 , 0.0);
	glVertex3f((GLfloat)mWidth - mBarWidth , 0 , 0.0f);
	glEnd();
	//*/

	// Slider bar
	int barWidthEpsilon = 1;
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.0f , 0.5f , 1.0f);
	glTexCoord2f(0.0 , 1.0);
	glVertex3f((GLfloat)mWidth - mBarWidth + barWidthEpsilon , (float)mHeight , 1.0f);
	glTexCoord2f(1.0 , 1.0);
	glVertex3f((GLfloat)mWidth - barWidthEpsilon , (float)mHeight , 1.0f);
	glTexCoord2f(1.0 , 0.0);
	glVertex3f((GLfloat)mWidth - barWidthEpsilon , (float)mHeight - 50 , 1.0f);
	glTexCoord2f(0.0 , 0.0);
	glVertex3f((GLfloat)mWidth - mBarWidth + barWidthEpsilon , (float)mHeight - 50 , 1.0f);
	glEnd();

	glPopMatrix();

	glPopAttrib();
}

/************************************************************************************
ʹ��OpenGL�е���ͼģʽ������ָ��λ���ϵ�ͼ�񡣸��ݴ����λ���Լ������Ϣ������Ӧ��Ŀ����Σ�
֮��Ӹ�����ͼ���л�ȡ��Ӧ����ͼ��Ϣ��������ʩ�ӵ���ǰ��Ŀ������в����л��ơ�
************************************************************************************/
void Canvas::Render(int posX , long posY , int width , int height , Image* pImage)
{
	if(pImage)
	{
		int xEpsilon = 2;
		int yEpsilon = 2;

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glEnable(GL_TEXTURE_2D);
		//glEnable(GL_BLEND);

		glBindTexture(GL_TEXTURE_2D , pImage->GetGLIndex());

		glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_NEAREST);  
		glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_NEAREST); 

		glPushMatrix();

		glBegin(GL_TRIANGLE_FAN);
		glColor3f(1.0f , 1.0f , 1.0f);

		glTexCoord2f(0.0 , 1.0);
		glVertex3f((float)(posX + xEpsilon) , (float)mHeight - posY - yEpsilon , 0.0f);

		glTexCoord2f(1.0 , 1.0);
		glVertex3f((GLfloat)width + posX - xEpsilon, (float)mHeight - posY - yEpsilon , 0.0f);

		glTexCoord2f(1.0 , 0.0);
		glVertex3f((GLfloat)width + posX - xEpsilon , (float)mHeight - (GLfloat)(height) - posY + yEpsilon , 0.0f);

		glTexCoord2f(0.0 , 0.0);
		glVertex3f((float)(posX + xEpsilon) , (float)mHeight - (GLfloat)(height) - posY + yEpsilon , 0.0f);

		glEnd();

		glPopMatrix();

		glPopAttrib();
	}
}

/************************************************************************************
ʹ��OpenGL�е���ͼģʽ������ָ��λ���ϵ�ͼ�񡣸��ݴ����λ���Լ������Ϣ������Ӧ��Ŀ����Σ�
֮��Ӹ�����ͼ���л�ȡ��Ӧ����ͼ��Ϣ��������ʩ�ӵ���ǰ��Ŀ������в����л��Ƽ��ɡ�
�����ϣ�ÿ�л���8��ͼ��ͼ�����ͼƬǽ����ʽ���Ų���
ÿһ�м������ǰһ��ͼ���ƽ���߶ȣ�֮����øø߶�����ͼ�����ͳһ�����ţ�
�����ź��ÿһ��ͼ����л��Ƽ��ɡ�
************************************************************************************/
void Canvas::Render(List<Image*>& imagePtrList)
{
	// Render all the image to picture wall
	long yPos = mHeightEpsilon;

	int rowSize = 8;
	int rowNums = imagePtrList.GetListSize() / rowSize + 1;

	int imageBaseIndex = 0;

	for(int row = 0 ; row < rowNums ; ++row)
	{
		imageBaseIndex = row * rowSize;

		int subRowSize = min(8 , imagePtrList.GetListSize() - imageBaseIndex);

		int totalHeight = 0;
		for(int i = 0 ; i < subRowSize ; ++i)
		{
			totalHeight += (*imagePtrList.GetItemPtr(imageBaseIndex + i))->GetHeight();
		}
		float avgHeight = (float)totalHeight / (float)rowSize;

		float totalWidth = 0.0f;
		for(int i = 0 ; i < subRowSize ; ++i)
		{
			totalWidth += (float)(*imagePtrList.GetItemPtr(imageBaseIndex + i))->GetWidth() * (avgHeight / (float)(*imagePtrList.GetItemPtr(imageBaseIndex + i))->GetHeight());
		}

		if (totalWidth > 0.0f)
		{
			avgHeight *= (float)(mWidth - mBarWidth) / totalWidth;
		}

		float xPos = 0;
		float imageScale = 1.0f;
		for(int i = 0 ; i < subRowSize ; ++i)
		{
			imageScale = ((float)avgHeight / (float)(*imagePtrList.GetItemPtr(imageBaseIndex + i))->GetHeight());
			Render(xPos , yPos , (int)((float)(*imagePtrList.GetItemPtr(imageBaseIndex + i))->GetWidth() * imageScale) , (int)((float)(*imagePtrList.GetItemPtr(imageBaseIndex + i))->GetHeight() * imageScale) , (*imagePtrList.GetItemPtr(imageBaseIndex + i)));
			xPos += (*imagePtrList.GetItemPtr(imageBaseIndex + i))->GetWidth() * imageScale;
		}

		yPos += (int)avgHeight;
	}

	mTotalHeight = yPos - mHeightEpsilon;
}

/************************************************************************************
�ⲿ�ɼ���ͼ����ƽӿڣ��ڲ���Ҫ����Render�Լ����������ƽӿ���ʵ��
************************************************************************************/
void Canvas::Draw(List<Image*>& imagePtrList)
{
	Render(imagePtrList);

	DrawScrollBar();
}

/************************************************************************************
���ݽ��յ���UIƫ��ֵ������м��Ĺ���ֵ����������ǰ��ͼƬǽ
�������������0ʱ�·���
���������С��0ʱ�Ϸ���
ͼ��ķ���ʵ����Ҫ�������޸��������ʱ�ĸ߶�ƫ��ֵmHeightEpsilon��
************************************************************************************/
void Canvas::Scroll(int delta)
{
	if (mTotalHeight <= 0)
	{
		return;
	}

	if (delta > 0)
	{
		mHeightEpsilon += 10;
	}
	else
	{
		mHeightEpsilon -= 10;
	}

	mHeightEpsilon = min(0 , mHeightEpsilon);
	mHeightEpsilon = max(-(mTotalHeight - mHeight) , mHeightEpsilon);
}

/************************************************************************************
���û�������Ҫ������ͼƬǽ����ʱ�ĸ߶�ƫ��
************************************************************************************/
void Canvas::Reset()
{
	mHeightEpsilon = 0;
}

/************************************************************************************
��¼����Ļ�����С�Ĳ��������ڻ���ʱʹ��
************************************************************************************/
void Canvas::SetSize(int width , int height)
{
	mWidth = width;
	mHeight = height;
}

Canvas::Canvas()
{
	mWidth = 800;
	mHeight = 600;

	mBarWidth = 20;

	mTotalHeight = 0;
	mHeightEpsilon = 0;
}

Canvas::~Canvas()
{

}