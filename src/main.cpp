#include "json.hpp"
#include "main.h"

#include <fstream>

#include <filesystem>
#include <functional>
#include <iostream>
#include <stdio.h>
#include <string>
#include <map>

namespace fs = std::filesystem;

const std::string VERSION = "0.2.0";

const std::string INDEX_FILE = "/usd/lemlib.index";

std::vector<std::string> split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

void writeToIndexFile(std::string path) {
	std::ofstream file(INDEX_FILE, std::ios_base::app);
	
	file << "/usd/" << path << std::endl;

	file.close();
}

std::vector<std::string> readIndexFile() {
	std::vector<std::string> files;

	std::ifstream file(INDEX_FILE);
	std::string line;

	while (std::getline(file, line)) {
		files.push_back(line);
	}

	return files;
}

bool version(std::string args) {
	std::string msg = "LemLib file-system manager version: " + VERSION;

	std::cout << msg << std::endl; 

	return true;
}

bool ls(std::string args) {
	std::vector<std::string> files = readIndexFile();

	for (std::string file : files) {
		std::cout << file << std::endl;
	}

	return true;
}

bool cat(std::string args) {
	std::ifstream file("/usd/" + args);
	std::string line;

	while (std::getline(file, line)) {
		std::cout << line << std::endl;
	}

	return true;
}

// TODO: doesn't work
bool rm(std::string args) {
	fs::remove("/usd/" + args);

	return true;
}

bool write(std::string args) {
	json::JSON data = json::JSON::Load(args);

	std::ofstream output;
	output.open("/usd/" + data["file"].ToString());

	for (json::JSON j : data["data"].ArrayRange()) {
		output << j.ToString() << std::endl;
	}

	output.close();

	writeToIndexFile(data["file"].ToString());

	return true;
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	std::map<std::string, std::function<bool(std::string)>> commands = {
		{"version", version},
		{"ls", ls},
		{"cat", cat},
		{"rm", rm},
		{"write", write}
	};

	while (true) {
		std::string input;
		std::cout << "lemlib> ";
		std::getline(std::cin, input);

		std::string command;
		std::string args;

		size_t space = input.find(' ');
		if (space != std::string::npos) {
			command = input.substr(0, space);
			args = input.substr(space + 1);
		} else {
			command = input;
		}

		if (commands.find(command) != commands.end()) {
			bool success = commands[command](args);
		
			if (!success) {
				std::cout << "Command failed: " << command << std::endl;
			}
		} else {
			std::cout << "Unknown command: " << command << std::endl;
		}
	}
}
