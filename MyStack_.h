#include <memory>
#define ERROR_ 10e8

template <typename T>
class StackNode_
{
public:
	StackNode_();
    T data;
    StackNode_* next;
};

template<typename T>
StackNode_<T>::StackNode_() {}

template <class T, class Alloc = std::allocator<T>>
class StackAllocator
{
 public:
	typedef StackNode_<T> Node;
	typedef StackNode_<T>* stack_pointer;
    typedef typename Alloc::template rebind<Node>::other allocator;

	StackAllocator();	//Default ctor
	~StackAllocator();	//destructor

	bool empty();	//to check if the stack is empty

	bool push(T elem);	//push the elements into the stack top
	T pop();	//delete the element which is on the stack top and return the element
 private:
    allocator MyAlloc;
    stack_pointer head_pointer;
};

template <class T, class Alloc>
StackAllocator<T, Alloc>::StackAllocator() {
	head_pointer = nullptr;
}

template <class T, class Alloc>
StackAllocator<T, Alloc>::~StackAllocator() {
	stack_pointer current_pointer = head_pointer;
	while (current_pointer != nullptr) {
		stack_pointer tmp = current_pointer->next;    //record next stack node
		MyAlloc.destroy(current_pointer);    //destroy current object
		MyAlloc.deallocate(current_pointer, 1);    //deallocate the memory
		current_pointer = tmp;
	}
	head_pointer = nullptr;
}

template <class T, class Alloc>
inline bool StackAllocator<T, Alloc>::empty() {
	return (head_pointer == nullptr);
}

template <class T, class Alloc>
bool StackAllocator<T, Alloc>::push(T elem) {
	stack_pointer new_elem = MyAlloc.allocate(1);    //apply new memory
	if (new_elem == nullptr) {    //allocate bad
		return false;
	}
	MyAlloc.construct(new_elem, Node());    //construct a new object

	new_elem->data = elem;
	new_elem->next = head_pointer;
	head_pointer = new_elem;    //adjust head pointer

	return true;
}

template <class T, class Alloc>
T StackAllocator<T, Alloc>::pop() {
	if (head_pointer == nullptr) {    //nothing in stack
		return ERROR_;
	}
	T pop_elem = head_pointer->data;    //record the element which need to be deleted
	stack_pointer tmp = head_pointer->next;
	MyAlloc.destroy(head_pointer);
	MyAlloc.deallocate(head_pointer, 1);
	head_pointer = tmp;

	return pop_elem;
}