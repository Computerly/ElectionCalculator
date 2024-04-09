#ifndef IRV_H
#define IRV_H
#include <iostream>
#include <string>
#include <vector>

#include "Candidate.h"
#include "Election.h"

/**
 * @brief Represents an IRV (Instant Runoff Voting) election.
 * 
 * Implements Election
 * 
 */

using namespace std;

class IRV : public Election{
public:
	// ****************************************************************************
	// *                        Constructor and Destructor                        *
	// ****************************************************************************

	/**
	 * @brief Construct a new IRV object
	 * 
	 * Initializes and confirms election
	 * 
	 * @param std::vector<std::string> _file_name. A vector of all files to run IRV the election.
	 */
	IRV(vector<string> _file_name);
	
	/**
	 * @brief Destroy the IRV::IRV object
	 * 
	 */
	virtual ~IRV();

	// ****************************************************************************
	// *                               Other Methods                              *
	// ****************************************************************************
	
	/**
	 * @brief Main method for running the IRV election process
	 * 
	 * Calls methods stratigically to compute election results
	 * 
	 */
	void runElection();
	
	/**
	 * @brief Method to write the election progression to an audit file
	 * 
	 * Writes each line from audits vector to file
	 * 
	 * @param std::string s is the file name of the to-be-generated audit file
	 */
	void writeToAudit(string s = "");

	/**
	 * @brief Adds line to audit file
	 * 
	 * Adds a string to a vector of strings to be added to an audit file 
	 * 
	 * @param s String to be added to the audit file
	 */
	void addAuditLine(string s);

	/**
	 * @brief Method to display the IRV election results to the screen
	 * 
	 * @note Includes the election information as well
	 * 
	 * @params bool doColors (see Election::displayResults() for more)
	 */
	void displayResults(bool doColors = true);

	/**
	 * @brief Parses the election file and inserts ballots in the tally array.
	 * 
	 *	Each ballot (row), is converted into a ranking number.
	 *	Ranking numbers are formed by ordering the nPreference of the candidates.
	 *	For example:
	 *		"2,1,3,5,4" is a given ballot.
	 *		The first choice is the second candidate. 
	 *		So the first number in the ranking number is 2.
	 *		Then the second choice is the first candidate.
	 *		So the second number in the ranking number is 1.
	 *		This repeats until all choices are recorded.
	 *		For this given ballot, the corollary ranking number is: "21354".
	 *	
	 *	If a voter does not vote for all candidates (but at least one),
	 *	it is translated into a zero.
	 *	So for example, if not all of the candidates were chosen,
	 *	a ballot could look like "4,,1,3,2".
	 *	Its corollary ranking number would be: "35410"
	 *	because there was no fifth choice given.
	 *	
	 *	While digesting ballots and converting into rankings,
	 *	the first choice is automatically tallied into the tally array.
	 * 
	 * @param std::string, a line from the election file representing a ballot, where a "1" represents the voter's first choice
	 * 
	 * */
	void digest(string buff);

	/**
	 * @brief Computes runoff of the IRV election
	 * 
	 */
	void runOff();

	/**
	 * @brief Method to invalidate a ballot
	 * 
	 * Adds ballot to a vector of invalidated ballots,
	 * removes the ballot from the total ballot count,
	 * and creates a log in audit.
	 * 
	 * @param std::string ballot, string representing a ballot to be invalidated 
	 */
	void invalidateBallot(string ballot);

	/**
	 * @brief Writes invalidated ballots to file
	 *
	 * Similar to writeToAudit, writes each line in invalidBallots vector to a file of
	 * invalidated ballots.
	 * 
	 */
	void writeInvalidBallots();

	/**
	 * @brief Method to store headers(strings) into their respective variables.
	 * 
	 */
	void processHeaders();

	/**
	 * @brief Confirms everything processed thus far, appears correctly.
	 * 
	 */
	void confirmHeadersDisplay();

	// ****************************************************************************
	// *                                  Helpers                                 *
	// ****************************************************************************

	/**
	 * @brief Helper to handle the vote tally for a round
	 * 
	 * @note Adds the non-winning candidates to the loser vector
	 */
	void handleWin();

	/**
	 * @brief Helper to log the Candidates status quo
	 * 
	 * Logs the current vote count and adds a line in audit with vote count.
	 */
	void logCandidates();

private:
	/**
	 * @brief A vector to store all ballots of the election
	 * 
	 */
	vector<int> vBallotArray;

	/**
	 * @brief A vector to store all line to-be-added to the audit file.
	 * 
	 */
	vector<string> audits;

	/**
	 * @brief A vector to store all line to-be-added to the invalidatedBallots file.
	 * 
	 */
	vector<string> invalidBallots;
};

#endif
