#include <gtkmm.h>
#include <glibmm.h>
#include <windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

using namespace Gtk;
using namespace std;

bool authenticateUser(string &usernameEntry, string &passwordEntry){
    //Ruta para o ficheiro do utilizador
    std::string filePath = "dados/utilizadores/" + usernameEntry + "/info.txt";

    //Abrir o ficheiro do utilizador
    std::ifstream passwordFile(filePath);

}

bool pathExists(const std::filesystem::path &p)
{
    return std::filesystem::exists(p);
}

bool ficheirocheck(const string &nomeficheiro)
{
    ifstream ficheiro(nomeficheiro);
    return ficheiro.good();
}

struct Utilizador
{
    string nomeutilizador, palavraPasse, email, contacto, nome, morada, nif, ccnumero, cvv;
    bool eadmin;
} utilizadoratual;

void criarutilizador(string nomeutilizador, string palavraPasse)
{
    filesystem::create_directory("dados/utilizadores/" + nomeutilizador);
    ofstream ficheiro("dados/utilizadores/" + nomeutilizador + "/info.txt");
    ficheiro << "Nome de utilizador: " << nomeutilizador << endl;
    ficheiro << "Palavra-passe: " << palavraPasse << endl;
    ficheiro << "Email: " << endl;
    ficheiro << "Contacto: " << endl;
    ficheiro << "Nome: " << endl;
    ficheiro << "Morada: " << endl;
    ficheiro << "NIF: " << endl;
    ficheiro << "Numero do cartão de crédito: " << endl;
    ficheiro << "CVV: " << endl;
    ficheiro.close();
}

int main(int argc, char **argv)
{
    // verificar se as pastas existem, se não existirem, criar
    filesystem::path dircarros = "dados/carros";
    filesystem::path dirutlizadores = "dados/utilizadores";
    filesystem::create_directory(dircarros);
    filesystem::create_directory(dirutlizadores);

    // verificar se os ficheiros existem, se não existirem, criar
    if (!ficheirocheck("dados/utilizadores/listautilizadores.txt"))
    {
        ofstream ficheiro("dados/utilizadores/listautilizadores.txt");
    }

    if (!ficheirocheck("dados/carros/listacarros.txt"))
    {
        ofstream ficheiro("dados/carros/listacarros.txt");
    }

    // criar a aplicação
    auto app = Application::create(argc, argv, "org.autocarrinhos.esas");
    auto settings = Settings::get_default();

    // carregar o css
    Glib::RefPtr<CssProvider> cssProvider = CssProvider::create();

    if (!cssProvider->load_from_path("theme.css"))
    {
        cerr << "CSS Falhou a abrir!.\n";
    }
    else
    {
        Glib::RefPtr<Gdk::Screen> screen = Gdk::Screen::get_default();
        StyleContext::add_provider_for_screen(screen, cssProvider, GTK_STYLE_PROVIDER_PRIORITY_USER);
    }

    // definir o tema escuro
    settings->property_gtk_application_prefer_dark_theme() = true;

    // criar a janela
    Window appWindow;
    appWindow.set_title("Autocarrinhos ESAS");
    appWindow.set_default_size(1200, 700);
    // appWindow.set_position(WindowPosition::WIN_POS_CENTER);
    appWindow.set_resizable(true);

    try
    {
        appWindow.set_icon_from_file("assets/logocar-small.ico");
    }
    catch (const Glib::FileError &ex)
    {
        cerr << "Erro: " << ex.what() << endl;
    }

    // criar o stack
    Stack stack;
    stack.set_transition_type(StackTransitionType::STACK_TRANSITION_TYPE_OVER_DOWN_UP);
    stack.set_transition_duration(500);

    // os #if e #endif é pra organizar o codigo, o 1 ativa e o 0 desativa a parte do codigo! MANTER NO 1 PARA A FUNCIONAR CORRETAMENTE!

#if 1 // Log in
    Box loginBox;
    loginBox.set_spacing(15);
    loginBox.set_orientation(Orientation::ORIENTATION_VERTICAL);
    loginBox.set_homogeneous(false);

    Image logo("assets/logo-med.png");

    Label title("Entre na sua conta"), erro("");
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

#if 1 // registrar
    Box registrarBox;
    registrarBox.set_spacing(15);
    registrarBox.set_orientation(Orientation::ORIENTATION_VERTICAL);
    registrarBox.set_homogeneous(false);

    Image logoreg("assets/logo-med.png");

    Label titlereg("Crie a sua conta"), erroreg("");
    titlereg.set_name("title");
    erroreg.set_name("erro");

    Entry usernamereg, passwordreg, passwordregconfirm;
    Button registerButton2("Registar"), cancelarreg("Cancelar");

    usernamereg.set_size_request(70, 50);
    passwordreg.set_size_request(70, 50);
    passwordregconfirm.set_size_request(70, 50);
    registerButton2.set_size_request(70, 50);
    cancelarreg.set_size_request(70, 50);
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
    registrarBox.pack_start(cancelarreg, false, false, 0);
    stack.add(registrarBox, "registrar");
#endif

#if 1 // Dashboardutilizador
    Box DashboardUser, TopBarUser;
    Button procurarbuttonCarros("Procurar Carros"), historicobutton("Histórico de transações"), defbutton("Definições de conta"), Suportebutton("Suporte ao cliente"), logoutbuttonUser("Sair");
    Stack contentStackUser;
    Label dashboardLabelUser("Bem-vindo, admin");
    Image logoUser("assets/logocar-xsmall.png");

    Box procurarCarrosBox, historicobox, defbox, Suportebox;

    contentStackUser.add(procurarCarrosBox, "procurarCarros");
    contentStackUser.add(historicobox, "historicobox");
    contentStackUser.add(defbox, "defbox");
    contentStackUser.add(Suportebox, "Suportebox");

    DashboardUser.set_orientation(Orientation::ORIENTATION_VERTICAL);
    TopBarUser.set_orientation(Orientation::ORIENTATION_HORIZONTAL);

    TopBarUser.pack_start(logoUser, PACK_SHRINK, 50);
    TopBarUser.pack_start(procurarbuttonCarros, PACK_SHRINK, 10);
    TopBarUser.pack_start(historicobutton, PACK_SHRINK, 10);
    TopBarUser.pack_start(defbutton, PACK_SHRINK, 10);
    TopBarUser.pack_start(Suportebutton, PACK_SHRINK, 10);
    TopBarUser.pack_start(logoutbuttonUser, PACK_SHRINK, 10);

    procurarbuttonCarros.signal_clicked().connect([&contentStackUser]
                                                  { contentStackUser.set_visible_child("procurarCarros"); });

    historicobutton.signal_clicked().connect([&contentStackUser]
                                             { contentStackUser.set_visible_child("historicobox"); });

    defbutton.signal_clicked().connect([&contentStackUser]
                                       { contentStackUser.set_visible_child("defbox"); });

    Suportebutton.signal_clicked().connect([&contentStackUser]
                                           { contentStackUser.set_visible_child("Suportebox"); });

    logoutbuttonUser.signal_clicked().connect([&stack]
                                              { stack.set_visible_child("login"); });

    DashboardUser.pack_start(TopBarUser, PACK_SHRINK);
    DashboardUser.pack_start(contentStackUser);
    DashboardUser.pack_start(dashboardLabelUser, PACK_SHRINK);

    stack.add(DashboardUser, "dashboarduser");
#endif

#if 1 // butoes
    loginButton.signal_clicked().connect([&stack, &usernameEntry, &passwordEntry, &dashboardLabelUser, &erro]
                                         {
        
        string temppath = "dados/utilizadores/" + usernameEntry.get_text(); 

        if(pathExists(temppath)){
            
        }else{
            erro.set_text("Nome de utilizador/Palavra Passe errados");
        }

        if(usernameEntry.get_text() == "admin" && passwordEntry.get_text() == "admin") {
            dashboardLabelUser.set_text("Autenticado como: " + usernameEntry.get_text() + ", bem-vindo!");
            stack.set_visible_child("dashboarduser");

            erro.set_text("");
            usernameEntry.set_text("");
            passwordEntry.set_text("");
        } else {
            erro.set_text("Utilizador ou palavra-passe incorretos");
        } });

    registerButton.signal_clicked().connect([&stack, &erro, &usernameEntry, &passwordEntry]
                                            {
        erro.set_text("");
        usernameEntry.set_text("");
        passwordEntry.set_text("");
        stack.set_visible_child("registrar"); });

    registerButton2.signal_clicked().connect([&stack, &usernamereg, &passwordreg, &passwordregconfirm, &erroreg, &usernameEntry, &passwordEntry]
                                             {

        if(passwordreg.get_text() == passwordregconfirm.get_text() && usernamereg.get_text() != "" && passwordreg.get_text() != "" && passwordregconfirm.get_text() != ""){
            erroreg.set_text("");

            string temppath = "dados/utilizadores/" + usernamereg.get_text();

            if(!pathExists(temppath)){

                criarutilizador(usernamereg.get_text(), passwordreg.get_text());

                //para fazer login automatico
                usernameEntry.set_text(usernamereg.get_text());
                passwordEntry.set_text(passwordreg.get_text());

                usernamereg.set_text("");
                passwordreg.set_text("");
                passwordregconfirm.set_text("");
                stack.set_visible_child("login");
            }
            else{
                erroreg.set_text("O nome de utilizador já existe");
            }
    
        } else {
            if(passwordreg.get_text() != passwordregconfirm.get_text()){
                erroreg.set_text("As palavras-passe não coincidem");
            }
            else if(usernamereg.get_text() == ""){
                erroreg.set_text("O nome de utilizador está vazio");
            }
            else if(passwordreg.get_text() == ""){
                erroreg.set_text("A palavra-passe está vazia");
            }
            else if(passwordregconfirm.get_text() == ""){
                erroreg.set_text("A confirmação da palavra-passe está vazia");
            }
            else
            erroreg.set_text("Erro no formulario de registo");
        } });

    cancelarreg.signal_clicked().connect([&stack, &erroreg, &usernamereg, &passwordreg, &passwordregconfirm]
                                         {
        erroreg.set_text("");
        usernamereg.set_text("");
        passwordreg.set_text("");
        passwordregconfirm.set_text("");
        stack.set_visible_child("login"); });
#endif

    // alinhamento
    Alignment *align = manage(new Alignment(0.5, 0.5, 0, 0));
    align->add(stack);
    appWindow.add(*align);
    appWindow.show_all();

    // executar a aplicação
    return app->run(appWindow);
}