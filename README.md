# ThreadPool
A simple C++11 thread pool library. 

Default constructor creates as many threads as there are processor cores (recommended).
A custom number of threads can be specified by the user using the secondary constructor by passing the value of threads in the constructor.

Use the *pushTask* function to push a task into the queue to be extecuted.

Use the *getNumThreads* function to get the number of threads in the pool.

Use the  thread safe *tp_printf* global function(stdio *printf* wrapper) for console printing while using the thread pool.

The *ThreadTask* class must be inherited and the abstract method run overriden by a custom run method.
