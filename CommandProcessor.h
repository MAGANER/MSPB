#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H
#include<map>
#include<functional>
#include<string>
#include<vector>
#include<iostream>
#include<filesystem>
#include<fstream>
#include"json.hpp"
using namespace std;
namespace fs = std::filesystem;
using json = nlohmann::json;

class CommandProcessor
{
private:
	map<string, function<void(const vector<string>&)>> commands;
	string curr_box;
	json* box = nullptr;
	enum class ErrorType
	{
		NotDefinedCommand
	};
	void error(ErrorType type);
	void create_new_box(const string& dir, const string& key);
	void open_box(const string& dir, const string& key);
	void make_directory(const string& dir_name, const vector<string>& files);
	void show_all_directories();

	bool check_table_exists(const string& name);
	bool check_arg_number(const vector<string>& args, int right_number);
public:
	CommandProcessor();
	~CommandProcessor();

	void process_command(const pair < string, vector<string>>& command);
	string get_cur_box()const;
};
#endif

