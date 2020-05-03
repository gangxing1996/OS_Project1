CFLAG = -std=c99 -O2

all: main
main: main.o scheduling.o process.o priority.o call_wait.o rr_queue.o common.o
	gcc $(CFLAG) main.o scheduling.o process.o priority.o call_wait.o rr_queue.o common.o -o main
main.o: main.c
	gcc $(CFLAG) -c main.c
scheduling.o: scheduling.c
	gcc $(CFLAG) -c scheduling.c
process.o: process.c
	gcc $(CFLAG) -c process.c
priority.o: priority.c
	gcc $(CFLAG) -c priority.c
call_wait.o: call_wait.c
	gcc $(CFLAG) -c call_wait.c
rr_queue.o: rr_queue.c
	gcc $(CFLAG) -c rr_queue.c
common.o: common.c
	gcc $(CFLAG) -c common.c

clean:
	rm ./*.o
	rm main
