#include <Core/AssetManager.h>
#include <Core/EnemyManager.h>
#include <Gameplay/Enemies/Cactus.h>
#include <Gameplay/Enemies/Enemy.h>
#include <Gameplay/Enemies/Frog.h>
#include <Gameplay/Enemies/Stomp.h>
#include <External/json.hpp>
#include <Utils/Constants.h>

using json = nlohmann::json;

EnemyManager::~EnemyManager()
{
	for (Enemy* enemy : m_enemiesVector)
	{
		delete enemy;
	}
	m_cactusTypeEnemiesVector.clear();
	m_frogTypeEnemiesVector.clear();
	m_stompTypeEnemiesVector.clear();
	m_enemiesVector.clear();
}

bool EnemyManager::loadEnemies() 
{
	json enemiesInfo = loadJsonFromFile(GAMEINFOJSON_ENEMIES)["Enemies"];
	json gameInfo = loadJsonFromFile(GAMEINFOJSON_CONFIG)["GameInfo"];

	for (auto& [enemyType, enemyList] : enemiesInfo.items())
	{
		for (auto& currentEnemyData : enemyList)
		{
			Enemy::EnemyDescriptor enemyDescriptor;
			enemyDescriptor.firstTexture = AssetManager::getInstance()->loadTexture(currentEnemyData["texture"].get<std::string>().c_str());
			enemyDescriptor.position = { gameInfo["mapTileSize"] * currentEnemyData["positionX"].get<float>(), gameInfo["mapTileSize"] * currentEnemyData["positionY"].get<float>() };
			enemyDescriptor.speed = { currentEnemyData["speedX"].get<float>() * millisecondsToSeconds, currentEnemyData["speedY"].get<float>() * millisecondsToSeconds };
			enemyDescriptor.tileWidth = currentEnemyData["tileWidth"].get<float>();
			enemyDescriptor.tileHeight = currentEnemyData["tileHeight"].get<float>();
			enemyDescriptor.totalFrames = currentEnemyData["totalFrames"].get<int>();
			enemyDescriptor.deathAnimationTotalFrames = currentEnemyData["deathAnimationTotalFrames"].get<int>();
			enemyDescriptor.frameDuration = currentEnemyData["frameDuration"].get<float>();
			enemyDescriptor.initialDirection = { currentEnemyData["initialDirectionX"].get<float>(), currentEnemyData["initialDirectionY"].get<float>() };
			enemyDescriptor.lifeCount = currentEnemyData["lifeCount"].get<int>();
			enemyDescriptor.offsetForAdjustedBoundsLeft = currentEnemyData["offsetForAdjustedBoundsLeft"].get<float>();
			enemyDescriptor.offsetForAdjustedBoundsTop = currentEnemyData["offsetForAdjustedBoundsTop"].get<float>();
			enemyDescriptor.offsetForAdjustedBoundsWidth = currentEnemyData["offsetForAdjustedBoundsWidth"].get<float>();
			enemyDescriptor.offsetForAdjustedBoundsHeight = currentEnemyData["offsetForAdjustedBoundsHeight"].get<float>();

			if (enemyType == "Cactus")
			{
				Cactus* currentEnemy = new Cactus();
				m_cactusTypeEnemiesVector.push_back(currentEnemy);
				m_enemiesVector.push_back(currentEnemy);
				const bool enemyLoaded = currentEnemy->init(enemyDescriptor);
				if (!enemyLoaded)
				{
					return false;
				}
			}
			else if (enemyType == "Frog")
			{
				Frog* currentEnemy = new Frog();
				m_frogTypeEnemiesVector.push_back(currentEnemy);
				m_enemiesVector.push_back(currentEnemy);
				const bool enemyLoaded = currentEnemy->init(enemyDescriptor);
				if (!enemyLoaded)
				{
					return false;
				}
			}
			else if (enemyType == "Stomp")
			{
				Stomp* currentEnemy = new Stomp();
				m_stompTypeEnemiesVector.push_back(currentEnemy);
				m_enemiesVector.push_back(currentEnemy);
				const bool enemyLoaded = currentEnemy->init(enemyDescriptor);
				if (!enemyLoaded)
				{
					return false;
				}
			}
			else
			{
				return false;
			}
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

void EnemyManager::destroyCactus(Cactus* cactus)
{
	auto cactusIterator = std::find(m_cactusTypeEnemiesVector.begin(), m_cactusTypeEnemiesVector.end(), cactus);
	if (cactusIterator != m_cactusTypeEnemiesVector.end())
	{
		m_cactusTypeEnemiesVector.erase(cactusIterator);
	}
	auto enemyIterator = std::find(m_enemiesVector.begin(), m_enemiesVector.end(), cactus);
	if (enemyIterator != m_enemiesVector.end())
	{
		m_enemiesVector.erase(enemyIterator);
	}
	delete cactus;
}
