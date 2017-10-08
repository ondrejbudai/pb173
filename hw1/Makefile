all: hw1

hw1: main.cc hash_set_linked_list.hh hash_set_linear_probing.hh
	c++ -std=c++14 -O2 -o $@ $^
	@echo Run using make run or ./hw1

run: hw1
	./hw1

clean:
	rm hw1
