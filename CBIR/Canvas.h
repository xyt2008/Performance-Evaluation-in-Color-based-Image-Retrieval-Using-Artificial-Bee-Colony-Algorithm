/******************************************************************
�ļ�����		Canvas.h
�����ˣ�		����ΰ���ż�漣��������������
��ע���ڣ�	2013-4-5
������		ͼ���ڻ���MFC��ͼ��OpenGL�����л��Ƶ�ʵ���ࡣ�䱾�ʵĻ���������OpenGL��ͼ��ʵ�֡�
******************************************************************/

#ifndef __CANVAS_H__
#define __CANVAS_H__

#include "Image.h"
#include "FuncsLib.h"

class Canvas
{
private:
	int mWidth;			// ���ڵĿ��
	int mHeight;		// ���ڵĵ���
	int mBarWidth;		// �����Ҳ��������Ŀ��

	int mTotalHeight;	// ��ǰ���Ƶ�ͼƬ�е��ܸ߶�
	int mHeightEpsilon;	// ��ǰ���Ƶ�ͼƬ�еĸ߶�ƫ�ƣ���Ҫ����ʵ��ͼ��Ĺ�����

private:
	/*
	��������	Render
	���ܣ�	ʵ�ֽ�����ͼ�������ָ����λ������ָ���Ŀ�߽��л���
	���룺	����1��2��Ŀ��ͼ���ڴ��������ڵ�λ�ã�����3��4��Ŀ��ͼ���ڴ���������Ӧ�Ŀ�ߣ�����5��Ŀ��ͼ���ָ��
	�����	��
	*/
	void Render(int posX , long posY , int width , int height , Image* pImage);

	/*
	�������� DrawScrooBar
	���ܣ�	ʵ�ִ����Ҳ�Ĺ������Ļ���
	���룺	��
	�����	��
	*/
	void DrawScrollBar();

	/*
	��������	Render
	���ܣ�	ʵ�ֶ�����ļ������ͼ���б��ڴ����ϵĻ��Ʋ���
	���룺	����1����������б��ͼ��ָ���б�
	�����	��
	*/
	void Render(List<Image*>& imagePtrList);

public:
	/*
	��������	SetSize
	���ܣ�	���ô��ڵĴ�С����
	�����	����1��2����������Ӧ�Ŀ���
	�����	��
	*/
	void SetSize(int width , int height);

	/*
	��������	Draw
	���ܣ�	�ⲿ�ӿڣ�ʵ�ֶԼ�������б��ͼ�����
	���룺	�������������ָ�����б�
	�����	��
	*/
	void Draw(List<Image*>& imageList);

	/*
	��������	Scroll
	���ܣ�	�������¹������ܵ�ʵ��
	���룺	�����ķ���ֵ
	�����	��
	*/
	void Scroll(int delta);

	/*
	��������	Reset
	���ܣ�	�Ի��ƴ��ڽ�������
	���룺	��
	�����	��
	*/
	void Reset();

public:
	Canvas();
	~Canvas();
};
#endif