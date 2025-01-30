#include <Core/AssetManager.h>
#include <External/json.hpp>
#include <Gameplay/Player.h>
#include <SFML/Window/Keyboard.hpp>
#include <Utils/Constants.h>

using json = nlohmann::json;

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
	playerDescriptor.offsetForAdjustedBoundsLeft = playerInfo["offsetForAdjustedBoundsLeft"].get<float>();
	playerDescriptor.offsetForAdjustedBoundsTop = playerInfo["offsetForAdjustedBoundsTop"].get<float>();
	playerDescriptor.offsetForAdjustedBoundsWidth = playerInfo["offsetForAdjustedBoundsWidth"].get<float>();
	playerDescriptor.offsetForAdjustedBoundsHeight = playerInfo["offsetForAdjustedBoundsHeight"].get<float>();
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
	if (m_currentGravity > 0.0f)
	{
		m_speed.y += m_currentGravity * (deltaMilliseconds / 1000.f);
	}
	if (!m_isJumping && m_currentGravity == 0.0f)
	{
		m_speed.y = 0.0f;
	}

	m_position.x += (m_direction.x * m_speed.x * deltaMilliseconds);
	m_position.y += m_speed.y * (deltaMilliseconds / 1000.f);
	
	
	if (m_liveAmountChanged)
	{ 
		m_liveAmountChanged = false;
		if (m_lifeCount <= 1)
		{
			m_sprite.setTexture(*AssetManager::getInstance()->getTexture("../data/Levels/images/png/craftpix-net-396765-free-simple-platformer-game-kit-pixel-art/1 Main Characters/MainCharacter2_1Live.png"));
		}
		else 
		{
			m_sprite.setTexture(*AssetManager::getInstance()->getTexture("../data/Levels/images/png/craftpix-net-396765-free-simple-platformer-game-kit-pixel-art/1 Main Characters/MainCharacter2.png"));
		}
	}

	if (m_isSpeedBoostActive)
	{
		m_speedBoostTimer += deltaMilliseconds;
		if (m_speedBoostTimer >= m_speedBoostDuration)
		{
			m_isSpeedBoostActive = false;
			m_speed.x /= 2.f; // Revert the speed boost
		}
	}

	m_animationTime += deltaMilliseconds;
	if (m_hasTakenDamage)
	{
		if (!m_damageAnimationStarted)
		{
			m_currentFrame = 0;
			m_damageAnimationStarted = true;
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
	printf("Score: %d\n", m_score);
	Entity::update(deltaMilliseconds);
}

void Player::addLife()
{
	if (m_lifeCount < 2)
	{
		m_lifeCount++;
		m_liveAmountChanged = true;
	}
	else 
	{
		addScore(10);
	}
}

void Player::applySpeedBoost()
{
	if (!m_isSpeedBoostActive)
	{
		m_speed.x *= 2.f;
		m_isSpeedBoostActive = true;
		m_speedBoostDuration = 10000.f;
		m_speedBoostTimer = 0.0f;
	}
}