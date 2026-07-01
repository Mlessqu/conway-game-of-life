//
// Created by Missqu on 23/06/2026.
//

#pragma once

#include <SFML/Graphics.hpp>

namespace sf
{
    class Font;
    class RenderWindow;
}

namespace automata
{
    struct GameGrid;
}

namespace automata::core_loop
{
    struct Camera
    {
        sf::View world_view_;
        bool is_panning_ = false;
        sf::Vector2i last_pan_mouse_position_;
        float zoom_level_ = 1.f;
    };
    class CoreLoop
    {
    public:
        CoreLoop();
        void game_loop(sf::RenderWindow& _render_window, const sf::Font& _font, sf::Time _fixed_time_step);

        [[nodiscard]] bool is_paused() const;
        void set_paused(bool _is_paused);
        void toggle_pause();
        void set_fixed_time_step(sf::Time _fixed_time_step);
    private:
        void try_flip_cell_at_pixel(sf::RenderWindow& _render_window, GameGrid& _grid, sf::Vector2i _pixel_position);

        void process_events(sf::RenderWindow& _render_window, GameGrid& _game_grid);

        void handle_key_pressed(sf::RenderWindow& _render_window, const sf::Event::KeyPressed& _key_pressed);
        void handle_mouse_pressed(sf::RenderWindow& _render_window, GameGrid& _game_grid,
                                  const sf::Event::MouseButtonPressed& _mouse_pressed);
        void handle_mouse_released(const sf::Event::MouseButtonReleased& _mouse_released);
        void handle_mouse_moved(sf::RenderWindow& _render_window, const sf::Event::MouseMoved& _mouse_moved);
        void handle_mouse_wheel(sf::RenderWindow& _render_window,
                                const sf::Event::MouseWheelScrolled& _mouse_wheel);

        void draw(sf::RenderWindow& _render_window, const GameGrid& _game_grid, const sf::Font& _font, unsigned int _fps) const;
        void change_simulation_fps(int _delta_fps);
        sf::Time fixed_time_step_;
        bool is_paused_ = false;
        Camera camera_;
    };
} // automata
