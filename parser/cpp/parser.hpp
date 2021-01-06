#include <fstream>
#include <iostream>
#include <sstream> 	//std::stringstream

#include <functional>
#include <cctype> 	// for std::isspace
#include <cstring> 	// for strchr
#include <vector>
#include <iterator>
#include <algorithm>

static std::string keywords[] = {
	"auto", "break", "case", "char", "const", "continue", "default", 
	"do", "double", "else", "enum", "extern", "float", "for", "goto", 
	"if", "inline", "int", "long", "register", "restrict", "return", 
	"short", "signed", "sizeof", "static", "struct", "switch", "typedef",
	"union", "unsigned", "void", "volatile", "while"
};

static std::string type[] = {
	"char", "double", "enum", "float", "int", "long", "short", "signed", "void"
};

typedef enum TokenType {
	NUMBER,
	STRING,
	KEYWORD,
	TYPE,
	SINGLEQUOTE,
	IDENTIFICATOR,
	PUNCTUATION,
	OPERATOR,
	EOT,
	ERROR,
	MACRO
} token_t;

class InputStream {
    std::string input;
    int pos = 0, line = 1, col = 0;
    int length;
public:
	//int block=0, bracket=0, istype=0;
    InputStream() = delete;
	InputStream(std::string& input) {
		this->input = input;
		this->length = input.length();
	}
    char next() {
        char ch = input.at(pos++);
        if (ch == '\n') line++, col = 0; else col++;
        return ch;
    }
    char peek() {
        return input.at(pos);
    }
    bool eof() {
    	return pos == length;
    }
    void croak(std::string msg) {
        throw std::invalid_argument( msg + " (" + std::to_string(line) + ":" + std::to_string(col) + ")" );
    }
    int get_pos() {
    	return pos;
    }
	int get_line() {
		return line;
	}
	int get_col() {
		return col;
	}
};
class Token {
	std::string content;
	token_t type;
	int line;
	int col;
public:
	Token() = delete;
	Token(std::string content, token_t type) {
		this->content = content;
		this->type = type;
	}
	Token(std::string content, token_t type, int line, int col) {
		this->content = content;
		this->type = type;
		this->line = line;
		this->col = col;
	}
	friend std::ostream& operator<<(std::ostream& os, const Token& dt);
    token_t get_type() {
    	return this->type;
    }
    std::string get_content() {
    	return this->content;
    }
	int get_line() {
		return line;
	}
	int get_col() {
		return col;
	}
	friend std::ostream& operator<<(std::ostream& os, const Token& dt);
};

static Token tokenError	("", ERROR);
static Token tokenEOF	("", EOT);

class TokenStream {
	InputStream* input;
public:
	TokenStream() = delete;
	TokenStream(InputStream* inputStream) {
		this->input = inputStream;
	}
	std::string read_while(int(*predicate)(int));
	Token read_next();
    bool eof() {
    	return input->eof();
    }
};

class ParseStream {
	TokenStream* inputStream;
public:
	ParseStream() = delete;
	ParseStream(TokenStream* inputStream) {
		this->inputStream = inputStream;
	}
	void parse(std::vector<Token>* tokens);
};
