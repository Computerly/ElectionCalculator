// ****************************************************************************
// *                                    IRV                                   *
// *                               Version: 1.2                               *
// * April, 2022                                                              *
// * CSCI 5801: Software Engineering I                                        *
// * Assigned: Caleb Venkatrathnam <venka147>                                 *
// ****************************************************************************

#include "IRV.h"
#include <fstream>
#include <cmath>
#include <random>
#include <iomanip>
#include <algorithm>
#include <ctime>

#include "helpers/Combinations.h"
#include "helpers/math.h"
#include "helpers/colors.h"
#include "helpers/config.h"
#include "Candidate.h"

IRV::IRV(vector<string> _file_name) : Election(_file_name){
	// ****************************************************************************
	// *                              Process Headers                             *
	// ****************************************************************************

	processHeaders();

	// ****************************************************************************
	// *                  Confirm Headers are processed correctly                 *
	// ****************************************************************************

	confirmHeaders();

	// ****************************************************************************
	// *                         Initialize Ballot Array                          *
	// * Sum (number of candidates) P r, where r ranges 1 to number of candidates *
	// ****************************************************************************
	
	int size = Combinations::sizeOf( getNumCandidates() );
	for(int elm = 0; elm < size; elm++){ vBallotArray.push_back(0); }
}

IRV::~IRV() {
	cout << RESET;
}

void IRV::processHeaders(){
	if(getHeader().size() != 5) {
		cout << RED <<"ERROR. Unexpected header size\nExpected 5, found " << getHeader().size() << '\n' << RESET;
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
		addCandidate(Candidate(candidate, party)); //add the candidate to the candidates vector			
	}
}

void IRV::confirmHeadersDisplay(){
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
		cout << getCandidates()[i].getName() << '/' << getCandidates()[i].getParty() << ", ";
	}
}

void IRV::runElection(){

	// ****************************************************************************
	// *                              Digest Ballots                              *
	// ****************************************************************************
	
	digestBallots();

	// ****************************************************************************
	// *                              Compute runoff                              *
	// ****************************************************************************
	
	runOff();
	
	// ****************************************************************************
	// *                              Display Results                             *
	// ****************************************************************************

	displayResults();

	// ****************************************************************************
	// *                         Write to Election Results                        *
	// ****************************************************************************

	writeElectionResults("IRVelection_" + to_string(getDroopQuota()) + ".txt");

	// ****************************************************************************
	// *                                Write Audit                               *
	// ****************************************************************************

	writeToAudit();

	// ****************************************************************************
	// *                       Write Invalid Ballots to file                      *
	// ****************************************************************************
	
	writeInvalidBallots();
}

void IRV::runOff(){
	
	// ****************************************************************************
	// *            Number of combinations associated with a candidate            *
	// ****************************************************************************

	int nBlockSize = Combinations::sizeOf(getNumCandidates()) / getNumCandidates(); 
	 
	int *nToTally = new int[nBlockSize]; 

	// ****************************************************************************
	// *                          Calculate Droop Quota                           *
	// ****************************************************************************

	setDroopQuota(( getNumBallots() / ( getNumSeats() + 1 ) ) + 1);

	// ****************************************************************************
	// *                              Run Droop Quota                             *
	// ****************************************************************************
	
	for(int nCount = 0; nCount < (getNumCandidates() - getNumSeats()); nCount++){

		// ****************************************************************************
		// *                           Log Round of Tallies                           *
		// ****************************************************************************

		logCandidates(); 

		// ****************************************************************************
		// *                   Check if droop quota has been reached                  *
		// *         ( also find candidate with fewest votes while I'm at it)         *
		// ****************************************************************************

		int nLowestCandidate = -1; 
		unsigned int cLowestVote = 999999; 
		for(int i = 0; i < getNumCandidates(); i++){
			if(getCandidates()[i].isLoser()){ continue; }
			
			if(getCandidates()[i].getNumVotes() == cLowestVote) { nLowestCandidate = flipCoin() == true ? i : nLowestCandidate;  } // Flip for loser 
			
			else if(getCandidates()[i].getNumVotes() < cLowestVote) { nLowestCandidate = i; cLowestVote = getCandidates()[i].getNumVotes();  } // Find lowest candidate 
			
			if(getCandidates()[i].getNumVotes() >= getDroopQuota()) { 
				addWinner(&getCandidates()[i]);
				if(getNumWinners() == getNumSeats()) { handleWin(); return; } // Stop runoff 
			} 
		} 
		
		// ****************************************************************************
		// *                               Update Loser                               *
		// ****************************************************************************
		getCandidates()[nLowestCandidate].setVotes(0); 
 
		//addLoser(&getCandidates()[nLowestCandidate]); 
 		
 		addAuditLine("Eliminated: " + getCandidates()[nLowestCandidate].getName() + " with " + to_string(getCandidates()[nLowestCandidate].getNumVotes()) + " votes.");
		
		// ****************************************************************************
		// *                              Compute Runoff                              *
		// ****************************************************************************
		// Redistribute lowest candidate's votes (randomly) to everyone who is not a winner
		// Check if transferred vote makes a candidate reach droop, if so
		// add them to the winners list

		int nOffset = (nBlockSize * nLowestCandidate); 
 
		nOffset += nOffset == 0 ? 0 : 1; 

		// ****************************************************************************
		// *         Create an array of indices from offset, nBlockSize size.         *
		// *      Shuffle elements to create a random order to select tallies in      *
		// ****************************************************************************

		for(int i = 0; i < nBlockSize; i++) { nToTally[i] = i + nOffset; } // fill array start to end. IMPORTANT! Skipping first index! 
 		
 		if(SHUFFLE == true){
 			srand(time(0));
			for(int i = 0; i < nBlockSize * 5; i++){ 
				// Generate two random indices  
				int nRandIdx1 = (rand() % (nBlockSize - 1)); 
				int nRandIdx2 = (rand() % (nBlockSize - 1)); 
				 
				// Swap elements 
				int nTemp = nToTally[nRandIdx1]; 
	 
				nToTally[nRandIdx1] = nToTally[nRandIdx2]; 
				nToTally[nRandIdx2] = nTemp; 
			} 
		}

		// ****************************************************************************
		// *                               Random Tally                               *
		// *                     Mathematically determine segments                    *
		// ****************************************************************************

		for(int i = 0; i < nBlockSize; i++){ 
			int nRank = Combinations::getRankingNumber(nToTally[i], getNumCandidates()); 
 
			for(int nPreference = nCount; nPreference < getNumCandidates() - 1; nPreference++){ 
				 
				int nCandidate = (( nRank / quick_pow10(getNumCandidates() - nPreference - 2) ) % 10) - 1; 
				 
				if( nCandidate != -1 && !getCandidates()[nPreference].isWinner() && !getCandidates()[nPreference].isLoser()){ 
					
					getCandidates()[nPreference].incrementVotes(vBallotArray[ nToTally[i] ]); 

 					addAuditLine(to_string(nRank) + " -> " + getCandidates()[nPreference].getName());
					
					if(getCandidates()[nPreference].getNumVotes() >= getDroopQuota()){ 
						addWinner(&getCandidates()[nPreference]);
						if(getNumWinners() == getNumSeats()) { handleWin(); return; } // stop runoff 
					} 
				} 
			} 
		}
		
	}
	delete[] nToTally;

	// ****************************************************************************
	// *                 If here, no one has reached droop quota,                 *
	// *   so make the top number of seats candidates the winners by popularity   *
	// ****************************************************************************

	sort(getCandidates().begin(), getCandidates().end(),[](auto a, auto b) { return a.getNumVotes() > b.getNumVotes(); } ); 

	// ****************************************************************************
	// *               Add top number_of_seats candidates to winners              *
	// ****************************************************************************

	for(int i = 0; i < getNumSeats(); i++){ 
		getCandidates()[i].setWinState(true);
		addWinner(&(getCandidates()[i])); 
	} 

	handleWin(); 
}

void IRV::displayResults(bool doColors /*= true*/){
	cout << "\n";

	if (doColors) { cout << BOLDWHITE; }
	cout << "Election Results:\n"; 
	if (doColors) { cout << RESET; }
	cout << "Type: IRV\n"
		 << "Number of Candidates: " << getNumCandidates() << "\n"
		 << "Number of Seats: " << getNumSeats() << '\n'
		 << "Number of Ballots: " << getNumBallots() << '\n'
		 << "Calculated Droop Quota: " << getDroopQuota() << '\n'
		 << "Candidates: " << getHeader()[4] << '\n'; // added getHeader()[4]

	auto column = [](auto s, int width){
		string sep = " | ";
		cout << sep << setw(width) << s << sep << setw(3);
	};

	vector<int> widths{
		10, // Candidates
		9,  // Round
		9,	// Candidate
		5,	// Party
		5	// Votes / pm
	};

	auto printCandidate = [&column, &doColors, &widths](auto c){
		vector<int> history;
		
		for(int i = c.size() - 1; i > -1 ; i--){			
			history = c[i]->getVoteHistory();

			if(doColors){
				if(c[i]->isWinner()) { cout << BOLDGREEN; }
				else { cout << RESET; }
			}
			column(c[i]->getName(), widths.at(2) );
			column(c[i]->getParty(), widths.at(3));
			for(int i = 0; i < history.size(); i++){
				if(i % 2 == 0){ // if even
					column(history.at(i), widths.at(4));
				}else if(history.at(i) != 0){
					column("+" + to_string(history.at(i)), widths.at(4));
				}else{
					column(0, widths.at(4));
				}

			}
			cout << '\n';
		}
	};

	for(int i = 0; i < getNumCandidates(); i++){
		widths.at(2) = max(widths.at(2), (int)getCandidates().at(i).getName().length());
		widths.at(3) = max(widths.at(3), (int)getCandidates().at(i).getParty().length());
	}

	widths.at(4) = getHeader().at(3).length();
	widths.at(1) = (widths.at(4) * 2) + 6;
	widths.at(0) = widths.at(2) + widths.at(3) + 6;

	vector<string> POS{"st", "nd", "rd", "th"};

	if (doColors) { cout << BOLDMAGENTA; }

	column("Candidates", widths.at(0) );
	for(int i = 0; i < getCandidates()[0].getVoteHistory().size() / 2; i++){
		column(to_string(i+1) + POS.at(min(i % 10, 3))  + " Round", widths.at(1)); // + POS.at(min(i % 10, 3))
	}
	
	cout << '\n';

	column("Candidate", widths.at(2));
	column("Party", widths.at(3)); 

	for(int i = 0; i < getCandidates().at(0).getVoteHistory().size()/2; i++){
		column("Votes",	widths.at(4) ); 
		column("+-", widths.at(4)); 
	}
	
	cout << '\n';

	if (doColors) { cout << RESET; }

	printCandidate(getWinners());
	printCandidate(getLosers());
	//printCandidate(getCandidates());
	
	cout << "\n";
}

void IRV::digest(string buff){ // Takes in a line
	
	unsigned short ranking = 0;
	unsigned short c = 1;
	unsigned short tallied = -1;

	unsigned short MIN_CANDIDATES = (short)(((float)getNumCandidates() / 2.0) + 0.5);
	
		for (int i = 0; i < buff.length(); i++){
			if(buff[i] == ','){				
				if(i == 0 || buff[i - 1] == ',') { c++; }	// check if first char, and check if last char was ','
				continue;									// else continue
			} else { // build ranking number				
				int nLine = buff[i] - '0';
				ranking += (c * quick_pow10((getNumCandidates() - nLine)));

				if( nLine == 1) { getCandidates()[c - 1].incrementVotes(); tallied = c - 1; } // Add Tally
				c++;
			} 
		}

		if(ranking % quick_pow10(getNumCandidates() - MIN_CANDIDATES + 1) == 0){ 
			invalidateBallot(buff); // Invalidate ballot
			getCandidates()[tallied].decrementVotes(); // Remove tally
		}
		else{ vBallotArray[ Combinations::indexOf(ranking, getNumCandidates()) ] += 1; } // Add Ranking 
		
}

// ****************************************************************************
// *                                  Helpers                                 *
// ****************************************************************************

void IRV::handleWin(){
	// ****************************************************************************
	// *                           Log Round of Tallies                           *
	// ****************************************************************************

	logCandidates();
	
	// ****************************************************************************
	// *                      Add not winners to loser vector                     *
	// ****************************************************************************
	
	for (int i = 0; i < getNumCandidates(); i++){
		if(!getCandidates().at(i).isWinner()){
			addLoser(&getCandidates().at(i));
		}
	}
}

void IRV::addAuditLine(string s){ audits.push_back(s); }

void IRV::writeToAudit(string s){
	time_t t = time(nullptr);
	if(s.length() == 0){
		s = "IRVElectionAudit" + to_string(t) + ".txt";
	}
	ofstream auditFile(s);
	if(auditFile.good()){
		if(SHUFFLE) { auditFile << "SHUFFLE Enabled\n"; }
		else 		{ auditFile << "SHUFFLE Disabled\n"; }

		auditFile 	<< "Election type: IRV\n"
					<< "Election was ran on: "<< asctime(localtime(&t)) << '\n';
		
		streambuf *psbuf, *backup;

		backup = std::cout.rdbuf();		// back up cout's streambuf

		psbuf = auditFile.rdbuf();		// get file's streambuf
		cout.rdbuf(psbuf);				// assign streambuf to cout

		displayResults(false); 			// Prevent colors

		cout.rdbuf(backup);				// restore cout's original streambuf

		auditFile << "\nBallot Allocations: \n";
		for (int i = 0; i < audits.size(); i++){
			auditFile << audits[i] << '\n';
		}
		auditFile << "--- END OF AUDIT FILE ---";

		auditFile.close();
		cout << CYAN << "Audit file created at: " + s + "\n" << RESET;
	}else{
		cout << RED << "ERROR: Creating audit file failed!\n" << RESET;
	}
	return;
}

void IRV::logCandidates(){
	for(int i = 0; i < getNumCandidates(); i++){ 
		getCandidates().at(i).logVoteCount();
		addAuditLine(getCandidates()[i].getName() + " has " + to_string(getCandidates()[i].getNumVotes()) + " votes.");
	}
}


void IRV::invalidateBallot(string ballot){
	invalidBallots.push_back(ballot);	// push back on invalid ballots

	removeBallots(/*1*/); // update ballot count

	addAuditLine("Invalidated Ballot: \"" + ballot + '"'); // write to audit
}

void IRV::writeInvalidBallots(){
	if(invalidBallots.size() < 1) { cout << CYAN << "No invalid ballots. File no created" << RESET; return;}

	time_t t = time(nullptr);
	
	string s = "invalidated_" + to_string(t) + ".txt";
	
	ofstream invalidFile(s);
	if(invalidFile.good()){
		
		invalidFile 	<< "Election type: IRV\n"
					<< "Election was ran on: "<< asctime(localtime(&t)) << '\n'
					<< "\nBallot Allocations: \n";

		for (int i = 0; i < invalidBallots.size(); i++){
			invalidFile << invalidBallots[i] << '\n';
		}

		invalidFile << "--- END OF INVALID BALLOTS FILE ---";

		invalidFile.close();
		cout << CYAN << "Invalidated Ballots were found in the election(s).\nA file was file created at: \"" + s + "\" containing invalid ballots.\n" << RESET;
	}else{
		cout << RED << "ERROR: Creating Invalidated Ballots file failed!\n" << RESET;
	}
	return;
}
