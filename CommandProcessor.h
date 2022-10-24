#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H
#include<map>
#include<functional>
#include<string>
#include<vector>
#include<iostream>
#include<filesystem>
#include<fstream>
#include <iterator>
#include"json.hpp"
#include"encryption/Encryption.h"
using namespace std;
namespace fs = std::filesystem;
using json = nlohmann::json;

class CommandProcessor
{
private:
	map<string, function<void(const vector<string>&)>> commands;
	string curr_box;
	json box;

	string key, iv;

	enum class ErrorType
	{
		NotDefinedCommand
	};
	void error(ErrorType type);
	void create_new_box(const string& dir);
	void open_box(const string& dir, const string& key, const string& iv);
	void open_box(const string& dir, const string& key_iv_file);
	void make_directory(const string& dir_name, const vector<string>& files);
	void show_all_directories();
	void show_dir_content(const string& dir_name);

	bool check_arg_number(const vector<string>& args, int right_number);
	vector<string> slice(const vector<string>& vec, size_t beg,size_t end);
	vector<pair<string, string>> parse_files_names(const vector<string>& files);
	void load_file_to_dir(const string& dir,const  pair<string, string> &name_dir);

	string GetStreamAsString(const istream& in);
	string GetFileAsString(const string& filePath);

	void save();
public:
	CommandProcessor();
	~CommandProcessor();

	void process_command(const pair < string, vector<string>>& command);
	string get_cur_box()const;
};
#endif

