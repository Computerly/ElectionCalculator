#ifndef PARTY_H
#define PARTY_H

#include <vector>
#include <string>

#include "Candidate.h"

/**
 * @brief Represents a Party in election
 * 
 * 
 */
class Party{
public:
	// ****************************************************************************
	// *                        Constructor and Destructor                        *
	// ****************************************************************************
	/**
	 * @brief Construct a new Party object
	 * 
	 * @param std::string name is the name of the party
	 */
	Party(std::string partyName);
	
	// ****************************************************************************
	// *                                  Getters                                 *
	// ****************************************************************************

	/**
	 * @brief Gets the total number of votes for the party
	 *        by summing all the candidate's votes
	 * 
	 * @return int (the total number of votes for the party)
	 */
	int getNumVotes();

	/**
	 * @brief Gets the number of seats allocated to the party
	 * 
	 * @return int (the number of seats)
	 */
	int getNumSeats();

	/**
	 * @brief Gets the remainder of the number of votes for a party divided by the quota (for OPL)
	 * 
	 * @return int (the remainder)
	 */
	int getRemainder();

	/**
	 * @brief Gets the number of votes at a point in time
	 * 
	 * @return number of _ at point of time (either vote count or delta vote count)
	 */
	int getHistory(int n);
	
	/**
	 * @brief Gets the party's name (e.g. Democrat, Republican, etc.)
	 * 
	 * @return std::string (the party name)
	 */
	std::string getPartyName();

	// ****************************************************************************
	// *                                  Setters                                 *
	// ****************************************************************************
 
	/**
	 * @brief Increments the numVotes variable for the party by the parameter n
	 * 
	 * @param Increment numVotes by n (optional, default 1)
	 */
	void incrementVotes(int n = 1);

	/**
	 * @brief Increments the nSeats variable for the party by the parameter n
	 * 
	 * @param Increment numSeats by n (optional, default 1)
	 */
	void incrementSeats(int n = 1);

	/**
	 * @brief Sets the remainder to the input value n.
	 * 
	 * @param Sets the remainder to n.
	 */
	void setRemainder(int n);

	// ****************************************************************************
	// *                               Other Methods                              *
	// ****************************************************************************
	
	/**
	 * @brief Adds n to logs .
	 * 
	 * @param Number to be pushed back on logs.
	 */
	void log(int n);

private:
	/**
	 * @brief Number of votes
	 * 
	 */
	int numVotes;
	
	/**
	 * @brief Number of seats
	 * 
	 */
	int nSeats;

	/**
	 * @brief remaining votes after first allocation of seats
	 * 
	 *  = votes % droop
	 * 
	 */
	int nRemainder;

	/**
	 * @brief Name of the party
	 * 
	 */
	std::string partyName;

	/**
	 * @brief Vector of histories
	 * 
	 * Records history of the candidate.
	 * 
	 */
	std::vector<int> vnHistory;
};

#endif
