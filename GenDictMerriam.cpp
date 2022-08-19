//
// Created by ferris on 8/3/22.
//
#include <future>
#include "string"
#include "fstream"
#include "map"
#include "filesystem"
#include "algorithm"
#include "thread"
#include "chrono"

#include "HtmlReader.h"
#include "Tools.h"
#include "Decoder.h"
#include "Dict.h"
#include "GenDictMerriam.h"

using namespace std;

#include "GenDictMerriam.h"
Entry * GenEntryMerriam(const string &fileString)
{
    HtmlReader reader = HtmlReader(fileString);

    auto content = reader.FindFirst("div", "left-content col-lg-7 col-xl-8", "left-content");

    if (content == nullptr)
    {
        cout << "content not found";
        return nullptr;
    }

    bool hasMultiEntry = false;
    string key, entryText;
    key = reader.FindFirst("h1", "hword", "")->ToString();

    for (auto node : * content)
    {
        if (node->GetClass() == "hword")
        {
            if (hasMultiEntry)
                entryText += endOfEntry;
            else
                hasMultiEntry = true;

            entryText += NewAttr(attrKey, key);

            string word = node->ToString();
            entryText += NewAttr(attrWord, word);
        }
        // ipa
        else if (node->GetClass() == "pr")
        {
            string ipa = node->ToString();
            entryText += NewAttr(attrIpa, ipa);
        }
        // pos
        else if (node->GetClass() == "important-blue-link")
        {
            string pos = node->ToString();
            entryText += NewAttr(attrPos, pos);
        }
        // grouped defs
        else if (node->GetClass() == "sb has-num has-let")
        {
            entryText += newDefinitionGroup;
        }
        else if (node->GetClass() == "dtText")
        {
            entryText += "\ndef=" + node->ToString();
        }
        // defs that each has its own group
        else if (node->GetClass() == "sb has-num")
        {
            entryText += newDefinitionGroup + "def=" + node->ToString();
        }
        else if (node->GetClass().starts_with("ex-sent"))
        {
            string nodeString = node->ToString();
            entryText += newLine + Italicize(nodeString);
        }
        else if (node->GetClass() == "et")
        {
            string ety;
            bool skip = false;

            for (auto subNode: * node)
            {
                if (subNode->GetTag() == "em")
                {
                    string italic = subNode->ToString();
                    ety += Italicize(italic);
                    skip = true;
                }
                else if (skip)
                {
                    skip = false;
                    continue;
                }
                else
                {
                    ety += node->GetString();
                }
            }

            entryText += NewAttr(attrEty, ety);
        }
        else if (node->GetClass() == "widget more_defs")
        {
            break;
        }
    }

    entryText += endOfEntry;

    Entry * entry = new Entry(key, entryText);

    return entry;
}

void GenDictMerriam()
{
    const string header = "dict=Merriam-Webster Dictionary\nlang=en-us\n\n";
    const string path = "/home/ferris/Documents/dict/html/merriam/";
    const string testFile = "/home/ferris/Documents/dict/html/merriam/be.html";
    const string ofile = "/home/ferris/projects/clion/DictGenerator/out/merriam.dict";
    const string logFile = "/home/ferris/projects/clion/DictGenerator/log/merriam.log";

    string testString = FileToString(testFile);

    auto e = GenEntryMerriam(testString);

    cout << e->text;

    return;

    vector<string> vFiles;
    vector<Entry*> vEntries = vector<Entry*>();
    vector<string> vHtmlStrings;

    ulong total = vHtmlStrings.size();

    // multi thread
    auto multiStart = chrono::high_resolution_clock::now();
    vector<future<Entry*>> futEntries;



    auto multiEnd = chrono::high_resolution_clock::now();
    auto multiDuration = chrono::duration_cast<chrono::milliseconds>(multiEnd - multiStart);
    cout << "Entries = " << vEntries.size() << "\ntime consumption = " << multiDuration << endl;


    ofstream ostream(ofile);
    ostream << header;

    cout << "\nsorting\n";
    auto sortStart = chrono::high_resolution_clock::now();
    sort(vEntries.begin(), vEntries.end(), Entry::ComparePointer);
    auto sortEnd = chrono::high_resolution_clock::now();
    auto sortDuration = chrono::duration_cast<chrono::seconds >(sortEnd - sortStart);
    cout << "Time consumption for sort: " << sortDuration << endl;
    for (auto entry : vEntries)
    {
        ostream << entry->text;
    }

//    ofstream logStream(logFile);
//    logStream << logString;

}