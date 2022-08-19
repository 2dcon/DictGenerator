//
// Created by ferris on 7/18/22.
//

#include "Decoder.h"


// string Decoder::Decode(string &str)
//{
//    string decodedStr;
//
//    ulong len = str.size();
//    for (ulong i = 0; i < len; ++i)
//    {
//        if (str[i] == '&')
//        {
//            for (ulong j = i + 1; j < len; ++j)
//            {
//                if (str[j] == ';')
//                {
//                    // convert the char code
//                    string charCode = str.substr(i, j - i + 1);
//                    string decodedChar = charMap[charCode];
//                    decodedStr += decodedChar;
//                    cout << "Decoder: converted char" << charCode << " to " << decodedChar << endl;
//                    // got to the end of char code
//                    i = j;
//                    break;
//                }
//            }
//        }
//        else
//        {
//            decodedStr += str[i];
//        }
//    }
//
//    return decodedStr;
//}
