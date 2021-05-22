#include "./Transpiler.hpp"

#include <iostream>
#include <fstream>

namespace transpiler {

    using namespace std;

    map<string, string> specialCases {
        {"return", "return {0}"},
        {"write", "cout << {.<<.}"},
        {"read", "cin >> {.>>.}"},
        {"IF", "if ({0}) {\n{.;.};\n}"}
    };

    map<string, string> connectorMap {
        {"{.<<.}", " << "},
        {"{.>>.}", " >> "},
        {"{.+.}", " + "},
        {"{.;.}", ";"}
    };

    void Transpiler::transpile(string path, map<string, FunctionDefinition> &mFunctions) {
        ofstream fout;
        fout.open(path);

        fout << "#include <iostream>\nusing namespace std;\n\n";

        for (pair<string, FunctionDefinition> func : mFunctions) {
            string returnType = discoverType(func.second);
            string params;
            // function declaration
            int size = func.second.mParameters.size();
            for (int i=0; i<size; i++) {
                ParameterDefinition param = func.second.mParameters[i];
                params += param.mType.mName + " " + param.mName + (i == size-1 ? "" : ", ");
            }
            fout << returnType << " " << func.second.mName << "(" << params << ") {\n";

            // statements
            for (Statement stmt : func.second.mStatements) {
                fout << transpileStatement(stmt) + ";\n";
            }

            // close function
            fout << "}\n";
        }
        fout.close();
    }

    string Transpiler::transpileStatement(Statement &stmt) {
        // LITTERAL
        if (stmt.mKind == StatementKind::LITERAL) {
            return transpileLitteral(stmt);
        }
        // VARIABLE DECLARATION
        if (stmt.mKind == StatementKind::VARIABLE_DECLARATION) {
            return transpileVariableDeclaration(stmt);
        }
        // LOGIC CALL
        if (stmt.mKind == StatementKind::LOGIC_CALL) {
            return "(" + transpileOperator(stmt) + ")";
        }
        // OPERATOR CALL
        if (stmt.mKind == StatementKind::OPERATOR_CALL) {
            return transpileOperator(stmt);
        }
        // VARIABLE CALL
        if (stmt.mKind == StatementKind::VARIABLE_CALL) {
            return stmt.mName;
        }
        // FUNCTION CALL
        if (stmt.mKind == StatementKind::FUNCTION_CALL) {
            return transpileFunctionCall(stmt);
        }
        return "";
    }

    string Transpiler::transpileLoop(Statement &stmt) {
        return "TODO: implement loops";
    }


    string Transpiler::transpileVariableDeclaration(Statement &stmt) {
        return stmt.mType.mName + " " + stmt.mName + " = " + transpileStatement(stmt.mStatements[0]) + "";
    }

    string Transpiler::transpileOperator(Statement &stmt) {
        vector<string> temp;
        for (Statement s : stmt.mStatements) {
            temp.emplace_back(transpileStatement(s));
        }
        return Transpiler::connect<string>(temp, stmt.mName);
    }

    string Transpiler::transpileLitteral(Statement &stmt) {
        if (stmt.mType.mName == "string") {
            return "\"" + stmt.mName + "\"";
        } else if (stmt.mType.mName == "char") {
            return "'" + stmt.mName + "'";
        } else {
            return stmt.mName;
        }
    }

    string Transpiler::transpileFunctionCall(Statement &stmt) {
        if (stmt.mName == "LOOP") { // this function is a loop :D
            return transpileLoop(stmt);
        } else if (specialCases.find(stmt.mName) != end(specialCases)) { // special case
            string special = specialCases[stmt.mName];
            int i=0;
            // normal values
            vector<string> args;
            for (Statement arg : stmt.mStatements) {
                string t = transpileStatement(arg);
                if (Transpiler::replace(special, "{" + to_string(i) + "}", t) == 0) {
                    args.emplace_back(t);
                }
                i++;
            }
            // connected values
            for (pair<string, string> c : connectorMap) {
                Transpiler::replace(special, c.first, Transpiler::connect<string>(args, c.second));
            }
            return special;
        } else { // normal case
            string result = stmt.mName + "(";
            int i=0;
            int size = stmt.mStatements.size();
            for (Statement arg : stmt.mStatements) {
                result += transpileStatement(arg) + (i == size-1 ? "" : ", ");
                i++;
            }
            return result + ")";
        }
    }

    string Transpiler::discoverType(FunctionDefinition &func) {
        if (func.mName == "main") return "int";
        return "???";
    }

    int Transpiler::replace(string &s, string from, string to) {
        size_t position = s.find(from);
        int changed = 0;

        while(position != string::npos) {
            s.replace(position, from.size(), to);
            changed++;
            position = s.find(from, position + to.size());
        }
        return changed;
    }

    template<typename T>
    string Transpiler::connect(vector<T> &v, string c) {
        string r;
        int size = v.size();
        for (int i=0; i<size; i++) {
            r += v[i] + (i == size-1 ? "" : c);
        }
        return r;
    }
    
}
