#include "Platform/Platform.hpp"

#include <list>

using namespace std;

int main()
{
	util::Platform platform;

	// Tamanho da janela
	int size = 400;
	// Tamanho da cobra/fruta
	int scaling = 10;

	sf::RenderWindow window;
	window.create(sf::VideoMode(size, size), "Snake");
	platform.setIcon(window.getSystemHandle());


	// Define os parametros visuais da cobra
	sf::RectangleShape body;
	body.setSize(sf::Vector2f(1, 1));
	body.setScale(scaling, scaling);
	body.setFillColor(sf::Color::White);

	// Define os parametros visuais da fruta
	sf::RectangleShape fruit;
	fruit.setSize(sf::Vector2f(1, 1));
	fruit.setScale(scaling, scaling);
	fruit.setFillColor(sf::Color::Red);


	// Ccobra
	list<pair<int, int>> snake;
	list<pair<int, int>>::iterator it;

	// Fruta
	int fruitX;
	int fruitY;

	// Variáveis de tempo
	sf::Event event;
	sf::Clock clock;
	sf::Time elapsed, delay;
	delay = sf::seconds(0.1); // Velocidade do jogo

	// Variáveis de controle do jogo (direcao, estado da fruta e gameover)
	pair<int, int> par;
	pair<int, int> dir;
	pair<int, int> lastDir;
	bool eaten;
	bool over = true; // Inicia como finalizado


	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();

		// Define a direção da cobra de acordo com a tecla pressionada
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			dir.first = -1;
			dir.second = 0;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			dir.first = 0;
			dir.second = -1;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			dir.first = 1;
			dir.second = 0;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			dir.first = 0;
			dir.second = 1;
		}

		// Verifica se jogo acabou
		// Reinicia caso sim
		if(over) {
			snake.clear();
			par = make_pair(size / scaling / 2, size / scaling / 2); // Centro do tabuleiro
			snake.push_front(par);
			over = false;
			eaten = true;
			dir = make_pair(0, 0);
			lastDir = make_pair(0, 0);
		}

		// Tick do jogo
		sf::Time elapsed = clock.getElapsedTime();
		if(elapsed.asSeconds() > delay.asSeconds()) {
			clock.restart();

			// Impede cobra de virar para a direção contrária
			if(snake.size() > 1)
				if(lastDir.first + dir.first == 0 && lastDir.second + dir.second == 0)
					dir = lastDir;

			// Cria outra fruta caso seja comida
			if(eaten) {
				fruitX = rand() % (size / scaling);
				fruitY = rand() % (size / scaling);
				fruit.setPosition(fruitX * scaling, fruitY * scaling);
				eaten = false;
			}
			window.draw(fruit);

			// Calcula o próximo movimento de acordo com a direção
			par.first = snake.front().first + dir.first;
			par.second = snake.front().second + dir.second;
			// "Teleporta" cobra para lado oposto caso ultrapasse os limites do tabuleiro
			if(par.first > size / scaling - 1)
				par.first = 0;
			if(par.first < 0)
				par.first = size / scaling;
			if(par.second > size / scaling - 1)
				par.second = 0;
			if(par.second < 0)
				par.second = size / scaling;
			snake.push_front(par);

			// Verifica se cobra comeu a fruta neste movimento
			if((snake.front().first == fruitX ) && (snake.front().second == fruitY)) {
				eaten = true;
			}
			else {
				snake.pop_back();
			}

			// Loop para renderizar cobra e verificar colisão
			for(it = snake.begin(); it != snake.end(); it++) {
				// Verifica se cabeça colidiu com o corpo
				if(it != snake.begin())
					if(snake.front().first == it->first && snake.front().second == it->second)
						over = true;

				// Renderiza
				body.setPosition(it->first * scaling, it->second * scaling);
				window.draw(body);
			}

			// Atualiza ultima direção
			lastDir = dir;

			window.display();
		}
	}

	return 0;
}
