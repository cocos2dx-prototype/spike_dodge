//
//  GameScene.cpp
//  SpikeDodge-desktop
//
//  Created by hoangbui-fe on 10/8/26.
//

#include "GameScene.hpp"
#include "Constants.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>

USING_NS_CC;

GameScene::GameScene() {
    
}

GameScene::~GameScene() {
    
}

Scene* GameScene::createScene()
{
    return GameScene::create();
}

bool GameScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    _initValues();
    _initObjects();

    return true;
}

void GameScene::_initValues() {
    
}

void GameScene::_initObjects() {
    setupBackground();
    setupPlayer();
    setupUI();
    setupKeyboard();
}

void GameScene::onEnter() {
    Scene::onEnter();
    startGame();
    scheduleUpdate();
}

void GameScene::onExit() {
    unscheduleUpdate();
    Scene::onExit();
}

void GameScene::setupBackground()
{
    auto background = DrawNode::create();

    const Vec2 vertices[4] =
    {
        Vec2(0.0f, 0.0f),
        Vec2(SpikeDodge::DESIGN_WIDTH, 0.0f),
        Vec2(SpikeDodge::DESIGN_WIDTH, SpikeDodge::DESIGN_HEIGHT),
        Vec2(0.0f, SpikeDodge::DESIGN_HEIGHT)
    };

    background->drawSolidPoly(
        vertices,
        4,
        Color4F(
            SpikeDodge::BACKGROUND_RED,
            SpikeDodge::BACKGROUND_GREEN,
            SpikeDodge::BACKGROUND_BLUE,
            1.0f
        )
    );

    addChild(background, kChildTagBackground);
}

void GameScene::setupPlayer()
{
    _playerDrawNode = DrawNode::create();

    _playerDrawNode->drawDot(
        Vec2::ZERO,
        SpikeDodge::PLAYER_RADIUS,
        Color4F::WHITE
    );

    addChild(_playerDrawNode, kChildTagPlayer);

    _playerPosition = Vec2(
        SpikeDodge::DESIGN_WIDTH * 0.5f,
        SpikeDodge::DESIGN_HEIGHT * 0.5f
    );

    _playerDrawNode->setPosition(_playerPosition);
}

void GameScene::setupUI()
{
    _timerLabel = Label::createWithTTF(
        "Time: 0",
        "fonts/arial.ttf",
        SpikeDodge::TIMER_FONT_SIZE
    );

    _timerLabel->setTextColor(Color4B::WHITE);
    _timerLabel->setAnchorPoint(Vec2(1.0f, 1.0f));

    _timerLabel->setPosition(
        SpikeDodge::DESIGN_WIDTH -
            SpikeDodge::TIMER_MARGIN_RIGHT,
        SpikeDodge::DESIGN_HEIGHT -
            SpikeDodge::TIMER_MARGIN_TOP
    );

    addChild(_timerLabel, kChildTagHUD);

    // -------------------------------------------------------------------------
    // Game-over UI
    // -------------------------------------------------------------------------

    _gameOverLabel = Label::createWithTTF(
        "GAME OVER",
        "fonts/arial.ttf",
        SpikeDodge::GAME_OVER_FONT_SIZE
    );

    _gameOverLabel->setTextColor(Color4B::WHITE);
    _gameOverLabel->setAnchorPoint(Vec2(0.5f, 0.5f));

    _gameOverLabel->setPosition(
        SpikeDodge::DESIGN_WIDTH * 0.5f,
        SpikeDodge::DESIGN_HEIGHT * 0.50f
    );

    addChild(_gameOverLabel, kChildTagHUDGameOver);

    _survivedLabel = Label::createWithTTF(
        "Survived: 0 seconds",
        "fonts/arial.ttf",
        SpikeDodge::GAME_OVER_DETAIL_FONT_SIZE
    );

    _survivedLabel->setTextColor(Color4B::WHITE);
    _survivedLabel->setAnchorPoint(Vec2(0.5f, 0.5f));

    _survivedLabel->setPosition(
        SpikeDodge::DESIGN_WIDTH * 0.5f,
        SpikeDodge::DESIGN_HEIGHT * 0.43f
    );

    addChild(_survivedLabel, kChildTagHUDGameOver);

    _bestLabel = Label::createWithTTF(
        "Best Time: 0 seconds",
        "fonts/arial.ttf",
        SpikeDodge::GAME_OVER_DETAIL_FONT_SIZE
    );

    _bestLabel->setTextColor(Color4B::WHITE);
    _bestLabel->setAnchorPoint(Vec2(0.5f, 0.5f));

    _bestLabel->setPosition(
        SpikeDodge::DESIGN_WIDTH * 0.5f,
        SpikeDodge::DESIGN_HEIGHT * 0.375f
    );

    addChild(_bestLabel, kChildTagHUDGameOver);

    _restartLabel = Label::createWithTTF(
        "Press any W A S D key to restart",
        "fonts/arial.ttf",
        SpikeDodge::GAME_OVER_PROMPT_FONT_SIZE
    );

    _restartLabel->setTextColor(Color4B::WHITE);
    _restartLabel->setAnchorPoint(Vec2(0.5f, 0.5f));

    _restartLabel->setPosition(
        SpikeDodge::DESIGN_WIDTH * 0.5f,
        SpikeDodge::DESIGN_HEIGHT * 0.30f
    );

    addChild(_restartLabel, kChildTagHUDGameOver);

    _gameOverLabel->setVisible(false);
    _survivedLabel->setVisible(false);
    _bestLabel->setVisible(false);
    _restartLabel->setVisible(false);

    _bestTime = 0;
}

void GameScene::setupKeyboard()
{
    auto listener = EventListenerKeyboard::create();

    listener->onKeyPressed =
        [this](EventKeyboard::KeyCode keyCode, Event*)
        {
            if (keyCode == EventKeyboard::KeyCode::KEY_W ||
                keyCode == EventKeyboard::KeyCode::KEY_A ||
                keyCode == EventKeyboard::KeyCode::KEY_S ||
                keyCode == EventKeyboard::KeyCode::KEY_D)
            {
                _pressedKeys.insert(keyCode);

                if (_gameState == GameState::GameOver)
                {
                    startGame();
                }
            }
        };

    listener->onKeyReleased =
        [this](EventKeyboard::KeyCode keyCode, Event*)
        {
            _pressedKeys.erase(keyCode);
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(
        listener,
        this
    );
}

void GameScene::startGame()
{
    // -------------------------------------------------------------------------
    // Remove every active spike.
    // -------------------------------------------------------------------------

    for (auto spike : _spikes)
    {
        if (spike)
        {
            spike->removeFromParent();
        }
    }

    _spikes.clear();

    // -------------------------------------------------------------------------
    // Reset gameplay state.
    // -------------------------------------------------------------------------

    _gameState = GameState::Playing;

    _survivalTime = 0.0f;
    _displayedSurvivalSeconds = 0;

    _spikeSpeed = SpikeDodge::INITIAL_SPIKE_SPEED;
    _spawnInterval = SpikeDodge::INITIAL_SPAWN_INTERVAL;

    _spawnTimer = 0.0f;

    _playerPosition = Vec2(
        SpikeDodge::DESIGN_WIDTH * 0.5f,
        SpikeDodge::DESIGN_HEIGHT * 0.5f
    );

    _playerDrawNode->setPosition(_playerPosition);

    // -------------------------------------------------------------------------
    // Reset UI.
    // -------------------------------------------------------------------------

    _timerLabel->setString("Time: 0");

    _gameOverLabel->setVisible(false);
    _survivedLabel->setVisible(false);
    _bestLabel->setVisible(false);
    _restartLabel->setVisible(false);
}

void GameScene::endGame()
{
    if (_gameState == GameState::GameOver)
    {
        return;
    }

    _gameState = GameState::GameOver;

    const int finalSeconds =
        static_cast<int>(std::floor(_survivalTime));

    if (finalSeconds > _bestTime)
    {
        _bestTime = finalSeconds;
    }

    // -------------------------------------------------------------------------
    // Stop all active spike movement and spawning by switching state.
    // Remove existing spikes from the scene.
    // -------------------------------------------------------------------------

    for (auto spike : _spikes)
    {
        if (spike)
        {
            spike->removeFromParent();
        }
    }

    _spikes.clear();

    // -------------------------------------------------------------------------
    // Update game-over labels.
    // -------------------------------------------------------------------------

    updateGameOverUI();
}

void GameScene::spawnSpike()
{
    const int edge = std::rand() % 4;

    Vec2 spawnPosition;

    switch (edge)
    {
        case 0:
        {
            // Top edge.
            const float x =
                static_cast<float>(std::rand()) /
                static_cast<float>(RAND_MAX) *
                SpikeDodge::DESIGN_WIDTH;

            spawnPosition = Vec2(
                x,
                SpikeDodge::DESIGN_HEIGHT
            );

            break;
        }

        case 1:
        {
            // Right edge.
            const float y =
                static_cast<float>(std::rand()) /
                static_cast<float>(RAND_MAX) *
                SpikeDodge::DESIGN_HEIGHT;

            spawnPosition = Vec2(
                SpikeDodge::DESIGN_WIDTH,
                y
            );

            break;
        }

        case 2:
        {
            // Bottom edge.
            const float x =
                static_cast<float>(std::rand()) /
                static_cast<float>(RAND_MAX) *
                SpikeDodge::DESIGN_WIDTH;

            spawnPosition = Vec2(
                x,
                0.0f
            );

            break;
        }

        default:
        {
            // Left edge.
            const float y =
                static_cast<float>(std::rand()) /
                static_cast<float>(RAND_MAX) *
                SpikeDodge::DESIGN_HEIGHT;

            spawnPosition = Vec2(
                0.0f,
                y
            );

            break;
        }
    }

    const Vec2 center(
        SpikeDodge::DESIGN_WIDTH * 0.5f,
        SpikeDodge::DESIGN_HEIGHT * 0.5f
    );

    auto spike = Spike::create(
        spawnPosition,
        center,
        _spikeSpeed
    );

    if (spike)
    {
        addChild(spike, kChildTagSpike);
        _spikes.pushBack(spike);
    }
}

Vec2 GameScene::getMovementDirection() const
{
    Vec2 direction = Vec2::ZERO;

    if (_pressedKeys.count(
            EventKeyboard::KeyCode::KEY_W) > 0)
    {
        direction.y += 1.0f;
    }

    if (_pressedKeys.count(
            EventKeyboard::KeyCode::KEY_S) > 0)
    {
        direction.y -= 1.0f;
    }

    if (_pressedKeys.count(
            EventKeyboard::KeyCode::KEY_D) > 0)
    {
        direction.x += 1.0f;
    }

    if (_pressedKeys.count(
            EventKeyboard::KeyCode::KEY_A) > 0)
    {
        direction.x -= 1.0f;
    }

    if (direction.lengthSquared() > 0.0f)
    {
        direction.normalize();
    }

    return direction;
}

void GameScene::updatePlayer(float delta)
{
    const Vec2 direction = getMovementDirection();

    _playerPosition +=
        direction * SpikeDodge::PLAYER_SPEED * delta;

    // The specification clamps the player CENTER to the screen bounds.
    _playerPosition.x = clampf(
        _playerPosition.x,
        0.0f,
        SpikeDodge::DESIGN_WIDTH
    );

    _playerPosition.y = clampf(
        _playerPosition.y,
        0.0f,
        SpikeDodge::DESIGN_HEIGHT
    );

    _playerDrawNode->setPosition(_playerPosition);
}

void GameScene::updateSpikes(float delta)
{
    for (int i = static_cast<int>(_spikes.size()) - 1;
         i >= 0;
         --i)
    {
        auto spike = _spikes.at(i);

        if (!spike)
        {
            _spikes.erase(i);
            continue;
        }

        spike->updateSpike(delta);

        // -------------------------------------------------------------
        // Collision.
        // -------------------------------------------------------------

        const float distance =
            _playerPosition.distance(spike->getPosition());

        if (distance < SpikeDodge::SPIKE_COLLISION_DISTANCE)
        {
            endGame();
            return;
        }

        // -------------------------------------------------------------
        // Remove spikes once their center leaves the opposite side
        // of the play area.
        // -------------------------------------------------------------

        if (isSpikeOutsideScreen(spike))
        {
            spike->removeFromParent();
            _spikes.erase(i);
        }
    }
}

bool GameScene::isSpikeOutsideScreen(const Spike* spike) const
{
    if (!spike)
    {
        return true;
    }

    const Vec2 position = spike->getPosition();

    return
        position.x < 0.0f ||
        position.x > SpikeDodge::DESIGN_WIDTH ||
        position.y < 0.0f ||
        position.y > SpikeDodge::DESIGN_HEIGHT;
}

void GameScene::updateDifficulty()
{
    const int difficultyLevel =
        static_cast<int>(
            std::floor(_survivalTime) /
            SpikeDodge::DIFFICULTY_INTERVAL_SECONDS
        );

    _spikeSpeed =
        SpikeDodge::INITIAL_SPIKE_SPEED +
        static_cast<float>(difficultyLevel) *
        SpikeDodge::SPIKE_SPEED_INCREMENT;

    _spawnInterval =
        SpikeDodge::INITIAL_SPAWN_INTERVAL -
        static_cast<float>(difficultyLevel) *
        SpikeDodge::SPAWN_INTERVAL_DECREMENT;

    _spawnInterval = std::max(
        SpikeDodge::MIN_SPAWN_INTERVAL,
        _spawnInterval
    );
}

void GameScene::updateTimerLabel()
{
    const int seconds =
        static_cast<int>(std::floor(_survivalTime));

    if (seconds == _displayedSurvivalSeconds)
    {
        return;
    }

    _displayedSurvivalSeconds = seconds;

    _timerLabel->setString(
        StringUtils::format(
            "Time: %d",
            seconds
        )
    );
}

void GameScene::updateGameOverUI()
{
    const int finalSeconds =
        static_cast<int>(std::floor(_survivalTime));

    _gameOverLabel->setVisible(true);
    _survivedLabel->setVisible(true);
    _bestLabel->setVisible(true);
    _restartLabel->setVisible(true);

    _survivedLabel->setString(
        StringUtils::format(
            "Survived: %d seconds",
            finalSeconds
        )
    );

    _bestLabel->setString(
        StringUtils::format(
            "Best Time: %d seconds",
            _bestTime
        )
    );
}

void GameScene::update(float delta)
{
    Scene::update(delta);
    if (_gameState == GameState::GameOver)
    {
        return;
    }

    // -------------------------------------------------------------------------
    // Survival timer.
    // -------------------------------------------------------------------------

    _survivalTime += delta;

    updateTimerLabel();

    // -------------------------------------------------------------------------
    // Difficulty is based on the number of complete 10-second intervals.
    // -------------------------------------------------------------------------

    updateDifficulty();

    // -------------------------------------------------------------------------
    // Player movement.
    // -------------------------------------------------------------------------

    updatePlayer(delta);

    // -------------------------------------------------------------------------
    // Spike spawning.
    // -------------------------------------------------------------------------

    _spawnTimer += delta;

    while (_spawnTimer >= _spawnInterval)
    {
        _spawnTimer -= _spawnInterval;

        spawnSpike();
    }

    // -------------------------------------------------------------------------
    // Spike movement and collision.
    // -------------------------------------------------------------------------

    updateSpikes(delta);
}
