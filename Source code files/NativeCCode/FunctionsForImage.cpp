//FunctionsForImage.cpp
#include "Stdafx.h"
#include "types.h"
#include "Array2D.h"
#include "LocationList.h"
#include "LocationQueue.h"
#include "FunctionsForImage.h"



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region Astar
#define OPENED 1
#define CLOSED 2
#define UNUSED 3
void Astar(const Array2D<int>* map,const LOCATION& start,const LOCATION& goal,CLocationQueue* pathque/*=NULL*/,Array2D<uchar>* result/*=NULL*/)
{
	assert(pathque!=NULL || result!=NULL);
	if (result!=NULL)
	{
		assert(map->info.nr==result->info.nr && map->info.nc==result->info.nc);
	}
	int nr=map->info.nr;
	int nc=map->info.nc;

	LOCATION neigh[8];
	neigh[0].row=-1, neigh[0].col=-1;
	neigh[1].row=-1, neigh[1].col=0;
	neigh[2].row=-1, neigh[2].col=1;
	neigh[3].row=0,  neigh[3].col=1;
	neigh[4].row=1,  neigh[4].col=1;
	neigh[5].row=1,  neigh[5].col=0;
	neigh[6].row=1,  neigh[6].col=-1;
	neigh[7].row=0,  neigh[7].col=-1;

	Array2D<uchar> path(nr,nc);
	Array2D<char> set(nr,nc);
	Array2D<ULONG> g(nr,nc);
	Array2D<ULONG> h(nr,nc);
	Array2D<ULONG> f(nr,nc);
	for (int i=0;i<nr;i++)
	{
		for (int j=0;j<nc;j++)
		{
			path.data[i][j]=0;
			set.data[i][j]=UNUSED;
			g.data[i][j]=0;
			h.data[i][j]=10*(abs(i-goal.row)+abs(j-goal.col));
			f.data[i][j]=0;
		}
	}


	CLocationList openlist;
	openlist.push(start);
	set.data[start.row][start.col]=OPENED;
	LOCATION current;
	bool bfindpath=true;
	while (set.data[goal.row][goal.col]!=CLOSED)
	{
		if(openlist.empty())
		{
			bfindpath=false;

		}
		current=openlist.pop_minf(&f);		
		set.data[current.row][current.col]=CLOSED;


		for (int i=0;i<8;i++)
		{
			LOCATION temp=current+neigh[i];
			if(temp.row>=0 && temp.row<nr && temp.col>=0 && temp.col<nc)
			{
				if(set.data[temp.row][temp.col]==UNUSED)
				{
					openlist.push(temp);
					set.data[temp.row][temp.col]=OPENED;
					path.data[temp.row][temp.col]=i;
					g.data[temp.row][temp.col]=g.data[current.row][current.col]+(map->data[temp.row][temp.col]+1)^3*(i%2==0?14:10);
					f.data[temp.row][temp.col]=g.data[temp.row][temp.col]+h.data[temp.row][temp.col];
				}
				else if(set.data[temp.row][temp.col]==OPENED)
				{
					ULONG tmp=g.data[current.row][current.col]+(map->data[temp.row][temp.col]+1)^3*(i%2==0?14:10);
					if(g.data[temp.row][temp.col]>tmp)
					{
						path.data[temp.row][temp.col]=i;
						g.data[temp.row][temp.col]=tmp;
						f.data[temp.row][temp.col]=g.data[temp.row][temp.col]+h.data[temp.row][temp.col];
					}
				}
			}
		}


	}
	if (bfindpath=true)
	{
		
		current=goal;	
		while(current.row!=start.row || current.col!=start.col)
		{
			if(pathque!=NULL) pathque->push(current);
			if(result!=NULL) result->data[current.row][current.col]=WHITE;
			current=current+neigh[(path.data[current.row][current.col]+4)%8];

		}
		//舍弃了start点
		//if(pathque!=NULL && pathque->Length()) pathque->push(start);
		//if(result!=NULL) result->data[start.row][start.col]=WHITE;

	}

		/*if (result==NULL)
		{
			current=goal;		
			while(current.row!=start.row || current.col!=start.col)
			{
				pathque->push(current);
				current=current+neigh[(path.data[current.row][current.col]+4)%8];

			}
			pathque->push(start);
		}
		else
		{
			/ *for (int i=0;i<nr;i++)
			{
				for (int j=0;j<nc;j++)
				{
					result->data[i][j]=BLACK;
					
				}
			}* /
			current=goal;		
			while(current.row!=start.row || current.col!=start.col)
			{
				pathque->push(current);
				result->data[current.row][current.col]=WHITE;
				current=current+neigh[(path.data[current.row][current.col]+4)%8];

			}
			pathque->push(start);
			result->data[start.row][start.col]=WHITE;
			
		}*/
		

}
#pragma endregion
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region DistanceTransform
//白色为目标对象，黑色为背景
void DistanceTransform(const Array2D<uchar>* bin,Array2D<int>* result)
{
	
	assert(bin->info.nr==result->info.nr && bin->info.nc==result->info.nc);
	
	int nr=bin->info.nr;
	int nc=bin->info.nc;

	Array2D<int> binext(nr+4,nc+4);
	for (int i=0;i<nr+4;i++)
	{
		for (int j=0;j<nc+4;j++)
		{
			binext.data[i][j]=30000;
		}
	}
	for (int i=2;i<nr+2;i++)
	{
		for (int j=2;j<nc+2;j++)
		{
			binext.data[i][j]=bin->data[i-2][j-2]*100;//注意这里的binext的非零值要尽量大一点，因为它决定了最小值的最大范围
		}
	}

	int mask[9];
	for (int i=2;i<nr+2;i++)
	{
		for (int j=2;j<nc+2;j++)
		{
			mask[0]=binext.data[i][j];
			mask[1]=binext.data[i][j-1]+5;
			mask[2]=binext.data[i-1][j-2]+11;
			mask[3]=binext.data[i-1][j-1]+7;
			mask[4]=binext.data[i-1][j]+5;
			mask[5]=binext.data[i-1][j+1]+7;
			mask[6]=binext.data[i-1][j+2]+11;
			mask[7]=binext.data[i-2][j-1]+11;
			mask[8]=binext.data[i-2][j+1]+11;
			binext.data[i][j]=arraymin(mask,9);
		}
	}
	for (int i=nr+1;i>1;i--)
	{
		for (int j=nc+1;j>1;j--)
		{
			mask[0]=binext.data[i][j];
			mask[1]=binext.data[i][j+1]+5;
			mask[2]=binext.data[i+1][j-2]+11;
			mask[3]=binext.data[i+1][j-1]+7;
			mask[4]=binext.data[i+1][j]+5;
			mask[5]=binext.data[i+1][j+1]+7;
			mask[6]=binext.data[i+1][j+2]+11;
			mask[7]=binext.data[i+2][j-1]+11;
			mask[8]=binext.data[i+2][j+1]+11;
			binext.data[i][j]=arraymin(mask,9);
		}
	}
	for (int i=0;i<nr;i++)
	{
		for (int j=0;j<nc;j++)
		{
			result->data[i][j]=binext.data[i+2][j+2];
		}
	}


}

int arraymin(const int a[],int n)
{
	int m=30000;
	for (int i=0;i<n;i++)
	{
		if (a[i]<m)
		{
			m=a[i];
		}
	}
	return m;
}
#pragma endregion
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region reconstructBybinary
void ReConstructByBinary(const Array2D<uchar>* mask,Array2D<uchar>* mark,CLocationQueue* markque)//markque将被修改
{
	assert(mask->info.nr==mark->info.nr && mask->info.nc==mark->info.nc);
	assert(markque);
	int nr=mask->info.nr;
	int nc=mask->info.nc;

	LOCATION neigh[8];
	neigh[0].row=-1, neigh[0].col=-1;
	neigh[1].row=-1, neigh[1].col=0;
	neigh[2].row=-1, neigh[2].col=1;
	neigh[3].row=0,  neigh[3].col=1;
	neigh[4].row=1,  neigh[4].col=1;
	neigh[5].row=1,  neigh[5].col=0;
	neigh[6].row=1,  neigh[6].col=-1;
	neigh[7].row=0,  neigh[7].col=-1;
	LOCATION cur,temp;
	while(!markque->empty())
	{
		cur=markque->front();
		for (int i=0;i<8;i++)
		{
			temp=cur+neigh[i];
			if (temp.row>=0 && temp.row<nr && temp.col>=0 && temp.col<nc)
			{
				if (mask->data[temp.row][temp.col]==WHITE && mark->data[temp.row][temp.col]!=WHITE)
				{
					mark->data[temp.row][temp.col]=WHITE;
					markque->push(temp);
				}
			}
		}
		markque->pop();

	}

}
void ReConstructByOrigin(const Array2D<int>* mask,Array2D<uchar>* mark,bool iscustom,int tolerate)
{
	assert(mask->info.nr==mark->info.nr && mask->info.nc==mark->info.nc);
	int nr=mask->info.nr;
	int nc=mask->info.nc;

	ULONG sumtemp=0;
	int num=0;
	CLocationQueue* markque=new CLocationQueue;
	LOCATION x;
	for (int i=0;i<nr;i++)
	{
		for (int j=0;j<nc;j++)
		{
			if (mark->data[i][j]==WHITE)
			{
				num++;
				x.row=i,x.col=j;
				markque->push(x);				
				sumtemp+=mask->data[i][j];
			}
		}
	}

	double t=0;
	if (iscustom)
	{
		t=tolerate;
	}
	else
	{
		CvScalar meanv;
		CvScalar stdv;
		IplImage* tempimg=mask->toIplImage();
		IplImage* tempmask=mark->toIplImage();
		cvAvgSdv(tempimg,&meanv,&stdv,tempmask);
		t=stdv.val[0];
		if (t<10) t=10;

		cvReleaseImage(&tempimg);
		cvReleaseImage(&tempmask);
	}
	

	float avg=float(sumtemp)/num;





/*
	FILE* fp=NULL;
	fp=fopen("test.txt","w");
	if(fp!=NULL)
	{

		fprintf(fp,"std:%f\n",t);



	}
	fclose(fp);*/

	LOCATION neigh[8];
	neigh[0].row=-1, neigh[0].col=-1;
	neigh[1].row=-1, neigh[1].col=0;
	neigh[2].row=-1, neigh[2].col=1;
	neigh[3].row=0,  neigh[3].col=1;
	neigh[4].row=1,  neigh[4].col=1;
	neigh[5].row=1,  neigh[5].col=0;
	neigh[6].row=1,  neigh[6].col=-1;
	neigh[7].row=0,  neigh[7].col=-1;
	LOCATION cur,temp;
	while(!markque->empty())
	{
		cur=markque->front();
		for (int i=0;i<8;i++)
		{
			temp=cur+neigh[i];
			if (temp.row>=0 && temp.row<nr && temp.col>=0 && temp.col<nc)
			{
				//if (fabs(mask->data[temp.row][temp.col]-avg)<=tolerate && mark->data[temp.row][temp.col]!=WHITE)
				//if (mask->data[temp.row][temp.col] <= tolerate && mark->data[temp.row][temp.col]!=WHITE)
				if (fabs(mask->data[temp.row][temp.col]-avg)<=t && mark->data[temp.row][temp.col]!=WHITE)
				{
					mark->data[temp.row][temp.col]=WHITE;
					num++;
					markque->push(temp);					
					sumtemp+=mask->data[temp.row][temp.col];
					avg=float(sumtemp)/num;
				}
			}
		}
		markque->pop();

	}
	delete markque;

}


#pragma endregion
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region thin
#define NORTH 0
#define SOUTH 1
void Thinz (const Array2D<uchar>* srcimg,Array2D<int>* result)//ZhangsuenBest,白色表示物体
{
	assert(srcimg->info.nr==result->info.nr && srcimg->info.nc==result->info.nc);
	int nr=srcimg->info.nr;
	int nc=srcimg->info.nc;

	Array2D<uchar> im(nr+2,nc+2);
	int i,j;
	for (i=0; i<nr; i++)
		for (j=0; j<nc; j++)
			im.data[i+1][j+1] =255-srcimg->data[i][j];//将原来的白色表示物体转成黑色表示物体
	for (i=0; i<nr+2; i++) 
	{
		im.data[i][0] = 255;
		im.data[i][nc+1] = 255;
	}
	for (j=0; j<nc+2; j++)
	{
		im.data[0][j] = 255;
		im.data[nr+1][j] = 255;
	}
	/*	Stentiford's boundary smoothing method		*/
	/*for (i=0; i<nr+2; i++)
		for (j=0; j<nc+2; j++)
			if (im.data[i][j] == 0)
				if (nays0(&im, i, j) <= 2 && Yokoi (&im, i, j)<2)
					im.data[i][j] = 2;*/


	/*	Zhang-Suen with Holt's staircase removal */
	/* BLACK = 1, WHITE = 0. */


	Array2D<uchar> tmp(nr+2,nc+2);
	for (i=0; i<nr+2; i++)
		for (j=0; j<nc+2; j++)
		{
			if (im.data[i][j] > 0) im.data[i][j] = 0;
			else im.data[i][j] = 1;
			tmp.data[i][j] = 0;
		}

	/* Mark and delete */
	int again=1,k;
	while (again)
	{
		again = 0;

		/* Second sub-iteration */
		for (i=1; i<nr+1; i++)
			for (j=1; j<nc+1; j++)
			{
				if (im.data[i][j] != 1) continue;
				k = nays1(&im, i, j);
				if ((k > 2 && k <= 6) && Connectivity(&im, i,j)==1)
				{
					if (im.data[i][j+1]*im.data[i-1][j]*im.data[i][j-1] == 0 &&
						im.data[i-1][j]*im.data[i+1][j]*im.data[i][j-1] == 0)
					{
						tmp.data[i][j] = 1;
						again = 1;
					}
				}
			}

		Delete (&im, &tmp);
		if (again == 0) break;

		/* First sub-iteration */
		for (i=1; i<nr+1; i++)
			for (j=1; j<nc+1; j++)
			{
				if (im.data[i][j] != 1) continue;
				k = nays1(&im, i, j);
				if ((k >2 && k <= 6) && Connectivity(&im, i,j)==1)
				{
					if (im.data[i-1][j]*im.data[i][j+1]*im.data[i+1][j] == 0 &&
						im.data[i][j+1]*im.data[i+1][j]*im.data[i][j-1] == 0)
					{
						tmp.data[i][j] = 1;
						again = 1;
					}
				}
			}

		Delete (&im, &tmp);
	}

	/* Post_process */
	stair (&im, &tmp, NORTH);
	Delete (&im, &tmp);
	stair (&im, &tmp, SOUTH);
	Delete (&im, &tmp);

	/* Restore levels */
	for (i=1; i<nr+1; i++)
		for (j=1; j<nc+1; j++)
			if (im.data[i][j] > 0) result->data[i-1][j-1] = 0;//目标物体
			else result->data[i-1][j-1] =1000;
	
}

/*	Delete any pixel in IM corresponding to a 1 in TMP	*/
void Delete (Array2D<uchar>* im, Array2D<uchar>* tmp)
{
	int i,j;

/* Delete pixels that are marked  */
	for (i=1; i<im->info.nr-1; i++)
	  for (j=1; j<im->info.nc-1; j++)
	    if (tmp->data[i][j])
	    {
	        im->data[i][j] = 0;
			tmp->data[i][j] = 0;
	    }
}

/*	Number of neighboring 1 pixels	*/
int nays1 (const Array2D<uchar>* im, int r, int c)
{
	int i,j,k=0;
	for (i=r-1; i<=r+1; i++)
	  for (j=c-1; j<=c+1; j++)
	    if (i!=r || c!=j)
	      if (im->data[i][j] >= 1) k++;
	return k;
}

/*	Number of neighboring 0 pixels	*/
int nays0 (const Array2D<uchar>* im, int r, int c)
{
    int i,j,k=0;
	for (i=r-1; i<=r+1; i++)
	  for (j=c-1; j<=c+1; j++)
	    if (i!=r || c!=j)
	      if (im->data[i][j] == 0) k++;
	return k;
}

/*	Connectivity by counting black-white transitions on the boundary	*/
int Connectivity (const Array2D<uchar>* im, int r, int c)
{
	int N=0;
	if (im->data[r][c+1]   >= 1 && im->data[r-1][c+1] == 0) N++;
	if (im->data[r-1][c+1] >= 1 && im->data[r-1][c]   == 0) N++;
	if (im->data[r-1][c]   >= 1 && im->data[r-1][c-1] == 0) N++;
	if (im->data[r-1][c-1] >= 1 && im->data[r][c-1]   == 0) N++;
	if (im->data[r][c-1]   >= 1 && im->data[r+1][c-1] == 0) N++;
	if (im->data[r+1][c-1] >= 1 && im->data[r+1][c]   == 0) N++;
	if (im->data[r+1][c]   >= 1 && im->data[r+1][c+1] == 0) N++;
	if (im->data[r+1][c+1] >= 1 && im->data[r][c+1]   == 0) N++;

	return N;
}

/*	Yokoi's connectivity measure	*/
int Yokoi (const Array2D<uchar>* im, int r, int c)
{
	int N[9];
	int i,k, i1, i2;
	N[0] = im->data[r][c]      != 0;
	N[1] = im->data[r][c+1]    != 0;
	N[2] = im->data[r-1][c+1]  != 0;
	N[3] = im->data[r-1][c]    != 0;
	N[4] = im->data[r-1][c-1]  != 0;
	N[5] = im->data[r][c-1]    != 0;
	N[6] = im->data[r+1][c-1]  != 0;
	N[7] = im->data[r+1][c]    != 0;
	N[8] = im->data[r+1][c+1]  != 0;

	k = 0;
	for (i=1; i<=7; i+=2)
	{
	  i1 = i+1; if (i1 > 8) i1 -= 8;
	  i2 = i+2; if (i2 > 8) i2 -= 8;
	  k += (N[i] - N[i]*N[i1]*N[i2]);
	}

	return k;
}

void stair (const Array2D<uchar>* im, Array2D<uchar>* tmp, int dir)
{
	int i,j;
	int N, S, E, W, NE, NW, SE, SW, C;

	for (i=1; i<im->info.nr-1; i++)
	  for (j=1; j<im->info.nc-1; j++)
	  {
	   NW = im->data[i-1][j-1]; N = im->data[i-1][j]; NE = im->data[i-1][j+1];
	   W = im->data[i][j-1]; C = im->data[i][j]; E = im->data[i][j+1];
	   SW = im->data[i+1][j-1]; S = im->data[i+1][j]; SE = im->data[i+1][j+1];

	   if (dir == NORTH)
	   {
	     if (C && !(N && 
		      ((E && !NE && !SW && (!W || !S)) || 
		       (W && !NW && !SE && (!E || !S)) )) )
	       tmp->data[i][j] = 0;		/* Survives */
	     else
	       tmp->data[i][j] = 1;
	   } 
	   else if (dir == SOUTH)
	   {
	     if (C && !(S && 
		      ((E && !SE && !NW && (!W || !N)) || 
		       (W && !SW && !NE && (!E || !N)) )) )
	       tmp->data[i][j] = 0;		/* Survives */
	     else
	       tmp->data[i][j] = 1;
	   }
	  }
}

#pragma endregion 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region find nearest zero value location
void FindNearestLoc(const Array2D<int>* map,const LOCATION& loc,LOCATION& nearestLoc)//距离map中最近的0的点
{
	if (map->data[loc.row][loc.col]==0)
	{
		nearestLoc=loc;
		return;
	}
	int nr=map->info.nr;
	int nc=map->info.nc;
	Array2D<uchar> visited(nr,nc);
	for (int i=0;i<nr;i++)
	{
		for (int j=0;j<nc;j++)
		{
			visited.data[i][j]=0;
		}
	}
	CLocationQueue tempqueue;
	tempqueue.push(loc);
	visited.data[loc.row][loc.col]=1;

	LOCATION neigh[8];
	neigh[0].row=-1, neigh[0].col=-1;
	neigh[1].row=-1, neigh[1].col=0;
	neigh[2].row=-1, neigh[2].col=1;
	neigh[3].row=0,  neigh[3].col=1;
	neigh[4].row=1,  neigh[4].col=1;
	neigh[5].row=1,  neigh[5].col=0;
	neigh[6].row=1,  neigh[6].col=-1;
	neigh[7].row=0,  neigh[7].col=-1;
	LOCATION cur,temp;
	nearestLoc.row=-1;//如果下面的过程中寻找不到最近的点，最终将返回负值
	nearestLoc.col=-1;
	while (!tempqueue.empty())
	{
		cur=tempqueue.front();
		for (int i=0;i<8;i++)
		{
			temp=cur+neigh[i];
			if (temp.row>=0 && temp.row<nr && temp.col>=0 && temp.col<nc)
			{
				if (visited.data[temp.row][temp.col]!=1)
				{
					if (map->data[temp.row][temp.col]==0)
					{
						nearestLoc=temp;
						return;
					}
					else 
					{
						tempqueue.push(temp);
						visited.data[temp.row][temp.col]=1;
					}
				}
				
			}
		}
		tempqueue.pop();
	}
}

#pragma endregion 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region binaryTH
//用Laplace边缘算子计算二值化阈值
//THbyLaplacianEdge函数思想摘自J.R.Parker-Algorithms for Image Processing and Computer Vision 2nd 第四章
//部分修改
int THbyLaplacianEdge (const IplImage* img,int percent/*=85*/)
{
	assert(img!=0);
	assert(percent>0 && percent<100);
	int nr=img->height;
	int nc=img->width;
	Array2D<UINT16> lapmat(nr,nc);
	

	char offset[10]={-1,-1,-1,0,1,1,1,0,-1,-1};
	int i,j;
	for (i=1; i<nr-1; i++)
	{
		for (j=1; j<nc-1; j++)
		{
			int temp=0;
			for (int k=0;k<8;k++)
			{
				temp+=CV_IMAGE_ELEM(img,uchar,i+offset[k],j+offset[k+2]);
			}
			lapmat.data[i][j]=abs(temp-8*CV_IMAGE_ELEM(img,uchar,i,j));
		}
	}
	
	//
	UINT16  hist[2048];
	for (i=0; i<2048; i++) hist[i] = 0;
	for (i=1; i<nr-1; i++)
	{
		for (j=1; j<nc-1; j++)
		{
			hist[lapmat.data[i][j]] += 1;
		}
	}


	//
	int val = -1;
	ULONG  m = (ULONG)percent*nr*nc/100;
	ULONG NH=0;
	for (i=0; i<2048; i++)
	{
		NH += hist[i];
		if (NH>=m)
		{
			val = i;
			break;
		}
	}
	//

	for (i=0; i<256; i++) hist[i] = 0;

	/* Find the histogram */
	ULONG N = nr*nc;
	for (i=1; i<nr-1; i++)
	{
		uchar* ptr=(uchar*)(img->imageData+i*img->widthStep);
		for (j=1; j<nc-1; j++)
		{
			if (lapmat.data[i][j]>= val)
				hist[*(ptr+j)] += 1;
		}
	}

	/* Find the first peak */
	j = 0;
	for (i=0; i<256; i++)
		if (hist[i] > hist[j]) j = i;

	/* Find the second peak */
	int k = 0;
	for (i=0; i<256; i++)
		if (i>0 && hist[i-1]<=hist[i] && i<255 && hist[i+1]<=hist[i])
			if ((k-j)*(k-j)*hist[k] < (i-j)*(i-j)*hist[i]) k = i;

	int t = j;
	if (j<k)
	{
		for (i=j; i<k; i++)
			if (hist[i] < hist[t]) t = i;
	} 
	else 
	{
		for (i=k; i<j; i++)
			if (hist[i] < hist[t]) t = i;
	}

	return t;

}
//THbyIterative函数代码摘自J.R.Parker-Algorithms for Image Processing and Computer Vision 2nd第四章
int THbyIterative (const IplImage* img)
{
	float tt, tb, to, t2;
	int   i, j, t;
	long   no, nb;

	Array2D<uchar> image(img);

	ULONG N=(ULONG)image.info.nr*(ULONG)image.info.nc;
	tb = 0.0;       to = 0.0;       no = 0;
	for (i=0; i<image.info.nr; i++) 
		for (j=0; j<image.info.nc; j++)
			to = to+ (image.data[i][j]);
	tt = (to/(float)N);

	while (N) 
	{
		no = 0; nb = 0; tb=0.0; to = 0.0;
		for (i=0; i<image.info.nr; i++) 
			for (j=0; j<image.info.nc; j++)
			{
				if ( (float)(image.data[i][j]) >= tt ) 
				{
					to = to + (float)(image.data[i][j]);
					no++;
				} else {
					tb = tb + (float)(image.data[i][j]);
					nb++;
				}
			}

			if (no == 0) no = 1;
			if (nb == 0) nb = 1;
			t2 = (tb/(float)nb + to/(float)no )/2.0;
			if (t2 == tt) N=0;
			tt = t2;
	}
	t = (int) tt;
	return t;

}




//nu和THbyOTSU函数代码摘自J.R.Parker-Algorithms for Image Processing and Computer Vision 2nd第四章
//部分修改

float nu (float *p, int k, float ut, float vt)
{
	float x=0.0, y=0.0;

	for (int i=1; i<=k; i++) y += p[i];
	for (int i=1; i<=k; i++) x += (float)i*p[i];
	x = ut*y - x;
	x = x*x;
	y = y*(1.0-y);
	if (y>0) x = x/y;
	else x = 0.0;
	return x/vt;
}
int THbyOTSU (const IplImage* img)
{
	int i,j,k,m, h[260];
	float y, z, p[260];
	float ut, vt;

	Array2D<uchar> x(img);
	ULONG N=(ULONG)x.info.nr*(ULONG)x.info.nc;

	for (i=0; i<260; i++) /* Zero the histograms  */
	{         
		h[i] = 0;
		p[i] = 0.0;
	}

	/* Accumulate a histogram */
	for (i=0; i<x.info.nr; i++)
		for (j=0; j<x.info.nc; j++)
		{
			k = x.data[i][j];
			h[k+1] += 1;
		}

	for (i=1; i<=256; i++)          /* Normalize into a distribution */
		p[i] = (float)h[i]/(float)N;

	ut=0.0;
	for (i=1; i<=256; i++) ut += (float)i*p[i];        /* Global mean */
	vt = 0.0;               /* Global Variance */
	for (i=1; i<=256; i++)
		vt += (i-ut)*(i-ut)*p[i];

	j = -1; k = -1;
	for (i=1; i<=256; i++) {
		if ((j<0) && (p[i] > 0.0)) j = i;       /* First index */
		if (p[i] > 0.0) k = i;                  /* Last index  */
	}
	z = -1.0;
	m = -1;
	for (i=j; i<=k; i++) {
		y = nu (p, i, ut, vt);          /* Compute NU */
		if (y>=z) {                     /* Is it the biggest? */
			z = y;                  /* Yes. Save value and i */
			m = i;
		}
	}

	return m;
}



void Thresh(const IplImage* img,Array2D<uchar>* result,int th)
{
	for (int i=0;i<img->height;i++)
	{
		uchar* p=(uchar*)(img->imageData+i*img->widthStep);
		for (int j=0;j<img->width;j++)
		{
			if (p[j]<=th)
			{
				result->data[i][j]=WHITE;
			}
			else
			{
				result->data[i][j]=BLACK;
			}

		}
	}
}
#pragma endregion

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region Interpolation
void InterpolateDistanceRelated(int* arr,int N,int criterion)
{
	/*
	int k=0,n;
	for (int i=0;i<N;i++)
	{
		if (arr[i]>criterion)
		{
			k++;
		}
	}
	n=k;
	int* f=new int[n];
	int* x=new int[n];
	k=0;
	for (int i=0;i<N;i++)
	{
		if (arr[i]>criterion)
		{
			f[k]=arr[i];
			x[k]=i;
			k++;
		}
	}
	double* tmp=new double[n];
	for (int i=0;i<N;i++)
	{
		if (arr[i]<=criterion)
		{
			double s=0;
			for (int j=0;j<n;j++)
			{
				tmp[j]=1.0/((i-x[j])*(i-x[j]));
				s+=tmp[j];
			}
			double stmp=0;
			for (int j=0;j<n;j++)
			{
				stmp=tmp[j]*f[j]/s;
			}
			arr[i]=cvRound(stmp);
		}
	}

	delete []f;
	delete []x;
	delete []tmp;*/

	int k=0;
	UINT sumtemp=0;
	for (int i=0;i<N;i++)
	{
		if (arr[i]>criterion)
		{
			sumtemp+=arr[i];
			k++;
		}
	}
	if (k==0)
	{
		return;
	}
	int temp=cvRound(sumtemp/k);

	for (int i=0;i<N;i++)
	{
		if (arr[i]<=criterion)
		{
		
			arr[i]=temp;
		}
	}

}

#pragma endregion

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region length calculate
double LengthofPath(const LOCATION* path,int N)
{
	UINT len=0;
	for (int i=0;i<N-1;i++)
	{
		int t=path[i+1].row-path[i].row+path[i+1].col-path[i].col;
		len+=(t==1 || t==-1)?10:14;
	}
	return double(len)/10;
}

#pragma endregion

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region angle calculate
double AngleofPath(const LOCATION* path,int N)
{
	CvMemStorage* storage = cvCreateMemStorage(5000);
	CvSeq* point_seq = cvCreateSeq( CV_32SC2, sizeof(CvSeq), sizeof(CvPoint), storage );
	for (int i=0;i<N;i++)
	{
		cvSeqPush(point_seq, &cvPoint(path[i].col,path[i].row));
	}
	float* line=new float[4];
	cvFitLine(point_seq,CV_DIST_L2, 0 , 0.01 , 0.01 , line);

	double angle=atan2(line[1],line[0])*180/3.14;
	if (angle<0)
	{
		angle+=180;
	}
	else if (angle==0)
	{
		angle=180;
	}
	angle=180-angle;//坐标系的y轴方向不一样，所以要变换一下

	cvClearSeq(point_seq);
	cvReleaseMemStorage(&storage);
	delete []line;
	return angle;

}

#pragma endregion


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region filling holes
void fillingholes(IplImage* img)
{
	int width=img->width;
	int height=img->height;
	IplImage* tempimg=cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,1);

	CLocationQueue que;
	LOCATION neigh[4];
	/*neigh[0].row=-1, neigh[0].col=-1;
	neigh[1].row=-1, neigh[1].col=0;
	neigh[2].row=-1, neigh[2].col=1;
	neigh[3].row=0,  neigh[3].col=1;
	neigh[4].row=1,  neigh[4].col=1;
	neigh[5].row=1,  neigh[5].col=0;
	neigh[6].row=1,  neigh[6].col=-1;
	neigh[7].row=0,  neigh[7].col=-1;*/
	neigh[0].row=-1, neigh[0].col=0;
	neigh[1].row=0, neigh[1].col=1;
	neigh[2].row=0, neigh[2].col=-1;
	neigh[3].row=1,  neigh[3].col=0;

	LOCATION x;
	for (int i=0;i<height;i++)
	{
		uchar* ptemp=(uchar*)(tempimg->imageData+i*tempimg->widthStep);//补集，mask
		uchar* p=(uchar*)(img->imageData+i*img->widthStep);//mark
		for (int j=0;j<width;j++)
		{
			ptemp[j]=255-p[j];
			if (i==0 || i==height-1 || j==0 || j==width-1 )
			{
				p[j]=255-p[j];
				if (p[j]==255)
				{
					x.row=i,x.col=j;
					que.push(x);
				}
				
			}
			else
			{
				p[j]=0;
			}

		}
	}


	LOCATION cur,temp;
	while(!que.empty())
	{
		cur=que.front();
		for (int i=0;i<4;i++)
		{
			temp=cur+neigh[i];
			if (temp.row>=0 && temp.row<height && temp.col>=0 && temp.col<width)
			{
				if ((*(uchar*)(tempimg->imageData+temp.row*tempimg->widthStep+temp.col))==255 && (*(uchar*)(img->imageData+temp.row*img->widthStep+temp.col))!=255)
				{
					(*(uchar*)(img->imageData+temp.row*img->widthStep+temp.col))=255;
					que.push(temp);
				}
			}
		}
		que.pop();

	}
	for (int i=0;i<height;i++)
	{
		uchar* p=(uchar*)(img->imageData+i*img->widthStep);
		for (int j=0;j<width;j++)
		{
			p[j]=255-p[j];
		}
	}
	cvReleaseImage(&tempimg);
}




#pragma endregion


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region histogram of region
int getTH(const IplImage* img,const Array2D<uchar>* pathresult)
{
	int nr=pathresult->info.nr;
	int nc=pathresult->info.nc;
	int elementsize=20;
	IplImage* temp=pathresult->toIplImage();
	IplConvKernel* element = cvCreateStructuringElementEx( elementsize*2+1, elementsize*2+1, elementsize, elementsize, CV_SHAPE_ELLIPSE, 0 );
	IplImage* regionimg=cvCreateImage(cvSize(nc,nr),IPL_DEPTH_8U,1);
	cvDilate(temp,regionimg,element);
	cvReleaseImage(&temp);
	cvReleaseStructuringElement(&element);

	int i,j,k,m, h[260];
	float y, z, p[260];
	float ut, vt;

	Array2D<uchar> x(img);

	ULONG N=0;
	//ULONG N=(ULONG)x.info.nr*(ULONG)x.info.nc;
	for (i=0;i<nr;i++)
	{
		uchar* p=(uchar*)(regionimg->imageData+i*regionimg->widthStep);
		for (j=0;j<nc;j++)
		{
			if (p[j]==255)
			{
				N++;
			}
		}
	}


	for (i=0; i<260; i++) /* Zero the histograms  */
	{         
		h[i] = 0;
		p[i] = 0.0;
	}

	/* Accumulate a histogram */
	for (i=0; i<x.info.nr; i++)
	{
		uchar* pp=(uchar*)(regionimg->imageData+i*regionimg->widthStep);
		for (j=0; j<x.info.nc; j++)
		{
			if (pp[j]==255)
			{
				k = x.data[i][j];
				h[k+1] += 1;
			}		
		}
	}
	cvReleaseImage(&regionimg);
	for (i=1; i<=256; i++)          /* Normalize into a distribution */
		p[i] = (float)h[i]/(float)N;

	ut=0.0;
	for (i=1; i<=256; i++) ut += (float)i*p[i];        /* Global mean */
	vt = 0.0;               /* Global Variance */
	for (i=1; i<=256; i++)
		vt += (i-ut)*(i-ut)*p[i];

	j = -1; k = -1;
	for (i=1; i<=256; i++) {
		if ((j<0) && (p[i] > 0.0)) j = i;       /* First index */
		if (p[i] > 0.0) k = i;                  /* Last index  */
	}
	z = -1.0;
	m = -1;
	for (i=j; i<=k; i++) {
		y = nuOfgetth(p, i, ut, vt);          /* Compute NU */
		if (y>=z) {                     /* Is it the biggest? */
			z = y;                  /* Yes. Save value and i */
			m = i;
		}
	}

	return m;
}

float nuOfgetth (float *p, int k, float ut, float vt)
{
	float x=0.0, y=0.0;

	for (int i=1; i<=k; i++) y += p[i];
	for (int i=1; i<=k; i++) x += (float)i*p[i];
	x = ut*y - x;
	x = x*x;
	y = y*(1.0-y);
	if (y>0) x = x/y;
	else x = 0.0;
	return x/vt;
}




#pragma endregion



