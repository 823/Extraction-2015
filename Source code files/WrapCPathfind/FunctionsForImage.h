//FunctionsForImage.h

#pragma once
//#include "Array2D.h"
//#include "LocationQueue.h"



/////////////////////////////////////////////////////////////////////////
void Astar(const Array2D<int>* map,const LOCATION& start,const LOCATION& goal,CLocationQueue* pathque=NULL,Array2D<uchar>* result=NULL);


/////////////////////////////////////////////////////////////////////////
void DistanceTransform(const Array2D<uchar>* bin,Array2D<int>* result);
int arraymin(const int a[],int n);

/////////////////////////////////////////////////////////////////////////
void ReConstructByBinary(const Array2D<uchar>* mask,Array2D<uchar>* mark,CLocationQueue* markque);//markque将被修改
void ReConstructByOrigin(const Array2D<int>* mask,Array2D<uchar>* mark,bool iscustom,int tolerate);

/////////////////////////////////////////////////////////////////////////
void Thinz (const Array2D<uchar>* srcimg,Array2D<int>* result);
void Delete (Array2D<uchar>* im, Array2D<uchar>* tmp);
int nays1 (const Array2D<uchar>* im, int r, int c);
int nays0 (const Array2D<uchar>* im, int r, int c);
int Connectivity (const Array2D<uchar>* im, int r, int c);
int Yokoi (const Array2D<uchar>* im, int r, int c);
void stair (const Array2D<uchar>* im, Array2D<uchar>* tmp, int dir);

/////////////////////////////////////////////////////////////////////////
void FindNearestLoc(const Array2D<int>* map,const LOCATION& loc,LOCATION& nearestLoc);//距离map中最近的0的点

/////////////////////////////////////////////////////////////////////////
int THbyLaplacianEdge (const IplImage* img,int percent=85);
int THbyIterative (const IplImage* img);
int THbyOTSU (const IplImage* img);
void Thresh(const IplImage* img,Array2D<uchar>* result,int th);


/////////////////////////////////////////////////////////////////////////
void InterpolateDistanceRelated(int* arr,int N,int criterion);
/////////////////////////////////////////////////////////////////////////
double LengthofPath(const LOCATION* path,int N);

/////////////////////////////////////////////////////////////////////////
double AngleofPath(const LOCATION* path,int N);

/////////////////////////////////////////////////////////////////////////
void fillingholes(IplImage* img);

/////////////////////////////////////////////////////////////////////////
int getTH(const IplImage* img,const Array2D<uchar>* pathresult);
float nuOfgetth (float *p, int k, float ut, float vt);


