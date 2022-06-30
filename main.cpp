#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <string>

using namespace sf;
using namespace std;

const int WIDTH = 800, HEIGHT = 800, MAX_IT = 200;

static Vector2<double> offset = Vector2<double>(0, 0);
static Vector2i mousePos = Vector2i(0, 0);
static double scale = 0.25f;
static Font font;

static Shader* shader = nullptr;
static Texture* texture = nullptr;
static Sprite* screen = nullptr;
static int TYPE = 0;
static float baseValue = 500;
static float actualValue = 0;
static float mixsherScale = actualValue / baseValue;
static float baseValueAdd = 5000000;
static float actualValueAdd = 0;
static float mixsherScaleAdd = actualValueAdd / baseValueAdd;

void draw_fractal_2d(RenderWindow& window)
{

	shader->setUniform("res", Glsl::Vec2(window.getSize().x, window.getSize().y));
	shader->setUniform("TYPE", TYPE);
	shader->setUniform("iter", static_cast<int>(actualValue));
	shader->setUniform("constantValue", static_cast<int>(actualValueAdd));
	window.draw(*screen, shader);

}

void draw_fractal_3d(RenderWindow& window)
{

	shader->setUniform("res", Glsl::Vec2(window.getSize().x, window.getSize().y));
	shader->setUniform("TYPE", TYPE);
	shader->setUniform("iter", static_cast<int>(actualValue));
	shader->setUniform("POWER", static_cast<int>(actualValueAdd));
	shader->setUniform("Ambience", Glsl::Vec3(.2, .2, .2));
	shader->setUniform("ColorFractal", Glsl::Vec3(.6, .6, .6));
	window.draw(*screen, shader);

}

int main(int argc, char** argv)
{
	ContextSettings settings;
	settings.antialiasingLevel = 8;
	RenderWindow window(VideoMode(WIDTH, HEIGHT), "Fractals", Style::Default, settings);
	window.setFramerateLimit(60);
	font.loadFromFile("resource/AGENCYR.TTF");
	void (*draw_fractal)(RenderWindow&) = draw_fractal_2d;
	Shader* shader2d = new Shader();
	shader2d->loadFromFile("shaders/fractals2d.frag", sf::Shader::Type::Fragment);
	Shader* shader3d = new Shader();
	shader3d->loadFromFile("shaders/fractals3d.frag", sf::Shader::Type::Fragment);
	shader = shader2d;
	texture = new Texture();
	texture->create(1, 1);
	screen = new Sprite(*texture, IntRect(0, 0, WIDTH, HEIGHT));

	sf::RectangleShape butt2d(sf::Vector2f(70, 35));
	butt2d.setFillColor(Color(0, 255, 0));
	butt2d.setPosition(10, 10);

	sf::Text butt2dtb;
	butt2dtb.setFont(font);
	butt2dtb.setString("2D");
	butt2dtb.setCharacterSize(24);
	butt2dtb.setFillColor(Color::Black);
	butt2dtb.setPosition(butt2d.getPosition().x + butt2d.getSize().x / 2 - 10, butt2d.getPosition().y + 2);

	sf::RectangleShape butt3d(sf::Vector2f(70, 35));
	butt3d.setFillColor(Color(0, 255, 0));
	butt3d.setPosition(butt2d.getPosition().x + butt2d.getSize().x + 10, butt2d.getPosition().y);

	sf::Text butt3dtb(butt2dtb);
	butt3dtb.setFillColor(Color::Black);
	butt3dtb.setString("3D");
	butt3dtb.setPosition(butt3d.getPosition().x + butt3d.getSize().x / 2 - 10, butt3d.getPosition().y + 2);

	sf::RectangleShape buttMandel(sf::Vector2f(70, 35));
	buttMandel.setFillColor(Color(0, 255, 0));
	buttMandel.setPosition(butt2d.getPosition().x, butt2d.getPosition().y + butt2d.getSize().y + 10);

	sf::RectangleShape buttJulia(sf::Vector2f(70, 35));
	buttJulia.setFillColor(Color(0, 255, 0));
	buttJulia.setPosition(butt2d.getPosition().x + butt2d.getSize().x + 10, butt2d.getPosition().y + butt2d.getSize().y + 10);

	sf::RectangleShape buttKoch(sf::Vector2f(70, 35));
	buttKoch.setFillColor(Color(0, 255, 0));
	buttKoch.setPosition(butt2d.getPosition().x, buttJulia.getPosition().y + buttJulia.getSize().y + 10);

	sf::RectangleShape butt3dMandel(sf::Vector2f(70, 35));
	butt3dMandel.setFillColor(Color(0, 255, 0));
	butt3dMandel.setPosition(butt2d.getPosition().x, butt2d.getPosition().y + butt2d.getSize().y + 10);

	sf::RectangleShape butt3dFlower(sf::Vector2f(70, 35));
	butt3dFlower.setFillColor(Color(0, 255, 0));
	butt3dFlower.setPosition(butt2d.getPosition().x + butt2d.getSize().x + 10, butt2d.getPosition().y + butt2d.getSize().y + 10);

	sf::RectangleShape butt3dCircle(sf::Vector2f(70, 35));
	butt3dCircle.setFillColor(Color(0, 255, 0));
	butt3dCircle.setPosition(butt2d.getPosition().x, butt3dFlower.getPosition().y + butt3dFlower.getSize().y + 10);

	sf::Text text2dMandel(butt2dtb);
	text2dMandel.setString("Mandel");
	text2dMandel.setPosition(buttMandel.getPosition().x + buttMandel.getSize().x / 2 - 24, buttMandel.getPosition().y + 2);

	sf::Text text2dJulia(butt2dtb);
	text2dJulia.setString("Julia");
	text2dJulia.setPosition(buttJulia.getPosition().x + buttJulia.getSize().x / 2 - 15, buttJulia.getPosition().y + 2);

	sf::Text text2dKoch(butt2dtb);
	text2dKoch.setString("Koch");
	text2dKoch.setPosition(buttKoch.getPosition().x + buttKoch.getSize().x / 2 - 17, buttKoch.getPosition().y + 2);

	sf::Text text3dMandel(butt2dtb);
	text3dMandel.setString("Mandel");
	text3dMandel.setPosition(butt3dMandel.getPosition().x + butt3dMandel.getSize().x / 2 - 24, butt3dMandel.getPosition().y + 2);

	sf::Text text3dFlower(butt2dtb);
	text3dFlower.setString("Flower");
	text3dFlower.setPosition(butt3dFlower.getPosition().x + butt3dFlower.getSize().x / 2 - 24, butt3dFlower.getPosition().y + 2);

	sf::Text text3dCircle(butt2dtb);
	text3dCircle.setString("Circle");
	text3dCircle.setPosition(butt3dCircle.getPosition().x + butt3dCircle.getSize().x / 2 - 21, butt3dCircle.getPosition().y + 2);

	sf::RectangleShape line(Vector2f(butt2d.getSize().x * 2 + 10, 1));
	line.setFillColor(Color(0, 255, 0));
	line.setPosition(Vector2f(butt2d.getPosition().x, butt3dCircle.getPosition().y + butt3dCircle.getSize().y * 2 + 10));

	sf::CircleShape mixsher(10);
	mixsher.setFillColor(Color(0, 200, 0));

	sf::RectangleShape lineAdd(Vector2f(butt2d.getSize().x * 2 + 10, 1));
	lineAdd.setFillColor(Color(0, 255, 0));
	lineAdd.setPosition(Vector2f(butt2d.getPosition().x, butt3dCircle.getPosition().y + butt3dCircle.getSize().y * 4 + 10));

	sf::CircleShape mixsherAdd(10);
	mixsherAdd.setFillColor(Color(0, 200, 0));

	sf::Text textActualValue(butt2dtb);
	textActualValue.setFillColor(Color(0, 255, 0));
	textActualValue.setCharacterSize(20);
	textActualValue.setPosition(butt2d.getPosition().x, butt3dCircle.getPosition().y + butt3dCircle.getSize().y + 10);

	sf::Text textActualValueAdd(butt2dtb);
	textActualValueAdd.setFillColor(Color(0, 255, 0));
	textActualValueAdd.setCharacterSize(20);
	textActualValueAdd.setPosition(butt2d.getPosition().x, butt3dCircle.getPosition().y + butt3dCircle.getSize().y * 3 + 10);


	sf::Transformable* elements[22]{ &butt2d, &butt3d, &butt2dtb, &butt3dtb, &buttMandel,
									 &buttJulia, &buttKoch, &butt3dMandel, &butt3dFlower,
									 &butt3dCircle, &text2dMandel, &text2dJulia, &text2dKoch,
									 &text3dMandel, &text3dFlower, &text3dCircle, &line, &mixsher,
									 &textActualValue, &lineAdd, &mixsherAdd, &textActualValueAdd
	};

	RectangleShape* buttons2d[3] = { &buttMandel, &buttJulia, &buttKoch };
	RectangleShape* buttons3d[3] = { &butt3dMandel, &butt3dFlower, &butt3dCircle };

	bool d2 = true, d3 = false, mixsherMove = false, mixsherAddMove = false;
	butt2d.setFillColor(Color(0, 122, 0));

	float lWidth = WIDTH, lHeight = HEIGHT, scaleX = 1., scaleY = 1.;

	while (window.isOpen())
	{
		mixsherScale = actualValue / baseValue;
		mixsherScaleAdd = actualValueAdd / baseValueAdd;
		mousePos = Mouse::getPosition(window);
		mousePos.x = (float)mousePos.x * WIDTH / static_cast<float>(window.getSize().x);
		mousePos.y = (float)mousePos.y * HEIGHT / static_cast<float>(window.getSize().y);
		if (mixsherMove) {
			float lineScale = mousePos.x / (line.getSize().x + line.getPosition().x) / WIDTH * static_cast<float>(window.getSize().x);
			if (lineScale < 1. && lineScale >= 0.) actualValue = baseValue * lineScale;
		}
		else if (mixsherAddMove) {
			float lineScale = mousePos.x / (lineAdd.getSize().x + lineAdd.getPosition().x) / WIDTH * static_cast<float>(window.getSize().x);
			if (lineScale < 1. && lineScale >= 0.) actualValueAdd = baseValueAdd * lineScale;

		}
		mixsher.setPosition(line.getPosition().x + line.getSize().x * mixsherScale * WIDTH / static_cast<float>(window.getSize().x) - mixsher.getRadius() * WIDTH / static_cast<float>(window.getSize().x),
			line.getPosition().y - mixsher.getRadius() * HEIGHT / static_cast<float>(window.getSize().y));
		mixsherAdd.setPosition(lineAdd.getPosition().x + lineAdd.getSize().x * mixsherScaleAdd * WIDTH / static_cast<float>(window.getSize().x) - mixsherAdd.getRadius() * WIDTH / static_cast<float>(window.getSize().x),
			lineAdd.getPosition().y - mixsherAdd.getRadius() * HEIGHT / static_cast<float>(window.getSize().y));
		textActualValue.setString("ITER: " + to_string((int)(actualValue)));
		if (d2) textActualValueAdd.setString("CONST: " + to_string((int)(actualValueAdd)));
		else textActualValueAdd.setString("POWER: " + to_string((int)(actualValueAdd)));
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
			}
			else if (event.type == Event::EventType::MouseWheelMoved && window.hasFocus())
			{
				scale *= pow(1.1, event.mouseWheel.delta);
				if (scale < 0.01f)
					scale = 0.01f;
			}
			else if (event.type == Event::MouseButtonPressed) {
				if (mixsher.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
					mixsherMove = true;
				}
				else if (mixsherAdd.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
					mixsherAddMove = true;
				}
				else if (butt2d.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
					d2 = true;
					d3 = false;
					butt2d.setFillColor(Color(0, 122, 0));
					butt3d.setFillColor(Color(0, 255, 0));
					(*buttons2d[TYPE]).setFillColor(Color(0, 255, 0));
					(*buttons3d[TYPE]).setFillColor(Color(0, 255, 0));
					TYPE = 0;
					actualValue = 0;
					actualValueAdd = 0;
					baseValue = 500;
				}
				else if (butt3d.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
					d2 = false;
					d3 = true;
					butt2d.setFillColor(Color(0, 255, 0));
					butt3d.setFillColor(Color(0, 122, 0));
					(*buttons2d[TYPE]).setFillColor(Color(0, 255, 0));
					(*buttons3d[TYPE]).setFillColor(Color(0, 255, 0));
					TYPE = 0;
					actualValue = 0;
					actualValueAdd = 0;
					baseValueAdd = 6;
					baseValue = 100;
				}
				else if (d2) {
					if (buttMandel.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
						(*buttons2d[TYPE]).setFillColor(Color(0, 255, 0));
						TYPE = 0;
						actualValue = 0;
						actualValueAdd = 0;
						baseValue = 500;
					}
					else if (buttJulia.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
						(*buttons2d[TYPE]).setFillColor(Color(0, 255, 0));
						TYPE = 1;
						actualValue = 0;
						actualValueAdd = 0;
						baseValueAdd = 5000000;
						baseValue = 500;
					}
					else if (buttKoch.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
						(*buttons2d[TYPE]).setFillColor(Color(0, 255, 0));
						TYPE = 2;
						actualValue = 0;
						actualValueAdd = 0;
						baseValue = 40;
					}
				}
				else if (d3) {
					if (butt3dMandel.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
						(*buttons3d[TYPE]).setFillColor(Color(0, 255, 0));
						TYPE = 0;
						actualValue = 0;
						actualValueAdd = 0;
					}
					else if (butt3dFlower.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
						(*buttons3d[TYPE]).setFillColor(Color(0, 255, 0));
						TYPE = 1;
						actualValue = 0;
						actualValueAdd = 0;
					}
					else if (butt3dCircle.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
						(*buttons3d[TYPE]).setFillColor(Color(0, 255, 0));
						TYPE = 2;
						actualValue = 0;
						actualValueAdd = 0;
					}
				}
			}
			else if (event.type == Event::MouseButtonReleased) {
				mixsherMove = false;
				mixsherAddMove = false;
			}
			else if (event.type == sf::Event::Resized) {
				glViewport(0, 0, event.size.width, event.size.height);
				scaleX = static_cast<float>(lWidth) / static_cast<float>(window.getSize().x);
				scaleY = static_cast<float>(lHeight) / static_cast<float>(window.getSize().y);
				for (int i = 0; i < 22; ++i) {
					if (scaleX <= 1. || scaleY <= 1.) elements[i]->setScale(scaleX, scaleY);
					else elements[i]->setScale(1., 1.);
					elements[i]->setPosition(elements[i]->getPosition().x * scaleX, elements[i]->getPosition().y * scaleY);
				}
				lHeight = window.getSize().y;
				lWidth = window.getSize().x;
			}
		}
		if (d2 && shader != shader2d) {
			shader = shader2d;
			draw_fractal = draw_fractal_2d;
		}
		else if (d3 && shader != shader3d) {
			shader = shader3d;
			draw_fractal = draw_fractal_3d;
		}
		if (d2) (*buttons2d[TYPE]).setFillColor(Color(0, 122, 0));
		else if (d3) (*buttons3d[TYPE]).setFillColor(Color(0, 122, 0));

		window.clear();
		draw_fractal(window);
		window.draw(butt2d);
		window.draw(butt3d);
		window.draw(butt2dtb);
		window.draw(butt3dtb);
		window.draw(textActualValue);
		if (d2) {
			window.draw(buttMandel);
			window.draw(buttJulia);
			window.draw(buttKoch);
			window.draw(text2dMandel);
			window.draw(text2dJulia);
			window.draw(text2dKoch);
			if (TYPE == 1) {
				window.draw(mixsherAdd);
				window.draw(lineAdd);
				window.draw(textActualValueAdd);
			}
		}
		else if (d3) {
			window.draw(butt3dMandel);
			window.draw(butt3dFlower);
			window.draw(butt3dCircle);
			window.draw(text3dMandel);
			window.draw(text3dFlower);
			window.draw(text3dCircle);
			window.draw(mixsherAdd);
			window.draw(lineAdd);
			window.draw(textActualValueAdd);
		}
		window.draw(line);
		window.draw(mixsher);
		window.display();

	}

	return 0;
}