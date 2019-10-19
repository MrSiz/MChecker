all:
	g++ -o test -std=c++03 mchecker.cc test.cc -Wall

clean:
	rm test
