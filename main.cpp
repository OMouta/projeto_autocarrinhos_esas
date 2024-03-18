#include <gtkmm.h>
#include <glibmm.h>
#include <windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

using namespace Gtk;
using namespace std;

struct carros {
    string marca, modelo, cor, combustivel, estado, imagempath;
    int ano, preco;
} c[1000];

int numerodecarrosatual = 0;
string utilizadoratual = "";

void mostrardebug(const char* msg) {
    MessageBoxA(NULL, msg, "DEBUG", MB_ICONASTERISK | MB_OK);
}

bool pathExists(const filesystem::path p) //verificar se o caminho existe
{
    return filesystem::exists(p);
}

void loadcarrosstruct()
{
    // caminho para os carros
    filesystem::path dircarros = "dados/carros";

    // resetar a struct
    for (int i = 0; i < 1000; i++)
    {
        c[i].marca = "";
        c[i].modelo = "";
        c[i].cor = "";
        c[i].combustivel = "";
        c[i].estado = "";
        c[i].ano = 0;
        c[i].preco = 0;
    }

    // resetar o numero de carros
    numerodecarrosatual = 0;

    // percorrer a pasta dos carros
    for (const auto & entry : filesystem::directory_iterator(dircarros)) 
    {
        // verificar se é uma pasta
        if (entry.is_directory()) 
        {
            // aumentar o numero de carros
            numerodecarrosatual++;

            // abrir o ficheiro de informação
            ifstream info(entry.path() / "info.txt");
            string linha;

            // percorrer o ficheiro de informação e adicionar a informação à struct
            while(getline(info, linha))
            {
                if (linha.find("Marca: ") != string::npos)
                {
                    c[numerodecarrosatual - 1].marca = linha.substr(linha.find("Marca: ") + 7);
                }
                
                if (linha.find("Modelo: ") != string::npos)
                {
                    c[numerodecarrosatual - 1].modelo = linha.substr(linha.find("Modelo: ") + 8);
                }
                
                if (linha.find("Cor: ") != string::npos)
                {
                    c[numerodecarrosatual - 1].cor = linha.substr(linha.find("Cor: ") + 5);
                }
                
                if (linha.find("Ano: ") != string::npos)
                {
                    c[numerodecarrosatual - 1].ano = stoi(linha.substr(linha.find("Ano: ") + 5));
                }
                
                if (linha.find("Estado: ") != string::npos)
                {
                    c[numerodecarrosatual - 1].estado = linha.substr(linha.find("Estado: ") + 8);
                }
                if (linha.find("Combustivel: ") != string::npos)
                {
                    c[numerodecarrosatual - 1].combustivel = linha.substr(linha.find("Combustivel: ") + 13);
                }
                
                if (linha.find("Preco: ") != string::npos)
                {
                    c[numerodecarrosatual - 1].preco = stoi(linha.substr(linha.find("Preco: ") + 7));
                }

                if (linha.find("Imagem: ") != string::npos)
                {
                    c[numerodecarrosatual - 1].imagempath = linha.substr(linha.find("Imagem: ") + 8);
                }
            }
        }
    }    
}

void mostrarcarros(Box& CarrosBox, string FiltroMarca, string FiltroModelo, string FiltroCor, int FiltroAno, int FiltroPrecoMin, int FiltroPrecoMax, bool FiltroUsado, bool FiltroNovo, bool FilroEletrico, bool FiltroGasolina, bool FiltroDiesel, bool FiltroHibrido, bool eadmin)
{
    // limpar a box
    vector<Widget*> carros = CarrosBox.get_children();
    for(auto carro: carros)
    {
        CarrosBox.remove(*carro);
        delete carro;
    }

    // percorrer a struct e adicionar os carros à box
    for (int i = 0; i < numerodecarrosatual; i++)
    {

        // verificar se o carro corresponde aos filtros

        if (FiltroMarca != "" && c[i].marca != FiltroMarca)
        {
            continue;
        }

        if (FiltroModelo != "" && c[i].modelo != FiltroModelo)
        {
            continue;
        }

        if (FiltroCor != "" && c[i].cor != FiltroCor)
        {
            continue;
        }

        if (FiltroAno != 0 && c[i].ano != FiltroAno)
        {
            continue;
        }

        if (FiltroPrecoMin != 0 && c[i].preco < FiltroPrecoMin)
        {
            continue;
        }

        if (FiltroPrecoMax != 0 && c[i].preco > FiltroPrecoMax)
        {
            continue;
        }

        if (FiltroUsado == true && c[i].estado != "Usado")
        {
            continue;
        }

        if (FiltroNovo == true && c[i].estado != "Novo")
        {
            continue;
        }

        if (FilroEletrico == true && c[i].combustivel != "Eletrico")
        {
            continue;
        }

        if (FiltroGasolina == true && c[i].combustivel != "Gasolina")
        {
            continue;
        }

        if (FiltroDiesel == true && c[i].combustivel != "Diesel")
        {
            continue;
        }

        if (FiltroHibrido == true && c[i].combustivel != "Hibrido")
        {
            continue;
        }

        // criar a box do carro
        Box* carro = new Box(Orientation::ORIENTATION_HORIZONTAL);
        carro->set_spacing(4);

        Box* informacao = new Box(Orientation::ORIENTATION_VERTICAL);

        // criar as labels do carro
        Label* ano_marca_modelo_cor = new Label(to_string(c[i].ano) + " " + c[i].marca + " " + c[i].modelo + " " + c[i].cor);
        Label* combustivel_estado = new Label("Combustivel: " + c[i].combustivel + " | Estado: " + c[i].estado);
        Label* preco = new Label(to_string(c[i].preco) + "€");

        Glib::RefPtr<Gdk::Pixbuf> pixbuf2;
        Glib::RefPtr<Gdk::Pixbuf> novaimagem2;

        try {
            pixbuf2 = Gdk::Pixbuf::create_from_file(c[i].imagempath);
            novaimagem2 = pixbuf2->scale_simple(200, 150, Gdk::INTERP_BILINEAR);
        } catch (const Glib::FileError& ex) {
            cerr << "Erro: " << ex.what() << endl;
        } catch (const Gdk::PixbufError& ex) {
            cerr << "Erro: " << ex.what() << endl;
        }

        if (!novaimagem2) {
            cerr << "Sem imagem: " << c[i].imagempath << endl;
            continue;
        }

        Image* imagem = manage(new Image());
        imagem->set(novaimagem2);

        Button* editarbutton = new Button("Editar");
        Button* removerbutton = new Button("Remover");
        Button* comprarbutton = new Button("Comprar");

        Label* erro = new Label("");

        // adicionar as labels à box do carro
        informacao->pack_start(*ano_marca_modelo_cor, PACK_SHRINK);
        informacao->pack_start(*combustivel_estado, PACK_SHRINK);
        informacao->pack_start(*preco, PACK_SHRINK);
        informacao->pack_start(*erro, PACK_SHRINK);

        if(eadmin == true)
        {
            informacao->pack_start(*editarbutton, PACK_SHRINK);
            informacao->pack_start(*removerbutton, PACK_SHRINK);
        }
        else
        {
            informacao->pack_start(*comprarbutton, PACK_SHRINK);
        }

        editarbutton->signal_clicked().connect([i]{
            mostrardebug("Editar carro " + i);
        });

        removerbutton->signal_clicked().connect([i, &CarrosBox]{
           filesystem::path deletarPath = "dados/carros/" + c[i].marca + c[i].modelo + to_string(c[i].ano);

           try {
             filesystem::remove_all(deletarPath);
           } catch(filesystem::filesystem_error& e) {
            mostrardebug(e.what());
           }

            loadcarrosstruct();
            mostrarcarros(CarrosBox, "", "", "", 0, 0, 0, false, false, false, false, false, false, true);
        });

        comprarbutton->signal_clicked().connect([i]{
            mostrardebug("Comprar carro " + i);
        });

        //imagem do carro
        carro->pack_start(*imagem, PACK_SHRINK);
        carro->pack_start(*informacao, PACK_SHRINK);

        //css
        ano_marca_modelo_cor->set_name("Carros");
        combustivel_estado->set_name("Carros");
        preco->set_name("Carros");
        carro->set_name("Carros");
        informacao->set_name("Carros");
        erro->set_name("erro");
        informacao->set_halign(Align::ALIGN_START);
        informacao->set_valign(Align::ALIGN_CENTER);
        informacao->set_hexpand(false);
        informacao->set_vexpand(false);
        informacao->set_hexpand_set(false);
        informacao->set_vexpand_set(false);
        informacao->set_spacing(5);
        carro->set_size_request(400, 150);
        carro->set_halign(Align::ALIGN_START);
        carro->set_valign(Align::ALIGN_CENTER);
        carro->set_hexpand(false);
        carro->set_vexpand(false);
        carro->set_hexpand_set(false);
        carro->set_vexpand_set(false);

        // adicionar a box do carro à box dos carros
        CarrosBox.pack_start(*carro);
        CarrosBox.show_all();
    }
}

void criarutilizador(string nomeutilizador, string palavraPasse)
{
    //criar a pasta do utilizador e o ficheiro de informação
    filesystem::create_directory("dados/utilizadores/" + nomeutilizador);
    ofstream ficheiro("dados/utilizadores/" + nomeutilizador + "/info.txt");

    //escrever a informação no ficheiro
    ficheiro << "Nome de utilizador: " << nomeutilizador << endl;
    ficheiro << "Palavra-passe: " << palavraPasse << endl;
    ficheiro << "Email: " << endl;
    ficheiro << "Contacto: " << endl;
    ficheiro << "Nome: " << endl;
    ficheiro << "Morada: " << endl;
    ficheiro << "NIF: " << endl;
    ficheiro << "Numero do cartão de crédito: " << endl;
    ficheiro << "CVV: " << endl;

    //fechar o ficheiro
    ficheiro.close();
}

void criarcarro(string marca, string modelo, string cor, string combustivel, string estado, string ano, string preco, string imagempath)
{
    //criar a pasta do carro e o ficheiro de informação
    filesystem::create_directory("dados/carros/" + marca + modelo + ano);
    ofstream ficheiro("dados/carros/" + marca + modelo + ano + "/info.txt");

    //adiciona a imagem à pasta do carro
    filesystem::path sourcePath = imagempath;
    filesystem::path imagemdestinationPath = "dados/carros/" + marca + modelo + ano + "/" + sourcePath.filename().string();
    filesystem::copy(sourcePath, imagemdestinationPath, filesystem::copy_options::overwrite_existing);

    //escrever a informação no ficheiro
    ficheiro << "Marca: " << marca << endl;
    ficheiro << "Modelo: " << modelo << endl;
    ficheiro << "Cor: " << cor << endl;
    ficheiro << "Combustivel: " << combustivel << endl;
    ficheiro << "Estado: " << estado << endl;
    ficheiro << "Ano: " << ano << endl;
    ficheiro << "Preco: " << preco << endl;
    ficheiro << "Imagem: " << "dados/carros/" + marca + modelo + ano + "/" + sourcePath.filename().string() << endl;

    //fechar o ficheiro e atualizar os carros
    ficheiro.close();

    //atualizar os carros
    loadcarrosstruct();
}

bool temcaracterespecial(string nome) //nao esta a funcionar corretamente
{
    for (char c : nome) // percorre a string letra a letra
    {
        if (c == '!' || c == '@' || c == '#' || c == '$' || c == '%' || c == '^' || c == '&' || c == '*' || c == '(' || c == ')' || c == '-' || c == '_' || c == '+' || c == '=' || c == '{' || c == '}' || c == '[' || c == ']' || c == ':' || c == ';' || c == '"' || c == '\'' || c == '<' || c == '>' || c == ',' || c == '.' || c == '?' || c == '/' || c == '|' || c == '\\' || c == '`' || c == '~' || c == ' ')
        {
            // se encontrar um caracter especial, retorna verdadeiro
            return true;
        }
    }

    // se nao encontrar nenhum caracter especial, retorna falso
    return false;
}

int main(int argc, char **argv)
{

// os #if e #endif é pra organizar o codigo, o 1 ativa e o 0 desativa a parte do codigo! MANTER NO 1 PARA A FUNCIONAR CORRETAMENTE!

#if 1 // Inicialização
    // verificar se as pastas existem, se não existirem, criar
    filesystem::path dircarros = "dados/carros";
    filesystem::path dirutlizadores = "dados/utilizadores";
    filesystem::create_directory(dircarros);
    filesystem::create_directory(dirutlizadores);

    // carregar os carros para a struct
    loadcarrosstruct();

    // criar a aplicação
    Main kit(argc, argv);
    auto settings = Settings::get_default();

    // carregar o css
    Glib::RefPtr<CssProvider> cssProvider = CssProvider::create();

    // verificar se o css foi carregado corretamente
    if (!cssProvider->load_from_path("theme.css"))
    {
        return 1;
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
    appWindow.set_default_size(800, 600);

    // centrar a janela no ecrã quando abre
    appWindow.set_position(WindowPosition::WIN_POS_CENTER);

    // permitir redimensionar a janela
    appWindow.set_resizable(true);

    // definir o tamanho minimo e maximo da janela
    appWindow.set_hexpand(true);
    appWindow.set_vexpand(true);

    // tentar carregar o logo
    try
    {
        appWindow.set_icon_from_file("assets/logocar-small.ico");
    }
    catch (const Gio::ResourceError &ex)
    {
        mostrardebug("Erro ao carregar o logo");
    }

    // criar o stack
    Stack stack;
    stack.set_transition_type(StackTransitionType::STACK_TRANSITION_TYPE_CROSSFADE);
    stack.set_transition_duration(300);
#endif

#if 1 // Log in
    Box loginBox;
    ScrolledWindow LoginScroll;
    loginBox.set_spacing(15);
    loginBox.set_orientation(Orientation::ORIENTATION_VERTICAL);

    // Scroll para a box
    LoginScroll.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

    // Logo, titulo, erro, entradas, botões
    Image logo("assets/logocar-small.png");

    Label title("Entre na sua conta"), erro("");
    title.set_name("title");
    erro.set_name("erro");

    Entry usernameEntry, passwordEntry;
    Button loginButton("Entrar"), registerButton("Registrar");

    // Placeholders
    usernameEntry.set_placeholder_text("Utilizador");
    passwordEntry.set_placeholder_text("Palavra-passe");
    passwordEntry.set_visibility(false);

    // Adicionar os elementos a box
    loginBox.pack_start(logo, PACK_SHRINK, 0);
    loginBox.pack_start(title, PACK_SHRINK, 0);
    loginBox.pack_start(usernameEntry, PACK_SHRINK, 0);
    loginBox.pack_start(passwordEntry, PACK_SHRINK, 0);
    loginBox.pack_start(erro, PACK_SHRINK, 0);
    loginBox.pack_start(loginButton, PACK_SHRINK, 0);
    loginBox.pack_start(registerButton, PACK_SHRINK, 0);

    // Adicionar a box ao scroll
    LoginScroll.add(loginBox);
    LoginScroll.set_margin_bottom(10);

    // Adicionar a box ao stack
    stack.add(LoginScroll, "login");
#endif

#if 1 // Registrar
    Box registrarBox;
    ScrolledWindow registrarScroll;
    registrarBox.set_spacing(15);
    registrarBox.set_orientation(Orientation::ORIENTATION_VERTICAL);

    // Scroll para a box
    registrarScroll.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

    // Logo, titulo, erro, entradas, botões
    Image logoreg("assets/logocar-small.png");

    Label titlereg("Crie a sua conta"), erroreg("");
    titlereg.set_name("title");
    erroreg.set_name("erro");

    Entry usernamereg, passwordreg, passwordregconfirm;
    Button registerButton2("Registar"), cancelarreg("Cancelar");

    // Placeholders
    usernamereg.set_placeholder_text("Nome de utilizador");
    passwordreg.set_placeholder_text("Palavra-passe");
    passwordreg.set_visibility(false);
    passwordregconfirm.set_placeholder_text("Confirme a palavra-passe");
    passwordregconfirm.set_visibility(false);

    // Adicionar os elementos a box
    registrarBox.pack_start(logoreg, false, false, 0);
    registrarBox.pack_start(titlereg, false, false, 0);
    registrarBox.pack_start(usernamereg, false, false, 0);
    registrarBox.pack_start(passwordreg, false, false, 0);
    registrarBox.pack_start(passwordregconfirm, false, false, 0);
    registrarBox.pack_start(erroreg, false, false, 0);
    registrarBox.pack_start(registerButton2, false, false, 0);
    registrarBox.pack_start(cancelarreg, false, false, 0);

    // Adicionar a box ao scroll
    registrarScroll.add(registrarBox);
    registrarScroll.set_margin_bottom(50);

    // Adicionar a box ao stack
    stack.add(registrarScroll, "registrar");
#endif

#if 1 //Dashboard Admin
    Box DashboardAdmin, TopBarAdmin;
    Button editarbuttonCarros("Gerenciar Carros"), criarbuttonCarros("Criar carro"), logoutbuttonadmin("Sair");
    Stack contentStackAdmin;
    Label dashboardLabelAdmin("Bem-vindo, admin");
    Image logoAdmin("assets/logotext-xsmall.png");

    Box editarCarrosBox, criarCarrosBox;

    //Adicionar as paginas para editar, criar e remover carros
    contentStackAdmin.add(criarCarrosBox, "criarBox");
    contentStackAdmin.add(editarCarrosBox, "editarBox");

    DashboardAdmin.set_orientation(Orientation::ORIENTATION_VERTICAL);
    TopBarAdmin.set_orientation(Orientation::ORIENTATION_HORIZONTAL);
    
    TopBarAdmin.pack_start(logoAdmin, PACK_SHRINK, 50);
    TopBarAdmin.pack_start(criarbuttonCarros, PACK_SHRINK, 10);
    TopBarAdmin.pack_start(editarbuttonCarros, PACK_SHRINK, 10);
    TopBarAdmin.pack_start(logoutbuttonadmin, PACK_SHRINK, 10);

    TopBarAdmin.set_center_widget(dashboardLabelAdmin);

    //Criar os espaços para escrever as informações do carro
    Entry criarCarroMarca, criarCarroModelo, criarCarroCor, criarCarroCombustivel,criarCarroEstado, criarCarroAno, criarCarroPreco;
    Button criarCarroConfirm("Confirmar"), criarCarroCancel("Cancelar");
    Label criarCarroLabel("Criar Carro"), criarCarroErro("");
    FileChooserButton criarCarroImagem("Escolher imagem", FILE_CHOOSER_ACTION_OPEN);
    string imagempath;
    Image criarCarroImagemPreview;

    Glib::RefPtr<FileFilter> filtroparaimagens = FileFilter::create();
    filtroparaimagens->set_name("Imagens");
    filtroparaimagens->add_pattern("*.png");
    filtroparaimagens->add_pattern("*.jpeg");
    filtroparaimagens->add_pattern("*.jpg");
    criarCarroImagem.add_filter(filtroparaimagens);

    criarCarroErro.set_name("erro");

    criarCarroMarca.set_placeholder_text("Marca do carro");
    criarCarroModelo.set_placeholder_text("Modelo do carro");
    criarCarroCor.set_placeholder_text("Cor do carro");
    criarCarroCombustivel.set_placeholder_text("Tipo de combustivel");
    criarCarroAno.set_placeholder_text("Ano do carro");
    criarCarroPreco.set_placeholder_text("Preco do carro");
    criarCarroEstado.set_placeholder_text("Estado do carro");

    criarCarrosBox.set_orientation(Orientation::ORIENTATION_VERTICAL);

    criarCarrosBox.pack_start(criarCarroLabel, PACK_SHRINK, 5);
    criarCarrosBox.pack_start(criarCarroMarca, PACK_SHRINK, 5);
    criarCarrosBox.pack_start(criarCarroModelo, PACK_SHRINK, 5);
    criarCarrosBox.pack_start(criarCarroCor, PACK_SHRINK, 5);
    criarCarrosBox.pack_start(criarCarroCombustivel, PACK_SHRINK, 5);
    criarCarrosBox.pack_start(criarCarroEstado, PACK_SHRINK, 5);
    criarCarrosBox.pack_start(criarCarroAno, PACK_SHRINK, 5);
    criarCarrosBox.pack_start(criarCarroPreco, PACK_SHRINK, 5);
    criarCarrosBox.pack_start(criarCarroImagem, PACK_SHRINK, 5);
    criarCarrosBox.pack_start(criarCarroImagemPreview, PACK_SHRINK, 5);
    criarCarrosBox.pack_start(criarCarroErro, PACK_SHRINK, 5);
    criarCarrosBox.pack_start(criarCarroConfirm, PACK_SHRINK, 5);
    criarCarrosBox.pack_start(criarCarroCancel, PACK_SHRINK, 5);

    criarCarroImagem.signal_file_set().connect([&criarCarroImagem, &imagempath, &criarCarroImagemPreview]() {
        imagempath = criarCarroImagem.get_filename();

        //carregar a imagem e mostrar no preview
        auto pixbuf = Gdk::Pixbuf::create_from_file(imagempath);
        auto novaimagem = pixbuf->scale_simple(100, 50, Gdk::INTERP_BILINEAR);
        criarCarroImagemPreview.set(novaimagem);
    });

    criarCarroConfirm.signal_clicked().connect([&criarCarroMarca, &criarCarroModelo, &criarCarroCor, &criarCarroCombustivel, &criarCarroEstado, &criarCarroAno, &criarCarroPreco, &criarCarroErro, &criarCarroImagemPreview, &imagempath, &criarCarroImagem]
    {
        bool tem = false;

        //verificar se os campos estão preenchidos
        if(criarCarroMarca.get_text() == ""){
            tem = true;
        }
        if(criarCarroModelo.get_text() == ""){
            tem = true;
        }
        if(criarCarroCor.get_text() == ""){
            tem = true;
        }
        if(criarCarroCombustivel.get_text() == ""){
            tem = true;
        }
        if(criarCarroEstado.get_text() == ""){
            tem = true;
        }
        if(criarCarroAno.get_text() == ""){
            tem = true;
        }
        if(criarCarroPreco.get_text() == ""){
            tem = true;
        }
        if(imagempath == ""){
            tem = true;
        }

        //se nao estiver preenchido, mostrar erro
        if(tem == true){
            criarCarroErro.set_text("Preencha todos os campos!");
        }
        else
        {
            //chama a função para criar o carro
            criarcarro(criarCarroMarca.get_text(), criarCarroModelo.get_text(), criarCarroCor.get_text(), criarCarroCombustivel.get_text(), criarCarroEstado.get_text(), criarCarroAno.get_text(), criarCarroPreco.get_text(), imagempath);
            
            //resetar os campos
            criarCarroMarca.set_text("");
            criarCarroModelo.set_text("");
            criarCarroCor.set_text("");
            criarCarroCombustivel.set_text("");
            criarCarroEstado.set_text("");
            criarCarroAno.set_text("");
            criarCarroPreco.set_text("");
            criarCarroErro.set_text("");
            criarCarroImagemPreview.set("");
            imagempath = "";
            criarCarroImagem.unselect_all();

            loadcarrosstruct();
        }
    });

    criarCarroCancel.signal_clicked().connect([&criarCarroMarca, &criarCarroModelo, &criarCarroCor, &criarCarroCombustivel, &criarCarroEstado, &criarCarroAno, &criarCarroPreco, &criarCarroErro, &criarCarroImagemPreview, &imagempath, &criarCarroImagem]
    {
        criarCarroMarca.set_text("");
        criarCarroModelo.set_text("");
        criarCarroCor.set_text("");
        criarCarroCombustivel.set_text("");
        criarCarroEstado.set_text("");
        criarCarroAno.set_text("");
        criarCarroPreco.set_text("");
        criarCarroErro.set_text("");
        criarCarroImagemPreview.set("");
        imagempath = "";
        criarCarroImagem.unselect_all();
    });

    ScrolledWindow adminCarrosScrolledWindow;
    Box CarrosBoxadmin;

    CarrosBoxadmin.set_orientation(Orientation::ORIENTATION_VERTICAL);
    CarrosBoxadmin.set_spacing(10);
    CarrosBoxadmin.set_hexpand(true);
    CarrosBoxadmin.set_hexpand_set(true);

    adminCarrosScrolledWindow.set_policy(POLICY_NEVER, POLICY_AUTOMATIC);
    adminCarrosScrolledWindow.add(CarrosBoxadmin);
    adminCarrosScrolledWindow.set_hexpand(true);
    adminCarrosScrolledWindow.set_hexpand_set(true);

    mostrarcarros(CarrosBoxadmin, "", "", "", 0, 0, 0, false, false, false, false, false, false, true);

    editarCarrosBox.pack_start(adminCarrosScrolledWindow, PACK_SHRINK, 5);

    //Mudar para a pagina de editar os carros
    editarbuttonCarros.signal_clicked().connect([&contentStackAdmin, &CarrosBoxadmin]{ 
        contentStackAdmin.set_visible_child("editarBox");
        mostrarcarros(CarrosBoxadmin, "", "", "", 0, 0, 0, false, false, false, false, false, false, true);
    });
    
    //Mudar para a pagina de criar os carros
    criarbuttonCarros.signal_clicked().connect([&contentStackAdmin]{ 
        contentStackAdmin.set_visible_child("criarBox");
    });

    //Espaçamento entre butões
    DashboardAdmin.set_spacing(60);

    //adicionar barra para criar, editar e remover carros a dashboard do admin
    DashboardAdmin.pack_start(TopBarAdmin, PACK_SHRINK);
    DashboardAdmin.pack_start(contentStackAdmin);

    //adicionar a dashboard ao stack
    stack.add(DashboardAdmin, "dashboardadmin");
#endif

#if 1 // Dashboard Utilizador
    Box DashboardUser, TopBarUser;
    Button procurarbuttonCarros("Procurar Carros"), historicobutton("Histórico de transações"), defbutton("Definições de conta"), logoutbuttonuser("Sair");
    Stack contentStackUser;
    Label dashboardLabelUser("Bem-vindo, admin");
    Image logoUser("assets/logocar-xsmall.png");

    Box procurarCarrosBox, historicobox, defbox;

    //adicionar as paginas de procura, historico, definições e suporte ao stack da dashboard
    contentStackUser.add(procurarCarrosBox, "procurarCarros");
    contentStackUser.add(historicobox, "historicobox");
    contentStackUser.add(defbox, "defbox");

    //orientação dos boxes
    DashboardUser.set_orientation(Orientation::ORIENTATION_VERTICAL);
    TopBarUser.set_orientation(Orientation::ORIENTATION_HORIZONTAL);
    defbox.set_orientation(Orientation::ORIENTATION_VERTICAL);

    TopBarUser.set_center_widget(dashboardLabelUser);
    TopBarUser.set_spacing(10);

    //adicionar os butoes a barra de cima
    TopBarUser.pack_start(logoUser, PACK_SHRINK, 25);
    TopBarUser.pack_start(procurarbuttonCarros, PACK_SHRINK);
    TopBarUser.pack_start(historicobutton, PACK_SHRINK);
    TopBarUser.pack_start(defbutton, PACK_SHRINK);
    TopBarUser.pack_start(logoutbuttonuser, PACK_SHRINK);
    TopBarUser.pack_start(dashboardLabelUser, PACK_SHRINK, 25);

    //BOX PROCURAR CARROS
    ScrolledWindow procurarCarrosScrolledWindow;
    Box FiltrosBar, CarrosBox, gridCarros;
    Label FiltrosLabel("Filtros"), FiltroAnoLabel("Ano"), FiltroPrecoMinLabel("Preço Min"), FiltroPrecoMaxLabel("Preço Max"), FiltroOutrosLabel("Outros");
    Entry FiltroMarca, FiltroModelo, FiltroCor;
    CheckButton FiltroUsado("Usado"), FiltroNovo("Novo"), FilroEletrico("Eletrico"), FiltroGasolina("Gasolina"), FiltroDiesel("Diesel"), FiltroHibrido("Hibrido");
    SpinButton FiltroAno, FiltroPrecoMin, FiltroPrecoMax;

    gridCarros.set_homogeneous(true);
    CarrosBox.set_hexpand(true);
    CarrosBox.set_hexpand_set(true);

    //Nome das classes para o css
    FiltrosBar.set_name("Filtros");
    FiltrosLabel.set_name("Filtros");
    FiltroAnoLabel.set_name("Filtros");
    FiltroPrecoMinLabel.set_name("Filtros");
    FiltroPrecoMaxLabel.set_name("Filtros");
    FiltroOutrosLabel.set_name("Filtros");
    FiltroUsado.set_name("Filtros");
    FiltroNovo.set_name("Filtros");
    FilroEletrico.set_name("Filtros");
    FiltroGasolina.set_name("Filtros");
    FiltroDiesel.set_name("Filtros");
    FiltroHibrido.set_name("Filtros");
    FiltroAno.set_name("Filtros");
    FiltroPrecoMin.set_name("Filtros");
    FiltroPrecoMax.set_name("Filtros");
    FiltroUsado.set_name("Filtros");
    FiltroNovo.set_name("Filtros");
    FilroEletrico.set_name("Filtros");
    FiltroGasolina.set_name("Filtros");
    FiltroDiesel.set_name("Filtros");
    FiltroHibrido.set_name("Filtros");

    //Place holder para  filtros
    FiltroMarca.set_placeholder_text("Marca");
    FiltroModelo.set_placeholder_text("Modelo");
    FiltroCor.set_placeholder_text("Cor");

    //Range dos filtros
    FiltroAno.set_range(0, 2025);
    FiltroPrecoMax.set_range(0, 1000000);
    FiltroPrecoMin.set_range(0, 1000000);

    //Incrementos dos filtros
    FiltroAno.set_increments(1, 10);
    FiltroPrecoMax.set_increments(100, 1000);
    FiltroPrecoMin.set_increments(100, 1000);

    //Valores iniciais dos filtros
    FiltroAno.set_value(0);
    FiltroPrecoMax.set_value(0);
    FiltroPrecoMin.set_value(0);

    //Orientacao dos filtros
    FiltrosBar.set_orientation(Orientation::ORIENTATION_VERTICAL);

    //Orientacao dos carros
    CarrosBox.set_orientation(Orientation::ORIENTATION_VERTICAL);
    CarrosBox.set_spacing(10);
    CarrosBox.set_hexpand(true);
    CarrosBox.set_hexpand_set(true);

    //Scroll para os carros
    procurarCarrosScrolledWindow.set_policy(POLICY_NEVER, POLICY_AUTOMATIC);
    procurarCarrosScrolledWindow.add(CarrosBox);
    procurarCarrosScrolledWindow.set_hexpand(true);
    procurarCarrosScrolledWindow.set_hexpand_set(true);

    //Adicionar os filtros
    FiltrosBar.pack_start(FiltrosLabel, PACK_SHRINK, 5);
    FiltrosBar.pack_start(FiltroMarca, PACK_SHRINK, 5);
    FiltrosBar.pack_start(FiltroModelo, PACK_SHRINK), 5;
    FiltrosBar.pack_start(FiltroCor, PACK_SHRINK, 5);
    FiltrosBar.pack_start(FiltroAnoLabel, PACK_SHRINK, 5);
    FiltrosBar.pack_start(FiltroAno, PACK_SHRINK, 5);
    FiltrosBar.pack_start(FiltroPrecoMinLabel, PACK_SHRINK, 5);
    FiltrosBar.pack_start(FiltroPrecoMin, PACK_SHRINK, 5);
    FiltrosBar.pack_start(FiltroPrecoMaxLabel, PACK_SHRINK, 5);
    FiltrosBar.pack_start(FiltroPrecoMax, PACK_SHRINK, 5);
    FiltrosBar.pack_start(FiltroOutrosLabel, PACK_SHRINK, 5);
    FiltrosBar.pack_start(FiltroUsado, PACK_SHRINK, 5);
    FiltrosBar.pack_start(FiltroNovo, PACK_SHRINK, 5);
    FiltrosBar.pack_start(FilroEletrico, PACK_SHRINK, 5);
    FiltrosBar.pack_start(FiltroGasolina, PACK_SHRINK, 5);
    FiltrosBar.pack_start(FiltroDiesel, PACK_SHRINK, 5);
    FiltrosBar.pack_start(FiltroHibrido, PACK_SHRINK, 5);

    //Adicionar os carros
    mostrarcarros(CarrosBox, FiltroMarca.get_text(), FiltroModelo.get_text(), FiltroCor.get_text(), FiltroAno.get_value(), FiltroPrecoMin.get_value(), FiltroPrecoMax.get_value(), FiltroUsado.get_active(), FiltroNovo.get_active(), FilroEletrico.get_active(), FiltroGasolina.get_active(), FiltroDiesel.get_active(), FiltroHibrido.get_active(), false);

    //Mudar os carros quando os filtros mudam
    FiltroMarca.signal_changed().connect([&CarrosBox, &FiltroMarca, &FiltroModelo, &FiltroCor, &FiltroAno, &FiltroPrecoMin, &FiltroPrecoMax, &FiltroUsado, &FiltroNovo, &FilroEletrico, &FiltroGasolina, &FiltroDiesel, &FiltroHibrido]
    {
        mostrarcarros(CarrosBox, FiltroMarca.get_text(), FiltroModelo.get_text(), FiltroCor.get_text(), FiltroAno.get_value(), FiltroPrecoMin.get_value(), FiltroPrecoMax.get_value(), FiltroUsado.get_active(), FiltroNovo.get_active(), FilroEletrico.get_active(), FiltroGasolina.get_active(), FiltroDiesel.get_active(), FiltroHibrido.get_active(), false);
    });

    FiltroModelo.signal_changed().connect([&CarrosBox, &FiltroMarca, &FiltroModelo, &FiltroCor, &FiltroAno, &FiltroPrecoMin, &FiltroPrecoMax, &FiltroUsado, &FiltroNovo, &FilroEletrico, &FiltroGasolina, &FiltroDiesel, &FiltroHibrido]
    {
        mostrarcarros(CarrosBox, FiltroMarca.get_text(), FiltroModelo.get_text(), FiltroCor.get_text(), FiltroAno.get_value(), FiltroPrecoMin.get_value(), FiltroPrecoMax.get_value(), FiltroUsado.get_active(), FiltroNovo.get_active(), FilroEletrico.get_active(), FiltroGasolina.get_active(), FiltroDiesel.get_active(), FiltroHibrido.get_active(), false);
    });

    FiltroCor.signal_changed().connect([&CarrosBox, &FiltroMarca, &FiltroModelo, &FiltroCor, &FiltroAno, &FiltroPrecoMin, &FiltroPrecoMax, &FiltroUsado, &FiltroNovo, &FilroEletrico, &FiltroGasolina, &FiltroDiesel, &FiltroHibrido]
    {
        mostrarcarros(CarrosBox, FiltroMarca.get_text(), FiltroModelo.get_text(), FiltroCor.get_text(), FiltroAno.get_value(), FiltroPrecoMin.get_value(), FiltroPrecoMax.get_value(), FiltroUsado.get_active(), FiltroNovo.get_active(), FilroEletrico.get_active(), FiltroGasolina.get_active(), FiltroDiesel.get_active(), FiltroHibrido.get_active(), false);
    });

    FiltroAno.signal_value_changed().connect([&CarrosBox, &FiltroMarca, &FiltroModelo, &FiltroCor, &FiltroAno, &FiltroPrecoMin, &FiltroPrecoMax, &FiltroUsado, &FiltroNovo, &FilroEletrico, &FiltroGasolina, &FiltroDiesel, &FiltroHibrido]
    {
        mostrarcarros(CarrosBox, FiltroMarca.get_text(), FiltroModelo.get_text(), FiltroCor.get_text(), FiltroAno.get_value(), FiltroPrecoMin.get_value(), FiltroPrecoMax.get_value(), FiltroUsado.get_active(), FiltroNovo.get_active(), FilroEletrico.get_active(), FiltroGasolina.get_active(), FiltroDiesel.get_active(), FiltroHibrido.get_active(), false);
    });

    FiltroPrecoMin.signal_value_changed().connect([&CarrosBox, &FiltroMarca, &FiltroModelo, &FiltroCor, &FiltroAno, &FiltroPrecoMin, &FiltroPrecoMax, &FiltroUsado, &FiltroNovo, &FilroEletrico, &FiltroGasolina, &FiltroDiesel, &FiltroHibrido]
    {
        mostrarcarros(CarrosBox, FiltroMarca.get_text(), FiltroModelo.get_text(), FiltroCor.get_text(), FiltroAno.get_value(), FiltroPrecoMin.get_value(), FiltroPrecoMax.get_value(), FiltroUsado.get_active(), FiltroNovo.get_active(), FilroEletrico.get_active(), FiltroGasolina.get_active(), FiltroDiesel.get_active(), FiltroHibrido.get_active(), false);
    });

    FiltroPrecoMax.signal_value_changed().connect([&CarrosBox, &FiltroMarca, &FiltroModelo, &FiltroCor, &FiltroAno, &FiltroPrecoMin, &FiltroPrecoMax, &FiltroUsado, &FiltroNovo, &FilroEletrico, &FiltroGasolina, &FiltroDiesel, &FiltroHibrido]
    {
        mostrarcarros(CarrosBox, FiltroMarca.get_text(), FiltroModelo.get_text(), FiltroCor.get_text(), FiltroAno.get_value(), FiltroPrecoMin.get_value(), FiltroPrecoMax.get_value(), FiltroUsado.get_active(), FiltroNovo.get_active(), FilroEletrico.get_active(), FiltroGasolina.get_active(), FiltroDiesel.get_active(), FiltroHibrido.get_active(), false);
    });

    FiltroUsado.signal_toggled().connect([&CarrosBox, &FiltroMarca, &FiltroModelo, &FiltroCor, &FiltroAno, &FiltroPrecoMin, &FiltroPrecoMax, &FiltroUsado, &FiltroNovo, &FilroEletrico, &FiltroGasolina, &FiltroDiesel, &FiltroHibrido]
    {
        mostrarcarros(CarrosBox, FiltroMarca.get_text(), FiltroModelo.get_text(), FiltroCor.get_text(), FiltroAno.get_value(), FiltroPrecoMin.get_value(), FiltroPrecoMax.get_value(), FiltroUsado.get_active(), FiltroNovo.get_active(), FilroEletrico.get_active(), FiltroGasolina.get_active(), FiltroDiesel.get_active(), FiltroHibrido.get_active(), false);
    });

    FiltroNovo.signal_toggled().connect([&CarrosBox, &FiltroMarca, &FiltroModelo, &FiltroCor, &FiltroAno, &FiltroPrecoMin, &FiltroPrecoMax, &FiltroUsado, &FiltroNovo, &FilroEletrico, &FiltroGasolina, &FiltroDiesel, &FiltroHibrido]
    {
        mostrarcarros(CarrosBox, FiltroMarca.get_text(), FiltroModelo.get_text(), FiltroCor.get_text(), FiltroAno.get_value(), FiltroPrecoMin.get_value(), FiltroPrecoMax.get_value(), FiltroUsado.get_active(), FiltroNovo.get_active(), FilroEletrico.get_active(), FiltroGasolina.get_active(), FiltroDiesel.get_active(), FiltroHibrido.get_active(), false);
    });

    FilroEletrico.signal_toggled().connect([&CarrosBox, &FiltroMarca, &FiltroModelo, &FiltroCor, &FiltroAno, &FiltroPrecoMin, &FiltroPrecoMax, &FiltroUsado, &FiltroNovo, &FilroEletrico, &FiltroGasolina, &FiltroDiesel, &FiltroHibrido]
    {
        mostrarcarros(CarrosBox, FiltroMarca.get_text(), FiltroModelo.get_text(), FiltroCor.get_text(), FiltroAno.get_value(), FiltroPrecoMin.get_value(), FiltroPrecoMax.get_value(), FiltroUsado.get_active(), FiltroNovo.get_active(), FilroEletrico.get_active(), FiltroGasolina.get_active(), FiltroDiesel.get_active(), FiltroHibrido.get_active(), false);
    });

    FiltroGasolina.signal_toggled().connect([&CarrosBox, &FiltroMarca, &FiltroModelo, &FiltroCor, &FiltroAno, &FiltroPrecoMin, &FiltroPrecoMax, &FiltroUsado, &FiltroNovo, &FilroEletrico, &FiltroGasolina, &FiltroDiesel, &FiltroHibrido]
    {
        mostrarcarros(CarrosBox, FiltroMarca.get_text(), FiltroModelo.get_text(), FiltroCor.get_text(), FiltroAno.get_value(), FiltroPrecoMin.get_value(), FiltroPrecoMax.get_value(), FiltroUsado.get_active(), FiltroNovo.get_active(), FilroEletrico.get_active(), FiltroGasolina.get_active(), FiltroDiesel.get_active(), FiltroHibrido.get_active(), false);
    });

    FiltroDiesel.signal_toggled().connect([&CarrosBox, &FiltroMarca, &FiltroModelo, &FiltroCor, &FiltroAno, &FiltroPrecoMin, &FiltroPrecoMax, &FiltroUsado, &FiltroNovo, &FilroEletrico, &FiltroGasolina, &FiltroDiesel, &FiltroHibrido]
    {
        mostrarcarros(CarrosBox, FiltroMarca.get_text(), FiltroModelo.get_text(), FiltroCor.get_text(), FiltroAno.get_value(), FiltroPrecoMin.get_value(), FiltroPrecoMax.get_value(), FiltroUsado.get_active(), FiltroNovo.get_active(), FilroEletrico.get_active(), FiltroGasolina.get_active(), FiltroDiesel.get_active(), FiltroHibrido.get_active(), false);
    });

    FiltroHibrido.signal_toggled().connect([&CarrosBox, &FiltroMarca, &FiltroModelo, &FiltroCor, &FiltroAno, &FiltroPrecoMin, &FiltroPrecoMax, &FiltroUsado, &FiltroNovo, &FilroEletrico, &FiltroGasolina, &FiltroDiesel, &FiltroHibrido]
    {
        mostrarcarros(CarrosBox, FiltroMarca.get_text(), FiltroModelo.get_text(), FiltroCor.get_text(), FiltroAno.get_value(), FiltroPrecoMin.get_value(), FiltroPrecoMax.get_value(), FiltroUsado.get_active(), FiltroNovo.get_active(), FilroEletrico.get_active(), FiltroGasolina.get_active(), FiltroDiesel.get_active(), FiltroHibrido.get_active(), false);
    });

    //adicionar o grid ao box
    procurarCarrosBox.pack_start(gridCarros);

    //Box definições da conta
    Entry nomeEntry, emailEntry, contactoEntry, moradaEntry, nifEntry, cartaoEntry, cvvEntry;
    Button salvarDef("Salvar"), mudarPasseDef("Mudar palavra-passe");
    Box mudarPasse;
    Label defLabelerro("");

    nomeEntry.set_placeholder_text("Nome");
    emailEntry.set_placeholder_text("Email");
    contactoEntry.set_placeholder_text("Numero de telemovel (+351)");
    moradaEntry.set_placeholder_text("Morada");
    nifEntry.set_placeholder_text("NIF");
    cartaoEntry.set_placeholder_text("Cartao de credito");
    cvvEntry.set_placeholder_text("CVV");

    defbox.pack_start(nomeEntry, PACK_SHRINK, 5);
    defbox.pack_start(emailEntry, PACK_SHRINK, 5);
    defbox.pack_start(contactoEntry, PACK_SHRINK, 5);
    defbox.pack_start(moradaEntry, PACK_SHRINK, 5);
    defbox.pack_start(nifEntry, PACK_SHRINK, 5);
    defbox.pack_start(cartaoEntry, PACK_SHRINK, 5);
    defbox.pack_start(cvvEntry, PACK_SHRINK, 5);
    defbox.pack_start(defLabelerro, PACK_SHRINK, 5);
    defbox.pack_start(salvarDef, PACK_SHRINK, 5);
    defbox.pack_start(mudarPasseDef, PACK_SHRINK, 5);

    //Mudar palavra passe
    Entry passeAntiga, passeNova, passeConfirm;
    Button mudarPassebtn("Mudar palavra-passe");
    Label mudarPasseErro("");
    contentStackUser.add(mudarPasse, "mudarPasse");

    passeAntiga.set_placeholder_text("Insira a palavra-passe atual");
    passeNova.set_placeholder_text("Insira a palavra-passe nova");
    passeConfirm.set_placeholder_text("Confirme a palavra passe");

    mudarPasse.pack_start(passeAntiga, PACK_SHRINK, 5);
    mudarPasse.pack_start(passeNova, PACK_SHRINK, 5);
    mudarPasse.pack_start(passeConfirm, PACK_SHRINK, 5);
    mudarPasse.pack_start(mudarPasseErro, PACK_SHRINK, 5);
    mudarPasse.pack_start(mudarPasse, PACK_SHRINK, 5);

    //Salvar as definições da conta quando dar click no botão
    salvarDef.signal_clicked().connect([&emailEntry, &contactoEntry, &nomeEntry, &moradaEntry, &nifEntry, &cartaoEntry, &cvvEntry]
    {
        ifstream infoIn("dados/utilizadores/" + utilizadoratual + "/info.txt");
        stringstream infoOut;
        string linha;

        // Read the file line by line
        while(getline(infoIn, linha))
        {
            if (linha.find("Email: ") != string::npos)
            {
                linha = "Email: " + emailEntry.get_text();
            }
            else if (linha.find("Contacto: ") != string::npos)
            {
                linha = "Contacto: " + contactoEntry.get_text();
            }
            else if (linha.find("Nome: ") != string::npos)
            {
                linha = "Nome: " + nomeEntry.get_text();
            }
            else if (linha.find("Morada: ") != string::npos)
            {
                linha = "Morada: " + moradaEntry.get_text();
            }
            else if (linha.find("NIF: ") != string::npos)
            {
                linha = "NIF: " + nifEntry.get_text();
            }
            else if (linha.find("Numero do cartão de crédito: ") != string::npos)
            {
                linha = "Numero do cartão de crédito: " + cartaoEntry.get_text();
            }
            else if (linha.find("CVV: ") != string::npos)
            {
                linha = "CVV: " + cvvEntry.get_text();
            }

            // Write the (possibly modified) line to the stringstream
            infoOut << linha << "\n";
        }

        // Close the input file
        infoIn.close();

        // Open the output file and write the modified content to it
        ofstream infoOutFile("dados/utilizadores/" + utilizadoratual + "/info.txt");
        infoOutFile << infoOut.str();
        infoOutFile.close();
    });

    mudarPassebtn.signal_clicked().connect([&passeAntiga, &passeNova]
    {
        ifstream info("dados/utilizadores" + utilizadoratual + "/info.txt");
        string linha, password;
        bool tem = false;

        while (getline(info, linha))
        {
            //encontrar a passe do utilizador
            size_t start = linha.find("Palavra-passe: ");

            // Encontrar a palavra passe no ficheiro
            if (start != std::string::npos)
            {
                // Obter a senha depois da string "palavra passe: "
                password = linha.substr(start + 15);

                // remover o carater da linha nova ao final da senha
                password.erase(password.find("\n"));

                // Verificar se a palavra passe inserida é igual à palavra passe salvada
                if (passeAntiga.get_text() == password)
                {
                    tem = true;
                }
            }
        }

        // fechar o ficheiro
        info.close();
    });

    //Adicionar os filtros e os carros ao grid
    gridCarros.pack_start(FiltrosBar, PACK_SHRINK);
    gridCarros.pack_start(procurarCarrosScrolledWindow);
    gridCarros.set_orientation(Orientation::ORIENTATION_HORIZONTAL);

    //mudar para a pagina de procurar carros
    procurarbuttonCarros.signal_clicked().connect([&contentStackUser]
    {
        contentStackUser.set_visible_child("procurarCarros");
    });

    //mudar para a pagina de historico
    historicobutton.signal_clicked().connect([&contentStackUser]
    {
        contentStackUser.set_visible_child("historicobox");
    });

    //mudar para a pagina de definições
    defbutton.signal_clicked().connect([&contentStackUser]
    {
        contentStackUser.set_visible_child("defbox");
    });

    defbutton.signal_clicked().connect([&contentStackUser]
    {
        contentStackUser.set_visible_child("defbox");
    });

    mudarPasseDef.signal_clicked().connect([&contentStackUser]
    {
        contentStackUser.set_visible_child("mudarPasse");
    });

    //espaçamento
    DashboardUser.set_spacing(60);

    //adicionar barra de filtros, menus e um texto a dashboard de utilizador
    DashboardUser.pack_start(TopBarUser);
    DashboardUser.pack_start(contentStackUser);

    //adicionar a dashboard ao stack
    stack.add(DashboardUser, "dashboarduser");
#endif

#if 1 // Butoes de ação
    loginButton.signal_clicked().connect([&stack, &usernameEntry, &passwordEntry, &dashboardLabelUser, &erro]
    {
        
        //caminho temporario para os dados do utilizador
        string temppath = "dados/utilizadores/" + usernameEntry.get_text(); 

        //verificar se o utilizador existe
        if(pathExists(temppath) && usernameEntry.get_text() != "" && passwordEntry.get_text() != "")
        {
            //verificar se a palavra-passe está correta
            ifstream info(temppath + "/info.txt");
            string linha;
            bool tem = false;
            string password = "Palavra-passe: " + passwordEntry.get_text();

            //loop para verificar se a palavra-passe existe
            while(getline(info, linha))
            {
                if(linha == password)
                {
                    tem = true;
                }
            }

            if(tem)
            {
                //mudar para a pagina da dashboard e meter o nome do utilizador na label
                dashboardLabelUser.set_text("Bem-vindo " + usernameEntry.get_text() + "!");
                stack.set_visible_child("dashboarduser");

                utilizadoratual = usernameEntry.get_text();

                cout << "Utilizador atual: " << utilizadoratual << endl;

                // resetar os campos
                erro.set_text("");
                usernameEntry.set_text("");
                passwordEntry.set_text("");
            }
            else
            {
                //erro na palavra-passe
                erro.set_text("Palavra passe incorreta");
            }

            //fechar o ficheiro
            info.close();
        }
        else if (usernameEntry.get_text() == "admin" && passwordEntry.get_text() == "admin")
        {
            //mudar para a pagina da dashboard e meter o nome do utilizador na label
            stack.set_visible_child("dashboardadmin");

            cout << "Conta de administrador" << endl;

            // resetar os campos
            erro.set_text("");
            usernameEntry.set_text("");
            passwordEntry.set_text("");
        }
        else if (usernameEntry.get_text() == "Debugshowcontas")
        {
            //mostrar os utilizadores
            string utilizadores = "Utilizadores: \n";
            for (const auto & entry : filesystem::directory_iterator("dados/utilizadores"))
            {
                utilizadores += entry.path().filename().string() + "\n";
            }
            cout << utilizadores.c_str() << endl;
        }
        else if (usernameEntry.get_text() == "Debugshowcarros")
        {
            //mostrar os carros
            string carros = "Carros: \n";
            for (const auto & entry : filesystem::directory_iterator("dados/carros"))
            {
                carros += entry.path().filename().string() + "\n";
            }
            cout << carros.c_str() << endl;
        }
        else if (usernameEntry.get_text() == "" || passwordEntry.get_text() == "")
        {
            //erro nos campos vazios
            erro.set_text("Campos vazios no formulario de login");
        }
        else
        {
            //erro no nome de utilizador
            erro.set_text("Nome de utilizador não existe");
        }
    });

    registerButton.signal_clicked().connect([&stack, &erro, &usernameEntry, &passwordEntry]
    {
        erro.set_text("");
        usernameEntry.set_text("");
        passwordEntry.set_text("");
        stack.set_visible_child("registrar");

        cout << "Mudar para a pagina de registo" << endl;
    });

    registerButton2.signal_clicked().connect([&stack, &usernamereg, &passwordreg, &passwordregconfirm, &erroreg, &usernameEntry, &passwordEntry]
    {
        string usernameEntrytemp = usernameEntry.get_text();

        if((passwordreg.get_text() == passwordregconfirm.get_text()) && (usernamereg.get_text() != "") && (passwordreg.get_text() != "") && (passwordregconfirm.get_text() != "") && (!temcaracterespecial(usernameEntrytemp)))
        {
            erroreg.set_text("");

            string temppath = "dados/utilizadores/" + usernamereg.get_text();

            if(!pathExists(temppath))
            {
                //criar o utilizador
                criarutilizador(usernamereg.get_text(), passwordreg.get_text());

                //para fazer login automatico
                usernameEntry.set_text(usernamereg.get_text());
                passwordEntry.set_text(passwordreg.get_text());

                //resetar os campos
                usernamereg.set_text("");
                passwordreg.set_text("");
                passwordregconfirm.set_text("");
                stack.set_visible_child("login");

                cout << "Utilizador criado" << endl;
            }
            else
            {
                //erro no nome de utilizador
                erroreg.set_text("O nome de utilizador já existe");
            }
    
        } else 
        {
            //erro no formulario de registo
            if(passwordreg.get_text() != passwordregconfirm.get_text())
            {
                erroreg.set_text("As palavras-passe não coincidem");
            }
            else if(usernamereg.get_text() == "")
            {
                erroreg.set_text("O nome de utilizador está vazio");
            }
            else if(passwordreg.get_text() == "")
            {
                erroreg.set_text("A palavra-passe está vazia");
            }
            else if(passwordregconfirm.get_text() == "")
            {
                erroreg.set_text("A confirmação da palavra-passe está vazia");
            }
            else if(temcaracterespecial(usernameEntrytemp))
            {
                erroreg.set_text("O nome de utilizador não pode conter caracteres especiais");
            }
            else
            {
                erroreg.set_text("Erro no formulario de registo");
            }
        }
    });

    cancelarreg.signal_clicked().connect([&stack, &erroreg, &usernamereg, &passwordreg, &passwordregconfirm]
    {
        erroreg.set_text("");
        usernamereg.set_text("");
        passwordreg.set_text("");
        passwordregconfirm.set_text("");
        stack.set_visible_child("login");

        cout << "Cancelar registo" << endl;
    });

    //mudar para a pagina de login
    logoutbuttonuser.signal_clicked().connect([&stack]{ 
        stack.set_visible_child("login");
        utilizadoratual = "";

        cout << "Sair da conta" << endl;
        });

    logoutbuttonadmin.signal_clicked().connect([&stack]{ 
        stack.set_visible_child("login");
        utilizadoratual = "";

        cout << "Sair da conta" << endl;
        });

#endif

#if 1 // Auto completar

    class ModelColumns : public TreeModel::ColumnRecord
    {
    public:
        ModelColumns()
        { add(m_col_name); }

        TreeModelColumn<Glib::ustring> m_col_name;
    };

    ModelColumns m_Columns;

    Glib::RefPtr<Gtk::EntryCompletion> compmarca = Gtk::EntryCompletion::create();
    Glib::RefPtr<Gtk::ListStore> modelmarca = Gtk::ListStore::create(m_Columns);
    compmarca->set_model(modelmarca);
    FiltroMarca.set_completion(compmarca);
    criarCarroMarca.set_completion(compmarca);

#endif

    // alinhamento
    Alignment *align = manage(new Alignment(0.5, 0.5, 0, 0));
    align->add(stack);
    appWindow.add(*align);
    appWindow.show_all();

    Main::run(appWindow);
    return 0;
}