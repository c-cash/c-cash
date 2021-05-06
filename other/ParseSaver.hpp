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

        private:
            ofstream fout;

            void saveStatement(parser::Statement &stmt);

            char toBinaryType(parser::Statement &stmt);
            char toBinarySubtype(parser::Statement &stmt);
            string getStatementName(parser::Statement &stmt);

            string escape(string v);
            string unescape(string v);

    };
}