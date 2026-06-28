#include <SFML/Graphics.hpp>
#include "CoreLoop.h++"


int main()
{
    constexpr unsigned width = 800u;
    constexpr unsigned height = 600u;
    constexpr unsigned int max_fps = 12;
    //wait no I am actually caluclating cell size from drawing and coupling them for no reason
    constexpr sf::Vector2<unsigned> resolution {width,height};
    constexpr std::string_view window_title{"Game of LIfe"};


    sf::RenderWindow window(sf::VideoMode(resolution), window_title);
    window.setFramerateLimit(max_fps/2);
    automata::core_loop::CoreLoop core_loop;
    core_loop.game_loop(window);
}
