TARGET="get_eccentricities"

all:
	gcc *.c -I/usr/local/include/igraph/ -L/usr/local/lib -ligraph -lm -o $(TARGET)


debug:
	gcc *.c -I/usr/local/include/igraph/ -L/usr/local/lib -ligraph -lm -o $(TARGET) -g

clean:
	rm $(TARGET)


