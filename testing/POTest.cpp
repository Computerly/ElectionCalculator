#include <gtest/gtest.h>
#include "IRV.h"
#include "Candidate.h"
#include "Election.h"
#include "Party.h"
#include "PO.h"
#include <stdexcept>

class POTest : public ::testing::Test {
 public:
  void SetUp( ) {
    // code here will execute just before the test ensues

  }

};


TEST_F(POTest, checkresults){
  PO test("samplelectionpo.csv");
  test.runElection();
  vector<Candidate*> winnerstest = test.getWinners();
  EXPECT_EQ(winnerstest.size(), 1);
  EXPECT_EQ(winnerstest[0]->getName(), "Pike");




}


int main(int argc, char **argv) {

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
