#ifndef ELECTION_H
#define ELECTION_H

#include <string>
#include <vector>

#include "Candidate.h"

/**
 * @brief Election.h is the header file for the Election class. Written by Charlie Nazarian
 */
using namespace std;
class Election{
public:
	// ****************************************************************************
	// *                        Constructor and Destructor                        *
	// ****************************************************************************

	Election(string inFile);

	virtual ~Election();

	// ****************************************************************************
	// *                                  Getters                                 *
	// ****************************************************************************
	
	int getNumSeats();

	int getNumCandidates();

	int getNumBallots();

	int getDroopQuota();

protected:
	int getNumWinners();
	
	int getNumLosers();

	string getElectionType();

	vector<Candidate> &getCandidates();

	vector<Candidate*> getWinners();

	vector<Candidate*> getLosers();

	// ****************************************************************************
	// *                                  Setters                                 *
	// ****************************************************************************

	void addWinner(Candidate *c);
	
	void addLoser(Candidate *c);

	void setDroopQuota(unsigned int n);

	// ****************************************************************************
	// *                               Other Methods                              *
	// ****************************************************************************
	bool flipCoin();
	
	virtual void runElection() = 0;
	
	virtual void displayResults(bool doColors = 0) = 0;

	virtual void writeToAudit(string s = "") = 0;

	virtual void digest(string buff) = 0;

	void digestBallots();

	void writeElectionResults(string s);

	void confirmHeaders();

private:
	Election(const Election& e);

	unsigned int cSeats;

	unsigned int cCandidates;

	unsigned int cBallots;
	
	unsigned int nDroopQuota;

	string inputFile;

	string sElectionType;

	vector<Candidate> vCandidates;

	vector<Candidate*> vWinners;

	vector<Candidate*> vLosers;
};

#endif
