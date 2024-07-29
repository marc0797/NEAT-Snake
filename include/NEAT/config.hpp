// config.hpp

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>

using std::string, std::unordered_map, std::ifstream,
      std::ofstream, std::stoi, std::to_string, 
      std::stod, std::endl, std::cerr, 
      std::istringstream;

class Config {
    public:
        Config(const string &filename);
        bool load();
        bool save(const string &filename) const;

        // Getters
        int getInt(
            const string &section, 
            const string &key, 
            int default_value) const;
        
        double getDouble(
            const string &section,
            const string &key, 
            double default_value) const;
        
        string getString(
            const string &section,
            const string &key, 
            const string &default_value) const;

        int population_size() const { return _population_size; }
        int max_generations() const { return _max_generations; }
        float survival_threshold() const { return _survival_threshold; }

        // Setters
        void setInt(
            const string &section, 
            const string &key, 
            int value);
        
        void setDouble(
            const string &section, 
            const string &key, 
            double value);
        
        void setString(
            const string &section, 
            const string &key, 
            const string &value);

    private:
        string filename;
        unordered_map<string, unordered_map<string,string>> settings;

        // NEAT parameters
        int _population_size;
        int _max_generations;
        float _survival_threshold;

        void parseLine(const string &line, string &current_section);

};

#endif // CONFIG_HPP