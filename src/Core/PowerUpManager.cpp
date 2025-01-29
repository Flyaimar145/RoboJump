#include <Core/PowerUpManager.h>
#include <Utils/Constants.h>
#include <Core/AssetManager.h>
#include <External/json.hpp>
#include <SFML/Graphics/RenderWindow.hpp>


PowerUpManager::~PowerUpManager()
{
	for (PowerUp* powerUp : m_powerUpsVector)
	{
		delete powerUp;
	}
	m_powerUpsVector.clear();
}

bool PowerUpManager::loadPowerUps()
{
	json powerUpsInfo = loadJsonFromFile(GAMEINFOJSON_POWERUPS)["PowerUps"];
	json gameInfo = loadJsonFromFile(GAMEINFOJSON_CONFIG)["GameInfo"];
	for (auto& [powerUpType, powerUpList] : powerUpsInfo.items())
	{
		for (auto& currentPowerUpData : powerUpList)
		{
			PowerUp::PowerUpDescriptor powerUpDescriptor;
			powerUpDescriptor.position = { gameInfo["mapTileSize"] * currentPowerUpData["positionX"].get<float>(), gameInfo["mapTileSize"] * currentPowerUpData["positionY"].get<float>() };
			powerUpDescriptor.texture = AssetManager::getInstance()->loadTexture(currentPowerUpData["texture"].get<std::string>().c_str());
			powerUpDescriptor.tileWidth = currentPowerUpData["tileWidth"].get<float>();
			powerUpDescriptor.tileHeight = currentPowerUpData["tileHeight"].get<float>();
			PowerUp* currentPowerUp = new PowerUp();
			m_powerUpsVector.push_back(currentPowerUp);
			if (powerUpType == "ExtraLife")
			{
				currentPowerUp->setPowerUpType(PowerUp::PowerUpType::ExtraLife);
			}
			else if (powerUpType == "SpeedBoost")
			{
				currentPowerUp->setPowerUpType(PowerUp::PowerUpType::SpeedBoost);
			}
			const bool powerUpLoaded = currentPowerUp->init(powerUpDescriptor);
			if (!powerUpLoaded)
			{
				return false;
			}
		}
	}
	return true;
}

void PowerUpManager::destroyPowerUp(PowerUp* powerUp)
{
	auto powerUpIterator = std::find(m_powerUpsVector.begin(), m_powerUpsVector.end(), powerUp);
	if (powerUpIterator != m_powerUpsVector.end())
	{
		delete* powerUpIterator;
		m_powerUpsVector.erase(powerUpIterator);
	}
}
void PowerUpManager::update(uint32_t deltaMilliseconds)
{
	for (PowerUp* powerUp : m_powerUpsVector)
	{
		powerUp->update(deltaMilliseconds);
	}
}

void PowerUpManager::render(sf::RenderWindow& window)
{
	for (PowerUp* powerUp : m_powerUpsVector)
	{
		powerUp->render(window);
	}
}