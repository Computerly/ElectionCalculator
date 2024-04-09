#include <stdexcept>
#include <iostream> /* CAN REMOVE */
#include "Candidate.h"

Candidate::Candidate(){
	candidateName = "";
	numVotes = 0;
	lose = false;
	win = false;
}

Candidate::Candidate(std::string name){
	candidateName = name;
	numVotes = 0;
	lose = false;
	win = false;
}

Candidate::Candidate(std::string name, std::string party){
	candidateName = name;
	candidateParty = party;
	numVotes = 0;
	lose = false;
	win = false;
}

int Candidate::getNumVotes(){ return numVotes; }

void Candidate::incrementVotes(int n /*= 1*/){ numVotes += n; }

void Candidate::decrementVotes(int n /*= 1*/){ numVotes -= n; }

std::string Candidate::getName(){ return candidateName; }

std::string Candidate::getParty(){ return candidateParty; }

void Candidate::setName(std::string name){ candidateName = name; }

void Candidate::setParty(std::string party){ candidateParty = party; }

void Candidate::logVoteCount(){ 
	vnVoteHistory.push_back(numVotes); // Log Vote Count
	// Log delta
	if(vnVoteHistory.size() == 1){
		vnVoteHistory.push_back(numVotes);
	}else if(vnVoteHistory.at(vnVoteHistory.size() - 1) != 0){
		vnVoteHistory.push_back( numVotes - vnVoteHistory.at(vnVoteHistory.size() - 2));
	}else{
		vnVoteHistory.push_back(0);
	}

}

std::vector<int> Candidate::getVoteHistory(){ return vnVoteHistory; }

bool Candidate::isWinner() { return win; }
bool Candidate::isLoser() { return lose; }

void Candidate::setWinState(bool _win) { win = _win; }
void Candidate::setLoseState(bool _lose) { lose = _lose; }

void Candidate::setVotes(int n){ numVotes = n; }
