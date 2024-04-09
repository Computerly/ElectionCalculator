#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <ctime>

#include "Election.h"
#include "Candidate.h"
#include "helpers/colors.h"


// ****************************************************************************
// *                       Constructor and Destructor                         *
// ****************************************************************************
Election::Election(vector<string> inFiles){
	inputFiles = inFiles;
	cout << "Number of election files: " << inputFiles.size() << '\n';
	ifstream electionFile(inFiles[0]); // open the file
	
	if(electionFile.good()){
		string line;
		bool header = true;
		int count = 0;
		const int SAFETY_BRAKE_POINT = 10; // safety break
		while(getline(electionFile, line) && count < SAFETY_BRAKE_POINT){
			
			bool hasComma = false;
			for(char const &c : line){
				if(c == ',') { hasComma = true;goto ENDOFHEADER; }
				else if( !isdigit(c) ) { break; } // is not a number or comma
				if(hasComma && isdigit(c)) { goto ENDOFHEADER; }
			}

			headers.push_back(line);
			count++;
		}

ENDOFHEADER:
		electionFile.close(); // close the file
	}
	else { cout << RED << "Couldn't open file\n" << RESET; exit(-1); }

	cBallots = 0;
	for (int cFiles = 1; cFiles < inputFiles.size(); cFiles++){
		ifstream file(inputFiles[cFiles]); // open the file
		for (int cHeaders = 0; cHeaders < headers.size(); cHeaders++){
			string line;
			getline(file, line);
			//cout << "Line: " << line << "\nHeader Number: " << cHeaders << "\n";
			if(cHeaders == 3){ cBallots += stoi(line); } // Number of ballots
			else if(line != headers[cHeaders]){ // File mismatch
				cout << RED << "Error File mismatch!\nExpected \"" << headers[cHeaders] << "\". Found \"" << line << "\"\n" << RESET;
				exit(-1);
			}
		}
	}
}

Election::~Election() = default;

// ****************************************************************************
// *                                  Getters                                 *
// ****************************************************************************

int Election::getNumWinners(){ return vWinners.size(); }

int Election::getNumLosers(){ return vLosers.size(); }

int Election::getNumSeats(){ return cSeats; }

int Election::getNumCandidates(){ return cCandidates; }

int Election::getNumBallots(){ return cBallots; }

int Election::getDroopQuota() { return nDroopQuota; }

string Election::getElectionType(){ return sElectionType; }

vector<Candidate> &Election::getCandidates(){ return vCandidates; }

vector<Candidate*> Election::getWinners() { return vWinners; }

vector<Candidate*> Election::getLosers() { return vLosers; }

vector<string> &Election::getHeader() { return headers; }



// ****************************************************************************
// *                                  Setters                                 *
// ****************************************************************************

void Election::addWinner(Candidate *c){
	// unique entry only
	for(int i = 0; i < vWinners.size(); i++){
		if(vWinners[i]->getName() == c->getName()) { return; }
	}
	c->setWinState(true);
	c->setLoseState(false);
	vWinners.push_back(c);
}

void Election::addLoser(Candidate *c){
	for(int i = 0; i < vLosers.size(); i++){
		if(vLosers[i]->getName() == c->getName()) { return; }
	}
	c->setWinState(false);
	c->setLoseState(true);
	vLosers.push_back(c);
}

void Election::setDroopQuota(unsigned int n) { nDroopQuota = n; }

void Election::addBallots(unsigned int n) { 
	/*static bool notFirstPass = false;
	if(notFirstPass){
		cBallots += n;
	}
	notFirstPass = true;*/
	cBallots += n;
}

void Election::removeBallots(unsigned int n /*= 1*/){ cBallots -= n; }

void Election::setNumberOfCandidates(unsigned int n){ cCandidates = n; }

void Election::setNumberOfSeats(unsigned int n){ cSeats = n; }

void Election::setNumberOfBallots(unsigned int n){ cBallots = n; }

void Election::addCandidate(Candidate c){ vCandidates.push_back(c); }

void Election::setElectionType(string type){ sElectionType = type; }

// ****************************************************************************
// *                               Other Methods                              *
// ****************************************************************************

bool Election::flipCoin(){
	int flip = 0;
	for(int i = 0; i < 5; i++){ flip = rand() % 2; }
	return (bool)flip;
}

void Election::confirmHeaders(){
	// ****************************************************************************
	// *                              Confirm Header                              *
	// ****************************************************************************

	bool check = true;
	do{
		
		confirmHeadersDisplay();

		cout << RESET
			 << "\nPlease confirm the information above appears correct (Y/n): ";

		string choice = "";
		choice = cin.get();
		
		if( choice == "n" || choice == "N") { exit(-1); }

		else if( choice == "y" || choice == "Y" || choice == "\n") { cout << '\n'; check = false; }

		else { cout << "\nInvalid entry!\n\n"; }

	} while( check == true );
}

void Election::writeElectionResults(string fileName){
	ofstream resultsFile(fileName);

	if(!resultsFile.good()){
		cout << BOLDRED << "Error creating election results file!\n" << RESET;
		return;
	}

	streambuf *psbuf, *backup;

	backup = std::cout.rdbuf();		// back up cout's streambuf

	psbuf = resultsFile.rdbuf();	// get file's streambuf
	cout.rdbuf(psbuf);				// assign streambuf to cout

	displayResults(false); 			// Prevent colors

	cout.rdbuf(backup);				// restore cout's original streambuf

	resultsFile.close();

	cout << CYAN << "Election results saved.\n" << RESET;
}

// ****************************************************************************
// *                         WARNING!!! BREAKS FOR PO                         *
// ****************************************************************************
void Election::digestBallots(){
	string buff;
	for(int ballot = 0; ballot < inputFiles.size(); ballot++){
		ifstream file_reader(inputFiles[ballot]);
		if( !file_reader.good() ) { cout << RED << "Unable to open file!\n" << RESET; }
		else {	
			
			// ****************************************************************************
			// *                               Skip headers                               *
			// ****************************************************************************

			for(int i = 0; i < headers.size(); i++){ file_reader >> buff; }

			// ****************************************************************************
			// *                              Digest Ballots                              *
			// ****************************************************************************

			while( file_reader >> buff ){ digest(buff); }

			file_reader.close();
		}
		
	} /* END OF BALLOTS FOR */
}
