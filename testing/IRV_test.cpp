#include <gtest/gtest.h>
#include "IRV.h"
#include "Candidate.h"
#include "Election.h"
#include "Party.h"
#include <stdexcept>


class IRVTest : public ::testing::Test {
 public:
  void SetUp( ) {
    // code here will execute just before the test ensues
    bool SHUFFLE = true;
  }

};

TEST_F(IRVTest, checkinitialization){
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

  IRV test("sampleelection1");
  test.runElection();
//check the winners
  vector<Candidate*> winnerstest = test.getWinners();
  EXPECT_EQ(winnerstest.size(), 1);
  EXPECT_EQ(winnerstest[0]->getName(), "Susan");
//check the losers
  vector<Candidate*> loserstest = test.getLosers();
  EXPECT_EQ(loserstest.size(), 4);
  EXPECT_EQ(loserstest[3]->getName(), "Rosen");

  EXPECT_EQ(loserstest[2]->getName(), "Himmelberg");

  EXPECT_EQ(loserstest[1]->getName(), "Chou");

  EXPECT_EQ(loserstest[0]->getName(), "Kleinberg");


}

int main(int argc, char **argv) {

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
