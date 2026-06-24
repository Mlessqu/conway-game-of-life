//
// Created by Missqu on 23/06/2026.
//

#include "CoreLoop.h++"
#include<SFML/Graphics.hpp>

namespace automata::core_loop
{
    constexpr unsigned int CELL_SIZE = 50;

    namespace
    {
        void draw(sf::RenderWindow& _render_window);
        void process_events(sf::RenderWindow& _render_window);
        void draw_grid(sf::RenderWindow& _render_window, int _cell_size);
    }


    void game_loop(sf::RenderWindow& _render_window)
    {
        while (_render_window.isOpen())
        {
            process_events(_render_window);
            draw(_render_window);
        }
    }


    namespace
    {
        void draw(sf::RenderWindow& _render_window)
        {
            constexpr sf::Color background_color{sf::Color::White};
            _render_window.clear(background_color);
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


        void draw_grid(sf::RenderWindow& _render_window, int _cell_size)
        {
            sf::Vector2u window_size = _render_window.getSize();
            sf::VertexArray grid_lines{sf::PrimitiveType::Lines};
            sf::Color grid_color{sf::Color::Black};
            for (int i = 0; i < window_size.x; i += _cell_size)
            {
                float kolumny = static_cast<float>(i);
                float wysokosc = static_cast<float>(window_size.y);
                grid_lines.append(sf::Vertex(sf::Vector2f{kolumny, 0}, grid_color));
                grid_lines.append(sf::Vertex(sf::Vector2f{kolumny, wysokosc}, grid_color));
            }


            for (int i = 0; i < window_size.y; i += _cell_size)
            {
                float wiersze = static_cast<float>(i);
                float szerokosc = static_cast<float>(window_size.x);
                grid_lines.append(sf::Vertex(sf::Vector2f{0, wiersze}, grid_color));
                grid_lines.append(sf::Vertex(sf::Vector2f{szerokosc, wiersze}, grid_color));
            }
            _render_window.draw(grid_lines);
        }
    }
} // automata
