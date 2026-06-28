//
// Created by Missqu on 23/06/2026.
//

#pragma once

namespace sf
{
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
        void game_loop(sf::RenderWindow& _render_window);

        [[nodiscard]] bool is_paused() const;
        void set_paused(bool _is_paused);
        void toggle_pause();

    private:
        void process_events(sf::RenderWindow& _render_window, GameGrid& _game_grid);
        void draw(sf::RenderWindow& _render_window, const GameGrid& _game_grid) const;

        bool is_paused_ = false;
    };
} // automata
