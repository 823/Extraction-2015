// This is the main DLL file.

#include "stdafx.h"

#include "NativeCCode.h"
#include "WrapCPathfind.h"

namespace WrapCPathfind {

	CWrapPathfinding::CWrapPathfinding()
		:cracktool(new CCrackbyPathfinding)
	{
		crackparas=gcnew CLRCrackParas();
		bitmapforshow=nullptr;
		bitmapformarkshow=nullptr;
		
	}
	CWrapPathfinding::~CWrapPathfinding()
	{
		delete cracktool;
	}
	CWrapPathfinding::!CWrapPathfinding()
	{
		
	}
	void CWrapPathfinding::Init(Bitmap^ bitmap)
	{
		IplImage* img=BitmapToIplImagePointer(bitmap);
		cracktool->Init(img);
		bitmapforshow=IplImagePointerToBitmap(cracktool->mAddimgWithLight);
		bitmapformarkshow=IplImagePointerToBitmap(cracktool->mImgForMark);
	

		/*
		
		Bitmap^ temp=IplImagePointerToBitmap(cracktool->mResizeSrcimg);
		IplImage* tempimg=BitmapToIplImagePointer(temp);
		cvNamedWindow("src");
		cvShowImage("src",tempimg);
		cvWaitKey(0);

		//cvReleaseImage(&(cracktool->mResizeSrcimg));
		//cvCircle(cracktool->mResizeSrcimg,cvPoint(200,200),300,cvScalar(0,0,0,0),-1);
		//cvSetImageROI(cracktool->mResizeSrcimg,cvRect(0,0,100,100));
		//IplImage* tempimg2=BitmapToIplImagePointer(temp);
		BitmapData^ data;
		data=temp->LockBits(System::Drawing::Rectangle(0,0,temp->Width,temp->Height),ImageLockMode::ReadWrite,PixelFormat::Format8bppIndexed);
		Byte*p=(Byte*)(data->Scan0.ToPointer());
		for (int i=0;i<100;i++)
		{
			for (int j=0;j<100;j++)
			{
				*p=100;
				p++;
			}
			p+=data->Stride;
		}
		temp->UnlockBits(data);
		//IplImage* tempimg2=BitmapToIplImagePointer(temp);
		cvNamedWindow("src1");
		cvShowImage("src1",cracktool->mResizeSrcimg);
		cvWaitKey(0);*/
	}

	bool CWrapPathfinding::Calculate(const array<CLRLocation>^ clrlocArray,CLROptionalSET^ clroptionalset)
	{
		
		//LOCATION s,g;
		//s.row=start.Row;s.col=start.Col;
		//g.row=goal.Row;g.col=goal.Col;
		assert(clrlocArray->Length>=2);
		clroptionalset->Copyto(cracktool->mOptionalparas);
		
		int locCount=clrlocArray->Length;
		LOCATION* locArray=new LOCATION[locCount];
		for (int i=0;i<locCount;i++)
		{
			locArray[i].row=clrlocArray[i].Row;
			locArray[i].col=clrlocArray[i].Col;
		}
		
		bool b=cracktool->Calculate(locArray,locCount);
		delete []locArray;
		if (!b)
		{
			return false;
		}

		cracktool->LightSelected();

		crackparas->Copyfrom(cracktool->mCrackParas);
		return true;
		/*
		crackparas->No=cracktool->mCracknum;
		crackparas->Angle=cracktool->mCrackParas.angle;
		crackparas->Maxwidth=cracktool->mCrackParas.maxwidth;
		crackparas->Minwidth=cracktool->mCrackParas.minwidth;
		crackparas->Meanwidth=cracktool->mCrackParas.meanwidth;
		crackparas->Length=cracktool->mCrackParas.length;
		crackparas->Area=cracktool->mCrackParas.area;*/

		//crackbitmap=IplImagePointerToBitmap(cracktool->mCrackimg);


	}
	
	Bitmap^ CWrapPathfinding::IplImagePointerToBitmap(const IplImage* mi)//此函数产生的bitmap 与mi的像素数据是共享的，即改变mi中的图像数据会影响bitmap
	{
		//MIplImage mi = IplImagePointerToMIplImage(img);
		PixelFormat pixelFormat;    //像素格式
		switch(mi->nChannels)
		{
		case 1:
			switch (mi->depth)
			{
			case IPL_DEPTH_8U:
				pixelFormat = PixelFormat::Format8bppIndexed;
				break;
			case IPL_DEPTH_16U:
				pixelFormat = PixelFormat::Format16bppGrayScale;
				break;
			}
			break;
		case 3:
			switch (mi->depth)
			{
			case IPL_DEPTH_8U:
				pixelFormat = PixelFormat::Format24bppRgb;
				break;
			case IPL_DEPTH_16U:
				pixelFormat = PixelFormat::Format48bppRgb;
				break;
			}
			break;

		}
		Bitmap^ bitmap =gcnew Bitmap(mi->width, mi->height, mi->widthStep, pixelFormat, (IntPtr)mi->imageData);
		if (pixelFormat == PixelFormat::Format8bppIndexed) 
		{
			ColorPalette^ palette = bitmap->Palette;
			for (int i = 0; i < palette->Entries->Length; i++)
				palette->Entries[i] = Color::FromArgb(255, i, i, i);
			bitmap->Palette = palette;
		}
			
		return bitmap;
	}

	IplImage* CWrapPathfinding::BitmapToIplImagePointer( Bitmap^ bitmap)
	{
		IImage^ iimage = nullptr;
		switch (bitmap->PixelFormat)
		{
		case PixelFormat::Format8bppIndexed:
			iimage = gcnew Emgu::CV::Image<Gray, Byte>(bitmap);
			break;
		case PixelFormat::Format16bppGrayScale:
			iimage = gcnew Emgu::CV::Image<Gray, UInt16>(bitmap);
			break;
		case PixelFormat::Format24bppRgb:
			iimage = gcnew Emgu::CV::Image<Bgr, Byte>(bitmap);
			break;
		case PixelFormat::Format32bppArgb:
			iimage = gcnew Emgu::CV::Image<Bgra, Byte>(bitmap);
			break;
		case PixelFormat::Format48bppRgb:
			iimage = gcnew Emgu::CV::Image<Bgr, UInt16>(bitmap);
			break;
		case PixelFormat::Format64bppArgb:
			iimage = gcnew Emgu::CV::Image<Bgra, UInt16>(bitmap);
			break;
		default:
			Emgu::CV::Image<Bgra, Byte>^ tmp1 = gcnew Emgu::CV::Image<Bgra, Byte>(bitmap->Size);
			array<Byte,3>^ data = tmp1->Data;
			for (int i = 0; i < bitmap->Width; i++)
			{
				for (int j = 0; j < bitmap->Height; j++)
				{
					Color color = bitmap->GetPixel(i, j);
					data[j, i, 0] = color.B;
					data[j, i, 1] = color.G;
					data[j, i, 2] = color.R;
					data[j, i, 3] = color.A;
				}
			}
			iimage = tmp1;
			break;
		}
		return (IplImage*)(iimage->Ptr.ToPointer());
	}

	void CWrapPathfinding::LightSelected(int index)
	{
		cracktool->LightSelected(index);

	}


	void CWrapPathfinding::MarkPoint(CLRLocation location,bool whetherclearbefore)
	{
		LOCATION x;
		x.row=location.Row;
		x.col=location.Col;
		cracktool->MarkPoint(x,whetherclearbefore);
	}

	void CWrapPathfinding::DeleteCrack(int index)
	{
		cracktool->DeleteCrack(index);
		cracktool->LightSelected();
	}

}

