#include "quirkFrontend.hpp"
#include "qalamFrontend.hpp"

void Quirk::parseJSON(std::string filename)
{
    
    //helpful regex for parsing this file
    std::regex gateRStr("\"(.+)\",?", std::regex_constants::ECMAScript);
    std::regex emptyRStr("1,?", std::regex_constants::ECMAScript);

    //how many qbits
    size_t numQbits = 0;
    std::vector<std::vector<std::string>> jsonGates;

    std::ifstream quirkInput;
    quirkInput.open(filename);
    if (quirkInput.is_open())
    {
        //////////////////////////////////////////////////////////////////////
        //these characters should be at the start of every quirk input file//
        /////////////////////////////////////////////////////////////////////
        std::string line;
        std::getline(quirkInput, line);
        if(line != "{")
            error("Malformed Quirk JSON input, starting characters invalid");

        std::getline(quirkInput, line);
        if(line != "  \"cols\": [")
            error("Malformed Quirk JSON input, starting characters invalid");
        
        while(true)
        {
            std::getline(quirkInput, line);
            if(line != "    [")
                error("Malformed Quirk JSON input, starting square bracket for column incorrect");
            

            //keep track of which qbit we're on
            size_t index = 0;
            std::vector<std::string> col;

            //loop and extract which gates need to be used for each Qbit in this col
            while(true)
            {
                std::getline(quirkInput, line);

                std::smatch matches;
                //check if this is a gate or empty:
                if(std::regex_search(line, matches, gateRStr))
                {
                    //add gate to IR
                    col.push_back(matches[1]);
                }
                else if(!std::regex_search(line, emptyRStr))
                {
                    std::cout << line << std::endl;
                    error("Malformed Quirk JSON Input, a column has invalid data in it");
                }
                else
                {
                    col.push_back("1");
                }

                index++;

                //do we have to break out of this col?
                //check the very last character, should be ',' otherwise break out
                if(line.back() != ',')
                    break;
            }

            if(numQbits < index)
                numQbits = index;
            
            jsonGates.push_back(col);

            //check if we need to leave the loop (i.e no more cols to process)
            std::getline(quirkInput, line);
            if(line != "    ],")
            {
                if(line != "    ]")
                    error("Malformed Quirk JSON input, ending square bracket for column incorrect");
                else
                    break;
            }
        }

        //////////////////////////////////////////////////////////////////////
        //these characters should be at the end of every quirk input file//
        /////////////////////////////////////////////////////////////////////
        std::getline(quirkInput, line);
        if(line != "  ]")
            error("Malformed Quirk JSON input, end characters invalid");
        
        std::getline(quirkInput, line);
        if(line != "}")
            error("Malformed Quirk JSON input, end characters invalid");
    }


    //////////////////////////////
    //Init all of our qbit lines//
    /////////////////////////////
    for(int i = 0; i < numQbits; i++)
    {
        g_variables.push_back(std::to_string(i), 1);
    }

    
    //////////////////////////
    //Add gates into the IR//
    /////////////////////////
    for(std::vector<std::string> col : jsonGates)
    {
        int index = 0;
        for(std::string gate : col)
        {
            if(gate != "1")
            {
                g_statements.push_back(CircuitExpression(std::to_string(index), gate));
            }
            index++;
        }
    }
}