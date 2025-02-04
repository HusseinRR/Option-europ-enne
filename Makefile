CXX = g++
CXXFLAGS = -O2 -std=c++11

OBJS = main.o assemble.o

# DEPS = Vecteur.h Matrice.h Maillage.h  // if needed

all: basket2d

basket2d: $(OBJS)
	$(CXX) $(CXXFLAGS) -o basket2d $(OBJS)

main.o: main.cpp 
	$(CXX) $(CXXFLAGS) -c main.cpp

assemble.o: assemble.cpp 
	$(CXX) $(CXXFLAGS) -c assemble.cpp

clean:
	rm -f *.o basket2d
