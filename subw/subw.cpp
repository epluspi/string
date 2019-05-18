#include <map>
#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>

class SubWordGraph {
public:
    void add(char c) {
        content += c;
        states.push_back(Node());
        const int cur = states.size() - 1;

        states[cur].len = states[last].len + 1;

        int p = last;
        for (; p != -1 && !states[p].next.count(c); p = states[p].slink) {
            states[p].next[c] = cur;
        }

        if (p == -1) {
            states[cur].slink = 0;
        } else {
            const int q = states[p].next[c];
            if (states[p].len + 1 == states[q].len) {
                states[cur].slink = q;
            } else {
                states.push_back(states[q]);
                const int clone = states.size() - 1;
                states[clone].len = states[p].len + 1;

                for (; p != -1 && states[p].next[c] == q; p = states[p].slink) {
                    states[p].next[c] = clone;
                }

                states[q].slink = states[cur].slink = clone;
            }
        }
        last = cur;
    }

    void add(const std::string& s) {
        for (std::size_t i = 0; i < s.size(); ++i) {
            add(s[i]);
        }
    }

    std::vector<bool> getTerminalStates() const {
        std::vector<bool> res(states.size(), false);
        for (int i = last; i != -1; i = states[i].slink) {
            res[i] = true;
        }
        return res;
    }

    std::ostream& draw(std::ostream& oss) const {
        oss << "digraph {\n"
            "\trankdir=LR\n"
            "\tfontsize=24\n"
            "\tnode[shape=circle]\n"
            "\tedge[color=blue fontsize=16]\n"
            "\tlabel=\"Subword Graph of \\\"" << content << "\\\"\"\n"
            "\n\n";

        const std::vector<bool>& terms = getTerminalStates();

        for (std::size_t i = 0; i < states.size(); ++i) {

            //draw node
            oss << "\t" << i << "[label=\"" << states[i].len << "\"";
            if (terms[i]) {
                oss << " shape=doublecircle";
            }
            oss << "]\n";

            //draw slink
            if (states[i].slink != -1) {
                oss << "\t\t" << i << "->" << states[i].slink 
                    << "[color=dimgray constraint=false style=dashed]\n";
            }

            //draw edges
            for (Node::NextType::const_iterator itr = states[i].next.begin(); itr != states[i].next.end(); ++itr) {
                const Node::NextType::value_type& item = *itr;
                oss << "\t\t" << i << "->" << item.second 
                    << "[label=\"" << item.first << "\"";
                if (states[i].len + 1 != states[item.second].len) {
                    oss << " color=green";
                }
                oss << "]\n";
            }
            if (i + 1 != states.size()) {
                oss << "\n";
            }
        }
        return oss << "}";
    }

    SubWordGraph() : states(1), last(0) {}

private: 
    struct Node {
        Node() : len(0), slink(-1) {}
        typedef std::map<char, int> NextType;
        unsigned len;
        int slink;
        NextType next;
    };
    std::vector<Node> states;
    int last;
    std::string content;
};

std::ostream& operator<<(std::ostream& oss, const SubWordGraph& swg) {
    return swg.draw(oss);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: subw.tsk [word]" << std::endl;
        return EXIT_FAILURE;
    }
    const std::string s(argv[1]);
    SubWordGraph swg;
    swg.add(s);
    std::cout << swg << std::endl;
    return EXIT_SUCCESS;
}

