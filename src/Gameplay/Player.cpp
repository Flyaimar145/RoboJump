#include <Core/AssetManager.h>
#include <Gameplay/Player.h>
#include <iostream>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>

Player::PlayerDescriptor Player::load()
{
	json playerInfo = loadJsonFromFile(GAMEINFOJSON_PLAYER)["Player"];
	json gameInfo = loadJsonFromFile(GAMEINFOJSON_CONFIG)["GameInfo"];
	sf::Texture* playerFirstTexture = AssetManager::getInstance()->loadTexture(playerInfo["firstTexture"].get<std::string>().c_str());
	sf::Texture* oneLifeLeftTexture = AssetManager::getInstance()->loadTexture(playerInfo["oneLifeLeftTexture"].get<std::string>().c_str());
	Player::PlayerDescriptor playerDescriptor;
	playerDescriptor.firstTexture = playerFirstTexture;
	playerDescriptor.position = { gameInfo["mapTileSize"] * playerInfo["positionX"].get<float>(), gameInfo["mapTileSize"] * playerInfo["positionY"].get<float>() };
	playerDescriptor.speed = { playerInfo["speedX"].get<float>() * millisecondsToSeconds, playerInfo["speedY"].get<float>() * millisecondsToSeconds };
	playerDescriptor.tileWidth = playerInfo["tileWidth"].get<float>();
	playerDescriptor.tileHeight = playerInfo["tileHeight"].get<float>();
	playerDescriptor.jumpSpeed = playerInfo["jumpSpeed"].get<float>();
	playerDescriptor.totalFrames = playerInfo["totalFrames"].get<int>();
	playerDescriptor.deathAnimationTotalFrames = playerInfo["deathAnimationTotalFrames"].get<int>();
	playerDescriptor.lifeCount = playerInfo["lifeCount"].get<int>();
	return playerDescriptor;
}

bool Player::init(const PlayerDescriptor& playerDescriptor)
{
	m_jumpSpeed = playerDescriptor.jumpSpeed;
	Entity::init(playerDescriptor);
	return true;
}

void Player::update(float deltaMilliseconds)
{
	bool isMovingLeftInput = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
	bool isMovingRightInput = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
	bool isJumpingInput = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
	bool isDyingInput = sf::Keyboard::isKeyPressed(sf::Keyboard::K);

	if (isDyingInput)
	{
		m_isDead = true;
	}
	bool isMoving = isMovingLeftInput || isMovingRightInput;
	if (isMoving)
	{
		if (isMovingLeftInput)
		{
			m_direction.x = -1.0f;
			m_sprite.setScale(-1.f, 1.f);
			m_sprite.setOrigin(m_tileWidth, 0.f);
		}
		else if (isMovingRightInput)
		{
			m_direction.x = 1.0f;
			m_sprite.setScale(1.f, 1.f);
			m_sprite.setOrigin(0.f, 0.f);
		}
	}
	else
	{
		m_direction.x = .0f;
	}

	if ((isJumpingInput && !m_isJumping) || m_makeJump)
	{
		m_makeJump = false;
		m_isJumping = true;
		m_speed.y = -m_jumpSpeed;
	}
	// Apply gravity
	if (m_currentGravity > 0.0f)
	{
		m_speed.y += m_currentGravity * (deltaMilliseconds / 1000.f);
	}
	// Check if player is on the ground
	if (!m_isJumping && m_currentGravity == 0.0f)
	{
		m_speed.y = 0.0f;
	}

	// Update final position
	m_position.x += (m_direction.x * m_speed.x * deltaMilliseconds);
	m_position.y += m_speed.y * (deltaMilliseconds / 1000.f);
	

	//Player's info
	//printf("X: %f, Y: %f \n", m_position.x, m_position.y);
	//printf("Sprite X: %f, Sprite Y: %f \n", m_sprite.getPosition().x, m_sprite.getPosition().y);
	//("Speed X: %f, Speed Y: %f \n", m_speed.x, m_speed.y);
	//printf("Sprite top: %f \n", m_sprite.getGlobalBounds().top);
	//printf("Sprite left: %f \n", m_sprite.getGlobalBounds().left);
	//printf("Live Count: %d \n", m_liveCount);
	//printf("Is dead: %d \n", m_isDead);
	//printf("Is jumping: %d \n", m_isJumping);
	
	if (m_liveAmountChanged)
	{ 
		m_liveAmountChanged = false;
		//Update texture for when 1 live left
		if (m_lifeCount <= 1)
		{
			m_sprite.setTexture(*AssetManager::getInstance()->getTexture("../data/Levels/images/png/craftpix-net-396765-free-simple-platformer-game-kit-pixel-art/1 Main Characters/MainCharacter2_1Live.png"));
		}
		else 
		{
			m_sprite.setTexture(*AssetManager::getInstance()->getTexture("../data/Levels/images/png/craftpix-net-396765-free-simple-platformer-game-kit-pixel-art/1 Main Characters/MainCharacter2"));
		}
	}

	// Update animation
	m_animationTime += deltaMilliseconds;
	if (m_hasTakenDamage)
	{
		// First moment of taking damage: Reset the current frame to 0 to start the animation from the beginning, and reduce the live count
		if (!m_damageAnimationStarted)
		{
			m_currentFrame = 0;
			m_damageAnimationStarted = true;
			printf("Damage taken\n");
			m_lifeCount--;
			m_liveAmountChanged = true;
		}

		if (m_animationTime >= m_frameDuration)
		{
			updateAnimation(m_totalFrames, 2.f);
			if (m_currentFrame == m_totalFrames - 1)
			{
				m_hasTakenDamage = false;
				m_damageAnimationStarted = false;
				if (m_lifeCount <= 0)
				{
					m_isDead = true;
				}
			}
		}
	}
	else if (m_isDead)
	{
		// Reset the current frame to 0 when damage is taken, so the animation starts from the beginning
		if (!m_deathAnimationStarted)
		{
			m_currentFrame = 0;
			m_deathAnimationStarted = true;
		}

		if (m_animationTime >= m_frameDuration)
		{
			updateAnimation(m_deathAnimationTotalFrames, 4.f);
			if (m_currentFrame == m_deathAnimationTotalFrames - 1)
			{
				m_position = { 1000.f, 1000.f };
			}
		}
	}
	else
	if (m_speed.y != 0)
	{
		if (m_speed.y > 0.0f)
		{
			m_currentSpriteStartingX = m_tileWidth * 1.f;
			m_currentSpriteStartingY = m_tileHeight * 3.f;
		}
		else
		{
			m_currentSpriteStartingX = m_tileWidth * 0.f;
			m_currentSpriteStartingY = m_tileHeight * 3.f;
		}
	}
	else if (m_direction.x != 0.0f)
	{
		if (m_animationTime >= m_frameDuration)
		{
			updateAnimation(m_totalFrames, 1.f);
		}
	}
	else
	{
		if (m_animationTime >= m_frameDuration)
		{
			updateAnimation(m_totalFrames, 0.f);
		}
	}
	Entity::update(deltaMilliseconds);
}

void Player::render(sf::RenderWindow& window)
{
	//Show adjusted bounds
	const sf::FloatRect adjustedBounds = this->getAdjustedBounds();
	sf::RectangleShape boundsRect(sf::Vector2f(adjustedBounds.width, adjustedBounds.height));
	boundsRect.setPosition(adjustedBounds.left, adjustedBounds.top);
	boundsRect.setOutlineColor(sf::Color::Blue);
	boundsRect.setOutlineThickness(.5f);
	boundsRect.setFillColor(sf::Color::Transparent);
	window.draw(boundsRect);

	Entity::render(window);
}