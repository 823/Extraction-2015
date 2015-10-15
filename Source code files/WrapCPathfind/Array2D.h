#pragma once
#include "StdAfx.h"

typedef struct _header 
{
	int nr;
	int nc;             /* Rows and columns in the image */
}
ArrayHeader;

template<typename T>
class Array2D
{
public:
	ArrayHeader info;            /* Pointer to header */
	T **data;           /* Pixel values */

public:
	void Create(int nr, int nc);
	void Release();
	void fromIplImage(const IplImage* x);
	IplImage* toIplImage() const;
	void Copyto(Array2D<uchar>* x) const;

public:
	Array2D(void);
	Array2D(int nr,int nc);
	Array2D(const IplImage* img);
	~Array2D(void);
};


template<typename T>
Array2D<T>::Array2D(void)
{
	info.nr=0;
	info.nc=0;
	data=NULL;
}

template<typename T>
Array2D<T>::Array2D(int nr,int nc)
{
	info.nr=0;
	info.nc=0;
	data=NULL;
	Create(nr,nc);
}

template<typename T>
Array2D<T>::Array2D(const IplImage* img)
{
	info.nr=0;
	info.nc=0;
	data=NULL;
	fromIplImage(img);
}


template<typename T>
Array2D<T>::~Array2D(void)
{
	Release();
}



template<typename T>
void Array2D<T>::Create(int nr, int nc)
{
	assert(nr > 0 && nc > 0);
	Release();
	info.nr = nr;       
	info.nc = nc;

	data =new T*[nr];
	for (int i=0;i<nr;i++)
	{
		data[i]=new T[nc];
	}

}


template<typename T>
void Array2D<T>::Release()
{

	if (info.nr!=0) 
	{
		for (int i=0;i<info.nr;i++)
		{
			delete [](data[i]);
		}
		delete []data;
		info.nr=0;
		info.nc=0;
		data=NULL;
	}
}


template<typename T>
void Array2D<T>::fromIplImage(const IplImage* x)
{
	assert(x!=NULL && x->nChannels==1);
	int nr=x->height;
	int nc=x->width;

	if (info.nr!=nr || info.nc!=nc)
	{
		Create(nr,nc);
	}

	for (int i=0;i<nr;i++)
	{
		uchar* p=(uchar*)(x->imageData+i*x->widthStep);
		for (int j=0;j<nc;j++)
		{
			data[i][j]=p[j];
		}
	}

}

template<typename T>
IplImage* Array2D<T>::toIplImage() const
{
	assert(info.nr!=0);
	IplImage* x=cvCreateImage(cvSize(info.nc,info.nr),IPL_DEPTH_8U, 1);
	for (int i=0; i<info.nr; i++)
	{
		uchar* p=(uchar*)(x->imageData+i*x->widthStep);
		for (int j=0; j<info.nc; j++)
		{
			p[j]=data[i][j];
		}
	}
	return x;
}

template<typename T>
void Array2D<T>::Copyto(Array2D<uchar>* x) const
{
	assert(x!=NULL);
	x->Create(info.nr,info.nc);
	for (int i=0;i<info.nr;i++)
	{
		for (int j=0;j<info.nc;j++)
		{
			x->data[i][j]=data[i][j];
		}
	}

}