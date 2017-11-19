all: hw1

hw1: hw1.src/hw1.cc hw1.src/hash_set_linked_list.hh hw1.src/hash_set_linear_probing.hh
	c++ -std=c++14 -O2 -o $@ $<
	@echo Run using ./hw1
clean:
	rm hw1 hw2 hw3 hw4 hw5

hw2:
	@echo Sorry, hw2 isn\'t ready yet. I will write you an e-mail when I finish it.
hw3:
	@echo Sorry, hw3 isn\'t ready yet. I will write you an e-mail when I finish it.
hw4: hw4.src/hw4.cc hw4.src/int16_set_bitvector.hh hw4.src/int16_set_trie.hh
	c++ -std=c++1z -O2 -Ihw4.src/bricks -o $@ $<
hw5:
	@echo Sorry, hw5 isn\'t ready yet. I will write you an e-mail when I finish it.
