#ifndef TOKEN_GUARD
#define TOKEN_GUARD
#include <string>
#include <bitset>

/*These tokens are used during operator precedence parsing as indexes to an array. Only tokens that
are actual terminals are needed to index the array. Use TOKEN_COUNT as the array size.
The array is organized as follows. Each pair of tokens gets 2bits in an unsigned char * array
of size TOKEN_COUNT*CHUNK_SIZE where Chunk_Size is 1 + (TOKEN_COUNT - 1)/4 Assuming chars are 8 bits.
We index a chunk using the first TOKEN_COUNT and the the 2bit are of the CHUNK is indexed by the second token count.
Thus we have (array[TK1 * CHUNK_SIZE + TK2/4] & (0xC0u >> 2*(TK2%4))) >> 2*(TK2%4)  which gives the value of the token. If this value
is 0 we say TK1 < TK2. If 1, TK1 = TK2, and 2, TK1 > TK2*/
enum class token {
  ID = 0,
  NUMBER,
  LEFT_PAREN,
  RIGHT_PAREN,
  EXCLAMATION,
  CARET,
  UNARY_MINUS,
  ASTERISK,
  FORWARD_SLASH,
  FUNCTION,
  PLUS,
  MINUS,
  /*Used to indicate start and end of input*/
  ENDPOINT,

  /*Symbols for precedence parsing but are not used to index the array*/
  TOKEN_COUNT,
  NONTERMINAL,
  ERROR
};

enum class precedence {
  LESS_THAN = 0,
  EQUAL_TO = 1,
  GREATER_THAN = 2
};

bool is_operator(token tok);
std::string token_to_string(token tok);
std::ostream& operator<<(std::ostream& out, token tok);

#endif
