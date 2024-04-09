#include <iostream>
#include <string>
#include <algorithm>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <fstream>

#include "PO.h"
#include "Candidate.h"
#include "helpers/colors.h"

PO::PO(vector<string> files) : Election(files){
	// ****************************************************************************
	// *                              Process Headers                             *
	// ****************************************************************************

	processHeaders();

	// ****************************************************************************
	// *                  Confirm Headers are processed correctly                 *
	// ****************************************************************************

	confirmHeaders();
}

PO::~PO() = default;

void PO::runElection(){

	// ****************************************************************************
	// *                              Digest ballots                              *
	// ****************************************************************************

	digestBallots();

	// ****************************************************************************
	// *                              Computer Winner                             *
	// ****************************************************************************

	runOff();

	// ****************************************************************************
	// *                        Display Results of Election                       *
	// ****************************************************************************

	displayResults(1);
	
	// ****************************************************************************
	// *                             Write Audit File                             *
	// ****************************************************************************

	writeToAudit();

	// ****************************************************************************
	// *                          Write Election Results                          *
	// ****************************************************************************

	writeElectionResults("POElectionResults.txt");
}

void PO::displayResults(bool doColors /*= true*/){
	auto column = [](auto s, int width){
		string sep = " | ";
		cout << sep << setw(width) << s << sep << setw(3);
	};

	vector<int> widths{
		9,
		(int)getHeader().at(3).length(),
		7
	};

	for(int i = 0; i < getNumCandidates(); i++){
		widths.at(0) = max(widths.at(0), (int)getCandidates().at(i).getName().length() );
	}

	if (doColors) { cout << BOLDMAGENTA; }
	
	column("Candidate", widths.at(0));
	column("Votes", widths.at(1));
	column("% votes", widths.at(2));

	
	cout << '\n';
	
	if (doColors) { cout << RESET; }

	for (int i = 0; i < getNumCandidates(); ++i){
		if(doColors && i == 0){ cout << BOLDGREEN; }
		column(getCandidates()[i].getName(), widths.at(0));
		column(getCandidates()[i].getNumVotes(), widths.at(1));
		int percentVote = (((float)getCandidates()[i].getNumVotes() / (float)getNumBallots()) * 100.0) + 0.5;
		column(to_string(percentVote) + "%", widths.at(2));
		if(doColors){ cout << RESET; }
		cout << '\n';
	}
}

void PO::writeToAudit(string s /*= ""*/){
	time_t t = time(nullptr);
	if(s.length() == 0){
		s = "POElectionAudit" + to_string(t) + ".txt";
	}
	
	ofstream auditFile(s);
	
	if(auditFile.good()){
		auditFile 	<< "Election was ran on: "<< asctime(localtime(&t)) << '\n';
		
		streambuf *psbuf, *backup;

		backup = std::cout.rdbuf();		// back up cout's streambuf

		psbuf = auditFile.rdbuf();		// get file's streambuf
		cout.rdbuf(psbuf);				// assign streambuf to cout

		displayResults(false); 			// Prevent colors

		cout.rdbuf(backup);				// restore cout's original streambuf

		auditFile << "\n--- END OF AUDIT FILE ---";

		auditFile.close();
		cout << CYAN << "Audit file created at: " + s + "\n" << RESET;
	}else{
		cout << RED << "ERROR: Creating audit file failed!\n" << RESET;
	}
	return;
}

void PO::runOff(){
	// sort candidates
	sort(getCandidates().begin(), getCandidates().end(), [](Candidate &a, Candidate &b) { 
		if(a.getNumVotes() != b.getNumVotes()){ return a.getNumVotes() > b.getNumVotes(); }
		return Election::flipCoin();
	});
}

void PO::digest(string buff){
	unsigned int pos = 0;
	for (int i = 0; i < buff.length(); i++){
		if(buff[i] == ',') { 
			if(i == 0 || buff[i - 1] == ',') { pos++; }
			continue;
		}else { 
			getCandidates()[pos].incrementVotes();
			pos++; 
		}
	}
}

void PO::confirmHeadersDisplay(){
	cout << "Election Type: " << BOLDWHITE << getElectionType() << RESET << '\n'
			 << "Number of Candidates: " << BOLDWHITE << getNumCandidates() << RESET << '\n'
			 << "Number of Ballots: " << BOLDWHITE << getNumBallots() << RESET << '\n'
			 << "Candidates: "
			 << BOLDWHITE;
		
		// ****************************************************************************
		// *                           Print each candidate                           *
		// ****************************************************************************

		for(int i = 0; i < getNumCandidates(); i++){
			cout << getCandidates()[i].getName() << '/' << getCandidates()[i].getParty() << ", ";
		}
}

void PO::processHeaders(){
	if(getHeader().size() != 4) {
		cout << RED <<"ERROR. Unexpected header size\nExpected 4, found " << getHeader().size() << '\n' << RESET;
		exit(-1);
	}
	setElectionType(getHeader()[0]);
	setNumberOfCandidates(stoi(getHeader()[1]));
	addBallots(stoi(getHeader()[3]));
	
	stringstream cStream(getHeader()[2]);
	while(cStream.good()){
		string candidate;
		string party;
		getline(cStream, candidate, '/');
		getline(cStream, party, ',');
		addCandidate(Candidate(candidate, party)); //add the candidate to the candidates vector			
	}
}