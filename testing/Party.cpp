#include "Party.h"
#include <stdexcept>

/**
 * @brief Construct a new Party:: Party object
 * 
 * Automatically assigns the name to be "Independent"
 */
Party::Party()
{
    numVotes = 0;
    partyName = "Independent";
}


/**
 * @brief Construct a new Party:: Party object
 * 
 * @param name is the name of the party
 */
Party::Party(std::string name)
{
    partyName = name;
    numVotes = 0;
}


/**
 * @brief Construct a new Party:: Party object
 * 
 * @param candidateList is the list of candidates belonging to the party
 * 
 * Automatically assigns the name to be "Independent"
 */
Party::Party(std::vector<Candidate> candidateList)
{
    partyName = "Independent";
    numVotes = getNumVotes();
    candidates = candidateList;
}

/**
 * @brief Construct a new Party:: Party object
 * 
 * @param candidateList is the list of candidates belonging to the party
 * @param name is the name of the party
 */
Party::Party(std::vector<Candidate> candidateList, std::string name)
{
    partyName = name;
    numVotes = getNumVotes();
    candidates = candidateList;
}


/**
 * @brief Gets the total number of votes for the party
 *        by summing all the candidate's votes
 * 
 * @return int (the total number of votes for the party)
 */
int Party::getNumVotes()
{
    int sumOfVotes = 0;
    for(int i = 0; i < this->candidates.size(); i++)
    {
        sumOfVotes += this->candidates.at(i).getNumVotes();
    }

    return sumOfVotes;
}


/**
 * @brief Gets the party's name (e.g. Democrat, Republican, etc.)
 * 
 * @return std::string (the party name)
 */
std::string Party::getPartyName()
{
    return partyName;
}


/**
 * @brief Gets the candidates vector of the party
 * 
 * @return std::vector<Candidate> (the candidates vector)
 */
std::vector<Candidate> Party::getCandidates()
{
    return candidates;
}


/**
 * @brief Adds the candidate c to the candidates vector
 * 
 * @param c is the candidate to be added
 */
void Party::addCandidate(Candidate c)
{
    candidates.push_back(c);
}


/**
 * @brief Removes the candidate c from the candidates vector
 * 
 * @param c is the candidate to be removed
 */
void Party::removeCandidate(Candidate c)
{
    std::string nm = c.getName();    //name of candidate to be removed

    for(int i = 0; i < candidates.size(); i++)
    {
        //if the candidate's name in the vector equals the one to be removed, remove them
        if(candidates.at(i).getName() == nm)
        {
            candidates.erase(candidates.begin() + i);
        }
    }
}

/**
 * @brief Gets the number of seats allocated to the party
 * 
 * @return int (the number of seats)
 */
int Party::getNumSeats(){ return nSeats; }

/**
 * @brief Gets the remainder of the number of votes for a party divided by the quota (for OPL)
 * 
 * @return int (the remainder)
 */
int Party::getRemainder(){ return nRemainder; }

/**
 * @brief Sets the remainder to the input value n
 * 
 * @param n The remainder value to set the remainder variable to
 */
void Party::setRemainder(int n) { nRemainder = n; }

/**
 * @brief Increments the numVotes variable for the party by the parameter n
 * 
 * @param n The number of votes to increment numVotes by
 */
void Party::incrementVotes(int n /*= 1*/){ numVotes += n; }

/**
 * @brief Increments the nSeats variable for the party by the parameter n
 * 
 * @param n The number of seats to increment nSeats by
 */
void Party::incrementSeats(int n /*= 1*/){ nSeats += n; }
