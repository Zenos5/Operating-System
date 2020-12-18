#include <iostream>
#include <fstream>
#include <string>
#include "Lexer.h"
#include "DatalogProgram.h"
#include "Database.h"

using namespace std;

int parseIOFiles(int argc, const char* argv[], ifstream& in);

int main(int argc, const char* argv[])
{
    ifstream in(argv[1]);
    parseIOFiles(argc, argv, in);

    Lexer processor = Lexer();
    while (in.peek() != EOF) {
        processor.nextChar(in.get());
        processor.nextPeek(in.peek());
    }
    processor.finalToken();

    DatalogProgram parser = DatalogProgram();
    parser.inputTokenList(processor.outputList());

    parser.parse();
    //cout << parser.toString();

    Database interpreter = Database();
    interpreter.process(parser.getSchemes(), parser.getFacts(), parser.getRules(), parser.getQueries());

    return 0;
}

int parseIOFiles(int argc, const char* argv[], ifstream& in) {
    if (argc < 2) {
        cerr << "Please provide name of input and output files" << endl;
        return 1;
    }
    //cout << "Input file: " << argv[1] << endl;
    if (!in) {
        cerr << "Unable to open " << argv[1] << " for input" << endl;
        return 2;
    }

    return 0;
}