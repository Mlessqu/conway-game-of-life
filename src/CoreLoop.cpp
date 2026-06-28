//
// Created by Missqu on 23/06/2026.
//

#include "CoreLoop.h++"
#include<SFML/Graphics.hpp>

#include "GameGrid.h++"

namespace automata::core_loop
{
    constexpr unsigned int CELL_SIZE = 50;
    constexpr sf::Color ALIVE_CELL_COLOR = sf::Color::Cyan;
    namespace
    {
        void draw(sf::RenderWindow& _render_window, const GameGrid& _game_grid);
        void process_events(sf::RenderWindow& _render_window);
        void draw_grid(sf::RenderWindow& _render_window, int _cell_size);
        void add_line_to_grid(sf::VertexArray& _lines, const sf::Vertex& _start, const sf::Vertex& _end);
        void draw_cells(sf::RenderWindow& _render_window, const GameGrid& _game_grid, int _cell_size);
    }


    void game_loop(sf::RenderWindow& _render_window)
    {
        automata::GameGrid grid{_render_window.getSize().x, _render_window.getSize().y, CELL_SIZE};
        while (_render_window.isOpen())
        {
            process_events(_render_window);
            grid.simulate_step();
            draw(_render_window, grid);
        }
    }


    namespace
    {
        void draw(sf::RenderWindow& _render_window, const GameGrid& _game_grid)
        {
            constexpr sf::Color background_color{sf::Color::White};
            _render_window.clear(background_color);
            draw_cells(_render_window, _game_grid, CELL_SIZE);
            draw_grid(_render_window, CELL_SIZE);

            _render_window.display();
        }


        void process_events(sf::RenderWindow& _render_window)
        {
            while (const std::optional event = _render_window.pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                    _render_window.close();
            }
        }


        void add_line_to_grid(sf::VertexArray& _lines, const sf::Vertex& _start, const sf::Vertex& _end)
        {
            _lines.append(_start);
            _lines.append(_end);
        }


        void draw_grid(sf::RenderWindow& _render_window, const int _cell_size)
        {
            sf::Vector2u window_size = _render_window.getSize();
            sf::VertexArray grid_lines{sf::PrimitiveType::Lines};
            sf::Color grid_color{sf::Color::Black};
            for (int i = 0; i < window_size.x; i += _cell_size) //vertical
            {
                float kolumny = static_cast<float>(i);
                float wysokosc = static_cast<float>(window_size.y);
                const sf::Vertex start = sf::Vertex(sf::Vector2f{kolumny, 0}, grid_color);
                const sf::Vertex end = sf::Vertex(sf::Vector2f{kolumny, wysokosc}, grid_color);
                add_line_to_grid(grid_lines, start, end);
            }
            for (int i = 0; i < window_size.y; i += _cell_size) //horizontal
            {
                const float wiersze = static_cast<float>(i);
                const float szerokosc = static_cast<float>(window_size.x);
                const sf::Vertex start = sf::Vertex(sf::Vector2f{0, wiersze}, grid_color);
                const sf::Vertex end = sf::Vertex(sf::Vector2f{szerokosc, wiersze}, grid_color);
                add_line_to_grid(grid_lines, start, end);
            }
            _render_window.draw(grid_lines);
        }


        void draw_cells(sf::RenderWindow& _render_window, const GameGrid& _game_grid, int _cell_size)
        {
            const std::vector<bool> cells = _game_grid.get_cells();
            const int width = _game_grid.get_grid_dimensions().x;


            size_t vertex_index = 0;
            sf::VertexArray alive_cells_verticies(sf::PrimitiveType::Triangles);
            alive_cells_verticies.resize(cells.size()*6);


            for (int i = 0; i < cells.size(); ++i)
            {
                if (cells[i] == true)
                {
                    int col = i % width;
                    int row = i / width;
                    float p_x = static_cast<float>(col * _cell_size);
                    float p_y = static_cast<float>(row * _cell_size);
                    float size = static_cast<float>(_cell_size);
                    sf::Vector2f top_left = {p_x,p_y};
                    sf::Vector2f top_right = {p_x+size,p_y};
                    sf::Vector2f bottom_left = {p_x,p_y+size};
                    sf::Vector2f bottom_right = {p_x+size,p_y+size};


                    alive_cells_verticies[vertex_index+0] = sf::Vertex({top_left,ALIVE_CELL_COLOR});
                    alive_cells_verticies[vertex_index+1] = sf::Vertex({top_right,ALIVE_CELL_COLOR});
                    alive_cells_verticies[vertex_index+2] = sf::Vertex({bottom_left,ALIVE_CELL_COLOR});
                    alive_cells_verticies[vertex_index+3] = sf::Vertex({top_right,ALIVE_CELL_COLOR});
                    alive_cells_verticies[vertex_index+4] = sf::Vertex(bottom_left,ALIVE_CELL_COLOR);
                    alive_cells_verticies[vertex_index+5] = sf::Vertex(bottom_right,ALIVE_CELL_COLOR);
                    vertex_index+=6;
                }

            }
            _render_window.draw(alive_cells_verticies);
        }
    }
} // automata
