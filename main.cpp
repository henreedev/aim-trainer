#include "Engine/application.h"
#include "Engine/window.h"
#include "Warmup/App.h"

int main(int argc, char *argv[])
{
    std::unique_ptr<Window> m_window = std::make_unique<Window>();

    std::cout<<"Start"<<std::endl;
    // Pass the app into window to run the game
    if(m_window->start(0) != 0){
        return -1;
    }

    std::cout<<"Loop"<<std::endl;
    m_window->loop();
}
