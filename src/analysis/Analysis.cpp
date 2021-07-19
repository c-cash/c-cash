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
            case StatementKind::VARIABLE_CALL:
                checkVaribleCall(stmt);
                break;

            default:
                cout << "here\n";
                break;
        }
    }

    void Analysis::checkVaribleCall(Statement &statement){
        varClass var = *find_if(varibles.cbegin(), varibles.cend(), [&statement](const varClass& element){ return element.name == statement.mName;});
        switch (statement.mStatements[0].mKind) {
            case StatementKind::FUNCTION_CALL:
                break;
            case StatementKind::LITERAL:
                checkVariblesOutOfRange(statement, var);
                break;
            case StatementKind::VARIABLE_CALL:
                checkVariblesOutOfRange(statement, var);
                break;

            default:
                break;
        }
    }

    void Analysis::checkDeclaration(Statement &statement) {
        varClass var;
        if(statement.mKind == StatementKind::MULTIPLE_VARIABLE_DECLARATION){
            for(auto &stmt : statement.mStatements) {
                checkVariblesOutOfRange(stmt, var);
                var.name = stmt.mName;
                var.type = stmt.mType.mType;
                varibles.emplace_back(var);
            }
            return;
        }
        checkVariblesOutOfRange(statement, var);
        var.name = statement.mName;
        var.type = statement.mType.mType;
        varibles.emplace_back(var);
    }

    void  Analysis::checkVariblesOutOfRange(Statement &stmt, varClass &var) {
        double num;
        if(stmt.mKind == StatementKind::VARIABLE_CALL){
            auto it = find_if(varibles.cbegin(), varibles.cend(), [&stmt](const varClass& element){ return element.name == stmt.mName;});
            //Don't find varible
            if(it == varibles.cend()){
                throw runtime_error(string("You don't declare varible - line: ") + to_string(stmt.mLine));
            }
            stmt.mType.mType = it->type;
        }
        //If equal varible call break
        if(stmt.mStatements[0].mKind == StatementKind::VARIABLE_CALL) {
            Statement &statement = stmt.mStatements[0];
            auto it = *find_if(varibles.cbegin(), varibles.cend(), [&statement](const varClass& element){ return element.name == statement.mName;});
            num = it.val;
        } else { num = num; }

        switch (stmt.mType.mType) {
            //int
            case BUILTIN_TYPE::INT32:
                if (num > 2147483648 || num < -2147483648) {
                    throw runtime_error(
                            string("Int can't save number higher than 2147483648 and lower than -2147483648 - line: ") +
                            to_string(stmt.mLine));
                }
                var.val = (int) num;
                break;
                //uint
            case BUILTIN_TYPE::UINT32:
                if (num > 4294967296 || num< 0) {
                    throw runtime_error(
                            string("Uint can't save number higher than 4294967296 and lower than 0 - line: ") +
                            to_string(stmt.mLine));
                }
                var.val = (unsigned int) num;
                break;
                //long
            case BUILTIN_TYPE::INT64:
                if (num > 9.223372e+18 || num < -9.223372e+18) {
                    throw runtime_error(
                            string("Long can't save number higher than 9.223372e+18 and lower than -9.223372e+18 - line: ") +
                            to_string(stmt.mLine));
                }
                var.val = (long long) num;
                break;
                //ulong
            case BUILTIN_TYPE::UINT64:
                if (num > 1.8446744e+19 || num < 0) {
                    throw runtime_error(
                            string("Ulong can't save number higher than 1.8446744e+19 and lower than 0 - line: ") +
                            to_string(stmt.mLine));
                }
                var.val = (unsigned long long) num;
                break;
                //double
            case BUILTIN_TYPE::DOUBLE:
                if (num > 9.223372e+18 || num < -9.223372e+18) {
                    throw runtime_error(
                            string("Double can't save number higher than 9.223372e+18 and lower than -9.223372e+18 - line: ") +
                            to_string(stmt.mLine));
                }
                var.val = (double ) num;
                break;
                //bool
            case BUILTIN_TYPE::BOOL:
                if (num > 1 || num < 0) {
                    throw runtime_error(string("Bool can't save number higher than 1 and lower than 0 - line: ") +
                                        to_string(stmt.mLine));
                }
                var.val = (bool) num;
                break;
                //char
            case BUILTIN_TYPE::INT8:
                if (num > 128 || num < -128) {
                    throw runtime_error(string("Char can't save number higher than 128 and lower than -128 - line: ") +
                                        to_string(stmt.mLine));
                }
                var.val = (char) num;
                break;
                //uchar
            case BUILTIN_TYPE::UINT8:
                if (num > 256 || num < 0) {
                    throw runtime_error(string("Uchar can't save number higher than 256 and lower than 0 - line: ") +
                                        to_string(stmt.mLine));
                }
                var.val = (unsigned  char) num;
                break;

            default:
                break;
        }
    }
}