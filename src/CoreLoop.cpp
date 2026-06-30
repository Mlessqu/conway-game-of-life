//
// Created by Missqu on 23/06/2026.
//

#include "CoreLoop.h++"
#include<SFML/Graphics.hpp>

#include <algorithm>

#include "GameGrid.h++"

namespace automata::core_loop
{
    constexpr unsigned int CELL_SIZE = 50;
    constexpr sf::Color ALIVE_CELL_COLOR = sf::Color::Cyan;
    constexpr int SIMULATION_DELTA_FPS = 5;
    constexpr int MINIMUM_FPS_ALLOWED = 1;
    constexpr int MAX_SIM_FPS_ALLOWED = 240;


    void CoreLoop::change_simulation_fps(int _delta_fps)
    {
        const float current_step_seconds = fixed_time_step_.asSeconds();
        int current_sim_fps;
        if (current_step_seconds > 0.f)
        {
            current_sim_fps= static_cast<int>(1.f / current_step_seconds);
        }
        else
        {
            current_sim_fps= MINIMUM_FPS_ALLOWED;
        }


        const int new_sim_fps = std::clamp(
            current_sim_fps + _delta_fps,
            MINIMUM_FPS_ALLOWED,
            MAX_SIM_FPS_ALLOWED);
        fixed_time_step_ = sf::seconds(1.f / static_cast<float>(new_sim_fps));
    }


    namespace
    {
        void draw_grid(sf::RenderWindow& _render_window, int _cell_size);
        void add_line_to_grid(sf::VertexArray& _lines, const sf::Vertex& _start, const sf::Vertex& _end);
        void draw_cells(sf::RenderWindow& _render_window, const GameGrid& _game_grid, int _cell_size);
        int convert_2d_1d(int _x, int _y, unsigned int _width);
        sf::Vector2f mouse_screen_pos_to_game_global_pos(sf::RenderWindow& _render_window,
                                                         sf::Vector2i _mouse_screen_pos);
        sf::Text make_text(const sf::Font& _font, const sf::String& _value, sf::Vector2f _pos,
                           unsigned int _char_size = 18, sf::Color _color = sf::Color::Black);
    }


    void CoreLoop::game_loop(sf::RenderWindow& _render_window, const sf::Font& _font, const sf::Time _fixed_time_step)
    {
        fixed_time_step_ = _fixed_time_step;
        if (fixed_time_step_ <= sf::Time::Zero)
        {
            fixed_time_step_ = sf::seconds(1.f / static_cast<float>(MINIMUM_FPS_ALLOWED));
        }
        automata::GameGrid grid{_render_window.getSize().x, _render_window.getSize().y, CELL_SIZE};
        sf::Clock frame_clock;
        sf::Time accumulated_time = sf::Time::Zero;
        while (_render_window.isOpen())
        {
            const sf::Time frame_time = frame_clock.restart();
            const float frame_seconds = frame_time.asSeconds();
            int current_fps;
            if (frame_seconds > 0.f)
            {
                current_fps = static_cast<unsigned int>(1.f / frame_seconds);
            }
            else
            {
                current_fps = 0u;
            }
            process_events(_render_window, grid);
            if (!is_paused_)
            {
                accumulated_time += frame_time;
                while (accumulated_time >= fixed_time_step_)
                {
                    grid.simulate_step();
                    accumulated_time -= fixed_time_step_;
                }
            }
            else
            {
                accumulated_time = sf::Time::Zero;
            }
            draw(_render_window, grid, _font, current_fps);
        }
    }


    bool CoreLoop::is_paused() const
    {
        return is_paused_;
    }


    void CoreLoop::set_paused(const bool _is_paused)
    {
        is_paused_ = _is_paused;
    }


    void CoreLoop::toggle_pause()
    {
        is_paused_ = !is_paused_;
    }


    void CoreLoop::draw(sf::RenderWindow& _render_window, const GameGrid& _game_grid, const sf::Font& _font,
                        const unsigned int _fps) const
    {
        constexpr sf::Color background_color{sf::Color::White};
        _render_window.clear(background_color);
        draw_cells(_render_window, _game_grid, CELL_SIZE);
        draw_grid(_render_window, CELL_SIZE);
        sf::Text pause_text = make_text(_font, is_paused_ ? "Paused" : "Running", {10.f, 10.f});
        sf::Text fps_text = make_text(_font, "FPS: " + std::to_string(_fps), {10.f, 32.f});
        sf::Text simulation_step = make_text(
            _font, "Sim step time in ms:" + std::to_string(fixed_time_step_.asMilliseconds()), {10.f, 52.f});
        _render_window.draw(pause_text);
        _render_window.draw(fps_text);
        _render_window.draw(simulation_step);

        _render_window.display();
    }


    void CoreLoop::set_fixed_time_step(const sf::Time _fixed_time_step)
    {
        fixed_time_step_ = _fixed_time_step;
    }


    void CoreLoop::process_events(sf::RenderWindow& _render_window, GameGrid& _game_grid)
    {
        while (const std::optional event = _render_window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                _render_window.close();
            }

            if (const auto* key_pressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (key_pressed->code == sf::Keyboard::Key::Escape)
                {
                    _render_window.close();
                }
                if (key_pressed->code == sf::Keyboard::Key::Space)
                {
                    toggle_pause();
                }

                if (key_pressed->scancode == sf::Keyboard::Scan::NumpadMinus)
                {
                    change_simulation_fps(-SIMULATION_DELTA_FPS);
                }
                if (key_pressed->scancode == sf::Keyboard::Scan::NumpadPlus)
                {
                    change_simulation_fps(SIMULATION_DELTA_FPS);
                }
            }
            if (const auto* mouse_pressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouse_pressed->button == sf::Mouse::Button::Left && is_paused_)
                {
                    auto global_mouse_position =
                        mouse_screen_pos_to_game_global_pos(_render_window, mouse_pressed->position);
                    const float global_px = global_mouse_position.x;
                    const float global_py = global_mouse_position.y;
                    int x = global_px / CELL_SIZE;
                    int y = global_py / CELL_SIZE;

                    const unsigned int width = _game_grid.get_grid_dimensions().x;
                    _game_grid.flip_grid_cell(convert_2d_1d(x, y, width));
                }
            }
        }
    }


    CoreLoop::CoreLoop()
    {
    }


    namespace
    {
        sf::Text make_text(const sf::Font& _font, const sf::String& _value, sf::Vector2f _pos,
                           const unsigned int _char_size, const sf::Color _color)
        {
            sf::Text text(_font, _value, _char_size);
            text.setPosition(_pos);
            text.setFillColor(_color);
            return text;
        }


        int convert_2d_1d(int _x, int _y, unsigned int _width)
        {
            return _y * _width + _x;
        }


        sf::Vector2f mouse_screen_pos_to_game_global_pos(sf::RenderWindow& _render_window,
                                                         const sf::Vector2i _mouse_screen_pos)
        {
            return _render_window.mapPixelToCoords(_mouse_screen_pos);
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


                    alive_cells_verticies[vertex_index + 0] = sf::Vertex({top_left, ALIVE_CELL_COLOR});
                    alive_cells_verticies[vertex_index + 1] = sf::Vertex({top_right, ALIVE_CELL_COLOR});
                    alive_cells_verticies[vertex_index + 2] = sf::Vertex({bottom_left, ALIVE_CELL_COLOR});
                    alive_cells_verticies[vertex_index + 3] = sf::Vertex({top_right, ALIVE_CELL_COLOR});
                    alive_cells_verticies[vertex_index + 4] = sf::Vertex(bottom_left, ALIVE_CELL_COLOR);
                    alive_cells_verticies[vertex_index + 5] = sf::Vertex(bottom_right, ALIVE_CELL_COLOR);
                    vertex_index += 6;
                }
            }
            _render_window.draw(alive_cells_verticies);
        }
    }
} // automata
