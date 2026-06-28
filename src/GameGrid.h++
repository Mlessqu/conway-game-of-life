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
        GameGrid(const unsigned int _window_pixel_width, const unsigned int _window_pixel_height, const unsigned int _cell_size): cell_size_(_cell_size)
        {
            const int row = _window_pixel_height / cell_size_;
            const int col = _window_pixel_width / cell_size_;
            const int size = row*col;
            std::vector<bool> initialized_cells(size, false);
            width_ = col;
            height_ = row;
            current_cells_ = std::move(initialized_cells);
            future_cells_ = std::move(initialized_cells);
            randomize_cells();
        }
        int get_index(int _x, int _y) const
        {
            return _y * width_ + _x;
        }
        void resize_grid(int _new_width, int _new_height)
        {
                //TODO:
        }
        unsigned int get_grid_size() const
        {
            return current_cells_.size();
        }
        void simulate_step();
        void randomize_cells(float _alive_probability = 0.2f);
        void flip_grid();
        std::vector<bool> get_cells() const { return current_cells_; }
        sf::Vector2u get_grid_dimensions() const {return sf::Vector2u(width_, height_);}
    private:
        int calculate_grid_size()
        {
            return width_*height_;
        }

        unsigned int width_;
        unsigned int height_;
        const unsigned int cell_size_;
        std::vector<bool> current_cells_;
        std::vector<bool> future_cells_;
    };
} // automata
