#include "parser.hpp"
#include "strip_cmt.hpp"

#include <set>

// https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
// http://www.cplusplus.com/forum/general/178067/

//static char* preprocessor = " if elif else endif defined ifdef ifndef define undef include line error pragma ";

/**
Różnica pomiędzy callem a deklaracją !!!
    o jeśli mamy deklarację to przed id mamy typ a jeśli mamy call to mamy nic albo znak =

Typy:
    o tylko po słowach kluczowych struct/class/union/enum/typedef


 */

bool _isnotmacro(Token& token) {
    return token.get_type() != MACRO;
}

bool _is_function(std::vector<Token>::iterator iter) {
    if ( (iter->get_type() != IDENTIFICATOR) ) {
        return false;
    }
    iter++;
    if ( iter->get_content() == "(" ) {
        return true;
    }
    return false;
}

std::vector<Token> types;
std::vector<Token> functions;
std::vector<Token> variables;

void parse () {
start:
    return;
end:
    return;
}

int main() {
    std::ifstream inFile;
    //inFile.open("count.c"); 			                //open the input file
    inFile.open("/home/rafal/IdeaProjects/gtksourceview-my-ide/highlight_syntax/highlight_syntax.c");                //open the input file

    std::stringstream strStream;
    strStream << inFile.rdbuf(); 		//read the file
    std::string str = strStream.str(); 	//str holds the content of the file
    std::string out = strip_comments(str);

    InputStream inputStream(out);
    TokenStream tokenStream(&inputStream);
    ParseStream parseStream(&tokenStream);

    std::vector<Token> tokens;
    parseStream.parse(&tokens);

    // just print tokens
    std::copy_if(tokens.begin(), tokens.end(), std::ostream_iterator<Token>(std::cout, "\n"), _isnotmacro);
    


    int block = 0, bracket = 0;;
    std::set<std::string> functionCalls, functionDefs;
    for (auto iter = tokens.begin(); iter != tokens.end(); iter++) {

        if ( (*iter).get_content() == "{" ) {
            block++;
        }
        
        if ( iter->get_content() == "struct" ) {
            iter++;
            std::cout << "\t\t\t struct" << iter->get_content() << std::endl;
            continue;
        }

        if (block > 0) {
            //std::cout << "INSIDE["<< block<<"]:" << *iter << std::endl;
            if (_is_function(iter) ) {
                std::string str = iter->get_content() + " line:" + std::to_string(iter->get_line()) + " col:" + std::to_string(iter->get_col());
                functionCalls.insert(str);
            } else {
                if (iter->get_type() == IDENTIFICATOR) {
                    std::cout << "\t\t\tvariable????" << iter->get_type() << "\t\t" << iter->get_content() << std::endl;
                }
            }
        } else {
            //std::cout << "OUTSIDE["<< block<<"]: " << *iter << std::endl;         
            if (_is_function(iter) ) {
                std::string str = iter->get_content() + " line:" + std::to_string(iter->get_line()) + " col:" + std::to_string(iter->get_col());
                functionDefs.insert(str);
            } else {
                if (iter->get_type() == IDENTIFICATOR) {
                    std::cout << "\t\t\tvariable????" << iter->get_type() << "\t\t" << iter->get_content() << std::endl;
                }
            }
        }

        // !!!!!!!!!!!!!!!!!!!
        // jeśli następny token to ; albo = to mamy variable, jeśli następny token to ; albo { albo variable to deklaracja struct
        // jeśli poprzedni token to struct albo union to mamy nazwę structa



        if ( (*iter).get_content() == "}" ) {
            block--;
        }

    }

    std::cout << "[function defs]" << std::endl;
    std::copy(functionDefs.begin(), functionDefs.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
    std::cout << "[function calls]" << std::endl;
    std::copy(functionCalls.begin(), functionCalls.end(), std::ostream_iterator<std::string>(std::cout, "\n"));


    /*
    std::vector<Token> tmp;
    for (auto iter = tokens.begin(); iter != tokens.end(); iter++) {
        if ( iter->get_type() == IDENTIFICATOR ) {
            while ( iter->get_content() != ";" ) {
                tmp.push_back(*iter);
                iter++;
            }
        }
        std::copy_if(tmp.begin(), tmp.end(), std::ostream_iterator<Token>(std::cout, "\n"), _isnotmacro);
        std::cout << "----------" << std::endl;
        tmp.clear();
    }
    */

}
