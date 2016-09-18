#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

class Counter {
public:
    Counter() { ++count_; }
    ~Counter() { --count_; }
    static int count() { return count_; }
     
private:
    static int count_;
};

int Counter::count_ = 0;

class MyGraph {
public:
    class Node : public Counter {
        vector<shared_ptr<Node>> children;
        vector<Node *> parents;

    public:
        void AddChild(const shared_ptr<Node>& node) {
            children.push_back(node);
            node->parents.push_back(this);
        }

        void RemoveChildInternal(const shared_ptr<Node>& node,
                                  vector<Node *> &parentsSeen)
        {
            auto selfSeen = find(parentsSeen.begin(), parentsSeen.end(), this);
            if (selfSeen != parentsSeen.end()) {
                return;
            }
            parentsSeen.push_back(this);

            for (auto p : node->parents) {
                auto parentSeen = find(parentsSeen.begin(), parentsSeen.end(), p);
                if (p == this || parentSeen != parentsSeen.end()) {
                    continue;
                }
                p->RemoveChildInternal(node, parentsSeen);
            }

            auto iter = find(children.begin(), children.end(), node);
            if (iter != children.end()) {
                children.erase(iter);
            }
        }

        void RemoveChild(const shared_ptr<Node>& node)
        {
            auto parentsSeen =  vector<Node *>();
            RemoveChildInternal(node, parentsSeen);
        }
    };
 
    void SetRoot(const shared_ptr<Node>& node) {
        root = node;
    }
 
    void ShrinkToFit() {
    }
 
    static auto MakeNode() {
        return make_shared<MyGraph::Node>();
    }
 
private:
    shared_ptr<Node> root;
};

bool TestCase1() {
    MyGraph g;
    {
        auto a = MyGraph::MakeNode();
        g.SetRoot(a);
        auto b = MyGraph::MakeNode();
        a->AddChild(b);
        auto c = MyGraph::MakeNode();
        b->AddChild(c);
        a->RemoveChild(b);
    }
    g.ShrinkToFit();
    return Counter::count() == 1;
}

bool TestCase2() {
    MyGraph g;
    {
        auto a = MyGraph::MakeNode();
        g.SetRoot(a);
        auto b = MyGraph::MakeNode();
        a->AddChild(b);
        auto c = MyGraph::MakeNode();
        b->AddChild(c);
        auto d = MyGraph::MakeNode();
        b->AddChild(d);
        d->AddChild(b);
        a->RemoveChild(b);
    }
    g.ShrinkToFit();
    return Counter::count() == 1;
}

bool TestCase3() {
    MyGraph g;
    {
        auto a = MyGraph::MakeNode();
        g.SetRoot(a);
        auto b = MyGraph::MakeNode();
        a->AddChild(b);
        auto c = MyGraph::MakeNode();
        b->AddChild(c);
        auto d = MyGraph::MakeNode();
        b->AddChild(d);
        d->AddChild(b);
    }
    g.ShrinkToFit();
    return Counter::count() == 4;
}

bool TestCase4() {
    MyGraph g;
    {
        auto a = MyGraph::MakeNode();
        g.SetRoot(a);
        auto b = MyGraph::MakeNode();
        a->AddChild(b);
        auto c = MyGraph::MakeNode();
        b->AddChild(c);
        auto d = MyGraph::MakeNode();
        b->AddChild(d);
        d->AddChild(b);
        a->RemoveChild(b);
    }
    g.ShrinkToFit();
    return Counter::count() == 4;
}

int main() {
    cout.setf(ios::boolalpha);

    bool passed1 = TestCase1();
    cout << passed1 << endl;

    bool passed2 = TestCase2();
    cout << passed2 << endl;

    bool passed3 = TestCase3();
    cout << passed3 << endl;

    bool passed4 = TestCase4();
    cout << passed4 << endl;
}