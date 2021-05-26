#pragma once

#include <fstream>
#include <vector>
#include "../parser/Statement.hpp"
#include "../parser/Parser.hpp"

namespace parsesaver {
    using namespace std;

    class ParseSaver {
        public:
            ParseSaver() {};

            void save(string path, map<string, parser::FunctionDefinition> &functions);
            void writeHelp();
            map<string, parser::FunctionDefinition> load(string path);

        private:
            ofstream fout;
            map<string, string> varmap;
            int clen;

            void saveStatement(parser::Statement &stmt);

            char toBinaryType(parser::Statement &stmt);
            char toBinaryType(parser::BUILTIN_TYPE type);
            char toBinarySubtype(parser::Statement &stmt);
            string getStatementName(parser::Statement &stmt);
            string getVariableName(string name, bool minimize);

            string getNextName(string current);

            pair<string, parser::FunctionDefinition> loadFunction(vector<char> &line);
            void loadStatement(vector<char> &line, int &i, parser::Statement* &current, vector<parser::Statement> &statements);

            string escape(string v);
            string unescape(string v);
    };
}