//
//  Spike.cpp
//  SpikeDodge-desktop
//
//  Created by hoangbui-fe on 10/8/26.
//

#include "Spike.hpp"
#include "Constants.h"

#include <cmath>

USING_NS_CC;

Spike* Spike::create(
    const Vec2& spawnPosition,
    const Vec2& targetPosition,
    float speed)
{
    auto spike = new (std::nothrow) Spike();

    if (spike && spike->init(spawnPosition, targetPosition, speed))
    {
        spike->autorelease();
        return spike;
    }

    delete spike;
    return nullptr;
}

bool Spike::init(
    const Vec2& spawnPosition,
    const Vec2& targetPosition,
    float speed)
{
    if (!Node::init())
    {
        return false;
    }

    _drawNode = DrawNode::create();

    if (!_drawNode)
    {
        return false;
    }

    addChild(_drawNode, kChildTagDrawNode);

    setPosition(spawnPosition);

    // -------------------------------------------------------------------------
    // Calculate normalized direction from spawn position toward screen center.
    // -------------------------------------------------------------------------

    Vec2 direction = targetPosition - spawnPosition;

    if (direction.lengthSquared() > 0.0f)
    {
        direction.normalize();
    }

    _velocity = direction * speed;

    // -------------------------------------------------------------------------
    // Create triangle.
    //
    // Local triangle:
    //
    //             tip
    //              /\
    //             /  \
    //            /    \
    //       base -------
    //
    // Height = 24
    // Base   = 16
    //
    // The triangle points along +X and the node is rotated toward the center.
    // -------------------------------------------------------------------------

    const float halfBase = SpikeDodge::SPIKE_BASE_WIDTH * 0.5f;

    Vec2 vertices[3] =
    {
        Vec2(SpikeDodge::SPIKE_HEIGHT, 0.0f),
        Vec2(0.0f, halfBase),
        Vec2(0.0f, -halfBase)
    };

    _drawNode->drawSolidPoly(
        vertices,
        3,
        Color4F(
            SpikeDodge::SPIKE_RED,
            SpikeDodge::SPIKE_GREEN,
            SpikeDodge::SPIKE_BLUE,
            1.0f
        )
    );

    // -------------------------------------------------------------------------
    // Rotate the triangle so its tip points toward the screen center.
    // -------------------------------------------------------------------------

    const float angleRadians =
        std::atan2(direction.y, direction.x);

    setRotation(
        -CC_RADIANS_TO_DEGREES(angleRadians)
    );

    return true;
}

void Spike::updateSpike(float delta)
{
    setPosition(
        getPosition() + _velocity * delta
    );
}

float Spike::getCollisionRadius() const
{
    return SpikeDodge::SPIKE_EFFECTIVE_RADIUS;
}
