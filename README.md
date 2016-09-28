# ThreadPool
A simple C++ thread pool header library. 

Default constructor creates as mayn threads as there are processor cores (recommended).
A custom number of threads can be specified by the user using the secondary constructor by passing the value in the constructor.

The ThreadTask class must be inherited and the abstract method run overriden by a custom run method.
