#include "App.hpp"

#include "MainMenuScreen.hpp"
#include "Player.hpp"
#include "TimeLine.hpp"
#include "SongsBPM.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

void App::Start() {
    LOG_TRACE("Start");

    m_CurrentScreen = std::make_shared<MainMenuScreen>();

    m_Player = std::make_shared<Player>();
    m_Player->SetPosition(glm::vec2(0.0f, 0.0f));
    m_Player->SetZIndex(50);
    m_Root.AddChild(m_Player);

    Main_Menu_Music = std::make_shared<TimeLine>("../Resources/Audio/Main_Menu.mp3", static_cast<float>(SongsBPM::Main_Menu));
    Main_Menu_Music->Start();

    SDL_StopTextInput();

    m_CurrentState = State::UPDATE;
}

void App::Update() {
    
    m_Player->Moving();
    Main_Menu_Music->Update();

    // Render all game objects managed by the root renderer.
    if (m_CurrentScreen) {
        m_CurrentScreen->Update();
    }
    m_Root.Update();

    /*
     * Do not touch the code below as they serve the purpose for
     * closing the window.
     */
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
