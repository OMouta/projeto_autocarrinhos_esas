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
    stack.set_transition_type(StackTransitionType::STACK_TRANSITION_TYPE_OVER_DOWN_UP);
    stack.set_transition_duration(500);

    //os #if e #endif é pra organizar o codigo, o 1 ativa e o 0 desativa a parte do codigo! MANTER NO 1 PARA A FUNCIONAR CORRETAMENTE!

    #if 1 //Log in
    Box loginBox;
    loginBox.set_spacing(15);
    loginBox.set_orientation(Orientation::ORIENTATION_VERTICAL);
    loginBox.set_homogeneous(false);

    Image logo("assets/logo-med.png");

    Label title("Entre na sua conta"),erro("");
    title.set_name("title");
    erro.set_name("erro");

    Entry usernameEntry, passwordEntry;
    Button loginButton("Entrar"), registerButton("Registrar");

    usernameEntry.set_size_request(70, 50);
    passwordEntry.set_size_request(70, 50);
    loginButton.set_size_request(70, 50);
    registerButton.set_size_request(70, 50);
    erro.set_size_request(70, 5);
    logo.set_size_request(70, 70);

    usernameEntry.set_placeholder_text("Utilizador");
    passwordEntry.set_placeholder_text("Palavra-passe");
    passwordEntry.set_visibility(false);

    loginBox.pack_start(logo, false, false, 0);
    loginBox.pack_start(title, false, false, 0);
    loginBox.pack_start(usernameEntry, false, false, 0);
    loginBox.pack_start(passwordEntry, false, false, 0);
    loginBox.pack_start(erro, false, false, 0);
    loginBox.pack_start(loginButton, false, false, 0);
    loginBox.pack_start(registerButton, false, false, 0);
    stack.add(loginBox, "login");
    #endif

    #if 1 //registrar
    Box registrarBox;
    registrarBox.set_spacing(15);
    registrarBox.set_orientation(Orientation::ORIENTATION_VERTICAL);
    registrarBox.set_homogeneous(false);

    Image logoreg("assets/logo-med.png");

    Label titlereg("Crie a sua conta"),erroreg("");
    titlereg.set_name("title");
    erroreg.set_name("erro");

    Entry usernamereg, passwordreg, passwordregconfirm;
    Button registerButton2("Registar");

    usernamereg.set_size_request(70, 50);
    passwordreg.set_size_request(70, 50);
    passwordregconfirm.set_size_request(70, 50);
    registerButton2.set_size_request(70, 50);
    erroreg.set_size_request(70, 5);
    logo.set_size_request(70, 70);

    usernamereg.set_placeholder_text("Nome de utilizador");
    passwordreg.set_placeholder_text("Palavra-passe");
    passwordreg.set_visibility(false);
    passwordregconfirm.set_placeholder_text("Confirme a palavra-passe");
    passwordregconfirm.set_visibility(false);

    registrarBox.pack_start(logoreg, false, false, 0);
    registrarBox.pack_start(titlereg, false, false, 0);
    registrarBox.pack_start(usernamereg, false, false, 0);
    registrarBox.pack_start(passwordreg, false, false, 0);
    registrarBox.pack_start(passwordregconfirm, false, false, 0);
    registrarBox.pack_start(erroreg, false, false, 0);
    registrarBox.pack_start(registerButton2, false, false, 0);
    stack.add(registrarBox, "registrar");
    #endif

    #if 1 //DashboardUser
    Box DashboardUser, TopBarUser;
    Button procurarbuttonCarros("Procurar Carros"), historicobutton("Histórico de transações"), defbutton("Definições de conta"), Suportebutton("Suporte ao cliente"), logoutbuttonUser("Sair");
    Stack contentStackUser;
    Label dashboardLabelUser("Bem-vindo, admin");

    Box procurarCarrosBox, historicobox, defbox, Suportebox;

    logoutbuttonUser.set_size_request(40, 30);

    contentStackUser.add(procurarCarrosBox, "procurarCarros");
    contentStackUser.add(historicobox, "historicobox");
    contentStackUser.add(defbox, "defbox");
    contentStackUser.add(Suportebox, "Suportebox");

    DashboardUser.set_orientation(Orientation::ORIENTATION_VERTICAL);
    TopBarUser.set_orientation(Orientation::ORIENTATION_HORIZONTAL);

    TopBarUser.pack_start(procurarbuttonCarros);
    TopBarUser.pack_start(historicobutton);
    TopBarUser.pack_start(defbutton);
    TopBarUser.pack_start(Suportebutton);
    TopBarUser.pack_start(logoutbuttonUser);

    procurarbuttonCarros.signal_clicked().connect([&contentStackUser] {
        contentStackUser.set_visible_child("procurarCarros");
    });

    historicobutton.signal_clicked().connect([&contentStackUser] {
        contentStackUser.set_visible_child("historicobox");
    });

    defbutton.signal_clicked().connect([&contentStackUser] {
        contentStackUser.set_visible_child("defbox");
    });

    Suportebutton.signal_clicked().connect([&contentStackUser] {
        contentStackUser.set_visible_child("Suportebox");
    });

    logoutbuttonUser.signal_clicked().connect([&stack] {
        stack.set_visible_child("login");
    });

    DashboardUser.pack_start(TopBarUser, PACK_SHRINK);
    DashboardUser.pack_start(contentStackUser);
    DashboardUser.pack_start(dashboardLabelUser, PACK_SHRINK);

    stack.add(DashboardUser, "dashboarduser");
    #endif

    #if 1 //butoes, ajuda pls
    loginButton.signal_clicked().connect([&stack, &usernameEntry,&passwordEntry, &dashboardLabelUser, &erro] {
        if(usernameEntry.get_text() == "admin" && passwordEntry.get_text() == "admin") {
            dashboardLabelUser.set_text("Welcome, " + usernameEntry.get_text() + "!");
            stack.set_visible_child("dashboarduser");
        } else {
            erro.set_text("Utilizador ou palavra-passe incorretos");
        }
    });

    registerButton.signal_clicked().connect([&stack, &erro] {
        erro.set_text("");
        stack.set_visible_child("registrar");
    });

    registerButton2.signal_clicked().connect([&stack, &usernamereg, &passwordreg, &passwordregconfirm, &erroreg] {
        if(passwordreg.get_text() == passwordregconfirm.get_text()) {
            erroreg.set_text("");
            stack.set_visible_child("login");
        } else {
            erroreg.set_text("As palavras-passe não coincidem");
        }
    });
    #endif

    Alignment* align = manage(new Alignment(0.5, 0.5, 0, 0));
        align->add(stack);
        appWindow.add(*align);
    appWindow.show_all();

    return app->run(appWindow);
}