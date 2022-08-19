//
// Created by ferris on 7/12/22.
//
#include "string"
#include "fstream"
#include "map"
#include "filesystem"
#include "algorithm"
#include "thread"
#include "chrono"

#include "GenDictMauro.h"
#include "HtmlReader.h"
#include "Tools.h"
#include "Decoder.h"
#include "Dict.h"

using namespace std;

const static string mauroBodyBegin = "<!-- *** lemma begin *** -->";
const static string mauroBodyEnd = "<!-- *** lemma end *** -->";

void GenEntry(const string &fileString, promise<Entry*> && pro)
{
    HtmlReader reader = HtmlReader(fileString);

    string key, entryText;

    // find word
    auto nWord = reader.Find("h1", "hentry__title", "");
    if (!nWord.empty())
    {
        string word = nWord[0]->ToString();
        key = RemoveNumber(word);
        entryText += NewAttr(attrKey, key);
        entryText += NewAttr(attrWord, word);
    }
//    else
//    {
//        *logString += "couldn't find word in file " + fileName + '\n';
//    }

    // find pos
    auto nPos = reader.Find("section", "", "lemma_qualifica");
    if (!nPos.empty())
    {
        entryText += NewAttr(attrPos, nPos[0]->ToString());
    }

    // find ipa and ety
    auto nIpaEty = reader.Find("section", "", "lemma_lemma");
    string ipa;
    string ety;
    switch (nIpaEty.size())
    {
        case 0:
            cout << "Node not found!";
            break;
        case 1:
            ipa = nIpaEty[0]->ToString();
            break;
        default:
            ipa = nIpaEty[0]->ToString();
            ety = nIpaEty[1]->ToString();

            break;
    }

    if (!ipa.empty())
        entryText += NewAttr(attrIpa, ipa);
    if (!ety.empty())
        entryText += NewAttr(attrEty, ety);

    // definitions
    auto vDef = reader.Find("section", "", "descrizione");
    if (!vDef.empty())
    {
        string defString;
        // numbering
        bool bIsNumbering = false;
        int currentGroupNum = 0;
        // skip irrelevant stuff
        bool bSkip = false;
        // if a verb has multiple pos
        bool bIsPos = false;
        // when there is no numbering (no "ac"), add a def= manually
        bool bDefAttrAdded = false;

        for (auto node: *vDef[0])
        {
            auto pElement = node->pElement;
            if (pElement)
            {
                if (pElement->class_ == "ac")
                {
                    bIsNumbering = true;
                    defString += '\n';
                }
                else if (pElement->class_ == "mu")
                {
                    bSkip = true;
                }
                else if (!pElement->text.empty())
                {
                    if (bSkip)
                    {
                        bSkip = false;
                        continue;
                    }
                    else if (bIsPos)
                    {
                        defString += NewAttr(attrPos, pElement->text);
                        bIsPos = false;
                        bDefAttrAdded = false;
                    }

                    else if (bIsNumbering)
                    {
                        // decide if a new def group is needed
//                        cout << "pElement->text = " << pElement->text << endl;
                        int groupNum = GetNumbering(pElement->text);
                        if (groupNum > 0)
                        {
                            if (groupNum > currentGroupNum)
                            {
                                defString += "#def\ndef=";
                                currentGroupNum = groupNum;
                            } else
                            {
                                defString += "def=";
                            }
                            bDefAttrAdded = true;
                        }
                        else
                        {
                            // if the text doesn't contain any digit, suppose that it is a new pos group
                            bIsPos = true;
                        }
                        bIsNumbering = false;
                    }
                    else
                    {
                        if (!bDefAttrAdded)
                        {
                            defString += "def=";
                            bDefAttrAdded = true;
                        }
                        defString += pElement->text;
                    }
                }
            }
        }
        if (!bDefAttrAdded)
        {
            defString = "def=" + defString;
        }
        entryText += defString;
    }

    //expressions
    auto nPoli = reader.Find("section", "", "polirematiche");

    if (!nPoli.empty())
    {
        bool bIsTitle = false;

        for (auto node : *nPoli[0])
        {
            auto pElement = node->pElement;
            if (pElement)
            {
                if (pElement->tag == "strong")
                {
                    bIsTitle = true;
                }
                else if (pElement->class_ == "ac")
                {
                    entryText += newLine;
                }
                else if (StringNotBlank(pElement->text))
                {
                    string text = pElement->text;

                    if (bIsTitle)
                    {
                        entryText += "\nexp=" + text + "\ndes=";
                        bIsTitle = false;
                    }
                    else
                    {
                        entryText += pElement->text + ' ';
                    }
                }
            }
        }
    }

    entryText += "\n#eoe\n\n";

    Entry * entry = new Entry(key, entryText);

    pro.set_value(entry);
}

void GenDictMauro()
{
    const string header = "dict=De Mauro\n\n";
    const string path = "/home/ferris/Documents/dict/html/mauro/";
    const string testFile = "/home/ferris/Documents/dict/html/mauro/trarre";
    const string ofile = "/home/ferris/projects/clion/DictGenerator/out/mauro.dict";
    const string logFile = "/home/ferris/projects/clion/DictGenerator/log/mauro.log";

    string logString;
//    const string testFile = "/home/ferris/Documents/dict/html/mauro/andare";

//    auto entry = GenEntry(testFile);
//    cout << "entry text = " << entry->text << endl;
//    return;

    vector<string> vFiles;
    vector<Entry*> vEntries = vector<Entry*>();
    vector<string> vHtmlStrings;

//    ulong total = 20000;
    int counter = 0;
    auto readStart = chrono::high_resolution_clock::now();
    for (const auto & entry : filesystem::directory_iterator(path))
    {
        cout << "\rReading files: " << counter++ << flush;
//        if (counter > total)
//            break;
//        vFiles.emplace_back(entry.path());
        vHtmlStrings.emplace_back(FileToString(entry.path(), mauroBodyBegin, mauroBodyEnd));
    }
    auto readEnd = chrono::high_resolution_clock::now();
    auto readDuration = chrono::duration_cast<chrono::seconds >(readEnd - readStart);
    cout << "\nRead complete in " << readDuration << endl;

    ulong total = vHtmlStrings.size();

    // multi thread
    auto multiStart = chrono::high_resolution_clock::now();
    vector<future<Entry*>> futEntries;

    ulong maxThreads = 1024;
    for (ulong i = 0; i < total; i += maxThreads)
    {
        vector<thread> vThreads;
        for (ulong j = i; j < i + maxThreads && j < total; j++)
        {
            promise<Entry *> proEntry;
            future<Entry*> f = proEntry.get_future();
//        futEntries.emplace_back(f);
//        thread th(GenEntry, move(vFiles[i]), move(proEntry));
//        th.join();
            vThreads.emplace_back(GenEntry, move(vHtmlStrings[j]), move(proEntry));
            vEntries.emplace_back(f.get());

        }
        for (thread & th : vThreads)
        {
            th.join();
        }
        vThreads.clear();
    }

    auto multiEnd = chrono::high_resolution_clock::now();
    auto multiDuration = chrono::duration_cast<chrono::milliseconds>(multiEnd - multiStart);
    cout << "Entries = " << vEntries.size() <<
            "\nthreads = " << maxThreads <<
            "\ntime = " << multiDuration << endl;


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