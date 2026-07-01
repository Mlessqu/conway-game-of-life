#pragma once

#include "SFML/Graphics/View.hpp"
#include "SFML/System/Vector2.hpp"

namespace sf
{
    class RenderWindow;
}

namespace automata::core_loop
{
    struct Camera
    {
        sf::View world_view_;
        bool is_panning_ = false;
        sf::Vector2i last_pan_mouse_position_;
        float zoom_level_ = 1.f;

        void reset(const sf::RenderWindow& _render_window);
        void start_pan(sf::Vector2i _mouse_position);
        void stop_pan();
        void pan_to(sf::RenderWindow& _render_window, sf::Vector2i _mouse_position);
        void zoom_at(sf::RenderWindow& _render_window, sf::Vector2i _mouse_position, float _wheel_delta);
    };
} // automata::core_loop
