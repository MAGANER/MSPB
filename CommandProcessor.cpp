#include "CommandProcessor.h"

CommandProcessor::CommandProcessor()
{
	commands["q"] = [&](const vector<string>&) { exit(-1); };
	commands["new"] = [&](const vector<string>& args)
	{
		if (check_arg_number(args, 2))
			create_new_box(args[0], args[1]);
	};
}
CommandProcessor::~CommandProcessor()
{
	if (db != nullptr) delete db;
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
void CommandProcessor::create_new_box(const string& dir, const string& key)
{
	db = new DataBase(dir, key, true);
	if (db->is_ok())
		cout << dir << " created sucessfully!" << endl;
	else
		cout << dir << " error:" << db->get_error_message() << endl;
}
bool CommandProcessor::check_arg_number(const vector<string>& args, int right_number)
{
	if (args.size() != right_number)
	{
		cout << "incorrect number of arguments! must be " << right_number << endl;
		return false;
	}
	return true;
}