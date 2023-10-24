#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works! :)");
    sf::VertexArray triangle(sf::Triangles, 3);

    triangle[0].position = sf::Vector2f(400, 100);
    triangle[0].color = sf::Color::Red;

    triangle[1].position = sf::Vector2f(200, 500);
    triangle[1].color = sf::Color::Green;

    triangle[2].position = sf::Vector2f(600, 500);
    triangle[2].color = sf::Color::Blue;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(triangle);
        window.display();
    }

    return 0;
}