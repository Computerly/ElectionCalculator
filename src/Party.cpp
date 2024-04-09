#include "Party.h"

#include <stdexcept>

Party::Party(std::string name){
	partyName = name;
	numVotes = 0;
	nSeats = 0;
	nRemainder = 0;
}

int Party::getNumVotes(){ return numVotes; }

int Party::getHistory(int n){ return vnHistory.at(n); }

std::string Party::getPartyName(){ return partyName; }

int Party::getNumSeats(){ return nSeats; }

int Party::getRemainder(){ return nRemainder; }

void Party::setRemainder(int n) { nRemainder = n; }

void Party::incrementVotes(int n /*= 1*/){ numVotes += n; }

void Party::incrementSeats(int n /*= 1*/){ nSeats += n; }

void Party::log(int n){ vnHistory.push_back( n ); }
