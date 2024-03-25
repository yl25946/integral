#include "board.h"

Move::Move(U8 from, U8 to) : data_(0) {
  set_from(from);
  set_to(to);
}

Move::Move(U8 from, U8 to, PromotionType promotion_type) : Move(from, to) {
  set_promotion_type(promotion_type);
}

Move Move::null_move() {
  return Move(0, 0);
}

bool Move::operator==(const Move& other) const {
  return data_ == other.data_;
}

std::optional<Move> Move::from_str(const BoardState &state, std::string_view str) {
  const int kMinMoveLen = 4, kMaxMoveLen = 5;
  if (str.length() < kMinMoveLen || str.length() > kMaxMoveLen)
    return std::nullopt;

  const int from_rank = str[1] - '1', from_file = str[0] - 'a';
  const int to_rank = str[3] - '1', to_file = str[2] - 'a';

  if (from_rank < 0 || from_rank >= 8 || to_rank < 0 || to_rank >= 8 ||
      from_file < 0 || from_file >= 8 || to_file < 0 || to_file >= 8)
    return std::nullopt;

  const auto from = rank_file_to_pos(from_rank, from_file);
  const auto to = rank_file_to_pos(to_rank, to_file);

  if (str.length() < kMaxMoveLen)
    return Move(from, to);

  PromotionType promotion_type;
  switch (str[4]) {
    case 'q':
    case 'Q':
      promotion_type = PromotionType::kQueen;
      break;
    case 'r':
    case 'R':
      promotion_type = PromotionType::kRook;
      break;
    case 'b':
    case 'B':
      promotion_type = PromotionType::kBishop;
      break;
    case 'n':
    case 'N':
      promotion_type = PromotionType::kKnight;
      break;
    default:
      return std::nullopt;
  }

  return Move(from, to, promotion_type);
}

bool Move::is_capture(const BoardState &state) const {
  const auto from = get_from();
  const auto to = get_to();
  return (state.get_piece_type(to) != PieceType::kNone) ||
      (state.get_piece_type(from) == PieceType::kPawn && state.en_passant.has_value() && (state.en_passant == to));
}

std::string Move::to_string() const {
  if (*this == null_move())
    return "null";

  const auto from_rank = get_from() / kBoardRanks, from_file = get_from() % kBoardFiles;
  const auto to_rank = get_to() / kBoardRanks, to_file = get_to() % kBoardFiles;

  std::string res = std::string(1, 'a' + from_file) + std::to_string(from_rank + 1) +
                    std::string(1, 'a' + to_file) + std::to_string(to_rank + 1);

  const auto promo_type = get_promotion_type();
  switch (promo_type) {
    case PromotionType::kAny:
    case PromotionType::kQueen:
      res += 'q';
      break;
    case PromotionType::kKnight:
      res += 'n';
      break;
    case PromotionType::kBishop:
      res += 'b';
      break;
    case PromotionType::kRook:
      res += 'r';
      break;
    default:
      break;
  }

  return res;
}