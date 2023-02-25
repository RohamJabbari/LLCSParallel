all: serial antidiagonal parallel-tasks parallel-tasks-v2 parallel-tasks-v3 parallel-taskloop parallel-taskloop-v2 parallel-taskloop-v3

serial:
	${CC} a2-llcs.c -o llcs-serial -O2 

antidiagonal:
	${CC} a2-llcs.c -o llcs-antidiagonal -O2 -fopenmp -D _ANTIDIAGONAL

parallel-tasks: 
	${CC} a2-llcs.c -o llcs-parallel-tasks -O2 -fopenmp -D _TASKS

parallel-tasks-v2: 
	${CC} a2-llcs.c -o llcs-parallel-tasks-v2 -O2 -fopenmp -D _TASKSV2

parallel-tasks-v3: 
	${CC} a2-llcs.c -o llcs-parallel-tasks-v3 -O2 -fopenmp -D _TASKSV3

parallel-taskloop: 
	${CC} a2-llcs.c -o llcs-parallel-taskloop -O2 -fopenmp -D _TASKLOOP

parallel-taskloop-v2: 
	${CC} a2-llcs.c -o llcs-parallel-taskloop-v2 -O2 -fopenmp -D _TASKLOOPV2

parallel-taskloop-v3: 
	${CC} a2-llcs.c -o llcs-parallel-taskloop-v3 -O2 -fopenmp -D _TASKLOOPV3

clean:
	rm llcs-serial
	rm llcs-antidiagonal
	rm llcs-parallel-tasks
	rm llcs-parallel-tasks-v2
	rm llcs-parallel-tasks-v3
	rm llcs-parallel-taskloop
	rm llcs-parallel-taskloop-v2
	rm llcs-parallel-taskloop-v3

