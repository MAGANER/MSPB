#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H
#include<map>
#include<functional>
#include<string>
#include<vector>
#include<iostream>
#include"sql/DataBase.h"
using namespace std;
using namespace SQLite3DataBaseTools;
class CommandProcessor
{
private:
	map<string, function<void(const vector<string>&)>> commands;
	DataBase* db = nullptr;

	enum class ErrorType
	{
		NotDefinedCommand
	};
	void error(ErrorType type);
	void create_new_box(const string& dir, const string& key);
	bool check_arg_number(const vector<string>& args, int right_number);
public:
	CommandProcessor();
	~CommandProcessor();

	void process_command(const pair < string, vector<string>>& command);
};
#endif

