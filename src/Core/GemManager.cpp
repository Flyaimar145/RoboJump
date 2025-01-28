#include <Core/GemManager.h>
#include <Utils/Constants.h>
#include <Core/AssetManager.h>
#include <External/json.hpp>
#include <SFML/Graphics/RenderWindow.hpp>


GemManager::~GemManager()
{
	for (Gem* gem : m_gemsVector)
	{
		delete gem;
	}
	m_gemsVector.clear();
}

bool GemManager::loadGems()
{
	json gemsInfo = loadJsonFromFile(GAMEINFOJSON_GEMS)["Gems"];
	json gameInfo = loadJsonFromFile(GAMEINFOJSON_CONFIG)["GameInfo"];
	for (auto& currentGemData : gemsInfo)
	{
		Gem::GemDescriptor gemDescriptor;
		gemDescriptor.position = { gameInfo["mapTileSize"] * currentGemData["positionX"].get<float>(), gameInfo["mapTileSize"] * currentGemData["positionY"].get<float>() };
		gemDescriptor.texture = AssetManager::getInstance()->loadTexture(currentGemData["texture"].get<std::string>().c_str());
		gemDescriptor.tileWidth = currentGemData["tileWidth"].get<float>();
		gemDescriptor.tileHeight = currentGemData["tileHeight"].get<float>();
		gemDescriptor.points = currentGemData["points"].get<int>();
		Gem* currentGem = new Gem();
		m_gemsVector.push_back(currentGem);
		const bool gemLoaded = currentGem->init(gemDescriptor);
		if (!gemLoaded)
		{
			return false;
		}
	}
	return true;
}

void GemManager::destroyGem(Gem* gem)
{
	auto gemIterator = std::find(m_gemsVector.begin(), m_gemsVector.end(), gem);
	if (gemIterator != m_gemsVector.end())
	{
		delete* gemIterator;
		m_gemsVector.erase(gemIterator);
	}
}
void GemManager::update(uint32_t deltaMilliseconds)
{
	for (Gem* gem : m_gemsVector)
	{
		gem->update(deltaMilliseconds);
	}
}

void GemManager::render(sf::RenderWindow& window)
{
	for (Gem* gem : m_gemsVector)
	{
		gem->render(window);
	}
}