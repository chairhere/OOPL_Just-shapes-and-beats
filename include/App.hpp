#ifndef APP_HPP
#define APP_HPP

#include "Tool/ScreenState.hpp"
#include "pch.hpp" // IWYU pragma: export
#include "Material/Player.hpp"
#include "Screen/Screen.hpp"
#include "Util/TransformUtils.hpp"
#include "Util/Renderer.hpp"
#include "Material/FadeLayer.hpp"
#include "Screen/PlaygroundScreen.hpp"

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

    Util::Renderer m_Root;
    std::shared_ptr<Screen> m_CurrentScreen;
    ScreenState m_CurrentLevel = ScreenState::Main;
    void ChangeLevel(ScreenState newLevel);
    bool setting = false;

    std::shared_ptr<FadeLayer> m_FadeLayer;

    State m_CurrentState = State::START;

    bool direct_playground = false;
};

#endif
