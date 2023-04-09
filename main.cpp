#include <stdio.h>
#include <math.h>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

const int width = 1920;
const int height = 1080;

int max_iter = 128;
double zoom = 1.0;
double min_re = -3.375, max_re = 1.875;
double min_im = -1.5, max_im = 1.5;


int main()
{
    RenderWindow window(VideoMode(width, height), "Mandelbrot Set");
    
    Image image;
    image.create(width, height);
    Texture texture;
    Sprite sprite;
    Font font;
    font.loadFromFile("assets/OpenSans-Semibold.ttf");
    Text text;
    text.setFont(font);
    text.setCharacterSize(24);

    ///// BLACK AND WHITE /////
    // text.setFillColor(Color::Black);

    ///// COLORFUL /////
    text.setFillColor(Color::White);

    
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::S)
                    image.saveToFile("screenshot/set.png");

                if (event.key.code == Keyboard::R)
                {
                    min_re = -3.375, max_re = 1.875;
                    min_im = -1.5, max_im = 1.5;
                    max_iter = 128;
                    zoom = 1.0;
                }

                double dx = (max_re - min_re) * 0.3;
                double dy = (max_im - min_im) * 0.3;

                if (event.key.code == Keyboard::Left)  { min_re -= dx; max_re -= dx; }
                if (event.key.code == Keyboard::Right) { min_re += dx; max_re += dx; }
                if (event.key.code == Keyboard::Up)    { min_im -= dy; max_im -= dy; }
                if (event.key.code == Keyboard::Down)  { min_im += dy; max_im += dy; }

                if (event.key.code == Keyboard::Z) max_iter *= 2;
                if (event.key.code == Keyboard::X) max_iter /= 2;
                if (max_iter < 1) max_iter = 1;
            }

            if (event.type == Event::MouseButtonPressed)
            {
                auto zoom_x = [&](double z)
                {
                    double cr = min_re + (max_re - min_re)*event.mouseButton.x / width;
                    double ci = min_im + (max_im - min_im)*event.mouseButton.y / height;

                    double tminr = cr - (max_re - min_re) / 2 / z;
                    max_re = cr + (max_re - min_re) / 2 / z;
                    min_re = tminr;

                    double tmini = ci - (max_im - min_im) / 2 / z;
                    max_im = ci + (max_im - min_im) / 2 / z;
                    min_im = tmini;
                };

                if (event.mouseButton.button == Mouse::Left)
                {
                    zoom_x(5);
                    zoom *= 5;
                }

                if (event.mouseButton.button == Mouse::Right)
                {
                    zoom_x(1.0 / 5);
                    zoom /= 5;
                }
            }
        }

        window.clear();


        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                double cr = min_re + (max_re - min_re)*x / width;
                double ci = min_im + (max_im - min_im)*y / height;
                double re = 0, im = 0;
                int iter;

                for (iter = 0; iter < max_iter; iter++)
                {
                    double tr = re*re - im*im + cr;
                    im = 2 * re * im + ci;
                    re = tr;
                    if (re*re + im*im > 4) break;
                }

                ///// BLACK AND WHITE /////
                // int b = 1.0*(max_iter - iter) / max_iter * 255;
                // int g = b, r = b;
                // image.setPixel(x, y, Color(r, g, b));


                ///// COLORFUL /////
                int r = abs(sin(iter * (max_iter - iter))) * 255;
                int g = abs(cos(M_PI_2 + (max_iter - iter))) * 255, b = 0;
                image.setPixel(x, y, Color(r, g, b));
            }
        }
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        window.draw(sprite);

        char str[100];
        sprintf(str, "Max iterations: %d\nZoom: x%2.2lf", max_iter, zoom);
        text.setString(str);
        window.draw(text);


        window.display();
    }

    return 0;
}
