//
// Created by ferris on 7/6/22.
//

#ifndef DICTGENERATOR_HTMLTREE_H
#define DICTGENERATOR_HTMLTREE_H

#include "string"
#include "vector"
#include "iterator"

using namespace std;

struct HtmlElement
{
public:
    string tag;
    string id;
    string class_;
    string text;
    HtmlElement()
    {
        tag = "";
        id = "";
        class_ = "";
        text = "";
    }
    explicit HtmlElement(string & newText)
    {
        tag = "";
        id = "";
        class_ = "";
        text = newText;
    }
    HtmlElement(string newTag, string newId, string newClass, string newText)
    {
        tag = std::move(newTag);
        id = std::move(newId);
        class_ = std::move(newClass);
        text = std::move(newText);
    }
    ~HtmlElement()= default;;
};

class Node
{
public:
    HtmlElement * pElement;
    Node * pParent;
    Node * pChild;
    Node * pNext;
    Node * pLast;

    Node();
    ~Node();

    string GetTag() const;
    string GetClass() const;
    string GetId() const;
    void AddChild(Node &newChild);
    [[nodiscard]] Node* GetParent() const;
    [[nodiscard]] bool HasChildren() const;
    void AddNext(Node &);
    [[nodiscard]] Node * GetNext() const;
    [[nodiscard]] Node * GetLast() const;
    [[nodiscard]] bool IsLast() const;
    [[nodiscard]]string ToString();
    string GetString() const;

    class Iterator
    {
    private:
        Node * pCurrentNode;
        Node * pInitialNode;
    public:
        explicit Iterator(Node * pStartElement) noexcept: pCurrentNode(pStartElement){
            pInitialNode = pStartElement;
        }

        Iterator & operator=(Node * pNewElement)
        {
            pCurrentNode = pNewElement;
            return *this;
        }

        Iterator & operator++()
        {
            if (pCurrentNode)
            {
                if (pCurrentNode->pChild)
                {
//                    cout << "Node::Iterator Going to pChild" << endl;
                    pCurrentNode = pCurrentNode->pChild;
                }
                else if (pCurrentNode != pInitialNode)
                {
                    if (pCurrentNode->pNext)
                    {
//                        cout << "Node::Iterator Going to pNext" << endl;
                        pCurrentNode = pCurrentNode->pNext;
                    } else if (pCurrentNode->pParent)
                    {
//                        cout << "Node::Iterator Going to pParent" << endl;
                        // keeps going up until finds a node with next
                        for (Node *node = pCurrentNode->pParent; node != nullptr; node = node->pParent)
                        {
                            if (node == pInitialNode)
                            {
                                pCurrentNode = nullptr;
                                return *this;
                            }
                            if (node->pNext != nullptr)
                            {
                                pCurrentNode = node->pNext;
                                return *this;
                            }
                        }
                        // if the next node is not found, returns null
                        pCurrentNode = nullptr;
                    }
                }
                // if the initial node has no child, returns nullptr
                else
                {
                    pCurrentNode = nullptr;
                }

            }

            return * this;
        }

        Iterator operator++(int)
        {
            Iterator iter = * this;
            ++ * this;
            return iter;
        }

        bool operator!=(const Iterator & iter)
        {
            return pCurrentNode != iter.pCurrentNode;
        }

        Node * operator*()
        {
            return pCurrentNode;
        }

        Node * GetNode()
        {
            return pCurrentNode;
        }
    };

    Iterator begin()
    {
        return Iterator(this);
    }

    static Iterator end()
    {
        return Iterator(nullptr);
    }

};

class HtmlTree
{
public:
    Node * rootNode;

    explicit HtmlTree();
    explicit HtmlTree(Node&);
    ~HtmlTree();

    vector<Node *> Find(string & tag, string & class_, string & id);
    Node * FindFirst(string & tag, string & class_, string & id) const;
//    vector<HtmlElement> FindTag(string&);
//    vector<HtmlElement> FindClass(string&);
    [[nodiscard]] int NodeCount() const;
};


#endif //DICTGENERATOR_HTMLTREE_H
