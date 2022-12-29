#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// Delete default value
std::string markdown_path("./mark_down.md");
std::vector< std::string > input_markdown_lines;
std::vector< std::pair<int,std::string> > titles;

void print_titles(){
	for(auto& title:titles)
		std::cout << title.first << ' ' << title.second << std::endl;
}

std::string tabs(int);
int next_number(int);

int main(int argc, char* argv []){

	std::ifstream input_markdown_file;

	// Check if there is initial path
	
	// Open Markdown
	input_markdown_file.open(markdown_path);
	if(!input_markdown_file){
		std::cout << "Failed to open file with name " << markdown_path << std::endl;
		std::exit(EXIT_FAILURE);
	}
	// Read trough Markdown and store values in input_markdown_lines
	// Put titles in titles with their current index size
	std::string line;
	while(std::getline(input_markdown_file, line)){
		// Possible bugs, # is not the first character but its still a character
		if(!line.empty() && line[0] == '#'){
			int hash_count = 0;
			while(hash_count < line.size() && line[hash_count] == '#') hash_count++;
			titles.push_back({hash_count, line.substr(hash_count+1)}); 
		}
		input_markdown_lines.push_back(line);
	}
	input_markdown_file.close();

	// Remove print_titles()
	print_titles();
	
	// Create index file
	std::ofstream markdown_table_of_contents;
	markdown_table_of_contents.open(markdown_path.substr(0,markdown_path.size()-3) + "_TOC.md");

	markdown_table_of_contents << "# Table of Contents" << std::endl;

	for(auto& title:titles)
		markdown_table_of_contents << tabs(title.first-1) << next_number(title.first) << '.' << ' ' << title.second << std::endl;

	markdown_table_of_contents.close();
	
	// Ask if include in same file or not, if not, create markdown_path_index.md
	
	// Ask if too inlucde hyperlinks in initial file, if so, check if they exist, if not, create
	


	return 0;
}

std::string tabs(int number_of_tabs){
	std::string tabs;
	while(number_of_tabs--)
		tabs.push_back('\t');
	return tabs;
}

int next_number(int index_level){
	static std::vector<int> last_number;
	last_number.resize(index_level);
	return ++last_number.back();
}
