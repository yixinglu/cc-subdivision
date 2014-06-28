OBJECTS = ccsubdiv.obj helper.obj main.obj readobj.obj
vpath % ./
ccsubdiv : $(OBJECTS)
	g++ -o ccsubdiv $(OBJECTS)

ccsubdiv.obj : ccsubdiv.cpp datatype.h helper.h
	g++ -c ccsubdiv.cpp

helper.obj : helper.cpp helper.h 
	g++ -c helper.cpp

main.obj : main.cpp datatype.h readobj.h
	g++ -c main.cpp

readobj.obj : readobj.cpp helper.h
	g++ -c readobj.cpp

.PHONY : cleanall
cleanall :
	-rm -f *.o ccsubdiv

.PHONY : cleanobj
cleanobj :
	-rm -f *.o
