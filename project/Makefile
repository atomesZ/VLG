TARGET="get_eccentricities"
PATH_TEST_GRAPH="../data_graphs/facebook_combined.txt"

all:
	gcc *.c -Wall -Wextra -I/usr/local/include/igraph/ -L/usr/local/lib -ligraph -lm -o $(TARGET) -O3

debug:
	gcc *.c -Wall -Wextra -I/usr/local/include/igraph/ -L/usr/local/lib -ligraph -lm -o $(TARGET) -g -fsanitize=address

check: all
	cd testsuite && python3 main.py ../$(PATH_TEST_GRAPH)

check_debug: debug
	cd testsuite && python3 main.py ../$(PATH_TEST_GRAPH)

clean:
	rm $(TARGET)
