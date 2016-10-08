# ThreadPool
A simple C++11 thread pool library. 

A *thread pool* is a set of threads which get work from a queue of tasks. When a thread finishes executing a task it then checks if there is any work(task queue is not empty), gets a new task from the front of the queue and executes it. For the sake of simplicity I've implemented tasks by creating an abstract class *ThreadTask* which serves as an interface for tasks between the user and the thread pool. For debugging purposes I have also implemented a thread safe wrapper function for *printf* from the c stdio.h library and called it *tp_printf*.

# Usage
```c++
#include "ThreadPool.h" // including the library

class ExampleTask : public ThreadTask // an example task which only prints out "example"
{
public:
    virtual void run()
    {
        tp_printf("example");
    };
};

int main()
{
    ThreadPool tp;
  
    tp.pushTask(new ExampleTask()); // creating and pushing an example task which will be executed by some thread from the pool
}
```

The default constructor creates as many threads as there are logical processor cores (recommended).
A custom number of threads can be specified by passing the number of threads in the constructor.

The *ThreadTask* class is an interface for tasks, meaning one can program a task by simply inheriting the *ThreadTask* class and overriding the abstract function *run*. When creating the task object one must allocate it dynamically on the heap. 

Use the *pushTask* function to push a task into the queue to be extecuted.

Use the *getNumThreads* function to get the number of threads in the pool.

Use the  thread safe *tp_printf* global function(cstdio *printf* wrapper) for console printing while using the thread pool.
