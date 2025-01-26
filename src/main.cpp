
#include <cstdio>
#include <iostream>
#include <Core/Game.h>
#include <SFML/System/Clock.hpp>
#include <Utils/Constants.h>

#include <External/json.hpp>
#include <fstream>

using json = nlohmann::json;


int main()
{
	//std::ifstream gameInfoFile("../data/GameInfo.json");
	//if (!gameInfoFile.is_open())
	//{
	//	printf("GameInfo.json file could not be opened\n");
	//	return 0;
	//}
        
	//json gameInfoJSON;
	//gameInfoFile >> gameInfoJSON;
	//Print the full json file to see if it was loaded correctly
    //std::cout << gameInfo.dump(4) << std::endl;

	//Get the game info from file instead of hardcoding it
	//auto gameInfo = gameInfoJSON["GameInfo"];
    //Game::GameCreateInfo gameCI;
    //gameCI.gameTitle = gameInfo["gameTitle"].get<std::string>();
    //gameCI.screenWidth = gameInfo["screenWidth"];
    //gameCI.screenHeight = gameInfo["screenHeight"];
    //gameCI.frameRateLimit = gameInfo["frameRateLimit"];

    Game game;
	const bool gameInitialized = game.init();

    if (gameInitialized == false)
    {
        printf("Game could not be initialized\n");
        return 0;
    }

    sf::Clock clock;

    while (game.isRunning())
    {
        const sf::Time elapsed = clock.restart();

        game.update(elapsed.asMilliseconds());
        game.render();
    }
}
