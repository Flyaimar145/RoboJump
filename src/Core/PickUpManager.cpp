#include <Core/PickUpManager.h>
#include <Utils/Constants.h>
#include <Core/AssetManager.h>
#include <External/json.hpp>
#include <SFML/Graphics/RenderWindow.hpp>


PickUpManager::~PickUpManager()
{
	for (PickUp* pickUp : m_pickUpsVector)
	{
		delete pickUp;
	}
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
			PickUp::PickUpDescriptor pickUpDescriptor;
			pickUpDescriptor.position = { gameInfo["mapTileSize"] * currentPickUpData["positionX"].get<float>(), gameInfo["mapTileSize"] * currentPickUpData["positionY"].get<float>() };
			pickUpDescriptor.texture = AssetManager::getInstance()->loadTexture(currentPickUpData["texture"].get<std::string>().c_str());
			pickUpDescriptor.tileWidth = currentPickUpData["tileWidth"].get<float>();
			pickUpDescriptor.tileHeight = currentPickUpData["tileHeight"].get<float>();
			PickUp* currentPickUp = new PickUp();
			m_pickUpsVector.push_back(currentPickUp);
			const bool pickUpLoaded = currentPickUp->init(pickUpDescriptor);
			if (!pickUpLoaded)
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