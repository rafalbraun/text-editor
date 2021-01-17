#include "parser.hpp"

int _isnotnewline(int ch) {
	return ch!='\n';
}
int _ishash(int ch) {
	return ch=='#';
}
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
	return (strchr("+-*/%=&|!<>", ch) != NULL);
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

int _iskeyword(std::string token) {
	size_t size = sizeof(keywords)/sizeof(keywords[0]);
	for (int i=0; i<size; i++) {
		if (token.compare(keywords[i]) == 0) {
			return 1;
		}
	}
	return 0;
}
int _istype(std::string token) {
	size_t size = sizeof(type)/sizeof(type[0]);
	for (int i=0; i<size; i++) {
		if (token.compare(type[i]) == 0) {
			return 1;
		}
	}
	return 0;
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << "_";
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
			os << "opchar ";
			break;
		case EOT:
			os << "EOT";
			break;
		case ERROR:
			os << "ERROR";
			break;
		case MACRO:
			os << "macro  ";
			break;
    }
    os << " : " << token.content;
    return os;
}

std::string TokenStream::read_while(int(*predicate)(int)) {
    std::string str = "";
    while (!input->eof() && (*predicate)(input->peek()))
        str += input->next();
    return str;
}

Token TokenStream::read_next() {
	read_while(_isblank);	
	if (input->eof()) return tokenEOF;
	char ch = input->peek();
	if (_ishash(ch)) {
		read_while(_isnotnewline);
		return Token("", MACRO, input->get_line(), input->get_col());
	}
	if (_isquote(ch)) {
		input->next();
		std::string str = read_while(_isnotquote);
		input->next();
		return Token("\"" + str + "\"", STRING, input->get_line(), input->get_col());
    }
	if (_issinglequote(ch)) {
		input->next();
		std::string str = read_while(_isnotsinglequote);
		input->next();
		return Token("'" + str + "'", SINGLEQUOTE, input->get_line(), input->get_col());
    }
	if (_isdigit(ch)) {
		std::string str = read_while(_isdigit);
		return Token(str, NUMBER, input->get_line(), input->get_col());
    }
    if (_ispunc(ch)) {
		input->next();
    	return Token(std::string(1, ch), PUNCTUATION, input->get_line(), input->get_col());
    }
    if (_isopchar(ch)) {
		input->next();
    	return Token(std::string(1, ch), OPERATOR, input->get_line(), input->get_col());
    }
    if (_isnotblank(ch)) {
    	std::string str = read_while(_isid);
    	if (_iskeyword(str)) {
    		return Token(str, KEYWORD, input->get_line(), input->get_col());
    	}
    	return Token(str, IDENTIFICATOR, input->get_line(), input->get_col());
    }
	input->croak("Can't handle character: " + ch);
	return tokenError; //"error abcdefghijk";
}

void ParseStream::parse(std::vector<Token>* tokens) {
	while (!inputStream->eof()) {
		//std::cout << inputStream->read_next() << std::endl;
		tokens->push_back(inputStream->read_next());
	}
}
