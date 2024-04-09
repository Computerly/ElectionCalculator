#include "Candidate.h"
#include <stdexcept>

/**
 * @brief Creates a nameless candidate (which can be named later)
 * 
 */
Candidate::Candidate(){
    candidateName = "";
    numVotes = 0;
    lose = false;
    win = false;
}


/**
 * @brief Construct a new Candidate:: Candidate object
 * 
 * @param name is the name of the candidate
 */
Candidate::Candidate(std::string name)
{
    candidateName = name;
    numVotes = 0;
    lose = false;
    win = false;
}


/**
 * @brief Construct a new Candidate:: Candidate object
 * 
 * @param name is the name of the candidate
 * @param party is the name of the party the candidate belongs to
 */
Candidate::Candidate(std::string name, std::string party)
{
    candidateName = name;
    candidateParty = party;
    numVotes = 0;
    lose = false;
    win = false;
}


/**
 * @brief Gets the number of votes for the candidate
 * 
 * @return int (the number of votes)
 */
int Candidate::getNumVotes()
{
    return numVotes;
}


/**
 * @brief Increments the number of votes for the candidate by 1
 * 
 */
void Candidate::incrementVotes(int n /*= 1*/)
{
    numVotes += n;
}


/**
 * @brief Gets the name of the candidate
 * 
 * @return std::string (the name of the candidate)
 */
std::string Candidate::getName(){ 
    return candidateName; 
}


/**
 * @brief Gets the party of the candidate
 * 
 * @return std::string (the name of the party that the candidate belongs to)
 */
std::string Candidate::getParty()
{
    return candidateParty;
}

/**
 * @brief Sets the candidate's name to the parameter string
 * 
 * @param name is the new name of the candidate
 */
void Candidate::setName(std::string name)
{
    candidateName = name;
}

/**
 * @brief Sets the candidate's party to the parameter string
 * 
 * @param party is the new party of the candidate
 */
void Candidate::setParty(std::string party)
{
    candidateParty = party;
}

/**
 * @brief Adds the current number of votes for a candidate to the vnVoteHistory vector
 * 
 */
void Candidate::logVoteCount(){
    vnVoteHistory.push_back(numVotes);
}

/**
 * @brief Gets the vnVoteHistory vector
 * 
 * @return std::vector<int> (the vote history vector)
 */
std::vector<int> Candidate::getVoteHistory(){
    return vnVoteHistory;
}

/**
 * @brief Function to determine if a candidate is a winner based on the value of the win variable
 * 
 * @return true If win's value is true
 * @return false If win's value is false
 */
bool Candidate::isWinner() { return win; }

/**
 * @brief Function to determine if a candidate is a loser based on the value of the lose variable
 * 
 * @return true If lose's value is true
 * @return false If lose's value is false
 */
bool Candidate::isLoser() { return lose; }

/**
 * @brief Function to set the value of win
 * 
 * @param _win The value (true or false) to set win to
 */
void Candidate::setWinState(bool _win) { win = _win; }

/**
 * @brief Function to set the value of lose
 * 
 * @param _lose The value (true or false) to set lose to
 */
void Candidate::setLoseState(bool _lose) { lose = _lose; }

/**
 * @brief Function to set the numVotes variable
 * 
 * @param n The value to set numVotes equal to
 */
void Candidate::setVotes(int n){ numVotes = n; }
