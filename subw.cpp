#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <iostream>

class SubWordGraph {
    std::string str;

    struct Node {
        int len {0};
        int slink {-1};
        std::unordered_map<char, int> next;
    };
    std::vector<Node> st{{}};
    int last {0};

public:
    void add(char c) {
        str += c;
        st.emplace_back();
        int cur = st.size() - 1;

        st[cur].len = st[last].len + 1;

        int p = last;
        for (; p != -1 && !st[p].next.count(c); p = st[p].slink) {
            st[p].next[c] = cur;
        }

        if (p == -1) {
            st[cur].slink = 0;
        } else {
            int q = st[p].next[c];
            if (st[p].len + 1 == st[q].len) {
                st[cur].slink = q;
            } else {
                st.emplace_back(st[q]);
                int clone = st.size() - 1;
                st[clone].len = st[p].len + 1;

                for (; p != -1 && st[p].next[c] == q; p = st[p].slink) {
                    st[p].next[c] = clone;
                }

                st[q].slink = st[cur].slink = clone;
            }
        }
        last = cur;
    }

    void add(const std::string& s) {
        for (char c : s) {
            add(c);
        }
    }

    void draw() {
        std::cout << "digraph {\n"
            << "rankdir=LR;\n"
            << "fontsize=24;\n"
            << "label=\"Subword Graph of " << str << "\";\n"
            ;
        std::vector<std::vector<int>> nodes(str.size() + 1);
        for (int i = 0; i < st.size(); ++i) {
            nodes[st[i].len].push_back(i);
        }

        int i = 0;
        for (const auto &ns : nodes) {
            std::cout << "{\n";
            std::cout << "\tstyle=invis;\n";
            std::cout << "\t{rank = same; ";
            for (const auto& n : ns) {
                std::cout << n << "[label=" << st[n].len << "]; ";
            }
            std::cout << "}\n";
            
            
            for (const auto& n : ns) {
                if (st[n].slink != -1) {
                    std::cout << "\t" << n << " -> " << st[n].slink 
                              << "[color=\"gray\", constraint=\"false\" style=\"dashed\"];\n";
                }

                for (const auto& item : st[n].next) {
                    auto color = st[n].len + 1 == st[item.second].len ? "green" : "blue";
                    std::cout << "\t" << n << " -> " << item.second 
                              << "[label=\"" << item.first << "\""
                              << ", color=\"" << color << "\""
                              << ", fontsize=16"
                              << "];\n";
                }
            }
            std::cout << "}\n";
        }
        std::cout << "}\n";
    }
};

int main() {
    std::string s {"aabbabab"};
    SubWordGraph swg;
    swg.add(s);
    swg.draw();
}



