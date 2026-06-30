//
// Created by Missqu on 23/06/2026.
//

#pragma once

#include <SFML/System/Time.hpp>

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
        void process_events(sf::RenderWindow& _render_window, GameGrid& _game_grid);
        void draw(sf::RenderWindow& _render_window, const GameGrid& _game_grid, const sf::Font& _font, unsigned int _fps) const;
        void change_simulation_fps(int _delta_fps);
        sf::Time fixed_time_step_;
        bool is_paused_ = false;
    };
} // automata
