#pragma once
#include "Common.h"
#include "Entity.h"
#include "EntityManager.h"

// SR = Shape Radius, CR = Collision Radius, [FR, FG, FB] = Fill Color
// [OR, OG, OB] = Outline color, OT = Outline Thinkness, V = Verticies
// S = Speed, SI = Spawn Interval, L = Lifespan (small lifespan for enemy)
struct PlayerConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V; float S; };
struct EnemyConfig  { int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI; float SMIN, SMAX; };
struct BulletConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L; float S; };
// W = Width, H = Height, FR = Frame Rate, FS = Full Screen
struct WindowConfig { int W,  H,  FR, FS; };
// FN = file name, FS = font size, FR = font red, FG = font green, FB = font blue
struct FontConfig { std::string FN; int FS, FR, FG, FB; };

class Game
{
	sf::Clock        m_deltaClock;
	sf::RenderWindow m_window; // Window we will draw to
	sf::Font         m_font; // Font we will use to draw
	sf::Text         m_text; // The score will be drawn to the screen
	sf::FloatRect    m_textRect;
	EntityManager    m_entities; // Vector of entities to maintain
	PlayerConfig     m_playerConfig;
	EnemyConfig      m_enemyConfig;
	BulletConfig     m_bulletConfig;
	WindowConfig     m_windowConfig;
	FontConfig       m_fontConfig;

	int m_score                 = 0; // Current score
	int m_currentFrame          = 0;
	int m_lastEnemySpawnTime    = 0;
	int m_lastSpecialWeaponTime = 0;
	int m_specialWeaponCoolDown = 50;

	bool m_paused     = false; //whether we update game logic
	bool m_running    = true; // whether the game is running
	bool m_collision  = true;
	bool m_lifespan   = true;
	bool m_movement   = true;
	bool m_render     = true;
	//bool m_displayGUI = true;

	std::shared_ptr<Entity> m_player;

	void init(const std::string& config); //initialize the game state with a config file path
	void setPaused();
	void imguiCall();

	void sMovement(); //System: Entity position / movement update
	void sUserInput(); //System: User input
	void sLifeSpan(); //System: Life span
	void sScoreText(); //System: score text
	void sRender(); //System: render / drawing
	void sEnemySpawner(); //System: spawns enemies
	void sCollision(); //System: Collisions

	void spawnPlayer();
	void spawnEnemy();
	void spawnSmallEnemies(std::shared_ptr<Entity> entity);
	void spawnBullet(std::shared_ptr<Entity> entity, const Vec2& mousePos);
	void spawnSpecialWeapon(std::shared_ptr<Entity> entity);

public:
	Game(const std::string& config); // Constructor, takes in game config
	void run();
};