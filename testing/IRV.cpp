// ****************************************************************************
// *                                    IRV                                   *
// *                               Version: 1.0                               *
// * March, 2022                                                              *
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

#define GOT_HERE std::cout << "At " __FILE__ ":" << __LINE__ << std::endl

/**
 * @brief Construct a new IRV::IRV object
 *
 * @param _file_name The filename of the election file to run IRV the election for
 */
IRV::IRV(string _file_name) : Election(_file_name){

	// ****************************************************************************
	// *                         Initialize Ballot Array                          *
	// * Sum (number of candidates) P r, where r ranges 1 to number of candidates *
	// ****************************************************************************
	int sizeOfBallotArray = getNumCandidates();
	nBallotArray = new int[ Combinations::sizeOf( sizeOfBallotArray ) ]();

	// ****************************************************************************
	// *                          Calculate Droop Quota                           *
	// ****************************************************************************

	setDroopQuota(( getNumBallots() / ( getNumSeats() + 1 ) ) + 1);
}

/**
 * @brief Destroy the IRV::IRV object
 *
 */
IRV::~IRV() {
	delete[] nBallotArray;
}


/**
 * @brief Main function for running the IRV election process
 *
 * Handles digest and ingest of ballots
 * Handles runoff until completion
 * Calls displayResults() and writeToAudit()
 */
void IRV::runElection(){

	// ****************************************************************************
	//                                Digest Ballots
	//       Parses the election file and inserts ballots in the tally array.
	// ============================================================================
	//	Each ballot (row), is converted into a ranking number.
	//	Ranking numbers are formed by ordering the nPreference of the candidates.
	//	For example:
	//		"2,1,3,5,4" is a given ballot.
	//		The first choice is the second candidate.
	//		So the first number in the ranking number is 2.
	//		Then the second choice is the first candidate.
	//		So the second number in the ranking number is 1.
	//		This repeats until all choices are recorded.
	//		For this given ballot, the corollary ranking number is: "21354".
	//
	//	If a voter does not vote for all candidates (but at least one),
	//	it is translated into a zero.
	//	So for example, if not all of the candidates were chosen,
	//	a ballot could look like "4,,1,3,2".
	//	Its corollary ranking number would be: "35410"
	//	because there was no fifth choice given.
	//
	//	While digesting ballots and converting into rankings,
	//	the first choice is automatically tallied into the tally array.
	//
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

	writeElectionResults("IRVelection" + to_string(getDroopQuota()) + ".txt");

	// ****************************************************************************
	// *                                Write Audit                               *
	// ****************************************************************************

	writeToAudit();
}


/**
 * @brief Function to compute runoff until all seats are filled
 *
 */
void IRV::runOff(){

	// ****************************************************************************
	// *            Number of combinations associated with a candidate            *
	// ****************************************************************************

	int nBlockSize = Combinations::sizeOf(getNumCandidates()) / getNumCandidates();

	int *nToTally = new int[nBlockSize];

	// ****************************************************************************
	// *                              Run Droop Quota                             *
	// ****************************************************************************

	for(int nCount = 0; nCount < (getNumCandidates() - getNumSeats()); nCount++){ // TODO: ensure this is correct

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

		addLoser(&getCandidates()[nLowestCandidate]);

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
 			srand(time(0)); // INCLUDE??
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
		// *                                   TODO                                   *
		// *                     Mathematically determine segments                    *
		// ****************************************************************************
		for(int i = 0; i < nBlockSize; i++){
			int nRank = Combinations::getRankingNumber(nToTally[i], getNumCandidates());

			for(int nPreference = nCount; nPreference < getNumCandidates() - 1; nPreference++){

				int nCandidate = (( nRank / quick_pow10(getNumCandidates() - nPreference - 2) ) % 10) - 1;

				if( nCandidate != -1 && !getCandidates()[nPreference].isWinner() && !getCandidates()[nPreference].isLoser()){

					getCandidates()[nPreference].incrementVotes(nBallotArray[ nToTally[i] ]);

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

	/* UNTESTED !!*/
	sort(getCandidates().begin(), getCandidates().end(),[](auto a, auto b) { return a.getNumVotes() > b.getNumVotes(); } );

	// ****************************************************************************
	// *               Add top number_of_seats candidates to winners              *
	// ****************************************************************************

	for(int i = 0; i < getNumSeats(); i++){
		addWinner(&(getCandidates()[i]));
	}
	handleWin();
}


/**
 * @brief Function to display the IRV election results to the screen
 *
 * @note Includes The election information as well
 */
void IRV::displayResults(bool doColors /*= true*/){
	if (doColors) { cout << BOLDWHITE; }
	cout << "Election Results:\n";
	if (doColors) { cout << RESET; }
	cout << "Type: IRV\n"
		 << "Number of Candidates: " << getNumCandidates() << "\n"
		 << "Number of Seats: " << getNumSeats() << '\n'
		 << "Number of Ballots: " << getNumBallots() << '\n'
		 << "Calculated Droop Quota: " << getDroopQuota() << '\n'
		 << "Candidates: ";

		 for (int i = 0; i < getNumCandidates(); i++){
		 	cout << getCandidates()[i].getName() << "/" << getCandidates()[i].getParty() << ", ";
		 }


	string sep = " | ";
	int nTwoSep = 2 * sep.length();

	int nCadidateColWidth = 10 + sep.length();
	for(int i = 0; i < getNumCandidates(); i++){
		if( getCandidates()[i].getName().length() > nCadidateColWidth ) { nCadidateColWidth = getCandidates()[i].getName().length(); }
	}
	int cCols = getCandidates()[0].getVoteHistory().size();
	int nSpaceSize = 3;


	int nTransferWidth = 8;
	int nVoteWidth = getNumCandidates();
	if (doColors) { cout << BOLDMAGENTA; }
	   cout << '\n'
		 	<< left
			<< sep << setw(nCadidateColWidth) << "Candidates" << sep
			<< setw(nSpaceSize) << sep << setw(nVoteWidth) << "Votes" << sep << setw(nSpaceSize);

	for(int i = 0; i < cCols - 1; i++){
	   cout << left
	   		//<< sep << ' ' << setw(nTransferWidth) << "Transfer" << sep
	   		<< setw(nSpaceSize) << sep << setw(nVoteWidth) << "Votes" << sep << setw(nSpaceSize);

	}
	if (doColors) { cout << RESET; }
	cout << "\n";

	auto f = [&sep, &nSpaceSize, &nTransferWidth, &nVoteWidth, &nCadidateColWidth, &cCols, &doColors](auto c){
		vector<int> history;

		for(int i = c.size() - 1; i > -1 ; i--){

			history = c[i]->getVoteHistory();

			if(doColors){
				if(c[i]->isWinner()) { cout << BOLDGREEN; }
				else { cout << RESET; }
			}

			cout << right
				 << sep << setw(nCadidateColWidth) << c[i]->getName() + "/" + c[i]->getParty() << sep << setw(nSpaceSize)
				 << sep << setw(nVoteWidth) << history[0] << sep << setw(nSpaceSize);

			for(int column = 1; column < cCols; column++){
				// transfer happens iff
				if(column >= history.size() || history[column] == 0){
					cout << right
						// << sep  << '+' << setw(nTransferWidth) << history[h] - history[h - 1] << sep << setw(nSpaceSize)
						 << sep << setw(nVoteWidth) << string(nVoteWidth, '-')  << sep << setfill(' ') << setw(nSpaceSize);
				}else{
					cout << right
					// << sep  << '+' << setw(nTransferWidth) << history[h] - history[h - 1] << sep << setw(nSpaceSize)
				 	<< sep << setw(nVoteWidth) << history[column] << sep << setw(nSpaceSize);
				}
			}
			cout << '\n';
		}
	};

	f(getWinners());
	f(getLosers());
	cout << "\n";
}

/**
 * @brief Function to allocate votes to the first choice candidates
 *
 * @param buff The ballot itself, where a 1 represents the voter's first choice
 */
void IRV::digest(string buff){
	unsigned short ranking = 0;
	unsigned short c = 1;

		for (int i = 0; i < buff.length(); i++){
			if(buff[i] == ','){

				if(i == 0 || buff[i - 1] == ',') { c++; }	// check if first char, and check if last char was ','
				continue;									// else continue

			} else { // build ranking number

				int nLine = buff[i] - '0';
				ranking += (c * quick_pow10((getNumCandidates() - nLine)));

				if( nLine == 1) { getCandidates()[c - 1].incrementVotes(); } // Add Tally
				c++;
			}
		}

		nBallotArray[ Combinations::indexOf(ranking, getNumCandidates()) ] += 1; // Add Ranking
}

// ****************************************************************************
// *                                  Helpers                                 *
// ****************************************************************************

/**
 * @brief Helper function to handle the vote tally for a round
 *
 * @note Adds the non-winning candidates to the loser vector
 */
void IRV::handleWin(){

	// ****************************************************************************
	// *                           Log Round of Tallies                           *
	// ****************************************************************************
	logCandidates();

	// ****************************************************************************
	// *                      Add not winners to loser vector                     *
	// ****************************************************************************

	for (int i = 0; i < getNumCandidates(); i++){
		if(!getCandidates()[i].isWinner()){
			addLoser(&getCandidates()[i]);
		}
	}

}

/**
 * @brief Helper function to add string S to the audits vector
 *
 * @param s String to be added to the audits vector
 */
void IRV::addAuditLine(string s){
	audits.push_back(s);
}

/**
 * @brief Function to write the election progression to an audit file
 *
 * @param s String to be added to the audit file
 */
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

		psbuf = auditFile.rdbuf();	// get file's streambuf
		cout.rdbuf(psbuf);				// assign streambuf to cout

		displayResults(false); 			// Prevent colors

		cout.rdbuf(backup);				// restore cout's original streambuf

		auditFile << "\nBallot Allocations: \n";
		for (int i = 0; i < audits.size(); i++){
			auditFile << audits[i] << '\n';
		}
		auditFile << "END OF AUDIT FILE";

		auditFile.close();
		cout << CYAN << "Audit file created at: " + s + "\n" << RESET;
	}else{
		cout << RED << "ERROR: Creating audit file failed!\n" << RESET;
	}
	// When election was ran
	// System stats if time
	// Print headers
	// Vote history for each candidate
	return;
}

/**
 * @brief Helper function to add the candidates' votes to the audits vector to then
 * be added to the audit file
 *
 */
void IRV::logCandidates(){
	for(int i = 0; i < getNumCandidates(); i++){
		getCandidates()[i].logVoteCount();
		addAuditLine(getCandidates()[i].getName() + " has " + to_string(getCandidates()[i].getNumVotes()) + " votes.");
	}
}

vector<Candidate*> IRV::getWinners()
{
	return Election::getWinners();
}

vector<Candidate*> IRV::getLosers()
{
	return Election::getLosers();
}
