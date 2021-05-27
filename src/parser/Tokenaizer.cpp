#include "Tokenizer.hpp"
#include <stdexcept>
#include <iostream>

namespace parser {
    using namespace std;

    vector<Token> Tokenaizer::parse(const string &filename){
        vector<Token> tokens;
        Token currentToken;

        currentToken.mLine = 1;

        for(char currCh : filename){
            if(currentToken.mType == STRING_ESCAPE_SEQUENCE){
                switch (currCh) {
                    case 'n':
                        currentToken.mText.append(1, '\n');
                        break;
                    case 'r':
                        currentToken.mText.append(1, '\r');
                        break;
                    case 't':
                        currentToken.mText.append(1, '\t');
                        break;
                    case '\\':
                        currentToken.mText.append(1, '\\');
                        break;
                    
                    default:
                        throw runtime_error(string("unknow sentence") + string(1, currCh) + " in string on line " + to_string(currentToken.mLine));
                        break;
                }
                currentToken.mType = STRING_LITERAL;
                continue;
            }
            
            switch (currCh) {
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    if(currentToken.mType == POTENTIAL_SPECIFIC_OPERATOR) {
                        currentToken.mType = OPERATOR;
                        endToken(currentToken, tokens);
                        currentToken.mType = INTEGER_LITERAL;
                        currentToken.mText.append(1, currCh);
                    } else if(currentToken.mType == WHITESPACE){
                        currentToken.mType = INTEGER_LITERAL;
                        currentToken.mText.append(1, currCh);
                    } else if(currentToken.mType == POTENTIAL_DOUBLE){
                        currentToken.mType = DOUBLE_LITERAL;
                        currentToken.mText.append(1, currCh);
                    } else if(currentToken.mType == OPERATOR){
                        endToken(currentToken, tokens);
                        currentToken.mType = INTEGER_LITERAL;
                        currentToken.mText.append(1, currCh);
                    } else if(currentToken.mType == LOGIC){
                        endToken(currentToken, tokens);
                        currentToken.mType = INTEGER_LITERAL;
                        currentToken.mText.append(1, currCh);
                    } else {
                        currentToken.mText.append(1, currCh);
                    }
                    break;
                
                case '.':
                    if(currentToken.mType == WHITESPACE){
                        currentToken.mType = POTENTIAL_DOUBLE;
                        currentToken.mText.append(1, currCh);
                    } else if(currentToken.mType == INTEGER_LITERAL){
                        currentToken.mType = DOUBLE_LITERAL;
                        currentToken.mText.append(1, currCh);
                    } else if(currentToken.mType == STRING_LITERAL || currentToken.mType == COMMENT || currentToken.mType == BLOCK_COMMENT){
                        currentToken.mText.append(1, currCh);
                    } else {
                        endToken(currentToken, tokens);
                        currentToken.mType = OPERATOR;
                        currentToken.mText.append(1, currCh);
                        endToken(currentToken, tokens);
                    }
                    break;

                case '{':
                case '}':
                case '(':
                case ')':
                case ';':
                case ',':
                case '[':
                case ']':
                    if(currentToken.mType == POTENTIAL_SPECIFIC_OPERATOR) {
                        currentToken.mType = OPERATOR;
                        endToken(currentToken, tokens);
                        currentToken.mType = OPERATOR;
                        currentToken.mText.append(1, currCh);
                        endToken(currentToken, tokens);
                    } else if(currentToken.mType != STRING_LITERAL && currentToken.mType != COMMENT && currentToken.mType != BLOCK_COMMENT){
                        endToken(currentToken, tokens);
                        currentToken.mType = OPERATOR;
                        currentToken.mText.append(1, currCh);
                        endToken(currentToken, tokens);
                    } else {
                        currentToken.mText.append(1, currCh);
                    }
                    break;

                case '+':
                case '-':
                    if(currentToken.mType == POTENTIAL_SPECIFIC_OPERATOR) {
                        currentToken.mType = INC_DEC_OPERATOR;
                        currentToken.mText.append(1, currCh);
                        endToken(currentToken, tokens);
                    } else if(currentToken.mType != STRING_LITERAL && currentToken.mType != COMMENT && currentToken.mType != BLOCK_COMMENT){
                        endToken(currentToken, tokens);
                        currentToken.mType = POTENTIAL_SPECIFIC_OPERATOR;
                        currentToken.mText.append(1, currCh);
                    } else {
                        currentToken.mText.append(1, currCh);
                    }
                    break;

                case '*':
                case '/':
                case '%':
                    if(currentToken.mType != STRING_LITERAL && currentToken.mType != COMMENT && currentToken.mType != BLOCK_COMMENT){
                        endToken(currentToken, tokens);
                        currentToken.mType = POTENTIAL_SPECIFIC_OPERATOR;
                        currentToken.mText.append(1, currCh);
                    } else {
                        currentToken.mText.append(1, currCh);
                    }
                    break;

                case '=':
                    if(currentToken.mType == LOGIC || currentToken.mType == OPERATOR){
                        currentToken.mType = LOGIC;
                        currentToken.mText.append(1, currCh);
                        endToken(currentToken, tokens);
                    } else if(currentToken.mType == POTENTIAL_SPECIFIC_OPERATOR) {
                        currentToken.mType = COMPOUND_ASSIGNMENT_OPERATOR;
                        currentToken.mText.append(1, currCh);
                        endToken(currentToken, tokens);
                    } else if(currentToken.mType != STRING_LITERAL && currentToken.mType != COMMENT && currentToken.mType != BLOCK_COMMENT) {
                        endToken(currentToken, tokens);
                        currentToken.mType = OPERATOR;
                        currentToken.mText.append(1, currCh);
                    }  else {
                        currentToken.mText.append(1, currCh);
                    }
                    break;
                
                case '>':
                case '<':
                case '!':
                    if(currentToken.mType != STRING_LITERAL && currentToken.mType != COMMENT && currentToken.mType != BLOCK_COMMENT){
                        endToken(currentToken, tokens);
                        currentToken.mType = LOGIC;
                        currentToken.mText.append(1, currCh);
                    } else {
                        currentToken.mText.append(1, currCh);
                    }
                    break;
                
                case ' ':
                case '\t':
                    if(currentToken.mType == STRING_LITERAL || currentToken.mType == COMMENT || currentToken.mType == BLOCK_COMMENT){
                        currentToken.mText.append(1, currCh);
                    } else if(currentToken.mType == POTENTIAL_SPECIFIC_OPERATOR) {
                        currentToken.mType = OPERATOR;
                        endToken(currentToken, tokens);
                    } else {
                        endToken(currentToken, tokens);
                    }
                    break;
                case '\r':
                case '\n':
                    if(currentToken.mType == POTENTIAL_SPECIFIC_OPERATOR) {
                        currentToken.mType = OPERATOR;
                        endToken(currentToken, tokens);
                    } else if(currentToken.mType != BLOCK_COMMENT) {
                        endToken(currentToken, tokens);
                    } else {
                        currentToken.mText.append(1, currCh);
                    }         
                    ++currentToken.mLine;
                    break;
                
                case '"':
                    if(currentToken.mType != STRING_LITERAL && currentToken.mType != COMMENT && currentToken.mType != BLOCK_COMMENT){
                        endToken(currentToken, tokens);
                        currentToken.mType = STRING_LITERAL;
                    } else if(currentToken.mType == STRING_LITERAL){
                        endToken(currentToken, tokens);
                    }
                    break;
                
                case '\\':
                    if(currentToken.mType == STRING_LITERAL){
                        currentToken.mType = STRING_ESCAPE_SEQUENCE;
                    } else if(currentToken.mType == POTENTIAL_SPECIFIC_OPERATOR) {
                        currentToken.mType = OPERATOR;
                        endToken(currentToken, tokens);
                        currentToken.mType = OPERATOR;
                        currentToken.mText.append(1, currCh);
                        endToken(currentToken, tokens);
                    } else if(currentToken.mType == BLOCK_COMMENT || currentToken.mType == COMMENT) {
                        currentToken.mText.append(1, currCh);
                    } else {
                        endToken(currentToken, tokens);
                        currentToken.mType = OPERATOR;
                        currentToken.mText.append(1, currCh);
                        endToken(currentToken, tokens);
                    }
                    break;
                
                case '#':
                    if(currentToken.mType == STRING_LITERAL || currentToken.mType == BLOCK_COMMENT){
                        currentToken.mText.append(1, currCh);
                    } else if(currentToken.mType == POTENTIAL_SPECIFIC_OPERATOR) {
                        currentToken.mType = OPERATOR;
                        endToken(currentToken, tokens);
                        currentToken.mType = COMMENT;;
                        currentToken.mText.append(1, currCh);
                    } else {
                        endToken(currentToken, tokens);
                        currentToken.mType = COMMENT;
                        currentToken.mText.append(1, currCh);
                    }
                    break;

                case '|':
                    if(currentToken.mType == STRING_LITERAL) {
                        currentToken.mText.append(1, currCh);
                    } else if(currentToken.mType == BLOCK_COMMENT) {
                        currentToken.mText.append(1, currCh);
                        endToken(currentToken, tokens);
                        currentToken.mType = WHITESPACE;
                    } else if(currentToken.mType == POTENTIAL_SPECIFIC_OPERATOR) {
                        currentToken.mType = OPERATOR;
                        endToken(currentToken, tokens);
                        currentToken.mType = BLOCK_COMMENT;
                        currentToken.mText.append(1, currCh);
                    } else  {
                        endToken(currentToken, tokens);
                        currentToken.mType = BLOCK_COMMENT;
                        currentToken.mText.append(1, currCh);
                    }
                    break;

                default:
                    if(currentToken.mType == WHITESPACE || currentToken.mType == INTEGER_LITERAL || currentToken.mType == DOUBLE_LITERAL || currentToken.mType == OPERATOR || currentToken.mType == LOGIC) {
                        endToken(currentToken, tokens);
                        currentToken.mType = IDENTIFIER;
                        currentToken.mText.append(1, currCh);
                    } else if(currentToken.mType == POTENTIAL_SPECIFIC_OPERATOR) {
                        currentToken.mType = OPERATOR;
                        endToken(currentToken, tokens);
                        currentToken.mType = IDENTIFIER;
                        currentToken.mText.append(1, currCh);
                    } else {
                        currentToken.mText.append(1, currCh);
                    }
                    break;
            }
        }
        endToken(currentToken, tokens);

        return tokens;
    }

    void Tokenaizer::endToken(Token &token, vector<Token> &tokens){
        if (token.mType != WHITESPACE && (token.mType != COMMENT && token.mType != BLOCK_COMMENT)){
            tokens.push_back(token);
        }
        if(token.mType == POTENTIAL_DOUBLE) {
            if(token.mText.compare(".") == 0) {
                token.mType = OPERATOR; 
            }
            else {
                token.mType = DOUBLE_LITERAL;
            }
        }
        token.mType = WHITESPACE;
        token.mText.erase();
    }

    void Token::DebugPrint() const{
        cout << "\u001B[33mToken \u001B[0m(\u001B[36m" << sTokenTypeStrings[mType] << "\u001B[0m,\u001B[36m \"" << mText << "\"\u001B[0m,\u001B[36m " << mLine << "\u001B[0m)" << endl;
    }
}
