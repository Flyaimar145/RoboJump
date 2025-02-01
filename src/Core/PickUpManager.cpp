#include <Core/AssetManager.h>
#include <Core/PickUpManager.h>
#include <Gameplay/Gem.h>
#include <Gameplay/PickUp.h>
#include <Gameplay/PowerUp.h>
#include <External/json.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Utils/Constants.h>

PickUpManager::~PickUpManager()
{
	for (PickUp* pickUp : m_pickUpsVector)
	{
		delete pickUp;
	}
	m_gemsVector.clear();
	m_powerUpsVector.clear();
	m_pickUpsVector.clear();
}

bool PickUpManager::loadPickUps()
{
	json pickUpsInfo = loadJsonFromFile(GAMEINFOJSON_PICKUPS)["PickUps"];
	json gameInfo = loadJsonFromFile(GAMEINFOJSON_CONFIG)["GameInfo"];
	for (auto& [pickUpType, pickUpList] : pickUpsInfo.items())
	{
		for (auto& currentPickUpData : pickUpList)
		{
			sf::Vector2f jsonPosition = { gameInfo["mapTileSize"] * currentPickUpData["positionX"].get<float>(), gameInfo["mapTileSize"] * currentPickUpData["positionY"].get<float>() };
			sf::Texture* jsonTexture = AssetManager::getInstance()->loadTexture(currentPickUpData["texture"].get<std::string>().c_str());
			float jsonTileWidth = currentPickUpData["tileWidth"].get<float>();
			float jsonTileHeight = currentPickUpData["tileHeight"].get<float>();
			float jsonTotalFrames = currentPickUpData["totalFrames"].get<float>();
			float jsonFrameDuration = currentPickUpData["frameDuration"].get<float>();

			if (pickUpType == "Gems")
			{
				Gem* currentPickUp = new Gem();
				Gem::GemDescriptor gemDescriptor;
				gemDescriptor.position = jsonPosition;
				gemDescriptor.texture = jsonTexture;
				gemDescriptor.tileWidth = jsonTileWidth;
				gemDescriptor.tileHeight = jsonTileHeight;
				gemDescriptor.totalFrames = jsonTotalFrames;
				gemDescriptor.frameDuration = jsonFrameDuration;
				gemDescriptor.points = currentPickUpData["points"].get<int>();
				currentPickUp->setPickUpType(PickUp::PickUpType::Gem);
				const bool pickUpLoaded = currentPickUp->init(gemDescriptor);
				if (!pickUpLoaded)
				{
					return false;
				}
				else 
				{
					m_gemsVector.push_back(currentPickUp);
					m_pickUpsVector.push_back(currentPickUp);
				}
			}
			else if (pickUpType == "PowerUps")
			{
				PowerUp* currentPickUp = new PowerUp();
				PickUp::PickUpDescriptor pickUpDescriptor;
				pickUpDescriptor.position = jsonPosition;
				pickUpDescriptor.texture = jsonTexture;
				pickUpDescriptor.tileWidth = jsonTileWidth;
				pickUpDescriptor.tileHeight = jsonTileHeight;
				pickUpDescriptor.totalFrames = jsonTotalFrames;
				pickUpDescriptor.frameDuration = jsonFrameDuration;
				currentPickUp->setPickUpType(PickUp::PickUpType::PowerUp);
				std::string powerUpType = currentPickUpData["powerUpType"];
				if (powerUpType == "ExtraLife")
				{
					currentPickUp->setPowerUpType(PowerUp::PowerUpType::ExtraLife);
				}
				else if (powerUpType == "SpeedBoost")
				{
					currentPickUp->setPowerUpType(PowerUp::PowerUpType::SpeedBoost);
				}
				const bool pickUpLoaded = currentPickUp->init(pickUpDescriptor);
				if (!pickUpLoaded)
				{
					return false;
				}
				else
				{
					m_powerUpsVector.push_back(currentPickUp);
					m_pickUpsVector.push_back(currentPickUp);
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

void PickUpManager::destroyPickUp(PickUp* pickUp)
{
	auto pickUpIterator = std::find(m_pickUpsVector.begin(), m_pickUpsVector.end(), pickUp);
	if (pickUpIterator != m_pickUpsVector.end())
	{
		delete* pickUpIterator;
		m_pickUpsVector.erase(pickUpIterator);
	}
}
void PickUpManager::update(uint32_t deltaMilliseconds)
{
	for (PickUp* pickUp : m_pickUpsVector)
	{
		pickUp->update(deltaMilliseconds);
	}
}

void PickUpManager::render(sf::RenderWindow& window)
{
	for (PickUp* pickUp : m_pickUpsVector)
	{
		pickUp->render(window);
	}
}