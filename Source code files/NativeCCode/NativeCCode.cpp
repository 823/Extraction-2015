
#include "stdafx.h"
#include "types.h"
#include "Array2D.h"
#include "LocationList.h"
#include "LocationQueue.h"
#include "FunctionsForImage.h"

#include "NativeCCode.h"


//#define MAXCRACKNUM 100
//#define RESIZEWIDTH 800

COLORREF CCrackbyPathfinding::colormap[10]=
{
RGB(0,255,0),//green
RGB(255,0,0),//red
RGB(0,0,255),//blue
RGB(218,165,32),//goldenrod
RGB(0,255,127),//SpringGreen
RGB(0,255,255),//Cyan
RGB(255,160,122),//LightSalmon
RGB(139,134,78),//Khaki4
RGB(160,32,240),//Purple
RGB(205,79,57),//Tomato3
};
	

CCrackbyPathfinding::CCrackbyPathfinding()
	//:mImgVector(MAXCRACKNUM)
{
	mOriginimgGrayscale=NULL;
	mOriginimgWith3channel=NULL;
	mPreProcimg=NULL;
	mCrackimg=NULL;
	mAddimg=NULL;

	mAddimgWithLight=NULL;

	mImgForMark=NULL;

}

CCrackbyPathfinding::~CCrackbyPathfinding()
{
	if (mOriginimgGrayscale!=NULL)
	{
		cvReleaseImage(&mOriginimgGrayscale);
	}
	if (mOriginimgWith3channel!=NULL)
	{
		cvReleaseImage(&mOriginimgWith3channel);
	}
	if (mPreProcimg!=NULL)
	{
		cvReleaseImage(&mPreProcimg);
	}
	if (mCrackimg!=NULL)
	{
		cvReleaseImage(&mCrackimg);
	}
	
	if (mAddimg!=NULL)
	{
		cvReleaseImage(&mAddimg);
		mAddimg=NULL;
	}
	if (mAddimgWithLight!=NULL)
	{
		cvReleaseImage(&mAddimgWithLight);
	}
	if (mImgForMark!=NULL)
	{
		cvReleaseImage(&mImgForMark);
	}

	for (int i=0;i<mImgVector.size();i++)
	{
		IplImage* p=mImgVector[i];
		cvReleaseImage(&p);
	}
}

void CCrackbyPathfinding::Init(const IplImage* srcimg)
{
	assert(srcimg->nChannels==1 || srcimg->nChannels==3);
	if (mOriginimgGrayscale!=NULL)
	{
		cvReleaseImage(&mOriginimgGrayscale);
		mOriginimgGrayscale=NULL;
	}
	if (mOriginimgWith3channel!=NULL)
	{
		cvReleaseImage(&mOriginimgWith3channel);
		mOriginimgWith3channel=NULL;
	}
	if (mPreProcimg!=NULL)
	{
		cvReleaseImage(&mPreProcimg);
		mPreProcimg=NULL;
	}
	if (mCrackimg!=NULL)
	{
		cvReleaseImage(&mCrackimg);
		mCrackimg=NULL;
	}
	
	if (mAddimg!=NULL)
	{
		cvReleaseImage(&mAddimg);
		mAddimg=NULL;
	}
	if (mAddimgWithLight!=NULL)
	{
		cvReleaseImage(&mAddimgWithLight);
		mAddimgWithLight=NULL;
	}
	if (mImgForMark!=NULL)
	{
		cvReleaseImage(&mImgForMark);
		mImgForMark=NULL;
	}
	for (int i=0;i<mImgVector.size();i++)
	{
		IplImage* p=mImgVector[i];
		cvReleaseImage(&p);
	}
	mImgVector.clear();
	//mResizeBinArray.Release();

	mOriginheight=srcimg->height;
	mOriginwidth=srcimg->width;
	//mResizefactor=RESIZEWIDTH/double(mOriginwidth);
	//int resizeheight=cvRound(mOriginheight*mResizefactor);
	//mResizefactor=1;
	//int resizeheight=mOriginheight;

	mOriginimgGrayscale=cvCreateImage(cvSize(mOriginwidth,mOriginheight),IPL_DEPTH_8U,1);
	mOriginimgWith3channel=cvCreateImage(cvSize(mOriginwidth,mOriginheight),IPL_DEPTH_8U,3);	
	mPreProcimg=cvCreateImage(cvSize(mOriginwidth,mOriginheight),IPL_DEPTH_8U,1);
	mCrackimg=cvCreateImage(cvSize(mOriginwidth,mOriginheight),IPL_DEPTH_8U,1);
	mAddimg=cvCreateImage(cvSize(mOriginwidth,mOriginheight),IPL_DEPTH_8U,3);
	mAddimgWithLight=cvCreateImage(cvSize(mOriginwidth,mOriginheight),IPL_DEPTH_8U,3);
	mImgForMark=cvCreateImage(cvSize(mOriginwidth,mOriginheight),IPL_DEPTH_8U,3);

	if (srcimg->nChannels==1)
	{
		//cvResize(srcimg,mPreProcimg,CV_INTER_NN);
		//if (mOptionalparas.ispresmooth) cvSmooth(srcimg,mPreProcimg,CV_GAUSSIAN,3,3);
		//else cvCopy(srcimg,mPreProcimg);		
		cvCopy(srcimg,mOriginimgGrayscale);
		cvCvtColor(srcimg,mAddimg,CV_GRAY2BGR);
	}
	else if (srcimg->nChannels==3)
	{
		IplImage* temp=cvCreateImage(cvSize(mOriginwidth,mOriginheight),IPL_DEPTH_8U,1);
		cvCvtColor(srcimg,temp,CV_RGB2GRAY);
		//cvResize(temp,mPreProcimg,CV_INTER_NN);
		//if (mOptionalparas.ispresmooth) cvSmooth(temp,mPreProcimg,CV_GAUSSIAN,3,3);
		//else cvCopy(temp,mPreProcimg);
		cvCopy(temp,mOriginimgGrayscale);
		cvReleaseImage(&temp);

		cvCopy(srcimg,mAddimg);
		
	}
	cvCopy(mAddimg,mOriginimgWith3channel);
	cvCopy(mAddimg,mAddimgWithLight);
	//cvSaveImage("smooth.jpg",mPreProcimg);


	//BinaryInit();

	
		

}
/*
void CCrackbyPathfinding::BinaryInit()
{
	int nr=mPreProcimg->height;
	int nc=mPreProcimg->width;
	for (int i=0;i<BINARYMETHODNUM;i++)
	{
		mResizeBinArray[i].Create(nr,nc);
	}
	int th=THbyLaplacianEdge(mPreProcimg);
	Thresh(mPreProcimg,&mResizeBinArray[0],th);

	th=THbyIterative(mPreProcimg);
	Thresh(mPreProcimg,&mResizeBinArray[1],th);

	th=THbyOTSU(mPreProcimg);
	Thresh(mPreProcimg,&mResizeBinArray[2],th);*/

/*
	IplImage* t1=mResizeBinArray[0].toIplImage();
	IplImage* t2=mResizeBinArray[1].toIplImage();
	IplImage* t3=mResizeBinArray[2].toIplImage();
	cvNamedWindow("t1");
	cvShowImage("t1",t1);
	cvWaitKey(0);
	cvNamedWindow("t2");
	cvShowImage("t2",t2);
	cvWaitKey(0);
	cvNamedWindow("t3");
	cvShowImage("t3",t3);
	cvWaitKey(0);*/

	
//}

#pragma region Calculate previous version
/*
void CCrackbyPathfinding::Calculate(const LOCATION& startLOC,const LOCATION& goalLOC)
{

	
	assert(mPreProcimg);
	int nr=mPreProcimg->height;
	int nc=mPreProcimg->width;
	LOCATION start,goal;
	start.row=cvRound(startLOC.row*mResizefactor);
	start.col=cvRound(startLOC.col*mResizefactor);
	goal.row=cvRound(goalLOC.row*mResizefactor);
	goal.col=cvRound(goalLOC.col*mResizefactor);

	Array2D<int> map(mPreProcimg);
	CLocationQueue pathqueue;
	Array2D<uchar> result(nr,nc);
	Astar(&map,start,goal,&pathqueue,&result);

	ReConstructByBinary(&mResizeBinArray,&result,&pathqueue);

	int an=20;
	IplConvKernel* element = cvCreateStructuringElementEx( an*2+1, an*2+1, an, an, CV_SHAPE_ELLIPSE, 0 );
	IplImage* Iplresult=result.toIplImage();
	IplImage* temp=cvCreateImage(cvSize(nc,nr),IPL_DEPTH_8U,1);
	cvDilate(Iplresult,temp,element);
	cvErode(temp,Iplresult,element);

	result.fromIplImage(Iplresult);
		
	cvReleaseImage(&Iplresult);
	cvReleaseImage(&temp);
	cvReleaseStructuringElement(&element);

	Array2D<int> distancemap(nr,nc);
	DistanceTransform(&result,&distancemap);
		
	Thinz(&result,&map);
	pathqueue.clearall();
	Astar(&map,start,goal,&pathqueue,NULL);

///////////////////////////////////////////////////////////
	int N=pathqueue.Length();
	LOCATION* path=pathqueue.toArray();		
	int* aWidth=new int[N];

		
	IplImage* crackimgtemp=cvCreateImage(cvSize(nc,nr),IPL_DEPTH_8U,1);
	for (int i=0;i<nr;i++)
	{
		uchar* p=(uchar*)(crackimgtemp->imageData+i*crackimgtemp->widthStep);
		for (int j=0;j<nc;j++)
		{
			p[j]=0;
		}
	}
	CvPoint center;
	int* radius=new int[N];
	for (int i=0;i<N;i++)
	{
		aWidth[i]=distancemap.data[path[i].row][path[i].col];
		radius[i]=cvRound(aWidth[i]/double(5));
	}
	/////
	//这里对radius进行插值

	/////
	for (int i=0;i<N;i++)
	{
		center.x=path[i].col;
		center.y=path[i].row;
		cvCircle(crackimgtemp,center,radius[i],cvScalar(255),-1);			
			
	}
	UINT area=0;
	for (int i=0;i<nr;i++)
	{
		uchar* p=(uchar*)(crackimgtemp->imageData+i*crackimgtemp->widthStep);
		for (int j=0;j<nc;j++)
		{
			area+=p[j];
		}
	}

	mCrackParas.area=double(area)/255*mResizefactor*mResizefactor;

	cvResize(crackimgtemp,mCrackimg);
	PushCrackimg();
	IplImageAdd(mCrackimg);
	cvReleaseImage(&crackimgtemp);

	int maxw=aWidth[0];
	int minw=aWidth[0];
	UINT sumw=0;
	for (int i=0;i<N;i++)
	{
		if (aWidth[i]>maxw)
		{
			maxw=aWidth[i];
		}
		if (aWidth[i]<minw)
		{
			minw=aWidth[i];
		}
		sumw+=aWidth[i];
	}
	mCrackParas.maxwidth=2*double(maxw)/5*mResizefactor;//因为在求distance transformation时使用的是5，7，11，即每个像素宽是5
	mCrackParas.minwidth=2*double(minw)/5*mResizefactor;
	mCrackParas.meanwidth=2*double(sumw)/N/5*mResizefactor;


	mCrackParas.angle=AngleofPath(path,N);
	mCrackParas.length=LengthofPath(path,N)*mResizefactor;

	delete []path;
	delete []aWidth;

	mCracknum++;


}*/
#pragma endregion

bool CCrackbyPathfinding::Calculate(LOCATION* locArray,int locCount)
{
	assert(locArray!=NULL);
	assert(mPreProcimg);
	int nr=mPreProcimg->height;
	int nc=mPreProcimg->width;
	if (mOptionalparas.ispresmooth)
	{
		cvSmooth(mOriginimgGrayscale,mPreProcimg,CV_GAUSSIAN,3,3);
	}
	else
	{
		cvCopy(mOriginimgGrayscale,mPreProcimg);
	}

	for (int i=0;i<locCount;i++)
	{
		locArray[i].row=locArray[i].row;//cvRound(locArray[i].row*mResizefactor);
		locArray[i].col=locArray[i].col;//cvRound(locArray[i].col*mResizefactor);
	}


	Array2D<int> map(mPreProcimg);
	CLocationQueue pathqueue;
	Array2D<uchar> result(nr,nc);


	if (mOptionalparas.isinvert)//////////表示是否反转
	{
		for (int i=0;i<nr;i++)
		{
			for (int j=0;j<nc;j++)
			{
				map.data[i][j]=255-map.data[i][j];
			}
		}
	}
	for (int i=0;i<nr;i++)
	{
		for (int j=0;j<nc;j++)
		{
			result.data[i][j]=0;
		}
	}

	for (int i=0;i<locCount-1;i++)
	{
		Astar(&map,locArray[i+1],locArray[i],&pathqueue,&result);
	}

	
	//////////////////////////////////////////////////////////////
	//保存原始图像叠加路径图像
	///////////////////////////////////////////////////////////////
/*
	IplImage* imgwithpath=cvCreateImage(cvSize(mOriginwidth,mOriginheight),IPL_DEPTH_8U,3);
	for (int i=0;i<nr;i++)
	{
		uchar* p=(uchar*)(imgwithpath->imageData+i*imgwithpath->widthStep);
		for (int j=0;j<nc;j++)
		{
			p[j*3]=map.data[i][j];
			p[j*3+1]=map.data[i][j];
			p[j*3+2]=map.data[i][j];
			if (result.data[i][j]==255)
			{
				p[j*3]=0;
				p[j*3+1]=0;
				p[j*3+2]=255;
			}
		}
	}
	cvCircle(imgwithpath,cvPoint(locArray[0].col,locArray[0].row),3,CV_RGB(255,0,255),2);
	cvCircle(imgwithpath,cvPoint(locArray[1].col,locArray[1].row),3,CV_RGB(255,0,255),2);
	cvSaveImage("resultimgwithpath.jpg",imgwithpath);
	cvReleaseImage(&imgwithpath);*/

	/////////////////////////////////////////////////////////////


	//if (mOptionalparas.isoriginreconstruct)///////////
	{
		//int th=getTH(mPreProcimg,&result);
		ReConstructByOrigin(&map,&result,mOptionalparas.iscustomt,mOptionalparas.tolerate);

		//ReConstructByOrigin(&map,&result,th);
	}
	/*if(mOptionalparas.isbinaryreconstruct)
	{
		assert(mOptionalparas.binarymethod<BINARYMETHODNUM);
		if (!mOptionalparas.iscrackdark)
		{
			Array2D<uchar> binarytemp(nr,nc);
			for (int i=0;i<nr;i++)
			{
				for (int j=0;j<nc;j++)
				{
					binarytemp.data[i][j]=255-mResizeBinArray[mOptionalparas.binarymethod].data[i][j];
				}
			}
			ReConstructByBinary(&binarytemp,&result,&pathqueue);
		}
		else
		{
			ReConstructByBinary(&mResizeBinArray[mOptionalparas.binarymethod],&result,&pathqueue);
		}
		
	}*/



	
	Array2D<int> distancemap(nr,nc);
	AfterTreatment(distancemap,pathqueue,result,locArray,locCount);
	if (pathqueue.Length()==0)//因为Astar算法中舍弃了start点，所以如果两个点重合的情况下，pathqueue为空。
	{
		return false;
	}

	//////////////////////////////////
	int N=0;
	LOCATION* path=NULL;
	int* radius=NULL;
	CvPoint center;
	IplImage* imgtemp=cvCreateImage(cvSize(nc,nr),IPL_DEPTH_8U,1);

	/*if (mOptionalparas.isinterpolate)
	{
		N=pathqueue.Length();
		path=pathqueue.toArray();
		radius=new int[N];
		for (int i=0;i<N;i++)
		{
			radius[i]=distancemap.data[path[i].row][path[i].col];
		}
		InterpolateDistanceRelated(radius,N,5);


		for (int i=0;i<nr;i++)
		{
			uchar* p=(uchar*)(imgtemp->imageData+i*imgtemp->widthStep);
			for (int j=0;j<nc;j++)
			{
				p[j]=0;
			}
		}

		for (int i=0;i<N;i++)
		{
			center.x=path[i].col;
			center.y=path[i].row;
			cvCircle(imgtemp,center,cvRound(double(radius[i])/5),cvScalar(255),-1);			

		}
		result.fromIplImage(imgtemp);
		AfterTreatment(distancemap,pathqueue,result,locArray,locCount);	
		delete []path;
		delete []radius;
		if (pathqueue.Length()==0)
		{
			cvReleaseImage(&imgtemp);
			return false;
		}
	}*/
	
	/////////////////////////////////////////////////////////
	N=pathqueue.Length();
	path=pathqueue.toArray();
	radius=new int[N];
	for (int i=0;i<N;i++)
	{
		radius[i]=distancemap.data[path[i].row][path[i].col];
	}
	for (int i=0;i<nr;i++)
	{
		uchar* p=(uchar*)(imgtemp->imageData+i*imgtemp->widthStep);
		for (int j=0;j<nc;j++)
		{
			p[j]=0;
		}
	}

	for (int i=0;i<N;i++)
	{
		center.x=path[i].col;
		center.y=path[i].row;
		cvCircle(imgtemp,center,cvRound(double(radius[i])/5)-1,cvScalar(255),-1);			

	}
	///////////////////////////////////////////////////////////
	
	UINT area=0;
	for (int i=0;i<nr;i++)
	{
		uchar* p=(uchar*)(imgtemp->imageData+i*imgtemp->widthStep);
		for (int j=0;j<nc;j++)
		{
			area+=p[j];
		}
	}

	mCrackParas.area=double(area)/255;//  /(mResizefactor*mResizefactor);

	//cvResize(imgtemp,mCrackimg);
	cvCopy(imgtemp,mCrackimg);
	PushCrackimg();


	
	//cvSaveImage("result.jpg",mCrackimg);//仅用于输出测试结果图片，可以注释掉
	

	IplImageAdd(mCrackimg,mImgVector.size()-1);
	cvReleaseImage(&imgtemp);

	/*IplImage* imgt=cvCreateImage(cvSize(mOriginwidth,mOriginheight),IPL_DEPTH_8U,3);
	cvCopy(mAddimg,imgt);
	cvCircle(imgt,cvPoint(locArray[0].col,locArray[0].row),3,CV_RGB(255,0,255),2);
	cvCircle(imgt,cvPoint(locArray[1].col,locArray[1].row),3,CV_RGB(255,0,255),2);
	cvSaveImage("resultendpoints.jpg",imgt);
	cvReleaseImage(&imgt);*/
	//cvCircle(mAddimg,cvPoint(locArray[0].col,locArray[0].row),3,CV_RGB(255,0,255),2);
	//cvCircle(mAddimg,cvPoint(locArray[1].col,locArray[1].row),3,CV_RGB(255,0,255),2);

	int maxw=radius[0];
	int minw=radius[0];
	UINT sumw=0;
	for (int i=0;i<N;i++)
	{
		if (radius[i]>maxw)
		{
			maxw=radius[i];
		}
		if (radius[i]<minw)
		{
			minw=radius[i];
		}
		sumw+=radius[i];
	}
	/*mCrackParas.maxwidth=2*double(maxw)/(5*mResizefactor)-1;//因为在求distance transformation时使用的是5，7，11，即每个像素宽是5
	mCrackParas.minwidth=2*double(minw)/5/mResizefactor-1;
	mCrackParas.meanwidth=2*double(sumw)/N/5/mResizefactor-1;*/
	mCrackParas.maxwidth=2*double(maxw)/5-1;//因为在求distance transformation时使用的是5，7，11，即每个像素宽是5
	mCrackParas.minwidth=2*double(minw)/5-1;
	mCrackParas.meanwidth=2*double(sumw)/N/5-1;

	/////////////////////////////
	//print widthdata to txt file
	FILE* fp=NULL;
	fp=fopen("widthdata.txt","w");
	if(fp!=NULL)
	{
		for (int i=0;i<N;i++)
		{
			fprintf(fp,"%f\n",2*double(radius[i])/5);
		}
		
	}
	fclose(fp);
	///////////////////////////

	mCrackParas.angle=AngleofPath(path,N);
	mCrackParas.length=LengthofPath(path,N)+(double(radius[0])+double(radius[N-1]))/5-2;
	mCrackParas.no=mImgVector.size();

	delete []path;
	delete []radius;

	return true;
	


}

//void CCrackbyPathfinding::AfterTreatment(Array2D<int>& distancemap,CLocationQueue& pathqueue,const Array2D<uchar>& result,int nr,int nc,const LOCATION& start,const LOCATION& goal)
void CCrackbyPathfinding::AfterTreatment(Array2D<int>& distancemap,CLocationQueue& pathqueue,const Array2D<uchar>& result,const LOCATION* locArray,int locCount)
{
	int nr=mPreProcimg->height;
	int nc=mPreProcimg->width;
	Array2D<uchar> postresult;
/*
	if (mOptionalparas.isclose)/////////
	{
		int elementsize=mOptionalparas.closeradius;
		IplConvKernel* element = cvCreateStructuringElementEx( elementsize*2+1, elementsize*2+1, elementsize, elementsize, CV_SHAPE_ELLIPSE, 0 );
		IplImage* Iplresult=result.toIplImage();
		IplImage* temp=cvCreateImage(cvSize(nc,nr),IPL_DEPTH_8U,1);

		cvSaveImage("originresult.jpg",Iplresult);

		cvDilate(Iplresult,temp,element);
		cvErode(temp,Iplresult,element);

		cvSaveImage("closeresult.jpg",Iplresult);

		postresult.fromIplImage(Iplresult);

		cvReleaseImage(&Iplresult);
		cvReleaseImage(&temp);
		cvReleaseStructuringElement(&element);
	}
	else*/
	{
		IplImage* Iplresult=result.toIplImage();
		//cvSaveImage("resultimgrestore.jpg",Iplresult);/////////保存区域生长后图像
		fillingholes(Iplresult);//填充内部空洞
		//cvSaveImage("resultimgfillingholes.jpg",Iplresult);/////////////保存空洞填充后图像
		postresult.fromIplImage(Iplresult);
		cvReleaseImage(&Iplresult);
		//result.Copyto(&postresult);
		//postresult.fromIplImage(result);
	}
	
	DistanceTransform(&postresult,&distancemap);

	//////////////////////////////////////////////////////////////
	//保存可视化距离变换图像
	///////////////////////////////////////////////////////////////
/*
	{
	int M=0;
	for (int i=0;i<nr;i++)
	{
		for (int j=0;j<nc;j++)
		{
			if (distancemap.data[i][j]>M)
			{
				M=distancemap.data[i][j];
			}
		}
	}
	IplImage* img=cvCreateImage(cvSize(mOriginwidth,mOriginheight),IPL_DEPTH_8U,1);
	for (int i=0;i<nr;i++)
	{
		uchar* p=(uchar*)(img->imageData+i*img->widthStep);
		for (int j=0;j<nc;j++)
		{
			p[j]=distancemap.data[i][j]*255/M;
		}
	}
	cvSaveImage("resultimgdistancemap.jpg",img);
	cvReleaseImage(&img);
	}*/
	/////////////////////////////////////////////////////////////

	Array2D<int> map(nr,nc);
	Thinz(&postresult,&map);


	pathqueue.clearall();
	//LOCATION startONskeleton,goalONskeleton;
	//FindNearestLoc(&map,start,startONskeleton);
	//FindNearestLoc(&map,goal,goalONskeleton);
	//Astar(&map,startONskeleton,goalONskeleton,&pathqueue,NULL);
	LOCATION* locArrayONskeleton=new LOCATION[locCount];
	for (int i=0;i<locCount;i++)
	{
		FindNearestLoc(&map,locArray[i],locArrayONskeleton[i]);
		if (locArrayONskeleton[i].row<0)
		{
			delete[] locArrayONskeleton;
			return;
		}
	}

	/////////////////////////////////
	//////////////////////////////////////////////////////////////
	//保存初始骨架图像
	///////////////////////////////////////////////////////////////
	/*{

		IplImage* img=cvCreateImage(cvSize(mOriginwidth,mOriginheight),IPL_DEPTH_8U,3);
		for (int i=0;i<nr;i++)
		{
			uchar* p=(uchar*)(img->imageData+i*img->widthStep);
			for (int j=0;j<nc;j++)
			{
				if (map.data[i][j]>0)
				{
					p[j*3]=255;
					p[j*3+1]=255;
					p[j*3+2]=255;
				}
				else
				{
					p[j*3]=0;
					p[j*3+1]=0;
					p[j*3+2]=0;
				}
			}
		}
		cvCircle(img,cvPoint(locArray[0].col,locArray[0].row),2,CV_RGB(0,0,255),-1);
		cvCircle(img,cvPoint(locArray[1].col,locArray[1].row),2,CV_RGB(0,0,255),-1);
		cvCircle(img,cvPoint(locArrayONskeleton[0].col,locArrayONskeleton[0].row),2,CV_RGB(255,0,0),-1);
		cvCircle(img,cvPoint(locArrayONskeleton[1].col,locArrayONskeleton[1].row),2,CV_RGB(255,0,0),-1);
		cvSaveImage("resultimgthinmap.jpg",img);
		cvReleaseImage(&img);
	}*/
	/////////////////////////////////////////////////////////////
	////////////////////////////////
	/*Array2D<uchar> secondresult(nr,nc);
	for (int i=0;i<nr;i++)
	{
		for (int j=0;j<nc;j++)
		{
			secondresult.data[i][j]=0;
		}
	}*/
	/////////////////////////////////
	for (int i=0;i<locCount-1;i++)
	{
		Astar(&map,locArrayONskeleton[i+1],locArrayONskeleton[i],&pathqueue,NULL);
		//Astar(&map,locArrayONskeleton[i+1],locArrayONskeleton[i],&pathqueue,&secondresult);
	}

	//////////////////////////////////////////////////////////////
	//保存剪枝后骨架图像
	///////////////////////////////////////////////////////////////
	/*{

		IplImage* img=cvCreateImage(cvSize(mOriginwidth,mOriginheight),IPL_DEPTH_8U,1);
		for (int i=0;i<nr;i++)
		{
			uchar* p=(uchar*)(img->imageData+i*img->widthStep);
			for (int j=0;j<nc;j++)
			{
					p[j]=secondresult.data[i][j];
			}
		}
		cvSaveImage("resultimgthinimg.jpg",img);
		cvReleaseImage(&img);
	}*/
	/////////////////////////////////////////////////////////////
	delete[] locArrayONskeleton;

}

void CCrackbyPathfinding::PushCrackimg()
{
	IplImage* tmp=cvCreateImage(cvSize(mOriginwidth,mOriginheight),IPL_DEPTH_8U,1);
	cvCopy(mCrackimg,tmp);
	mImgVector.push_back(tmp);

	/*
	CLocationQueue* que=new CLocationQueue;
	LOCATION cur;
	for (int i=1;i<mCrackimg->height-1;i++)
	{
		uchar* p=(uchar*)(mCrackimg->imageData+i*mCrackimg->widthStep);
		for (int j=1;j<mCrackimg->width-1;j++)
		{
			if (p[j]==255 && (*(p-mCrackimg->widthStep+j)==0 || *(p+mCrackimg->widthStep+j)==0
				|| *(p+j-1)==0 || *(p+j+1)==0))
			{
				cur.row=i;
				cur.col=j;
				que->push(cur);
			}
			
		}
	}

	LOCATION* arr=que->toArray();
	LOCATIONARRAY a;
	a.arr=arr;
	a.length=que->Length();
	mImgVector.push_back(a);
	que->clearall();
	
	IplImage* tmp=cvCreateImage(cvGetSize(mCrackimg),IPL_DEPTH_8U,1);
	cvErode(mCrackContour,tmp);
	for (int i=0;i<mCrackimg->height;i++)
	{
		uchar* p=(uchar*)(mCrackContour->imageData+i*mCrackContour->widthStep);
		uchar* p1=(uchar*)(tmp->imageData+i*tmp->widthStep);
		for (int j=0;j<mCrackimg->width;j++)
		{
			p[j]-=p1[j];
		}
	}*/


}

void CCrackbyPathfinding::IplImageAdd(IplImage* img, int colorindex)
{
	/*
	if (img->nChannels==1)
	{
		for (int i=0;i<mAddimg->height;i++)
		{
			uchar* p0=(uchar*)(mAddimg->imageData+i*mAddimg->widthStep);
			uchar* p=(uchar*)(img->imageData+i*img->widthStep);
			for (int j=0;j<mAddimg->width;j++)
			{
				p0[j*3]=p0[j*3]|p[j];
				p0[j*3+1]=p0[j*3+1]|p[j];
				p0[j*3+2]=p0[j*3+2]&(~(p[j]));
			}
		}
	}*/



	if (img==NULL)
	{
		return;
	}
	assert(img->nChannels==1);
	colorindex%=10;
	uchar r=GetRValue(colormap[colorindex]);
	uchar g=GetGValue(colormap[colorindex]);
	uchar b=GetBValue(colormap[colorindex]);

	for (int i=0;i<mAddimg->height;i++)
	{
		uchar* p0=(uchar*)(mAddimg->imageData+i*mAddimg->widthStep);
		uchar* p=(uchar*)(img->imageData+i*img->widthStep);
		for (int j=0;j<mAddimg->width;j++)
		{
			if (p[j]==255)
			{
				if (p0[j*3]>128) p0[j*3]=(255 -2*((255 -b) * (255-p0[j*3]))/255);
				else p0[j*3]=(2*p0[j*3]*b)/255;
				//p0[j*3]  =(p0[j*3] +b)/2;

				if (p0[j*3+1]>128) p0[j*3+1]=(255 -2*((255 -g) * (255-p0[j*3+1]))/255);
				else p0[j*3+1]=(2*p0[j*3+1]*g)/255;
				//p0[j*3+1]=(p0[j*3+1]+g)/2;

				if (p0[j*3+2]>128) p0[j*3+2]=(255 -2*((255 -r) * (255-p0[j*3+2]))/255);
				else p0[j*3+2]=(2*p0[j*3+2]*r)/255;
				//p0[j*3+2]=(p0[j*3+2]+r)/2;		
/*
				p0[j*3]  =158;
				p0[j*3+1]=74;
				p0[j*3+2]=0;*/

			}
				
		}
	}

	
	

}

void CCrackbyPathfinding::LightSelected(int index/*=-1*/)//在maddimg基础上添加轮廓显示,i=-1时表示显示最后添加到vector中的轮廓
{
	cvCopy(mAddimg,mAddimgWithLight);
	int N=mImgVector.size();
	if (N==0)
	{
		return;
	}
	assert(index>=-1 && index<N);
	int realindex=index;
	if (index==-1)
	{
		realindex=N-1;
	}	
	IplImage* tmp=mImgVector[realindex];
	uchar r=0;
	uchar g=74;
	uchar b=158;
	for (int i=0;i<tmp->height;i++)
	{
		uchar* p0=(uchar*)(mAddimgWithLight->imageData+i*mAddimgWithLight->widthStep);
		uchar* p=(uchar*)(tmp->imageData+i*tmp->widthStep);
		for (int j=0;j<tmp->width;j++)
		{
			if (p[j]==255)
			{
				p0[j*3]  =158;
				p0[j*3+1]=74;
				p0[j*3+2]=0;
			/*	if (p0[j*3]>128) p0[j*3]=(255 -2*((255 -b) * (255-p0[j*3]))/255);
				else p0[j*3]=(2*p0[j*3]*b)/255;

				if (p0[j*3+1]>128) p0[j*3+1]=(255 -2*((255 -g) * (255-p0[j*3+1]))/255);
				else p0[j*3+1]=(2*p0[j*3+1]*g)/255;

				if (p0[j*3+2]>128) p0[j*3+2]=(255 -2*((255 -r) * (255-p0[j*3+2]))/255);
				else p0[j*3+2]=(2*p0[j*3+2]*r)/255;*/
			}

		}
	}


	/*
	int N=mImgVector.size();
	assert(num>=-1 && num<N);
	int index=num;
	if (num==-1)
	{
		index=N-1;
	}
	cvCopy(mAddimg,mAddimgWithLight);
	LOCATIONARRAY a=mImgVector[index];
	LOCATION* p=a.arr;
	for (int  i=0;i<a.length;i++)
	{
		((uchar*)(mAddimgWithLight->imageData + mAddimgWithLight->widthStep*(p[i].row)))[p[i].col]=255;
		((uchar*)(mAddimgWithLight->imageData + mAddimgWithLight->widthStep*(p[i].row)))[p[i].col+1]=0;
		((uchar*)(mAddimgWithLight->imageData + mAddimgWithLight->widthStep*(p[i].row)))[p[i].col+2]=0;
	}*/
	
}

void CCrackbyPathfinding::MarkPoint(const LOCATION& loc,bool iscopy)
{
	if (iscopy)
	{
		cvCopy(mAddimgWithLight,mImgForMark);
	}	
	cvCircle(mImgForMark,cvPoint(loc.col,loc.row),3,CV_RGB(255,0,255),2);
	

}

void CCrackbyPathfinding::DeleteCrack(int index)
{
	int N=mImgVector.size();
	assert(index>=0 && index<N);
	IplImage* temp=mImgVector[index];
	cvReleaseImage(&temp);
	IplImageVector::iterator it=mImgVector.begin();
	mImgVector.erase(it+index);
	N=mImgVector.size();
	cvCopy(mOriginimgWith3channel,mAddimg);
	for (int i=0;i<N;i++)
	{
		IplImageAdd(mImgVector[i],i);

	}
}

