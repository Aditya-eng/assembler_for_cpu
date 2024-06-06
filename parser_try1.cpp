#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>
#include <map>

using namespace std;

int main()
{
    //open the program.hack file or make it
    string filename = "nand2tetris_6/output.hack";

    //open the program.asm file (file with assembly code)
    string asem = "nand2tetris_6/program.asm";


    //create an ofstream object
    ofstream outfile;
    outfile.open(filename);
    if (!outfile) {
        cerr << "Error opening file: " << filename << endl;
        return 1; // Exit with error code
    }
    // outfile << "Hello, world!" << endl;

    // outfile.close();

//////////////Program.asm to be read.////////////
    ifstream infile;

    infile.open(asem);
    // Check if the file was opened successfully
    if (!infile) {
        cerr << "Error opening file: " << asem << endl;
        return 1; // Exit with error code
    }
////////////////////////////////////////////////

    // string regexpt1 = R"(\d+)";
    string regexpt1 = "@";
    regex re1(regexpt1);

    string regex_cmt = "[(]";
    regex re2(regex_cmt);

    vector <int> numbers;

        //// DEST /////
    map <string, string> destable;

    destable["null"] = "000";
    destable["M"] = "001";
    destable["D"] = "010";
    destable["MD"] = "011";
    destable["A"] = "100";
    destable["AM"] = "101";
    destable["AD"] = "110";
    destable["AMD"] = "111";
    /////////////////

    /////COMP //////////
    map <string,string> comptable;

    comptable["0"]   = "0101010";
    comptable["1"]   = "0111111";
    comptable["-1"]  = "0111010";
    comptable["D"]   = "0001100";
    comptable["!D"]  = "0001101";
    comptable["-D"]  = "0001111";
    comptable["D+1"] = "0011111";
    comptable["D-1"] = "0001110";

    comptable["A"]   = "0110000";
    comptable["!A"]  = "0110001";
    comptable["-A"]  = "0110011";
    comptable["A+1"] = "0110111";
    comptable["A-1"] = "0110010";
    comptable["D+A"] = "0000010";
    comptable["D-A"] = "0010011";
    comptable["A-D"] = "0000111";
    comptable["D&A"] = "0000000";
    comptable["D|A"] = "0010101";

    comptable["M"]   = "1110000";
    comptable["!M"]  = "1110001";
    comptable["-M"]  = "1110011";
    comptable["M+1"] = "1110111";
    comptable["M-1"] = "1110010";
    comptable["D+M"] = "1000010";
    comptable["D-M"] = "1010011";
    comptable["M-D"] = "1000111";
    comptable["D&M"] = "1000000";
    comptable["D|M"] = "1010101";

    /////////////////////////

    //////jump////////
    map <string,string> jmptable;

    jmptable["null"] = "000";
    jmptable["JGT"] = "001";
    jmptable["JEQ"] = "010";
    jmptable["JGE"] = "011";
    jmptable["JLT"] = "100";
    jmptable["JNE"] = "101";
    jmptable["JLE"] = "110";
    jmptable["JMP"] = "111";
    ///////////////////



    //Read each line
    string line;
    while (getline(infile,line)){
    
        if(regex_search(line,re1) == true) // A instruction
        {
            cout << "A instruction ";
            line.erase(0,1); //removes '@'
            int n = stoi(line);  //The number after '@'
            string bin = "";
            int i = 0;
            while (n > 0) {
            
                // storing remainder in binary array
                bin.append(to_string(n%2));
                // bin[i] = n % 2;
                n = n / 2;
                i++;
            }
            // cout << bin <<" ";
            int l = bin .length();
            for (int i = 0; i < l / 2; ++i)
            {
                swap(bin[i], bin[l - i - 1]);
            }
            int diff = 16 - l;
            for(int i = 0; i < diff; i++)
            {
                bin.insert(0,"0");
            }
            cout << bin<<endl;
            outfile << bin<<endl; // instead of this just write in output.hack
        
        }
        else if (regex_search(line,re2))
        {
            cout << "Comment" << endl;
        }
        else // C instruction
        {
            // cout << "C instruction"<<endl;
            // return "C_COMMAND";
            string regtillequal = "^[^=]*=";
            regex re3(regtillequal);

            //if statement to check if not null using regex_search
            if (regex_search(line,re3))
            {
                regex pattern(R"([=;])"); // Split by commas with optional surrounding spaces

                // Tokenize the input string based on the pattern
                sregex_token_iterator iter(line.begin(), line.end(), pattern, -1);
                sregex_token_iterator end;

                vector<string> parts; // this stores all the parts
                while (iter != end) {
                    parts.push_back(*iter);
                    ++iter;
                }

                vector <string> machlang = {"1","1","1","","",""};
                
                /// DEST COMPLETE ///////
                // if (destable.find(parts[0]) == destable.end())
                machlang[4] = destable[parts[0]];
                
                // else
                // {
                //     machlang[5] = "000";
                // }
                /////////////////////////

                machlang[3] = comptable[parts[1]];

                if (parts.size() == 3)
                {  
                    machlang[5] = jmptable[parts[2]];
                }
                else if (parts.size() ==2)
                {
                    machlang[5] = "000";
                }

                string resultC;
                for (const auto& elem: machlang)
                {
                    resultC += elem;
                }
                cout << "C instruction "<<resultC<<endl;
                outfile << resultC<<endl;
            }

            else
            {
                regex pattern(R"([=;])"); // Split by commas with optional surrounding spaces

                // Tokenize the input string based on the pattern
                sregex_token_iterator iter(line.begin(), line.end(), pattern, -1);
                sregex_token_iterator end;

                vector<string> parts; // this stores all the parts
                while (iter != end) {
                    parts.push_back(*iter);
                    ++iter;
                }

                vector <string> machlang = {"1","1","1","","",""};

                //dest is always 000
                machlang[4] = "000";

                //comp 
                machlang[3] = comptable[parts[0]];

                //jump
                machlang[5] = jmptable[parts[1]];

                string resultC;
                for (const auto& elem: machlang)
                {
                    resultC += elem;
                }

                cout << "C instruction "<<resultC<<endl;
                outfile << resultC<<endl;


            }
        }
    }
    infile.close();
    // Output the extracted numbers
    cout << endl;
    return 0;
}