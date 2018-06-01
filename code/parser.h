/*
 * parser.h
 *
 * Created on: May 12, 2018
 *	 Author: Xarvie
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <iostream>
#include <stddef.h>

enum {
	PASSED = 0,
};

enum state{ NORMAL, STR, COMMENTA, COMMENTB};

class Scanner
{
public:
	Scanner();
	virtual ~Scanner();
	static char getCurCh();
	static char get();
	static char getCh();
	static char what();
	static char whatCh();
	static int getStr(std::string& str, size_t& len);
	static char skipComment(char ch);
	static char skipCommentByWhat(char ch, size_t& pos);

public:
	static size_t pos;
	static char oldCh;
	static int lineNumber;
	static int linePos;
	static int oldLinePos;
	static int goBack;
	static state sState;
};

class Parser
{
public:
	Parser();
	virtual ~Parser();
	static int parse_value();
	static int parse_number();
	static int parse_string();
	static int parse_array();
	static int parse_object();
	static int parse_literal(const char* literal);
	static int error(const char * str);
	static int errorNum;
};

#endif /* PARSER_H_ */
