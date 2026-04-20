CXX = g++

FLAGS = -Wall -Wextra -pedantic-errors -Wcast-align -Wuseless-cast -Wconversion -Wduplicated-cond -Wduplicated-branches -Wformat=2 -Wshadow -Wnon-virtual-dtor

INCLUDE = includes/
SRC = src/
TARGET = target/

build:
	${CXX} -O3 -I ${INCLUDE} ${FLAGS} ${SRC}main.cpp ${SRC}pendule.cpp $(SRC)maestro.cpp $(SRC)tui.cpp -o ${TARGET}polyekkreme

run:
	make -s build
	echo -e "lancement du programme\n\n"
	./${TARGET}polyekkreme

plottraj:
	typst compile --root=. ./src/ploting/plottraj.typ ./target/plot/plottraj.pdf

plotenergie:
	typst compile --root=. ./src/ploting/plot_energie.typ ./target/plot/plot_energie.pdf

plotphase:
	typst compile --root=. ./src/ploting/plot_portrait_phase.typ ./target/plot/plot_portrait_phase.pdf

plot:
	make -s plottraj
	make -s plotenergie
	make -s plotphase

run_et_plot:
	make -s run
	make -s plot