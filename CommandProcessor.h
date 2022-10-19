#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H
#include<map>
#include<functional>
#include<string>
#include<vector>
#include<iostream>
using namespace std;
class CommandProcessor
{
private:
	map<string, function<void(const vector<string>&)>> commands;
	
	enum class ErrorType
	{
		NotDefinedCommand
	};
	void error(ErrorType type);
public:
	CommandProcessor();
	~CommandProcessor();

	void process_command(const pair < string, vector<string>>& command);
};
#endif

