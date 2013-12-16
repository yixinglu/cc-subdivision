OBJECTS = test.o Vertex.o
vpath % ../Mesh/
test : $(OBJECTS)
	g++ -o test $(OBJECTS)

test.o : test.cpp Vector3.h Vertex.h
	g++ -c test.cpp

Vector3.o : Vertex.cpp Vector3.h Point3.h
	g++ -c ../Mesh/Vertex.cpp

.PHONY : cleanall
cleanall :
	-rm -f *.o test

.PHONY : cleanobj
cleanobj :
	-rm -f *.o
