//
// Created by ferris on 7/6/22.
//
#include "iostream"
#include "HtmlTree.h"
#include "Tools.h"

Node::Node()
{
    pElement = nullptr;
    pParent = nullptr;
    pChild = nullptr;
    pNext = nullptr;
    pLast = this;
}

Node::~Node()
{
    delete pElement;
//    cout << "Destroying Node." << endl;
}

Node* Node::GetParent() const
{
    return this->pParent;
}

void Node::AddNext(Node & nextElement)
{
    nextElement.pParent = this->pParent;
    this->pNext = & nextElement;
}

Node * Node::GetNext() const
{
    return this->pNext;
}

bool Node::HasChildren() const
{
    return this->pChild != nullptr;
}

Node *Node::GetLast() const
{
    return this->pLast;
}

bool Node::IsLast() const
{
    return this->pLast == this;
}

void Node::AddChild(Node & newChild)
{
    newChild.pParent = this;
    newChild.pNext = this;
    if (pChild)
    {
        this->pChild->pLast->pNext = &newChild;
        this->pChild->pLast = &newChild;
    }
    else
    {
        this->pChild = &newChild;
    }
}

string Node::ToString()
{
//    cout << "I'm here!\n";
    string s;
    for (auto node : *this)
    {
        if (node->pElement && StringNotBlank(node->pElement->text))
        {
            s += node->pElement->text + ' ';
        }
//        else
//        {
//            cout << "Node has no text." << endl;
//        }
    }

    s = Trim(s);

    return s;
}

string Node::GetTag() const
{
    if (this->pElement == nullptr)
        return "";

    return this->pElement->tag;
}

string Node::GetClass() const
{
    if (this->pElement == nullptr)
        return "";
    return this->pElement->class_;
}

string Node::GetId() const
{
    if (this->pElement == nullptr)
        return "";
    return this->pElement->id;
}

string Node::GetString() const
{
    string s;
    if (this->pElement)
    {
        s = this->pElement->text;
    }
    return s;
}

HtmlTree::HtmlTree()
{
    rootNode = nullptr;
}

HtmlTree::HtmlTree(Node & root)
{
    rootNode = & root;
}

int HtmlTree::NodeCount() const
{
    using namespace std;
    int count = 0;
    for (auto node : * rootNode)
    {
        count++;
    }
    return count;
}

vector<Node *> HtmlTree::Find(string &tag, string &class_, string &id)
{
    vector<Node*> matches = vector<Node*>();
    for (auto node : * rootNode)
    {
        if (node->pElement && node->pElement->tag == tag && node->pElement->class_ == class_
            && node->pElement->id == id)
        {
            matches.emplace_back(node);
        }
    }
    return matches;
}

Node * HtmlTree::FindFirst(string &tag, string &class_, string &id) const
{
    for (auto node : * rootNode)
    {
        if (node->pElement && node->pElement->tag == tag && node->pElement->class_ == class_
            && node->pElement->id == id)
        {
            return node;
        }
    }
    return nullptr;
}

HtmlTree::~HtmlTree()
{
    vector<Node*> toBeDeleted;
    for (Node * node : * rootNode)
    {
        toBeDeleted.emplace_back(node);
    }
    for (auto node : toBeDeleted)
    {
        delete node;
    }
}
