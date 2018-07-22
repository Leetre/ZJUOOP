#include <iostream>
#include <time.h>
#include "MyMemoryPool_.h"
#include "MyStack_.h"
#include "MyQueue_.h"
using namespace std;

const size_t element_num = 100000;
const int LOOPS = 50;

int main()
{
    clock_t start;
    cout << "Provided to compare the default allocator to MemoryPool..." << endl;
	cout << "Use self defined stack..." << endl;
    
	/* Use the default allocator std::allocator*/
	/*Using the Stack to testing the time of std::allocator */
    StackAllocator<int, std::allocator<int>> stackDefault;
    start = clock();
    for (int j = 0; j < LOOPS; j++)
    {
        for (int i = 0; i < element_num; i++) {
            stackDefault.push(i);
        }
        for (int i = 0; i < element_num; i++) {
			stackDefault.pop();
		}
	}
	cout << "Default Allocator Time: ";
	cout << (((double)clock() - start) / CLOCKS_PER_SEC) << endl;

	/* Use MemoryPool */
	/*Using the Stack to testing the time of MemoryPool */
	StackAllocator<int, MyMemoryPool<int>> stackPool;
	start = clock();
	for (int j = 0; j < LOOPS; j++)
	{
		for (int i = 0; i < element_num; i++) {
			stackPool.push(i);
		}
		for (int i = 0; i < element_num; i++) {
			stackPool.pop();
		}
	}
	cout << "MemoryPool Allocator Time: ";
	cout << (((double)clock() - start) / CLOCKS_PER_SEC) << endl;
	cout << endl;

	/* Use the default allocator std::allocator*/
	/*Using the Queue to testing the time of std::allocator */
	cout << "Use self defined queue..." << endl;
	QueueAllocator<int, std::allocator<int>> queueDefault;
	start = clock();
	for (int j = 0; j < LOOPS; j++)
	{
		for (int i = 0; i < element_num; i++) {
			queueDefault.push(i);
		}
		for (int i = 0; i < element_num; i++) {
			queueDefault.pop();
		}
	}
	cout << "Default Allocator Time: ";
	cout << (((double)clock() - start) / CLOCKS_PER_SEC) << endl;

	/* Use MemoryPool */
	/*Using the Queue to testing the time of MemoryPool */
	QueueAllocator<int, MyMemoryPool<int>> queuePool;
	start = clock();
	for (int j = 0; j < LOOPS; j++)
	{
		for (int i = 0; i < element_num; i++) {
			queuePool.push(i);
		}
		for (int i = 0; i < element_num; i++) {
			queuePool.pop();
		}
	}
	cout << "MemoryPool Allocator Time: ";
	cout << (((double)clock() - start) / CLOCKS_PER_SEC) << endl;

	system("Pause");
	return 0;
}
