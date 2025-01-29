#include <Core/AssetManager.h>
#include <Core/EnemyManager.h>
#include <External/json.hpp>
#include <Utils/Constants.h>

using json = nlohmann::json;

EnemyManager::~EnemyManager()
{
	for (Enemy* enemy : m_enemiesVector)
	{
		delete enemy;
	}
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


