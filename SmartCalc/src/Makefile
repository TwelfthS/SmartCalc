all: install open

install: clean
	mkdir build
	cd build/ && qmake ../SmartCalc.pro && make

open:
	cd build/SmartCalc.app/Contents/MacOS/ && ./SmartCalc

uninstall:
	rm -rf build

dvi:
	doxygen -g doxyconfig
	doxygen doxyconfig
	open html/index.html

dist:
	tar -cf archive.tar *.c *.h *.pro *.cpp *.ui SmartCalc.pro.user

test:
	gcc tests.c s21_smart_calc.c -lcheck -o test
	-./test
	rm -rf *.o test

gcov_report:
	gcc -c s21_smart_calc.c --coverage
	gcc -c tests.c
	gcc s21_smart_calc.o tests.o -lcheck -lgcov -o test
	-./test
	lcov -o tests.info -c -d .
	genhtml -o report tests.info
	open ./report/index.html

clean:
	rm -rf *.o test report *.gcno *.gcda *.gcov *.info build html latex *.tar doxyconfig

style:
	clang-format -n -style=google *.c *.h *.cpp
	clang-format -i -style=google *.c *.h *.cpp
