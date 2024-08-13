// species.hpp

#ifndef SPECIES_HPP
#define SPECIES_HPP

#include "genome.hpp"
#include <vector>

class Species {
    public:
        int _age;
        int _stagnation;

        Species(int id, Genome &g) : _id(id), _representative(g), _age(0), _stagnation(0) {
            _members.push_back(g);
        }

        // Default constructor with empty species
        Species() : _id(0), _age(0), _stagnation(0), _adjusted_fitness(0) {}

        void add_member(Genome &g) {
            _members.push_back(g);
        }

        void clear() {
            _members.clear();
        }

        void update_best() {
            for (const auto &genome : _members) {
                if (genome.fitness() > _best.fitness()) {
                    _best = genome;
                }
            }
        }

        int id() const { return _id; }
        float adjusted_fitness() const { return _adjusted_fitness; }

        void adjust_fitness() {
            _adjusted_fitness = 0;
            for (const auto &genome : _members) {
                _adjusted_fitness += genome.fitness() / _members.size();
            }
        }

        const Genome &representative() const { return _representative; }
        const Genome &best() const { return _best; }
        vector<Genome> &members() { return _members; }

        friend std::ostream& operator<<(std::ostream &os, const Species &s) {
            os << s._id << "\n";
            os << s._representative << "\n";
            os << s._members.size() << "\n";
            for (const auto &genome : s._members) {
                os << genome << "\n";
            }
            os << s._best << "\n";
            os << s._adjusted_fitness << "\n";
            return os;
        }

        friend std::istream& operator>>(std::istream &is, Species &s) {
            is >> s._id;
            is >> s._representative;
            int size;
            is >> size;
            s._members.clear();
            for (int i = 0; i < size; i++) {
                Genome g;
                is >> g;
                s._members.push_back(g);
            }
            is >> s._best;
            is >> s._adjusted_fitness;
            return is;
        }

    private:
        int _id;
        Genome _representative;
        vector<Genome> _members;
        Genome _best = Genome();
        float _adjusted_fitness;
};

bool is_compatible(const Genome &g1, const Genome &g2, const Config &config);

#endif // SPECIES_HPP