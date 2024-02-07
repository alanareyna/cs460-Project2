/*****************************************************************************/
/*                                                                           */
/* Assignment: CS 460 Project 2                                              */
/* Author: Dr. Watts                                                         */
/* Date: September 2022                                                      */
/* Modified by: Alana Reyna                                                  */
/* Date:                                                                     */
/* Description: This file contains the driver program for Project 2.         */
/*                                                                           */
/*****************************************************************************/

#include <iostream>
#include "SyntacticalAnalyzer.h"

int main (int argc, char * argv[])
{
	if (argc < 2)
	{
		printf ("format: P2.out <filename>\n");
		exit (1);
	}
	SyntacticalAnalyzer parser (argv[1]);

	return 0;
}
