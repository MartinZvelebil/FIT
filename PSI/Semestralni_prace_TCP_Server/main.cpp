#include <iostream>
#include <sys/socket.h> // socket(), bind(), connect(), listen()
#include <unistd.h> // close(), read(), write()
#include <netinet/in.h> // struct sockaddr_in
#include <strings.h> // bzero()
#include <wait.h> // waitpid()
#include <string>
#include <math.h>
#include <regex>

#define TIMEOUT 1 //Zatím vyšší pro testing
#define BUFFER_SIZE 1024
using namespace std;

int stepsToSkip = 0;
bool flag_turned_left = false;

int determinateQuadrant(int x, int y)
{
    if(x > 0 && y > 0)//pravo nahoře
    {
        return 1;
    }
    else if(x > 0 && y < 0)//pravo dole
    {
        return 2;
    }
    else if(x < 0 && y < 0) //levo dole
    {
        return 3;
    }
    else if(x < 0 && y > 0) //levo nahoře
    {
        return 4;
    }
    return -1;
}

int checkInsideHandling(int socket_number, int x, int y)
{
    if(x == 0 && y == 0){ //Pokud jsem během obcházení překážky se objevil na 0,0 tak vezmu message
        write(socket_number,"105 GET MESSAGE\a\b", strlen("105 GET MESSAGE\a\b"));
        cout << "PICK_UP_MESSAGE (inside handling) - from SERVER" << endl;
        return -1;
    }
    else if(x == 0){ //Pokud jsem se objevil na x souřadnici 0, tak se otočím a jdu rovně
        flag_turned_left = true;
        write(socket_number, "103 TURN LEFT\a\b", strlen("103 TURN LEFT\a\b"));
        cout << "TURN LEFT ON x/y==0 (inside handling)- from SERVER" << endl;
        return -1;
    }
    return 0;
}

void handleObstacleRight(int socket_number, int x, int y)
{
    cout << "Going from the right site" << endl;
    write(socket_number, "104 TURN RIGHT\a\b", strlen("104 TURN RIGHT\a\b"));
    cout << "RIGHT (1) - FROM SERVER" << endl;
    write(socket_number,"102 MOVE\a\b", strlen("102 MOVE\a\b"));
    cout << "MOVE (2) - FROM SERVER" << endl;
    write(socket_number, "103 TURN LEFT\a\b", strlen("103 TURN LEFT\a\b"));
    cout << "LEFT (3) - FROM SERVER" << endl;
    write(socket_number,"102 MOVE\a\b", strlen("102 MOVE\a\b"));
    cout << "MOVE (4) - FROM SERVER" << endl;
        x--; //Chtělo by to přijímat real responses a ne si vytvářet kudy vlastně jdu nějak takhle
        if(checkInsideHandling(socket_number, x, y) == -1)
        {stepsToSkip = 4; return;}
    write(socket_number,"102 MOVE\a\b", strlen("102 MOVE\a\b"));
    cout << "MOVE (5) - FROM SERVER" << endl;
        x--;
        if(checkInsideHandling(socket_number, x, y) == -1)
        {stepsToSkip = 5;return;}
    write(socket_number, "103 TURN LEFT\a\b", strlen("103 TURN LEFT\a\b"));
    cout << "LEFT (6) - FROM SERVER" << endl;
    write(socket_number,"102 MOVE\a\b", strlen("102 MOVE\a\b"));
    cout << "MOVE (7) - FROM SERVER" << endl;
    write(socket_number, "104 TURN RIGHT\a\b", strlen("104 TURN RIGHT\a\b"));
    cout << "RIGHT (8) - FROM SERVER" << endl;
    stepsToSkip=7;
}

void handleObstacleLeft(int socket_number, int x, int y)
{
    cout << "Going from the left site" << endl;
    write(socket_number, "103 TURN LEFT\a\b", strlen("103 TURN LEFT\a\b"));
    cout << "LEFT (1) - FROM SERVER" << endl;
    write(socket_number,"102 MOVE\a\b", strlen("102 MOVE\a\b"));
    cout << "MOVE (2) - FROM SERVER" << endl;
    write(socket_number, "104 TURN RIGHT\a\b", strlen("104 TURN RIGHT\a\b"));
    cout << "RIGHT (3) - FROM SERVER" << endl;
    write(socket_number,"102 MOVE\a\b", strlen("102 MOVE\a\b"));
    cout << "MOVE (4) - FROM SERVER" << endl; //Je to jen u těch move steps
        x--;
        if(checkInsideHandling(socket_number, x, y) == -1)
        {stepsToSkip = 4;return;}
    write(socket_number,"102 MOVE\a\b", strlen("102 MOVE\a\b"));
    cout << "MOVE (5) - FROM SERVER" << endl;
        x--;
        if(checkInsideHandling(socket_number, x, y) == -1)
        {stepsToSkip = 5;return;}
    write(socket_number, "104 TURN RIGHT\a\b", strlen("104 TURN RIGHT\a\b"));
    cout << "RIGHT (6) - FROM SERVER" << endl;
    write(socket_number,"102 MOVE\a\b", strlen("102 MOVE\a\b"));
    cout << "MOVE (7) - FROM SERVER" << endl;
    write(socket_number, "103 TURN LEFT\a\b", strlen("103 TURN LEFT\a\b"));
    cout << "LEFT (8) - FROM SERVER" << endl;
    stepsToSkip=7;
}

void changeDirection (int socket_number)
{
    stepsToSkip = 2;
    write(socket_number, "104 TURN RIGHT\a\b", strlen("104 TURN RIGHT\a\b"));
    cout << "RIGHT (1) - FROM SERVER" << endl;
    write(socket_number, "104 TURN RIGHT\a\b", strlen("104 TURN RIGHT\a\b"));
    cout << "RIGHT (2) - FROM SERVER" << endl;
    write(socket_number,"102 MOVE\a\b", strlen("102 MOVE\a\b")); //Move nakonec, jinak by totiž bylo x_before a x v další interaci stejný
    cout << "MOVE (3) - FROM SERVER" << endl;
}

char * IntToCharArray(int number)
{
    int n = log10(number) + 1;
    int i;
    char *numberArray = (char *)calloc(n, sizeof(char));
    for (i = n-1; i >= 0; --i, number /= 10)
    {
        numberArray[i] = (number % 10) + '0';
    }
    return numberArray;
}
//Funkce na autentikaci - přijme přezdívku
int authenticateUserName(const string & username, int socket_number)
{
    //Nesmí víc než 18 znaku + end znaky
    if(username.length() > 22)
    {
        send(socket_number ,  "LOGIN_FAILED\a\b", strlen("LOGIN_FAILED\a\b") , 0 );
        cerr << "LOGIN_FAILED Send from SERVER\a\b" << endl;
        return -1;
    }
    else if(username.length() <= 22) //Pokud je validní
    {
        send(socket_number , "107 KEY REQUEST\a\b" ,  strlen("107 KEY REQUEST\a\b"), 0 );
        cout << "107 KEY REQUEST Send from SERVER" << endl;
        return 0;
    }
    else{cerr << "Unexpected behaviour" << endl; return -1;}
}

//Ověření + zahashování KEY_ID
int keyIdAccept(int ID, int socket_number, const string & username)
{
    //KEY_OUT_OF_RANGE_ERROR
    cout << ID << endl;
    if(ID < 0 || ID > 4)
    {
        send(socket_number ,  "303 KEY OUT OF RANGE\a\b", strlen("303 KEY OUT OF RANGE\a\b") , 0 );
        cerr << "KEY_OUT_OF_RANGE_ERROR - from SERVER\a\b" << endl;
        return -1;
    }
    else //Pokud ID v pořádku
    {
        int ServerKeys[] = {23019, 32037, 18789, 16443, 18189 };
        int asciisum = 0;
        int hash = 0;

        for (char i : username)  //Sečtu ascii vyjádření username
        {
            asciisum += i;
        }
        hash = (asciisum * 1000) % 65536; //Asciisum dle vzorečku
        hash = (hash + ServerKeys[ID]) % 65536; //Přičtení server keyID

        char * h = IntToCharArray(hash);
        write(socket_number, h, strlen(h)); //Write = simplified send
        write(socket_number,"\a\b",2);
        cout << "SERVER_CONFIRMATION - Hash send to Client: " << hash << endl;
        return 0;
    }
}

//Client confirmation message = client side vypocitany hash - acceptance + confirmation
int keyIdResponseClient(int ID, int socket_number, const string & username, int expectedClientHash)
{
    int ClientKeys[] = {32037, 29295, 13603, 29533, 21952 };
    int asciisum = 0;
    int hash = 0;

    for (char i : username)  //Sečtu ascii vyjádření username
    {
        asciisum += i;
    }
    hash = (asciisum * 1000) % 65536; //Asciisum dle vzorečku
    hash = (hash + ClientKeys[ID]) % 65536; //Přičtení server keyID
    if(expectedClientHash != hash)
    {
        send(socket_number ,  "300 LOGIN FAILED\a\b", strlen("300 LOGIN FAILED\a\b") , 0 );
        cerr << "SERVER_LOGIN_FAILED - Neshoduje se clientem zaslany hash\a\b" << endl;
        return -1;
    }
    else
    {
        write(socket_number,"200 OK\a\b", strlen("200 OK\a\b"));
        cout << "SERVER_OK - from SERVER" << endl;
        write(socket_number,"102 MOVE\a\b", strlen("102 MOVE\a\b"));
        cout << "SERVER_move - from SERVER" << endl;
        return 0;
    }
}

int stepNumNavigate_gl = -1; //Kolikrát proběhla funkce navigate robot
int x_before = 0; //Proměnné, co hlídají, jaké byly funkční hodnoty v minulém kroku (kvůli překážkám apod.)
int y_before = 0;
bool justHandledObstacle = false;
bool justTurnedArround = false;
bool initial_flag_turned_left = false;
//Navigace robota
int navigateRobot(int socket_number, int x, int y)
{
    //x = abs(x); //Udělám si z souřadnic absolutní hodnoty pro lehčí počítání
    //y = abs(y);
    if(stepsToSkip > 0)
    {
        cout << "Skipping one response - from SERVER" << endl;
        stepsToSkip = stepsToSkip - 1;
        return 0;
    }

    stepNumNavigate_gl++;
    if(x == 0 && y == 0) //Pokud jsem v cíli, tak vyzvedni message
    {
        write(socket_number,"105 GET MESSAGE\a\b", strlen("105 GET MESSAGE\a\b"));
        cout << "PICK_UP_MESSAGE - from SERVER" << endl;
        return 0;
    }

    //Initial movement
    if(stepNumNavigate_gl == 0) //Vždycky začnu otočkou doprava, ani nevím proč, taky už nastavím initial souřadnice ({x,y}_before)
    {
        if(x == 0 || y == 0) //Pokud jsem začal rovnou na nule, tak se nechci nikdy otáčet doleva
        {
            initial_flag_turned_left = true;
        }
        //write(socket_number,"102 MOVE\a\b", strlen("102 MOVE\a\b"));
        write(socket_number, "104 TURN RIGHT\a\b", strlen("104 TURN RIGHT\a\b"));
        cout << "INITIAL TURN RIGHT - from SERVER" << endl;
        x_before = x;
        y_before = y;
        return 0;
    }

    //Pokud o řešení, at hajzl jde k X a ne k Y
    if(stepNumNavigate_gl >= 2 && stepNumNavigate_gl <= 4 && y_before > y && !initial_flag_turned_left && x != 0 && y != 0)
    {
        initial_flag_turned_left = true;
        write(socket_number, "103 TURN LEFT\a\b", strlen("103 TURN LEFT\a\b"));
        cout << "Turn left, because bitch going to Y - from SERVER" << endl;
        return 0;
    }

    //Jaká proměnná změnila směr ? - jakým jdu směrem
    if(!flag_turned_left && (x == 0 || y == 0)) //Když se v nějaké ose dostanu na počátek, tak jen jedna otočka doleva, pokud bych se otočil špatně, tak mě zase otočí funkce níže
    {
        flag_turned_left = true;
        write(socket_number, "103 TURN LEFT\a\b", strlen("103 TURN LEFT\a\b"));
        cout << "TURN LEFT ON x/y==0 - from SERVER" << endl;
        return 0;
    }
    else if(x_before != x && y_before == y)                                                       //Pokud jsem změnil polohu na ose x a nejsem ještě v [0,y]
    {
        if(abs(x_before) < abs(x))                                                                               //Pokud předtím byla souřadnice menší, tak to znamená, že jdu opačně než chci, takže dvakrát otočka
        {
            changeDirection(socket_number);
            cout << "Wrong direction on X -> changing direction (turned arround) - from SERVER" << endl;
        }
        else if(abs(x_before) > abs(x))                                                                          //Pokud jdu správným směrem, tak paráda, pokračuju v tom
        {
            write(socket_number,"102 MOVE\a\b", strlen("102 MOVE\a\b"));
            cout << "All OK - going to the X zero - from SERVER" << endl;
        }
        justTurnedArround = false;
        justHandledObstacle = false;
    }
    else if(x_before == x && y_before != y)                                                  //Pokud se změnilo y a ještě nejsem na y==0
    {
        if(abs(y_before) < abs(y))                                                                               //Pokud předtím byla souřadnice menší, tak to znamená, že jdu opačně než chci, takže dvakrát otočka
        {
            changeDirection(socket_number);
            cout << "Wrong direction on Y -> changing direction - from SERVER" << endl;
        }
        else if(abs(y_before) > abs(y))                                                                          //Pokud jdu správným směrem, tak paráda, pokračuju v tom
        {
            write(socket_number,"102 MOVE\a\b", strlen("102 MOVE\a\b"));
            cout << "All OK - going to the Y zero - from SERVER" << endl;
        }
        justTurnedArround = false;
        justHandledObstacle = false;
    }
    else if(x_before == x && y_before == y && (!justHandledObstacle || !justTurnedArround) && stepNumNavigate_gl != 1)        //Pokud při zavolání předchozího commandu byly stejné souřadnice, tak jsem narazil na překážku a tudíž ji kompletně obejdu (zprava)
    {                                                                                                 //Pokud jsem právě přeskakoval objective nebo se otáčel, tak je správně, že jsou 2x stejny souřadnice
        justHandledObstacle = true;                                                                   //Pokud to byl jen initial turn right, tak je taky vše ok a nechci vykonávat tohle
        if(determinateQuadrant(x,y) == 1 || determinateQuadrant(x,y) == 3) //Abych obcházel Směrem k [0,0]
        {
            handleObstacleLeft(socket_number, abs(x), abs(y));
        }
        else if(determinateQuadrant(x,y) == 2 || determinateQuadrant(x,y) == 4) //Abych obcházel Směrem k [0,0]
        {
            handleObstacleRight(socket_number, abs(x), abs(y));
        }
        else //Pokud už jsem mimo kvadranty = jdu po nějaké ose {x,y} == 0, tak je jedno kudy půjdu
        {
            handleObstacleLeft(socket_number, abs(x), abs(y));
        }
        cout << "Obstacle handled - from SERVER" << endl;
    }
    else if(stepNumNavigate_gl == 1 && x_before == x && y_before == y)
    {
        write(socket_number, "102 MOVE\a\b", strlen("102 MOVE\a\b"));
        cout << "Move on the start - from SERVER" << endl;
    }
    else
    {
        write(socket_number, "102 MOVE\a\b", strlen("102 MOVE\a\b"));
        cout << "Movement for fun - from SERVER" << endl;
    }
    x_before = x;
    y_before = y;
    return 0;
}

string username_gl; //Globální username
int ID_gl;
int messNum_gl = 0; //Globální proměnná počítající kolikrát už bylo něco odesláno

//All message management = rozlišuje, jakou mám udělat operaci + pokud return value = -1, potom konec programu = uzavřít spojení
int messageManage(char * buffer, int socket_number)
{
    //cout << "TAAAAAAAADY:" << buffer << endl;
    int returnvalue = 0; //Všechny kroky
    int buffer_length = strlen(buffer); //Všechny kroky
    regex rx("[[:digit:]]{1,5}\a\b"); //3. Krok
    regex rx2("[[:digit:]]{1}\a\b"); //2. Krok
    regex rx3("OK -?[0-9]+ -?[0-9]+\a\b"); //Souřadnice

    //KROKY - Jsou řazeny od nejvíce restriktivních po ty nejméně, aby vše fungovalo
    if(regex_match(buffer, rx3)) //Pokud přijdou nějaký souřadnice
    {
        //Rozdělování jednotlivých souřadnic pomocí strtok
        int coordinates[2] = {0};
        int index = -1;
        char *ptr = nullptr;
        ptr = strtok(buffer, " ");
        while (ptr != nullptr)
        {
            if(index != -1) //Potřebuju přeskočit to OK
            {
                coordinates[index] = atoi(ptr);
                //cout << ptr << endl;
            }
            ptr = strtok (nullptr, " ");
            index ++;
        }
        cout << "X: " << coordinates[0] << endl; //x
        cout << "Y: " << coordinates[1] << endl; //y
        returnvalue = navigateRobot(socket_number, coordinates[0], coordinates[1]);
        messNum_gl++;
    }
    //Pokud se jedná o IDKey -> druhý krok (Odešlu mnou vypočítaný server-site hash)
    else if(regex_match(buffer,rx2)) //Pokud ty hodnoty jsou 0-4 + ukončovací znaky
    {
        returnvalue = keyIdAccept(buffer[0]-48, socket_number, username_gl);
        if(returnvalue != -1)
        {
            ID_gl = buffer[0]-48;
        }
        messNum_gl++;
    }
    //Pokud se jedná o hash ze strany clienta -> třetí krok (NEDOKONČENÝ - )
    else if(regex_match(buffer,rx) && messNum_gl < 5)
    {
        string client_hash;
        for (int i = 0; i < buffer_length; ++i)
        {
            client_hash += buffer[i]; //client_hash = čistej hash ready na convert na int (bez \a\b)
        }
        returnvalue = keyIdResponseClient(ID_gl,socket_number,username_gl,stoi(client_hash));
        messNum_gl++;
    }
    //Když přijde username -> první krok (Musí být skoro poslední, protože matchuje nejvíc cases - nejméně restrictive)
    else if(buffer_length <= 20 && buffer_length > 2 && messNum_gl == 0) //Pokud je vstup 4-20 znaku (dle zadani) a messNum = 0 (je to první zpráva)
    {
        for (int i = 0; i < buffer_length-2; ++i) //uložím do stringu username global tu přezdívku bez \a\b
        {
            if(buffer[i] != '`') //Když jsem nahrazoval "\0" zpětným apostrofem, tak ten znak přeskočím
            {
                username_gl += buffer[i];
            }
        }
        returnvalue = authenticateUserName(username_gl,socket_number);
        messNum_gl++;
    }
    //Když přijde závěrečná zpráva -> poslední krok
    else if(buffer_length <= 102 && buffer_length > 2 && messNum_gl > 3)//Pokud je vstup 3-102 znaku - tajemstvi a messNum > 3 (proběhla auth + minimalně jeden move)
    {
        write(socket_number,"106 LOGOUT\a\b", strlen("106 LOGOUT\a\b"));
        cout << "LOGOUT - from SERVER" << endl;
    }
    else //Pokud žádná horní podmínka se nezavolá, tak prostě špatná zpráva, takže syntax error
    {
        write(socket_number,"301 SYNTAX ERROR\a\b", strlen("301 SYNTAX ERROR\a\b"));
    }
    return returnvalue; //S tím musím ještě dále pracovat
}

//Funkce hlídající timeout
int timeOut(int uniq_client_socket)
{
    struct timeval timeout;
    fd_set sockets; //fd_set makro -> definuje dat. strukturu
    int retval; //Proměnná na uložení návratové hodnoty
    FD_ZERO(&sockets); //Adresa sockets, vynuluje bajty
    FD_SET(uniq_client_socket, &sockets); //Přidává do struktury číslo socketu, na kterém chceme naslouchat
    timeout.tv_sec = TIMEOUT; //Nastavení timeout v sekundách -> po kolika sekundách bez odpovědi ukončit vlákno
    timeout.tv_usec = 0;

    // Prvnim parametrem je cislo nejvyssiho soketu v 'sockets' zvysene o jedna.
    // (Velka jednoduchost a efektivnost funkce je vyvazena spatnou ergonomii pro uzivatele.)
    // Posledni z parametru je samotny timeout. Je to struktura typu 'struct timeval',
    // ktera umoznuje casovani s presnosti na mikrosekundy (+/-). Funkci se predava
    // odkaz na promennou a funkce ji muze modifikovat. Ve vetsine implementaci
    // odecita funkce od hodnoty timeoutu cas straveny cekanim. Podle manualu na
    // to nelze spolehat na vsech platformach a ve vsech implementacich funkce
    // select()!!!
    retval = select(uniq_client_socket + 1, &sockets, nullptr, nullptr, &timeout); //Kontroluje, zda-li došlo k nějaké události na socketech po TIMEOUT sekund
    if (retval < 0)
    {
        perror("Chyba v select(): ");
        close(uniq_client_socket);
        return -1;
    }
    if (!FD_ISSET(uniq_client_socket, &sockets)) //Pokud nedošlo k žádné události na socketu po těch TIMEOUT sekund - pustila se select funkce
    {
        // Zde je jasne, ze funkce select() skoncila cekani kvuli timeoutu.
        cout << "Connection timeout!" << endl;
        close(uniq_client_socket);
        return -2;
    }
    return 0;
}

int main(int argc, char **argv)
{
    //Čtení přes sockety
    //Beru číslo portu z příkazové řádky - Třeba 3999 (Určitě víc než 1024)
    if(argc < 2)
    {
        cerr << "Usage: server port" << endl;
        return -1;
    }

    //Vytvoreni koncoveho bodu spojeni (socketu)
    int l = socket(AF_INET, SOCK_STREAM, 0); //(Jaký protokol na síťové vrstvě [IPV4], jaký protokol na transportní vrstvě [TCP], příznaky socketu)
    if(l < 0) //Pokud přišel err code (číslo menší než 0)
    {
        perror("Nemohu vytvorit socket: "); //perror vypíše chybovou hlášku
        return -1;
    }

    //Hledání portu
    int port = atoi(argv[1]); //První argument obsahuje číslo portu (atoi konvertuje číslový řetězec na int)
    if(port == 0) //Pokud se číslo nepřevedlo, tak atoi vrátí 0 (Pokud nebylo zadáno)
    {
        cerr << "Usage failure: Add server port" << endl;
        close(l); //Zavíráme port, protože už je vytvořený socket
        return -1;
    }

    struct sockaddr_in adresa; //Založí strukturu
    bzero(&adresa, sizeof(adresa)); //Vyčistí paměť struktury

    adresa.sin_family = AF_INET; //IPV4
    adresa.sin_port = htons(port); //htons = převádí kód dle endianů (Námi získaný port v argumentu)
    adresa.sin_addr.s_addr = htonl(INADDR_ANY); //Nastavení adresy htonl = host to nlong -> převod na IP addresu (Tady by stačil dát prostě localhost - nebo jen jedna IP addresa)

    //Adresa ted obsahuje všechno info, aby se server mohl spustit
    //Prirazeni socketu k síťovému rozhranim
    if(bind(l, (struct sockaddr *) &adresa, sizeof(adresa)) < 0) //Přetypovávání (struct sockaddr *)
    {
        perror("Problem s bind(): "); //perror opět reflektuje systémovou chybu
        close(l);
        return -1;
    }

    // Oznacim socket jako naslouchající (listen)
    if(listen(l, 10) < 0) //Kolik maximálních spojení = n
    {
        perror("Problem s listen()!");
        close(l);
        return -1;
    }

    struct sockaddr_in vzdalena_adresa; //Opět struktura sockaddr_in -> ale remote (vyplní se akcí accept)
    socklen_t velikost;

    //Tady už je vše nastaveno, takže jen rozjedu nekonečný naslouchání
    //Čeká se furt na nový klienty
    while(true)
    {
        // Cekam na prichozi spojeni
        int uniq_client_socket = accept(l, (struct sockaddr *) &vzdalena_adresa, &velikost); //trochu jako bind
        if (uniq_client_socket < 0)
        {
            perror("Problem s accept()!");
            close(l);
            return -1;
        }

        pid_t pid = fork(); //Fork = kopie aktuálního procesu
        if (pid == 0) //Samostatný proces pro každého klienta -> podmínka ověřuje, že je to potomek procesu
        {
            // Kopie hlavniho vlakna ma vlastni referenci na naslouchajici soket.
            // Podproces, ktery obsluhuje klienta, tuto referenci nepotrebuje, takze je dobre
            // tuto referenci smazat. V hlavnim vlakne samozrejme reference na naslouchajici
            // soket zustava.
            close(l); //Zavření reference na naslouchající socket - on to zavře jen v potomkovi
            string string_buffer;

            while (true) //Přijímá data z klienta a vypisuje na obrazovku
            {
                char buffer[BUFFER_SIZE] = {0};

                string_buffer = "";
                while(string_buffer.find("\a\b") == string::npos)
                {
                    if(timeOut(uniq_client_socket)) //Drží dohled na timeoutem
                    {
                        return -1;
                    }
                    size_t bytesRead = recv(uniq_client_socket, buffer, 1,0); //Blokující funkce na přijímání dat
                    buffer[bytesRead] = '\0'; //Přidávám zakončovací nulu
                    if(buffer[0] == '\0') //Pokud přijde special case, kdy mi je zaslána \0 ve jméně, tak ji nahradím zpětným apostrofem, který pak ve funkci odečítám
                    {
                        string_buffer += '`';
                        continue;
                    }
                    else
                    {
                        string_buffer += buffer;
                    }
                }
                cout << endl << "Command: " << string_buffer << endl;

                //Konverze stringu na buffer (už nechci předělávat message manage)
                unsigned int n = string_buffer.length();
                char chararr[n + 1]; //ekvivalent stringu s, jen v poli
                strcpy(chararr, string_buffer.c_str());

                int messageManager_Return = messageManage(chararr, uniq_client_socket);
                if (messageManager_Return == -1) //Pokud přišlo z metody, že se něco pokazilo, tak break
                {
                    break;
                }

                string_buffer = "";
            }
            close(uniq_client_socket);
            return 0;
        }

        // Aby nam nezustavaly v systemu zombie procesy po kazdem obslouzeneme klientovi,
        // je nutne otestovat, zda se uz nejaky podproces ukoncil.
        // Prvni argument rika, cekej na jakykoliv proces potomka, treti argument zajisti,
        // ze je funkce neblokujici (standardne blokujici je, coz ted opravdu nechceme).
        int status = 0;
        waitpid(0, &status, WNOHANG);

        close(uniq_client_socket); // Nove vytvoreny socket je nutne zavrit v hlavnim procesu, protoze by na nem v systemu
        // zustala reference a jeho zavreni v novem procesu by nemelo zadny efekt.
    }
    return 0;
}
