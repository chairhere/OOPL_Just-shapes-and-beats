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
#include "Screen/SettingScreen.hpp"

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

    void Conversion_animate();

    static void RestrictSetting(bool need_restrict);

private:

    Util::Renderer m_Root;
    std::shared_ptr<Screen> m_CurrentScreen;
    ScreenState m_CurrentLevel = ScreenState::Main;
    void ChangeLevel(ScreenState newLevel);
    void SettingBehavior(ScreenState settingCommand);
    bool conversion = false;

    std::shared_ptr<FadeLayer> m_FadeLayer;

    State m_CurrentState = State::START;

    std::shared_ptr<SettingScreen> m_SettingScreen;
    inline static bool setting = false;
    inline static bool setting_restrict = false;

    bool direct_playground = false;

    std::vector<Util::Color> First_Color = { {0, 0, 0, 255}, {0, 0, 0, 0}};
    std::vector<float> First_Duration = {0.0f, 2000.0f};
    std::vector<float> First_Rotation = {0.0f, 0.0f};
    std::vector<float> First_Vertices = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f};
    //std::vector<float> First_Vertices = {-0.288f, 0.5f, -0.577f, -0.0f, -0.288f, -0.5f, 0.288f, -0.5f, 0.577f, 0.0f, 0.288f, 0.5f};
    std::vector<glm::vec2> First_Position = {{0.0f, 0.0f}, {0.0f, 0.0f}};
    std::vector<glm::vec2> First_Scale = {{WINDOW_WIDTH, WINDOW_HEIGHT}, {WINDOW_WIDTH, WINDOW_HEIGHT}};

};

#endif
