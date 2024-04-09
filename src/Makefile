CXX=g++
CXXFLAGS = -std=c++17 -g

EXEC = all
OBJS = Main.o Candidate.o Party.o Election.o IRV.o OPL.o Combinations.o PO.o


${EXEC}: ${OBJS}
	${CXX} ${CXXFLAGS} -o ${EXEC} ${OBJS}


Candidate.o: Candidate.cpp Candidate.h
	${CXX} ${CXXFLAGS} -c Candidate.cpp

Party.o: Party.cpp Party.h
		${CXX} ${CXXFLAGS} -c Party.cpp

Election.o: Election.cpp Election.h
	${CXX} ${CXXFLAGS} -c Election.cpp
	
Combinations.o: helpers/Combinations.cpp helpers/Combinations.h
	${CXX} ${CXXFLAGS} -c helpers/Combinations.cpp

IRV.o: IRV.cpp IRV.h
	${CXX} ${CXXFLAGS} -c IRV.cpp

OPL.o: OPL.cpp OPL.h
	${CXX} ${CXXFLAGS} -c OPL.cpp

PO.o: PO.cpp PO.h
	${CXX} ${CXXFLAGS} -c PO.cpp

Main.o: Main.cpp
		${CXX} ${CXXFLAGS} -c Main.cpp

clean:
	rm -f ${EXEC} ${OBJS}
