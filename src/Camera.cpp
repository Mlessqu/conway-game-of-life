#include "Camera.h++"

#include <algorithm>

#include "SFML/Graphics/RenderWindow.hpp"

namespace automata::core_loop
{
    namespace
    {
        constexpr float ZOOM_STEP = 1.1f;
        constexpr float MIN_ZOOM = 0.25f;
        constexpr float MAX_ZOOM = 4.f;
    }


    void Camera::reset(const sf::RenderWindow& _render_window)
    {
        world_view_ = _render_window.getDefaultView();
        is_panning_ = false;
        zoom_level_ = 1.f;
    }


    void Camera::start_pan(const sf::Vector2i _mouse_position)
    {
        is_panning_ = true;
        last_pan_mouse_position_ = _mouse_position;
    }


    void Camera::stop_pan()
    {
        is_panning_ = false;
    }


    void Camera::pan_to(sf::RenderWindow& _render_window, const sf::Vector2i _mouse_position)
    {
        if (!is_panning_)
        {
            return;
        }

        const sf::Vector2f previous_world_position = _render_window.mapPixelToCoords(
            last_pan_mouse_position_, world_view_);
        const sf::Vector2f current_world_position = _render_window.mapPixelToCoords(
            _mouse_position, world_view_);

        world_view_.move(previous_world_position - current_world_position);
        last_pan_mouse_position_ = _mouse_position;
    }


    void Camera::zoom_at(sf::RenderWindow& _render_window, const sf::Vector2i _mouse_position,
                         const float _wheel_delta)
    {
        if (_wheel_delta == 0.f)
        {
            return;
        }

        const float wanted_zoom_factor = _wheel_delta > 0.f ? 1.f / ZOOM_STEP : ZOOM_STEP;
        const float new_zoom_level = std::clamp(
            zoom_level_ * wanted_zoom_factor,
            MIN_ZOOM,
            MAX_ZOOM);
        const float zoom_factor = new_zoom_level / zoom_level_;

        const sf::Vector2f mouse_world_before = _render_window.mapPixelToCoords(_mouse_position, world_view_);
        world_view_.zoom(zoom_factor);
        zoom_level_ = new_zoom_level;
        const sf::Vector2f mouse_world_after = _render_window.mapPixelToCoords(_mouse_position, world_view_);
        world_view_.move(mouse_world_before - mouse_world_after);
    }
} // automata::core_loop
