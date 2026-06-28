//
// Created by Missqu on 24/06/2026.
//

#include "GameGrid.h++"

#include <random>

namespace automata
{
    void GameGrid::simulate_step()
    {
        //first check cell loop
        int total_cell_size = current_cells_.size();
        future_cells_.resize(current_cells_.size());
        for (int current_cell_index = 0; current_cell_index < total_cell_size; ++current_cell_index)
        {
            int x = current_cell_index % width_;
            int y = current_cell_index / width_;

            const bool has_left = x > 0;
            const bool has_right = x < (width_-1);
            const bool has_up = y > 0;
            const bool has_down = y < (height_-1);

            int alive_neighbours = 0;

            if (has_up && has_left)
            {
                if (current_cells_[current_cell_index - 1 - width_]) ++alive_neighbours;
            }
            if (has_up)
            {
                if (current_cells_[current_cell_index - width_]) ++alive_neighbours;
            }
            if (has_up && has_right)
            {
                if (current_cells_[current_cell_index - width_ + 1]) ++alive_neighbours;
            }


            if (has_left)
            {
                if (current_cells_[current_cell_index - 1]) ++alive_neighbours;
            }
            if (has_right)
            {
                if (current_cells_[current_cell_index + 1]) ++alive_neighbours;
            }


            if (has_down && has_left)
            {
                if (current_cells_[current_cell_index + width_ - 1]) ++alive_neighbours;
            }
            if (has_down)
            {
                if (current_cells_[current_cell_index + width_]) ++alive_neighbours;
            }
            if (has_down && has_right)
            {
                if (current_cells_[current_cell_index + width_ + 1]) ++alive_neighbours;
            }
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
        //we check if it's within bounds, we don't care about how mouse maps to index, we expect the right index here that's all

            current_cells_[_index] = !current_cells_[_index];

    }


    //implementation details here
} // automata
