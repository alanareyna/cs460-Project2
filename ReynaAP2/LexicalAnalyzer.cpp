/*******************************************************************************
* Assignment: Project 1 - Lexical Analyzer for Scheme to C++ Translator        *
* Author: Dr. Watts                                                            *
* Date: Fall 2022                                                              *
* File: LexicalAnalyzer.cpp                                                    *
*                                                                              *
* Description: This file contains the implementations  of the methods of the   *
*	       Lexical_analyzer class.                                         *
*******************************************************************************/

#include <iomanip>
#include <cstdlib>
#include "LexicalAnalyzer.h"

using namespace std;

/*******************************************************************************
* Variable: token_names                                                        *
*                                                                              *
* Description: The token_names variable is an array of string containing the   *
*              names of the tokens. It should be indexed using an element      *
*              of the token_type enumerate type.                               *
*              This array variable is used by the GetTokenName function.       *
*******************************************************************************/

extern string token_names[];

/*******************************************************************************
* Variable: char2col                                                           *
*                                                                              *
* Description: The char2col array is used to determine the appropriate column  *
*              in the state_table for each ASCII character in the input file.  *
*******************************************************************************/

extern int char2col [128];

/*******************************************************************************
* Variable: table                                                              *
*                                                                              *
* Description: The state_table variable contains the tabular representation of *
*              the DFA used to identify the lexemes in a file and associate    *
*              them with the approprate element in the token_type enymerated   *
*              type. This 2D-array variable is used by the GetToken function.  *
*******************************************************************************/

extern int state_table [18][25];

/*******************************************************************************
* Function: LexicalAnalyzer                                                    *
*                                                                              *
* Parameters: none                                                             *
* Return value: none                                                           *
* Description: This constructor will initialize the lexical analyzer class     *
*              object.                                                         *
*******************************************************************************/

LexicalAnalyzer::LexicalAnalyzer (char * filename)
{
	// Verify that the input file is of the appropriate type
	cout << "Input file: " << filename << endl;
	string name = filename;
	string extension;
	if (name.length() > 3)
		extension = name.substr (name.length()-3, 3);
	if (extension != ".ss")
	{
		cout << "Invalid file extension; must be '.ss'\n";
		exit (1);
	}

	// Open files used by the lexical analyzer
	inputFile.open (filename);
	if (inputFile.fail())
	{
		cout << "File " << filename << " not found\n";
		exit (2);
	}
	string p1name = name.substr (0, name.length()-3) + ".p1"; 
	tokenFile.open (p1name.c_str());
	string listingname = name.substr (0, name.length()-3) + ".lst"; 
	listingFile.open (listingname.c_str());
	listingFile << "Input file: " << filename << endl;
	string debugFilename = name.substr (0, name.length()-3) + ".dbg"; 
	debugFile.open (debugFilename.c_str());
	debugFile << "Input file: " << filename << endl;
	line = " ";
	linenum = 0;
	pos = 0;
	lexeme = "";
	errors = 0;

	// Initialize the keywords map
	keywords.insert (pair<string,token_type> ("cons", LISTOP2_T));
	keywords.insert (pair<string,token_type> ("list", LISTOP1_T));
	keywords.insert (pair<string,token_type> ("append", LISTOP2_T));
	keywords.insert (pair<string,token_type> ("if", IF_T));
	keywords.insert (pair<string,token_type> ("cond", COND_T));
	keywords.insert (pair<string,token_type> ("display", DISPLAY_T));
	keywords.insert (pair<string,token_type> ("newline", NEWLINE_T));
	keywords.insert (pair<string,token_type> ("and", AND_T));
	keywords.insert (pair<string,token_type> ("or", OR_T));
	keywords.insert (pair<string,token_type> ("not", NOT_T));
	keywords.insert (pair<string,token_type> ("define", DEFINE_T));
	keywords.insert (pair<string,token_type> ("let", LET_T));
	keywords.insert (pair<string,token_type> ("number?", NUMBERP_T));
	keywords.insert (pair<string,token_type> ("list?", LISTP_T));
	keywords.insert (pair<string,token_type> ("zero?", ZEROP_T));
	keywords.insert (pair<string,token_type> ("null?", NULLP_T));
	keywords.insert (pair<string,token_type> ("string?", STRINGP_T));
	keywords.insert (pair<string,token_type> ("modulo", MODULO_T));
	keywords.insert (pair<string,token_type> ("round", ROUND_T));
	keywords.insert (pair<string,token_type> ("else", ELSE_T));
}

/*******************************************************************************
* Function: ~LexicalAnalyzer                                                   *
*                                                                              *
* Parameters: none                                                             *
* Return value: none                                                           *
* Description: This function will complete the execution of the lexical        *
*              analyzer class.                                                 *
*******************************************************************************/

LexicalAnalyzer::~LexicalAnalyzer ()
{
	inputFile.close ();
	cout << errors << " errors found in input file\n";
	listingFile << errors << " errors found in input file\n";
	listingFile.close();
	debugFile << errors << " errors found in input file\n";
	debugFile.close();
}

/*******************************************************************************
* Function: GetToken                                                           *
*                                                                              *
* Parameters: none                                                             *
* Return value: a member of the token_type enumerated type                     *
* Description: This function will find the next lexeme in the input file and   *
*              return the token_type value associated with that lexeme.        *
*******************************************************************************/

token_type LexicalAnalyzer::GetToken ()
{
	lexeme = "";
	int state = 1;
	token_type token = NONE;
	while (isspace(line[pos]) || line[pos] == ';')
	{ // end of line or begining of comment
		pos++;
		if (pos >= line.length() || line[pos-1] == ';') 
			if (!GetALine ())
				return EOF_T;
	}
	while (state > 0)
	{ // traverse the state_table until a terminal (< 0) state is reached
		char c = line[pos++];
		lexeme += c;
		int col = char2col [c];
		state = state_table[state][col];
		if (state == 17 && pos >= line.length ()) 
			if (GetALine ())
				lexeme += '\n';
			else
			{
				state = -ERROR_NBU;
				lexeme = "end of file";
				line = " ";
				pos = 0;
			}
	}
	// set the token type
	token = token_type (-state);
	if (token <= ERROR_BU)
	{
		pos--;
		lexeme.pop_back();
	}
	if (token == IDKEY_T)
	{ // determine if the current lexeme is a keyword or an identifier
		map <string, token_type>::iterator itr = keywords.find (lexeme);
		if (itr != keywords.end())
			token = itr->second;
		else 
		{
			token = IDENT_T;
			if (lexeme [lexeme.length()-1] == '?')
			{
				pos--;
				lexeme.pop_back();
			}
		}
	}
	else if (token == ERROR_BU || token == ERROR_NBU)
	{ // handle errors
		ReportError (string("Unexpected '") + lexeme + "' found");
		token = ERROR_T;
	}
	// write to appropriate files
	debugFile << '\t' << setw(16) << left << token_names[token] << lexeme << endl;
	tokenFile << '\t' << setw(16) << left << token_names[token] << lexeme << endl;
	return token;
}

/*******************************************************************************
* Function: GetTokenName                                                       *
*                                                                              *
* Parameters: a member of the token_type enumerated type                       *
* Return value:                                                                *
* Description: This function returns a string containing the name of the       *
*              token passed to it.                                             *
*******************************************************************************/

string LexicalAnalyzer::GetTokenName (token_type t) const
{
	return token_names [t];
}

/*******************************************************************************
* Function: GetLexeme                                                          *
*                                                                              *
* Parameters: none                                                             *
* Return value: a string                                                       *
* Description: This function will return the lexeme found by the most recent   *
*              call to the Get_Token function.                                 *
*******************************************************************************/

string LexicalAnalyzer::GetLexeme () const
{
	return lexeme;
}

/*******************************************************************************
* Function: ReportError                                                        *
*                                                                              *
* Parameters: a string contaning the error message                             *
* Return value: none                                                           *
* Description: This function will be called to write an error message to the   *
*              listing file. This function can be used by all components       *
*              of the translator.                                              *
*******************************************************************************/

void LexicalAnalyzer::ReportError (const string & msg)
{
	listingFile << "Error at " << linenum << ',' << pos << ": " << msg << endl;
	debugFile << "Error at " << linenum << ',' << pos << ": " << msg << endl;
	errors++;
}

/*******************************************************************************
* Function: GetALine                                                           *
*                                                                              *
* Parameters: none                                                             *
* Return value: A boolean - true if the end of the file has not been reached.  *
* Description: This function will read the next line from the input file.      *
*******************************************************************************/

bool LexicalAnalyzer::GetALine ()
{
	// get a new line of characters from the input file
	getline (inputFile, line);
	if (inputFile.fail())
	{
		debugFile << '\t' << setw(16) << left << "EOF_T" << endl;
		tokenFile << '\t' << setw(16) << left << "EOF_T" << endl;
		return false;
	}
	linenum++;
	// write to appropriate files
	listingFile << setw(4) << right << linenum << ": " << line << endl;	
	debugFile << setw(4) << right << linenum << ": " << line << endl;	
	line += ' ';
	pos = 0;
	return true;
}
