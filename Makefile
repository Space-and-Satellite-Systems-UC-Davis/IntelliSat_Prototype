CC 	= gcc
CFLAGS 	= -g -Wall -Wextra 
OBJLOC = temp
SRCLOC = src

all:	StarCoreKernel

$(OBJLOC)/main.o:	src/main.c src/scheduler/task.h src/scheduler/status.h src/scheduler/scheduler.h
	$(CC) -c $(CFLAGS) $(SRCLOC)/main.c -o $(OBJLOC)/main.o

$(OBJLOC)/task.o:	src/scheduler/task.c src/scheduler/task.h src/scheduler/status.h
	$(CC) -c $(CFLAGS) $(SRCLOC)/scheduler/task.c -o $(OBJLOC)/task.o

$(OBJLOC)/scheduler.o: src/scheduler/scheduler.c src/scheduler/status.h src/scheduler/task.h
	$(CC) -c $(CFLAGS) $(SRCLOC)/scheduler/scheduler.c -o $(OBJLOC)/scheduler.o

$(OBJLOC)/status.o: src/scheduler/status.c src/scheduler/status.h
	$(CC) -c $(CFLAGS) $(SRCLOC)/scheduler/status.c -o $(OBJLOC)/status.o

StarCoreKernel:	$(OBJLOC)/main.o $(OBJLOC)/task.o $(OBJLOC)/scheduler.o $(OBJLOC)/status.o
	$(CC) $(OBJLOC)/main.o $(OBJLOC)/task.o $(OBJLOC)/scheduler.o $(OBJLOC)/status.o -o StarCoreKernel 

flushTemp:
	rm -fv $(OBJLOC)/*.o $(OBJLOC)/*~ 

clean:
	make flushTemp
	rm -fv StarCoreKernel
