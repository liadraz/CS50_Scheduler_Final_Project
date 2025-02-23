# Simple Scheduler

### This project is the Final Project of the CS50 Harvard University online course
Written in C language, run and tested on Linux Ubuntu 24v OS <br>

<br>

## Video Demo URL
https://youtu.be/h4Dk4DDv4f4

<br>

# Description
This is a program that executes several operations (tasks) at predetermined time intervals.

This project is an experiment on how a modern Operating System synchronized scheduler works.

<br>

# API Design

The scheduler program is built on several APIs:
* Scheduler - The main public API, `scheduler.h`
* UID - Each task gets a unique ID, which is created when a task is being added to the scheduler, `uid.h`
* Container - Added tasks are sorted in a priority queue data structure - `pqueue.h`. The priority-queue is a wrapper API built on a sorted list module - `sorted_list.h`, that is based on a doubly linked-list module - `dlinked_list.h`.

> Usage explanation below describes how to build and use the scheduler API. <br>
> For further information on a specific module see the `./include` directory.

</br>

# Usage

The user can only access the Scheduler and the UID APIs.
Other modules are being built as a library archive consisting of executable objects.

<br>

## Create The Scheduler
Starting with creating a scheduler. Invoke `SchedCreate()` to create it.

```c
scheduler_ty *SchedCreate(void);
```

RETURN:
```c
struct scheduler
{
    pqueue_ty 	*tasks;
    time_t 		initial_time;
    task_ty 	*current_task;
    int 		should_run;
};
```

<br>

## Adding A Task
Invoke `SchedAdd()` to add a task to the Scheduler.
Each added task gets a unique id and is inserted into the priority queue container.
The pqueue sorts the tasks by time priority. The closest task time to be executed (interval time added to current time) will be pulled from the queue and executed first.

```c
sched_id_ty SchedAdd(scheduler_ty *scheduler, TaskFunc add_task, void *params, time_t interval);
```

PARAMETERS
- `scheduler`, The scheduler you refer to.
- `add_task`, A function pointer to a defined custom task. `See *Creating A TaskFunc`
- `params`, The parameters provided to `TaskFunc`.
- `interval`, The time interval in which each task will be pulled from the queue and executed. Represented in seconds.

RETURN :
```c
struct uid
{
    size_t counter;
    time_t time;
    pid_t pid;
};
```

> NOTE
> - Tasks can be added only when the scheduler is not RUNNING.
> - Each task has an interval time that determines when the task will be executed.
> - Time interval determined in seconds.
> - Task is being executed and returns to the scheduler unless the user asked to remove it from the queue.
> - Tasks can remove themselves or other tasks. (`See Removing A Task`)

Task Struct Definition:
```c
struct task
{
    TaskFunc	task_func_p;
    void	 	*params;
    time_t	 	interval;
    time_t 		next_run;
    uid_ty 		id;
};
```


### * Creating A TaskFunc

Tasks are custom functions that the user implements in his code. When adding a task to the scheduler the user must provide a function pointer of that task.

```c
typedef int (*TaskFunc)(void *params);
```

PARAMETERS
- `params`, additional parameters that the task uses to execute its logic. It can refer to a primitive built-in or a struct.

RETURN status


<br>

## Removing A Task
Invoke `SchedRemove()` to remove a task from the Scheduler.

```c
int SchedRemove(scheduler_ty *scheduler, uid_ty to_remove);
```

PARAMETERS
- `scheduler`, The scheduler you refer to.
- `uid_ty`, The task uid you want to remove.

RETURN status

> NOTE
> - Tasks can remove themselves or other tasks.
> - Removing all tasks from the scheduler will cause it to stop running.

<br>

## RUN
To start running the scheduler invoke `SchedRun`. The scheduler will run forever or until the user will call to pause it.

```c
enum run_status_ty SchedRun(scheduler_ty *scheduler);
```

PARAMETERS
- `scheduler`, The scheduler you refer to.

RETURN
```c
enum run_status_ty
{
	EMPTY = 0,
	STOPED = 1
};
```

> NOTE
> - In case the pqueue container is empty from tasks the scheduler will not start running.


<br>

## Pause
Invoke `SchedPause()` to stop the scheduler from running.

```c
void SchedPause(scheduler_ty *scheduler);
```

PARAMETERS
- `scheduler`, The scheduler you refer to.

> NOTE
> - You can stop the scheduler only by calling the `SchedPause()` function or terminating the process by calling ctrl+c (SIGINT).


<br>

## Destroy The Scheduler
At last, invoke `SchedDestroy` to destroy the scheduler and free all the allocated memory.

```c
void SchedDestroy(scheduler_ty *scheduler);
```

PARAMETERS
- `scheduler`, The scheduler you want to destroy.

> NOTE
> - avoid memory leaks by calling SchedDestory

<br>

## Run The Example Code
To try out the example code, open a terminal and run `make`. Then run the output executable:

```bash
    $ make
    $ ./run.out
```


## Tests
To perform the test run `make` first then run `./test.out` file.

```bash
    $ make
    $ ./test.out
```
For other modules tests see scripts in `test/` directory.
Each test file consists of a copy compile line that produces an executable.


