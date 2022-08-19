#include <iostream>
#include <vector>
#include "Dict.h"
#include "filesystem"
#include "HtmlReader.h"
#include "HtmlTree.h"
#include "algorithm"

#include "GenDictMerriam.h"

vector<vector<string>> GenFileList(const string& folder, int maxListSize)
{
    vector<vector<string>> fileList;
    int counter = 0;

    for (const auto & file : filesystem::directory_iterator(folder))
    {
        if (counter % maxListSize == 0)
        {
            fileList.emplace_back(vector<string>());
        }
        fileList.back().push_back(file.path());
        counter++;
    }

    return fileList;
}

void NodeTest()
{
    Node root = Node(), lv0_child0 = Node(), lv1_child0 = Node(),
            lv0_child1 = Node(), lv0_child2 = Node();

    root.pElement = new HtmlElement("root", "", "", "root");
    lv0_child0.pElement = new HtmlElement("lv0", "", "", "lv0_child0");
    lv0_child1.pElement = new HtmlElement{"lv0", "", "", "lv0_child1"};
    lv0_child2.pElement = new HtmlElement{"lv0", "", "", "lv0_child2"};
    lv1_child0.pElement = new HtmlElement{"lv1", "", "", "lv1_child0"};

    root.AddChild(lv0_child0);
    root.AddChild(lv0_child1);
    root.AddChild(lv0_child2);
    lv0_child0.AddChild(lv1_child0);

//    HtmlTree tree = HtmlTree(root);

    for (auto node : lv0_child0)
    {
        cout << "pElement.text = " << node->pElement->text << endl;
    }
}

void ReaderTest(const string& file)
{
    HtmlReader reader = HtmlReader(file);
    vector<Node*> results = reader.Find("lv_1", "lv_1", "lv_1_tag_1");
//    cout << "results.size() = " << results.size() << endl;
//    cout << "results[0]->pElement->text = " << results[0]->pElement->text << endl;
    cout << "results[0]->ToString() = " << results[0]->ToString() << endl;
}

void StringTest(string & s)
{
    s = "modified!";
}

void GeneralTest()
{
}

int main() {
    cout << "Program started.\n";
//    GeneralTest();
    GenDictMerriam();

    return 0;
}
