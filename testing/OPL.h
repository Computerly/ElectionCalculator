#ifndef OPL_H
#define OPL_H
#include <iostream>
#include <vector>
#include <string>
#include "Election.h"
#include "Candidate.h"
#include "Party.h"

using namespace std;

class OPL : public Election
{
public:
    // ****************************************************************************
	// *                        Constructor and Destructor                        *
	// ****************************************************************************
    OPL(string _file_name);
    virtual ~OPL();

    // ****************************************************************************
	// *                                  Getters                                 *
	// ****************************************************************************
    int getNumParties();

    vector<Party> &getParties();

    // ****************************************************************************
	// *                                  Setters                                 *
	// ****************************************************************************
    void setParties(vector<Party> parties);
    
    void addParty(Party p);

    void allocateCandidates();

    vector<Candidate> distributeSeats(Party parties);

    // ****************************************************************************
	// *                               Other Methods                              *
	// ****************************************************************************
    void runElection();

    void displayResults(bool doColors = true);

    void writeToAudit(string s = "");

    void digest(string buff);

    void addAuditLine(string s);

    vector<Candidate*> getWinners();
private:
    int nDroopQuota;
    vector<string> audits;
    vector<Party> vParties;
};

#endif
