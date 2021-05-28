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
        STATEMENT_BEGIN = 0b10,
        CONTENT_BEGIN = 0b0,
        CONTENT_END = 0b1,
        PARAMETERS_START = 0b11,
        PARAMETERS_END = 0b100,
        PARAMETERS_NEXT = 0b101
    };


    /*
        ARRAY_DECLARATION,
        ARRAY,
        ARRAY_ELEMENT,
        ARRAY_CALL
    */
    map<parser::StatementKind, char> statementMap {
        {parser::StatementKind::FUNCTION_CALL, 0b110},
        {parser::StatementKind::LITERAL, 0b111},
        {parser::StatementKind::LOGIC_CALL, 0b1000},
        {parser::StatementKind::OPERATOR_CALL, 0b1001},
        {parser::StatementKind::VARIABLE, 0b1011}, // skip new line (0x1010)
        {parser::StatementKind::VARIABLE_CALL, 0b1100},
        {parser::StatementKind::VARIABLE_DECLARATION, 0b1101},
        {parser::StatementKind::ARRAY_DECLARATION, 0b1110},
        {parser::StatementKind::ARRAY, 0b1111},
        {parser::StatementKind::ARRAY_ELEMENT, 0b10000},
        {parser::StatementKind::ARRAY_CALL, 0b10001},
        {parser::StatementKind::NAMESPACE, 0b10010}
    };
    map<char, parser::StatementKind> inverseStatementMap {
        {0b110, parser::StatementKind::FUNCTION_CALL},
        {0b111, parser::StatementKind::LITERAL},
        {0b1000, parser::StatementKind::LOGIC_CALL},
        {0b1001, parser::StatementKind::OPERATOR_CALL},
        {0b1011, parser::StatementKind::VARIABLE}, // skip new line (0x1010)
        {0b1100, parser::StatementKind::VARIABLE_CALL},
        {0b1101, parser::StatementKind::VARIABLE_DECLARATION},
        {0b1110, parser::StatementKind::ARRAY_DECLARATION},
        {0b1111, parser::StatementKind::ARRAY},
        {0b10000, parser::StatementKind::ARRAY_ELEMENT},
        {0b10001, parser::StatementKind::ARRAY_CALL},
        {0b10010, parser::StatementKind::NAMESPACE}
    };

    map<parser::BUILTIN_TYPE, char> typeMap {
        {parser::BUILTIN_TYPE::FUNC, 0b110},
        {parser::BUILTIN_TYPE::INT32, 0b111},
        {parser::BUILTIN_TYPE::UINT32, 0b1000},
        {parser::BUILTIN_TYPE::INT8, 0b1001},
        {parser::BUILTIN_TYPE::UINT8, 0b1011}, // skip new line (0x1010)
        {parser::BUILTIN_TYPE::DOUBLE, 0b1100},
        {parser::BUILTIN_TYPE::STRING, 0b1101},
        {parser::BUILTIN_TYPE::FUNC_PARAM, 0b1110}
    };
    map<char, pair<string, parser::BUILTIN_TYPE>> inverseTypes {
        {0b110, {"func", parser::BUILTIN_TYPE::FUNC}},
        {0b111, {"signed integer", parser::BUILTIN_TYPE::INT32}},
        {0b1000, {"unsigned integer", parser::BUILTIN_TYPE::UINT32}},
        {0b1001, {"signed char", parser::BUILTIN_TYPE::INT8}},
        {0b1011, {"unsigned char", parser::BUILTIN_TYPE::UINT8}}, // skip new line (0x1010)
        {0b1100, {"double", parser::BUILTIN_TYPE::DOUBLE}},
        {0b1101, {"string", parser::BUILTIN_TYPE::STRING}},
        {0b1110, {"func_param", parser::BUILTIN_TYPE::FUNC_PARAM}}
    };

    string varnamechars = "abcdefghijklmoprstuvwxyzABCDEFGHIJKLMNOPRSTUVWXYZ";

    vector<string> staticnames = {
        "main", "write", "read", "IF", "ELSE", "LOOP", "ELIF", "break", "continue"
    };

    // SAVE
    void ParseSaver::save(string path, map<string, parser::FunctionDefinition> &functions) {

        clen = 1;
        varmap.clear();
        for(string s : staticnames) {
            varmap[s] = s;
        }

        fout.open(path, ios::binary | ios::app); // open file in append mode

        fout << "#!/parsed\n"; // parsed file indicator

        for (auto &f : functions) {
            fout << getNextName(f.first); // function name
            fout << (char) Special::PARAMETERS_START;
            for(parser::ParameterDefinition &p : f.second.mParameters) {
                fout << toBinaryType(p.mType.mType) << getVariableName(p.mName, true) << (char) Special::PARAMETERS_NEXT;
            }
            fout << (char) Special::PARAMETERS_END;
            for(parser::Statement &s : f.second.mStatements) {
                saveStatement(s);
            }
            fout << '\n'; // each function is a line
        }
        cout << "written " << fout.tellp() << " bytes to the file" << endl;
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
    char ParseSaver::toBinaryType(parser::BUILTIN_TYPE type) {
        if (typeMap.count(type) == 0) throw "No Statement Type Error: " + type;
        return typeMap[type];
    }
    char ParseSaver::toBinarySubtype(parser::Statement &stmt) {
        if (typeMap.count(stmt.mType.mType) == 0) throw "No Statement Type Error: " + stmt.mType.mName;
        return typeMap[stmt.mType.mType];
    }
    string ParseSaver::getStatementName(parser::Statement &stmt) {
        if (stmt.mName == "") return "-";
        if (stmt.mKind==parser::StatementKind::VARIABLE_DECLARATION ||
            stmt.mKind==parser::StatementKind::VARIABLE_CALL ||
            stmt.mKind==parser::StatementKind::FUNCTION_CALL) {
                return getNextName(stmt.mName);
            }
        return escape(stmt.mName);
    }
    string ParseSaver::getVariableName(string name, bool minimize) {
        if (name == "") return "-";
        if (minimize) return getNextName(name);
        return escape(name);
    }

    string ParseSaver::getNextName(string current) {
        return current;
        // TEMPORARY DISABLED DUE TO THE BIG CHANGES
        map<string, string>::iterator index = varmap.find(current);
        if (index == varmap.end()) {
            // generate new name
            string newName;
            int value = clen;
            int size = varnamechars.size();
            while(value > 0) {
                unsigned int remainder = value % size;
                value /= size;
                newName += varnamechars[remainder];
            }
            varmap[current] = newName;
            clen++;
            return newName;
        } else {
            return index->second;
        }
    }

    // LOAD
    map<string, parser::FunctionDefinition> ParseSaver::load(string path) {
        ifstream file(path, ios::binary);
        vector<char> bytes((istreambuf_iterator<char>(file)), (istreambuf_iterator<char>()));
        map<string, parser::FunctionDefinition> functions;
        vector<char> line;
        bool isFirst = true;
        for(char c : bytes) {
            if (c == '\n') {
                if (isFirst) {isFirst = false; line.clear(); continue;}
                functions.insert(loadFunction(line));
                line.clear();
            } else {
                line.push_back(c);
            }
        }
        return functions;
    }

    pair<string, parser::FunctionDefinition> ParseSaver::loadFunction(vector<char> &line) {
        parser::FunctionDefinition func;
        string name;
        // parse name
        for(int i=0; i<line.size(); i++) {
            if (line[i] == (char) Special::PARAMETERS_START) break;
            name += line[i];
        }
        func.mName = name;

        // parse parameters
        int i;
        for (i=name.size()+1; i<line.size(); i++) {
            if (line[i] == (char) Special::PARAMETERS_END) break;
            parser::ParameterDefinition param;
            pair<string, parser::BUILTIN_TYPE> temp = inverseTypes[line[i]];
            param.mType = parser::Type(temp.first, temp.second);
            i++;
            while(line[i] != (char) Special::PARAMETERS_NEXT) {
                param.mName += line[i];
                i++;
            }
            func.mParameters.push_back(param);
        }

        // parse statements
        parser::Statement* current = nullptr;

        for(;i<line.size(); i++)
            loadStatement(line, i, current, func.mStatements);

        return {name, func};
    }

    void ParseSaver::loadStatement(vector<char> &line, int &i, parser::Statement* &current, vector<parser::Statement> &statements) {
        if (line[i] == (char) Special::STATEMENT_BEGIN) {
            parser::Statement* stmt = new parser::Statement();
            current = stmt;
            stmt->mKind = inverseStatementMap[line[i+1]];
            pair<string, parser::BUILTIN_TYPE> temp = inverseTypes[line[i+2]];
            stmt->mType = parser::Type(temp.first, temp.second);
            string name;
            i += 3;
            while(line[i] != (char) Special::CONTENT_BEGIN) {
                name += line[i++];
            }
            stmt->mName = unescape(name);
            i++;
            while(line[i] != (char) Special::CONTENT_END) {
                loadStatement(line, i, current, stmt->mStatements);
                i++;
                current = stmt;
            }
            statements.push_back(*stmt);
        }
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
        string result = "";
        int size = v.size();
        for (int i=0; i<v.size(); i++) {
            if (v[i] == '\\' && (i+1 < size && v[i+1] == 'n')) {
                result += "\n";
                i++;
            } else {
                result += v[i];
            }
        }
        return result;
    }

    void ParseSaver::writeHelp() {
        cout << "-H --> shows this\n";
        cout << "-D --> shows a tree of program\n";
        cout << "-E <name> --> makes name.ccc file with ccash code compiled to program tree saved in binary\n";
        cout << "-E --> makes c.ccc file with ccash code compiled to program tree saved in binary\n";
        cout << "You can see documentation on: <website>\n";
    }

}