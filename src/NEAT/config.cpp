// config.cpp

#include "NEAT/config.hpp"

Config::Config(const string &filename) : filename(filename) {
    // load();
    _population_size = getInt("NEAT", "population_size", 150);
    _max_generations = getInt("NEAT", "max_generations", 100);
    _survival_threshold = getDouble("NEAT", "survival_threshold", 0.2);
}

/**
 * Loads the configuration from a file.
 * 
 * @return true if the configuration was successfully loaded, false otherwise.
 */
bool Config::load() {
    // Open the file
    filename = "../" + this->filename;
    ifstream file(filename);
    if (!file.is_open()) {
        // If the file could not be opened, print an error message and return false
        cerr << "Could not open file: " << filename << endl;
        return false;
    }


    // Parse the file line by line
    string line, current_section;
    while(getline(file, line)) {
        parseLine(line, current_section);
    }
    file.close();
    return true;
}

/**
 * Saves the configuration to a file.
 * 
 * @param filename The name of the file to save the configuration to.
 * @return true if the configuration was successfully saved, false otherwise.
 */
bool Config::save(const string &filename) const {
    // Open the file
    ofstream file(filename);
    if (!file.is_open()) {
        // If the file could not be opened, print an error message and return false
        cerr << "Could not open file: " << filename << endl;
        return false;
    }

    // Write the settings to the file
    for (const auto &section : settings) {
        file << "[" << section.first << "]" << endl;
        for (const auto &setting : section.second) {
            file << setting.first << " = " << setting.second << endl;
        }
        file << endl;
    }
    file.close();
    return true;
}

/**
 * Get a value from the configuration.
 * 
 * @param section The section of the configuration.
 * @param key The key of the setting.
 * @param default_value The default value to return if the setting is not found.
 * @return The value of the setting.
 */
int Config::getInt(const string &section, const string &key, int default_value) const {
    auto it = settings.find(section);
    if (it != settings.end()) {
        auto it2 = it->second.find(key);
        if (it2 != it->second.end()) {
            return stoi(it2->second);
        }
    }
    // If the setting is not found, return the default value
    return default_value;
}

double Config::getDouble(const string &section, const string &key, double default_value) const {
    auto it = settings.find(section);
    if (it != settings.end()) {
        auto it2 = it->second.find(key);
        if (it2 != it->second.end()) {
            return stod(it2->second);
        }
    }
    // If the setting is not found, return the default value
    return default_value;
}

string Config::getString(const string &section, const string &key, const string &default_value) const {
    auto it = settings.find(section);
    if (it != settings.end()) {
        auto it2 = it->second.find(key);
        if (it2 != it->second.end()) {
            return it2->second;
        }
    }
    // If the setting is not found, return the default value
    return default_value;
}

/**
 * Set a value in the configuration.
 * 
 * @param section The section of the configuration.
 * @param key The key of the setting.
 * @param value The value to set.
 */
void Config::setInt(const string &section, const string &key, int value) {
    settings[section][key] = to_string(value);
}

void Config::setDouble(const string &section, const string &key, double value) {
    settings[section][key] = to_string(value);
}

void Config::setString(const string &section, const string &key, const string &value) {
    settings[section][key] = value;
}

/**
 * Parse a line from the configuration file.
 * 
 * @param line The line to parse.
 * @param current_section The current section of the configuration.
 */
void Config::parseLine(const string &line, string &current_section) {
    // Trim whitespace from the line
    string trimmed_line = line;
    trimmed_line.erase(0, trimmed_line.find_first_not_of(" \t\n\r"));
    trimmed_line.erase(trimmed_line.find_last_not_of(" \t\n\r") + 1);

    // Check if the line is a comment or empty
    if (trimmed_line.empty() || trimmed_line[0] == '#') {
        return;
    }

    // Check if the line is a section
    if (trimmed_line[0] == '[' && trimmed_line[trimmed_line.size() - 1] == ']') {
        current_section = trimmed_line.substr(1, trimmed_line.size() - 2);
    } else {
        // Parse the key and value
        istringstream iss(trimmed_line);
        string key, value;
        if (getline(iss, key, '=') && getline(iss, value)) {
            // Trim whitespace from the key and value
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            settings[current_section][key] = value;
        }
    }
}