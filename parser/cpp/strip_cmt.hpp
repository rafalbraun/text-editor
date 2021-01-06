#include <fstream>
#include <iostream>
#include <sstream>  //std::stringstream

std::string strip_comments(std::string str) {
  std::stringstream strStream;
  char quote = 0, comment = 0;

  for (int i = 0; i < str.length(); i++) {
    switch (str.at(i)) {
      /* Take care of control characters starting with '\' */
      case '\\':
        if (comment) /* If we're in a comment, don't bother */
          break;
        strStream << str.at(i);
        strStream << str.at(++i);
        break;

      /* Handle either type of quotes (' or "). */
      case '\"':
      case '\'':
        if (comment) break;
        strStream << str.at(i);
        if (!quote)
          quote = str.at(i);
        else if (quote == str.at(i))
          quote = 0;
        break;

      /* This handles both C++ comments and opening C comments */
      case '/':
        if (quote) strStream << str.at(i);
        else if (str.at(i + 1) == '/')
        {
          while ( (i!= str.length()) && (str.at(i+1)!='\n') )
            i++;
        } else if (str.at(i + 1) == '*')
        {
          comment = 1;
          i++;
        } else if (!comment)
          strStream << str.at(i);
        break;

      /* This handles closing C style comments */
      case '*':
        if (quote) {
          strStream << str.at(i);
          break;
        } else if (comment && str.at(i + 1) == '/')  // buffer[i+1] == '/')
        {
          comment = 0;
          i++;
          break;
        } else if (comment)
          break;
        strStream << str.at(i);
        break;

      /* Anything else?  Show it if necessary. */
      default:
        if (!comment)
          strStream << str.at(i);
        break;
    } /* End of switch */
  }   /* End of for() */

  return strStream.str();
}
/*
int main() {
    std::ifstream inFile;
    inFile.open("sample.c");            //open the input file

    std::stringstream strStream;
    strStream << inFile.rdbuf();        //read the file
    std::string str = strStream.str();  //str holds the content of the file
    std::string out = strip_comments (str);

    std::cout << out << std::endl;
}
*/


