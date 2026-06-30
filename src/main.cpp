#include <SFML/Graphics.hpp>
#include "CoreLoop.h++"


int main()
{
    constexpr unsigned width = 800u;
    constexpr unsigned height = 600u;
    constexpr unsigned int simulation_fps = 12;
    //wait no I am actually caluclating cell size from drawing and coupling them for no reason
    constexpr sf::Vector2<unsigned> resolution {width,height};
    constexpr std::string_view window_title{"Game of LIfe"};
    sf::Font default_font;
    if (!default_font.openFromFile("arial.ttf"))
    {
        return 1;
    }

    sf::RenderWindow window(sf::VideoMode(resolution), window_title);
    constexpr sf::Time fixed_time_step = sf::seconds(1.f / static_cast<float>(simulation_fps));
    automata::core_loop::CoreLoop core_loop;
    core_loop.game_loop(window, default_font, fixed_time_step);
}
