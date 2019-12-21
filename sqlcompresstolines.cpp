#include <iostream>
#include <fstream>

using namespace std;


std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}
string trim(const string& str)
{

    if(str.find_first_not_of(' ') == string::npos)//the string is empty or contains only spaces
        return "";
    
    string result = str;
    result = ReplaceAll( result, "\r", "");
    size_t first = result.find_first_not_of(' '), last = result.find_last_not_of(' ');
    result = result.substr(first, (last - first + 1));
    
    int len;
    do{
        len = result.length();
        result = ReplaceAll( result, "  ", " ");
        result = ReplaceAll( result, ", ", ",");
        result = ReplaceAll( result, " ,", ",");
        result = ReplaceAll( result, ") ", ")");
        result = ReplaceAll( result, " )", ")");
        result = ReplaceAll( result, "( ", "(");
        result = ReplaceAll( result, " (", "(");

    }while(result.length() != len);
    return result;
}

int main(int argc, char** argv)  {
    
    if(argc!=3)
    {
        cout<< "USAGE: sqlcompresstolines [Input filename] [output filename]"<<endl;
        return 0;
    }

    ifstream inputFile;
    inputFile.open(argv[1]);

    if(!inputFile.is_open()) 
    {
        cout << "Unable to open output file for creation queries"<<endl;
        return 1;
    }

    ofstream fo (argv[2]);

    if (!fo.is_open())
    {
        inputFile.close();
        cout << "Unable to open output file for insert queries"<<endl;
        return 1;
    }

    string newLine="";
    string line="";
    int lastOpenCommentType3 = -1, len;
    int idxEndOfQuery, idxCommentType1, idxCommentType2, idxOpenCommentType3, idxCloseCommentType3;
    while (getline(inputFile, line)){ 
        len = newLine.length();
        
        idxEndOfQuery           = line.find(";");
        idxCommentType1         = line.find("--");
        idxCommentType2         = line.find("#");
        idxOpenCommentType3     = line.find("/*");
        idxCloseCommentType3    = line.find("*/");
        
        if( (idxEndOfQuery != string::npos) &&
            ((idxEndOfQuery < idxCommentType1 && idxCommentType1 != string::npos) || idxCommentType1 == string::npos) &&
            ((idxEndOfQuery < idxCommentType2 && idxCommentType2 != string::npos) || idxCommentType2 == string::npos) &&
            (((idxEndOfQuery < idxOpenCommentType3 && idxOpenCommentType3 != string::npos)|| idxOpenCommentType3 == string::npos) && (lastOpenCommentType3 == -1))
        ){        
            newLine += line.substr(0, idxEndOfQuery+1);//1: length of ';'
            fo<< trim(newLine) <<endl;
            newLine = "";
        } else if( (idxCommentType1 != string::npos) &&
            (idxCommentType1 < idxCommentType2 || idxCommentType2 == string::npos) &&
            ((idxCommentType1 < idxOpenCommentType3 || idxOpenCommentType3 == string::npos )&& lastOpenCommentType3 == -1)
        ){
            newLine += line.substr(0, idxCommentType1);
        } else if( (idxCommentType2 != string::npos) &&
            ((idxCommentType2 < idxOpenCommentType3 || idxOpenCommentType3 == string::npos) && lastOpenCommentType3 == -1)
        ){
            newLine += line.substr(0, idxCommentType2);
        } else if( (idxOpenCommentType3 != string::npos) &&
                   (lastOpenCommentType3 == -1)
        ){
            lastOpenCommentType3 = 1;
            newLine += line.substr(0, idxOpenCommentType3);
        } else if(lastOpenCommentType3 == 1 && idxCloseCommentType3 != string::npos)
        {
            lastOpenCommentType3 = -1;
            newLine += line.substr(idxCloseCommentType3 + 2);//2 length of '*/';
        } else if(lastOpenCommentType3 == -1){    
            newLine += line;
        } 
        if(len != newLine.length())
            newLine += " ";
    }
    
    inputFile.close();
    fo.close();
    return 0;
}
