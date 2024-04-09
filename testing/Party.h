#ifndef PARTY_H
#define PARTY_H

#include "Candidate.h"
#include <vector>
#include <string>


class Party
{
public:
    Party();
    Party(std::vector<Candidate>);
    Party(std::string partyName);
    Party(std::vector<Candidate>, std::string partyName);
	
    int getNumVotes();

    int getNumSeats();

    int getRemainder();

    std::string getPartyName();

    std::vector<Candidate> getCandidates();
    
    void incrementVotes(int n = 1);

    void addCandidate(Candidate c);

    void removeCandidate(Candidate c);

    void incrementSeats(int n = 1);

    void setRemainder(int n);
private:
    int numVotes;
    int nSeats;
    int nRemainder;

    std::string partyName;
    std::vector<Candidate> candidates;
};

#endif
