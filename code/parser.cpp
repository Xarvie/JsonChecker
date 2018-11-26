/*
* parser.cpp
*
* Created on: May 12, 2018
*	 Author: Xarvie
*/

#include "parser.h"
#include "common.h"

// TODO Scanner
size_t	Scanner::pos = 0;
char	Scanner::oldCh = ' ';
int		Scanner::lineNumber = 0;
int		Scanner::linePos = 0;
int		Scanner::oldLinePos = 0;
int		Scanner::goBack = 0;
state	Scanner::sState = NORMAL;


int CONFIG_USE_COMMENT = 1;
int CONFIG_TAB_IN_STR = 1;
int CONFIG_ZERO_FIRST = 1;
int CONFIG_DEEP_LIMIT = 1;

int CONFIG_PRINT_ERROR = 0;

Scanner::Scanner()
{
}

Scanner::~Scanner()
{
}

char Scanner::getCurCh()
{
	return fileStr[Scanner::pos - 1];
}

char Scanner::get()
{
	if (pos == fileStrLen)
		return 0;
	char ch = fileStr[pos];
	if (pos>0)
		Scanner::oldCh = fileStr[pos - 1];
	if (Scanner::oldCh == '\n')
	{
		Scanner::lineNumber++;
		Scanner::linePos = 0;
	}
	Scanner::pos++;

	ch = Scanner::skipComment(ch);
	return ch;
}

char Scanner::getCh()
{
	char ch;
	do {
		ch = Scanner::get();
	} while (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t');
	return ch;
}

char Scanner::what()
{
	if (pos == fileStrLen)
		return 0;
	size_t tmpPos = pos;
	char ch = fileStr[tmpPos++];
	ch = skipCommentByWhat(ch, tmpPos);
	return ch;
}

char Scanner::whatCh()
{
	char ch = 0;
	size_t pos = Scanner::pos;
	if (pos >= fileStrLen)
		return 0;
	do
	{
		if (pos >= fileStrLen)
			return 0;
		ch = fileStr[pos++];
		ch = skipCommentByWhat(ch, pos);
	} while (ch == ' ' || ch == '\r' || ch == '\n' || ch == '\t');
	return ch;
}

int Scanner::getStr(std::string& str, size_t& len)
{
	Scanner::sState = STR;
	char ch = 0;
	for (;;)
	{
		ch = Scanner::get();
		if (ch == '\"')
		{
			Scanner::sState = NORMAL;
			return PASSED;
		}
		else if (ch == '\\')
		{
			ch = Scanner::get();
			switch (ch)
			{
			case '\"':break;
			case '\\':break;
			case '/': break;
			case 'b': break;
			case 'f': break;
			case 'n': break;
			case 'r': break;
			case 't': break;
			case 'u':
				break;
			default:
				ERRORSTR("后面只能是\" \\ b f n r t u");
			}
		}
		else if (ch == '\r' || ch == '\n')
		{
			ERRORSTR("没有右引号");
		}
		else if (((unsigned char)ch < ' ' && !(CONFIG_TAB_IN_STR && ch == '\t')))
		{
			ERRORSTR("字符串中有非法字符");
		}
	}
	Scanner::sState = NORMAL;
	return 0;
}

#define NEXT do{\
	if(pos==fileStrLen)\
		return 0;\
	ch = fileStr[pos];\
	if(pos>0)\
		oldCh = fileStr[pos - 1];\
	if(oldCh=='\n')\
	{\
		lineNumber++;\
		linePos=0;\
	}\
	pos++;\
}while(0)

#define SKIP_COMMENT do								\
{													\
if(Scanner::sState == STR)							\
	return ch;										\
int i = 0;											\
while(CONFIG_USE_COMMENT && ch == '/'){				\
	i++;											\
	char ch2 = fileStr[pos];						\
	if(ch2 == 0) return 0;							\
	if(ch2 == '/')									\
		while((ch2=fileStr[pos])){					\
			NEXT;									\
			if((ch2 == '\r' || ch2 =='\n'))			\
				break;								\
		}											\
	else if(ch2 == '*'){							\
		NEXT;										\
		for(;;){									\
			NEXT;									\
			if(ch== '*'){							\
				NEXT;								\
				if(ch == '/'){						\
					NEXT;							\
					break;							\
	}}}}											\
	else if(i == 1) {								\
		break; 										\
	}												\
}													\
}while(0)

char Scanner::skipComment(char ch)
{
	SKIP_COMMENT;
	return ch;
}

char Scanner::skipCommentByWhat(char ch, size_t& pos)
{
	char oldCh = Scanner::oldCh;
	int lineNumber = Scanner::lineNumber;
	int linePos = Scanner::linePos;
	SKIP_COMMENT;
	return ch;
}

// TODO Parser
int		Parser::errorNum = 0;

Parser::Parser()
{
}

Parser::~Parser()
{

}

int Parser::parse_literal(const char* literal)
{
	literal++;
	char ch = Scanner::get();
	for (size_t i = 0; literal[i + 1]; i++)
	{
		ch = Scanner::get();
		if (ch != literal[i + 1])
			ERRORSTR("关键字错误");
	}
	return PASSED;
}

int Parser::parse_number()
{
	char beginch = Scanner::getCurCh();
	char ch = beginch;
	if (beginch == '-')
		ch = Scanner::get();

	if ('0' == ch && !CONFIG_ZERO_FIRST) {}
	else if (IS0T9(ch))
	{
		for (char wch = Scanner::what(); IS0T9(wch); wch = Scanner::what())
			ch = Scanner::get();
	}
	else
	{
		if (beginch == '-')
			ERRORSTR("负数号后面必须是数字");
		ERRORSTR("奇怪的符号");
	}

	char wch = Scanner::what();
	if (wch == '.')
	{
		ch = Scanner::get();
		ch = Scanner::get();
		if (!IS0T9(ch))
			ERRORSTR("小数点后不是数字");
		for (wch = Scanner::what(); IS0T9(wch); wch = Scanner::what())
			ch = Scanner::get();
	}
	if (wch == 'e' || wch == 'E')
	{
		ch = Scanner::get();
		ch = Scanner::get();
		if (ch == '+' || ch == '-')
			ch = Scanner::get();
		if (!IS0T9(ch))
			ERRORSTR("科学记数E后不是数字加号减号");
		for (char wch = Scanner::what(); IS0T9(wch); wch = Scanner::what())
			ch = Scanner::get();
	}
	return PASSED;
}

int Parser::parse_string()
{
	int ret = 0;
	size_t len;
	std::string str = "";
	ret = Scanner::getStr(str, len);
	return ret;
}

int Parser::parse_array()
{
	char ch = Scanner::whatCh();
	if (ch == ']')
	{
		Scanner::getCh();
		return PASSED;
	}

	for (;;)
	{
		if (parse_value() != PASSED)
			return -1;
		ch = Scanner::getCh();
		if (ch == ',')
		{
		}
		else if (ch == ']')
		{
			return PASSED;
		}
		else
		{
			ERRORSTR("缺少]右中括号 | 有奇怪的字符挡在]右中括号前面");
		}
	}
	return PASSED;
}

int Parser::parse_object()
{
	int size = 0;

	char wch = Scanner::whatCh();
	if (wch == '}')
	{
		Scanner::getCh();
		return PASSED;
	}
	char ch = Scanner::getCh();
	for (;;)
	{
		std::string str;
		size_t len = 0;
		if (ch != '"')
		{
			if (ch == '}')
				ERRORSTR("末尾多了逗号");
			else
				ERRORSTR("key丢失");
		}
		Scanner::getStr(str, len);
		if ((ch = Scanner::getCh()) != ':')
			ERRORSTR("key后面丢失冒号");

		if (parse_value() != PASSED)
			return -1;
		size++;
		ch = Scanner::getCh();
		if (ch == ',')
		{
			ch = Scanner::getCh();
		}
		else if (ch == '}')
		{
			return PASSED;
		}
		else {
			ERRORSTR("object没逗号或者右花括号");
			break;
		}
	}
	return 0;
}

int Parser::parse_value()
{
	char ch = Scanner::getCh();
	switch (ch)
	{
	case 't': return parse_literal("true");
	case 'f': return parse_literal("false");
	case 'n': return parse_literal("null");
	case '"': return parse_string();
	case '[': return parse_array();
	case '{': return parse_object();
	case '\0': return -1;
	default: return parse_number();
	}
	return 0;
}

int Parser::error(const char * str)
{
	size_t epos = Scanner::pos;
	size_t elineNum = Scanner::lineNumber;
	//size_t elinePos = Scanner::linePos;
	{
		Scanner::pos = 0;
		Scanner::oldCh = ' ';
		Scanner::lineNumber = 0;

		Scanner::linePos = 0;
		Scanner::oldLinePos = 0;
		Scanner::goBack = 0;
	}
	if (CONFIG_PRINT_ERROR)
	{
		std::cout << "1:";
		for (size_t i = 0; i < epos; i++)
		{
			if (Scanner::pos == fileStrLen)
				return 0;
			char ch = fileStr[Scanner::pos];
			if (Scanner::pos>0)
				Scanner::oldCh = fileStr[Scanner::pos - 1];
			if (Scanner::oldCh == '\n')
			{
				Scanner::lineNumber++;
				Scanner::linePos = 0;
				std::cout << Scanner::lineNumber + 1 << ":";
			}
			Scanner::pos++;
			std::cout << ch;
		}
	}
	std::cout << "\n\n错误: 第" << elineNum + 2 << "行上方" << "," << str << std::endl;
	for (size_t i = 0; i < Scanner::pos; i++)
		Scanner::get();

	return -1;
}
