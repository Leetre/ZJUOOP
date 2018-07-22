#pragma once
#include <climits>
#include <cstddef>
#include <iostream>
using namespace std;

template <typename _Ty, size_t block_size = 1000000>
class MyMemoryPool
{
public:
	size_t BlockSize;	//This variable is used to extend blocksize.

	typedef _Ty value_type;
	typedef _Ty* pointer;
	typedef _Ty& reference;
	typedef const _Ty* const_pointer;
	typedef const _Ty& const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef std::true_type  propagate_on_container_move_assignment;
	typedef std::true_type  is_always_equal;

	template <typename _Uty> struct rebind {	//using the same strategy to get another allocator of _Uty
		typedef MyMemoryPool<_Uty> other;
	};


	MyMemoryPool() noexcept;	//default ctor
	MyMemoryPool(const MyMemoryPool& MyMemoryPool) noexcept;
	MyMemoryPool(MyMemoryPool&& MyMemoryPool) noexcept;
	template <class _Uty> MyMemoryPool(const MyMemoryPool<_Uty>& MyMemoryPool) noexcept;

	~MyMemoryPool() noexcept;	//destructor

	MyMemoryPool& operator=(const MyMemoryPool& MyMemoryPool) = delete;
	MyMemoryPool& operator=(MyMemoryPool&& MyMemoryPool) noexcept;

	void* operator new(size_t n_size);	//overload operator new(new function can not overload, but operator new can overload)
	void operator delete(void *p);	//overload operator delete(delete function can not overload, but operator delete can overload)

	pointer address(reference x) const noexcept;	//to get address of a entity object
	const_pointer address(const_reference x) const noexcept;

	/*These two functions are used to allocate or deallocate memory*/
	pointer allocate(size_type n = 1, const_pointer hint = 0);	//to allocate memory
	void deallocate(pointer p, size_type n = 1);	//to deallocate memory

	size_type max_size() const noexcept;
	template <class _Uty, class... _Types> void construct(_Uty* p, _Types&&... args);	//Construct an object on allocated memory.
	template <class _Uty> void destroy(_Uty* p);	//destroy object
	template <class... _Types> pointer newElement(_Types&&... args);	//create new objects
	void deleteElement(pointer p);	//delete object

private:
	/*data structure to record type _Ty and next slot and next block*/
	typedef class _Chunk {
	public:
		value_type Elem;
		_Chunk* next_slot;
		_Chunk* next_block;
	}my_chunk;

	typedef _Chunk* new_pointer;
	typedef _Chunk* chunk_pointer;

	chunk_pointer this_block;
	chunk_pointer this_slot;
	chunk_pointer last_slot;
	chunk_pointer free_slots;

	void extend_block();	//extend blocksize
	void apply_newblock();	//apply a new block
	size_type calculate_last(chunk_pointer _Pty, size_type Align) const noexcept;	//to calculate where the last slot

};

// The ctor of MymemoryPool
template <typename _Ty, size_t block_size>
MyMemoryPool<_Ty, block_size>::MyMemoryPool()
noexcept
{
	extend_block();    //to check if it is necessary to extend blocksize

	this_block = nullptr;
	this_slot = nullptr;
	last_slot = nullptr;
	free_slots = nullptr;
}

// The ctor of MymemoryPool
template <typename _Ty, size_t block_size>
MyMemoryPool<_Ty, block_size>::MyMemoryPool(const MyMemoryPool& memoryPool)
noexcept :
MyMemoryPool()
{
	extend_block();    //to check if it is necessary to extend blocksize
}

// The ctor of MymemoryPool
template <typename _Ty, size_t block_size>
MyMemoryPool<_Ty, block_size>::MyMemoryPool(MyMemoryPool&& memoryPool)
noexcept
{
	extend_block();    //to check if it is necessary to extend blocksize

	this_block = memoryPool.this_block;
	memoryPool.this_block = nullptr;
	this_slot = memoryPool.this_slot;
	last_slot = memoryPool.last_slot;
	free_slots = memoryPool.freeSlots;
}

// The ctor of MymemoryPool
template <typename _Ty, size_t block_size>
template<class _Uty>
MyMemoryPool<_Ty, block_size>::MyMemoryPool(const MyMemoryPool<_Uty>& memoryPool)
noexcept :
MyMemoryPool()
{
	extend_block();    //to check if it is necessary to extend blocksize
}

//overload the operator =
template <typename _Ty, size_t block_size>
MyMemoryPool<_Ty, block_size>&
MyMemoryPool<_Ty, block_size>::operator=(MyMemoryPool&& memoryPool)
noexcept
{
	if (this != &memoryPool)
	{
		std::swap(this_block, memoryPool.this_block);
		this_slot = memoryPool.this_slot;
		last_slot = memoryPool.last_slot;
		free_slots = memoryPool.free_slots;
	}
	return *this;
}
// The destructor
template <typename _Ty, size_t block_size>
MyMemoryPool<_Ty, block_size>::~MyMemoryPool()
noexcept
{
	chunk_pointer curr = this_block;	//to record current block address
	while (curr != nullptr) {
		chunk_pointer prev = curr->next_block;    //to record next block address
		chunk_pointer temp = curr;
		operator delete((void*)temp);    //operator delete need parameter (void*)
		curr = prev;
	}
}

//overload the new
template <typename _Ty, size_t block_size>
void* MyMemoryPool<_Ty, block_size>::operator new(size_t n_size)
{
	return ::operator new(n_size);
}

//overload the delete
template <typename _Ty, size_t block_size>
void MyMemoryPool<_Ty, block_size>::operator delete(void* p)
{
	::operator delete(p);
}

//return the address
template <typename _Ty, size_t block_size>
inline typename MyMemoryPool<_Ty, block_size>::pointer
MyMemoryPool<_Ty, block_size>::address(reference x)
const noexcept
{
	return &x;
}

//return the address
template <typename _Ty, size_t block_size>
inline typename MyMemoryPool<_Ty, block_size>::const_pointer
MyMemoryPool<_Ty, block_size>::address(const_reference x)
const noexcept
{
	return &x;
}

//allocate the space 
template <typename _Ty, size_t block_size>
inline typename MyMemoryPool<_Ty, block_size>::pointer
MyMemoryPool<_Ty, block_size>::allocate(size_type n, const_pointer hint)
{
	pointer result = nullptr;
	chunk_pointer temp = nullptr;

	if (this_slot + n <= last_slot) {    //if current block can hold n slots that program apply
		result = (pointer)this_slot;	//to record the address of current slot
		this_slot = this_slot + n;    //to move current slot to the address n slots later
	}
	else {    //if current block can not hold n slots that program apply
		while (this_slot < last_slot) {    //to put the slots which are not used into free slot
			temp = this_slot;
			this_slot++;

			temp->next_slot = free_slots;
			free_slots = temp;
		}

		apply_newblock();    //to allocate a new block
		result = (pointer)this_slot;    ////to record the address of current slot in new block
		this_slot = this_slot + n;    //to move current slot to the address n slots later
	}

	return result;    //return current block address
}
//deallocate the space 
template <typename _Ty, size_t block_size>
inline void MyMemoryPool<_Ty, block_size>::deallocate(pointer p, size_type n)
{
	if (p != nullptr) {
		((chunk_pointer)p)->next_slot = free_slots;
		free_slots = (chunk_pointer)p;
	}
}

// define the max size of the object
template <typename _Ty, size_t block_size>
inline typename MyMemoryPool<_Ty, block_size>::size_type
MyMemoryPool<_Ty, block_size>::max_size()
const noexcept
{
	size_type max_blocks = -1 / BlockSize;
	return (BlockSize - sizeof(new_pointer)) / sizeof(my_chunk) * max_blocks;
}

template <typename _Ty, size_t block_size>
template <class _Uty, class... _Types>
inline void MyMemoryPool<_Ty, block_size>::construct(_Uty* p, _Types&&... args)
{
	new (p) _Uty(std::forward<_Types>(args)...);
}

template <typename _Ty, size_t block_size>
template <class _Uty>
inline void MyMemoryPool<_Ty, block_size>::destroy(_Uty* p)
{
	p->~_Uty();
}

// return the pointer of the new object
template <typename _Ty, size_t block_size>
template <class... _Types>
inline typename MyMemoryPool<_Ty, block_size>::pointer
MyMemoryPool<_Ty, block_size>::newElement(_Types&&... args)
{
	pointer result = allocate();
	construct<value_type>(result, std::forward<_Types>(args)...);
	return result;
}

// delete one object
template <typename _Ty, size_t block_size>
inline void MyMemoryPool<_Ty, block_size>::deleteElement(pointer p)
{
	if (p != nullptr) {
		p->~value_type();
		deallocate(p);
	}
}

// if the block size can't satisfy the object ,extend it
template <typename _Ty, size_t block_size>
inline void MyMemoryPool<_Ty, block_size>::extend_block()
{
	if (sizeof(_Ty) > block_size) {
		BlockSize = 10 * sizeof(_Ty);
	}
	else {
		BlockSize = block_size;
	}
}

//calculate the last chunk of a block
template <typename _Ty, size_t block_size>
inline typename MyMemoryPool<_Ty, block_size>::size_type
MyMemoryPool<_Ty, block_size>::calculate_last(chunk_pointer _Ptr, size_type Align)
const noexcept
{
	size_type ptr_begin = (size_type)_Ptr;
	size_type ptr_end = ptr_begin + BlockSize;
	size_type last_slot = ptr_end - (Align + (BlockSize%Align));    //to calculate last slot position
	return last_slot;
}

template <typename _Ty, size_t block_size>
void MyMemoryPool<_Ty, block_size>::apply_newblock()
{
	new_pointer newBlock = reinterpret_cast<new_pointer>(operator new(BlockSize));
	if (!newBlock) {
		throw std::bad_alloc();
	}

	(reinterpret_cast<chunk_pointer>(newBlock))->next_block = this_block;
	this_block = reinterpret_cast<chunk_pointer>(newBlock);

	this_slot = this_block;    //to decide where this_slot
	this_slot++;    //to leave a position to reserve data structure(_Chunk)
	last_slot = reinterpret_cast<chunk_pointer>(calculate_last(this_block, alignof(my_chunk)));
}
