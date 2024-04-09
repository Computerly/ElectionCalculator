#include <gtest/gtest.h>
#include "Candidate.h"
#include <stdexcept>


class CandidateTest : public ::testing::Test {
 public:
  void SetUp( ) {
    // code here will execute just before the test ensues
	Candidate first_candidate = Candidate("John");
	Candidate second_candidate = Candidate("Michael");
  }

};

TEST_F(CandidateTest, checkinitialization){
  Candidate first("John");
  int num_of_votes = first.getNumVotes();
  std::string name = first.getName();
  EXPECT_EQ(num_of_votes, 0);
  EXPECT_EQ(name, "John");

}


TEST_F(CandidateTest, checkinitializationwithparty){
  Candidate first("George Washington", "Republican");
  Candidate second("Obama", "Democrat");
  Candidate third("Julius Caesar", "Independent");

  EXPECT_EQ(first.getNumVotes(), 0);
  EXPECT_EQ(first.getName(), "George Washington");
  EXPECT_EQ(first.getParty(), "Republican");
  EXPECT_EQ(first.isLoser(), false);
  EXPECT_EQ(first.isWinner(), false);
  EXPECT_EQ(second.getNumVotes(), 0);
  EXPECT_EQ(second.getName(), "Obama");
  EXPECT_EQ(second.getParty(), "Democrat");
  EXPECT_EQ(second.isLoser(), false);
  EXPECT_EQ(second.isWinner(), false);
  EXPECT_EQ(third.getNumVotes(), 0);
  EXPECT_EQ(third.getName(), "Julius Caesar");
  EXPECT_EQ(third.getParty(), "Independent");
  EXPECT_EQ(third.isLoser(), false);
  EXPECT_EQ(third.isWinner(), false);

}

TEST_F(CandidateTest, incrementtest){
  Candidate check("John");
  EXPECT_EQ(check.getNumVotes(), 0);
  check.incrementVotes();
  EXPECT_EQ(check.getNumVotes(), 1);
  check.incrementVotes();
  check.incrementVotes();
  check.incrementVotes();
  EXPECT_EQ(check.getNumVotes(), 4);
}


TEST_F(CandidateTest, getpartytest){
  Candidate candidate1("John", "Republican");
  Candidate candidate2("Julius", "Democrat");
  Candidate candidate3("Obama", "Independent");
  EXPECT_EQ(candidate1.getParty(), "Republican");
  EXPECT_EQ(candidate2.getParty(), "Democrat");
  EXPECT_EQ(candidate3.getParty(), "Independent");
}

TEST_F(CandidateTest, getnametest){
  Candidate candidate1("John", "Republican");
  Candidate candidate2("Julius", "Democrat");
  Candidate candidate3("Obama", "Independent");
  EXPECT_EQ(candidate1.getName(), "John");
  EXPECT_EQ(candidate2.getName(), "Julius");
  EXPECT_EQ(candidate3.getName(), "Obama");
}

TEST_F(CandidateTest, setnametest){
  Candidate test;
  test.setName("Biden");
  EXPECT_EQ(test.getName(), "Biden");

}

TEST_F(CandidateTest, setpartytest){
  Candidate test;
  test.setParty("Democratic");
  EXPECT_EQ(test.getParty(), "Democratic");
}

TEST_F(CandidateTest, logvotecounttest){
  Candidate test;
  test.setParty("Democratic");
  test.logVoteCount();
  std::vector<int> testvector = test.getVoteHistory();
  EXPECT_EQ(testvector[0], 0);
  test.incrementVotes();
  test.logVoteCount();
  testvector = test.getVoteHistory();
  EXPECT_EQ(testvector[1], 1);

}

TEST_F(CandidateTest, setvotetest){
  Candidate test;
  test.setParty("Democratic");
  test.setVotes(5);
  EXPECT_EQ(test.getNumVotes(), 5);
}







int main(int argc, char **argv) {

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
