#ifndef OPL_H
#define OPL_H

#include <iostream>
#include <vector>
#include <string>

#include "Election.h"
#include "Candidate.h"
#include "Party.h"

using namespace std;


/**
 * @brief Represents an OPL (Open Party Listing) election.
 * 
 * Implements Election
 * 
 */
class OPL : public Election
{
public:
	// ****************************************************************************
	// *                        Constructor and Destructor                        *
	// ****************************************************************************
	
	/**
	 * @brief Construct a new OPL object
	 * 
	 * Initializes and confirms election
	 * 
	 * @param std::vector<std::string> _file_name. A vector of all files to run IRV the election.
	 */
	OPL(vector<string> _file_name);
	
	/**
	 * @brief Destroy the OPL::OPL object
	 * 
	 */
	virtual ~OPL();

	// ****************************************************************************
	// *                                  Getters                                 *
	// ****************************************************************************
	
	/**
	* @brief Gets the number of parties participating in the election
	* 
	* @return int representing the number of parties in the election
	*/
	int getNumParties();

	/**
	 * @brief Gets the vector of the parties participating in the election
	 * 
	 * @return std::vector<Party> reference to the vector of parties
	 */
	vector<Party> &getParties();

	/**
	 * @brief Gets a pointer to a Party object with Party's name matching string p
	 * 
	 * @param std::string name of party
	 * @return Point to a Party object or null if none found
	 */
	Party *getParty(string p);


	// ****************************************************************************
	// *                                  Setters                                 *
	// ****************************************************************************

	/**
	 * @brief Uniquely adds a Party object with name p
	 * 
	 * Creates and adds a new Party object with name p to the vector of parties.
	 * @param std::string name of party
	 */
	void addParty(string p);

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
	 * @brief Method to display the IRV election results to the screen
	 * 
	 * @note Includes the election information as well
	 * 
	 * @params bool doColors (see Election::displayResults() for more)
	 */
	void displayResults(bool doColors = true);

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
	 * @brief Parses the election file and updates candidate and party votes.
	 *  
	 * Parses who was voted for, then updates the candidates votes count,
	 * then finds the candidate's party and updates the party's total votes as well.
	 * 
	 * @param std::string, a line from the election file representing a ballot, where a "1" represents the voter's vote
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

private:
	/**
	 * @brief Droop quota
	 * 
	 */
	int nDroopQuota;

	/**
	 * @brief A vector to store all line to-be-added to the audit file.
	 * 
	 */
	vector<string> audits;
	
	/**
	 * @brief A vector to store all parties in.
	 * 
	 */
	vector<Party> vParties;
};

#endif