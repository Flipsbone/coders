*This project has been created as part of the 42 curriculum by advacher.*

## Description
Codexion is a multithreading and synchronization simulation written in C. It models a shared workspace where multiple coders must coordinate to use limited resources (USB dongles). Each coder alternates between compiling, debugging, and refactoring. To compile their quantum code, a coder must acquire two dongles simultaneously. The simulation's objective is to manage resource synchronization efficiently to prevent data corruption, deadlocks, and process starvation (burnout).

## Instructions
To compile the project, run `make` at the root of the repository.

Execute the program with the following mandatory arguments:
`./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler`

**Note:** The `scheduler` argument must be strictly set to either `fifo` or `edf`.

## Thread synchronization mechanisms
This project relies on POSIX threads (`pthreads`) and synchronization primitives to manage concurrent access to shared resources:

* **Threads (`pthread_create`)**: Each coder is represented by an individual thread. An independent monitor thread is also created to oversee the simulation's state.
* **Mutexes (`pthread_mutex_t`)**: Used to ensure exclusive access to shared resources. 
  * Each dongle is protected by its own mutex to prevent concurrent access by multiple coders. 
  * A `sim_mutex` protects the global simulation variables (like the start flag and the stop condition).
  * A `print_mutex` serializes the standard output to ensure that log messages are printed cleanly and never interleave on a single line.
* **Condition Variables (`pthread_cond_t`)**: Used for efficient thread synchronization without busy-waiting.
  * A global `start_cond` acts as a synchronization barrier: all coder threads are put to sleep upon creation and are awakened simultaneously via `pthread_cond_broadcast` by the main thread to ensure a fair start.
  * Condition variables are also used to put coders to sleep while they wait in the queue for a requested dongle.

## Blocking cases handled
It is crucial to understand the difference between how coders request dongles, and how the simulation grants them.

* **Deadlock Prevention (Coffman's conditions)**: To prevent circular wait (where coders hold one dongle and indefinitely wait for the second), the program implements an Even/Odd acquisition strategy.

  * Odd-numbered coders always attempt to lock their left dongle first. 
  * Even-numbered coders lock their right dongle first. This guarantees that at least one coder can acquire both dongles, entirely avoiding deadlocks even with a large number of coders.

* **Scheduling**: When multiple coders request the same dongle, the dongle grants access based on the chosen arbitration policy (`scheduler`):
  * `fifo`: The dongle is granted to the coder whose request arrived first.
  * `edf` (Earliest Deadline First): The dongle is granted to the coder closest to their burnout deadline.
* **Cooldown Handling**: Once a coder finishes compiling and releases their dongles, the dongles are marked as unavailable until the mandatory `dongle_cooldown` duration has passed.
* **Burnout Detection**: The dedicated monitor thread runs in a continuous loop to check every coder's timestamps. If it detects that a coder has missed their compile deadline (`now - last_compile_start > time_to_burnout`), it immediately flags the simulation to stop, broadcasts a signal to wake up any sleeping threads for a clean exit, and prints the burnout log within the required 10 ms precision.

## Resources
* Unix `pthread` documentation (man pages for mutexes and condition variables).
* **Multithreading in C:** [CodeVault - Pthreads & Multithreading Courses](https://www.youtube.com/@CodeVault/courses)
* **AI Usage**: Artificial Intelligence was used to brainstorm deadlock prevention strategies, specifically transitioning from an ID-based hierarchy to the Even/Odd acquisition algorithm, and to review the English phrasing of this README.

## Example Usage

## Project Architecture
Below is a visual representation of how the different components of **Codexion** interact:
```mermaid
graph TD
    %% Define Styles
    classDef thread fill:#e1f5fe,stroke:#03a9f4,stroke-width:2px;
    classDef resource fill:#fff3e0,stroke:#ff9800,stroke-width:2px;
    classDef monitor fill:#ffebee,stroke:#f44336,stroke-width:2px;
    classDef action fill:#f1f8e9,stroke:#8bc34a,stroke-width:1px;

    %% Main Process
    subgraph Main_Process ["Main Thread"]
        A["Initialization & Parsing"] --> B["Create Monitor Thread"]
        A --> C["Create Coder Threads"]
        B -.Waiting.-> Barrier(("Start Barrier<br/>is_ready + cond_wait"))
        C -.Waiting.-> Barrier
        A -->|"Broadcast Signal (GO!)"| Barrier
    end

    %% Monitor Thread
    subgraph Monitor_Thread ["Monitor Thread"]
        Barrier --> M1["Monitoring Loop"]
        M1 --> M2{"Check Status"}
        M2 -->|"now - last_compile > burnout"| M3["Burnout Detected!"]
        M2 -->|"All required compiles done"| M4["Simulation Finished"]
        M2 -->|"All good"| M1
        M3 --> M5["Set stop_simulation = true"]
        M4 --> M5
        M5 --> M6["Broadcast & Wake up all coders"]
        M6 --> M7["End Monitor"]
    end

    %% Coder Thread
    subgraph Coder_Thread ["Coder Thread x N"]
        Barrier --> C1{"stop_simulation == true?"}
        C1 -->|Yes| C_End["Clean Thread Exit"]
        C1 -->|No| C2["ft_take_dongles"]
        
        %% The New Logic: Deadlock Prevention Strategy
        C2 --> C3{"Is Coder ID Even or Odd?"}
        C3 -->|Odd ID| C3_Odd["1. Lock Left Dongle<br/>2. Lock Right Dongle"]
        C3 -->|Even ID| C3_Even["1. Lock Right Dongle<br/>2. Lock Left Dongle"]
        
        %% The Scheduler Queue
        C3_Odd -.->|"Wait in Queue (FIFO or EDF)"| Shared_Dongles
        C3_Even -.->|"Wait in Queue (FIFO or EDF)"| Shared_Dongles
        
        Shared_Dongles -.->|Access Granted| C4["Compiling"]
        
        C4 --> C5["ft_drop_dongles"]
        C5 -.->|"Update Cooldown & Release"| Shared_Dongles
        
        C5 --> C6["Debugging"]
        C6 --> C7["Refactoring"]
        C7 --> C1
    end

    %% Shared Resources (Mutexes)
    subgraph Shared_Resources ["Shared Resources"]
        Shared_Dongles[("Dongles Array<br/>Mutex + Wait Queue")]
        Sim_Mutex[("sim_mutex<br/>Protects flags & timestamps")]
        Print_Mutex[("print_mutex<br/>Log serialization")]
    end

    %% Interactions with Shared Resources
    C4 -.->|"Update last_compile"| Sim_Mutex
    M2 -.->|"Read last_compile"| Sim_Mutex
    
    C4 -.->|"printf 'is compiling'"| Print_Mutex
    C6 -.->|"printf 'is debugging'"| Print_Mutex
    C7 -.->|"printf 'is refactoring'"| Print_Mutex
    M3 -.->|"printf 'burned out'"| Print_Mutex

    %% Apply Styles (CORRECTED)
    class A,B,C thread;
    class M1,M2,M3,M4,M5,M6,M7 monitor;
    class C1,C2,C3,C3_Odd,C3_Even,C4,C5,C6,C7,C_End action;
    class Shared_Dongles,Sim_Mutex,Print_Mutex resource;