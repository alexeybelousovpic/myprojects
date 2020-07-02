snake: main.c snake.c snake.h main.h
	gcc -lncurses main.c snake.c -o snake

clean:
	rm -rf *.o a.out snake
