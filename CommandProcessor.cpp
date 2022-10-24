#include "CommandProcessor.h"

CommandProcessor::CommandProcessor()
{
	commands["q"] = [&](const vector<string>&) { save(); exit(-1); };
	commands["cls"] = [&](const vector<string>&) {system("cls"); };
	commands["new"] = [&](const vector<string>& args)
	{
		if (check_arg_number(args, 1))
			create_new_box(args[0]);
	};
	commands["open"] = [&](const vector<string>& args)
	{
		if (args.size() == 3)
			open_box(args[0], args[1], args[2]);
		else if (args.size() == 2)
			open_box(args[0], args[1]);
		else
			cout << "incorrect arguments!";
	};
	commands["md"] = [&](const vector<string>& args)
	{
		if (!args.empty())
			make_directory(args[0], slice(args, 1, args.size()));
		else
			make_directory(args[0], vector<string>());
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

		AutoSeededRandomPool rnd;

		SecByteBlock key(0x00, AES::DEFAULT_KEYLENGTH);
		rnd.GenerateBlock(key, key.size());

		SecByteBlock iv(AES::BLOCKSIZE);
		rnd.GenerateBlock(iv, iv.size());
		
		auto key_converted = Encryption::convert_bytes(key);
		auto iv_converted = Encryption::convert_bytes(iv);
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

			ofstream data(path);
			data <<"key:"<< key_converted << '\n' <<" iv:"<< iv_converted;
			data.close();
			cout << path << " is written!" << endl;
		}

		file << Encryption::encrypt(make_pair(key,iv),box.dump().c_str());
		cout << dir << " is created sucessfully!" << endl;

	}
}
void CommandProcessor::open_box(const string& dir, const string& key, const string& iv)
{
	this->key = key;
	this->iv = iv;
	
	Encryption::SecByteBlock _key = Encryption::convert_to_bytes(key);
	Encryption::SecByteBlock _iv = Encryption::convert_to_bytes(iv);

	if (fs::exists(dir))
	{
		ifstream box_file(dir,ios::binary);
		string encrypted_box;
		box_file >> encrypted_box;

		auto decrypted_box = Encryption::decrypt(make_pair(_key, _iv), encrypted_box);
		curr_box = dir;
		box = json::parse(decrypted_box);
	}
}
void CommandProcessor::open_box(const string& dir, const string& key_iv_file)
{
	if (fs::exists(dir))
	{
		ifstream key_iv(key_iv_file);
		string key, iv;
		key_iv >> key >> iv;

		auto pos1 = key.find(':');
		auto pos2 = iv.find(':');

		if (pos1 == string::npos or pos2 == string::npos)
		{
			cout << "can not read " << key_iv_file<<"!";
		}
		else
		{
			key = key.substr(pos1+1, key.size());
			iv = iv.substr(pos2+1, iv.size());
			open_box(dir, key, iv);
		}
	}
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
vector<string> CommandProcessor::slice(const vector<string>& vec, size_t beg, size_t end)
{
	vector<string> res;
	for (size_t n = beg; n < end; n++)res.push_back(vec[n]);
	return res;
}
void CommandProcessor::make_directory(const string& dir_name, const vector<string>& files)
{
	box[dir_name] = nullptr;
}
void CommandProcessor::show_all_directories()
{
	for (auto& el : box.items())
		cout << el.key() << " ";
}
void CommandProcessor::save()
{
	auto key_converted = Encryption::convert_to_bytes(key);
	auto iv_converted = Encryption::convert_to_bytes(iv);

	ofstream data(curr_box,ios::binary);
	string encrypted = Encryption::encrypt(make_pair(key_converted, iv_converted), box.dump().c_str());
	data << encrypted;
	data.close();
}