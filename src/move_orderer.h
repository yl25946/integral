#ifndef INTEGRAL_MOVE_ORDERER_H_
#define INTEGRAL_MOVE_ORDERER_H_

#include "move_gen.h"
#include "search.h"

class MoveOrderer {
 public:
  explicit MoveOrderer(Board &board, List<Move> moves, MoveType move_type, const int &ply) noexcept;

  const Move &get_move(int start) noexcept;

  const int &get_move_score(int start) noexcept;

  static const int &get_history_score(const Move &move, Color turn) noexcept;

  [[nodiscard]] std::size_t size() const;

  static void update_killer_move(const Move &move, int ply);

  static void update_counter_move(const Move &prev_move, const Move &counter);

  static void update_move_history(const Move &move, List<Move>& quiet_non_cutoffs, Color turn, int depth);

  static void penalize_move_history(List<Move>& moves, Color turn, int depth);

  static void clear_move_history();

  static void clear_killers(int ply);

 private:
  void score_moves() noexcept;

  int calculate_move_score(const Move &move, const Move &tt_move) const;

 private:
  Board &board_;
  List<Move> moves_;
  MoveType move_type_;
  std::array<int, 256> move_scores_;
  int ply_;

 public:
  static constexpr int kNumKillerMoves = 2;
  static std::array<std::array<Move, kNumKillerMoves>, kMaxPlyFromRoot> killer_moves;
  static std::array<std::array<Move, Square::kSquareCount>, Square::kSquareCount> counter_moves;
  static std::array<std::array<std::array<int, Square::kSquareCount>, Square::kSquareCount>, 2> move_history;
};

#endif // INTEGRAL_MOVE_ORDERER_H_