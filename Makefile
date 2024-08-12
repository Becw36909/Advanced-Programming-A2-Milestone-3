.default: all

all: qwirkle.exe

clean:
	rm -rf qwirkle.exe *.o *.dSYM

qwirkle.exe: qwirkle.o Tile.o Node.o LinkedList.o TileBag.o Player.o FileHandler.o Rules.o InputValidator.o Student.o GameBoard.o Tests.o
	g++ -Wall -Werror -std=c++14 -g -O -o $@ $^

%.o: %.cpp
	g++ -Wall -Werror -std=c++14 -g -O -c $<
