#pragma once
class CLocationQueue
{
private:
	Node* head;
	Node* tail;
	UINT length;

public:
	void push(const LOCATION& loc);
	LOCATION front();
	void pop();
	bool empty();
	void clearall();
	UINT Length();
	LOCATION* toArray();

public:
	CLocationQueue(void);
	~CLocationQueue(void);
};

