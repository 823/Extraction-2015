#include "StdAfx.h"
#include "types.h"
#include "LocationQueue.h"


CLocationQueue::CLocationQueue(void)
{
	head=NULL;
	tail=NULL;
	length=0;
}


CLocationQueue::~CLocationQueue(void)
{
	clearall();
}


void CLocationQueue::push(const LOCATION& loc)
{
	Node* pnode=new Node;
	pnode->data=loc;
	pnode->next=NULL;
	if (head==NULL)
	{
		head=tail=pnode;
	}
	else
	{
		tail->next=pnode;
		tail=pnode;
	}	
	length++;

}
LOCATION CLocationQueue::front()
{
	assert(head!=NULL);
	return head->data;

}
void CLocationQueue::pop()
{
	assert(head!=NULL);
	Node* p=head;
	head=head->next;
	if (head==NULL)
	{
		tail=NULL;
	}
	delete p;
	length--;

}
bool CLocationQueue::empty()
{
	if (head==NULL)
	{
		return true;
	}
	return false;
}

void CLocationQueue::clearall()
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
		tail=NULL;
		length=0;
	}

}

UINT CLocationQueue::Length()
{
	return length;
}

LOCATION* CLocationQueue::toArray()
{
	
	LOCATION* x=new LOCATION[length];
	Node* p=head;
	for (int i=0;i<length;i++)
	{
		x[i]=p->data;
		p=p->next;
	}
	return x;
}