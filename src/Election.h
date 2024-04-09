#ifndef ELECTION_H
#define ELECTION_H

#include <string>
#include <vector>

#include "Candidate.h"


/**
 * @brief Represents a generic election.
 * 
 * A hybrid "abstract" class of members and methods to run any given election.
 * 
 */

using namespace std;

class Election{
public:
	// ****************************************************************************
	// *                        Constructor and Destructor                        *
	// ****************************************************************************

	/**
	 * @brief a constructor to create the election class based on a file
	 *
	 * @return string (file name)
	 */
	Election(vector<string> inFiles);

	/**
	 * @brief Destroy the Election:: Election object
	 * 
	 */
	virtual ~Election();

	// ****************************************************************************
	// *                                  Getters                                 *
	// ****************************************************************************
	
	/**
	 * @brief Gets the size of the seats in the election
	 *
	 * @return int (the number of seats)
	 */
	int getNumSeats();

	/**
	 * @brief Gets the number of candidates in the election
	 *
	 * @return int (the number of candidates)
	 */
	int getNumCandidates();

	/**
	 * @brief Gets the number of ballots in the election
	 *
	 * @return int (the number of ballots)
	 */
	int getNumBallots();

	/**
	 * @brief Gets the number of Drooop quota  in the election
	 *
	 * @return int (the number of DroopQuoata)
	 */
	int getDroopQuota();

protected:
	/**
	 * @brief Gets the size of the winners in the election
	 *
	 * @return int (the number of winners)
	 */
	int getNumWinners();
	
	/**
	 * @brief Gets the size of the losers in the election
	 *
	 * @return int (the number of losers)
	 */
	int getNumLosers();

	/**
	 * @brief Gets the election type in the election
	 *
	 * @return string (the election type)
	 */
	string getElectionType();

	/**
	 * @brief Gets the vector of candidates in the election
	 *
	 * @return vector<Candidate> (all the candidates in the election)
	 */
	vector<Candidate> &getCandidates();

	/**
	 * @brief Gets the winners vector  in the election
	 *
	 * @return vector<Candidate*> (the candidates that are winners)
	 */
	vector<Candidate*> getWinners();

	/**
	 * @brief Gets the losers vector in the election
	 *
	 * @return vector<Candidate*> (the candidates that are losers)
	 */
	vector<Candidate*> getLosers();

	/**
	 * @brief Gets the vector of each header line in the election
	 *
	 * @return vector<string>
	 */
	vector<string> &getHeader();

	// ****************************************************************************
	// *                                  Setters                                 *
	// ****************************************************************************

	/**
	 * @brief Method to add a candidate to the winners vector
	 * 
	 * @param c is the candidate to be added to the winners vector
	 */
	void addWinner(Candidate *c);
	
	/**
	 * @brief Method to add a candidate to the losers vector
	 * 
	 * @param c is the candidate to be added to the losers vector
	 */
	void addLoser(Candidate *c);

	/**
	 * @brief Setter function to set the droop quota
	 * 
	 * @param n the droop quota
	 */
	void setDroopQuota(unsigned int n);

	/**
	 * @brief Setter function to add n ballots
	 * 
	 * @param unsigned int n of ballots
	 */
	void addBallots(unsigned int n);

	/**
	 * @brief Setter function to remove n ballots
	 * 
	 * @param unsigned int n of ballots (optional, default 1)
	 */
	void removeBallots(unsigned int n = 1);

	/**
	 * @brief Setter function to set number of Candidates
	 * 
	 * @param unsigned int n
	 */
	void setNumberOfCandidates(unsigned int);

	/**
	 * @brief Setter function to add n ballots
	 * 
	 * @param unsigned int n of ballots
	 */
	void setNumberOfSeats(unsigned int);

	/**
	 * @brief Setter function to set number of ballots
	 * 
	 * @param unsigned int n
	 */
	void setNumberOfBallots(unsigned int);

	/**
	 * @brief Setter function to add c Candidate to candidates vector
	 * 
	 * @param Candidate c
	 */
	void addCandidate(Candidate c);

	/**
	 * @brief Setter function to set election type
	 * 
	 * @param std::string type
	 */
	void setElectionType(string type);

	// ****************************************************************************
	// *                               Other Methods                              *
	// ****************************************************************************
	
	/**
	 * @brief Function to randomly and fairly "flip a coin"
	 * 
	 * @return true if the result variable is equal to 1
	 * @return false if the result variable is equal to 0
	 */
	static bool flipCoin();
	
	/**
	 * @brief Abstract method to orchestrate an elections process
	 * 
	 */
	virtual void runElection() = 0;
	
	/**
	 * @brief Abstract method to display results of the election
	 * 
	 * @param bool doColors, indicates if displayResults should print using colors(true) or not(false)
	 */
	virtual void displayResults(bool doColors = 1) = 0;

	/**
	 * @brief Abstract method to write election results and election process to an audit file
	 * 
	 * @param std::string s, file name of the to-be-generated audit file
	 */
	virtual void writeToAudit(string s = "") = 0;

	/**
	 * @brief Abstract method to process and save a ballot into the election's data structure.
	 * 
	 * @param std::string, line from csv passed from digestBallots()
	 */
	virtual void digest(string buff) = 0;

	/**
	 * @brief Abstract method to store lines from headers vector into respective members
	 * 
	 */
	virtual void processHeaders() = 0;
	
	/**
	 * @brief Abstract method to display header files
	 * 
	 * @note see confirmHeadersDisplay()
	 */
	virtual void confirmHeadersDisplay() = 0;
	
	/**
	 * @brief Wrapper method of confirmHeadersDisplay()
	 * 
	 * Continuously calls confirmHeadersDisplay until the user confirms the results are correct
	 */
	void confirmHeaders();

	/**
	 * @brief Wrapper method to "digest" the ballots from a file.
	 * 
	 * Automatically skips election's header and reads line by line in Election file.
	 * Calls digest() (implemented in each child class of Election) with the current line as its sole parameter.
	 * 
	 * @note "Digest" means to process the ballots and sort them appropriately into the data structure
	 * 
	 */
	void digestBallots();
	
	/**
	 * @brief Method to create a results file for the election
	 * 
	 * Hijacks cout to ofstream and calls displayResults()
	 * 
	 * @param std::string s, the name of the results file to be created
	 */
	void writeElectionResults(string s);


private:
	Election(const Election& e);

	/**
	 * @brief Count of seats in election
	 * 
	 */
	unsigned int cSeats;

	/**
	 * @brief Count of candidates in election
	 * 
	 */
	unsigned int cCandidates;

	/**
	 * @brief Count of ballots in election
	 * 
	 */
	unsigned int cBallots;
	
	/**
	 * @brief Droop quota
	 * 
	 * The ratio of number of ballots to number of candidates
	 * 
	 */
	unsigned int nDroopQuota;

	/**
	 * @brief Election type
	 * 
	 */
	string sElectionType;

	/**
	 * @brief Vector of input files
	 * 
	 * Vector of strings containing filenames for each input election file.
	 * 
	 */
	vector<string> inputFiles;

	/**
	 * @brief All lines in the header
	 * 
	 * Vector of strings copied from the first input file's header
	 */
	vector<string> headers;

	/**
	 * @brief Vector of each Candidate in election
	 * 
	 */
	vector<Candidate> vCandidates;

	/**
	 * @brief vector of pointers to Candidates who are winners
	 * 
	 */
	vector<Candidate*> vWinners;

	/**
	 * @brief vector of pointers to Candidates who are losers
	 * 
	 */
	vector<Candidate*> vLosers;
};

#endif
