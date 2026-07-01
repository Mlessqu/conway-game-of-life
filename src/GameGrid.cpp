//
// Created by Missqu on 24/06/2026.
//

#include "GameGrid.h++"

#include <random>
#include <utility>

namespace automata
{
    GameGrid::GameGrid(const unsigned int _window_pixel_width, const unsigned int _window_pixel_height,
                       const unsigned int _cell_size): cell_size_(_cell_size)
    {
        const int row = _window_pixel_height / cell_size_;
        const int col = _window_pixel_width / cell_size_;
        const int size = row * col;
        std::vector<bool> initialized_cells(size, false);
        width_ = col;
        height_ = row;
        current_cells_ = std::move(initialized_cells);
        future_cells_ = std::move(initialized_cells);
    }


    int GameGrid::get_index(const int _x, const int _y) const
    {
        return _y * width_ + _x;
    }


    void GameGrid::resize_grid(int _new_width, int _new_height)
    {
        //TODO:
    }


    unsigned int GameGrid::get_grid_size() const
    {
        return current_cells_.size();
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
            //1 rule for dead cell
            if (!is_alive && (alive_neighbours == 3))
            {
                future_cells_[current_cell_index] = true;
            }
            else
            {
                //and 3 rules for alive cells
                if (alive_neighbours < 2) future_cells_[current_cell_index] = false;
                if (alive_neighbours == 2 || alive_neighbours == 3) future_cells_[current_cell_index] = true;
                if (alive_neighbours > 3) future_cells_[current_cell_index] = false;
            }
        }
        std::swap(current_cells_, future_cells_);
    }


    int GameGrid::check_neighbours(const unsigned int _index) const
    {
        int x = _index % width_;
        int y = _index / width_;

        const bool has_left = x > 0;
        const bool has_right = x < (width_-1);
        const bool has_up = y > 0;
        const bool has_down = y < (height_-1);

        int alive_neighbours = 0;

        if (has_up && has_left)
        {
            if (current_cells_[_index - 1 - width_]) ++alive_neighbours;
        }
        if (has_up)
        {
            if (current_cells_[_index - width_]) ++alive_neighbours;
        }
        if (has_up && has_right)
        {
            if (current_cells_[_index - width_ + 1]) ++alive_neighbours;
        }


        if (has_left)
        {
            if (current_cells_[_index - 1]) ++alive_neighbours;
        }
        if (has_right)
        {
            if (current_cells_[_index + 1]) ++alive_neighbours;
        }


        if (has_down && has_left)
        {
            if (current_cells_[_index + width_ - 1]) ++alive_neighbours;
        }
        if (has_down)
        {
            if (current_cells_[_index + width_]) ++alive_neighbours;
        }
        if (has_down && has_right)
        {
            if (current_cells_[_index + width_ + 1]) ++alive_neighbours;
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
