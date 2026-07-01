//
// Created by Missqu on 23/06/2026.
//

#include "CoreLoop.h++"
#include<SFML/Graphics.hpp>

#include <algorithm>
#include <cmath>

#include "GameGrid.h++"
#include "GridRenderer.h++"

namespace automata::core_loop
{
    constexpr unsigned int CELL_SIZE = 50;
    constexpr sf::Color ALIVE_CELL_COLOR = sf::Color::Cyan;
    constexpr int SIMULATION_DELTA_FPS = 5;
    constexpr int MINIMUM_FPS_ALLOWED = 1;
    constexpr int MAX_SIM_FPS_ALLOWED = 240;
    constexpr float ZOOM_STEP = 1.1f;
    constexpr float MIN_ZOOM = 0.25f;
    constexpr float MAX_ZOOM = 4.f;



    namespace
    {
        int convert_2d_1d(int _x, int _y, unsigned int _width);
        sf::Vector2f mouse_screen_pos_to_game_global_pos(sf::RenderWindow& _render_window,
                                                         sf::Vector2i _mouse_screen_pos,
                                                         const sf::View& _view);
    }


    void CoreLoop::change_simulation_fps(int _delta_fps)
    {
        const float current_step_seconds = fixed_time_step_.asSeconds();
        int current_sim_fps;
        if (current_step_seconds > 0.f)
        {
            current_sim_fps = static_cast<int>(1.f / current_step_seconds);
        }
        else
        {
            current_sim_fps = MINIMUM_FPS_ALLOWED;
        }


        const int new_sim_fps = std::clamp(
            current_sim_fps + _delta_fps,
            MINIMUM_FPS_ALLOWED,
            MAX_SIM_FPS_ALLOWED);
        fixed_time_step_ = sf::seconds(1.f / static_cast<float>(new_sim_fps));
    }


    void CoreLoop::game_loop(sf::RenderWindow& _render_window, const sf::Font& _font, const sf::Time _fixed_time_step)
    {
        camera_.world_view_ = _render_window.getDefaultView();
        camera_.is_panning_ = false;
        camera_.zoom_level_ = 1.f;
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


        _render_window.setView(camera_.world_view_);
        rendering::draw_cells(_render_window, _game_grid, CELL_SIZE);
        rendering::draw_grid(_render_window, _game_grid, CELL_SIZE);


        sf::Text pause_text = rendering::make_text(_font, is_paused_ ? "Paused" : "Running", {10.f, 10.f});
        sf::Text fps_text = rendering::make_text(_font, "FPS: " + std::to_string(_fps), {10.f, 32.f});
        sf::Text simulation_step = rendering::make_text(
            _font, "Sim step time in ms:" + std::to_string(fixed_time_step_.asMilliseconds()), {10.f, 52.f});

        _render_window.setView(_render_window.getDefaultView());
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
                if (mouse_pressed->button == sf::Mouse::Button::Right)
                {
                    camera_.is_panning_ = true;
                    camera_.last_pan_mouse_position_ = mouse_pressed->position;
                }

                if (mouse_pressed->button == sf::Mouse::Button::Left && is_paused_)
                {
                    auto global_mouse_position = mouse_screen_pos_to_game_global_pos(
                        _render_window, mouse_pressed->position, camera_.world_view_);
                    try_flip_cell_at_pixel(_render_window, _game_grid, global_mouse_position);
                    int x = static_cast<int>(std::floor(global_mouse_position.x / CELL_SIZE));
                    int y = static_cast<int>(std::floor(global_mouse_position.y / CELL_SIZE));

                    const sf::Vector2u grid_dimensions = _game_grid.get_grid_dimensions();
                    if (x >= 0 && y >= 0 &&
                        x < static_cast<int>(grid_dimensions.x) &&
                        y < static_cast<int>(grid_dimensions.y))
                    {
                        _game_grid.flip_grid_cell(convert_2d_1d(x, y, grid_dimensions.x));
                    }
                }
            }

            if (const auto* mouse_released = event->getIf<sf::Event::MouseButtonReleased>())
            {
                if (mouse_released->button == sf::Mouse::Button::Right)
                {
                    camera_.is_panning_ = false;
                }
            }

            if (const auto* mouse_moved = event->getIf<sf::Event::MouseMoved>())
            {
                if (camera_.is_panning_)
                {
                    const sf::Vector2f previous_world_position = _render_window.mapPixelToCoords(
                        camera_.last_pan_mouse_position_, camera_.world_view_);
                    const sf::Vector2f current_world_position = _render_window.mapPixelToCoords(
                        mouse_moved->position, camera_.world_view_);

                    camera_.world_view_.move(previous_world_position - current_world_position);
                    camera_.last_pan_mouse_position_ = mouse_moved->position;
                }
            }

            if (const auto* mouse_wheel = event->getIf<sf::Event::MouseWheelScrolled>())
            {
                if (mouse_wheel->wheel == sf::Mouse::Wheel::Vertical && mouse_wheel->delta != 0.f)
                {
                    const float wanted_zoom_factor = mouse_wheel->delta > 0.f ? 1.f / ZOOM_STEP : ZOOM_STEP;
                    const float new_zoom_level = std::clamp(
                        camera_.zoom_level_ * wanted_zoom_factor,
                        MIN_ZOOM,
                        MAX_ZOOM);
                    const float zoom_factor = new_zoom_level / camera_.zoom_level_;

                    const sf::Vector2f mouse_world_before = _render_window.mapPixelToCoords(
                        mouse_wheel->position, camera_.world_view_);
                    camera_.world_view_.zoom(zoom_factor);
                    camera_.zoom_level_ = new_zoom_level;
                    const sf::Vector2f mouse_world_after = _render_window.mapPixelToCoords(
                        mouse_wheel->position, camera_.world_view_);
                    camera_.world_view_.move(mouse_world_before - mouse_world_after);
                }
            }
        }
    }


    void CoreLoop::try_flip_cell_at_pixel(sf::RenderWindow& _render_window, GameGrid& _grid,
                                          sf::Vector2f _pixel_position)
    {
        //convert px to world position using camera worldview
        //convert world position to grid cords
        //call grid.flip_cell(x,y);
    }


    CoreLoop::CoreLoop()
    {
    }


    namespace
    {

        int convert_2d_1d(int _x, int _y, unsigned int _width)
        {
            return _y * _width + _x;
        }


        sf::Vector2f mouse_screen_pos_to_game_global_pos(sf::RenderWindow& _render_window,
                                                         const sf::Vector2i _mouse_screen_pos,
                                                         const sf::View& _view)
        {
            return _render_window.mapPixelToCoords(_mouse_screen_pos, _view);
        }
    }
} // automata
