#include "Interface.h"
Interface::Interface(){}
Interface::~Interface(){}


string Interface::get_user_input()
{
	string result;
	getline(cin, result);
	return result;
}
pair<string, vector<string>> Interface::parse(const string& input)
{
	string command;
	vector<string> arguments;
    
    vector<string> splitted = split(input);
    command = splitted[0];

    for (auto beg = ++splitted.begin(); beg != splitted.end(); ++beg)
        arguments.push_back(*beg);

    return make_pair(command, arguments);
}
void Interface::run()
{
    while (true)
    {
        cout <<cmdp.get_cur_box()<< ">>";
        auto input = get_user_input();
        if (input.empty())continue;

        auto parsed = parse(input);
        cmdp.process_command(parsed);
        cout << endl;
    }
}
vector<string> Interface::split(const std::string& txt)
{
    vector<string> tokens;
    string token;

    for (const auto& c : txt) {
        if (!isspace(c))
            token += c;
        else {
            if (token.length()) tokens.push_back(token);
            token.clear();
        }
    }
    if (token.length()) tokens.push_back(token);
    return tokens;
}