#include "OPL.h"
#include <stdexcept>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <sstream>
#include <stdio.h>
#include <iomanip>

#include "helpers/colors.h"
#include "helpers/config.h"

using namespace std;

OPL::OPL(vector<string> _file_name) : Election(_file_name){

	// ****************************************************************************
	// *                              Process Headers                             *
	// ****************************************************************************

	processHeaders();

	// ****************************************************************************
	// *                  Confirm Headers are processed correctly                 *
	// ****************************************************************************

	confirmHeaders();
}

OPL::~OPL() = default;

void OPL::runElection(){
	// ****************************************************************************
	// *                              Digest Ballots                              *
	// ****************************************************************************

	digestBallots();

	// ****************************************************************************
	// *                              Compute Runoff                              *
	// ****************************************************************************

	runOff();

	// ****************************************************************************
	// *                              Display Results                             *
	// ****************************************************************************

	displayResults();

	// ****************************************************************************
	// *                         Write to Election Results                        *
	// ****************************************************************************

	writeElectionResults("OPLElection_" + to_string(getDroopQuota()) + ".txt");

	// ****************************************************************************
	// *                                Write Audit                               *
	// ****************************************************************************

	writeToAudit();
}

void OPL::runOff(){
	// ****************************************************************************
	// *                          Calculate Droop Quota                           *
	// ****************************************************************************

	setDroopQuota(getNumBallots() / getNumSeats());

	// ****************************************************************************
	// *                               Sort by votes                              *
	// ****************************************************************************

	sort(vParties.begin(), vParties.end(), [](Party &a, Party &b) { 
		if(a.getNumVotes() != b.getNumVotes()){ return a.getNumVotes() > b.getNumVotes(); }
		return Election::flipCoin();
	});


	// ****************************************************************************
	// *                               Initial Tally                              *
	// ****************************************************************************

	for (Party &i : vParties){
		int nSeats = i.getNumVotes() / getDroopQuota();
		i.incrementSeats( min(getNumSeats(), nSeats) );
		setNumberOfSeats( max(getNumSeats() - nSeats, 0) );		
		i.setRemainder(i.getNumVotes() % getDroopQuota());
		i.log( i.getNumSeats() );
		i.log( i.getRemainder() );
	}

	// ****************************************************************************
	// *                             Sort by remainder                            *
	// ****************************************************************************

	sort(vParties.begin(), vParties.end(), [](Party &a, Party &b) { 
		if(a.getRemainder() != b.getRemainder()){ return a.getRemainder() > b.getRemainder(); }
		return Election::flipCoin();
	});

	// ****************************************************************************
	// *                        Distribute remaining seats                        *
	// ****************************************************************************

	for(Party &i : vParties){
		i.incrementSeats( min(getNumSeats(), 1) );
		i.log(min(getNumSeats(), 1));
		setNumberOfSeats( max(getNumSeats() - 1, 0) );
	}
	
}

void OPL::displayResults(bool doColors /*= false*/){
	if (doColors) { cout << BOLDWHITE; }
	cout << "Election Results:\n"; 
	if (doColors) { cout << RESET; }
	cout << "Type: OPL\n"
		 << "Number of Candidates: " << getNumCandidates() << "\n"
		 << "Number of Seats: " << getHeader()[2] << '\n'
		 << "Number of Ballots: " << getNumBallots() << '\n'
		 << "Calculated Droop Quota: " << getDroopQuota() << '\n'
		 << "Candidates: " << getHeader()[4];

	auto column = [](auto s, int width){
		string sep = " | ";
		cout << sep << setw(width) << s << sep << setw(3);
	};

	int partyWidth = 7;
	for(Party &p : vParties){
		partyWidth = max(partyWidth, (int)p.getPartyName().length());
	}

	if (doColors) { cout << BOLDMAGENTA; }
	
	cout << '\n';
	column("Parties", partyWidth);
	column("Votes", getNumCandidates());
	column("Seats", getNumCandidates());
	column("Remainder", 9);
	column("Seats", getNumCandidates());
	column("Final Seats", 11);
	column("%vote/%seats", 12);

	if (doColors) { cout << RESET; }

	cout << "\n";

	for(Party &i : vParties){
		column(i.getPartyName(), partyWidth);
		column(i.getNumVotes(), getNumCandidates());
		column(i.getHistory(0), getNumCandidates());
		column(i.getHistory(1), 9);
		column(i.getHistory(2), getNumCandidates());
		column(i.getNumSeats(), 11);
		
		int percentVotes = ((float)i.getNumVotes() / (float)getNumBallots()) * 100;
		int percentSeats = ((float)i.getNumSeats() / stof(getHeader()[2])) * 100;
		column( to_string(percentVotes) + "% / " + to_string(percentSeats) + "%", 12);
		
		cout << '\n';
	}
}

void OPL::digest(string buff){
	unsigned int pos = 0;
	for (int i = 0; i < buff.length(); i++){
		if(buff[i] == ',') { 
			if(i == 0 || buff[i - 1] == ',') { pos++; }
			continue;
		}else { 
			getCandidates().at(pos).incrementVotes();
			getParty(getCandidates().at(pos).getParty())->incrementVotes();
			pos++; 
		}
	}
}

void OPL::writeToAudit(string s){
	time_t t = time(nullptr);
	if(s.length() == 0){
		s = "OPLElectionAudit" + to_string(t) + ".txt";
	}
	ofstream auditFile(s);
	if(auditFile.good()){
		if(SHUFFLE) { auditFile << "SHUFFLE Enabled\n"; }
		else 		{ auditFile << "SHUFFLE Disabled\n"; }

		auditFile 	<< "Election type: OPL\n"
					<< "Election was ran on: "<< asctime(localtime(&t)) << '\n';
		
		streambuf *psbuf, *backup;

		backup = std::cout.rdbuf();		// back up cout's streambuf

		psbuf = auditFile.rdbuf();		// get file's streambuf
		cout.rdbuf(psbuf);				// assign streambuf to cout

		displayResults(false); 			// Prevent colors

		cout.rdbuf(backup);				// restore cout's original streambuf

		auditFile << "--- END OF AUDIT FILE ---";

		auditFile.close();
		cout << CYAN << "Audit file created at: " + s + "\n" << RESET;
	}else{
		cout << RED << "ERROR: Creating audit file failed!\n" << RESET;
	}
	return;
}

void OPL::processHeaders(){
	if(getHeader().size() != 5) {
		cout << RED <<"ERROR. Unexpected header size\nExpected 5, found " << getHeader().size() << '\n' << RESET;
		for(string &s : getHeader()){

			cout << s << '\n';
		}
		exit(-1);
	}
	setElectionType(getHeader()[0]);
	setNumberOfCandidates(stoi(getHeader()[1]));
	setNumberOfSeats(stoi(getHeader()[2]));
	addBallots(stoi(getHeader()[3]));
	
	stringstream cStream(getHeader()[4]);
	while(cStream.good()){
		string candidate;
		string party;
		getline(cStream, candidate, '/');
		getline(cStream, party, ',');
		/*
			addParty(string p) => checks if p is in array, if not creates new party, if does not create new party.
			getParty(string p) => returns index to party matching string p if string p does not exist returns nullptr.

		*/
		addParty(party);
		addCandidate(Candidate(candidate, party)); //add the candidate to the candidates vector			
	}
}

void OPL::addParty(string p){ // adds unique
	// Check if p exists
	for (Party &i : vParties){
		if(i.getPartyName() == p) { return; } // found, we do nothing
	}
	vParties.push_back(Party(p));
}

Party *OPL::getParty(string p){
	for(Party &i : vParties){
		if(i.getPartyName() == p) { return &i; } // found, give back address to party;
	}
	return nullptr;
}

void OPL::confirmHeadersDisplay(){
	cout << "Election Type: " << BOLDWHITE << getElectionType() << RESET << '\n'
		 << "Number of Candidates: " << BOLDWHITE << getNumCandidates() << RESET << '\n'
		 << "Number of Seats: " << BOLDWHITE << getNumSeats() << RESET << '\n'
		 << "Approximate number of Ballots: " << BOLDWHITE << getNumBallots() << RESET << '\n'
		 << "Candidates: "
		 << BOLDWHITE;
	
	// ****************************************************************************
	// *                           Print each candidate                           *
	// ****************************************************************************

	for(int i = 0; i < getNumCandidates(); i++){
		cout << getCandidates().at(i).getName() << '/' << getCandidates().at(i).getParty();
		if(i != getNumCandidates() - 1) { cout << ", "; }		
	}
	cout << RESET;
}