all:
	gcc main.cpp set.cpp row.cpp hand.cpp -o drop

clean:
	rm -f drop *.o
