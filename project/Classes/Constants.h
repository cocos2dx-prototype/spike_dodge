//
//  Constants.h
//  SpikeDodge-desktop
//
//  Created by hoangbui-fe on 10/8/26.
//

#ifndef Constants_h
#define Constants_h

namespace SpikeDodge
{
    // -------------------------------------------------------------------------
    // Design resolution
    // -------------------------------------------------------------------------

    constexpr float DESIGN_WIDTH = 960.0f;
    constexpr float DESIGN_HEIGHT = 640.0f;

    // -------------------------------------------------------------------------
    // Player
    // -------------------------------------------------------------------------

    constexpr float PLAYER_RADIUS = 12.0f;
    constexpr float PLAYER_SPEED = 220.0f;

    // -------------------------------------------------------------------------
    // Spike
    // -------------------------------------------------------------------------

    constexpr float SPIKE_BASE_WIDTH = 16.0f;
    constexpr float SPIKE_HEIGHT = 24.0f;

    constexpr float SPIKE_EFFECTIVE_RADIUS = 16.0f;
    constexpr float SPIKE_COLLISION_DISTANCE =
        PLAYER_RADIUS + SPIKE_EFFECTIVE_RADIUS;

    constexpr float INITIAL_SPIKE_SPEED = 200.0f;
    constexpr float SPIKE_SPEED_INCREMENT = 20.0f;

    // -------------------------------------------------------------------------
    // Spawn timing
    // -------------------------------------------------------------------------

    constexpr float INITIAL_SPAWN_INTERVAL = 0.8f;
    constexpr float SPAWN_INTERVAL_DECREMENT = 0.05f;
    constexpr float MIN_SPAWN_INTERVAL = 0.25f;

    constexpr int DIFFICULTY_INTERVAL_SECONDS = 10;

    // -------------------------------------------------------------------------
    // Colors
    // -------------------------------------------------------------------------

    constexpr float BACKGROUND_RED = 0.0f;
    constexpr float BACKGROUND_GREEN = 0.015f;
    constexpr float BACKGROUND_BLUE = 0.10f;

    constexpr float SPIKE_RED = 1.0f;
    constexpr float SPIKE_GREEN = 0.25f;
    constexpr float SPIKE_BLUE = 0.22f;

    // -------------------------------------------------------------------------
    // UI
    // -------------------------------------------------------------------------

    constexpr float TIMER_MARGIN_RIGHT = 24.0f;
    constexpr float TIMER_MARGIN_TOP = 20.0f;

    constexpr int TIMER_FONT_SIZE = 32;

    constexpr int GAME_OVER_FONT_SIZE = 52;
    constexpr int GAME_OVER_DETAIL_FONT_SIZE = 26;
    constexpr int GAME_OVER_PROMPT_FONT_SIZE = 22;
}

#endif /* Constants_h */
