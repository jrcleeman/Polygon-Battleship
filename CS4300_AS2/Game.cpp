#include "Game.h"

Game::Game(const std::string& config)
{
	init(config);
}

void Game::init(const std::string& path)
{
	//TODO read in config file here
	//use premade PlayerConfig, EnemyConfig, BulletConfig variables
	//read in window size
	std::ifstream fin(path);
	fin >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.S;

	//set up default window parameters
	m_window.create(sf::VideoMode(1280, 720), "Assignment 2");
	m_window.setFramerateLimit(60);

	spawnPlayer();
}

void Game::run()
{
	//TODO add pause functionality
	// some system should function while paused (rendering)
	// some systems shouldnt (movement/input)
	while (m_running)
	{
		m_entities.update();
		if (m_paused)
		{
			
		}
		sEnemySpawner();
		sMovement();
		sCollision();
		sUserInput();
		sRender();

		//increment the current frame
		// may need to be moved when pause implemented
		m_currentFrame++;
	}
}

//pause game function
void Game::setPaused(bool paused)
{
	//TODO
}

//respawn player in the middle of the screen
void Game::spawnPlayer()
{
	//TODO: finish adding all properties of the player with the correct values from the config

	//We create every entity by calling EntityManager.addEntity(tag)
	// This returns a std::shared_ptr<Entity> so we use auto to save typing
	auto entity = m_entities.addEntity("player");

	//give this entity a transform so it spwans at  (200,200) with velocity (1,1) and angle 0
	entity->cTransform = std::make_shared<CTransform>(Vec2(200.0f, 200.0f), Vec2(1.0f, 1.0f), 0.0f);

	//the entity's shape will have radius 32, 8 sides, dark grey fill, and red outline of thickness 4
	entity->cShape = std::make_shared<CShape>(32.0f, 8, sf::Color(10, 10, 10), sf::Color(255, 0, 0), 4.0f);

	//Add an input component to the player so that we can use inputs
	entity->cInput = std::make_shared <CInput>();

	//Since we want this Entity to be our player, set our Games's player variable to be this Entity
	// this goes slightly against the EntityManager paradigm, but we use the player so much its worth it
	m_player = entity;
}

//spwan an enemy at a random position
void Game::spawnEnemy()
{
	//TODO: make sure enemy is spwaned properly within the m_enemyConfig variables
	// the enemy must be spwaned completley within the bounds of the window

	//record when the most recent enemy was spwaned
	m_lastEnemySpawnTime = m_currentFrame;
}

//spwans the small enemies when a big one (input entity e) explodes
void Game::spawnSmallEnemies(std::shared_ptr<Entity> e)
{
	//TODO: spawn small enemies at the location of the input enemy e
	int verticies = e->cShape->circle.getPointCount();
	for (int i = 0; i < verticies; i++)
	{
		auto smallEnemy = m_entities.addEntity("small");
		smallEnemy->cScore = std::make_shared<CScore>(e->cScore->score * 2);
	}
	//when we create the smaller enemy we have to read the values of the original enemy
	//-spwan a number of small enemies equal to the veritcies of the original enemy
	//-set each small enemy to the same color as the original and half the size
}

//spawns a bullet from a given entity to a target location
void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& target)
{
	//TODO implement the spawnining of a bullet which travels towards target
	// bullet speed is given as scalar speed
	//you must set velocity by that in notes
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
	//implelemtn
}

void Game::sMovement()
{
	//TODO implement all entity movement in this function
	//you shld read the m_player->cInput component to determine if the player is moving
	Vec2 playervelocity;
	if (m_player->cInput->left)
	{
		playervelocity.x -= m_playerConfig.S;
	}
	if (m_player->cInput->right)
	{
		playervelocity.x += m_playerConfig.S;
	}

	m_player->cTransform->velocity = playervelocity;
	//sample movement speed update
	//once Vec2 is implemented:
	for (auto e : m_entities.getEntities())
	{
		e->cTransform->pos += e->cTransform->velocity;
	}
	m_player->cTransform->pos.x += m_player->cTransform->velocity.x;
	m_player->cTransform->pos.y += m_player->cTransform->velocity.y;
}

void Game::sLifeSpan()
{
	for (auto e : m_entities.getEntities())
	{
		if (!e->cLifespan) { continue; }
	}
	//TODO: implement all lifespan functionality 
	//for all entities
	// if no li
	// if entity has lifespan > 0  - subtract 1
	//if it has lifespan and is alive - scale alpha channel properly
	auto color = m_player->cShape->circle.getFillColor();
	int newAlpha = 100;
	sf::Color newColor(color.r, color.g, color.b, newAlpha);
	m_player->cShape->circle.setFillColor(newColor);
	//if it has a lifespan and lifespan is up - delete
	e->destroy();
}

void Game::sCollision()
{
	//collision btwn player and enemies
	for (auto p : m_entities.getEntities("player"))
	{
		for (auto e : m_entities.getEntities("enemy"))
		{
			//if collision 
			float dist = p->cTransform->pos.dist(e->cTransform->pos);
			p->cCollision->radius;
		}
	}

	//todo collisions btwn bullets and enemies

}

void Game::sEnemySpawner()
{
	//TODO code which implements enemy spawning shoudl go here
	//use m_currentFrame - m_lastEnemySpawnTime to determine
	//how long has it been since the enemy last spawned
}

void Game::sRender()
{
	//TODO change the code below to draw all of the entities
	//sample drawing of the player entity that we have created 
	m_window.clear();

	//set the position of the shape based on the entitys transform->pos
	m_player->cShape->circle.setPosition(m_player->cTransform->pos.x, m_player->cTransform->pos.y);

	//set the rotation of the shape based on the entitys transform->angle
	m_player->cTransform->angle += 1.0f;
	m_player->cShape->circle.setRotation(m_player->cTransform->angle);

	//draw the entity
	m_window.draw(m_player->cShape->circle);
	
	m_window.display();
}

void Game::sUserInput()
{
	//TODO handle user input here
	//note that you should only be setting the players input component variables here
	//you should not iomplemenet movement logic here
	//movement system will read the variables youi set in this function

	sf::Event event;
	while (m_window.pollEvent(event))
	{
		//this event triggers window close
		if (event.type == sf::Event::Closed)
		{
			m_running = false;
		}

		//evenmt triggers when a key is pressed
		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				std::cout << "W Key Pressed\n";
				//TODO set players input component "up" to true
				break;
			}
		}

		//this event triggers when a key is released
		if (event.type == sf::Event::KeyReleased)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				std::cout << "W Key Released\n";
				//TODO set players input component "up" to false
				break;
			}
		}

		//mouse event
		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				std::cout << "Left Mouse Button Clicked at (" << event.mouseButton.x << ", " << event.mouseButton.y << ")\n";
				//TODO call bullet spawn here
			}
			if (event.mouseButton.button == sf::Mouse::Right)
			{
				std::cout << "Right Mouse Button Clicked at (" << event.mouseButton.x << ", " << event.mouseButton.y << ")\n";
				//TODO call special bullet spawn here
			}
		}
	}

}
