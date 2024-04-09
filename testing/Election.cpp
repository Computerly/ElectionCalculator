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

/**
 * @brief a constructor to create the election class based on a file
 *
 * @return string (file name)
 */
Election::Election(string inFile){
	inputFile = inFile;

	ifstream electionFile(inFile); // open the file
	bool isPo = false;
	if(electionFile.good() && !isPo){
		string line;

		getline (electionFile, sElectionType); // skip first line (Election Type)

		if(sElectionType == "PO")
		{
			isPo = true;
		}
		if(isPo)
		{
			getline (electionFile, line); // get second line, number of candidates (int)
			cCandidates = stoi(line);
			getline (electionFile, line); //get fifth line, candidate list
			stringstream cStream(line);
			while(cStream.good()){
				string candidate;
				string party;
				string nullStr;
				getline(cStream, nullStr,  '[');
				getline(cStream, candidate, ',');
				getline(cStream, party, ']');
				vCandidates.push_back(Candidate(candidate, party)); //add the candidate to the candidates vector
			}

			getline (electionFile, line); // get fourth line, number of ballots (int)
			cBallots = stoi(line);
		}
		else
		{
			getline (electionFile, line); // get second line, number of candidates (int)
			cCandidates = stoi(line);

			getline (electionFile, line); // get third line, number of seats (int)
			cSeats = stoi(line);

			getline (electionFile, line); // get fourth line, number of ballots (int)
			cBallots = stoi(line);

			getline (electionFile, line); //get fifth line, candidate list

			stringstream cStream(line);
			while(cStream.good()){
				string candidate;
				string party;
				getline(cStream, candidate, '/');
				getline(cStream, party, ',');
				vCandidates.push_back(Candidate(candidate, party)); //add the candidate to the candidates vector
			}
		}
		
		electionFile.close(); // close the file
	}
	else {
		cout << "Couldn't open file\n";
	}

	// ****************************************************************************
	// *                      Confirm File appears correctly                      *
	// ****************************************************************************

	confirmHeaders();
}

Election::~Election() = default;

// ****************************************************************************
// *                                  Getters                                 *
// ****************************************************************************
/**
 * @brief Gets the size of the winners in the election
 *
 * @return int (the number of winners)
 */
int Election::getNumWinners(){ return vWinners.size(); }

/**
 * @brief Gets the size of the losers in the election
 *
 * @return int (the number of losers)
 */
int Election::getNumLosers(){ return vLosers.size(); }

/**
 * @brief Gets the size of the seats in the election
 *
 * @return int (the number of seats)
 */

int Election::getNumSeats(){ return cSeats; }

/**
 * @brief Gets the number of candidates in the election
 *
 * @return int (the number of candidates)
 */

int Election::getNumCandidates(){ return cCandidates; }


/**
 * @brief Gets the number of ballots in the election
 *
 * @return int (the number of ballots)
 */

int Election::getNumBallots(){ return cBallots; }

/**
 * @brief Gets the number of Drooop quota  in the election
 *
 * @return int (the number of DroopQuoata)
 */

int Election::getDroopQuota() { return nDroopQuota; }


/**
 * @brief Gets the election type in the election
 *
 * @return string (the election type)
 */
string Election::getElectionType(){ return sElectionType; }


/**
 * @brief Gets the vector of candidates in the election
 *
 * @return vector<Candidate> (all the candidates in the election)
 */
vector<Candidate> &Election::getCandidates(){ return vCandidates; }

/**
 * @brief Gets the winners vector  in the election
 *
 * @return vector<Candidate*> (the candidates that are winners)
 */
vector<Candidate*> Election::getWinners() { return vWinners; }

/**
 * @brief Gets th losers vector in the election
 *
 * @return vector<Canddiate*> (the candidates that are losers)
 */
vector<Candidate*> Election::getLosers() { return vLosers; }



// ****************************************************************************
// *                                  Setters                                 *
// ****************************************************************************

void Election::addWinner(Candidate *c){
	// unique entry only
	for(int i = 0; i < vWinners.size(); i++){
		if(vWinners[i]->getName() == c->getName()) { return; }
	}
	c->setWinState(true);
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

// ****************************************************************************
// *                               Other Methods                              *
// ****************************************************************************

bool Election::flipCoin(){
	int result;
    srand(time(0));
    //This loop exists to truly randomize the result (as discussed in class)
    for(int i = 0; i < 50; i++)
    {
        result = rand() % 2;
    }

    //This line is the actual coin flip
    result = rand() % 2;

    return result;
}

void Election::confirmHeaders(){
	// ****************************************************************************
	// *                              Confirm Header                              *
	// ****************************************************************************

	bool check = true;
	do{
		cout << "Election Type: " << BOLDWHITE << getElectionType() << RESET << '\n'
			 << "Number of Candidates: " << BOLDWHITE << getNumCandidates() << RESET << '\n'
			 << "Number of Seats: " << BOLDWHITE << getNumSeats() << RESET << '\n'
			 << "Number of Ballots: " << BOLDWHITE << getNumBallots() << RESET << '\n'
			 << "Candidates: "
			 << BOLDWHITE;

		// ****************************************************************************
		// *                           Print each candidate                           *
		// ****************************************************************************

		for(int i = 0; i < getNumCandidates(); i++){
			cout << vCandidates[i].getName() << '/' << vCandidates[i].getParty() << ", ";
		}

		cout << RESET
			 << "\nPlease confirm the information above appears correct (Y/n): ";

		string choice = "";
		cin.ignore();
		choice = cin.get();

		if( choice == "n" || choice == "N") { exit(-1); }

		else if( choice == "y" || choice == "Y" || choice == "\n") { cout << '\n'; check = false; }

		else { cout << "\nInvalid entry!\n\n"; }

	} while( check == true );


}

void Election::writeElectionResults(string fileName){ // "IRV_election" + to_string(getDroopQuota()) + "_results.txt"
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

void Election::digestBallots(){
	char line[getNumCandidates() * 2];
	string buff;
	ifstream file_reader(inputFile.c_str());

	if( !file_reader.good() ) { cout << "Unable to open file!\n"; } // replace with just good
	else {
		// ****************************************************************************
		// *                                Skip Header                               *
		// ****************************************************************************
		for (int i = 0; i < 5; ++i) { file_reader >> buff; }

		// ****************************************************************************
		// *                              Digest Ballots                              *
		// ****************************************************************************

		while( true ){
			file_reader >> buff;

			digest(buff); // Call digest function

			if( file_reader.eof() ) { break; } // EOF Catch
		}
		file_reader.close();
	}
}
