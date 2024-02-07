/*****************************************************************************/
/*                                                                           */
/* Assignment: CS 460 Project 2                                              */
/* Author:                                                                   */
/* Date: November 2022                                                       */
/* Modified by:                                                              */
/* Date:                                                                     */
/* Description: This file contains the description of a class called         */
/*      Syntactical Analyzer.                                                */
/*                                                                           */
/*****************************************************************************/

#ifndef SYNTACTICALANLYZER_H
#define SYNTACTICALANLYZER_H

#include <iostream>
#include <fstream>
#include "LexicalAnalyzer.h"

using namespace std;

/*****************************************************************************/
/*                                                                           */
/* Class name: Syntactical Analyzer                                          */
/* Description: This class is designed to define methods and attributes      */
/*      needed to perform Syntactical Analysis on a Scheme input file.       */
/*                                                                           */
/*****************************************************************************/

class SyntacticalAnalyzer 
{
    public:
	SyntacticalAnalyzer (char * filename);
	~SyntacticalAnalyzer ();


    private:
	LexicalAnalyzer * lex;
	token_type token;
	ofstream p2File;

	void program ();
	void more_defines();
	void define();
	void stmt();
	void stmt_list();
	void literal();
	void quoted_lit();
	void logical_lit();
	void more_tokens();
	void param_list();
	void else_part();
	void stmt_pair();
	void stmt_pair_body();
	void assign_pair();
	void more_assigns();
	void action();
	void any_other_token();
	
};

#endif
