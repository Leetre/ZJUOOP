//This is the header file of Queue;
//The structure queue we implement is used to test our own allocator
#pragma once
#include <memory>
#define ERROR 1e5

//The basic node of the queue,and T is the type of T is the object to store in the queue
template <typename T>
struct QueueNode_ {
	T Element;
	QueueNode_* Next;
	QueueNode_* Last;
};

/** T is the object to store in the stack, Alloc is the allocator to use */
template <class T, class Alloc >
class QueueAllocator
{
public:
	typedef QueueNode_<T>*PtrToNode;
	typedef QueueNode_<T> Node;
	typedef typename Alloc::template rebind<Node>::other allocator;


	QueueAllocator();	//Default ctor
	~QueueAllocator();	//destructor

	bool push(T element);	//push the elements into the queue on the rear
	T pop();	//delete the element from the front of the queue
private:
	allocator MyAlloc;//the allocator
	PtrToNode Front;// the head of the queue
	PtrToNode Rear;// the end of the queue
};

//Default ctor,which construct the head of the queue
template <class T, class Alloc>
QueueAllocator<T, Alloc>::QueueAllocator() {
	Front = MyAlloc.allocate(1);
	Front->Next = nullptr;
	Front->Last = nullptr;
	Front->Element = (T)(0);
	Rear = Front;
}
//Destructor,release all the space and return to the system
template <class T, class Alloc>
QueueAllocator<T, Alloc>::~QueueAllocator() {
	PtrToNode p = Front;
	while (p != nullptr) {
		PtrToNode tmp = p->Next;
		MyAlloc.destroy(p);
		MyAlloc.deallocate(p, 1);
		p = tmp;
	}
	Front = nullptr;
	Rear = nullptr;
}
//Push the elements into the queue on the rear
template <class T, class Alloc>
bool QueueAllocator<T, Alloc>::push(T X) {
	PtrToNode p = MyAlloc.allocate(1);
	PtrToNode r = Front;
	PtrToNode s = Front->Next;
	// add a new element to the end and move the rear to the new element
	p->Element = X;
	p->Last = r;
	p->Next = s;
	if (Front == Rear) {
		r->Next = p;
		Rear = p;
	}
	else {
		r->Next = p;
		s->Last = p;
	}
	return true;
}

template <class T, class Alloc>
T QueueAllocator<T, Alloc>::pop() {
	PtrToNode p = Front->Next;
	//if the queue is empty,return error
	if (Front == Rear)
		return ERROR;
	T x = p->Element;
	//delete the element at the beginning and update the front
	PtrToNode q = p->Next;
	Front->Next = q;
	if (q == NULL) {
		Rear = Front;
	}
	else {
		q->Last = Front;
	}
	MyAlloc.destroy(p);
	MyAlloc.deallocate(p, 1);
	return x;
}