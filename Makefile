all: hw1

hw1: hw1.src/hw1.cc hw1.src/hash_set_linked_list.hh hw1.src/hash_set_linear_probing.hh
	c++ -std=c++14 -O2 -o $@ $<
	@echo Run using ./hw1
clean:
	rm hw1

hw2:
	@echo Sorry, hw2 isn\'t ready yet. I will write you an e-mail when I finish it.
