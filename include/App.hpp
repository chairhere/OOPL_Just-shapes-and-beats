#ifndef APP_HPP
#define APP_HPP

#include "ScreenState.hpp"
#include "pch.hpp" // IWYU pragma: export
#include "Player.hpp"
#include "Screen.hpp"
#include "TimeLine.hpp"
#include "Util/TransformUtils.hpp"
#include "Util/Renderer.hpp"
#include "FadeLayer.hpp"

class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    State GetCurrentState() const { return m_CurrentState; }

    void Start();

    void Update();

    void End(); // NOLINT(readability-convert-member-functions-to-static)

private:
    void ValidTask();

private:

    Util::Renderer m_Root;
    std::shared_ptr<Screen> m_CurrentScreen;
    ScreenState m_CurrentLevel;
    void ChangeLevel(ScreenState newLevel);

    std::shared_ptr<Player> m_Player;

    std::shared_ptr<FadeLayer> m_FadeLayer;

    State m_CurrentState = State::START;
};

#endif
