#include "CommandProcessor.h"
#define SAY_TO_OPEN_BOX cout << "you should open box first!";

CommandProcessor::CommandProcessor()
{
	//init all commands, available from terminal
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
		if (!curr_box.empty())
		{
			if (!args.empty())
				make_directory(args[0], slice(args, 1, args.size()));
			else
				make_directory(args[0], vector<string>());
		}
		else SAY_TO_OPEN_BOX
	};
	commands["ls"] = [&](const vector<string>& args)
	{
		if (!curr_box.empty())
		{
			show_all_directories();
		}
		else SAY_TO_OPEN_BOX
	};
	commands["sdc"] = [&](const vector<string>& args)
	{
		if (!curr_box.empty())
		{
			if (!args.empty())
			{
				for (auto& arg : args)
				{
					if (box.find(arg) != box.end())
					{
						cout << "Content of " << arg << endl;
						show_dir_content(arg);
						cout << "-----------";
						cout << endl;
					}
				}
			}
			else
			{
				cout << "error:no arguments!" << endl;
			}
		}
		else SAY_TO_OPEN_BOX
	};
	commands["add"] = [&](const vector<string>& args)
	{
		if (!curr_box.empty())
		{
			auto dir = args[0];
			auto files = slice(args, 1, args.size());
			if (does_dir_exist(dir))
			{
				auto parsed = parse_files_names(dir,files);
				for (auto& f : parsed)load_file_to_dir(dir, f);
			}
			else
			{
				cout << dir << " doesn't exist inside box!";
			}
		}
		else SAY_TO_OPEN_BOX
	};
}
CommandProcessor::~CommandProcessor()
{
}
void CommandProcessor::process_command(const pair <string, vector<string>>& command)
{
	//find it in hash map, if it's not found, say entered command doesn't exist
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
	//dir where box will be created must not exist
	if (!fs::exists(dir))
	{	
		using namespace Encryption;
		box["box"] = nullptr;
		ofstream file(dir,ios::binary);

		//generate random key
		//then it can be wrote in file
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


		//clear current box, make it unusable
		box = nullptr;
	}
}
void CommandProcessor::open_box(const string& dir, const string& key, const string& iv)
{
	this->key = key;
	this->iv = iv;
	
	Encryption::SecByteBlock _key = Encryption::convert_to_bytes(key);
	Encryption::SecByteBlock _iv = Encryption::convert_to_bytes(iv);

	//if box exists, read entire file and try to decrypt it
	if (fs::exists(dir))
	{
		ifstream file(dir, ios::binary);
		std::stringstream strStream;
		strStream << file.rdbuf();
		string encrypted_box = strStream.str();
		file.close();

		auto decrypted_box = Encryption::decrypt(make_pair(_key, _iv), encrypted_box);
		curr_box = dir;
		box = json::parse(decrypted_box);
	}
	else
	{
		cout << dir << "doesn't exist!" << endl;
	}
}
void CommandProcessor::open_box(const string& dir, const string& key_iv_file)
{
	//this method is just like previous, but it read key and iv from file
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
	//make directory and optionally add files
	box[dir_name] = nullptr;

	auto _files = parse_files_names(dir_name,files);
	for (auto& f : _files)
	{
		load_file_to_dir(dir_name, f);
	}
	cout << dir_name << " is created sucessfully!";
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

	ofstream data(curr_box, ios::binary);
	string encrypted = Encryption::encrypt(make_pair(key_converted, iv_converted), box.dump().c_str());
	data << encrypted;
	data.close();
}
vector<pair<string, string>> CommandProcessor::parse_files_names(const string& dir,const vector<string>& files)
{
	/*
		there are two way to express files:
		1)name:dir
		2)dir
		
		if name is omitted, then it will have numeric name
	*/

	vector<pair<string, string>> result;

	int counter = get_max_el_number(dir);//get max numeric name of file in the directory
	for (auto& f : files)
	{
		auto sep_pos = f.find(":");
		if (sep_pos != string::npos)
		{
			auto name = f.substr(0, sep_pos);
			auto file_path = f.substr(sep_pos + 1, f.size());
			auto res = make_pair(name, file_path);
			result.push_back(res);
		}
		else
		{
			auto res = make_pair(to_string(counter), f);
			counter++;
			result.push_back(res);
		}
	}
	return result;
}
int CommandProcessor::get_max_el_number(const string& dir)
{
	//if it has no file max numeric name will be 0
	//so if it has, then name will be max name + 1
	int max = -1;
	auto is_digit = [&](const string& str)
	{
		for (auto& ch : str)
		{
			if (!isdigit(ch))return false;
		}
		return true;
	};
	for (auto& el : box[dir].items())
	{
		auto key = string(el.key().c_str());
		if (is_digit(key.c_str()))
		{
			auto digit_key = atoi(key.c_str());
			if (digit_key > max)max = digit_key;
		}
	}
	return max+1;
}
void CommandProcessor::load_file_to_dir(const string& dir, const  pair<string, string>& name_dir)
{
	map<string, string> data = { {GetFileAsString(name_dir.second), fs::path(name_dir.second).extension().string() } };
	
	if (box[dir][name_dir.first].empty())
	{
		json j_map(data);
		box[dir][name_dir.first] = j_map;
	}
	else
	{
		map<string, string> _data = box[dir][name_dir.first];
		_data[GetFileAsString(name_dir.second)] = fs::path(name_dir.second).extension().string();
		json j_map(_data);
		box[dir][name_dir.first] = j_map;

	}
}
void CommandProcessor::show_dir_content(const string& dir_name)
{
	auto ext = [&](const string& extension) 
	{
		return extension.empty() ? "no extension" : extension;
	};
	for (auto& f : box[dir_name].items())
	{
		map<string, string> data = f.value();
		for (auto& el : data)
		{
			cout << f.key() << " : " << ext(el.second.c_str()) << endl;
		}
	}
}
bool CommandProcessor::does_dir_exist(const string& name)
{
	for (auto& i : box.items())
	{
		if (i.key() == name) return true;
	}
	return false;
}
string CommandProcessor::GetStreamAsString(const istream& in)
{
	stringstream out;
	out << in.rdbuf();
	return out.str();
}
string CommandProcessor::GetFileAsString(const string& filePath)
{
	ifstream stream;
	try
	{
		// Set to throw on failure
		stream.exceptions(fstream::failbit | fstream::badbit);
		stream.open(filePath);
	}
	catch (system_error& error)
	{
		cerr << "Failed to open '" << filePath << "'\n" << error.code().message() << endl;
		return "Open fail";
	}

	return GetStreamAsString(stream);
}