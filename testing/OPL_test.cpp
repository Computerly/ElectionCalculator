#include <gtest/gtest.h>
#include "OPL.h"
#include "Candidate.h"
#include "Election.h"
#include "Party.h"
#include <stdexcept>


class OPLTest : public ::testing::Test {
 public:
  void SetUp( ) {
    // code here will execute just before the test ensues

  }

};

TEST_F(OPLTest, checkalogrithm){
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

  OPL test("sampleelection2.csv");
  test.runElection();
  test.displayResults();

  //check the winners queue
  vector<Candidate*> winnerstest = test.getWinners();
  EXPECT_EQ(winnerstest.size(), 3);
  EXPECT_EQ(winnerstest[0]->getName(), "Pike");

  EXPECT_EQ(winnerstest[1]->getName(), "Kleinberg");
  EXPECT_EQ(winnerstest[2]->getName(), "Rosen");

}
//checks for the case where a coin flip is necessary for the tie case

TEST_F(OPLTest, checktiecase){
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

  OPL test("sampleelection-tie.csv");
  test.runElection();
  test.displayResults();

  vector<Candidate*> winnerstest = test.getWinners();
  EXPECT_EQ(winnerstest.size(), 3);
  EXPECT_EQ(winnerstest[0]->getName(), "Pike");

  EXPECT_EQ(winnerstest[1]->getName(), "Himmelberg");
  bool flag = false;
  if ((winnerstest[2]->getName() == "Rosen") || (winnerstest[2]->getName()=="Susan")) {
    flag = true;
  }
  EXPECT_EQ(flag, true);



}

int main(int argc, char **argv) {

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
