#include <bits/stdc++.h>

using namespace std;

string unique(string s)
{
    string str;
    int len = s.length();
    for (int i = 0; i < len; i++)
    {
        char c = s[i];
        auto found = str.find(c);
        if (found == std::string::npos)
        {
            str += c;
        }
    }
    return str;
}

struct FA
{
    string start;                                       // start state
    vector<string> accept;                              // set of accepting states
    vector<string> lang;                                // set of input symbols
    vector<string> states;                              // set of states
    map<pair<string, string>, string> transition_table; // transition function; mapping state x lang => state
};

void setAcceptingStates(FA nfa, FA &dfa)
{
    vector<string> accepted;
    accepted.assign(nfa.accept.begin(), nfa.accept.end());
    for (int i = 0; i < dfa.states.size(); i++)
    {
        for (int j = 0; j < accepted.size(); j++)
        {
            if (dfa.states[i].find(accepted[j]) != std::string::npos)
            {
                dfa.accept.push_back(dfa.states[i]);
                break;
            }
        }
    }
}

void printDFA(FA dfa)
{

    // print Start State
    cout << "Start: " << dfa.start << endl;

    // print accepted states
    cout << "Accepted States: ";
    for (int i = 0; i < dfa.accept.size(); i++)
        cout << dfa.accept[i] << " ";
    cout << endl;

    // print input symbols
    cout << "Lang: ";
    for (int i = 0; i < dfa.lang.size(); i++)
        cout << dfa.lang[i] << " ";
    cout << endl;

    // print states
    cout << "States: ";
    for (int i = 0; i < dfa.states.size(); i++)
        cout << dfa.states[i] << " ";
    cout << endl;

    // print transition table
    cout << "Transition table:" << endl;
    for (auto &pair : dfa.transition_table)
        cout << pair.first.first << ", "
             << pair.first.second
             << " -> "
             << pair.second
             << endl;

    return;
}

FA convertToDFA(FA nfa)
{

    FA dfa;
    dfa.start.assign(nfa.start.begin(), nfa.start.end());
    dfa.lang.assign(nfa.lang.begin(), nfa.lang.end());
    dfa.states.push_back(dfa.start);

    queue<string> newStates;
    newStates.push(nfa.start);
    while (!newStates.empty())
    {
        string curState = newStates.front();
        int stateLen = curState.size();
        int langLen = nfa.lang.size();
        string newState;
        for (int i = 0; i < langLen; ++i)
        {
            newState.clear();
            for (int j = 0; j < stateLen; ++j)
            {
                string ch;
                ch += curState[j];
                if (nfa.transition_table[{ch, nfa.lang[i]}] != "$")
                {
                    newState += nfa.transition_table[{ch, nfa.lang[i]}];
                }
            }

            if (newState.empty())
                newState = "$";

            newState = unique(newState);
            sort(newState.begin(), newState.end());
            dfa.transition_table[{curState, nfa.lang[i]}] = newState;
            vector<string>::iterator itr;
            itr = find(dfa.states.begin(), dfa.states.end(), newState);
            if (itr == dfa.states.end())
            {
                newStates.push(newState);
                dfa.states.push_back(newState);
            }
        }
        newStates.pop();
    }
    setAcceptingStates(nfa, dfa);
    return dfa;
}

int main()
{
    FA nfa;

    nfa.start = "A";
    nfa.accept = {"C"};
    nfa.lang = {"a", "b"};
    nfa.states = {"A", "B", "C"};

    nfa.transition_table.insert({{"A", "a"}, "AB"});
    nfa.transition_table.insert({{"A", "b"}, "C"});
    nfa.transition_table.insert({{"B", "a"}, "A"});
    nfa.transition_table.insert({{"B", "b"}, "B"});
    nfa.transition_table.insert({{"C", "b"}, "AB"});

    FA dfa = convertToDFA(nfa);

    printDFA(dfa);
}