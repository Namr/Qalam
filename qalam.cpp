#include <iostream>
#include <fstream>
#include <string>

void error(std::string error)
{
    std::cerr << "ERROR: " << error << std::endl;
    exit(1);
}

int definition_statements(std::string& line)
{
    //DEFSTATEMENT -> identifier DEFINITION

}

int main()
{
    std::ifstream qalamInput;
    qalamInput.open("filename.qlm");
    if (qalamInput.is_open())
    {
        std::string line;
        std::getline(qalamInput, line);

        //start -> "given:" {DEFSTATEMENT} "circut:" {STATEMENT}
        if(line != "given:")
            error("no 'given:' section of program");

        while(line != "circut:")
        {
            std::getline(qalamInput, line);
            definition_statements(line);
        }

    }
    else
    {
        error("can't open input file");
    }

    qalamInput.close();
}