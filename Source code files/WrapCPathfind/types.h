#pragma once
#include <vector>
#define BLACK 0
#define WHITE 255
typedef struct _location
{
	int row;
	int col;
	struct _location operator+(struct _location x)
	{
		struct _location t;
		t.row=row+x.row;
		t.col=col+x.col;
		return t;
	}

}
LOCATION;

typedef struct _node
{
	LOCATION data;
	struct _node* next;

}
Node;


typedef struct _crackparas
{
	int no;
	double length;
	double maxwidth;
	double minwidth;
	double meanwidth;
	double angle;
	double area;
}
CrackParas;


/*
#define MEANInterpolateMETHOD 0
#define OTHERInterpolateMETHOD 1*/
typedef struct _optionalset
{
	bool isinvert;

	bool iscustomt;
	bool ispresmooth;
	//int closeradius;

	//bool ismultipick;
	//bool isbinaryreconstruct;
	//int binarymethod;
	//bool isoriginreconstruct;
	int tolerate;

	//bool isinterpolate;
	//int interpolatemethod;
/*
	struct _optionalset()
	{
		iscrackdark=true;
		isclose=true;
		closeradius=20;
		//ismultipick=false;
		isoriginreconstruct=false;
		tolerate=20;
		isinterpolate=true;
		interpolatemethod=MEANInterpolateMETHOD;
	}*/

}
OptionalSET;

typedef std::vector<IplImage*> IplImageVector;


