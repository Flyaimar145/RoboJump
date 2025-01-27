#include <cassert>
#include <Core/AssetManager.h>
#include <Core/Game.h>
#include <Core/World.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <Utils/Constants.h>
#include <External/json.hpp>
#include <Core/EnemyManager.h>

EnemyManager::~EnemyManager()
{
	for (Enemy* enemy : m_enemiesVector)
	{
		delete enemy;
	}
}

bool EnemyManager::loadEnemies() 
{
	json enemiesInfo = loadJsonFromFile(GAMEINFOJSON_ENEMIES)["Enemies"];
	json gameInfo = loadJsonFromFile(GAMEINFOJSON_CONFIG)["GameInfo"];

	for (auto& [enemyType, enemyList] : enemiesInfo.items())
	{
		for (auto& currentEnemyData : enemyList)
		{
			Enemy* currentEnemy = nullptr;
			if (enemyType == "Cactus")
			{
				currentEnemy = new Cactus();
				m_cactusTypeEnemiesVector.push_back(currentEnemy);
			}
			else if (enemyType == "Frog")
			{
				currentEnemy = new Frog();
				m_frogTypeEnemiesVector.push_back(currentEnemy);
			}
			else if (enemyType == "Stomp")
			{
				//currentEnemy = new Stomp();
				printf("Stomp enemy not implemented yet\n");
			}
			else
			{
				assert(false && "Unknown enemy type");
			}

			Enemy::EnemyDescriptor enemyDescriptor;
			enemyDescriptor.firstTexture = AssetManager::getInstance()->loadTexture(currentEnemyData["texture"].get<std::string>().c_str());
			enemyDescriptor.position = { gameInfo["mapTileSize"] * currentEnemyData["positionX"].get<float>(), gameInfo["mapTileSize"] * currentEnemyData["positionY"].get<float>() };
			enemyDescriptor.speed = { currentEnemyData["speedX"].get<float>() * millisecondsToSeconds, currentEnemyData["speedY"].get<float>() * millisecondsToSeconds };
			enemyDescriptor.tileWidth = currentEnemyData["tileWidth"].get<float>();
			enemyDescriptor.tileHeight = currentEnemyData["tileHeight"].get<float>();
			enemyDescriptor.totalFrames = currentEnemyData["totalFrames"].get<int>();
			enemyDescriptor.deathAnimationTotalFrames = currentEnemyData["deathAnimationTotalFrames"].get<int>();
			enemyDescriptor.initialDirection = { currentEnemyData["initialDirectionX"].get<float>(), currentEnemyData["initialDirectionY"].get<float>() };
			enemyDescriptor.lifeCount = currentEnemyData["lifeCount"].get<int>();

			const bool enemyLoaded = currentEnemy->init(enemyDescriptor);
			if (!enemyLoaded)
			{
				return false;
			}
			m_enemiesVector.push_back(currentEnemy);
		}
	}
	return true;
}


void EnemyManager::update(uint32_t deltaMilliseconds)
{
	for (Enemy* enemy : m_enemiesVector)
	{
		enemy->update(deltaMilliseconds);
	}
}

void EnemyManager::render(sf::RenderWindow& window)
{
	for (Enemy* enemy : m_enemiesVector)
	{
		enemy->render(window);
	}
}


