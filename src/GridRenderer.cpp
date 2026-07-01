#include "GridRenderer.h++"
#include "GameGrid.h++"
#include<SFML/Graphics.hpp>

namespace rendering
{
    constexpr sf::Color ALIVE_CELL_COLOR = sf::Color::Cyan;

    void add_line_to_grid(sf::VertexArray& _lines, const sf::Vertex& _start, const sf::Vertex& _end)
    {
        _lines.append(_start);
        _lines.append(_end);
    }


    void draw_grid(sf::RenderWindow& _render_window, const automata::GameGrid& _game_grid, const int _cell_size)
    {
        const sf::Vector2u grid_dimensions = _game_grid.get_grid_dimensions();
        const int grid_width_pixels = static_cast<int>(grid_dimensions.x * _cell_size);
        const int grid_height_pixels = static_cast<int>(grid_dimensions.y * _cell_size);
        sf::VertexArray grid_lines{sf::PrimitiveType::Lines};
        sf::Color grid_color{sf::Color::Black};
        for (int i = 0; i <= grid_width_pixels; i += _cell_size) //vertical
        {
            float kolumny = static_cast<float>(i);
            float wysokosc = static_cast<float>(grid_height_pixels);
            const sf::Vertex start = sf::Vertex(sf::Vector2f{kolumny, 0}, grid_color);
            const sf::Vertex end = sf::Vertex(sf::Vector2f{kolumny, wysokosc}, grid_color);
            add_line_to_grid(grid_lines, start, end);
        }
        for (int i = 0; i <= grid_height_pixels; i += _cell_size) //horizontal
        {
            const float wiersze = static_cast<float>(i);
            const float szerokosc = static_cast<float>(grid_width_pixels);
            const sf::Vertex start = sf::Vertex(sf::Vector2f{0, wiersze}, grid_color);
            const sf::Vertex end = sf::Vertex(sf::Vector2f{szerokosc, wiersze}, grid_color);
            add_line_to_grid(grid_lines, start, end);
        }
        _render_window.draw(grid_lines);
    }


    void draw_cells(sf::RenderWindow& _render_window, const automata::GameGrid& _game_grid, int _cell_size)
    {
        const std::vector<bool> cells = _game_grid.get_cells();
        const int width = _game_grid.get_grid_dimensions().x;


        size_t vertex_index = 0;
        sf::VertexArray alive_cells_verticies(sf::PrimitiveType::Triangles);
        alive_cells_verticies.resize(cells.size() * 6);


        for (int i = 0; i < cells.size(); ++i)
        {
            if (cells[i] == true)
            {
                int col = i % width;
                int row = i / width;
                float p_x = static_cast<float>(col * _cell_size);
                float p_y = static_cast<float>(row * _cell_size);
                float size = static_cast<float>(_cell_size);
                sf::Vector2f top_left = {p_x, p_y};
                sf::Vector2f top_right = {p_x + size, p_y};
                sf::Vector2f bottom_left = {p_x, p_y + size};
                sf::Vector2f bottom_right = {p_x + size, p_y + size};


                alive_cells_verticies[vertex_index + 0] = sf::Vertex(top_left, ALIVE_CELL_COLOR);
                alive_cells_verticies[vertex_index + 1] = sf::Vertex(top_right, ALIVE_CELL_COLOR);
                alive_cells_verticies[vertex_index + 2] = sf::Vertex(bottom_left, ALIVE_CELL_COLOR);
                alive_cells_verticies[vertex_index + 3] = sf::Vertex(top_right, ALIVE_CELL_COLOR);
                alive_cells_verticies[vertex_index + 4] = sf::Vertex(bottom_left, ALIVE_CELL_COLOR);
                alive_cells_verticies[vertex_index + 5] = sf::Vertex(bottom_right, ALIVE_CELL_COLOR);
                vertex_index += 6;
            }
        }
        _render_window.draw(alive_cells_verticies);
    }


    sf::Text make_text(const sf::Font& _font, const sf::String& _value, sf::Vector2f _pos,
                       const unsigned int _char_size, const sf::Color _color)
    {
        sf::Text text(_font, _value, _char_size);
        text.setPosition(_pos);
        text.setFillColor(_color);
        return text;
    }
}
