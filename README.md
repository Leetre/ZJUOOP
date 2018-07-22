# ZJUOOP

STL Allocator + MemoryPool
Date:           Jun 28, 2018.
Description:    
    This program design our own allocator by using memory pool.During program execution, 
		memory is not returned to the system but is returned to our memory pool. 
		The advantage of this is that when we next apply for memory, 
		we can apply directly to the memory pool instead of applying to the system. 
		In this way, we can reduce memory fragmentation and speed up memory allocation.
Header Files:   MyMemoryPool_.h. 	MyStack_.h 	MyQueue_.h
Test Codes:   	test_correct.cpp  	test_speed.cpp.
Executable file:test_correct.exe	test_speed.exe		test_speed_2.exe.

MyMemoryPool_.h:   
		1.download the file
		2.Put the file in the folder which you want to use.
    3.add  # include "MyMemoryPool_.h" on the top of the cpp program you are using.
		4.replace the std::allocator with our allocator"MyMemoryPool".


MyStack_.h:   	
    1.download the file
		2.Put the file in the folder which you want to use.
    3.add  # include "MyStack_.h" on the top of the cpp program you are using.
		4.using the class "StackAllocator" .


MyQueue_.h:   	
    1.download the file
		2.Put the file in the folder which you want to use.
    3.add  # include "Queue_.h" on the top of the cpp program you are using.
		4.using the class "QueueAllocator" .

test_correct.cpp:	
		1.download the file
		2.Put the file in the folder together with "MyMemoryPool_.h","MyStack_.h","MyQueue_.h",.
    3.using "g++ -o test_correct test_correct.cpp" to compile it


test_speed.cpp:	
    1.download the file
		2.Put the file in the folder together with "MyMemoryPool_.h","MyStack_.h","MyQueue_.h",.
    3.using "g++ -o test_correct test_correct.cpp" to compile it
