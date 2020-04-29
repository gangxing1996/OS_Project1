CFLAG = -std=c99 -O2

all: main
main: main.o scheduling.o process.o priority.o call_wait.o rr_queue.o common.o
	gcc $(CFLAG) main.o scheduling.o process.o priority.o call_wait.o rr_queue.o common.o -o main
main.o: main.c
	gcc $(CFLAG) -c main.c
scheduling.o: scheduling.c scheduling.h
	gcc $(CFLAG) -c scheduling.c
process.o: process.c process.h
	gcc $(CFLAG) -c process.c
priority.o: priority.c priority.h
	gcc $(CFLAG) -c priority.c
call_wait.o: call_wait.c call_wait.h
	gcc $(CFLAG) -c call_wait.c
rr_queue.o: rr_queue.c rr_queue.h
	gcc $(CFLAG) -c rr_queue.c
common.o: common.c common.h
	gcc $(CFLAG) -c common.c

clean:
	rm ./*.o
	rm main
