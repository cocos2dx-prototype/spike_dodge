//
//  Spike.hpp
//  SpikeDodge-desktop
//
//  Created by hoangbui-fe on 10/8/26.
//

#ifndef Spike_hpp
#define Spike_hpp

#include "cocos2d.h"

class Spike : public cocos2d::Node
{
public:
    enum ChildTag {
        None = 0,
        kChildTagDrawNode,
    };
    
public:
    static Spike* create(
        const cocos2d::Vec2& spawnPosition,
        const cocos2d::Vec2& targetPosition,
        float speed
    );

private:
    bool init(
        const cocos2d::Vec2& spawnPosition,
        const cocos2d::Vec2& targetPosition,
        float speed
    );

private:
    cocos2d::DrawNode* _drawNode;
    cocos2d::Vec2 _velocity;
    
public:
    void updateSpike(float delta);

    float getCollisionRadius() const;
};

#endif /* Spike_hpp */
