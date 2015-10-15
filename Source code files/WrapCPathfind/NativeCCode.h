// CCode.h

#pragma once
//#include "Array2D.h"

//#define BINARYMETHODNUM 3
class CCrackbyPathfinding
{
public:
	static COLORREF colormap[10]; //预存10组颜色值索引
private: 
	IplImage* mOriginimgGrayscale;
	IplImage* mOriginimgWith3channel;
	IplImage* mPreProcimg;
	
	//Array2D<uchar> mResizeBinArray[BINARYMETHODNUM];
	int mOriginwidth;
	int mOriginheight;
	//double mResizefactor;
	//int mResizefactor;

	IplImage* mCrackimg;
	IplImageVector mImgVector;
	IplImage* mAddimg;
public:
	IplImage* mAddimgWithLight;
	CrackParas mCrackParas;
	OptionalSET mOptionalparas;

	IplImage* mImgForMark;

public:
	CCrackbyPathfinding();
	~CCrackbyPathfinding();

public://下面的函数中的行列坐标均是相对于原始图像的

	bool Calculate(LOCATION* locArray,int locCount);
	void Init(const IplImage* srcimg);
	//void BinaryInit();
	void LightSelected(int index=-1);
	void MarkPoint(const LOCATION& loc,bool iscopy);
	void DeleteCrack(int index);
private:
	void AfterTreatment(Array2D<int>& distancemap,CLocationQueue& pathqueue,const Array2D<uchar>& result,const LOCATION* locArray,int locCount);
	void PushCrackimg();
	void IplImageAdd(IplImage* img, int colorindex);
	// TODO: Add your methods for this class here.
};




