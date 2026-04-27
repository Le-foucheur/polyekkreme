CXX = g++

FLAGS = -O3 -Wall -Wextra -pedantic-errors -Wcast-align -Wuseless-cast -Wconversion -Wduplicated-cond -Wduplicated-branches -Wformat=2 -Wshadow -Wnon-virtual-dtor -fsanitize=address,undefined
INCLUDE = includes/
SRC = src/
TARGET = target/

build:
	@if [ ! -d ./target/plot/ ]; then\
		mkdir ./target/plot;\
	fi
	
	@if [ ! -d ./target/data/ ]; then\
		mkdir ./target/data;\
	fi
	
	${CXX} -I ${INCLUDE} ${FLAGS} ${SRC}main.cpp ${SRC}pendule.cpp $(SRC)maestro.cpp $(SRC)tui.cpp -o ${TARGET}polyekkreme

run:
	make -s build
	./${TARGET}polyekkreme

plottraj:
	typst compile --root=. ./src/ploting/plottraj.typ ./target/plot/plot_trajectoire.pdf

plotenergie:
	typst compile --root=. ./src/ploting/plot_energie.typ ./target/plot/plot_energie.pdf

plotphase:
	typst compile --root=. ./src/ploting/plot_portrait_phase.typ ./target/plot/plot_portrait_phase.pdf

plot:
	make -s plottraj
	make -s plotenergie
	make -s plotphase

run_plot:
	make -s run
	make -s plot

clean:
	rm -rf ./target/
	mkdir target