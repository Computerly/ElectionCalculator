#ifndef IRV_H
#define IRV_H
#include <iostream>
#include <string>
#include <vector>
#include "Candidate.h"
#include "Election.h"

using namespace std;

class IRV : public Election{
public:
	// ****************************************************************************
	// *                        Constructor and Destructor                        *
	// ****************************************************************************

	IRV(string _file_name);

	virtual ~IRV();

	// ****************************************************************************
	// *                                  Getters                                 *
	// ****************************************************************************


	// ****************************************************************************
	// *                                  Setters                                 *
	// ****************************************************************************



	// ****************************************************************************
	// *                               Other Methods                              *
	// ****************************************************************************

	void runElection();

	void writeToAudit(string s = "");

	void addAuditLine(string s);

	void displayResults(bool doColors = true);

	void digest(string buff);

	void runOff();

	// ****************************************************************************
	// *                                  Helpers                                 *
	// ****************************************************************************

	void handleWin();

	void logCandidates();

	vector<Candidate*> getWinners();
	vector<Candidate*> getLosers();
private:
	int *nBallotArray;
	vector<string> audits;
};

#endif
