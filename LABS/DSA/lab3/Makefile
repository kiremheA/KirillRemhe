all: main

FLAGS = -g -O
main: *.o
	cc *.o $(FLAGS) -o $@

%.o: %.c
	cc -c $(FLAGS) *.c


run: 
	./main

clean: 
	rm *.o
	rm main 
