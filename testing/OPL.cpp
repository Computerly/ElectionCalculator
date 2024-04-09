#include "OPL.h"
#include <stdexcept>
#include <fstream>
#include <algorithm>
#include "helpers/colors.h"

using namespace std;

/**
 * @brief Construct a new OPL::OPL object given a file name
 * 
 * @param _file_name The filename of the election file to run the OPL election for
 */
OPL::OPL(string _file_name) : Election(_file_name){

	// ****************************************************************************
	// *                        make unique list of parties                       *
	// ****************************************************************************
	//cout <<" yo" ;
	for(int i = 0; i < getNumCandidates(); i++){
		addParty( getCandidates()[i].getParty() );
		//cout << getCandidates()[i].getParty();
	}


	// ****************************************************************************
	// *                          Calculate Droop Quota                           *
	// ****************************************************************************

	setDroopQuota(getNumBallots() / getNumSeats());
}

/**
 * @brief Destroy the OPL::OPL object
 * 
 */
OPL::~OPL() = default;

/**
 * @brief The function to run the OPL election process
 * 
 * Digest all ballots
 * While digesting, tally votes for candidate
 * Once finished, sum votes for each party
 * Allocate votes by party then candidate
 * Repeat until all seats gone
 */
void OPL::runElection(){
	
	// ****************************************************************************
	// *                              Digest Ballots                              *
	// ****************************************************************************

	digestBallots();
	allocateCandidates();

	// ****************************************************************************
	// *                                Compute OPL                               *
	// ****************************************************************************

	// Find number of seats for each party; keep track of seats and remainder.
	// Subtract from total seats
	int nTotalSeats = getNumSeats();
//	cout << "Number of Seats: " << nTotalSeats << "\n";
	
	for (int i = 0; i < getNumParties(); i++) {
		//cout << "Num votes " << getParties()[i].getNumVotes();
		int nSeats = getParties()[i].getNumVotes() / getDroopQuota();
		int nRemainder = getParties()[i].getNumVotes() % getDroopQuota();
		//cout << getParties()[i].getNumSeats();
		//cout <<"\n";
		//cout << "hi \n";

		nTotalSeats -= nSeats;

		string audit = "Add ";
		audit.append(to_string(nSeats));
		audit.append(" seat(s) to ");
		audit.append(getParties()[i].getPartyName());
		addAuditLine(audit);
		getParties()[i].incrementSeats(nSeats);
		getParties()[i].setRemainder(nRemainder);
		//cout << "Party " << getParties()[i].getPartyName() << " " << getParties()[i].getNumVotes() << " " << getParties()[i].getRemainder() << " "<< getParties()[i].getNumSeats() << "\n";
	}
	// sort parties
	// if nTotalSeats
	if(nTotalSeats > 0){
		//Sort Parties by remainder of votes and add votes until complete
		vector<Party> toSortParties = getParties();

		sort(toSortParties.begin(), toSortParties.end(), [](auto a, auto b) { return a.getRemainder() > b.getRemainder();});
		
		/*for(int i=0; i<toSortParties.size(); i++)
		{
			cout << toSortParties[i].getPartyName() << " " << toSortParties[i].getRemainder() << "\n";
		}*/

		if(toSortParties[0].getRemainder() == toSortParties[1].getRemainder())
		{
			if(flipCoin())
			{
				string audit = "Tiebreaker, swap ";
				audit.append(toSortParties[0].getPartyName());
				audit.append(" and ");
				audit.append(toSortParties[1].getPartyName());
				
				addAuditLine(audit);
				//cout << "test" << "\n";
				Party temp = toSortParties[0];
				toSortParties[0] = toSortParties[1];
				toSortParties[1] = temp;
			}
			else{
				string audit = "Tiebreaker, don't swap ";
				audit.append(toSortParties[0].getPartyName());
				audit.append(" and ");
				audit.append(toSortParties[1].getPartyName());

				addAuditLine(audit);
			}
		}

		/*for(int i=0; i<toSortParties.size(); i++)
		{
			cout << toSortParties[i].getPartyName() << " " << toSortParties[i].getRemainder() << "\n";
		}*/
		setParties(toSortParties);
		for (int i = 0; i < nTotalSeats; ++i){
			string audit = "Add 1 seat to ";
			audit.append(getParties()[i].getPartyName());
			addAuditLine(audit);

			getParties()[i].incrementSeats();
		}
		
	}

	// Divide remaining seats in order of larget remainders


	// ****************************************************************************
	// *                              Display Results                             *
	// ****************************************************************************

	displayResults();

	// ****************************************************************************
	// *                         Write to Election Results                        *
	// ****************************************************************************

	writeElectionResults("OPLElection" + to_string(getDroopQuota()) + ".txt");
	writeToAudit();
}

/**
 * @brief Function to display the OPL election results to the screen
 * 
 * @note Includes all parties and winning candidates
 * 
 */
void OPL::displayResults(bool doColors /*= true*/){
	if (doColors) { cout << BOLDWHITE; }
	cout << "Election Results:\n"; 
	if (doColors) { cout << RESET; }
	cout << "Type: OPL\n"
		 << "Number of Candidates: " << getNumCandidates() << "\n"
		 << "Number of Seats: " << getNumSeats() << '\n'
		 << "Number of Ballots: " << getNumBallots() << '\n'
		 << "Calculated Droop Quota: " << getDroopQuota() << '\n'
		 << "Candidates: ";

		 for (int i = 0; i < getNumCandidates() -1 ; i++){
		 	cout << getCandidates()[i].getName() << "/" << getCandidates()[i].getParty() << ", ";
		 }
		 cout << getCandidates()[getNumCandidates()-1].getName() << "/" << getCandidates()[getNumCandidates()-1].getParty();
	cout << "\n" << "\n";
	for (int i = 0; i < getNumCandidates(); i++){
		cout << "Candidate: " << getCandidates()[i].getName() << " Votes: " << getCandidates()[i].getNumVotes() << '\n';
	}
	for (int i = 0; i < getNumParties(); i++){
		cout << "Party: " << getParties()[i].getPartyName() << " Seats: " << getParties()[i].getNumSeats() << '\n';
	}
	vector<Candidate> winners;
	vector<Party> topCands;
	Party party;
	//cout << "hello" << "\n";
	if(getNumWinners() == 0)
	{
		for (int i=0; i< getNumParties(); i++)
		{
			party = getParties()[i];
			if(party.getNumSeats() > 0)
			{
				topCands.push_back(distributeSeats(party));
			}
			for(int x=0; x<party.getNumSeats(); x++)
			{
				//cout << topCands[i].getCandidates()[x].getName() << "\n";
				winners.push_back(topCands[i].getCandidates()[x]);
			}
		}
	}
	
	// /cout << winners.size() << "\n";
	//cout << "hello" << "\n";
	//cout << winners.size() << "\n";
	for(int z=0 ; z < winners.size(); z++)
	{
		cout << "Winner " << winners[z].getName() << " " << "\n";
		addWinner(&winners[z]);
		string audit = "Add winner ";
		audit.append(winners[z].getName());
		audit.append(" from party ");
		audit.append(winners[z].getParty());
		audit.append(" ");
		addAuditLine(audit);
	}
}

vector<Candidate> OPL::distributeSeats(Party parties)
{
	int seats = parties.getNumSeats();
	vector<Candidate> candList = parties.getCandidates();
	
	//Shuffle in case of tie
	int random = rand() % 17;

	string audit = "Shuffle the ";
	audit.append(parties.getPartyName());
	audit.append(" party ");
	audit.append(to_string(random));
	audit.append(" amount of times");
	addAuditLine(audit);
	//cout << audit << "\n";

	for (int i=0; i < random; i++)
	{
		random_shuffle(candList.begin(), candList.end());
	}
	

	sort(candList.begin(), candList.end(), [](auto a, auto b) { return a.getNumVotes() > b.getNumVotes();});

	return candList;
}

/**
 * @brief Function to allocate votes to the correct candidate based on the ballot
 * 
 * @param buff The ballot itself, where a 1 represents the voter's preference
 */
void OPL::digest(string buff){
	for (int x = 0; x < buff.length(); x++){
		if(buff[x] == '1'){
			getCandidates()[x].incrementVotes();
			//getParties()[x].incrementVotes();
		} 
	}
}

/**
 * @brief Gets the number of parties participating in the election
 * 
 * @return int The number of parties in the election
 */
int OPL::getNumParties() { return vParties.size(); }

/**
 * @brief Gets the vector of the parties participating in the election
 * 
 * @return vector<Party>& The vector of parties
 */
vector<Party> &OPL::getParties(){ return vParties; }

void OPL::setParties(vector<Party> parties){ vParties = parties; }


/**
 * @brief Function to add a party to the election
 * 
 * @param p The party to be added to the party vector
 */
void OPL::addParty(Party p){
	// Ensure uniqueness
	for(int i = 0; i < getNumParties(); i++){
		if(p.getPartyName() == getParties()[i].getPartyName()) { return; }
	}
	if(p.getNumSeats() < 0) // Solves issue of parties starting with negative seats
	{
		p.incrementSeats(-1*p.getNumSeats());
	}
	vParties.push_back(p);
}

/**
 * @brief Function to distribute the candidates to their respective parties
 * 
 */
void OPL::allocateCandidates()
{
	for (int i=0; i< getNumCandidates(); i++)
	{
		//cout << pArray[i].getPartyName();
		for (int x=0; x < getNumParties(); x++)
		{
			if(getCandidates()[i].getParty() == vParties[x].getPartyName())
			{
				//cout << "Adding to " << vParties[x].getPartyName() << " " << getCandidates()[i].getName() << " " << getCandidates()[i].getNumVotes() << '\n';
				vParties[x].addCandidate(getCandidates()[i]);
				//vParties[x].incrementSeats(getCandidates()[i].getNumVotes());
			}
		}
	}
}

/**
 * @brief Helper function to add string S to the audits vector
 * 
 * @param s String to be added to the audits vector
 */
void OPL::addAuditLine(string s){
	audits.push_back(s);
}

void OPL::writeToAudit(string s){
	time_t t = time(nullptr);
	if(s.length() == 0){
		s = "OPLElectionAudit" + to_string(t) + ".txt";
	}
	ofstream auditFile(s);
	if(auditFile.good()){
		
		auditFile 	<< "Election type: OPL\n"
					<< "Election was ran on: "<< asctime(localtime(&t)) << '\n';
		
		streambuf *psbuf, *backup;

		backup = std::cout.rdbuf();		// back up cout's streambuf

		psbuf = auditFile.rdbuf();	// get file's streambuf
		cout.rdbuf(psbuf);				// assign streambuf to cout

		displayResults(false); 			// Prevent colors

		cout.rdbuf(backup);				// restore cout's original streambuf

		auditFile << "\nElection Decisions: \n";
		for (int i = 0; i < audits.size(); i++){
			auditFile << audits[i] << '\n';
		}
		auditFile << "\n END OF AUDIT FILE";

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

vector<Candidate*> OPL::getWinners()
{
	return Election::getWinners();
}