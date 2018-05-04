CC=g++
OFF=offline
ON=online
AVL=avltree
FLAGS=-Wall
INCLUDE=-Iinclude -std=c++11

all: $(OFF) $(ON)

$(ON): $(ON).cpp $(AVL).o
	$(CC) $^ -o $(ON) $(FLAGS) $(INCLUDE)

$(OFF): $(OFF).cpp $(AVL).o
	$(CC) $^ -o $(OFF) $(FLAGS) $(INCLUDE)

$(AVL).o: $(AVL).cpp
	$(CC) -c $^ -o $(AVL).o $(FLAGS) $(INCLUDE)

clean:
	rm $(AVL).o
	rm $(OFF)
	rm $(ON)
	rm dictionary.txt
	rm postings.txt
	rm docids.txt