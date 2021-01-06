#include <fstream>
#include <iostream>
#include <sstream> 	//std::stringstream

#include <functional>
#include <cctype> 	// for std::isspace
#include <cstring> 	// for strchr

#include "strip_cmt.cpp"

// https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
// http://www.cplusplus.com/forum/general/178067/

//static char* preprocessor = " if elif else endif defined ifdef ifndef define undef include line error pragma ";

/**
Różnica pomiędzy callem a deklaracją !!!
	o jeśli mamy deklarację to przed id mamy typ a jeśli mamy call to mamy nic albo znak =

Typy:
	o tylko po słowach kluczowych struct/class/union/enum/typedef


 */
static std::string keywords[] = {
	"auto", "break", "case", "char", "const", "continue", "default", 
	"do", "double", "else", "enum", "extern", "float", "for", "goto", 
	"if", "inline", "int", "long", "register", "restrict", "return", 
	"short", "signed", "sizeof", "static", "struct", "switch", "typedef",
	"union", "unsigned", "void", "volatile", "while"
};

typedef enum TokenType {
	NUMBER,
	STRING,
	KEYWORD,
	SINGLEQUOTE,
	IDENTIFICATOR,
	PUNCTUATION,
	OPERATOR,
	EOT,
	ERROR
} token_t;

class InputStream {
    std::string input;
    int pos = 0, line = 1, col = 0;
    int length;
public:
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
	int get_line() {
		return line;
	}
	int get_col() {
		return col;
	}
};

int _isblank(int ch) {
	return (isblank(ch) || ch=='\n');
}
int _isnotblank(int ch) {
	return !_isblank(ch);
}
int _isquote(int ch) {
	return ch == '"';
}
int _isnotquote(int ch) {
	return !_isquote(ch);
}
int _issinglequote(int ch) {
	return ch == '\'';
}
int _isnotsinglequote(int ch) {
	return !_issinglequote(ch);
}
int _isdigit(int ch) {
	return ((ch>='0') && (ch<='9'));
}
int _isnotdigit(int ch) {
	return !(_isdigit(ch));
}
int _isopchar(int ch) {
	return (strchr("+-*/%=&|!", ch) != NULL);
    //return "+-*/%=&|<>!".indexOf(ch) >= 0;
}
int _isnotopchar(int ch) {
	return !_isopchar(ch);
}
int _ispunc(int ch) {
	return (strchr(",;(){}[]", ch) != NULL);
    //return ",;(){}[]".indexOf(ch) >= 0;
}
int _isnotpunc(int ch) {
	return !_ispunc(ch);
}
int _isid(int ch) {
	return _isnotblank(ch) && _isnotopchar(ch) && _isnotpunc(ch);
}

// https://www.tutorialspoint.com/c_standard_library/c_function_strstr.htm
int _iskeyword(std::string token) {
	size_t size = sizeof(keywords)/sizeof(keywords[0]);
	for (int i=0; i<size; i++) {
		if (token.compare(keywords[i]) == 0) {
			return 1;
		}
	}
	return 0;
}
int _isinclude(std::string token) {
	return (token.compare("#include")==0);
}


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
	int get_line() {
		return line;
	}
	int get_col() {
		return col;
	}
	friend std::ostream& operator<<(std::ostream& os, const Token& dt);
};
/*
std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << "_________";
    switch (token.type) {
		case NUMBER:
			os << "number ";
			break;
		case STRING:
			os << "string ";
			break;
		case KEYWORD:
			os << "keyword";
			break;
		case SINGLEQUOTE:
			os << "squote ";
			break;
		case IDENTIFICATOR:
			os << "ident  ";
			break;
		case PUNCTUATION:
			os << "punc   ";
			break;
		case OPERATOR:
			os << "opchar  ";
			break;
		case EOT:
			os << "EOT";
			break;
		case ERROR:
			os << "ERROR";
			break;
    }
    os << " \t: " << token.content;
    return os;
}*/
static Token tokenError("", ERROR);
static Token tokenEOF("", EOT);

class TokenStream {
	InputStream* input;
public:
	TokenStream() = delete;
	TokenStream(InputStream* inputStream) {
		this->input = inputStream;
	}
	std::string read_while(int(*predicate)(int)) {
	    std::string str = "";
        while (!input->eof() && (*predicate)(input->peek()))
            str += input->next();
        return str;
	}
	Token read_next() {
		read_while(_isblank);
    	if (input->eof()) return tokenEOF;
    	char ch = input->peek();
    	if (_isquote(ch)) {
    		input->next();
    		std::string str = read_while(_isnotquote);
    		input->next();
    		return Token("\"" + str + "\"", STRING);
	    }
    	if (_issinglequote(ch)) {
    		input->next();
    		std::string str = read_while(_isnotsinglequote);
    		input->next();
    		return Token("'" + str + "'", SINGLEQUOTE);
	    }
    	if (_isdigit(ch)) {
    		std::string str = read_while(_isdigit);
    		return Token(str, NUMBER);
	    }
	    if (_ispunc(ch)) {
    		input->next();
	    	return Token(std::string(1, ch), PUNCTUATION);
	    }
	    if (_isopchar(ch)) {
    		input->next();
	    	return Token(std::string(1, ch), OPERATOR);
	    }
	    if (_isnotblank(ch)) {
	    	std::string str = read_while(_isid);
	    	if (_iskeyword(str)) {
	    		Token(str, KEYWORD);
	    	}
	    	return Token(str, IDENTIFICATOR);
	    }
    	input->croak("Can't handle character: " + ch);
    	return tokenError; //"error abcdefghijk";
	}
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
	void parse() {
		while (!inputStream->eof()) {
			std::cout << inputStream->read_next() << std::endl;
		}
	}
};

int main() {
    std::ifstream inFile;
    inFile.open("count.c"); 			//open the input file

    std::stringstream strStream;
    strStream << inFile.rdbuf(); 		//read the file
    std::string str = strStream.str(); 	//str holds the content of the file
	std::string out = strip_comments(str);

    InputStream inputStream(out);
    TokenStream tokenStream(&inputStream);
    ParseStream parseStream(&tokenStream);

    parseStream.parse();

}
