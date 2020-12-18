NUM?=5
numbers=0 1 2 3 4 5 6 7
nums=1 2 3 5 6 7 8
tests=project$(NUM)-exampleIO

run:
	for number in $(numbers); \
	do \
		./lab$(NUM) $(tests)/5-80/input$$number.txt > $(tests)/out.txt ; \
		diff $(tests)/5-80/answer$$number.txt $(tests)/out.txt || (echo "diff failed on test $number \n") ; \
	done
	rm $(tests)/out.txt
	for num in $(nums); \
	do \
		./lab$(NUM) $(tests)54-100/input$$num.txt > $(tests)/out.txt ; \
		diff $(tests)/5-100/answer$$num.txt $(tests)/out.txt || (echo "diff failed on test $num \n") ; \
	done
	rm $(tests)/out.txt

compile:
	g++ -Wall -Werror -std=c++17 -g *.cpp -o lab$(NUM)
