#ifndef PO_H
#define PO_H

#include <iostream>
#include <vector>
#include <string>
#include "Election.h"

using namespace std;

/**
 * @brief Represents an IRV (Instant Runoff Voting) election.
 * 
 * Implements Election
 * 
 */
class PO : public Election{
public:
	// ****************************************************************************
	// *                        Constructor and Destructor                        *
	// ****************************************************************************
	
	/**
	 * @brief Construct a new PO object
	 * 
	 * Initializes and confirms election
	 * 
	 * @param std::vector<std::string> _file_name. A vector of all files to run IRV the election.
	 */
	PO(vector<string> _file_name);

	/**
	 * @brief Destroy the IRV::IRV object
	 * 
	 */
	virtual ~PO();
	
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
	 * @brief Computes runoff of the IRV election
	 * 
	 */
	void runOff();
	
	/**
	 * @brief Method to display the PO election results to the screen
	 * 
	 * @note Includes the election information as well
	 * 
	 * @params bool doColors (see Election::displayResults() for more)
	 */
	void displayResults(bool doColors = 0);
	
	/**
	 * @brief Method to write the election progression to an audit file
	 * 
	 * Writes each line from audits vector to file
	 * 
	 * @param std::string s is the file name of the to-be-generated audit file
	 */
	void writeToAudit(string s = "");

	/**
	 * @brief Parses the election file and updates candidate's votes.
	 * 
	 * @param std::string, a line from the election file representing a ballot, where a "1" represents the voter's vote.
	 * 
	 * */
	void digest(string buff);

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
};

#endif
