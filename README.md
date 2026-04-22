# coders:
Master POSIX threads (pthreads) and mutexes to manage shared states, avoid deadlocks, and prevent "burnout" (process starvation) under strict timing constraints.

Le but de Codexion est d'apprendre à maîtriser le multithreading (faire tourner des choses en même temps avec des threads)  et la synchronisation.

When you run a program on your computer (like a web browser, a video game, or your C program), the operating system creates what is called a process.
    The Process: This is simply the program currently executing. It is given its own memory space, its own variables, and its own code.

    The PID (Process IDentifier): This is a unique identification number that the operating system assigns to this process. This number allows your computer's system to track and manage the process (like giving it more memory or forcing it to stop).


If your program needs to do several things at the exact same time. You have two main options:

1 Create new processes (using fork())
When a process creates a new process (the "child"), it makes an exact copy of itself.
The child receives a new PID, completely different from the parent's PID.
Memory is separated: The child gets a full copy of all the parent's variables. But remember, these are just copies. If the child changes a variable in its own memory, it changes absolutely nothing in the parent's memory. They are completely isolated from one another.

2 Create Threads (using pthread_create())
A thread is a line of execution that lives inside an already existing process.
they all share the same PID.
Memory is shared: This is the most crucial difference. Threads do not copy memory. They all use the exact same central memory of the process. If Thread A modifies a variable, Thread B will see this change instantly because they are looking at the exact same physical spot in the computer's memory.



the process : /.codexion 
thread : codeurs 
mutext : cadnas qui va sur chaque dongle pour eviter que 2 thread ne prenne le meme dongle



The general idea is to pick the right tool for the job you are trying to do:

    Processes (separated): You use them when you want to run completely independent tasks that shouldn't interfere with each other (like launching two different programs). The advantage is safety (one process crashing won't break the other's memory), but communication between them is slow and complicated.

    Threads (shared): You use them when you have one big task that you want to split into smaller workers that collaborate on the same data (like your Codexion project). The advantage is that communication and data sharing are instant and very fast. The downside (and the big trap) is that you must be very careful to ensure two threads don't try to change the same piece of data at the exact same millisecond, otherwise the data gets corrupted. This is exactly why you are required to use tools like "mutexes" to protect your shared variables!



Race condition:
A race condition occurs when two threads try to modify the same shared variable at the exact same time, which completely corrupts the data.

Why does the computer make mistakes?
An operation, such as ++ (incrementing), is not instantaneous; it happens in 3 distinct steps for the processor:

    Read: The thread reads the current value from the shared program memory/RAM.

    Increment: The thread adds +1 to this value in its own internal register (the value inside the CPU).

    Write: The thread rewrites the new final value back into the shared memory.

The problem arises because the operating system can put a thread "on pause" at any given microsecond to let another thread work.

It is precisely because step 2 (the calculation) is done on a private scratchpad in the CPU that chaos ensues. If Thread 1 and Thread 2 read the same value from the table at the same time, they will both add +1 on their private scratchpads independently, without knowing what the other is doing. The last one to write its scratchpad back to the table will completely overwrite the work of the first one!



how to solve this issue us MUTEXT
pthread_mutex_t mutex;  --> creat mutex
pthread_mutex_init(&mutex, NULL) --> init mutex pourquoi faire cela apres la creation du mutex ?
pthread_mutex_destroy(&mutex) --> Free le mutex 
pthread_mutex_lock(&mutex); --> lock operation en cours protect against other thread trying to run the code at the same time 
phtread_mutex_unlock(&mutex) --> laisse la place libre 
pthread_join                 --> suspends the execution of the current thread until the specified thread terminates, allowing you to retrieve its return value and cleanly release its system resources.



to creat phread inside a loop:
phread th;
while i < nbr_coders
    if (phread_creat(th[i]), NULL, &routine, NULL) != 0
    perror("failed to creat thread")
    return -1;

while i < nbr_coders
    if (phread_join(th[i], NULL) != 0 )
        return ??,

pthread_mutex_destroy(&mutex);


do 2 loops because if you do in the same loop we will not have multithread 




get return of thread function into the main :

*void name_function()
int value = (valuer quon veut)
int* result = malloc(sizeof(int))

*result = value;
return (void*) result,

phread_join(th , (void **) &res)
take a function void **