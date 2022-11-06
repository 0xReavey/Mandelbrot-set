#include <SFML/Graphics.hpp>
#include <array>
#include <future>
#include <iomanip>
#include <iostream>

#include "mandelbrot.hpp"

static constexpr std::size_t window_size = 600;

int main() {
    sf::RenderWindow window(sf::VideoMode(window_size, window_size), "Fractal", sf::Style::Close);
    sf::Image output_image;
    output_image.create(window_size, window_size);

    sf::Texture output_texture;
    output_texture.loadFromImage(output_image);

    sf::Sprite output_sprite;
    output_sprite.setTexture(output_texture);

    sf::Font font;

    if (!font.loadFromFile("../font/font.ttf")) {
        throw std::runtime_error("Font not found");
    }

    sf::Text text;
    text.setFont(font);
    text.setFillColor(sf::Color::White);
    text.setOutlineThickness(2);
    text.setOutlineColor(sf::Color::Black);
    text.setPosition(10, 5);

    long double zoom = 1.0;
    int iterations = 250;
    std::complex<long double> xyoff;
    std::array<std::array<sf::Color, window_size>, window_size> pixels;
    auto futures = std::vector<std::future<void>>(std::thread::hardware_concurrency());

    const size_t parts = futures.size();
    const size_t parts_size = window_size / parts;

    const auto make_pixels = [&pixels, &zoom, &xyoff, &iterations](const size_t start, const size_t end) {
        for (size_t x = start; x < end; x++) {
            for (size_t y = 0; y < window_size; y++) {
                long double px = std::lerp(-2.0, 2.0, static_cast<long double>(x) / window_size);
                long double py = std::lerp(-2.0, 2.0, static_cast<long double>(y) / window_size);
                int value = inSet(zoom * std::complex<long double>(py, px) + xyoff, iterations);
                pixels[x][y] = sf::Color(5 * value % 255, 3 * value % 155, 5 * value % 255);
            }
        }
    };

    sf::Clock clock;
    bool recalculate = true;

    window.setFramerateLimit(60);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                recalculate = true;
                if (event.key.code == sf::Keyboard::S) {
                    xyoff = {xyoff.real(), xyoff.imag() + zoom / 27};
                } else if (event.key.code == sf::Keyboard::W) {
                    xyoff = {xyoff.real(), xyoff.imag() - zoom / 27};
                } else if (event.key.code == sf::Keyboard::D) {
                    xyoff = {xyoff.real() + zoom / 27, xyoff.imag()};
                } else if (event.key.code == sf::Keyboard::A) {
                    xyoff = {xyoff.real() - zoom / 27, xyoff.imag()};
                } else if (event.key.code == sf::Keyboard::J) {
                    zoom *= 1.5;
                } else if (event.key.code == sf::Keyboard::K) {
                    zoom /= 1.5;
                } else if (event.key.code == sf::Keyboard::Up) {
                    iterations += 25;
                } else if (event.key.code == sf::Keyboard::Down) {
                    iterations -= 25;
                } else if (event.key.code == sf::Keyboard::P) {
                    output_image.saveToFile("fractal.jpg");
                }
            } else if (event.type == sf::Event::MouseButtonPressed) {
                recalculate = true;
                if (event.mouseButton.button == sf::Mouse::Left) {
                    xyoff += std::complex<long double>(std::lerp(-2.0, 2.0, static_cast<long double>(event.mouseButton.x) / window_size),
                                                       std::lerp(-2.0, 2.0, static_cast<long double>(event.mouseButton.y) / window_size))
                             * zoom;
                    zoom /= 1.5;
                } else if (event.mouseButton.button == sf::Mouse::Right) {
                    xyoff -= std::complex<long double>(std::lerp(-2.0, 2.0, static_cast<long double>(event.mouseButton.x) / window_size),
                                                       std::lerp(-2.0, 2.0, static_cast<long double>(event.mouseButton.y) / window_size))
                             * zoom;
                    zoom *= 1.5;
                }
            }
        }

        window.clear();

        if (recalculate) {
            for (size_t i = 0; i < parts; i++) {
                futures[i] = std::async(std::launch::async, make_pixels, i * parts_size, (i + 1) * parts_size);
            }
            for (auto& future : futures) {
                future.wait();
            }

            output_image.create(window_size, window_size, reinterpret_cast<uint8_t*>(pixels.data()));
            recalculate = false;
        }

        output_texture.update(output_image);
        output_sprite.setTexture(output_texture);
        window.draw(output_sprite);
        window.draw(text);
        window.display();

        auto text_builder = std::ostringstream();
        text_builder << std::setw(4) << int(1 / clock.restart().asSeconds()) << " fps\n";
        text_builder << std::setw(4) << iterations << " iters\n";
        text_builder << std::setw(4) << zoom << " zoom\n";
        text.setString(text_builder.str());
    }
}
