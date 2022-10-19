#include "CommandProcessor.h"

CommandProcessor::CommandProcessor()
{
	commands["q"] = [&](const vector<string>&) { exit(-1); };
}
CommandProcessor::~CommandProcessor()
{

}
void CommandProcessor::process_command(const pair <string, vector<string>>& command)
{
	if (commands.end() != commands.find(command.first))
	{
		commands[command.first](command.second);
	}
	else error(ErrorType::NotDefinedCommand);
}
void CommandProcessor::error(ErrorType type)
{
	if (type == ErrorType::NotDefinedCommand)
	{
		cout << "command is not defined!";
	}
}