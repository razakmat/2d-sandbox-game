CXX=g++
CXXFLAGS= -std=c++11 -Wall -pedantic -Wno-long-long -O0 -ggdb
CXXLIBS= -lSDL2 -lSDL2main -lSDL2_image

all: compile doc
	
compile: ./src/main.o ./src/CGame.o ./src/CMap.o ./src/CObject.o ./src/CEffect.o ./src/CVector.o ./src/CTile.o ./src/CInput.o
	$(CXX) $(CXXFLAGS) $^ -o razakmat $(CXXLIBS)

run: 
	./razakmat

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

./src/main.o: ./src/main.cpp ./src/CGame.h ./src/CException.h	
./src/CGame.o: ./src/CGame.cpp ./src/CMap.h ./src/CObject.h ./src/CException.h	
./src/CMap.o: ./src/CMap.cpp ./src/CTile.h ./src/CGame.h
./src/CObject.o: ./src/CObject.cpp ./src/CVector.h ./src/CMap.h ./src/CInput.h
./src/CEffect.o: ./src/CEffect.cpp ./src/CObject.h
./src/CVector.o: ./src/CVector.cpp
./src/CTile.o: ./src/CTile.cpp ./src/CEffect.h
./src/CInput.o: ./src/CInput.cpp ./src/CVector.h

doc:
	doxygen Doxyfile

clean:	
	rm ./src/*.o 
	rm razakmat
	rm -R ./doc


