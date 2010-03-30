#include "unittest++/UnitTest++.h"
#include "score_keeper.h"

namespace {
  TEST(score_should_start_at_zero) {
    ScoreKeeper default_score_keeper;
    CHECK(default_score_keeper.get_score() == 0);
  }

  TEST(increment_the_score_by_one) {
    ScoreKeeper score_keeper;
    score_keeper.add_point();
    CHECK(score_keeper.get_score() == 1);
  }

  TEST(indicate_when_player_has_enough_points_to_win) {
    ScoreKeeper score_keeper;
    
    // this should add enough points to win minus one. we can
    // then check to verify the player wins at exactly the right
    // amoutn of points
    for (int i = 0; i < ScoreKeeper::PointsToWin - 1; ++i) {
      score_keeper.add_point();
    }

    CHECK(score_keeper.has_enough_points_to_win() == false);
    score_keeper.add_point();
    CHECK(score_keeper.has_enough_points_to_win() == true);
  }

  TEST(reset_the_scores_back_to_zero) {
    ScoreKeeper score_keeper;

    score_keeper.add_point();
    score_keeper.add_point();
    score_keeper.add_point();
    
    score_keeper.reset_points();
    CHECK(score_keeper.get_score() == 0);
  }
}

