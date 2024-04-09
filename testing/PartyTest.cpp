#include <gtest/gtest.h>
#include "Party.h"
#include "Candidate.h"
#include <stdio.h>

class PartyTest : public ::testing::Test {
 public:
  void SetUp( ) {

  }



};

//check Party initialization with no arguments
TEST_F(PartyTest, Partyinitialization){
  Party first;
  EXPECT_EQ(first.getPartyName(), "Independent");
  EXPECT_EQ(first.getNumVotes(), 0);
  std::vector<Candidate> list_of_candidates = first.getCandidates();
  EXPECT_EQ(list_of_candidates.size(), 0); //checks to see if this is an empty vector

}


//check Party initialization with a new party object

TEST_F(PartyTest, Newpartywithname){
  Party Democrat("Democratic");
  Party Republic("Republican");
  EXPECT_EQ(Democrat.getPartyName(), "Democratic");
  EXPECT_EQ(Democrat.getNumVotes(), 0);
  EXPECT_EQ(Republic.getPartyName(), "Republican");
  EXPECT_EQ(Republic.getNumVotes(), 0);
  EXPECT_EQ(Democrat.getCandidates().size(), 0);
  EXPECT_EQ(Republic.getCandidates().size(), 0);


}

TEST_F(PartyTest, Newpartywithcandidateslist) {
  Candidate first("John");
  Candidate second("Julius");
  Candidate third("Obama");
  std::vector<Candidate> democrats;
  std::vector<Candidate> other;
  democrats.push_back(third);
  democrats.push_back(second);
  other.push_back(first);
  Party check(democrats);
  EXPECT_EQ(check.getPartyName(), "Independent");
  EXPECT_EQ(check.getNumVotes(), 0);
  std::vector<Candidate> test;
  test = check.getCandidates();

  EXPECT_EQ(test.size(), democrats.size());
  for (int i = 0; i < 2; i++){
    EXPECT_EQ(test[i].getName(), democrats[i].getName());
  }

}

TEST_F(PartyTest, newpartywithnameandcandidatelst){
  Candidate first("John");
  Candidate second("Julius");
  std::vector<Candidate> democrats;
  democrats.push_back(first);
  democrats.push_back(second);
  Party check(democrats, "Democratic");
  EXPECT_EQ(check.getPartyName(), "Democratic");
  EXPECT_EQ(check.getNumVotes(), 0);
  std::vector<Candidate> test = check.getCandidates();
  for (int i = 0; i < 2; i++){
    EXPECT_EQ(test[i].getName(), democrats[i].getName());
  }


}

TEST_F(PartyTest, checkwronginput){}

TEST_F(PartyTest, getvotestest){
  Candidate first("John");
  first.incrementVotes();
  first.incrementVotes();
  Candidate second("Julius");
  second.incrementVotes();
  second.incrementVotes();
  std::vector<Candidate> democrats;
  democrats.push_back(first);
  democrats.push_back(second);
  Party Democratic(democrats, "Democratic");

  std::vector<Candidate> republicans;
  Candidate third("John");
  Candidate fourth("Julius");
  republicans.push_back(third);
  republicans.push_back(fourth);
  Party Republican(republicans, "Republican");
  Party Random;
  EXPECT_EQ(Democratic.getNumVotes(), 4);
  EXPECT_EQ(Republican.getNumVotes(), 0);
  EXPECT_EQ(Random.getNumVotes(), 0);
  fourth.incrementVotes();
  third.incrementVotes();
  EXPECT_EQ(Republican.getNumVotes(), 2); //failing this test

}

TEST_F(PartyTest, addCandidatetest){
  Party Democratic;
  Candidate Biden("Joe Biden");
  Candidate Obama("Barrack Obama");
  Democratic.addCandidate(Biden);
  Democratic.addCandidate(Obama);
  std::vector<Candidate> testvector = Democratic.getCandidates();
  EXPECT_EQ(testvector.size(), 2);
  EXPECT_EQ(testvector[0].getName(), "Joe Biden");
  EXPECT_EQ(testvector[1].getName(), "Barrack Obama");

  Party Republican;
  Candidate Trump("Donald Trump");
  Republican.addCandidate(Trump);
  std::vector<Candidate> testvector2 = Republican.getCandidates();
  EXPECT_EQ(testvector2.size(), 1);
  EXPECT_EQ(testvector2[0].getName(), "Donald Trump");


}

TEST_F(PartyTest, removeCandidatetest){

  std::vector<Candidate> democrats;
  Candidate Biden("Joe Biden");
  Candidate Obama("Barrack Obama");
  democrats.push_back(Biden);
  democrats.push_back(Obama);
  Party Democratic(democrats, "Democratic");
  Democratic.removeCandidate(Biden);
  std::vector<Candidate> testvector = Democratic.getCandidates();
  EXPECT_EQ(testvector.size(), 1);
  EXPECT_EQ(testvector[0].getName(), "Barrack Obama");

  std::vector<Candidate> republicans;
  Candidate Trump("Donald Trump");
  republicans.push_back(Trump);
  Party Republican(republicans, "Republican");
  Republican.removeCandidate(Trump);
  std::vector<Candidate> testvector2 = Republican.getCandidates();
  EXPECT_EQ(testvector2.size(), 0);


}





int main(int argc, char **argv) {

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
