#include "App.hpp"

#include "MainMenuScreen.hpp"
#include "Player.hpp"
#include "PlaygroundScreen.hpp"
#include "PlaygroundScreen.hpp"
#include "TimeLine.hpp"
#include "SongsBPM.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

void App::Start() {
    LOG_TRACE("Start");

    SDL_Window* window = SDL_GL_GetCurrentWindow();
    if (window != nullptr) {
        SDL_SetWindowBordered(window, SDL_FALSE);  //無框
        SDL_SetWindowSize(window, 1920, 1080);  //1920*1080
        SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    }

    m_CurrentScreen = std::make_shared<MainMenuScreen>();
    m_CurrentLevel = ScreenState::Main;

    m_Player = std::make_shared<Player>();
    m_Player->SetPosition(glm::vec2(0.0f, 0.0f));
    m_Player->SetZIndex(50);
    m_Player->SetVisible(false);
    m_Root.AddChild(m_Player);

    SDL_StopTextInput();

    m_CurrentState = State::UPDATE;
}

void App::Update() {
    
    m_Player->Moving();
/*
    if (m_FadeLayer && !m_FadeLayer->IsFinished()) {
        m_FadeLayer->Update(); // 推進 1.5 秒的計時與透明度變化
        LOG_DEBUG("Fade Layer Updated");
    }
    // 當動畫播完後，將其從渲染清單移除並釋放資源
    else if (m_FadeLayer && m_FadeLayer->IsFinished()) {
        //m_Root.RemoveChild(m_FadeLayer); // 從畫面中剔除 [5]
        m_FadeLayer = nullptr;               // 清空指標，釋放記憶體
        LOG_DEBUG("Fade Layer finished");
    }
*/
    // Render all game objects managed by the root renderer.
    if (m_CurrentScreen) {
        ScreenState newLevel = m_CurrentScreen->Update();

        if (newLevel != m_CurrentLevel) {
            ChangeLevel(newLevel);
        }
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

    //滑鼠位置顯示
    if (true && Util::Input::IsKeyDown(Util::Keycode::TAB)) {
        glm::vec2 mousePos = Util::Input::GetCursorPosition();
        std::string log = "mousePos(";
        log.append(std::to_string(mousePos.x));
        log.append(", ");
        log.append(std::to_string(mousePos.y));
        log.append(")");
        LOG_DEBUG(log);
    }
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}

void App::ChangeLevel(ScreenState newLevel) {
    m_CurrentLevel = newLevel;
    switch (m_CurrentLevel) {
        case ScreenState::Exit:
            m_CurrentState = State::END;
            break;
        case ScreenState::Main:
            m_CurrentScreen = std::make_shared<MainMenuScreen>();
            break;
        case ScreenState::LevelList:
            m_CurrentScreen = std::make_shared<PlaygroundScreen>();
            break;
        case ScreenState::Settlement:
            // m_CurrentScreen = std::make_shared<>();
            break;
        case ScreenState::GameOver:
            // m_CurrentScreen = std::make_shared<>();
            break;
        case ScreenState::Playground:
            m_CurrentScreen = std::make_shared<PlaygroundScreen>();
            break;
    }
}
