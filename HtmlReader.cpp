//
// Created by ferris on 7/6/22.
//

#include <utility>
#include <stack>
#include "HtmlReader.h"
#include "Tools.h"
#include "HtmlTree.h"

HtmlReader::HtmlReader(const string & htmlString)
{

    ReadHtml(htmlString);
}

HtmlReader::TagProp HtmlReader::GetProp(const string & propString)
{
    string name, value;

    for (int i = 0; i < propString.size(); i++)
    {
        if (propString[i] == '=' and propString[i + 1] == '"')
        {
            for (int j = i + 2; j < propString.size(); j++)
            {
                if (propString[j] == '"') break;

                value += propString[j];
            }

            break;
        }
        name += propString[i];
    }

    HtmlReader::TagProp tagProp = HtmlReader::TagProp(name, value);

    return tagProp;
}

/**
 *
 * @param tagString string inside brackets (brackets not included!)
 * @return
 */
HtmlElement * HtmlReader::GenElement(const string & tagString)
{
    HtmlElement * pElement = new HtmlElement();
    vector<string> props = SplitProps(tagString);

    if (!props.empty())
    {
        if (props[0] == "!doctype")
        {
            pElement->tag = "html";
            return pElement;
        }

        pElement->tag = props[0];

        for (const string & prop: props)
        {
            TagProp tagProp = GetProp(prop);

            if (tagProp.name == "class")
            {
                pElement->class_ = tagProp.value;
            }
            else if (tagProp.name == "id")
            {
                pElement->id = tagProp.value;
            }
        }
    }
    return pElement;
}

void HtmlReader::ReadHtml(const string& htmlString)
{
    ulong htmlSize = htmlString.size();

    Node * currentNode = new Node();
    string textBuff;
    htmlTree = new HtmlTree(*currentNode);

    for (int i = 0; i < htmlSize; i++)
    {
        if (htmlString[i] == '<')
        {
            // find closing bracket
            int end = -1;
            for (int j = i + 1; j < htmlSize; j++)
                if (htmlString[j] == '>')
                {
                    end = j;
                    break;
                }

            if (end > 0 && end < htmlSize)
            {
                ++i;

                // skip <script></script>
                if (htmlString.substr(i, 6) == "script")
                {
                    for (int j = i + 7; j < htmlSize; j++)
                    {
                        if (htmlString[i] == '\n')
                        if (htmlString[j] == '<' && htmlString.substr(j + 1, 7) == "/script")
                        {
                            end = j + 8;
                            break;
                        }
                    }
                }
                // skip comments, single tags and <br>s
                else if (htmlString[i] == '!' || htmlString[end - 1] == '/'
                || (htmlString[i] == 'b' && htmlString[i + 1] == 'r') || (htmlString.substr(i, 3) == "img"))
                {
                    i = end;
                    continue;
                }
                // keep italic tag
                else if (htmlString[i] == 'i')
                {
                    // find </i>
                    for (int j = end; j < htmlSize; j++)
                    {
                        if (htmlString[j] == '<' && htmlString.substr(j + 1, 3) == "/i>")
                        {
                            end = j + 3;
                            break;
                        }
                    }

                    textBuff += htmlString.substr(i - 1, end - i + 2);

                    i = end;
                    continue;
                }
                // decide whether it is an opening or a closing tag
                else if (htmlString[i] == '/')
                {
                    string closeTagName = htmlString.substr(i + 1, end - i - 1);
                    if (currentNode->pElement && closeTagName == currentNode->pElement->tag)
                    {
                        // add a child node for the text if the text is not blank
                        if (StringNotBlank(textBuff))
                        {
                            Node * textNode = new Node();
                            string text = RemoveNewline(Trim(textBuff));
                            textNode->pElement = new HtmlElement(text);
                            currentNode->AddChild(* textNode);
                            textBuff.clear();
                        }
                        currentNode = currentNode->pParent;
                    }
//                    else
//                    {
//                        cout << "HtmlReader::ReadHtml - Opening tag not found for " << closeTagName << endl;
//                        cout << "currentNode->pElement->tag = " << currentNode->pElement->tag << endl;
//                    }
                }
                else
                {
                    // if the current node contains text before the new node, make a child node for the text
                    if (!textBuff.empty() && StringNotBlank(textBuff))
                    {
                        string text = RemoveNewline(Trim(textBuff));
                        Node * textNode = new Node();
                        textNode->pElement = new HtmlElement(text);
                        currentNode->AddChild(*textNode);
                        textBuff.clear();
                    }

                    Node * newNode = new Node();
                    currentNode->AddChild(*newNode);

                    string substring = htmlString.substr(i, end - i);
                    currentNode = newNode;
                    currentNode->pElement = GenElement(substring);
                }
                i = end;
            }
            else
            {
//                cout << "end < 0" << endl;
                textBuff += htmlString[i];
            }
        }
        else
        {
            textBuff += htmlString[i];
        }
    }
//    cout << "ReadHtml finished\n";
}

vector<Node *> HtmlReader::Find(string tag, string class_, string id)
{
    return htmlTree->Find(tag, class_, id);
}

Node * HtmlReader::FindFirst(string tag, string class_, string id) const
{
    return htmlTree->FindFirst(tag, class_, id);
}
