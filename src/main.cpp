#include "json.hpp"
#include "main.h"

#include <fstream>

#include <filesystem>
#include <functional>
#include <iostream>
#include <string>
#include <map>

namespace fs = std::filesystem;

const std::string VERSION = "0.1.0";

bool version(std::string args) {
	std::string msg = "LemLib file-system manager version: " + VERSION;

	std::cout << msg << std::endl; 

	return true;
}

// TODO: doesn't work
bool ls(std::string args) {
	std::cout << "Listing files..." << std::endl;

	for (const auto & entry : fs::directory_iterator("/usd/")) std::cout << entry.path() << std::endl; 

	return true;
}

bool cat(std::string args) {
	std::cout << "Reading file..." << std::endl;

	std::ifstream file("/usd/" + args);
	std::string line;

	while (std::getline(file, line)) {
		std::cout << line << std::endl;
	}

	return true;
}

// TODO: doesn't work
bool rm(std::string args) {
	std::cout << "Removing file..." << std::endl;

	fs::remove("/usd/" + args);

	return true;
}

bool write(std::string args) {
	std::cout << "Writing file..." << std::endl;

	json::JSON data = json::JSON::Load(args);

	std::cout << "Writing to file: " << data["file"].ToString() << std::endl;

	std::cout << "JSON data: '" << data << "'" << std::endl;

	std::ofstream file("/usd/" + data["file"].ToString());
	file << data["data"].ToString();

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

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {

}
