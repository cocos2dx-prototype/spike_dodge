//
//  GameScene.hpp
//  SpikeDodge-desktop
//
//  Created by hoangbui-fe on 10/8/26.
//

#ifndef GameScene_hpp
#define GameScene_hpp

#include "cocos2d.h"
#include "Spike.hpp"

#include <unordered_set>

enum class GameState {
    Playing,
    GameOver
};

class GameScene : public cocos2d::Scene
{
public:
    GameScene();
    ~GameScene();
    
public:
    enum ChildTag {
        None = 0,
        kChildTagBackground,
        kChildTagPlayer,
        kChildTagSpike,
        kChildTagHUD,
        kChildTagHUDGameOver,
        
    };
    
public:
    static cocos2d::Scene* createScene();
    
private:
    CREATE_FUNC(GameScene);
    virtual bool init() override;
    virtual void update(float delta) override;
    virtual void onEnter() override;
    virtual void onExit() override;
    
private:
    // -------------------------------------------------------------------------
    // Input
    // -------------------------------------------------------------------------
    
    std::unordered_set<cocos2d::EventKeyboard::KeyCode> _pressedKeys;
    
    // -------------------------------------------------------------------------
    // Gameplay nodes
    // -------------------------------------------------------------------------
    
    cocos2d::DrawNode* _playerDrawNode;
    
    cocos2d::Label* _timerLabel;
    
    cocos2d::Label* _gameOverLabel;
    cocos2d::Label* _survivedLabel;
    cocos2d::Label* _bestLabel;
    cocos2d::Label* _restartLabel;
    
    cocos2d::Vector<Spike*> _spikes;
    
    // -------------------------------------------------------------------------
    // Gameplay state
    // -------------------------------------------------------------------------
    
    GameState _gameState;
    
    float _survivalTime;
    int _displayedSurvivalSeconds;
    
    float _spikeSpeed;
    float _spawnInterval;
    float _spawnTimer;
    
    int _bestTime;
    
    cocos2d::Vec2 _playerPosition;
    
private:
    
    void _initValues();
    void _initObjects();
    
    // -------------------------------------------------------------------------
    // Scene setup
    // -------------------------------------------------------------------------
    
    void setupBackground();
    void setupPlayer();
    void setupUI();
    void setupKeyboard();
    
    // -------------------------------------------------------------------------
    // Gameplay
    // -------------------------------------------------------------------------
    
    void startGame();
    void endGame();
    
    void spawnSpike();
    void updatePlayer(float delta);
    void updateSpikes(float delta);
    void updateDifficulty();
    
    void updateTimerLabel();
    void updateGameOverUI();
    
    // -------------------------------------------------------------------------
    // Helpers
    // -------------------------------------------------------------------------
    
    cocos2d::Vec2 getMovementDirection() const;
    bool isSpikeOutsideScreen(const Spike* spike) const;
    
};

#endif /* GameScene_hpp */
