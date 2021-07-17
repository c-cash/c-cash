#include "Analysis.hpp"

namespace analysis {
    void Analysis::analize(map<string, FunctionDefinition> &aFunctions, map<string, ClassDefinition> &aClasses) {
        //For in functions
        for(auto &func : aFunctions){
            for(Statement &stmt : func.second.mStatements){
                checkStatement(stmt);
            }
        }
    }

    void Analysis::checkStatement(Statement &stmt) {
        switch(stmt.mKind) {
            case StatementKind::MULTIPLE_VARIABLE_DECLARATION:
                checkDeclaration(stmt);
                break;
            case StatementKind::VARIABLE_DECLARATION:
                checkDeclaration(stmt);
                break;

            default:
                break;
        }
    }

    void Analysis::checkDeclaration(Statement &statement) {
        if(statement.mKind == StatementKind::MULTIPLE_VARIABLE_DECLARATION){
            for(auto &stmt : statement.mStatements) {
                checkDeclarationChecks(stmt);
            }
            return;
        }
        checkDeclarationChecks(statement);
        return;
    }

    void  Analysis::checkDeclarationChecks(Statement &stmt) {
        switch (stmt.mType.mType) {
            //int
            case BUILTIN_TYPE::INT32:
                if (stoi(stmt.mStatements[0].mName) > 2147483648 || stoi(stmt.mStatements[0].mName) < -2147483648) {
                    throw runtime_error(
                            string("Int can't save number higher than 2147483648 and lower than -2147483648 - line: ") +
                            to_string(stmt.mLine));
                }
                break;
                //uint
            case BUILTIN_TYPE::UINT32:
                if (stoi(stmt.mStatements[0].mName) > 4294967296 || stoi(stmt.mStatements[0].mName) < 0) {
                    throw runtime_error(
                            string("Uint can't save number higher than 4294967296 and lower than 0 - line: ") +
                            to_string(stmt.mLine));
                }
                break;
                //long
            case BUILTIN_TYPE::INT64:
                if (stoi(stmt.mStatements[0].mName) > 9.223372e+18 || stoi(stmt.mStatements[0].mName) < -9.223372e+18) {
                    throw runtime_error(
                            string("Long can't save number higher than 9.223372e+18 and lower than -9.223372e+18 - line: ") +
                            to_string(stmt.mLine));
                }
                break;
                //ulong
            case BUILTIN_TYPE::UINT64:
                if (stoi(stmt.mStatements[0].mName) > 1.8446744e+19 || stoi(stmt.mStatements[0].mName) < 0) {
                    throw runtime_error(
                            string("Ulong can't save number higher than 1.8446744e+19 and lower than 0 - line: ") +
                            to_string(stmt.mLine));
                }
                break;
                //double
            case BUILTIN_TYPE::DOUBLE:
                if (stoi(stmt.mStatements[0].mName) > 9.223372e+18 || stoi(stmt.mStatements[0].mName) < -9.223372e+18) {
                    throw runtime_error(
                            string("Double can't save number higher than 9.223372e+18 and lower than -9.223372e+18 - line: ") +
                            to_string(stmt.mLine));
                }
                break;
                //bool
            case BUILTIN_TYPE::BOOL:
                if (stoi(stmt.mStatements[0].mName) > 1 || stoi(stmt.mStatements[0].mName) < 0) {
                    throw runtime_error(string("Bool can't save number higher than 1 and lower than 0 - line: ") +
                                        to_string(stmt.mLine));
                }
                break;
                //char
            case BUILTIN_TYPE::INT8:
                if (stoi(stmt.mStatements[0].mName) > 128 || stoi(stmt.mStatements[0].mName) < -128) {
                    throw runtime_error(string("Char can't save number higher than 128 and lower than -128 - line: ") +
                                        to_string(stmt.mLine));
                }
                break;
                //uchar
            case BUILTIN_TYPE::UINT8:
                if (stoi(stmt.mStatements[0].mName) > 256 || stoi(stmt.mStatements[0].mName) < 0) {
                    throw runtime_error(string("Uchar can't save number higher than 256 and lower than 0 - line: ") +
                                        to_string(stmt.mLine));
                }
                break;

            default:
                break;
        }
    }
}