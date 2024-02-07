/*******************************************************************************
* Assignment: Project 1 - Lexical Analyzer for Scheme to C++ Translator        *
* Author: Dr. Watts                                                            *
* Date: Fall 2022                                                              *
* File: LexicalAnalyzer.cpp                                                    *
*                                                                              *
* Description: This file contains the implementations  of the methods of the   *
*	       Lexical_analyzer class.                                         *
*******************************************************************************/

#include "LexicalAnalyzer.h"

/*******************************************************************************
* Variable: token_names                                                        *
*                                                                              *
* Description: The token_names variable is an array of string containing the   *
*              names of the tokens. It should be indexed using an element      *
*              of the token_type enumerate type.                               *
*              This array variable is used by the GetTokenName function.       *
*******************************************************************************/

string token_names[] =
{
	"NONE", "IDKEY_T", "NUMLIT_T", "LISTOP1_T", "PLUS_T", "MINUS_T",
	"GT_T", "LT_T", "ERROR_BU", "TRUE_T", "FALSE_T", "DIV_T", "MULT_T",
	"EQUALTO_T", "GTE_T", "LTE_T", "LPAREN_T", "RPAREN_T", "SQUOTE_T",
	"ERROR_NBU", "IDENT_T", "IF_T", "COND_T", "DISPLAY_T", "NEWLINE_T",
	"AND_T", "OR_T", "NOT_T", "DEFINE_T", "LET_T", "LISTOP2_T", "NUMBERP_T",
	"LISTP_T", "ZEROP_T", "NULLP_T", "STRINGP_T", "MODULO_T", "ROUND_T",
	"ELSE_T", "STRLIT_T", "ERROR_T", "EOF_T", "MAX_TOKENS"
};

/*******************************************************************************
* Variable: char2col                                                           *
*                                                                              *
* Description: The char2col array is used to determine the appropriate column  *
*              in the state_table for each ASCII character in the input file.  *
*******************************************************************************/

int char2col [128] = 
{
//	  ,   ,   ,   ,   ,   ,   ,   ,   , sp, sp, sp, sp, sp,   ,   ,
	24, 24, 24, 24, 24, 24, 24, 24, 24,  0,  0,  0,  0,  0, 24, 24,
//	  ,   ,   ,   ,   ,   ,   ,   ,   ,   ,   ,   ,   ,   ,   ,   ,
	24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
//	sp,  !,  ",  #,  $,  %,  &,  ',  (,  ),  *,  +,  ,,  -,  .,  /,
	 0, 24,  1,  2, 24, 24, 24,  3,  4,  5,  6,  7, 24,  8,  9, 10,
//	 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  :,  ;,  <,  =,  >, ? ,
	23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 24, 24, 11, 12, 13, 14,
//	 @,  A,  B,  C,  D,  E,  F,  G,  H,  I,  J,  K,  L,  M,  N,  O,
	24, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
//	 P,  Q,  R,  S,  T,  U,  V,  W,  X,  Y,  Z,  [,  \,  ],  ^, _ ,
	22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 24, 24, 24, 24, 15,
//	 ',  a,  b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,
	24, 16, 22, 17, 18, 22, 19, 22, 22, 22, 22, 22, 22, 22, 22, 22,
//	 p,  q,  r,  s,  t,  u,  v,  w,  x,  y,  z,  {,  |,  },  ~,   ,
	22, 22, 20, 22, 21, 22, 22, 22, 22, 22, 22, 24, 24, 24, 24, 24
};

/*******************************************************************************
* Variable: state_table                                                        *
*                                                                              *
* Description: The state_table variable contains the tabular representation of *
*              the DFA used to identify the lexemes in a file and associate    *
*              them with the approprate element in the token_type enymerated   *
*              type. This 2D-array variable is used by the GetToken function.  *
*******************************************************************************/

//column     0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,  22,  23, 24
//character sp, ", #, ', (, ), *, +, -, .,  /,  <,  =,  >,  ?,  _,  a,  c,  d,  f,  r,  t, alf, num, other
int state_table [18][25] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
// State 1
	{0, 17, 2, -SQUOTE_T, -LPAREN_T, -RPAREN_T, -MULT_T, 3, 4, 6, -DIV_T, 10,
		-EQUALTO_T, 11, -ERROR_NBU, -ERROR_NBU, 12, 14, 12, 12, 12, 12, 12,
		5, -ERROR_NBU},
// State 2
	{-ERROR_BU, -ERROR_BU, -ERROR_BU, -ERROR_BU, -ERROR_BU, -ERROR_BU, -ERROR_BU,
		-ERROR_BU, -ERROR_BU, -ERROR_BU, -ERROR_BU, -ERROR_BU, -ERROR_BU,
		-ERROR_BU, -ERROR_BU, -ERROR_BU, -ERROR_BU, -ERROR_BU, -ERROR_BU,
		-FALSE_T, -ERROR_BU, -TRUE_T, -ERROR_BU, -ERROR_BU, -ERROR_BU},
// State 3
	{-PLUS_T, -PLUS_T, -PLUS_T, -PLUS_T, -PLUS_T, -PLUS_T, -PLUS_T, -PLUS_T,
		-PLUS_T, 6, -PLUS_T, -PLUS_T, -PLUS_T, -PLUS_T, -PLUS_T, -PLUS_T,
		-PLUS_T, -PLUS_T, -PLUS_T, -PLUS_T, -PLUS_T, -PLUS_T, -PLUS_T, 5, 
		PLUS_T},
// State 4
	{-MINUS_T, -MINUS_T, -MINUS_T, -MINUS_T, -MINUS_T, -MINUS_T, -MINUS_T,
		-MINUS_T, -MINUS_T, 6, -MINUS_T, -MINUS_T, -MINUS_T, -MINUS_T,
		-MINUS_T, -MINUS_T, -MINUS_T, -MINUS_T, -MINUS_T, -MINUS_T,
		-MINUS_T, -MINUS_T, -MINUS_T, 5, -MINUS_T},
// State 5
	{-NUMLIT_T, -NUMLIT_T, -NUMLIT_T, -NUMLIT_T, -NUMLIT_T, -NUMLIT_T, -NUMLIT_T,
		-NUMLIT_T, -NUMLIT_T, 8, 7, -NUMLIT_T, -NUMLIT_T, -NUMLIT_T, -NUMLIT_T,
		-NUMLIT_T, -NUMLIT_T, -NUMLIT_T, -NUMLIT_T, -NUMLIT_T, -NUMLIT_T,
		-NUMLIT_T, -NUMLIT_T, 5, -NUMLIT_T},
// State 6
	{-ERROR_BU, -ERROR_BU, -ERROR_BU, -ERROR_BU, -ERROR_BU, -ERROR_BU, -ERROR_BU,
		-ERROR_BU, -ERROR_BU, -ERROR_BU, -ERROR_BU, -ERROR_BU, -ERROR_BU,
		-ERROR_BU, -ERROR_BU, -ERROR_BU, -ERROR_BU, -ERROR_BU, -ERROR_BU,
		-ERROR_BU, -ERROR_BU, -ERROR_BU, -ERROR_BU, 9, -ERROR_BU},
// State 7
	{-ERROR_BU, -ERROR_BU, -ERROR_BU, -ERROR_BU, -ERROR_BU, -ERROR_BU, -ERROR_BU,
		-ERROR_BU, -ERROR_BU, -ERROR_BU, -ERROR_BU, -ERROR_BU, -ERROR_BU,
		-ERROR_BU, -ERROR_BU, -ERROR_BU, -ERROR_BU, -ERROR_BU, -ERROR_BU,
		-ERROR_BU, -ERROR_BU, -ERROR_BU, -ERROR_BU, 9, -ERROR_BU},
// State 8
	{-NUMLIT_T, -NUMLIT_T, -NUMLIT_T, -NUMLIT_T, -NUMLIT_T, -NUMLIT_T, -NUMLIT_T,
		-NUMLIT_T, -NUMLIT_T, -NUMLIT_T, -NUMLIT_T, -NUMLIT_T, -NUMLIT_T,
		-NUMLIT_T, -NUMLIT_T, -NUMLIT_T, -NUMLIT_T, -NUMLIT_T, -NUMLIT_T,
		-NUMLIT_T, -NUMLIT_T, -NUMLIT_T, -NUMLIT_T, 9, -NUMLIT_T},
// State 9
	{-NUMLIT_T, -NUMLIT_T, -NUMLIT_T, -NUMLIT_T, -NUMLIT_T, -NUMLIT_T, -NUMLIT_T,
		-NUMLIT_T, -NUMLIT_T, -NUMLIT_T, -NUMLIT_T, -NUMLIT_T, -NUMLIT_T,
		-NUMLIT_T, -NUMLIT_T, -NUMLIT_T, -NUMLIT_T, -NUMLIT_T, -NUMLIT_T,
		-NUMLIT_T, -NUMLIT_T, -NUMLIT_T, -NUMLIT_T, 9, -NUMLIT_T},
// State 10
	{-LT_T, -LT_T, -LT_T, -LT_T, -LT_T, -LT_T, -LT_T, -LT_T, -LT_T, -LT_T,
		-LT_T, -LT_T, -LTE_T, -LT_T, -LT_T, -LT_T, -LT_T, -LT_T, -LT_T,
		-LT_T, -LT_T, -LT_T, -LT_T, -LT_T, -LT_T},
// State 11
	{-GT_T, -GT_T, -GT_T, -GT_T, -GT_T, -GT_T, -GT_T, -GT_T, -GT_T, -GT_T,
		-GT_T, -GT_T, -GTE_T, -GT_T, -GT_T, -GT_T, -GT_T, -GT_T, -GT_T,
		-GT_T, -GT_T, -GT_T, -GT_T, -GT_T, -GT_T},
// State 12
	{-IDKEY_T, -IDKEY_T, -IDKEY_T, -IDKEY_T, -IDKEY_T, -IDKEY_T, -IDKEY_T,
		-IDKEY_T, -IDKEY_T, -IDKEY_T, -IDKEY_T, -IDKEY_T, -IDKEY_T,
		-IDKEY_T, 13, 12, 12, 12, 12, 12, 12, 12, 12, 12, -IDKEY_T},
// State 13
	{-IDKEY_T, -IDKEY_T, -IDKEY_T, -IDKEY_T, -IDKEY_T, -IDKEY_T, -IDKEY_T,
		-IDKEY_T, -IDKEY_T, -IDKEY_T, -IDKEY_T, -IDKEY_T, -IDKEY_T,
		-IDKEY_T, -IDKEY_T, -IDKEY_T, -IDKEY_T, -IDKEY_T, -IDKEY_T,
		-IDKEY_T, -IDKEY_T, -IDKEY_T, -IDKEY_T, -IDKEY_T, -IDKEY_T},
// State 14
	{-IDKEY_T, -IDKEY_T, -IDKEY_T, -IDKEY_T, -IDKEY_T, -IDKEY_T, -IDKEY_T,
		-IDKEY_T, -IDKEY_T, -IDKEY_T, -IDKEY_T, -IDKEY_T, -IDKEY_T,
		-IDKEY_T, 13, 12, 15, 12, 15, 12, 12, 12, 12, 12, -IDKEY_T},
// State 15
	{-IDKEY_T, -IDKEY_T, -IDKEY_T, -IDKEY_T, -IDKEY_T, -IDKEY_T, -IDKEY_T,
		-IDKEY_T, -IDKEY_T, -IDKEY_T, -IDKEY_T, -IDKEY_T, -IDKEY_T,
		-IDKEY_T, 13, 12, 15, 12, 15, 12, 16, 12, 12, 12, -IDKEY_T},
// State 16
	{-LISTOP1_T, -LISTOP1_T, -LISTOP1_T, -LISTOP1_T, -LISTOP1_T, -LISTOP1_T,
		-LISTOP1_T, -LISTOP1_T, -LISTOP1_T, -LISTOP1_T, -LISTOP1_T,
		-LISTOP1_T, -LISTOP1_T, -LISTOP1_T, 13, 12, 12, 12, 12, 12,
		12, 12, 12, 12, -LISTOP1_T},
// State 17
	{17, -STRLIT_T, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
		17, 17, 17, 17, 17, 17, 17, 17, 17, 17}};
