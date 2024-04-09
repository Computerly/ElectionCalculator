#include <gtest/gtest.h>
#include "Candidate.h"
#include "Party.h"
#include "Election.h"
#include "IRV.h"
#include "OPL.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;


class InvalidateballotsTest : public ::testing::Test {
 public:
  void SetUp( ) {

  }

};


TEST_F(InvalidateballotsTest, checkforIRV){
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
  std::vector<string> invalidballots = test.getinvalidballots();
  EXPECT_EQ(invalidballots.size(), 2);
  EXPECT_EQ(invalidballots[0], ",1,,2,");
  EXPECT_EQ(invalidballots[1], "1,2,,,");

}

TEST_F(InvalidateballotsTest, checkforIRVtwo){
  std::vector<Candidate> vectortest;

  Candidate Rosen("Rosen", "Democrat");
  Candidate Susan("Susan", "Democrat");
  Candidate Kleinberg("Kleinberg", "Republican");
  Candidate Himmelberg("Himmelberg", "Republican");
  Candidate Chou("Chou", "Independent");
  Candidate Peter("Peter", "Independent");
  vectortest.push_back(Rosen);
  vectortest.push_back(Susan);
  vectortest.push_back(Kleinberg);
  vectortest.push_back(Himmelberg);
  vectortest.push_back(Chou);
  vectortest.push_back(Peter);

  IRV test("sampleelection3.csv", 6, 1, 10, vectortest);
  test.runElection();
  std::vector<string> invalidballots = test.getinvalidballots();
  EXPECT_EQ(invalidballots.size(), 4);
  EXPECT_EQ(invalidballots[0], ",1,,2,,");
  EXPECT_EQ(invalidaballots[1], "1,,3,,,");
  EXPECT_EQ(invalidaballots[2], "1,2,,,,");
  EXPECT_EQ(invalidaballots[3], "1,,3,,,");


}




int main(int argc, char **argv) {

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
