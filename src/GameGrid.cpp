//
// Created by Missqu on 24/06/2026.
//

#include "GameGrid.h++"

#include <algorithm>
#include <random>
#include <utility>

namespace automata
{
    int GameGrid::get_index(const int _x, const int _y) const
    {
        return _y * width_ + _x;
    }


    void GameGrid::resize_grid(int _new_width, int _new_height)
    {
        if (_new_width <= 0 || _new_height <= 0)
        {
            return;
        }

        const unsigned int new_width = static_cast<unsigned int>(_new_width);
        const unsigned int new_height = static_cast<unsigned int>(_new_height);
        std::vector<bool> new_cells(new_width * new_height, false);

        const unsigned int copy_width = std::min(width_, new_width);
        const unsigned int copy_height = std::min(height_, new_height);
        for (unsigned int y = 0; y < copy_height; ++y)
        {
            for (unsigned int x = 0; x < copy_width; ++x)
            {
                new_cells[y * new_width + x] = current_cells_[y * width_ + x];
            }
        }

        width_ = new_width;
        height_ = new_height;
        current_cells_ = std::move(new_cells);
        future_cells_.assign(current_cells_.size(), false);
    }


    unsigned int GameGrid::get_grid_size() const
    {
        return current_cells_.size();
    }


    GameGrid::GameGrid(const unsigned int _width, const unsigned int _height,
                       const unsigned int _cell_size) : width_(_width), height_(_height), cell_size_(_cell_size),
                                                        current_cells_(width_ * height_, false),
                                                        future_cells_(width_ * height_, false)
    {
    }


    bool GameGrid::apply_rules(const bool _is_alive, const int _neighbours)
    {
        if (!_is_alive)
        {
            if (_neighbours == 3) return true;
            return false;
        }

        if (_neighbours < 2) return false;
        if (_neighbours == 2 || _neighbours == 3) return true;
        if (_neighbours > 3) return false;
    }


    void GameGrid::simulate_step()
    {
        //first check cell loop
        int total_cell_size = current_cells_.size();
        future_cells_.resize(current_cells_.size());
        for (int current_cell_index = 0; current_cell_index < total_cell_size; ++current_cell_index)
        {
            int alive_neighbours = check_neighbours(current_cell_index);
            //apply rules of life based on amount of alive neightbours
            const bool is_alive = current_cells_[current_cell_index];
            future_cells_[current_cell_index] = apply_rules(is_alive,alive_neighbours);
        }
        std::swap(current_cells_, future_cells_);
    }


    int GameGrid::check_neighbours(const unsigned int _index) const
    {
        if (width_ == 0 || height_ == 0)
        {
            return 0;
        }

        const int width = static_cast<int>(width_);
        const int height = static_cast<int>(height_);
        const int x = static_cast<int>(_index % width_);
        const int y = static_cast<int>(_index / width_);

        int alive_neighbours = 0;
        for (int offset_y = -1; offset_y <= 1; ++offset_y)
        {
            for (int offset_x = -1; offset_x <= 1; ++offset_x)
            {
                if (offset_x == 0 && offset_y == 0)
                {
                    continue;
                }

                const int neighbour_x = (x + offset_x + width) % width;
                const int neighbour_y = (y + offset_y + height) % height;
                if (current_cells_[neighbour_y * width + neighbour_x])
                {
                    ++alive_neighbours;
                }
            }
        }
        return alive_neighbours;
    }


    void GameGrid::randomize_cells(float _alive_probability)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::bernoulli_distribution d(_alive_probability);

        for (size_t i = 0; i < current_cells_.size(); ++i)
        {
            current_cells_[i] = d(gen);
        }
    }


    void GameGrid::flip_grid_cell(const int _index)
    {
        if (_index < 0 || static_cast<size_t>(_index) >= current_cells_.size())
        {
            return;
        }
        current_cells_[_index] = !current_cells_[_index];
    }


    void GameGrid::flip_cell(const int _x, const int _y)
    {
        if (_x < 0 || _y < 0)
        {
            return;
        }
        if (static_cast<unsigned int>(_x) >= width_ || static_cast<unsigned int>(_y) >= height_)
        {
            return;
        }
        flip_grid_cell(get_index(_x, _y));
    }


    const std::vector<bool>& GameGrid::get_cells() const
    {
        return current_cells_;
    }


    sf::Vector2u GameGrid::get_grid_dimensions() const
    {
        return sf::Vector2u(width_, height_);
    }


    int GameGrid::calculate_grid_size() const
    {
        return width_ * height_;
    }


    //implementation details here
} // automata
