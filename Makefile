all:invertedIndex

first: invertedIndex.c
	gcc -Wall -Werror -fsanitize=address invertedIndex.c -o invertedIndex -lm

clean:
	rm -rf invertedIndex
