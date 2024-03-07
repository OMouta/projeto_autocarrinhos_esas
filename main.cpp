#include <gtkmm.h>

using namespace Gtk;

int main(int argc, char** argv)
{
        auto app = Application::create(argc, argv);
        
        Window window;
        window.set_default_size(920,700);

        Box box;
        window.add(box);
        box.show();
        
        return app->run(window);
}