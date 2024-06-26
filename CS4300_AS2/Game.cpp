#include "Game.h"

Game::Game(const std::string& config)
{
	init(config);
}

void Game::init(const std::string& path)
{
	// Read in config file 
	std::ifstream fin(path);
	unsigned int  count = 0;
	for (std::string line; std::getline(fin, line);)
	{
		std::istringstream iss(line);
		std::string        name;
		if (count == 0) // Window
		{
			iss >> name >> m_windowConfig.W >> m_windowConfig.H >> m_windowConfig.FR 
				>> m_windowConfig.FS;
		}
		else if (count == 1) // Font
		{
			iss >> name >> m_fontConfig.FN >> m_fontConfig.FS >> m_fontConfig.FR 
				>> m_fontConfig.FG >> m_fontConfig.FB;
		}
		else if (count == 2) // Player
		{
			iss >> name >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.S
				>> m_playerConfig.FR >> m_playerConfig.FG >> m_playerConfig.FB >> m_playerConfig.OR
				>> m_playerConfig.OG >> m_playerConfig.OB >> m_playerConfig.OT >> m_playerConfig.V;
		}
		else if (count == 3) // Enemy
		{
			iss >> name >> m_enemyConfig.SR >> m_enemyConfig.CR >> m_enemyConfig.SMIN
				>> m_enemyConfig.SMAX >> m_enemyConfig.OR >> m_enemyConfig.OG >> m_enemyConfig.OB
				>> m_enemyConfig.OT >> m_enemyConfig.VMIN >> m_enemyConfig.VMAX >> m_enemyConfig.L
				>> m_enemyConfig.SI;
		}
		else if (count == 4) // Bullet
		{
			iss >> name >> m_bulletConfig.SR >> m_bulletConfig.CR >> m_bulletConfig.S
				>> m_bulletConfig.FR >> m_bulletConfig.FG >> m_bulletConfig.FB >> m_bulletConfig.OR
				>> m_bulletConfig.OG >> m_bulletConfig.OB >> m_bulletConfig.OT >> m_bulletConfig.V
				>> m_bulletConfig.L;
		}
		count++;
	}

	// Window set up
	m_window.create(sf::VideoMode(m_windowConfig.W, m_windowConfig.H), "Assignment 2");
	m_window.setFramerateLimit(m_windowConfig.FR);
	ImGui::SFML::Init(m_window);

	// Font set up
	m_font.loadFromFile(m_fontConfig.FN); // Select font file
	m_text.setCharacterSize(m_fontConfig.FS); // Font size
	m_text.setFont(m_font); // Set font
	m_text.setFillColor(sf::Color(m_fontConfig.FR, m_fontConfig.FG, m_fontConfig.FB));
	
	// Spawn player
	spawnPlayer();
}

void Game::run()
{
	while (m_running)
	{
		m_entities.update();
		imguiCall(); 
		if (m_paused)
		{
			sUserInput(); 
			sScoreText();
			sRender();
		}
		else
		{
			if (m_lifespan)  { sLifeSpan(); }
			sEnemySpawner();
			if (m_movement)  { sMovement(); }
			if (m_collision) { sCollision(); }
			sUserInput();
			sScoreText();
			sRender();
			m_currentFrame++;
		}
		if (m_player->isActive () == false)
		{
			spawnPlayer();
		}		
	}
	
	ImGui::SFML::Shutdown();
}

void Game::imguiCall()
{
	ImGui::SFML::Update(m_window, m_deltaClock.restart());
	ImGui::ShowDemoWindow();
	ImGui::Begin("Control Panel");
	//ImGui::Text("Window text!");
	//ImGui::Separator();
	if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem("Systems Manager"))
		{
			//Movement control
			static int movementToggle = 0;
			if (ImGui::Button("Movement")) { movementToggle++; }
			if (movementToggle & 1)
			{
				m_movement = false;
				ImGui::SameLine();
				ImGui::Text("Movement Turned Off");
			}
			else
			{
				m_movement = true;
				ImGui::SameLine();
				ImGui::Text("Movement Turned On");
			}
			//Lifespan control
			static int lifespanToggle = 0;
			if (ImGui::Button("Lifespan")) { lifespanToggle++; }
			if (lifespanToggle & 1)
			{
				m_lifespan = false;
				ImGui::SameLine();
				ImGui::Text("Lifespan Turned Off");
			}
			else
			{
				m_lifespan = true;
				ImGui::SameLine();
				ImGui::Text("Lifespan Turned On");
			}
			//Collision control
			static int collisionToggle = 0;
			if (ImGui::Button("Collision")) { collisionToggle++; }
			if (collisionToggle & 1)
			{
				m_collision = false;
				ImGui::SameLine();
				ImGui::Text("Collision Turned Off");
			}
			else
			{
				m_collision = true;
				ImGui::SameLine();
				ImGui::Text("Collision Turned On");
			}
			//Rendering control
			static int renderToggle = 0;
			if (ImGui::Button("Rendering")) { renderToggle++; }
			if (renderToggle & 1)
			{
				m_render = false;
				ImGui::SameLine();
				ImGui::Text("Rendering Turned Off");
			}
			else
			{
				m_render = true;
				ImGui::SameLine();
				ImGui::Text("Rendering Turned On");
			}
			//Manual Enemy Spawner control
			static int manualEnemySpawn = 0;
			if (ImGui::Button("Spawn Enemy")) { manualEnemySpawn++; }
			if (manualEnemySpawn & 1)
			{
				spawnEnemy();
				manualEnemySpawn = 0;
			}
			ImGui::SliderInt("Enemy Spawn Interval", &m_enemyConfig.SI, 5, 300);
			ImGui::EndTabItem();
		}

		//Entities Manager Controls
		if (ImGui::BeginTabItem("Entities Manager"))
		{
			//Entities by Tag
			if (ImGui::CollapsingHeader("Entities By Tag", ImGuiTreeNodeFlags_None))
			{
				if (ImGui::CollapsingHeader("Enemies", ImGuiTreeNodeFlags_None))
				{
					for (auto e : m_entities.getEntities("Enemy"))
					{
						ImGui::PushID(e->id());
						if (ImGui::Button("Destroy")) { e->destroy(); } 
						ImGui::SameLine(); ImGui::Text("ID: %d", e->id());
						ImGui::SameLine(); ImGui::Text("Tag: Enemy");
						ImGui::SameLine(); ImGui::Text("Pos: ( %f", e->cTransform->pos.x);
						ImGui::SameLine(); ImGui::Text(", %f", e->cTransform->pos.y);
						ImGui::SameLine(); ImGui::Text(")");
						ImGui::PopID();
					}
				}
				if (ImGui::CollapsingHeader("Small Enemies", ImGuiTreeNodeFlags_None))
				{
					for (auto e : m_entities.getEntities("Small"))
					{
						ImGui::PushID(e->id());
						if (ImGui::Button("Destroy")) { e->destroy(); }
						ImGui::SameLine(); ImGui::Text("ID: %d", e->id());
						ImGui::SameLine(); ImGui::Text("Tag: Small");
						ImGui::SameLine(); ImGui::Text("Pos: ( %f", e->cTransform->pos.x);
						ImGui::SameLine(); ImGui::Text(", %f", e->cTransform->pos.y);
						ImGui::SameLine(); ImGui::Text(")");
						ImGui::PopID();
					}
				}
				if (ImGui::CollapsingHeader("Bullets", ImGuiTreeNodeFlags_None))
				{
					for (auto e : m_entities.getEntities("Bullet"))
					{
						ImGui::PushID(e->id());
						if (ImGui::Button("Destroy")) { e->destroy(); }
						ImGui::SameLine(); ImGui::Text("ID: %d", e->id());
						ImGui::SameLine(); ImGui::Text("Tag: Bullet");
						ImGui::SameLine(); ImGui::Text("Pos: ( %f", e->cTransform->pos.x);
						ImGui::SameLine(); ImGui::Text(", %f", e->cTransform->pos.y);
						ImGui::SameLine(); ImGui::Text(")");
						ImGui::PopID();
					}
				}
				if (ImGui::CollapsingHeader("Player", ImGuiTreeNodeFlags_None))
				{
					for (auto e : m_entities.getEntities("Player"))
					{
						ImGui::PushID(e->id());
						if (ImGui::Button("Destroy")) { e->destroy(); }
						ImGui::SameLine(); ImGui::Text("ID: %d", e->id());
						ImGui::SameLine(); ImGui::Text("Tag: Player");
						ImGui::SameLine(); ImGui::Text("Pos: ( %f", e->cTransform->pos.x);
						ImGui::SameLine(); ImGui::Text(", %f", e->cTransform->pos.y);
						ImGui::SameLine(); ImGui::Text(")");
						ImGui::PopID();
					}
				}
			}
			if (ImGui::CollapsingHeader("All Entities", ImGuiTreeNodeFlags_None))
			{
				for (auto e : m_entities.getEntities())
				{
					ImGui::PushID(e->id());
					if (ImGui::Button("Destroy")) { e->destroy(); }
					ImGui::SameLine(); ImGui::Text("ID: %d", e->id());
					ImGui::SameLine(); 
					if (e->tag() == "Enemy") { ImGui::Text("Tag: Enemy"); }
					else if (e->tag() == "Small") { ImGui::Text("Tag: Small"); }
					else if (e->tag() == "Bullet") { ImGui::Text("Tag: Bullet"); }
					else if (e->tag() == "Player") { ImGui::Text("Tag: Player"); }
					ImGui::SameLine(); ImGui::Text("Pos: ( %f", e->cTransform->pos.x);
					ImGui::SameLine(); ImGui::Text(", %f", e->cTransform->pos.y);
					ImGui::SameLine(); ImGui::Text(")");
					ImGui::PopID();
				}
			}
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	
	ImGui::End();
}


//pause game function
void Game::setPaused()
{
	if (!m_paused)
	{
		std::cout << "Game Paused\n";
		m_paused = true;
	}
	else
	{
		std::cout << "Game Resumed\n";
		m_paused = false;
	}
}

//respawn player in the middle of the screen
void Game::spawnPlayer()
{
	// Create entity by calling EntityManager.addEntity(tag), returns std::shared_ptr<Entity> use auto to save typing
	auto entity = m_entities.addEntity("Player"); 
	
	entity->cTransform = std::make_shared<CTransform>(Vec2(m_windowConfig.W / 2, m_windowConfig.H / 2), Vec2(m_playerConfig.S, m_playerConfig.S), 0.0f); // Position, Speed, Angle
	entity->cShape     = std::make_shared<CShape>(m_playerConfig.CR, m_playerConfig.V, sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB), // Radius, Verticies, Fill, Outline, Outline Thickness
		                 sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB), m_playerConfig.OT);
	entity->cInput     = std::make_shared <CInput>(); // Can use inputs
	entity->cCollision = std::make_shared<CCollision>(m_playerConfig.CR);

	// Since we want this Entity to be our player, set our Games's player variable to be this Entity
	// this goes slightly against the EntityManager paradigm, but we use the player so much its worth it
	m_player = entity;
}

//spwan an enemy at a random position
void Game::spawnEnemy()
{
	auto entity = m_entities.addEntity("Enemy");

	srand((unsigned)time(NULL));
	int minX           = m_enemyConfig.CR * 2;
	int maxX           = m_windowConfig.W - (m_enemyConfig.CR * 2);
	int minY           = m_enemyConfig.CR * 2;
	int maxY           = m_windowConfig.H - (m_enemyConfig.CR * 2);
	float xPos         = minX + (rand() % (1 + maxX - minX));
	float yPos         = minY + (rand() % (1 + maxY - minY));
	float speed        = m_enemyConfig.SMIN + (rand() % (1 + (int)m_enemyConfig.SMAX - (int)m_enemyConfig.SMIN));
	int verticies      = m_enemyConfig.VMIN + (rand() % (1 + m_enemyConfig.VMAX - m_enemyConfig.VMIN));
	int redFill        = rand() % (256);
	int greenFill      = rand() % (256);
	int blueFill       = rand() % (256);
	int redOutline     = rand() % (256);
	int greenOutline   = rand() % (256);
	int blueOutline    = rand() % (256);

	Vec2 location(xPos, yPos);
	entity->cTransform = std::make_shared<CTransform>(location, Vec2(speed, speed), 0.0f);
	entity->cShape     = std::make_shared<CShape>(m_enemyConfig.CR, verticies, sf::Color(redFill, greenFill, blueFill), sf::Color(redOutline, greenOutline, blueOutline), m_enemyConfig.OT);
	entity->cCollision = std::make_shared<CCollision>(m_enemyConfig.CR);
	entity->cScore     = std::make_shared<CScore>(verticies);

	// Record when the most recent enemy was spwaned
	m_lastEnemySpawnTime = m_currentFrame;
}

//spwans the small enemies when a big one (input entity e) explodes
void Game::spawnSmallEnemies(std::shared_ptr<Entity> e)
{
	int verticies       = e->cShape->circle.getPointCount();
	int angleIncrements = 360 / verticies;

	for (int i = 0; i < verticies; i++)
	{
		auto smallEnemy = m_entities.addEntity("Small");
		Vec2 velocity(m_enemyConfig.VMIN * cos((angleIncrements * (i + 1)) * 3.14 / 180), m_enemyConfig.VMIN * sin((angleIncrements * (i + 1)) * 3.14 / 180));

		smallEnemy->cTransform = std::make_shared<CTransform>(e->cTransform->pos, velocity, angleIncrements * i);
		smallEnemy->cShape     = std::make_shared<CShape>(m_enemyConfig.CR / 3, verticies, e->cShape->circle.getFillColor(), e->cShape->circle.getOutlineColor(), m_enemyConfig.OT);
		smallEnemy->cCollision = std::make_shared<CCollision>(m_enemyConfig.CR / 3);
		smallEnemy->cLifespan  = std::make_shared<CLifespan>(m_enemyConfig.L);
		smallEnemy->cScore     = std::make_shared<CScore>(e->cScore->score * 2); // Double points of the large enemy destroyed
	}
}

// Spawns a bullet from a given entity to a target location
void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& target)
{
	auto bulletEntity = m_entities.addEntity("Bullet");

	Vec2  startPos = entity->cTransform->pos;
	Vec2  distVec  = target - startPos;
	float length   = distVec.length();
	Vec2  speedVec (m_bulletConfig.S * distVec.x / length , m_bulletConfig.S * distVec.y / length);

	bulletEntity->cTransform = std::make_shared<CTransform>(startPos, speedVec, 0.0f);
	bulletEntity->cShape     = std::make_shared<CShape>(m_bulletConfig.CR, m_bulletConfig.V, sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB), 
		                       sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB), m_bulletConfig.OT);
	bulletEntity->cCollision = std::make_shared<CCollision>(m_bulletConfig.CR);
	bulletEntity->cLifespan  = std::make_shared<CLifespan>(m_bulletConfig.L);
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
	if ((m_currentFrame - m_lastSpecialWeaponTime) > m_specialWeaponCoolDown)
	{
		m_lastSpecialWeaponTime = m_currentFrame;
		int numBullets = 10;
		int angleIncrements = 360 / numBullets;
		for (int i = 0; i < numBullets; i++)
		{
			auto bulletEntity = m_entities.addEntity("Bullet");

			Vec2  startPos = entity->cTransform->pos;
			Vec2  speedVec(m_bulletConfig.S * cos((angleIncrements * (i + 1)) * 3.14 / 180), m_bulletConfig.S * sin((angleIncrements * (i + 1)) * 3.14 / 180));

			bulletEntity->cTransform = std::make_shared<CTransform>(startPos, speedVec, 0.0f);
			bulletEntity->cShape     = std::make_shared<CShape>(m_bulletConfig.CR, m_bulletConfig.V, sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB),
									   sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB), m_bulletConfig.OT);
			bulletEntity->cCollision = std::make_shared<CCollision>(m_bulletConfig.CR);
			bulletEntity->cLifespan  = std::make_shared<CLifespan>(m_bulletConfig.L);
		}
	}
}

void Game::sMovement()
{
	// All entity movement in this function
	Vec2 playervelocity; //Default initializes to (0, 0)
	if (m_player->cInput->left)	 { playervelocity.x -= m_playerConfig.S; }
	if (m_player->cInput->right) { playervelocity.x += m_playerConfig.S; }
	if (m_player->cInput->up)    { playervelocity.y -= m_playerConfig.S; }
	if (m_player->cInput->down)	 { playervelocity.y += m_playerConfig.S; }

	for (auto e : m_entities.getEntities())
	{
		if ((e->cTransform->pos.x < e->cCollision->radius) || (e->cTransform->pos.x > (m_windowConfig.W - e->cCollision->radius))) // X wall collision
		{ 
			if (e->tag() == "Bullet")     { e->destroy(); } // Destroy bullet when it hits wall
			else if (e->tag() == "Enemy") { e->cTransform->velocity.x *= -1; } //Bounce
		} 

		if ((e->cTransform->pos.y < e->cCollision->radius) || (e->cTransform->pos.y > (m_windowConfig.H - e->cCollision->radius))) // Y wall collision
		{
			if (e->tag() == "Bullet")     { e->destroy(); } // Destroy bullet when it hits wall
			else if (e->tag() == "Enemy") { e->cTransform->velocity.y *= -1; } // Bounce
		}

		if (e->tag() != "Player") { e->cTransform->pos += e->cTransform->velocity; } // Passive entity velocity
		e->cTransform->angle += 1.0f; // Passive rotation
	}

	// Player bounds logic
	m_player->cTransform->velocity = playervelocity;
	float newX_Pos = m_player->cTransform->pos.x + m_player->cTransform->velocity.x;
	if ((newX_Pos > m_player->cCollision->radius) && (newX_Pos < (m_windowConfig.W - m_player->cCollision->radius)))
	{
		m_player->cTransform->pos.x += m_player->cTransform->velocity.x;
	}

	float newY_Pos = m_player->cTransform->pos.y + m_player->cTransform->velocity.y;
	if ((newY_Pos > m_player->cCollision->radius) && (newY_Pos < (m_windowConfig.H - m_player->cCollision->radius)))
	{
		m_player->cTransform->pos.y += m_player->cTransform->velocity.y;
	}
}

void Game::sLifeSpan()
{
	for (auto e : m_entities.getEntities())
	{
		if (e->cLifespan) 
		{ 
			e->cLifespan->remaining -= 1;
			if (e->cLifespan->remaining <= 0)
			{
				e->destroy();
			}
			else // Change color
			{
				auto fillColor    = e->cShape->circle.getFillColor();
				auto outlineColor = e->cShape->circle.getOutlineColor();
				int newAlpha      = 255 * e->cLifespan->remaining / e->cLifespan->total;

				sf::Color newFillColor(fillColor.r, fillColor.g, fillColor.b, newAlpha);
				sf::Color newOutlineColor(outlineColor.r, outlineColor.g, outlineColor.b, newAlpha);

				e->cShape->circle.setFillColor(newFillColor);
				e->cShape->circle.setOutlineColor(newOutlineColor);
			}

		}
	}
}

void Game::sCollision()
{
	// Collision btwn player and enemies
	for (auto p : m_entities.getEntities("Player"))
	{
		for (auto e : m_entities.getEntities("Enemy"))
		{
			Vec2 distVec     = p->cTransform->pos - e->cTransform->pos;
			float radiusSum  = p->cCollision->radius + e->cCollision->radius; // Sum the radius of the player and the enemy
			bool isCollision = (radiusSum * radiusSum) > ((distVec.x * distVec.x) + (distVec.y * distVec.y));
			if (isCollision)
			{
				p->destroy();
				e->destroy();
			}
		}
		for (auto e : m_entities.getEntities("Small"))
		{
			Vec2 distVec     = p->cTransform->pos - e->cTransform->pos;
			float radiusSum  = p->cCollision->radius + e->cCollision->radius; // Sum the radius of the player and the enemy
			bool isCollision = (radiusSum * radiusSum) > ((distVec.x * distVec.x) + (distVec.y * distVec.y));
			if (isCollision)
			{
				p->destroy();
				e->destroy();
			}
		}
	}

	// Collisions btwn bullets and large enemies
	for (auto e : m_entities.getEntities("Enemy"))
	{
		for (auto b : m_entities.getEntities("Bullet"))
		{
			//if collision 
			Vec2 distVec     = e->cTransform->pos - b->cTransform->pos;
			float radiusSum  = e->cCollision->radius + b->cCollision->radius; // Sum the radius of the player and the enemy
			bool isCollision = (radiusSum * radiusSum) > ((distVec.x * distVec.x) + (distVec.y * distVec.y));
			if (isCollision)
			{
				m_score += e->cScore->score; // Score increase
				spawnSmallEnemies(e); // Spawn small enemies
				e->destroy();
				b->destroy();
				std::cout << "Enemy Destroyed!\n";
			}
		}
	}

	// Collisions btwn bullets and small enemies
	for (auto e : m_entities.getEntities("Small"))
	{
		for (auto b : m_entities.getEntities("Bullet"))
		{
			// If collision 
			Vec2 distVec     = e->cTransform->pos - b->cTransform->pos;
			float radiusSum  = e->cCollision->radius + b->cCollision->radius; // Sum the radius of the player and the enemy
			bool isCollision = (radiusSum * radiusSum) > ((distVec.x * distVec.x) + (distVec.y * distVec.y));
			if (isCollision)
			{
				m_score += e->cScore->score; // Score increase
				e->destroy();
				b->destroy();
				std::cout << "Small Enemy Destroyed!\n";
			}
		}
	}
}

void Game::sEnemySpawner()
{
	// How long has it been since the enemy last spawned
	if ((m_currentFrame - m_lastEnemySpawnTime) > m_enemyConfig.SI)	{ spawnEnemy(); }
}

void Game::sScoreText()
{
	// Update the text string and reposition for placement
	std::string scoreString = std::to_string(m_score);
	m_text.setString("Score: " + scoreString);
	m_textRect = m_text.getLocalBounds();
	m_text.setOrigin(m_textRect.left + m_textRect.width / 2.0f, m_textRect.top + m_textRect.height / 2.0f); // Text box origin
	m_text.setPosition(100.0, 100.0);
}

void Game::sRender()
{	
	m_window.clear();

	if (m_render)
	{
		for (auto e : m_entities.getEntities()) // Draw all of the entities
		{
			e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y); // SFML Circle position update
			e->cShape->circle.setRotation(e->cTransform->angle); // SFML Circle rotation update
			m_window.draw(e->cShape->circle); // Draw entity to window
		}
	}

	//Draw the game score
	m_window.draw(m_text);
	ImGui::SFML::Render(m_window);
	m_window.display();
}

void Game::sUserInput()
{
	// Handle user input here
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(event);
		//this event triggers window close
		if (event.type == sf::Event::Closed)
		{
			m_running = false;
		}

		//If the game is not paused
		if (!m_paused)
		{
			// Mouse event
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					std::cout << "Left Mouse Button Clicked at (" << event.mouseButton.x << ", " << event.mouseButton.y << ")\n";
					spawnBullet(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));
				}
				if (event.mouseButton.button == sf::Mouse::Right)
				{
					std::cout << "Right Mouse Button Clicked at (" << event.mouseButton.x << ", " << event.mouseButton.y << ")\n";
					spawnSpecialWeapon(m_player);
				}
			}

			//evenmt triggers when a key is pressed
			if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::W:
					std::cout << "W Key Pressed\n";
					m_player->cInput->up = true;
					break;
				case sf::Keyboard::A:
					std::cout << "A Key Pressed\n";
					m_player->cInput->left = true;
					break;
				case sf::Keyboard::S:
					std::cout << "S Key Pressed\n";
					m_player->cInput->down = true;
					break;
				case sf::Keyboard::D:
					std::cout << "D Key Pressed\n";
					m_player->cInput->right = true;
					break;
				case sf::Keyboard::P:
					std::cout << "P Key Pressed\n";
					setPaused();
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
					m_player->cInput->up = false;
					break;
				case sf::Keyboard::A:
					std::cout << "A Key Pressed\n";
					m_player->cInput->left = false;
					break;
				case sf::Keyboard::S:
					std::cout << "S Key Pressed\n";
					m_player->cInput->down = false;
					break;
				case sf::Keyboard::D:
					std::cout << "D Key Pressed\n";
					m_player->cInput->right = false;
					break;
				}
			}
		}
		else
		{
			//event triggers when a key is pressed
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::P)
				{
					std::cout << "P Key Pressed\n";
					setPaused();
					break;
				}
			}
		}
		
	}
}
