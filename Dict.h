//
// Created by ferris on 7/5/22.
//

#include <string>
#include <utility>

#ifndef DICTGENERATOR_ENOLEX_H
#define DICTGENERATOR_ENOLEX_H

using namespace std;

const string attrKey = "key";
const string attrWord = "word";
const string attrDef = "def";
const string attrIpa = "ipa";
const string attrEty = "ety";
const string attrPos = "pos";
const string newLine = "<br>";
const string endOfEntry = "\n#eoe\n\n";
const string newDefinitionGroup = "\n#def\n";

struct Entry {
    string key;
    string word;
    string text;
public:
    Entry(string entryKey, string entryText)
    {
        key = std::move(entryKey);
//        word = move(entryWord);
        text = std::move(entryText);
    }
    bool operator < (const Entry & anotherEntry) const
    {
        return key < anotherEntry.key;
    }
    static bool ComparePointer (Entry * entry0, Entry * entry1)
    {
        return entry0->key < entry1->key;
    }
};

#endif //DICTGENERATOR_ENOLEX_H