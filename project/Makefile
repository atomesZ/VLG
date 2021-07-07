TARGET="get_eccentricities"

all:
	gcc *.c -Wall -Wextra -I/usr/local/include/igraph/ -L/usr/local/lib -ligraph -lm -o $(TARGET) -O3


debug:
	gcc *.c -Wall -Wextra -I/usr/local/include/igraph/ -L/usr/local/lib -ligraph -lm -o $(TARGET) -g -fsanitize=address

clean:
	rm $(TARGET)
