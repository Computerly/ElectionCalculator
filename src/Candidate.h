#ifndef CANDIDATE_H
#define CANDIDATE_H

#include <string>
#include <vector>

#include "Party.h"

class Party;

class Candidate{
public:
	/**
	 * Constructs Candidate object with nothing initialized.
	 * 
	 * Sets all other members to default state of 0 or false
	 * 
	*/
	Candidate();

	/**
	 * Constructs Candidate object with name initialized.
	 * 
	 * Sets all other members to default state of 0 or false
	 * 
	 *  @param std::string name
	*/
	Candidate(std::string name);

	/**
	 * Constructs Candidate object with name and party initialized.
	 * 
	 * Sets all other members to default state of 0 or false
	 * 
	 *  @param std::string name
	 *  @param std::string party
	*/
	Candidate(std::string name, std::string party);
	
	/**
	 * @brief Gets the number of votes for the candidate
	 * 
	 * @return int (the number of votes)
	 */
	int getNumVotes();

	/**
	 * @brief Function to determine if a candidate is a winner based on the value of the win variable
	 * 
	 * @return true If win's value is true
	 * @return false If win's value is false
	 */
	bool isWinner();

	/**
	 * @brief Function to determine if a candidate is a loser based on the value of the lose variable
	 * 
	 * @return true If lose's value is true
	 * @return false If lose's value is false
	 */
	bool isLoser();

	/**
	 * Increments Candidate's votes by n amount
	 * 
	 * @param int n (optional, default 1)
	*/
	void incrementVotes(int n = 1);

	/**
	 * Increments Candidate's party votes by n amount
	 * 
	 * @param int n (optional, default 1)
	*/
	void incrementPartyVotes(int n = 1);

	/**
	 * Decrements Candidate's votes by n amount
	 * 
	 * @param int n (optional, default 1)
	*/
	void decrementVotes(int n = 1);

	/**
	 * Sets Candidate's name to name
	 * 
	 * @param std::string name
	*/
	void setName(std::string name);

	/**
	 * Sets Candidate's party to party
	 * 
	 * @param std::string
	*/
	void setParty(std::string party);

	/**
	 * Sets Candidate's votes to n
	 * 
	 * @param int n
	*/
	void setVotes(int n);

	/**
	 * Logs current vote count of Candidate
	 * 
	 * This is used by IRV to track candidates during runoff
	 * 
	*/
	void logVoteCount();

	/**
	 * Sets win state of the Candidate to _win
	 * 
	 * @param bool _win
	*/
	void setWinState(bool _win);

	/**
	 * Sets lose state of the Candidate to _lose
	 * 
	 * @param bool _lose
	*/
	void setLoseState(bool _lose);

	/**
	 * Returns the name of the Candidate
	 * 
	*/
	std::string getName();

	/**
	 * @brief Gets the party of the candidate
	 * 
	 * @return std::string (the name of the party that the candidate belongs to)
	 */
	std::string getParty();

	/**
	 * @brief Returns the vote history of the Candidate
	 * 
	 * @return std::vector<int> (the vote history vector)
	 */
	std::vector<int> getVoteHistory();

	std::string candidateName;
private:
	int numVotes;
	bool win;
	bool lose;
	std::string candidateParty;
	std::vector<int> vnVoteHistory;
};

#endif
