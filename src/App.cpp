#include "App.hpp"

#include "MainMenuScreen.hpp"
#include "MusicPlayerManager.hpp"
#include "Player.hpp"
#include "PlaygroundScreen.hpp"
#include "PlaygroundScreen.hpp"
#include "SongListScreen.hpp"
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
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;  // 防止ImGui改變游標狀態，讓專案控制
    io.ConfigWindowsMoveFromTitleBarOnly = true;

    if (direct_playground) {
        MusicPlayerManager::Setting().AddMusic(Levels::Chronos);
        m_CurrentScreen = std::make_shared<OpeningAnimateScreen>(MusicPlayerManager::Setting().GetCurrentLevel());
        m_CurrentLevel = ScreenState::Playground;
    }else {
        m_CurrentScreen = std::make_shared<MainMenuScreen>();
        m_CurrentLevel = ScreenState::Main;
    }

    SDL_StopTextInput();

    m_CurrentState = State::UPDATE;
}

void App::Update() {
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
    if (Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE)) {
        if (setting) {
            // m_Root.RemoveChild(SettingScreen);
            setting = false;
        }else {
            switch (m_CurrentLevel) {
                case ScreenState::Main:
                    ChangeLevel(ScreenState::Exit);
                    MusicPlayerManager::Setting().PlayEffect(MusicPlayerManager::Return);
                    break;
                case ScreenState::LevelList:
                    ChangeLevel(ScreenState::Main);
                    MusicPlayerManager::Setting().PlayEffect(MusicPlayerManager::Return);
                    break;
                case ScreenState::Playground:
                    ChangeLevel(ScreenState::Main);
                    MusicPlayerManager::Setting().PlayEffect(MusicPlayerManager::Return);
                    // pause game
                    // Root.AddChild(SettingScreen);
                    break;
                default:
                    break;
            }
        }

    }

    if (Util::Input::IsKeyUp(Util::Keycode::O)) {
        switch (m_CurrentLevel) {
            case ScreenState::Main:
                // Root.AddChild(SettingScreen);
                // setting = true;
                break;
            case ScreenState::LevelList:
                // Root.AddChild(SettingScreen);
                // setting = true;
                break;
            default:
                break;
        }
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
            m_CurrentScreen = std::make_shared<SongListScreen>();
            break;
        case ScreenState::Settlement:
            // m_CurrentScreen = std::make_shared<>();
            break;
        case ScreenState::Playground:
            m_CurrentScreen = std::make_shared<OpeningAnimateScreen>(MusicPlayerManager::Setting().GetCurrentLevel());
            break;
    }
}
