/*******************************************************************************
* Assignment: Project 2 -                                                      *
* Author: Alana Reyna                                                          *
* Date: Fall 2022                                                              *
* File: SyntacticalAnalyzer.cpp                                                *
*                                                                              *
* Description: This file contains the implementation of the Syntactical		   *
* 				Analyzer class.                                                *
*******************************************************************************/

#include "SyntacticalAnalyzer.h"
#include <set>

using namespace std;

/*******************************************************************************
* Function: Syntactical Analyzer default constructor                           *
*                                                                              *
* Parameters: char * filename                                                  *
* Return value: None                                                           *
* Description: This constructor will initialize the syntactical analyzer class *
*                object.                   									   *
*******************************************************************************/

SyntacticalAnalyzer::SyntacticalAnalyzer (char * filename)
{
	cout << "Input file for Syntactical Analyzer: " << filename << endl;
	string name = filename; 
	string extension;
	if (name.length() > 3)
		extension = name.substr (name.length()-3, 3);
	if (extension != ".ss")
	{
		cout << "Invalid file extension; must be '.ss'" << endl;
		exit(1);
	}
	string p2name = name.substr (0, name.length()-3) + ".p2";
	p2File.open(p2name.c_str()); 
	if (p2File.fail())
	{
		cout << "File " << filename << " not found" << endl;
		exit(2);
	}
	lex = new LexicalAnalyzer (filename);
	token_type t;
	token = lex->GetToken();
	program ();
}

/*******************************************************************************
* Function: Syntactical Analyzer destructor                                    *
*                                                                              *
* Parameters: None                                                             *
* Return value: None                                                           *
* Description: This function will complete the execution of the syntactical    *
* 				analyzer class.	                                               *
*******************************************************************************/

SyntacticalAnalyzer::~SyntacticalAnalyzer ()
{
	delete lex;
	p2File.close();
}

/*******************************************************************************
* Function: program                                                            *
*                                                                              *
* Parameters: None                                                             *
* Return value: None                                                           *
* Description: This function will execute rule one of the grammer rules.       *
*******************************************************************************/

void SyntacticalAnalyzer::program ()
{
	// not using the first and follows set as there is only LPAREN_T in the firsts
	// and no tokens in the follows set 
	if (token == LPAREN_T)
	{
		// rule 1
		p2File << "Using rule 1" << endl;
		// get next lexeme
		token = lex -> GetToken();
		// call define
		define();
		if (token == LPAREN_T)
		{
			token = lex -> GetToken();
			more_defines();
			if (token != EOF_T)
			{
				lex -> ReportError("Expected EOF_T after more_defines");
			}
		}
		else
		{
			lex -> ReportError("Expected LPAREN_T after define");
		}
	}
	else
	{
		lex -> ReportError("Expected a RPAREN_T at the start of program");
	}
}

/*******************************************************************************
* Function: more_defines                                                       *
*                                                                              *
* Parameters: None                                                             *
* Return value: None                                                           *
* Description: This function will execute rule "more_defines" of the grammer   *
*	rules. This is rules numbers two and three of the grammer rules.           *
*******************************************************************************/

void SyntacticalAnalyzer::more_defines()
{
	set <token_type> firsts = {IDENT_T, DEFINE_T};
	set <token_type> follows = {EOF_T};

	// First, check if token is not in the firsts set
	// if not, throw an error 
	if (! firsts.count(token))
	{
		lex -> ReportError("Invalid character for firsts set");
		while (! firsts.count(token))
		{
			token = lex -> GetToken();
		}
	}
	// check for LPAREN_T 
	if (token == LPAREN_T)
	{
		// rule 2
		p2File << "Using rule 2" << endl;
		define();
		more_defines();
	}
	else if (token == IDENT_T)
	{
		// rule 3
		p2File << "Using rule 3" << endl;
		token = lex -> GetToken();
		stmt_list();
		if (token != RPAREN_T)
		{
			lex -> ReportError("Expected a RPAREN_T");

		}
		// call getToken to continue on after error check
		token = lex -> GetToken();
	}
	else
	{
		lex -> ReportError("Invalid character");
	}
	// check follows set
	if (! follows.count(token))
	{
		lex -> ReportError("Invalid character for follows set");
		while (! follows.count(token))
			token = lex -> GetToken();
	}

}

/*******************************************************************************
* Function: define                                                             *
*                                                                              *
* Parameters: None                                                             *
* Return value: None                                                           *
* Description: This function will execute rule "define" of the grammer         *
*	rules. This is rule number four of the grammer rules.                      *
*******************************************************************************/

void SyntacticalAnalyzer::define()
{
	set <token_type> firsts {DEFINE_T};
	set <token_type> follows {LPAREN_T};

	if (! firsts.count(token))
	{
		lex -> ReportError("Invalid character for firsts sett");
		while (! firsts.count(token))
		{
			token = lex -> GetToken();
		}
	}
	// check for DEFINE_T
	if (token == DEFINE_T)
	{
		// rule 4
		p2File << "Using rule 4" << endl;
		token = lex -> GetToken();
		if (token == LPAREN_T)
		{
			token = lex -> GetToken();
			if (token == IDENT_T)
			{
				token = lex -> GetToken();
				param_list();
				if (token == RPAREN_T)
				{
					token = lex -> GetToken();
					stmt();
					stmt_list();
					if (token != RPAREN_T)
					{
						lex -> ReportError("Expected a RPAREN_T");
					}
					// continue after error checking
					token = lex -> GetToken();
				}
				else
				{
					lex -> ReportError("Expected a RPAREN_T");
				}
			}
			else
			{
				lex -> ReportError("Expected an IDENT_T");
			}
		}
		else
		{
			lex -> ReportError("Expected a LPAREN_T");
		}
	}
	else
	{
		lex -> ReportError("Invalid character");
	}
	// check follows 
	if (! follows.count(token))
	{
		lex -> ReportError("Invalid character for follows set");
		while (! follows.count(token))
		{
			token = lex -> GetToken();
		}
	}
}

/*******************************************************************************
* Function: stmt_list                                                          *
*                                                                              *
* Parameters: None                                                             *
* Return value: None                                                           *
* Description: This function will execute rule "stmt_list" of the grammer      *
*	rules. This is rules numbers five and six of the grammer rules.            *
*******************************************************************************/
void SyntacticalAnalyzer::stmt_list()
{
	set <token_type> firsts {IDENT_T, LPAREN_T, NUMLIT_T, STRLIT_T, SQUOTE_T, TRUE_T,
							FALSE_T, RPAREN_T};
	set <token_type> follows {RPAREN_T};
	if (! firsts.count(token))
	{
		lex -> ReportError("Invalid character for firsts set");
		while (! firsts.count(token))
		{
			token = lex -> GetToken();
		}
	}
	if (token == IDENT_T || token == LPAREN_T || token == NUMLIT_T 
			|| token == STRLIT_T || token == SQUOTE_T || token == TRUE_T || 
			token == FALSE_T )
	{
		// rule 5 
		p2File << "Using rule 5" << endl;
		stmt();
		stmt_list();
	}
	else if (token == RPAREN_T)
	{
		// rule 6 
		p2File << "Using rule 6" << endl;
	}
	else
	{
		lex->ReportError("Invalid character");
	}
	// token should be in follows of stmt_list
	if (! follows.count(token))
	{
		lex -> ReportError("Invalid character for follows set");
		while (! follows.count(token))
		{
			token = lex -> GetToken();
		}
	}
}

/*******************************************************************************
* Function: stmt                                                               *
*                                                                              *
* Parameters: None                                                             *
* Return value: None                                                           *
* Description: This function will execute rule "stmt" of the grammer           *
*	rules. This is rules number seven, eight, and nine of the grammer rules.   *
*******************************************************************************/

void SyntacticalAnalyzer::stmt()
{
	set <token_type> firsts {NUMLIT_T, STRLIT_T, SQUOTE_T, 
							TRUE_T, FALSE_T, IDENT_T, LPAREN_T};
	set <token_type> follows {RPAREN_T, IDENT_T, LPAREN_T, NUMLIT_T,
							STRLIT_T, SQUOTE_T, TRUE_T, FALSE_T};
	
	if (! firsts.count(token))
	{
		lex -> ReportError("Invalid character for firsts set");
		while (! firsts.count(token))
		{
			token = lex -> GetToken();
		}
	}
	// take care of the firsts terminals gotten from rule 7 
	if (token == NUMLIT_T || token == STRLIT_T || token == SQUOTE_T
		|| token == TRUE_T || token == FALSE_T)
	{
		// rule 7
		p2File << "Using rule 7" << endl;
		literal(); 
	}
	// check for rule 8
	else if (token == IDENT_T)
	{
		p2File << "Using rule 8" << endl;
		token = lex -> GetToken();
	}
	// check for rule 9
	else if (token == LPAREN_T)
	{
		p2File << "Using rule 9" << endl;
		token = lex -> GetToken();
		action();
		if (token != RPAREN_T)
		{
			lex -> ReportError("Expected a RPAREN_T");
		}
		token = lex -> GetToken();
	}
	else
	{
		lex -> ReportError("Invalid character");
	}
	if (! follows.count(token))
	{
		lex -> ReportError("Invalid character for follows set");
		while (! follows.count(token))
		{
			token = lex -> GetToken();
		}
	}

}

/*******************************************************************************
* Function: literal                                                            *
*                                                                              *
* Parameters: None                                                             *
* Return value: None                                                           *
* Description: This function will execute rule "literal" of the grammer        *
*	rules. This is rules numbers ten through thirteen of the grammer rules.    *
*******************************************************************************/

void SyntacticalAnalyzer::literal()
{
	set <token_type> firsts {NUMLIT_T, STRLIT_T, SQUOTE_T, TRUE_T, FALSE_T};
	set <token_type> follows {RPAREN_T, IDENT_T, LPAREN_T, NUMLIT_T, STRLIT_T,
								SQUOTE_T, TRUE_T, FALSE_T};
	if (! firsts.count(token))
	{
		lex -> ReportError("Invalid character for firsts set");
		while (! firsts.count(token))
		{
			token = lex -> GetToken();
		}
	}
	if (token == NUMLIT_T)
	{
		p2File << "Using rule 10" << endl;
		token = lex -> GetToken();
	}
	else if (token == STRLIT_T)
	{
		p2File << "Using rule 11" << endl;
		token = lex -> GetToken();
	}
	else if (token == SQUOTE_T)
	{
		p2File << "Using rule 12" << endl;
		token = lex -> GetToken();
		quoted_lit();
	}
	else if (token == TRUE_T || token == FALSE_T)
	{
		p2File << "Using rule 13" << endl;
		logical_lit();
	}
	else 
	{
		lex -> ReportError("Invalid character");
	}
	// we should be in follows list now
	if (! follows.count(token))
	{
		lex -> ReportError("Invalid character for follows set");
		while (! follows.count(token))
		{
			token = lex -> GetToken();
		}
	}
}

/*******************************************************************************
* Function: quoted_lit                                                         *
*                                                                              *
* Parameters: None                                                             *
* Return value: None                                                           *
* Description: This function will execute rule "quoted_lit" of the grammer     *
*	rules. This is rule number 14 in the grammer rules.                        *
*******************************************************************************/

void SyntacticalAnalyzer::quoted_lit()
{
	set <token_type> firsts {LPAREN_T, IDENT_T, NUMLIT_T, STRLIT_T, IF_T, DISPLAY_T,
							NEWLINE_T, LISTOP1_T, LISTOP2_T, AND_T, OR_T, NOT_T, DEFINE_T, 
							LET_T, NUMBERP_T, LISTP_T, ZEROP_T, NULLP_T, STRINGP_T, PLUS_T, 
							MINUS_T, DIV_T, MULT_T, MODULO_T, ROUND_T, EQUALTO_T, GT_T, LT_T,
							GTE_T, LTE_T, SQUOTE_T, COND_T, ELSE_T, TRUE_T, FALSE_T};
	set <token_type> follows {RPAREN_T};

	if (! firsts.count(token))
	{
		lex -> ReportError("Invalid character for firsts set");
		while (! firsts.count(token))
		{
			token = lex -> GetToken();
		}
	}
	if (token == LPAREN_T || token == IDENT_T || token == NUMLIT_T || token == STRLIT_T || 
		token == IF_T || token == DISPLAY_T || token == NEWLINE_T || token == LISTOP1_T || 
		token == LISTOP2_T || token == AND_T || token == OR_T || token == NOT_T || token == DEFINE_T || 
		token == LET_T || token == NUMBERP_T || token == LISTP_T || token == ZEROP_T || token == NULLP_T || 
		token == STRINGP_T || token == PLUS_T || token == MINUS_T || token == DIV_T || token == MULT_T ||
		token == MODULO_T || token == ROUND_T || token == EQUALTO_T || token == GT_T || token == LT_T || 
		token == GTE_T || token == LTE_T || token == SQUOTE_T || token == COND_T || token == ELSE_T || 
		token == TRUE_T || token == FALSE_T)
	{
		p2File << "Using rule 14" << endl;
		any_other_token();
	} 
	else
	{
		lex -> ReportError("Invalid character");
	}
	if (! follows.count(token))
	{
		lex -> ReportError("Invalid character for follows set");
		while (! follows.count(token))
		{
			token = lex -> GetToken();
		}
	}
}

/*******************************************************************************
* Function: logical_lit                                                        *
*                                                                              *
* Parameters: None                                                             *
* Return value: None                                                           *
* Description: This function will execute rule "logical_lit" of the grammer    *
*	rules. This is rules numbers fifteen and sixteen of the grammer rules.     *
*******************************************************************************/

void SyntacticalAnalyzer::logical_lit()
{
	set <token_type> firsts {TRUE_T, FALSE_T};
	set <token_type> follows {RPAREN_T};

	if (! firsts.count(token))
	{
		lex -> ReportError("Invalid character for firsts set");
		while (! firsts.count(token))
		{
			token = lex -> GetToken();
		}
	}
	if (token == TRUE_T)
	{
		p2File << "Using rule 15" << endl;
		token = lex -> GetToken();
	}
	else if (token == FALSE_T)
	{
		p2File << "Using rule 16" << endl;
		token = lex -> GetToken();
	}
	// check if we are in follows set now 
	if (! follows.count(token))
	{
		lex -> ReportError("Invalid character for follows set");
		while (! follows.count(token))
		{
			token = lex -> GetToken();
		}
	}
}

/*******************************************************************************
* Function: more_tokens                                                        *
*                                                                              *
* Parameters: None                                                             *
* Return value: None                                                           *
* Description: This function will execute rule "more_tokens" of the grammer    *
*	rules. This is rules numbers seventeen and eighteen of the grammer rules.  *
*******************************************************************************/

void SyntacticalAnalyzer::more_tokens()
{
	set <token_type> firsts {LPAREN_T, IDENT_T, NUMLIT_T, STRLIT_T, IF_T, DISPLAY_T,
							NEWLINE_T, LISTOP1_T, LISTOP2_T, AND_T, OR_T, NOT_T, DEFINE_T, 
							LET_T, NUMBERP_T, LISTP_T, ZEROP_T, NULLP_T, STRINGP_T, PLUS_T, 
							MINUS_T, DIV_T, MULT_T, MODULO_T, ROUND_T, EQUALTO_T, GT_T, LT_T,
							GTE_T, LTE_T, SQUOTE_T, COND_T, ELSE_T, TRUE_T, FALSE_T, RPAREN_T};
	set <token_type> follows {RPAREN_T};

	if (! firsts.count(token))
	{
		lex -> ReportError("Invalid character for firsts set");
		while (! firsts.count(token))
		{
			token = lex -> GetToken();
		}
	}
	if (token == LPAREN_T || token == IDENT_T || token == NUMLIT_T || token == STRLIT_T || 
		token == IF_T || token == DISPLAY_T || token == NEWLINE_T || token == LISTOP1_T || 
		token == LISTOP2_T || token == AND_T || token == OR_T || token == NOT_T || token == DEFINE_T || 
		token == LET_T || token == NUMBERP_T || token == LISTP_T || token == ZEROP_T || token == NULLP_T || 
		token == STRINGP_T || token == PLUS_T || token == MINUS_T || token == DIV_T || token == MULT_T ||
		token == MODULO_T || token == ROUND_T || token == EQUALTO_T || token == GT_T || token == LT_T || 
		token == GTE_T || token == LTE_T || token == SQUOTE_T || token == COND_T || token == ELSE_T || 
		token == TRUE_T || token == FALSE_T)
	{
		p2File << "Using rule 17" << endl;
		any_other_token();
		more_tokens();
	}
	else if (token == RPAREN_T)
	{
		p2File << "Using rule 18" << endl;
	}
	else
	{
		lex -> ReportError("Invalid character");
	}
	// error check we are in the follows set 
	if (! follows.count(token))
	{
		lex -> ReportError("Invalid character for follows set");
		while (! follows.count(token))
		{
			token = lex -> GetToken();
		}
	}
}

/*******************************************************************************
* Function: param_list                                                         *
*                                                                              *
* Parameters: None                                                             *
* Return value: None                                                           *
* Description: This function will execute rule "param_list" of the grammer     *
*	rules. This is rules numbers nineteen and twenty of the grammer rules.     *
*******************************************************************************/

void SyntacticalAnalyzer::param_list()
{
	set <token_type> firsts {IDENT_T, RPAREN_T};
	set <token_type> follows {RPAREN_T};

	if (! firsts.count(token))
	{
		lex -> ReportError("Invalid character for firsts set");
		while (! firsts.count(token))
		{
			token = lex -> GetToken();
		}
	}
	if (token == IDENT_T)
	{
		p2File << "Using rule 19" << endl;
		token = lex -> GetToken();
		param_list();
	}
	else if (token == RPAREN_T)
	{
		p2File << "Using rule 20" << endl;
	}
	// check to make sure we are in the follows set now
	if (! follows.count(token))
	{
		lex -> ReportError("Invalid character for follows set");
		while (! follows.count(token))
		{
			token = lex -> GetToken();
		}
	}
}

/*******************************************************************************
* Function: else_part                                                          *
*                                                                              *
* Parameters: None                                                             *
* Return value: None                                                           *
* Description: This function will execute rule "else_part" of the grammer      *
* rules. This is rules numbers twenty one and twenty two of the grammer rules. *
*******************************************************************************/

void SyntacticalAnalyzer::else_part()
{
	set <token_type> firsts {NUMLIT_T, STRLIT_T, SQUOTE_T, TRUE_T, FALSE_T, IDENT_T,
							LPAREN_T, RPAREN_T};
	set <token_type> follows {RPAREN_T};

	if (! firsts.count(token))
	{
		lex -> ReportError("Invalid character for firsts set");
		while (! firsts.count(token))
		{
			token = lex -> GetToken();
		}
	}
	if (token == NUMLIT_T || token == STRLIT_T || token == SQUOTE_T || 
		token == TRUE_T || token == FALSE_T || token == LPAREN_T)
	{
		p2File << "Using rule 21" << endl;
		stmt();
	}
	else if (token == RPAREN_T)
	{
		p2File << "Using rule 22" << endl;
	}
	else
	{
		lex -> ReportError("Invalid character");
	}
	// error check to see if we are in follows
	if(! follows.count(token))
	{
		lex -> ReportError("Invalid character for follows set");
		while (! follows.count(token))
		{
			token = lex -> GetToken();
		}
	}
}

/*******************************************************************************
* Function: stmt_pair                                                          *
*                                                                              *
* Parameters: None                                                             *
* Return value: None                                                           *
* Description: This function will execute rule "stmt_pair" of the grammer      *
* rules. This is rules numbers twenty-three and twenty-four of the grammer     *
* rules.   																	   *
*******************************************************************************/

void SyntacticalAnalyzer::stmt_pair()
{
	set <token_type> firsts {LPAREN_T};
	set <token_type> follows {RPAREN_T};
	
	if (! firsts.count(token))
	{
		lex -> ReportError("Invalid character for firsts set");
		while (! firsts.count(token))
		{
			token = lex -> GetToken();
		}
	}
	if (token == LPAREN_T)
	{
		p2File << "Using rule 23" << endl;
		token = lex -> GetToken();
		stmt_pair_body();
	}
	else if (token == RPAREN_T)
	{
		p2File << "Using rule 24" << endl;
	}
	else
	{
		lex -> ReportError("Invalid character");
	}
	// error check that we are in follows
	if(! follows.count(token))
	{
		lex -> ReportError("Invalid character for follows set");
		while (! follows.count(token))
		{
			token = lex -> GetToken();
		}
	}
}

/*******************************************************************************
* Function: stmt_pair_body                                                     *
*                                                                              *
* Parameters: None                                                             *
* Return value: None                                                           *
* Description: This function will execute rule "stmt_pair_body" of the grammer *
* rules. This is rules numbers twenty-five and twenty-six of the grammer       *
* rules.   																	   *
*******************************************************************************/

void SyntacticalAnalyzer::stmt_pair_body()
{
	set <token_type> firsts {NUMLIT_T, STRLIT_T, SQUOTE_T, 
							TRUE_T, FALSE_T, IDENT_T, LPAREN_T, ELSE_T};
	set <token_type> follows {RPAREN_T};

	if (! firsts.count(token))
	{
		lex -> ReportError("Invalid character for firsts set");
		while (! firsts.count(token))
		{
			token = lex -> GetToken();
		}
	}
	if (token == IDENT_T || token == LPAREN_T || token == NUMLIT_T 
		|| token == STRLIT_T || token == SQUOTE_T || token == TRUE_T || 
		token == FALSE_T )
	{
		p2File << "Using rule 25" << endl;
		stmt();
	}
	else if (token == ELSE_T)
	{
		p2File << "Using rule 26" << endl;
		token = lex -> GetToken();
		stmt();
		if (token != RPAREN_T)
		{
			lex -> ReportError("Expected a RPAREN_T");
		}
		token = lex -> GetToken();
	}
	else
	{
		lex -> ReportError("Invalid character");
	}
	// error check that we are in follows
	if(! follows.count(token))
	{
		lex -> ReportError("Invalid character for follows set");
		while (! follows.count(token))
		{
			token = lex -> GetToken();
		}
	}
}

/*******************************************************************************
* Function: assign_pair                                                        *
*                                                                              *
* Parameters: None                                                             *
* Return value: None                                                           *
* Description: This function will execute rule "assign_pair" of the grammer    *
* rules. This is rule number 27 											   *
*******************************************************************************/

void SyntacticalAnalyzer::assign_pair()
{
	set <token_type> firsts {LPAREN_T};
	set <token_type> follows {RPAREN_T};

	if (! firsts.count(token))
	{
		lex -> ReportError("Invalid character for firsts set");
		while (! firsts.count(token))
		{
			token = lex -> GetToken();
		}
	}
	if (token == LPAREN_T)
	{
		p2File << "Using rule 27" << endl;
		token = lex -> GetToken();
		if (token == IDENT_T)
		{
			token = lex -> GetToken();
			stmt();
			if (token != RPAREN_T)
			{
				lex -> ReportError("Expected a RPAREN_T");
			}
		}
		else
		{
			lex -> ReportError("Expected a IDENT_T");
		}
	}
	else
	{
		lex -> ReportError("Invalid character");
	}
	// error check that we are in follows
	if(! follows.count(token))
	{
		lex -> ReportError("Invalid character for follows set");
		while (! follows.count(token))
		{
			token = lex -> GetToken();
		}
	}
}

/*******************************************************************************
* Function: more_assigns                                                       *
*                                                                              *
* Parameters: None                                                             *
* Return value: None                                                           *
* Description: This function will execute rule "more_assigns" of the grammer   *
* rules. This is rules numbers twenty eight and twenty nine.  				   *
*******************************************************************************/

void SyntacticalAnalyzer::more_assigns()
{
	set <token_type> firsts {LPAREN_T, RPAREN_T};
	set <token_type> follows {RPAREN_T};

	if (! firsts.count(token))
	{
		lex -> ReportError("Invalid character for firsts set");
		while (! firsts.count(token))
		{
			token = lex -> GetToken();
		}
	}
	if (token == LPAREN_T)
	{
		p2File << "Using rule 28" << endl;
		assign_pair();
		more_assigns();
	}
	else if (token == RPAREN_T)
	{
		p2File << "Using rule 29" << endl;
	}
	else 
	{
		lex -> ReportError("Invalid character");
	}
	// error check that we are in follows
	if(! follows.count(token))
	{
		lex -> ReportError("Invalid character for follows set");
		while (! follows.count(token))
		{
			token = lex -> GetToken();
		}
	}
}

/*******************************************************************************
* Function: action                                                             *
*                                                                              *
* Parameters: None                                                             *
* Return value: None                                                           *
* Description: This function will execute rule "action" of the grammer         *
* rules. This is rules numbers thirty through fifty six.     				   *
*******************************************************************************/

void SyntacticalAnalyzer::action()
{
	set <token_type> firsts {IF_T, COND_T, LET_T, LISTOP1_T, LISTOP2_T, AND_T, 
							OR_T, NOT_T, NUMBERP_T, LISTP_T, ZEROP_T, NULLP_T, 
							STRINGP_T, PLUS_T, MINUS_T, DIV_T, MULT_T, MODULO_T,
							ROUND_T, EQUALTO_T, GT_T, LT_T, GTE_T, LTE_T, IDENT_T,
							DISPLAY_T, NEWLINE_T};
	set <token_type> follows {RPAREN_T};

	if (! firsts.count(token))
	{
		lex -> ReportError("Invalid character for firsts set");
		while (! firsts.count(token))
		{
			token = lex -> GetToken();
		}
	}
	if (token == IF_T)
	{
		p2File << "Using rule 30" << endl;
		token = lex -> GetToken();
		stmt();
		stmt();
		else_part();
	}
	else if (token == COND_T)
	{
		p2File << "Using rule 31" << endl;
		token = lex -> GetToken();
		if (token != LPAREN_T)
		{
			lex -> ReportError("Expected a LPAREN_T");
		}
		token = lex -> GetToken();
		stmt_pair_body();
	}
	else if (token == LET_T)
	{
		p2File << "Using rule 32" << endl;
		token = lex -> GetToken();
		if (token != LPAREN_T)
		{
			lex -> ReportError("Expected a LPAREN_T");
		}
		token = lex -> GetToken();
		more_assigns();
		token = lex -> GetToken();
		if (token != RPAREN_T)
		{
			lex -> ReportError("Expected a RPAREN_T");
		}
		token = lex -> GetToken();
		stmt();
		stmt_list();
	}
	else if (token == LISTOP1_T)
	{
		p2File << "Using rule 33" << endl;
		token = lex -> GetToken();
		stmt();
	}
	else if (token == LISTOP2_T)
	{
		p2File << "Using rule 34" << endl;
		token = lex -> GetToken();
		stmt();
		stmt();
	}
	else if (token == AND_T)
	{
		p2File << "Using rule 35" << endl;
		token = lex -> GetToken();
		stmt_list();
	}
	else if (token == OR_T)
	{
		p2File << "Using rule 36" << endl;
		token = lex -> GetToken();
		stmt_list();
	}
	else if (token == NOT_T)
	{
		p2File << "Using rule 37" << endl;
		token = lex -> GetToken();
		stmt();
	}
	
	else if (token == NUMBERP_T)
	{
		p2File << "Using rule 38" << endl;
		token = lex -> GetToken();
		stmt();
	}
	else if (token == LISTP_T)
	{
		p2File << "Using rule 39" << endl;
		token = lex -> GetToken();
		stmt();
	}
	else if (token == ZEROP_T)
	{
		p2File << "Using rule 40" << endl;
		token = lex -> GetToken();
		stmt();
	}
	else if (token == NULLP_T)
	{
		p2File << "Using rule 41" << endl;
		token = lex -> GetToken();
		stmt();
	}
	else if (token == STRINGP_T)
	{
		p2File << "Using rule 42" << endl;
		token = lex -> GetToken();
		stmt();
	}
	else if (token == PLUS_T)
	{
		p2File << "Using rule 43" << endl;
		token = lex -> GetToken();
		stmt_list();
	}
	else if (token == MINUS_T)
	{
		p2File << "Using rule 44" << endl;
		token = lex -> GetToken();
		stmt();
		stmt_list();
	}
	else if (token == DIV_T)
	{
		p2File << "Using rule 45" << endl;
		token = lex -> GetToken();
		stmt();
		stmt_list();
	}
	else if (token == MULT_T)
	{
		p2File << "Using rule 46" << endl;
		token = lex -> GetToken();
		stmt_list();
	}
	else if (token == MODULO_T)
	{
		p2File << "Using rule 47" << endl;
		token = lex -> GetToken();
		stmt();
		stmt();
	}
	else if (token == ROUND_T)
	{
		p2File << "Using rule 48" << endl;
		token = lex -> GetToken();
		stmt();
	}
	else if (token == EQUALTO_T)
	{
		p2File << "Using rule 49" << endl;
		token = lex -> GetToken();
		stmt_list();
	}
	else if (token == GT_T)
	{
		p2File << "Using rule 50" << endl;
		token = lex -> GetToken();
		stmt_list();
	}
	else if (token == LT_T)
	{
		p2File << "Using rule 51" << endl;
		token = lex -> GetToken();
		stmt_list();
	}
	else if (token == GTE_T)
	{
		p2File << "Using rule 52" << endl;
		token = lex -> GetToken();
		stmt_list();
	}
	else if (token == LTE_T)
	{
		p2File << "Using rule 53" << endl;
		token = lex -> GetToken();
		stmt_list();
	}
	else if (token == IDENT_T)
	{
		p2File << "Using rule 54" << endl;
		token = lex -> GetToken();
		stmt_list();
	}
	else if (token == DISPLAY_T)
	{
		p2File << "Using rule 55" << endl;
		token = lex -> GetToken();
		stmt();
	}
	else if (token == NEWLINE_T)
	{
		p2File << "Using rule 56" << endl;
		token = lex -> GetToken();
	}
	else
	{
		lex -> ReportError("Invalid character");
	}
	// error check to make sure we are in the follows set
	if(! follows.count(token))
	{
		lex -> ReportError("Invalid character for follows set");
		while (! follows.count(token))
		{
			token = lex -> GetToken();
		}
	}
}

/*******************************************************************************
* Function: any_other_token                                                    *
*                                                                              *
* Parameters: None                                                             *
* Return value: None                                                           *
* Description: This function will execute rule "any_other_token" of the grammer*
* rules. This is rules numbers fifty-seven through ninety-one.     			   *
*******************************************************************************/

void SyntacticalAnalyzer::any_other_token()
{
	set <token_type> firsts {LPAREN_T, IDENT_T, NUMLIT_T, STRLIT_T, IF_T, DISPLAY_T,
							NEWLINE_T, LISTOP1_T, LISTOP2_T, AND_T, OR_T, NOT_T, DEFINE_T, 
							LET_T, NUMBERP_T, LISTP_T, ZEROP_T, NULLP_T, STRINGP_T, PLUS_T, 
							MINUS_T, DIV_T, MULT_T, MODULO_T, ROUND_T, EQUALTO_T, GT_T, LT_T,
							GTE_T, LTE_T, SQUOTE_T, COND_T, ELSE_T, TRUE_T, FALSE_T};
	set <token_type> follows {RPAREN_T};

	if (! firsts.count(token))
	{
		lex -> ReportError("Invalid character for firsts set");
		while (! firsts.count(token))
		{
			token = lex -> GetToken();
		}
	}
	if (token == LPAREN_T)
	{
		p2File << "Using rule 57" << endl;
		token = lex -> GetToken();
		more_tokens();
		if (token != RPAREN_T)
		{
			lex -> ReportError("Expected a RPAREN_T");
		}
		token = lex -> GetToken();
	}
	else if (token == IDENT_T)
	{
		p2File << "Using rule 58" << endl;
		token = lex -> GetToken();
	}
	else if (token == NUMLIT_T)
	{
		p2File << "Using rule 59" << endl;
		token = lex -> GetToken();
	}
	else if (token == STRLIT_T)
	{
		p2File << "Using rule 60" << endl;
		token = lex -> GetToken();
	}
	else if (token == IF_T)
	{
		p2File << "Using rule 61" << endl;
		token = lex -> GetToken();
	}
	else if (token == DISPLAY_T)
	{
		p2File << "Using rule 62" << endl;
		token = lex -> GetToken();
	}
	else if (token == NEWLINE_T)
	{
		p2File << "Using rule 63" << endl;
		token = lex -> GetToken();
	}
	else if (token == LISTOP1_T)
	{
		p2File << "Using rule 64" << endl;
		token = lex -> GetToken();
	}
	else if (token == LISTOP2_T)
	{
		p2File << "Using rule 65" << endl;
		token = lex -> GetToken();
	}
	else if (token == AND_T)
	{
		p2File << "Using rule 66" << endl;
		token = lex -> GetToken();
	}
	else if (token == OR_T)
	{
		p2File << "Using rule 67" << endl;
		token = lex -> GetToken();
	}
	else if (token == NOT_T)
	{
		p2File << "Using rule 68" << endl;
		token = lex -> GetToken();
	}
	else if (token == DEFINE_T)
	{
		p2File << "Using rule 69" << endl;
		token = lex -> GetToken();
	}
	else if (token == LET_T)
	{
		p2File << "Using rule 70" << endl;
		token = lex -> GetToken();
	}
	else if (token == NUMBERP_T)
	{
		p2File << "Using rule 71" << endl;
		token = lex -> GetToken();
	}
	else if (token == LISTP_T)
	{
		p2File << "Using rule 72" << endl;
		token = lex -> GetToken();
	}
	else if (token == ZEROP_T)
	{
		p2File << "Using rule 73" << endl;
		token = lex -> GetToken();
	}
	else if (token == NULLP_T)
	{
		p2File << "Using rule 74" << endl;
		token = lex -> GetToken();
	}
	else if (token == STRINGP_T)
	{
		p2File << "Using rule 75" << endl;
		token = lex -> GetToken();
	}
	else if (token == PLUS_T)
	{
		p2File << "Using rule 76" << endl;
		token = lex -> GetToken();
	}
	else if (token == MINUS_T)
	{
		p2File << "Using rule 77" << endl;
		token = lex -> GetToken();
	}
	else if (token == DIV_T)
	{
		p2File << "Using rule 78" << endl;
		token = lex -> GetToken();
	}
	else if (token == MULT_T)
	{
		p2File << "Using rule 79" << endl;
		token = lex -> GetToken();
	}
	else if (token == MODULO_T)
	{
		p2File << "Using rule 80" << endl;
		token = lex -> GetToken();
	}
	else if (token == ROUND_T)
	{
		p2File << "Using rule 81" << endl;
		token = lex -> GetToken();
	}
	else if (token == EQUALTO_T)
	{
		p2File << "Using rule 82" << endl;
		token = lex -> GetToken();
	}
	else if (token == GT_T)
	{
		p2File << "Using rule 83" << endl;
		token = lex -> GetToken();
	}
	else if (token == LT_T)
	{
		p2File << "Using rule 84" << endl;
		token = lex -> GetToken();
	}
	else if (token == GTE_T)
	{
		p2File << "Using rule 85" << endl;
		token = lex -> GetToken();
	}
	else if (token == LTE_T)
	{
		p2File << "Using rule 86" << endl;
		token = lex -> GetToken();
	}
	else if (token == SQUOTE_T)
	{
		p2File << "Using rule 87" << endl;
		token = lex -> GetToken();
		any_other_token();
	}
	else if (token == COND_T)
	{
		p2File << "Using rule 88" << endl;
		token = lex -> GetToken();
	}
	else if (token == ELSE_T)
	{
		p2File << "Using rule 89" << endl;
		token = lex -> GetToken();
	}
	else if (token == TRUE_T)
	{
		p2File << "Using rule 90" << endl;
		token = lex -> GetToken();
	}
	else if (token == FALSE_T)
	{
		p2File << "Using rule 91" << endl;
		token = lex -> GetToken();
	}
	else
	{
		lex -> ReportError("Invalid character");
	}
	// error check to make sure we are in the follows set
	if(! follows.count(token))
	{
		lex -> ReportError("Invalid character for follows set");
		while (! follows.count(token))
		{
			token = lex -> GetToken();
		}
	}
}