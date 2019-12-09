#include <iostream>
#include <fstream>
#include <locale>         // std::locale, std::toupper

using namespace std;


string toupper(string str)
{
  string out="";
  locale loc;
  for (string::size_type i=0; i<str.length(); i++)
    out+= toupper(str[i],loc);
  return out;
}

int main(int argc, char** argv)  {
    
    if(argc!=4)
    {
        cout<< "USAGE: [Input filename] [output filename for insert queries] [output filename for creation queries]"<<endl;
        return 0;
    }

    ifstream inputFile;
    inputFile.open(argv[1]);

    if(!inputFile.is_open()) 
    {
        cout << "Unable to open output file for creation queries"<<endl;
        return 1;
    }

    ofstream fo1 (argv[2]);
    ofstream fo2 (argv[3]);

    if (!fo1.is_open())
    {
        inputFile.close();
        cout << "Unable to open output file for insert queries"<<endl;
        return 1;
    }
    if (!fo2.is_open())
    {
        inputFile.close();
        fo1.close();
        cout << "Unable to open output file for creation queries"<<endl;
        return 1;
    }

        
    string line;
    while (getline(inputFile, line)){             
        if(toupper(line).find("INSERT INTO ") != string::npos)
         { fo1 << line << endl;}
        else { fo2 << line << endl;}
    }
    
    inputFile.close();
    fo1.close();
    fo2.close();
    return 0;
}
