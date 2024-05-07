#ifndef APP_H
#define APP_H

#include "Engine/application.h"
#include "Warmup/gamescreen.h"
#include "Warmup/menuscreen.h"

class App : public Application {
private:
    std::shared_ptr<GameScreen> m_gameScreen;
    std::shared_ptr<MenuScreen> m_menuScreen;
public:
    App();
    void mouseButtonEvent(int button, int action) override;
    void keyEvent(int key, int action) override;
};

#endif // APP_H
