//
// Created by Missqu on 24/06/2026.
//

#pragma once
#include <vector>

#include "SFML/System/Vector2.hpp"

namespace automata
{
    struct GameGrid
    {
        GameGrid(unsigned _width, const unsigned int _height, const unsigned int _cell_size);
        int get_index(int _x, int _y) const;
        void resize_grid(int _new_width, int _new_height);
        unsigned int get_grid_size() const;
        void simulate_step();
        void randomize_cells(float _alive_probability = 0.2f);
        void flip_grid_cell(int _index);
        void flip_cell(int _x,int _y);
        const std::vector<bool>& get_cells() const;
        sf::Vector2u get_grid_dimensions() const;
    private:
        int check_neighbours(unsigned int _index) const;
        bool apply_rules(const bool _is_alive,const int _neighbours);
        int calculate_grid_size() const;

        unsigned int width_;
        unsigned int height_;
        const unsigned int cell_size_;
        std::vector<bool> current_cells_;
        std::vector<bool> future_cells_;
    };
} // automata
