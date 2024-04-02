CC 	= gcc
CFLAGS 	= -g -Wall -Wextra 
OBJLOC = temp
SRCLOC = src
INTLOC = src/integration

all:	StarCoreKernel

$(OBJLOC)/task.o:	$(SRCLOC)/scheduler/task.c $(SRCLOC)/scheduler/task.h $(SRCLOC)/scheduler/status.h
	$(CC) -c $(CFLAGS) $(SRCLOC)/scheduler/task.c -o $(OBJLOC)/task.o

$(OBJLOC)/scheduler.o: $(SRCLOC)/scheduler/scheduler.c $(SRCLOC)/scheduler/status.h $(SRCLOC)/scheduler/task.h
	$(CC) -c $(CFLAGS) $(SRCLOC)/scheduler/scheduler.c -o $(OBJLOC)/scheduler.o

$(OBJLOC)/status.o: $(SRCLOC)/scheduler/status.c $(SRCLOC)/scheduler/status.h
	$(CC) -c $(CFLAGS) $(SRCLOC)/scheduler/status.c -o $(OBJLOC)/status.o

$(OBJLOC)/main.o:	$(SRCLOC)/main.c $(SRCLOC)/scheduler/task.h $(SRCLOC)/scheduler/status.h $(SRCLOC)/scheduler/scheduler.h
	$(CC) -c $(CFLAGS) $(SRCLOC)/main.c -o $(OBJLOC)/main.o

$(OBJLOC)/ADCS.o:	$(INTLOC)/ADCS.c $(INTLOC)/ADCS.h $(INTLOC)/virtual_intellisat.h $(INTLOC)/adcs_math/matrix.h $(INTLOC)/adcs_math/vector.h
	$(CC) -c $(CFLAGS) $(INTLOC)/ADCS.c -o $(OBJLOC)/ADCS.o

StarCoreKernel:	$(OBJLOC)/main.o $(OBJLOC)/task.o $(OBJLOC)/scheduler.o $(OBJLOC)/status.o $(OBJLOC)/ADCS.o
	$(CC) $(OBJLOC)/main.o $(OBJLOC)/task.o $(OBJLOC)/scheduler.o $(OBJLOC)/status.o $(OBJLOC)/ADCS.o -o StarCoreKernel

flushTemp:
	rm -fv $(OBJLOC)/*.o $(OBJLOC)/*~ 

clean:
	make flushTemp
	rm -fv StarCoreKernel
