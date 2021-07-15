TARGET="get_eccentricities"
PATH_TEST_GRAPH="../data_graphs/facebook_combined.txt"
PATH_TEST_GRAPH2="../data_graphs/com-youtube.ungraph.txt"
PATH_TEST_GRAPH3="../data_graphs/inet_remastered"
PATH_TEST_GRAPH4="../data_graphs/facebook_combined.txt"
PATH_TEST_GRAPH5="../data_graphs/musae_git_edges.txt"
PATH_TEST_GRAPH6="../data_graphs/as-skitter.txt"


all:
	gcc *.c -Wall -Wextra -I/usr/local/include/igraph/ -L/usr/local/lib -ligraph -lm -o $(TARGET) -O3

debug:
	gcc *.c -Wall -Wextra -I/usr/local/include/igraph/ -L/usr/local/lib -ligraph -lm -o $(TARGET) -g -fsanitize=address

check: all
	cd testsuite && python3 main.py ../$(PATH_TEST_GRAPH)

check_yt: all
	cd testsuite && python3 main.py ../$(PATH_TEST_GRAPH2)

check_inet: all
	cd testsuite && python3 main.py ../$(PATH_TEST_GRAPH3)

check_fb: all
	cd testsuite && python3 main.py ../$(PATH_TEST_GRAPH4)

check_git: all
	cd testsuite && python3 main.py ../$(PATH_TEST_GRAPH5)

check_skitter: all
	cd testsuite && python3 main.py ../$(PATH_TEST_GRAPH6)

check_debug: debug
	cd testsuite && python3 main.py ../$(PATH_TEST_GRAPH)


clean:
	rm $(TARGET)
