#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

/*******************************************************************************
* Assignment: Project 1 - Lexical Analyzer for Scheme to C++ Translator        *
* Author: Dr. Watts                                                            *
* Date: Fall 2022                                                              *
* File: LexicalAnalyzer.h                                                      *
*                                                                              *
* Description: This file contains the description of the LexicalAnaylzer      *
*******************************************************************************/

#include <iostream>
#include <fstream>
#include <map>

using namespace std;

/*******************************************************************************
* Type: token_type                                                             *
*                                                                              *
* Description: The token_type enumerated type is used to identify the tokens   *
*              associated with the lexemes scanned from an input file.         *
*******************************************************************************/

enum token_type
{
	NONE, IDKEY_T, NUMLIT_T, LISTOP1_T, PLUS_T, MINUS_T, GT_T, LT_T,
	ERROR_BU, TRUE_T, FALSE_T, DIV_T, MULT_T, EQUALTO_T, GTE_T, LTE_T,
	LPAREN_T, RPAREN_T, SQUOTE_T, ERROR_NBU, IDENT_T, IF_T, COND_T,
	DISPLAY_T, NEWLINE_T, AND_T, OR_T, NOT_T, DEFINE_T, LET_T, LISTOP2_T,
	NUMBERP_T, LISTP_T, ZEROP_T, NULLP_T, STRINGP_T, MODULO_T, ROUND_T,
	ELSE_T, STRLIT_T, ERROR_T, EOF_T, MAX_TOKENS
};

/*******************************************************************************
* Class: LexicalAnalyzer                                                      *
*                                                                              *
* Description: This class is designed to                                       *
*******************************************************************************/

class LexicalAnalyzer 
{
    public:
	LexicalAnalyzer (char * filename);
	~LexicalAnalyzer ();
	token_type GetToken ();
	string GetTokenName (token_type t) const;
	string GetLexeme () const;
	void ReportError (const string & msg);
	ofstream debugFile;	// .dbg
	
    private:
	ifstream inputFile; 	// .ss 
	ofstream listingFile;	// .lst
	ofstream tokenFile;	// .p1
	string line;
	int linenum;
	int pos;
	string lexeme;
	int errors;
	map <string, token_type> keywords;
	bool GetALine ();
};
	
#endif
