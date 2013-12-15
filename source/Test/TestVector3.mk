vpath % ../Mesh/
TestVector3 :  TestVector3.o
	g++ -o TestVector3 TestVector3.o
TestVector3.o : TestVector3.cpp Vector3.h
	g++ -c TestVector3.cpp
