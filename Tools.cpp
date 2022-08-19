//
// Created by ferris on 7/6/22.
//

#include "Tools.h"
#include "iostream"
#include "fstream"

vector<string> SplitString(const string & sourceString)
{
    char delimiter = ' ';
    return SplitString(sourceString, delimiter);
}

vector<string> SplitString(const string & sourceString, const char & delimiter)
{
    vector<string> result = {""};
    for (char c : sourceString)
    {
        if (c == delimiter)
        {
            result.emplace_back("");
        }
        else
        {
            result.back() += c;
        }
    }

    return result;
}

vector<string> SplitProps(const string & sourceString)
{
    vector<string> props = {""};

    bool bIsInsideQuote = false;

    for (const char c : sourceString)
    {
        if (!bIsInsideQuote)
        {
            if (c == ' ')
            {
                props.emplace_back("");
                continue;
            }
            else if (c == '"')
            {
                bIsInsideQuote = true;
            }
        }
        else if (c == '"')
        {
            bIsInsideQuote = false;
        }

        props.back() += c;
    }

    return props;
}

/**
 *
 * @param stringToCheck
 * @return if the string contains only white spaces
 */
bool StringNotBlank (string & stringToCheck)
{
    for (char c : stringToCheck)
    {
        if (c != ' ' && c != '\n' && c != '\t')
        {
            return true;
        }
    }
    return false;
}

string RemoveNewline(string s)
{
    string spaceRemoved;
    for (auto c : s)
    {
        if (c != '\n' && c != '\r')
        {
            spaceRemoved += c;
        }
    }
    return spaceRemoved;
}

string Trim(string s)
{
    ulong size = s.size();
    ulong start = 0;
    ulong end = 0;

    for (ulong i = 0; i < size; i++)
    {
        if (s[i] != ' ' && s[i] != '\n' && s[i] != '\t')
        {
            start = i;
            break;
        }
    }

    for (ulong i = size - 1; i > 0; i--)
    {
        if (s[i] != ' ' && s[i] != '\n' && s[i] != '\t')
        {
            end = i;
            break;
        }
    }

    string trimmed = s.substr(start, end - start + 1);
//    cout << "s = " << s << endl << "trimmed = " << trimmed << endl;
    return trimmed;
}

/**
 *
 * @param fileName
 * @return a string storing the file data
 */
string FileToString(const string& fileName)
{
    ifstream stream(fileName);

    string fileString;

    for (char c; stream >> noskipws >> c; )
    {
        fileString += c;
    }

    return fileString;
}


string FileToString(const string& fileName, const string& startMarker, const string& endMarker)
{
//    cout << "startMarker = " << startMarker << "\nendMarker = " << endMarker << endl;
    ifstream stream(fileName);
    string fileString;

    for (char c; stream >> noskipws >> c; )
    {
        fileString += c;
    }

    ulong fileSize = fileString.size();
    int start = 0, end = 0;
    int sizeStart = startMarker.size(), sizeEnd = endMarker.size();

    for (int i = 0; i < fileSize; i++)
    {
        if (fileString.substr(i, sizeStart) == startMarker)
        {
            start = i + sizeStart;
            for (int j = i; j < fileSize; j++)
            {
                if (fileString.substr(j, sizeEnd) == endMarker)
                {
                    end = j;
                    break;
                }
            }
            break;
        }
    }

    string finalString = fileString.substr(start, end - start);

//    cout << "FileToString finished\n";

    return finalString;
}

string NewAttr(string name, string text)
{
    string attr = '\n' + name + '=' + text;
    return attr;
}

string Decode(string &str)
{
    string decodedStr;

    ulong len = str.size();
    for (ulong i = 0; i < len; ++i)
    {
        if (str[i] == '&')
        {
            for (ulong j = i + 1; j < len; ++j)
            {
                if (str[j] == ';')
                {
                    // convert the char code
                    string charCode = str.substr(i, j - i + 1);
                    string decodedChar = charMap[charCode];
                    decodedStr += decodedChar;
//                    cout << "Decoder: converted char" << charCode << " to " << decodedChar << endl;
                    // got to the end of char code
                    i = j;
                    break;
                }
            }
        }
        else
        {
            decodedStr += str[i];
        }
    }

    return decodedStr;
}

int GetNumbering(string & str)
{
    string digits;
    for (char c : str)
    {
        if(isdigit(c))
        {
            digits += c;
        }
        else
        { break;}
    }

    if (digits.empty())
        return -1;
    else
        return stoi(digits);
}

string RemoveNumber(string & str)
{
    int start = 0;
    for (int i = 0; i < str.size(); i++)
    {
        if (str[i] != ' ' && !isdigit(str[i]))
        {
            start = i;
            break;
        }
    }
    if (start > 0)
    {
        return str.substr(start);
    }
    else
    {
        return str;
    }
}

string Italicize(string &str)
{
    string italic = "<i>" + str + "</i>";
    return italic;
}