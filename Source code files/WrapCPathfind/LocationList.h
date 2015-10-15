#pragma once



class CLocationList
{
public:
	Node* head;
public:
	CLocationList(void);
	~CLocationList(void);

public:
	void push(const LOCATION& loc);
	LOCATION pop_minf(const Array2D<ULONG>* f);
	bool empty();
	void clearall();

};


