#include <SFML/Graphics.hpp>
#include "CoreLoop.h++"


int main()
{
    constexpr unsigned window_width = 800u;
    constexpr unsigned window_height = 600u;


    constexpr unsigned int simulation_fps = 12;
    constexpr unsigned int sim_width = 1000;
    constexpr unsigned int sim_height = 1000;

    constexpr sf::Vector2<unsigned> resolution {window_width,window_height};
    constexpr std::string_view window_title{"Game of LIfe"};
    sf::Font default_font;
    if (!default_font.openFromFile("arial.ttf"))
    {
        return 1;
    }

    sf::RenderWindow window(sf::VideoMode{resolution}, window_title);
    constexpr sf::Time fixed_time_step = sf::seconds(1.f / static_cast<float>(simulation_fps));
    automata::core_loop::CoreLoop core_loop;
    core_loop.game_loop(window, default_font, fixed_time_step);
}
