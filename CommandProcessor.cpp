#include "CommandProcessor.h"

CommandProcessor::CommandProcessor()
{
	commands["q"] = [&](const vector<string>&) { exit(-1); };
	commands["cls"] = [&](const vector<string>&) {system("cls"); };
	commands["new"] = [&](const vector<string>& args)
	{
		if (check_arg_number(args, 1))
			create_new_box(args[0]);
	};
	commands["open"] = [&](const vector<string>& args)
	{
		if (check_arg_number(args, 3))
			open_box(args[0],args[1],args[2]);
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
void CommandProcessor::create_new_box(const string& dir)
{
	if (!fs::exists(dir))
	{	
		using namespace Encryption;
		box["box"] = nullptr;
		ofstream file(dir);

		AutoSeededRandomPool prng;

		SecByteBlock key(AES::DEFAULT_KEYLENGTH);
		SecByteBlock iv(AES::BLOCKSIZE);
		prng.GenerateBlock(key, key.size());
		prng.GenerateBlock(iv, iv.size());
		
		std::string iv_converted(reinterpret_cast<const char*>(iv.data()), iv.size());
		std::string key_converted(reinterpret_cast<const char*>(key.data()), key.size());
		cout << "Remember next data!" << endl;
		cout << "key:" << key_converted << endl;
		cout << "iv:" << iv_converted << endl;
		cout << "-----------------" << endl;
		cout << "do you want to save encryption data in file?(y/n)";
		char choice;
		cin >> choice;
		if (choice == 'y')
		{
			string path;
			cout << "enter path:";
			cin >> path;

			ofstream data(path,ios::binary);
			data << key << " " << iv;
			data.close();
			cout << path << " is written!" << endl;
		}

		file << Encryption::encrypt(make_pair(key,iv),box.dump().c_str());
		cout << dir << " is created sucessfully!" << endl;

	}
}
void CommandProcessor::open_box(const string& dir, const string& key, const string& iv)
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