#ifndef INTERFACE_H
#define INTERFACE_H
#include<string>
#include<iostream>
#include<vector>
#include<utility>
#include"CommandProcessor.h"
using namespace std;
class Interface
{
private:
	CommandProcessor cmdp;
public:
	Interface();
	~Interface();
	void run();
private:
	string get_user_input();
	pair<string, vector<string>> parse(const string& input);
	vector<string> split(const std::string& txt);
};
#endif

