#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export
#include "Player.hpp"
#include "Time_Line.hpp"
#include "Util/TransformUtils.hpp"
#include "Util/Renderer.hpp"

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

    std::shared_ptr<Player> m_Player;
    std::shared_ptr<Time_Line> Main_Menu_Music;

    State m_CurrentState = State::START;
};

#endif
