#include <gtkmm.h>
#include <glibmm.h>
#include <windows.h>
#include <string>
#include <iostream>
#include <sqlite3.h>

using namespace Gtk;
using namespace std;

int main(int argc, char** argv)
{
    auto app = Application::create(argc, argv, "org.autocarrinhos.esas");
    auto settings = Settings::get_default();

    Glib::RefPtr<CssProvider> cssProvider = CssProvider::create();

    if(!cssProvider->load_from_path("theme.css")) 
    {
        std::cerr << "Failed to load CSS file.\n";
    } 
    else 
    {
        Glib::RefPtr<Gdk::Screen> screen = Gdk::Screen::get_default();
        StyleContext::add_provider_for_screen(screen, cssProvider, GTK_STYLE_PROVIDER_PRIORITY_USER);
    }

    settings->property_gtk_application_prefer_dark_theme() = true;

    Window appWindow;
    appWindow.set_title("Autocarrinhos ESAS");
    appWindow.set_default_size(1200, 700);
    appWindow.set_position(WindowPosition::WIN_POS_CENTER);
    appWindow.set_resizable(true);

    Stack stack;

    //Log In
    Box loginBox;

    loginBox.set_spacing(10);
    loginBox.set_orientation(Orientation::ORIENTATION_VERTICAL);
    loginBox.set_homogeneous(true);

    Label title("Entre na sua conta"), erro("");
    Entry usernameEntry, passwordEntry;
    Button loginButton("Entrar"), registerButton("Registar");

    usernameEntry.set_size_request(70, 50);
    passwordEntry.set_size_request(70, 50);
    loginButton.set_size_request(70, 50);
    registerButton.set_size_request(70, 50);

    usernameEntry.set_placeholder_text("Utilizador");
    passwordEntry.set_placeholder_text("Palavra-passe");
    passwordEntry.set_visibility(false);

    loginBox.pack_start(title, false, false, 0);
    loginBox.pack_start(usernameEntry, false, false, 0);
    loginBox.pack_start(passwordEntry, false, false, 0);
    loginBox.pack_start(loginButton, false, false, 0);
    loginBox.pack_start(registerButton, false, false, 0);
    loginBox.pack_start(erro, false, false, 0);
    stack.add(loginBox, "login");

    //Dashboard
    Box dashboardBox;
    Label dashboardLabel("Welcome, " + usernameEntry.get_text() + "!");
    dashboardBox.pack_start(dashboardLabel);
    stack.add(dashboardBox, "dashboard");

    loginButton.signal_clicked().connect([&stack, &usernameEntry,&passwordEntry, &dashboardLabel, &erro] {
        if(usernameEntry.get_text() == "admin" && passwordEntry.get_text() == "admin") {
            dashboardLabel.set_text("Welcome, " + usernameEntry.get_text() + "!");
            stack.set_visible_child("dashboard");
        } else {
            erro.set_text("Utilizador ou palavra-passe incorretos");
        }
    });

    Alignment* align = manage(new Alignment(0.5, 0.5, 0, 0));
        align->add(stack);
        appWindow.add(*align);
    appWindow.show_all();

    return app->run(appWindow);
}