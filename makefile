P2.out : Project2.o dataArrays.o LexicalAnalyzer.o SyntacticalAnalyzer.o
	g++ -g -o P2.out Project2.o dataArrays.o LexicalAnalyzer.o SyntacticalAnalyzer.o

Project2.o : Project2.cpp SyntacticalAnalyzer.h
	g++ -g -c Project2.cpp

dataArrays.o : dataArrays.cpp
	g++ -g -c dataArrays.cpp

LexicalAnalyzer.o : LexicalAnalyzer.cpp LexicalAnalyzer.h
	g++ -g -c LexicalAnalyzer.cpp

SyntacticalAnalyzer.o : SyntacticalAnalyzer.cpp SyntacticalAnalyzer.h LexicalAnalyzer.h
	g++ -g -c SyntacticalAnalyzer.cpp

clean : 
	rm *.o P2.out *.gch

submit : Project2.cpp dataArrays.cpp LexicalAnalyzer.h LexicalAnalyzer.cpp SyntacticalAnalyzer.h SyntacticalAnalyzer.cpp makefile README.txt
	rm -rf ReynaAP2
	mkdir ReynaAP2
	cp Project2.cpp ReynaAP2
	cp dataArrays.cpp ReynaAP2
	cp LexicalAnalyzer.h ReynaAP2
	cp LexicalAnalyzer.cpp ReynaAP2
	cp SyntacticalAnalyzer.h ReynaAP2
	cp SyntacticalAnalyzer.cpp ReynaAP2
	cp makefile ReynaAP2
	cp README.txt ReynaAP2
	tar cfvz ReynaAP2.tgz ReynaAP2
	cp ReynaAP2.tgz ~tiawatts/cs460drop
