# ThreadPool
A simple C++11 thread pool library. 

A *thread pool* is a set of threads which get work for a queue of tasks. When a thread finishes a task it then checkes if there is any work(task queue is not empty), gets the task from the front of the queue and then executes the task itself. For the sake of simplicity I decided to implement task by creating an abstract class *ThreadTask* which serves as an interface between the user and the thread pool for tasks. 

# Usage
Default constructor creates as many threads as there are processor cores (recommended).
A custom number of threads can be specified by the user using the secondary constructor by passing the value of threads in the constructor.

The *ThreadTask* class is an interface for tasks, meaning one can program a task by simply inheriting the *ThreadTask* class and overriding the abstract method *run*. When creating the task object one must allocate it dynamically on the heap. 

Use the *pushTask* function to push a task into the queue to be extecuted.

Use the *getNumThreads* function to get the number of threads in the pool.

Use the  thread safe *tp_printf* global function(stdio *printf* wrapper) for console printing while using the thread pool.
