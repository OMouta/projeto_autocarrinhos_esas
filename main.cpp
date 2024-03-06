#include <gtkmm.h>

using namespace Gtk;

int main(int argc, char** argv)
{
        auto app = Application::create(argc, argv);

        Glib::RefPtr<Gtk::Settings> settings = Gtk::Settings::get_default();
        if(settings)
        {
                settings->property_gtk_application_prefer_dark_theme() = true;
        }
        
        Window window;
        window.set_default_size(920,700);

        Box box;
        window.add(box);

        Button button("Click me");
        box.pack_start(button);
        button.show();
        box.show();
        
        return app->run(window);
}