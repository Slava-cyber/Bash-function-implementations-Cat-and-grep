
s21_cat: cat/s21_cat.o
	gcc cat/s21_cat.o -o cat/a

s21_cat.o: s21_cat.c
	gcc -Wall -Werror -Wextra -c cat/s21_cat.c -o cat/s21_cat.o

s21_grep: grep/s21_grep.o
	gcc grep/s21_grep.o -o grep/a

s21_grep.o: s21_grep.o
	gcc -Wall -Wextra -Werror -c grep/s21_grep.c -o grep/s21_grep.o

check_cat:
	cppcheck cat/s21_cat.c
	CK_FORK=no leaks --atExit -- cat/a

check_grep:
	cppcheck grep/s21_grep.c
	CK_FORK=no leaks --atExit -- grep/a

clean:
	rm -rf cat/s21_cat.o grep/s21_grep.o
	rm -rf cat/a grep/a
	rm -rf a.out
	rm -rf *.o
