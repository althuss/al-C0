
/*
  This file is part of Leela Chess Zero.
  Copyright (C) 2018-2019 The LCZero Authors

  Leela Chess is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Leela Chess is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Leela Chess.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <gtest/gtest.h>

#include <iostream>
using namespace std;
#include "src/chess/bitboard.h"
#include "src/chess/board.h"

namespace lczero {


TEST(ChessBoard, PseudolegalMovesStartingPos) {
  ChessBoard board;
  board.SetFromFen(ChessBoard::kStartposFen);
  board.Mirror();
  auto moves = board.GeneratePseudolegalMoves();

//    cout << "test test test"; 
//    for(auto x: moves){
//	cout<<x<<endl;
//    }

  EXPECT_EQ(moves.size(), 20);
 cout << board.DebugString(); 

  for (const auto& move : moves) {
    auto new_board = board;
    new_board.ApplyMove(move);
//    if (new_board.IsUnderCheck()) continue;
 //   new_board.Mirror();


      Move m = move;
      std::cerr << m.as_string() << ' ' << '\n'
                << new_board.DebugString();
  }




}




namespace {
int Perft(const ChessBoard& board, int max_depth, bool dump = false,
          int depth = 0) {
  if (depth == max_depth) return 1;
  int total_count = 0;
  auto moves = board.GeneratePseudolegalMoves();

  auto legal_moves = board.GenerateLegalMoves();
  auto iter = legal_moves.begin();

  for (const auto& move : moves) {
    auto new_board = board;
    new_board.ApplyMove(move);
    if (new_board.IsUnderCheck()) continue;
    new_board.Mirror();

    EXPECT_EQ(iter->as_packed_int(), move.as_packed_int())
        << board.DebugString() << iter->as_string() << move.as_string();
    ++iter;

    int count = Perft(new_board, max_depth, dump, depth + 1);
    if (dump && depth == 0) {
      Move m = move;
      std::cerr << m.as_string() << ' ' << count << '\n'
                << new_board.DebugString();
    }
    total_count += count;
  }

  EXPECT_EQ(iter, legal_moves.end());
  return total_count;
}
}  // namespace


}  // namespace lczero

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  lczero::InitializeMagicBitboards();
  return RUN_ALL_TESTS();
}
