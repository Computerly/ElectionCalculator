#include <gtest/gtest.h>
#include "Election.h"
#include "Candidate.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

class ElectionTest : public ::testing::Test {
 public:
  void SetUp( ) {
    // code here will execute just before the test ensues

  }

};



//checks to see if the coin check values are under 2, since they
//have to be either 0 or 1
TEST_F(ElectionTest, flipcoincheck){
  std::string inFile = "sampleelection1.txt";
  Election test(inFile);
  int result;
  result =  test.flipCoin();
  EXPECT_LT(result,2);

}

//executes flipcoin method in a loop to see if heads and tails are
//each 50%
TEST_F(ElectionTest, validitycoincheck){
  std::string inFile = "sampleelection1.txt";
  Election test(inFile);
  int count = 0;
  for (int i = 0; i<1000; i++){
    int result = test.flipCoin();
    if (result==0){
      count++;
    }
  }
  EXPECT_EQ(count, 500);

}

//checks to see if the addwinner methods is correct
TEST_F(ElectionTest, addwinnercheck){
  std::string inFile = "sampleelection1.txt";
  Election test(inFile);

  Candidate Biden("Joe Biden");
  Candidate b = &Biden;
  Candidate Obama("Barrack Obama");
  Candidate o = &Obama;

  test.addWinner(b);
  test.addWinner(o);
  std::vector<Candidate*> winnerstest = test.getWinners();
  EXPECT_EQ(winnerstest.size(), 2);
}

//checks to see if the losers are added to the stack by checking the size
//and the top element
TEST_F(ElectionTest, addlosercheck){
  std::string inFile = "sampleelection1.txt";
  Election test(inFile);
  Candidate Biden("Joe Biden");
  Candidate b = &Biden;

  test.addLoser(b);

  std::vector<Candidate*> loserstest = test.getLosers();
  EXPECT_EQ(loserstest.size(), 1);
  Candidate check = loserstest.top();
  EXPECT_EQ(check.getName(), "Joe Biden");

}

//the following tests check to see if the information is obtained
//from a file correctly. 2 sample electiosn files were made to check

TEST_F(ElectionTest, getelectiontypecheck){
  std::string inFile = "sampleelection1.txt";
  Election test(inFile);
  EXPECT_STREQ(test.getElectionType(), "IRV");

}

TEST_F(ElectionTest, getnumcandidatescheck){
  std::string inFile = "sampleelection1.txt";
  Election test(inFile);
  //put known numbers into the sample file
  EXPECT_EQ(test.getNumCandidates(), 5);


}
TEST_F(ElectionTest, getnumseatscheck){
  std::string inFile = "sampleelection1.txt";
  Election test(inFile);
  //put known numbers into the sample file
  EXPECT_EQ(test.getNumSeats(), 1);

}



TEST_F(ElectionTest, getnumballotscheck){
  std::string inFile = "sampleelection1.txt";
  Election test(inFile);
  //put known numbers into the sample file
  EXPECT_EQ(test.getNumBallots(), 10);


}


TEST_F(ElectionTest, getdroopquotacheck){
  std::string inFile = "sampleelection1.txt";
  Election test(inFile);
  test.setDroopQuota(4);
  //put known numbers into the sample file
  EXPECT_EQ(test.getDroopQuota(), 4);


}

//checks to see if the type OPL receiving information correctly
//by passing in a known file for the other type of voting algorithm

TEST_F(ElectionTest, getelectiontypecheck){
  std::string inFile = "sampleelection2.txt";
  Election test(inFile);
  EXPECT_STREQ(test.getElectionType(), "OPL");

}

TEST_F(ElectionTest, getcandidatescheck){
  std::string inFile = "sampleelection2.txt";
  Election test(inFile);
  std::vector<Candidate> testcand = test.getCandidates();
  EXPECT_EQ(testcand.size(), 6);


}



int main(int argc, char **argv) {

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
