#ifndef CANDIDATE_H
#define CANDIDATE_H

#include <string>
#include <vector>

class Candidate
{
public:
    Candidate();
    Candidate(std::string name);
    Candidate(std::string name, std::string party);
	
    int getNumVotes();

    bool isWinner();

    bool isLoser();

    void incrementVotes(int n = 1);

    void setName(std::string name);

    void setParty(std::string party);

    void setVotes(int n);

    void logVoteCount();

    void setWinState(bool _win);

    void setLoseState(bool _lose);

    std::string getName();

    std::string getParty();

    std::string candidateName;

    std::vector<int> getVoteHistory();
private:
    int numVotes;
    bool win;
    bool lose;
    std::string candidateParty;
    std::vector<int> vnVoteHistory;
};

#endif
