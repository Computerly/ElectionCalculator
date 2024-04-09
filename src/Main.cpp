#include <iostream>
#include <string>
#include <sys/stat.h>
#include <chrono>
#include <fstream>

#include "Election.h"
#include "IRV.h"
#include "OPL.h"
#include "PO.h"
#include "helpers/colors.h"
#include "helpers/config.h"

using namespace std;
using namespace std::chrono;

bool SHUFFLE = true;

/**
 * @brief Main function for facilitating system execution
 * 
 * Checks if "-S" has been passed to enable shuffle.
 * Asks for input files until successful.
 * Runs appropriate election based on first line in the first election file passed.
 * Tracks how long an election takes to run and outputs time after election finishes.
 * 
 * @param argc Number of command line arguments
 * @param argv The actual command line arguments
 * @return int The return value, where 0 represents successful execution, anything else error
 */
int main(int argc, char const *argv[]) {

	// ****************************************************************************
	// *                                 get args                                 *
	// ****************************************************************************

	if(argc == 2){
		if(argv[1][1] == 'S'){
			SHUFFLE = false;
			cout << BOLDYELLOW << "Shuffle off\n" << RESET;
		}else{
			cout << BOLDRED << "Invalid arg passed!: " << argv[1] << RESET;
			exit(-1);
		}
	}

	// ****************************************************************************
	// *                           Seed Random Function                           *
	// ****************************************************************************

	srand(time(NULL));

	// ****************************************************************************
	// *                                 Greeting                                 *
	// ****************************************************************************

	cout << BOLDYELLOW
		 << " __      __   _                  \n \\ \\    / /__| |__ ___ _ __  ___ \n  \\ \\/\\/ / -_) / _/ _ \\ '  \\/ -_)\n   \\_/\\_/\\___|_\\__\\___/_|_|_\\___|\n                                 \n"
		 << RESET;
	
	// ****************************************************************************
	// *                               Get File Name                              *
	// ****************************************************************************

	string sFiles;
	bool exists = true;
	bool valid = true;
	vector<string> files;
	
	// two character still breaking
	do{
		valid = true;
		cout << "Please Enter Ballot File(s) (comma seperated) (you can ommit '.csv'): " << YELLOW;
		getline(cin, sFiles);
		cout << RESET;
		
		int nTokenStart = 0;
		for(int i = 0; i < sFiles.length(); i++){
			if(sFiles.at(i) == ' ' || sFiles.at(i) == '\t'){ nTokenStart = i + 1; continue; } // skip spaces or indents
			
			if(nTokenStart == i - 1 && sFiles.length() != 2) { continue; } // skip duplicate commas or empty file names
			
			else if(sFiles.at(i) == ',' || i == sFiles.length() - 1){
				if(i == sFiles.length() - 1){ i++; }

				string token = sFiles.substr(nTokenStart, i - nTokenStart);
				nTokenStart = i + 1;

				if(!(token.length() > 5 && token[token.length() - 4] == '.')) {  token += ".csv"; } // add file extension if missing
				
				struct stat buffer;
				exists = (stat ((token).c_str(), &buffer) == 0);
				valid = exists && valid;

				if( exists ){ files.push_back(token); }
				else { cout << RED << '\"' << token << "\" does not exist.\n" << RESET; }
			}

		}

	} while( !valid );

	auto chrono_start = high_resolution_clock::now();

	// ****************************************************************************
	// *                             Get Election Type                            *
	// ****************************************************************************

	string sElectionType = "";
	
	ifstream electionFile(files[0]);
	
	if(electionFile.good()) { getline(electionFile, sElectionType); }

    // ****************************************************************************
	// *                          Run appropriate Election                        *
	// ****************************************************************************

	if(sElectionType == "IRV") {
		IRV irvElection(files);

		chrono_start = high_resolution_clock::now();

		irvElection.runElection();
	}
	else if(sElectionType == "OPL") {
		OPL oplElection(files);
		chrono_start = high_resolution_clock::now();
		oplElection.runElection();
	}
	else if(sElectionType == "PO"){
		PO poElection(files);
		chrono_start = high_resolution_clock::now();
		poElection.runElection();	
	}
	else { cout << RED << "\nAn error has occured reading the file. Please ensure the file exists and the formatting is correct.\n" << RESET; }

	auto chrono_stop = high_resolution_clock::now();
	auto duration = chrono::duration<double>(chrono_stop - chrono_start).count();
	
	// ****************************************************************************
	// *                     Color-coded time execution output                    *
	// ****************************************************************************

	if(duration < 0.5) 		{ cout << GREEN; 	}
	else if(duration < 1.0) { cout << YELLOW;	}
	else 					{ cout << RED;	 	}

	cout << "\nExecuted in: " << duration << " seconds.\n" << RESET;

	return 0;
}
