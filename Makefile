CXX = g++

FLAGS = -Wall -Wextra -pedantic-errors -Wcast-align -Wuseless-cast -Wconversion -Wduplicated-cond -Wduplicated-branches -Wformat=2 -Wshadow -Wnon-virtual-dtor

INCLUDE = includes/
SRC = src/
TARGET = target/

build:
	${CXX} -I ${INCLUDE} ${FLAGS} ${SRC}main.cpp ${SRC}pendule.cpp $(SRC)maestro.cpp -o ${TARGET}main

plot:
	typst compile --root=. ./target/plot/plot.typ ./target/plot.pdf

run:
	make -s build
	echo -e "lancement du programme\n\n"
	./${TARGET}main