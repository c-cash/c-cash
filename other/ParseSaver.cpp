#include "ParseSaver.hpp"
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <iomanip>

#include "../parser/Statement.hpp"
#include "../parser/Parser.hpp"

namespace parsesaver {
    using namespace std;

    enum class Special {
        STATEMENT_BEGIN = 0x10,
        CONTENT_BEGIN = 0x0,
        CONTENT_END = 0x1
    };

    static map<parser::StatementKind, char> statementMap {
        {parser::StatementKind::FUNCTION_CALL, 0x11},
        {parser::StatementKind::LITERAL, 0x100},
        {parser::StatementKind::LOGIC_CALL, 0x101},
        {parser::StatementKind::OPERATOR_CALL, 0x110},
        {parser::StatementKind::VARIABLE, 0x111},
        {parser::StatementKind::VARIABLE_CALL, 0x1000},
        {parser::StatementKind::VARIABLE_DECLARATION, 0x1001}
    };

    static map<string, char> typeMap {
        {"func", 0x11},
        {"signed int", 0x100},
        {"signed integer", 0x100}, // unnecessary
        {"unsigned int", 0x101},
        {"unsigned integer", 0x101}, // unnecessary
        {"signed char", 0x110},
        {"unsigned char", 0x111},
        {"double", 0x1000},
        {"string", 0x1001},
        {"func_param", 0x1010}
    };

    void ParseSaver::save(string path, map<string, parser::FunctionDefinition> &functions) {
        fout.open(path, ios::binary | ios::app); // open file in append mode

        // do stuff
        for (auto &f : functions) {
            fout << f.first; // function name
            for(parser::Statement &s : f.second.mStatements) {
                saveStatement(s);
            }
            fout << '\n'; // each function is a line
        }
        // do stuff

        fout.close(); // close file
    }

    void ParseSaver::saveStatement(parser::Statement &stmt) {
        fout << (char) Special::STATEMENT_BEGIN << toBinaryType(stmt) << toBinarySubtype(stmt) << getStatementName(stmt); // statement basic data
        fout << (char) Special::CONTENT_BEGIN; // statement content begin
        // statement content
        for (parser::Statement &s : stmt.mStatements) {
            saveStatement(s);
        }
        fout << (char) Special::CONTENT_END; // statement end
    }

    char ParseSaver::toBinaryType(parser::Statement &stmt) {
        return (char) statementMap[stmt.mKind];
    }
    char ParseSaver::toBinarySubtype(parser::Statement &stmt) {
        if (typeMap.count(stmt.mType.mName) == 0) throw "No Statement Type Error: " + stmt.mType.mName;
        return typeMap[stmt.mType.mName];
    }
    string ParseSaver::getStatementName(parser::Statement &stmt) {
        if (stmt.mName == "") return "-";
        return escape(stmt.mName);
    }


    string ParseSaver::escape(string v) {
        string result = "";
        for (int i=0; i<v.size(); i++) {
            if (v[i] == '\n') {
                result += "\\n";
            } else {
                result += v[i];
            }
        }
        return result;
    }  
    string ParseSaver::unescape(string v) {
        return v;
    }

}