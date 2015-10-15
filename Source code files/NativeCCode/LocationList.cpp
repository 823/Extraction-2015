#include "StdAfx.h"
#include "types.h"
#include "Array2D.h"
#include "LocationList.h"


CLocationList::CLocationList(void):head(NULL)
{
}


CLocationList::~CLocationList(void)
{
	clearall();
}

bool CLocationList::empty()
{
	if (head==NULL)
	{
		return true;
	}
	return false;
}

void CLocationList::push(const LOCATION& loc)
{
	Node* p=new Node;
	p->data=loc;
	p->next=head;
	head=p;

}
LOCATION CLocationList::pop_minf(const Array2D<ULONG>* f)
{
	assert(f && head);
	ULONG minf=f->data[head->data.row][head->data.col];
	Node* pmin=head;
	Node* p0=head;
	Node* p=head;
	while (p!=NULL)
	{

		if (f->data[p->data.row][p->data.col]<=minf)
		{
			pmin=p0;
			minf=f->data[p->data.row][p->data.col];
		}
		p0=p;
		p=p->next;
	}
	LOCATION x;
	if (pmin==head)
	{
		head=head->next;
		x=pmin->data;
		delete pmin;
		return x;
	}
	else
	{
		p=pmin->next;
		x=p->data;
		pmin->next=p->next;
		delete p;
		return x;

	}

}

void CLocationList::clearall()
{
	if (head!=NULL)
	{
		Node* p0;
		Node* p=head;
		while (p!=NULL)
		{
			p0=p->next;
			delete p;
			p=p0;
		}
		head=NULL;
	}
	
}