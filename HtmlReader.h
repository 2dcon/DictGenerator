//
// Created by ferris on 7/6/22.
//

#ifndef DICTGENERATOR_HTMLREADER_H
#define DICTGENERATOR_HTMLREADER_H

#include <utility>
#include <vector>
#include "string"
#include "fstream"
#include "iostream"
#include "HtmlTree.h"
#include "map"

using namespace std;

class HtmlReader
{
public:

    struct TagProp
    {
        string name;
        string value;
    public:
        TagProp(string & n, string & v)
        {
            name = n;
            value = v;
        }
    };

public:
    HtmlTree * htmlTree;

public:
    explicit HtmlReader(const string &htmlString);


    vector<Node *> Find(string tag, string class_, string id);

    static HtmlElement * GenElement(const string &tagString);

    static TagProp GetProp(const string&);

    Node * FindFirst(string tag, string class_, string id) const;

private:
    void ReadHtml(const string&);
};

#endif //DICTGENERATOR_HTMLREADER_H
