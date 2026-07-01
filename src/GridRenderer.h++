#pragma once
#include "SFML/Graphics/Color.hpp"
#include "SFML/System/String.hpp"
#include "SFML/System/Vector2.hpp"

namespace automata
{
    struct GameGrid;
}

namespace sf
{
    class VertexArray;
    class RenderWindow;
    class Text;
    class Vertex;
    class Font;
}

namespace rendering
{
    void draw_grid(sf::RenderWindow& _render_window, const automata::GameGrid& _game_grid, int _cell_size);
    void add_line_to_grid(sf::VertexArray& _lines, const sf::Vertex& _start, const sf::Vertex& _end);
    void draw_cells(sf::RenderWindow& _render_window, const automata::GameGrid& _game_grid, int _cell_size);
    sf::Text make_text(const sf::Font& _font, const sf::String& _value, sf::Vector2f _pos,
                       unsigned int _char_size = 18, sf::Color _color = sf::Color::Black);
}
