all: s21_cat

s21_cat.o: s21_cat.c
	gcc -Wall -Werror -Werror -c s21_cat.c -o s21_cat.o

s21_cat: s21_cat.o
	gcc s21_cat.o -o s21_cat

clean:
	rm -rf s21_cat.o
	rm -rf s21_cat

rebuild: clean all