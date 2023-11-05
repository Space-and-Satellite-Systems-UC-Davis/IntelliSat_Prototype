CC 	= gcc
CFLAGS 	= -g -Wall -Wextra
# -Werror

all:	demo

main.o:	main.c scheduler/task.h scheduler/status.h scheduler/scheduler.h
	$(CC) -c $(CFLAGS) main.c

task.o:	scheduler/task.c scheduler/task.h scheduler/status.h
	$(CC) -c $(CFLAGS) scheduler/task.c

scheduler.o: scheduler/scheduler.c scheduler/status.h scheduler/task.h
	$(CC) -c $(CFLAGS) scheduler/scheduler.c

status.o: scheduler/status.c scheduler/status.h
	$(CC) -c $(CFLAGS) scheduler/status.c

demo:	main.o task.o scheduler.o status.o
	$(CC) main.o task.o scheduler.o status.o -o demo

clean:
	rm -f *.o *~ core demo