vpath % ../Mesh/
#VPATH = ../Mesh/
TestVertex : TestVertex.o Vertex.o
	g++ -o TestVertex TestVertex.o Vertex.o
TestVertex.o : TestVertex.cpp Vertex.h
	g++ -c TestVertex.cpp
Vertex.o : Vertex.cpp Vertex.h Point3.h Vector3.h
	g++ -c ../Mesh/Vertex.cpp
