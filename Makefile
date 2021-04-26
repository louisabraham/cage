example: example.cpp
	$(CXX) --std=c++17 -O3 example.cpp -o example

clean:
	-rm example

.PHONY: clean
