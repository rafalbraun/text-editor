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

int main() {
    std::ifstream inFile;
    //inFile.open("count.c"); 			                //open the input file
    inFile.open("/home/rafal/IdeaProjects/gtksourceview-my-ide/highlight_syntax/highlight_syntax.c");                //open the input file

    //"/home/rafal/IdeaProjects/gtksourceview-my-ide/highlight_syntax/highlight_syntax.c"

    std::stringstream strStream;
    strStream << inFile.rdbuf(); 		//read the file
    std::string str = strStream.str(); 	//str holds the content of the file
    std::string out = strip_comments(str);

    InputStream inputStream(out);
    TokenStream tokenStream(&inputStream);
    ParseStream parseStream(&tokenStream);

    std::vector<Token> tokens;
    parseStream.parse(&tokens);
    
    std::copy_if(tokens.begin(), tokens.end(), std::ostream_iterator<Token>(std::cout, "\n"), _isnotmacro);
    

    std::cout << "[function calls]" << std::endl;

    int block = 0, bracket = 0;;
    std::set<std::string> functionCalls, functionDefs;
    for (auto iter = tokens.begin(); iter != tokens.end(); iter++) {

        if ( (*iter).get_content() == "{" ) {
            block++;
        }

        if (block > 0) {
            std::cout << "INSIDE["<< block<<"]:" << *iter << std::endl;
            if (_is_function(iter) ) {
                functionCalls.insert(iter->get_content());
            }
        } else {
            std::cout << "OUTSIDE["<< block<<"]: " << *iter << std::endl;         
            if (_is_function(iter) ) {
                functionDefs.insert(iter->get_content());
            }
        }

        if ( (*iter).get_content() == "}" ) {
            block--;
        }

        /*
        if (_is_function(iter) ) {
            //std::cout << *iter << " ";
            functionCalls.insert(iter->get_content());
            //std::cout << std::endl;
        }*/

        /*
        std::cout << *iter << " ";
        std::cout << _is_function(iter);
        std::cout << std::endl;
        */


















        /*
        if ( (*iter).get_type() == IDENTIFICATOR ) {
            std::cout << " \t\t---> maybe call or fdef or fdec or typename or varname " << block;
        }*/
    }

    std::cout << "============ functionDefs" << std::endl;
    std::copy(functionDefs.begin(), functionDefs.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
    std::cout << "============ functionCalls" << std::endl;
    std::copy(functionCalls.begin(), functionCalls.end(), std::ostream_iterator<std::string>(std::cout, "\n"));

    /*
    int block = 0, bracket = 0;;
    for (auto iter = tokens.rbegin(); iter != tokens.rend(); iter++) {
        std::cout << *iter;

        if ( (*iter).get_content() == "}" ) {
            block = 1;
        }
        if ( (*iter).get_content() == "{" ) {
            block = 0;
        }
        if ( (*iter).get_content() == ")" ) {
            bracket = 0;
        }
        if ( (*iter).get_content() == "(" ) {
            bracket = 1;
        }

        if ( (*iter).get_type() == IDENTIFICATOR ) {
            std::cout << " \t\t---> maybe call or fdef or fdec or typename or varname " << block << " " << bracket;
            bracket = 0;
        }
        if ( (*iter).get_type() == KEYWORD ) {
            bracket = 0;
        }
        std::cout << std::endl;

    }*/


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
