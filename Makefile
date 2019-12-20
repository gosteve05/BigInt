CC = g++
CFLAGS = -Wall --std=c++11
OBJECTS = BigInt.o

BigInt: $(OBJECTS) #OBJECTS refers to this
	$(CC) $(CFLAGS) -o BigInt $(OBJECTS) -lsfml-graphics -lsfml-window -lsfml-system

BigInt.o: BigInt.cpp #needs space after colon
	$(CC) $(CFLAGS) -c BigInt.cpp -o BigInt.o

clean:
	rm BigInt $(OBJECTS)