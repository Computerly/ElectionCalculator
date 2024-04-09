#include <gtest/gtest.h>
#include "Candidate.h"
#include "Party.h"
#include "Election.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;


class MultiplefilesTest : public ::testing::Test {
 public:
  void SetUp( ) {
    // code here will execute just before the test ensues
//	Candidate first_candidate = Candidate("John");
//	Candidate second_candidate = Candidate("Michael");
  }

};

TEST_F(MultiplefilesTest, checkonefile-IRV){
  std::vector<Candidate> vectortest;

  Candidate Rosen("Rosen", "Democrat");
  Candidate Susan("Susan", "Democrat");
  Candidate Kleinberg("Kleinberg", "Republican");
  Candidate Himmelberg("Himmelberg", "Republican");
  Candidate Chou("Chou", "Independent");
  vectortest.push_back(Rosen);
  vectortest.push_back(Susan);
  vectortest.push_back(Kleinberg);
  vectortest.push_back(Himmelberg);
  vectortest.push_back(Chou);

  IRV test("sampleelection1.csv", 5, 1, 10, vectortest);
  test.runElection();
//check the winners
  vector<Candidate*> winnerstest = test.getWinners();
  EXPECT_EQ(winnerstest.size(), 1);
  EXPECT_EQ(winnerstest[0].getName(), "Susan");
//check the losers
  vector<Candidate*> loserstest = test.getLosers();
  EXPECT_EQ(loserstest.size(), 4);
  EXPECT_EQ(loserstest[3].getName(), "Rosen");

  EXPECT_EQ(loserstest[2].getName(), "Himmelberg");

  EXPECT_EQ(loserstest[1].getName(), "Chou");

  EXPECT_EQ(loserstest[0].getName(), "Kleinberg");


}


TEST_F(MultiplefilesTest, checktwofiles-IRV){
  std::vector<Candidate> vectortest;

  Candidate Rosen("Rosen", "Democrat");
  Candidate Susan("Susan", "Democrat");
  Candidate Kleinberg("Kleinberg", "Republican");
  Candidate Himmelberg("Himmelberg", "Republican");
  Candidate Chou("Chou", "Independent");
  vectortest.push_back(Rosen);
  vectortest.push_back(Susan);
  vectortest.push_back(Kleinberg);
  vectortest.push_back(Himmelberg);
  vectortest.push_back(Chou);

  IRV test("sampleelection1.csv", "samplemultiple-1.csv", 5, 1, 10, vectortest);
  test.runElection();
//check the winners
  vector<Candidate*> winnerstest = test.getWinners();
  EXPECT_EQ(winnerstest.size(), 1);
  EXPECT_EQ(winnerstest[0].getName(), "Susan");
//check the losers
  vector<Candidate*> loserstest = test.getLosers();
  EXPECT_EQ(loserstest.size(), 4);
  EXPECT_EQ(loserstest[3].getName(), "Rosen");

  EXPECT_EQ(loserstest[2].getName(), "Himmelberg");

  EXPECT_EQ(loserstest[1].getName(), "Chou");

  EXPECT_EQ(loserstest[0].getName(), "Kleinberg");


}

TEST_F(MultiplefilesTest, checkonefile-OPL){
  std::vector<Candidate> vectortest;
  Candidate Pike("Pike", "Democrat");
  Candidate Rosen("Rosen", "Democrat");
  Candidate Susan("Susan", "Democrat");
  Candidate Kleinberg("Kleinberg", "Republican");
  Candidate Himmelberg("Himmelberg", "Republican");
  Candidate Chou("Chou", "Independent");
  vectortest.push_back(Pike);
  vectortest.push_back(Rosen);
  vectortest.push_back(Susan);
  vectortest.push_back(Kleinberg);
  vectortest.push_back(Himmelberg);
  vectortest.push_back(Chou);

  OPL test("sampleelection2.csv", 6, 3, 9, vectortest);
  test.runElection();
  test.displayResults();

  //check the winners queue
  vector<Candidate*> winnerstest = test.getWinners();
  EXPECT_EQ(winnerstest.size(), 3);
  EXPECT_EQ(winnerstest[0].getName(), "Pike");

  EXPECT_EQ(winnerstest[1].getName(), "Kleinberg");
  EXPECT_EQ(winnerstest[2].getName(), "Rosen");

}

TEST_F(MultiplefilesTest, checktwofiles-OPL){
  std::vector<Candidate> vectortest;
  Candidate Pike("Pike", "Democrat");
  Candidate Rosen("Rosen", "Democrat");
  Candidate Susan("Susan", "Democrat");
  Candidate Kleinberg("Kleinberg", "Republican");
  Candidate Himmelberg("Himmelberg", "Republican");
  Candidate Chou("Chou", "Independent");
  vectortest.push_back(Pike);
  vectortest.push_back(Rosen);
  vectortest.push_back(Susan);
  vectortest.push_back(Kleinberg);
  vectortest.push_back(Himmelberg);
  vectortest.push_back(Chou);

  OPL test("sampleelection2.csv", "samplemultiple-2.csv", 6, 3, 9, vectortest);
  test.runElection();
  test.displayResults();

  //check the winners queue
  vector<Candidate*> winnerstest = test.getWinners();
  EXPECT_EQ(winnerstest.size(), 3);
  EXPECT_EQ(winnerstest[0].getName(), "Pike");

  EXPECT_EQ(winnerstest[1].getName(), "Kleinberg");
  EXPECT_EQ(winnerstest[2].getName(), "Rosen");

}




int main(int argc, char **argv) {

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
