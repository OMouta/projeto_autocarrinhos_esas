#include <gtkmm.h>
#include <glibmm.h>
#include <windows.h>

using namespace Gtk;
using namespace std;

int main(int argc, char** argv)
{
    auto app = Application::create(argc, argv);

    Window appWindow;
    appWindow.set_title("Autocarrinhos ESAS - Log In");
    appWindow.set_default_size(1200, 700);

    Box Login;
    Login.set_size_request(600, 350);
    appWindow.add(Login);

    Grid LoginGrid;
    LoginGrid.set_column_homogeneous(true);
    LoginGrid.set_row_homogeneous(true);
    LoginGrid.set_row_spacing(10);
    LoginGrid.set_column_spacing(10);
    Login.pack_start(LoginGrid);

    Entry utilizador;
    utilizador.set_placeholder_text("Utilizador");
    LoginGrid.attach(utilizador, 0, 0, 1, 1);

    Entry palavrapasse;
    palavrapasse.set_placeholder_text("Palavra passe");
    palavrapasse.set_visibility(false);
    LoginGrid.attach(palavrapasse, 0, 1, 1, 1);

    Button loginButton("Login");
    LoginGrid.attach(loginButton, 0, 2, 1, 1);

    Login.show_all();

    return app->run(appWindow);
}