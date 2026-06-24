#include <SFML/Graphics.hpp>
#include "CoreLoop.h++"
int main()
{
    constexpr sf::Vector2<unsigned> resolution {800,600};
    constexpr std::string_view window_title{"window title"};


    sf::RenderWindow window(sf::VideoMode(resolution), window_title);
    automata::core_loop::game_loop(window);
}