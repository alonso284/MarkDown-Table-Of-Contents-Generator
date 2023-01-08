#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cmath>

#define DEBUG

struct title{
	std::string name;
	int tab_size;
	int location;
	title(std::string, int, int);
};

title::title(std::string name, int tab_size, int location){
	this -> name = name;
	this -> tab_size = tab_size;
	this -> location = location;
}

// Mark Down path and Table dir
std::string markdown_path;
// Inputed File
std::vector< std::string > input_markdown_lines;
// Titles and their respective indexes
std::vector< title > titles;
// Commands and their respective arguments
std::map< std::string, std::string > commands;
// Formated Titles
std::vector< std::string > generated_titles;
// Input file ifstream
std::ifstream input_markdown_file;
// Max size of title string
int size_of_string = 80;
// Tab size
int tab_size = 4;
// Tab
std::string tab;
// prefix
std::string prefix;

void print_titles(){
	for(auto& title:titles)
		std::cout << title.name << ' ' << title.tab_size << title.location << std::endl;
}

std::string repeat_chars(char, int);
std::string tabs(int);
int next_number(int);
void create_titles(bool, bool, bool);
std::string hyperlink(std::string str);
int transform(int);

/* 
 *  -l links the table to the titles
 *  -p adds a prefix to the hyperlink of the titles
 *  -i includes the table to the markdown file
 *  -line string_size include the line in which the title is in
 *  -t tab_size
 */


int main(int argc, char* argv []){

	
	// Validate commands
	for(int cmd = 1; cmd < argc; cmd++){
#ifdef DEBUG
		std::cout << "argument " << cmd << "- " << argv[cmd] << std::endl;
#endif
		if(!std::strcmp(argv[cmd], "-l")) commands.insert({"-l",""});
		else
		if(!std::strcmp(argv[cmd], "-i")) commands.insert({"-i",""});
		else
		if(!std::strcmp(argv[cmd], "-line")){
			cmd++;
			if(cmd >= argc){
				std::cout << "No argument passed for -line" << std::endl;
				exit(EXIT_FAILURE);
			}
			int indx = 0;
			while(argv[cmd][indx])
				if(!isdigit(argv[cmd][indx++])){
					std::cout << "argument passed for -line is not a number" << std::endl;
					exit(EXIT_FAILURE);
				}
			size_of_string = std::stoi(argv[cmd]);
			commands.insert({"-line",""});
		}
		else
		if(!std::strcmp(argv[cmd], "-t")){
			cmd++;
			if(cmd >= argc){
				std::cout << "No argument passed for -t" << std::endl;
				exit(EXIT_FAILURE);
			}
			int indx = 0;
			while(argv[cmd][indx])
				if(!isdigit(argv[cmd][indx++])){
					std::cout << "argument passed for -t is not a number" << std::endl;
					exit(EXIT_FAILURE);
				}
			tab_size = std::stoi(argv[cmd]);
		}
		else
		if(!std::strcmp(argv[cmd], "-p")){
			cmd++;
			prefix = argv[cmd];
		}
		else
			markdown_path = argv[cmd];
	}

	tab = repeat_chars(' ', tab_size);

#ifdef DEBUG
	std::cout << "Path: " << markdown_path << std::endl;
#endif

	// Valite path was passed
	if(markdown_path.empty()){
		std::cout << "No file path was provided" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Open Markdown
	input_markdown_file.open(markdown_path);
	if(!input_markdown_file){
		std::cout << "Failed to open file with name " << markdown_path << std::endl;
		std::exit(EXIT_FAILURE);
	}

	// Read through Markdown and store values in input_markdown_lines
	// Put titles in titles with their current index size
	std::string line;
	while(std::getline(input_markdown_file, line)){
	/*
	 * Rules for Header creation
	 * 1. The first # should be before four spaces or one tab
	 * 2. All #'s should be together
	 */	
		input_markdown_lines.push_back(line);
		if(!line.empty() && line[0] == '#'){
			int hash_count = 0;
			while(hash_count < line.size() && line[hash_count] == '#') hash_count++;
			titles.push_back(title(line.substr(hash_count+1), hash_count, input_markdown_lines.size())); 
		}
	}
	input_markdown_file.close();

	if(input_markdown_lines[0] == "# Table of Contents"){
		std::cout << "Table of contents has already been created" << std::endl;
		exit(EXIT_FAILURE);
	}

#ifdef DEBUG
	print_titles();
#endif

	// Create formated titles	
	create_titles(commands.count("-l"), commands.count("-i"), commands.count("-line"));

	// Create index file
	std::ofstream markdown_table_of_contents;
	markdown_table_of_contents.open(markdown_path.substr(0,markdown_path.size()-3) + "_TOC.md");

	markdown_table_of_contents << "# Table of Contents" << std::endl;

	for(auto& title:generated_titles)
		markdown_table_of_contents << title << std::endl;

	if(commands.count("-i")){
		markdown_table_of_contents << std::endl;

		for(auto& line:input_markdown_lines)
			markdown_table_of_contents << line << std::endl;
	}

	markdown_table_of_contents.close();


	return 0;
}

std::string repeat_chars(char chr, int number_of_chars){
	std::string chrs;
	while(number_of_chars--)
		if (chr == '\t')
			chrs += tab;
		else
			chrs.push_back(chr);
	return chrs;
}

int next_number(int index_level){
	static std::vector<int> last_number;
	last_number.resize(index_level);
	return ++last_number.back();
}

void create_titles(bool include_hyperlink, bool include_in_file, bool include_line_number){
	for(auto& title:titles)
		generated_titles.push_back(repeat_chars('\t', title.tab_size-1) + std::to_string(next_number(title.tab_size)) + '.' + ' ' + 
				(include_hyperlink?
				 '[' + title.name + ']' + '(' + prefix + hyperlink(title.name) + ')':
				 title.name
				));

	if(include_line_number)
		for(int indx = 0; indx < generated_titles.size(); indx++)
			generated_titles[indx] += repeat_chars('.', size_of_string-generated_titles[indx].size()-std::floor(std::log10(titles[indx].location+(include_in_file?titles.size()+2:0)))) + std::to_string(titles[indx].location+(include_in_file?titles.size()+2:0));;
}

std::string hyperlink(std::string str){
	std::string res = str;
	std::transform(res.begin(), res.end(), res.begin(), transform);
	return res;
}

int transform(int ch){
	if(ch == ' ') return '-';
	else return std::tolower(ch);
}
