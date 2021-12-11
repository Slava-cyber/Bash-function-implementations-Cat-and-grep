all: s21_grep

s21_grep.o: s21_grep.c
	gcc -Wall -Werror -Werror -c s21_grep.c -o s21_grep.o
	
s21_grep: s21_grep.o
	gcc s21_grep.o -o s21_grep

clean:
	rm -rf s21_grep.o
	rm -rf s21_grep

rebuild: clean all