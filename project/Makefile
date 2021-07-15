TARGET="get_eccentricities"
PATH_TEST_GRAPH="../data_graphs/as-skitter.txt "
PATH_TEST_GRAPH2="../data_graphs/com-youtube.ungraph.txt"
PATH_TEST_GRAPH3="../data_graphs/inet_remastered"

all:
	gcc *.c -Wall -Wextra -I/usr/local/include/igraph/ -L/usr/local/lib -ligraph -lm -o $(TARGET) -O3

debug:
	gcc *.c -Wall -Wextra -I/usr/local/include/igraph/ -L/usr/local/lib -ligraph -lm -o $(TARGET) -g -fsanitize=address

check: all
	cd testsuite && python3 main.py ../$(PATH_TEST_GRAPH)

check2: all
	cd testsuite && python3 main.py ../$(PATH_TEST_GRAPH2)

check3: all
	cd testsuite && python3 main.py ../$(PATH_TEST_GRAPH3)

check_debug: debug
	cd testsuite && python3 main.py ../$(PATH_TEST_GRAPH)

clean:
	rm $(TARGET)
