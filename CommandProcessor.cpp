#include "CommandProcessor.h"

CommandProcessor::CommandProcessor()
{
	commands["q"] = [&](const vector<string>&) { exit(-1); };
	commands["cls"] = [&](const vector<string>&) {system("cls"); };
	commands["new"] = [&](const vector<string>& args)
	{
		if (check_arg_number(args, 2))
			create_new_box(args[0], args[1]);
	};
	commands["open"] = [&](const vector<string>& args)
	{
		if (check_arg_number(args, 2))
			open_box(args[0], args[1]);
	};
	commands["md"] = [&](const vector<string>& args)
	{
		if (!args.empty())
			make_directory(args[0],args);
	};
	commands["ls"] = [&](const vector<string>& args)
	{
		show_all_directories();
	};
}
CommandProcessor::~CommandProcessor()
{
	if (box != nullptr)delete box;
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
}
void CommandProcessor::open_box(const string& dir, const string& key)
{

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
string CommandProcessor::get_cur_box()const
{
	return curr_box;
}
void CommandProcessor::make_directory(const string& dir_name, const vector<string>& files)
{

}
bool CommandProcessor::check_table_exists(const string& name)
{

}
void CommandProcessor::show_all_directories()
{

}