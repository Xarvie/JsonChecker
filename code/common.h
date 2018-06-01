/*
 * common.h
 *
 * Created on: May 12, 2018
 *	 Author: Xarvie
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <stdio.h>

extern char * fileStr;
extern void * fileBuff;
extern size_t fileStrLen;

#define ERRORSTR(x) do{return Parser::error(x);} while(0)
#define IS0T9(a) ((a) >='0' && (a)<='9')
#define IS1T9(a) ((a) >='1' && (a)<='9')

#endif /* COMMON_H_ */
