#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
#include <limits>

void mainMenu();

const std::string fileName = "HmedanBank.txt";
const std::string userFile = "Users.txt";
struct stClientInfo
{
    std::string accountNumber;
    std::string name;
    std::string phone;
    std::string PinCode;
    double accountBalance = 0;
    bool markToDelete = false;
};

enum enMainMenuOption
{
    eShowClientList = 1,
    eAddNewClient = 2,
    eDeleteClient = 3,
    eUpdateClient = 4,
    eFindClient = 5,
    eEndProgram = 6
};
void performMainMenuOption(enMainMenuOption);

std::vector<std::string> vSplit(std::string s, const std::string &delimiter)
{
    std::vector<std::string> vString;
    size_t pos = 0;
    std::string word = "";
    while ((pos = s.find(delimiter)) != std::string::npos)
    {
        word = s.substr(0, pos);
        if (word != "")
        {
            vString.push_back(word);
        }
        s.erase(0, pos + delimiter.length());
    }
    if (s != "")
    {
        vString.push_back(s);
    }
    return vString;
}

stClientInfo convertLineToRecord(const std::string &line, const std::string &separator = "#//#")
{
    std::vector<std::string> vString = vSplit(line, separator);
    stClientInfo client;
    client.accountNumber = vString[0];
    client.PinCode = vString[1];
    client.name = vString[2];
    client.phone = vString[3];
    client.accountBalance = stod(vString[4]);
    return client;
}

std::vector<stClientInfo> loadClientsFromFile(const std::string &fileName)
{
    std::vector<stClientInfo> vClients;
    std::fstream myFile;
    myFile.open(fileName, std::ios::in);
    if (myFile.is_open())
    {
        std::string line;
        while (getline(myFile, line))
        {
            stClientInfo client = convertLineToRecord(line);
            vClients.push_back(client);
        }
        myFile.close();
    }
    return vClients;
}

void printClientInfo(const stClientInfo &client)
{
    std::cout << "| " << std::setw(15) << std::left << client.accountNumber
              << "| " << std::setw(10) << std::left << client.PinCode
              << "| " << std::setw(40) << std::left << client.name
              << "| " << std::setw(12) << std::left << client.phone
              << "| " << std::setw(12) << std::left << client.accountBalance;
}
// 1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111
void showClientsListScreen()
{
    std::vector<stClientInfo> vClients = loadClientsFromFile(fileName);
    std::cout << "\t\t\t\t\tClinet List (" << vClients.size() << ") Client (s).\n";
    std::cout << "\n____________________________________________________________________________________________________________________\n";
    std::cout << "| " << std::setw(15) << std::left << "Account Number"
              << "| " << std::setw(10) << std::left << "Pin Code"
              << "| " << std::setw(40) << std::left << "Client Name"
              << "| " << std::setw(12) << std::left << "Phone"
              << "| " << std::setw(12) << std::left << "Balance";
    std::cout << "\n____________________________________________________________________________________________________________________\n\n";
    for (const stClientInfo &c : vClients)
    {
        printClientInfo(c);
        std::cout << std::endl;
    }
    std::cout << "\n____________________________________________________________________________________________________________________\n";
}

bool findClientByAccountNumber(const std::vector<stClientInfo> &vClients, const std::string &accountNumber, stClientInfo &client)
{
    for (const stClientInfo &c : vClients)
    {
        if (c.accountNumber == accountNumber)
        {
            client = c;
            return true;
        }
    }
    return false;
}
bool findClientByAccountNumber(const std::vector<stClientInfo> &vClients, const std::string &accountNumber)
{
    for (const stClientInfo &c : vClients)
    {
        if (c.accountNumber == accountNumber)
        {
            return true;
        }
    }
    return false;
}

std::string readAccountNumber()
{
    std::string s;
    std::cout << "Please Enter Account Number \n";
    getline(std::cin, s);
    return s;
}

stClientInfo readClientInfo(const std::vector<stClientInfo> &vClients)
{
    stClientInfo client;
    std::cout << "Enter Account Number?";
    getline(std::cin >> std::ws, client.accountNumber);
    while (findClientByAccountNumber(vClients, client.accountNumber))
    {
        std::cout << "Client with [" << client.accountNumber << "] already exists, Enter another Account Number?";
        getline(std::cin, client.accountNumber);
    }
    std::cout << "Enter Pine Code ?";
    getline(std::cin, client.PinCode);
    std::cout << "Enter Name  ?";
    getline(std::cin, client.name);
    std::cout << "Enter Phone ?";
    getline(std::cin, client.phone);
    std::cout << "Enter Account Balance?";
    std::cin >> client.accountBalance;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return client;
}

std::string convertRecordToLine(const stClientInfo &client, const std::string &separator = "#//#")
{
    return client.accountNumber + separator + client.PinCode + separator + client.name + separator + client.phone + separator + std::to_string(client.accountBalance);
}

bool addClient(const ::std::string &fileName, std::vector<stClientInfo> &vClients)
{
    std::fstream myFile;
    stClientInfo client = readClientInfo(vClients);
    vClients.push_back(client);
    myFile.open(fileName, std::ios::app | std::ios::out);
    if (myFile.is_open())
    {

        myFile << convertRecordToLine(client);
        myFile << std::endl;
        myFile.close();
        return true;
    }
    myFile.close();
    return false;
}
// 2222222222222222222222222222222222222222222222222
void addClientsScreen()
{
    std::vector<stClientInfo> vClients = loadClientsFromFile(fileName);
    std::cout << "-------------------------------------------------------------------------------\n";
    std::cout << "\t\t\t\tAdd New Client\n";
    std::cout << "--------------------------------------------------------------------------------\n";
    char addMore = 'y';
    do
    {
        std::cout << "Adding New Client:\n";
        if (addClient(fileName, vClients))
        {
            std::cout << "Client Added Successfully, do you want to add more clients? Y/N?";
            std::cin >> addMore;
        }
        else
        {
            std::cerr << "Error, try again\n";
        }
    } while (tolower(addMore) == 'y');
}

void printClientCard(const stClientInfo &client)
{
    std::cout << "The Following are the client details\n";
    std::cout << "----------------------------------------------------------------\n";
    std::cout << "Account Number   : " << client.accountNumber
              << "\nPin Code       : " << client.PinCode
              << "\nName           : " << client.name
              << "\nPhone          : " << client.phone
              << "\nAccount Balance: " << client.accountBalance;
    std::cout << "\n----------------------------------------------------------------\n";
}

bool saveClientsToFile(const std::string &fileName, const std::vector<stClientInfo> &vClients)
{
    std::fstream myFile;
    myFile.open(fileName, std::ios::out);
    if (myFile.is_open())
    {
        for (const stClientInfo &c : vClients)
        {
            if (c.markToDelete == false)
            {
                myFile << convertRecordToLine(c) << std::endl;
            }
        }
        myFile.close();
        return true;
    }
    myFile.close();
    return false;
}
// 333333333333333333333333333333333333333
void deleteClientByAccountNumberScreen()
{
    std::vector<stClientInfo> vClients = loadClientsFromFile(fileName);
    std::cout
        << "--------------------------------------------------------------------\n";
    std::cout << "\t\t\t Delete Client Screen\n";
    std::cout << "--------------------------------------------------------------------\n\n";
    std::string accountNumber = readAccountNumber();
    stClientInfo client;
    if (findClientByAccountNumber(vClients, accountNumber, client))
    {
        printClientCard(client);
        char confirm = 'y';
        std::cout << "Are you suer you want delete this client? Y/N?";
        std::cin >> confirm;
        if (tolower(confirm) == 'y')
        {
            for (stClientInfo &c : vClients)
            {
                if (c.accountNumber == accountNumber)
                {
                    c.markToDelete = true;
                    break;
                }
            }
        }
        if (saveClientsToFile(fileName, vClients))
        {
            std::cout << "Client Deleted Successfully\n";
        }
        else
        {
            std::cerr << "Error, pla bla pla\n";
        }
    }
    else
    {
        std::cout << "Client with Account Number(" << accountNumber << ") is not found\n";
    }
}

void updateClientInfo(stClientInfo &client)
{
    std::cout << "Enter PinCode? ";
    getline(std::cin >> std::ws, client.PinCode);
    std::cout << "Enter Name? ";
    getline(std::cin, client.name);
    std::cout << "Enter Phone? ";
    getline(std::cin, client.phone);
    std::cout << "Enter Balance? ";
    std::cin >> client.accountBalance;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
// 4444444444444444444444444444444
void updateClientInfoByAccountNumberScreen()
{
    std::vector<stClientInfo> vClients = loadClientsFromFile(fileName);
    std::cout << "---------------------------------------------------------------------------\n";
    std::cout << "\t\t\t Update Client Info Screen\n";
    std::cout << "----------------------------------------------------------------------------\n\n";
    stClientInfo client;
    std::string accountNumber = readAccountNumber();
    if (findClientByAccountNumber(vClients, accountNumber, client))
    {
        printClientCard(client);
        char confirm = 'y';
        std::cout << "Are you sure you want to update this client? Y/N? ";
        std::cin >> confirm;
        if (tolower(confirm) == 'y')
        {
            for (stClientInfo &C : vClients)
            {
                if (C.accountNumber == accountNumber)
                {
                    updateClientInfo(C);
                    break;
                }
            }
            if (saveClientsToFile(fileName, vClients))
            {
                std::cout << "Client Updated Successfully. \n";
            }
        }
    }
    else
    {
        std::cout << "Client with Account Number (" << accountNumber << ") is not found \n";
    }
}
// 5555555
void findClientScreen()
{
    const std::vector<stClientInfo> vClients = loadClientsFromFile(fileName);
    std::cout
        << "---------------------------------------------------------------------------\n";
    std::cout << "\t\t\t Find Client Info Screen\n";
    std::cout << "----------------------------------------------------------------------------\n\n";
    stClientInfo client;
    std::string accountNumber = readAccountNumber();
    if (findClientByAccountNumber(vClients, accountNumber, client))
    {
        printClientCard(client);
    }
    else
    {
        std::cout << "Client with Account Number (" << accountNumber << ") is not found \n";
    }
}

void endProgramScreen()
{
    std::cout << "---------------------------------------------------------------------------\n";
    std::cout << "\t\t\t Program Ends :-)\n";
    std::cout << "----------------------------------------------------------------------------\n\n";
}

void goBackToMainMenu()
{
    std::cout << "Press any key to go back to main menu...";
    std::string line;
    std::getline(std::cin >> std::ws, line);
    system("clear");
    mainMenu();
}

short readNumberBetween(short from, short to)

{
    short num;
    do
    {
        std::cout << "Choose what do you want to do?  [1 to 6]?\n";
        std::cin >> num;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } while (num < from || num > to);
    return num;
}

void mainMenu()
{
    system("clear");
    std::cout << "====================================================================================================\n";
    std::cout << "\t\t\t\tMain Menu Screen\n";
    std::cout << "====================================================================================================\n";
    std::cout << "\t[1] Show Client List.\n";
    std::cout << "\t[2] Add New Client.\n";
    std::cout << "\t[3] Delete Client.\n";
    std::cout << "\t[4] Update Client.\n";
    std::cout << "\t[5] Find Client.\n";
    std::cout << "\t[6] Exit.\n";
    std::cout << "====================================================================================================\n";
    performMainMenuOption((enMainMenuOption)readNumberBetween(1, 6));
}

void performMainMenuOption(enMainMenuOption option)
{
    switch (option)
    {
    case enMainMenuOption::eShowClientList:
        system("clear");
        showClientsListScreen();
        goBackToMainMenu();

        break;
    case enMainMenuOption::eAddNewClient:
    {
        system("clear");
        addClientsScreen();
        goBackToMainMenu();
        break;
    }
    case enMainMenuOption::eDeleteClient:
    {
        system("clear");
        deleteClientByAccountNumberScreen();
        goBackToMainMenu();
        break;
    }
    case enMainMenuOption::eUpdateClient:
    {
        system("clear");
        updateClientInfoByAccountNumberScreen();
        goBackToMainMenu();
        break;
    }
    case enMainMenuOption::eFindClient:
    {
        system("clear");
        findClientScreen();
        goBackToMainMenu();
        break;
    }
    case enMainMenuOption::eEndProgram:
    {
        system("clear");
        endProgramScreen();
        break;
    }
    default:
        break;
    }
}

struct stUserInfo
{
    std::string userName;
    std::string password;
    int permissions;
};
stUserInfo convertLineUserToRecord(const std::string &line, std::string separator = "#//#")
{
    stUserInfo user;
    std::vector<std::string> vUserString = vSplit(line, separator);
    user.userName = vUserString[0];
    user.password = vUserString[1];
    user.permissions = std::stoi(vUserString[2]);
    return user;
};
std::vector<stUserInfo> loadUserFromFile(const std::string &userFile)
{
    std::vector<stUserInfo> vUsersInfo;
    std::fstream myfile;
    myfile.open(userFile, std::ios::in);
    if (myfile.is_open())
    {
        std::string line;

        while (getline(myfile, line))
        {
            stUserInfo user = convertLineUserToRecord(line);
            vUsersInfo.push_back(user);
        }
        myfile.close();
    }
    return vUsersInfo;
}
// login screen function () {check two value username and password}
//  function to check the value and return true or false if true go to main menu of false print invalid username/password! the ask to reenter the user name and password
//  I need to convert line to record
bool checkUser(std::string username, std::string password)
{
    std::vector<stUserInfo> vUsers = loadUserFromFile(userFile);
    for (stUserInfo &user : vUsers)
    {

        if (user.userName == username)
        {
            if (user.password == password)
            {
                return true;
            }
            return false;
        }
    }

    return false;
}
void loginScreen()
{

    bool isValid = true;
    do
    {
        std::cout << "---------------------------------------------------------\n";
        std::cout << "                     Login Screen                        \n";
        std::cout << "---------------------------------------------------------\n";
        if (!isValid)
        {
            std::cout << "invalid Username/Password!\n";
        }
        std::string username, password;
        std::cout << "Enter Username?   ";
        std::getline(std::cin >> std::ws, username);
        std::cout << "Enter Password?   ";
        std::getline(std::cin, password);
        isValid = checkUser(username, password);
        // I must do trim for the inputs before checking
        // also later I must refactor this function
        if (isValid)
        {
            mainMenu();
        }
        else
        {
            system("clear");
        }
    } while (!isValid);
}

void printUserCard(stUserInfo user)
{
    std::cout << "| " << std::setw(30) << std::left << user.userName << " | " << std::setw(15) << user.password << " | " << std::setw(5) << user.permissions << std::endl;
}
void userListScreen()
{
    std::vector<stUserInfo> vUser = loadUserFromFile(userFile);

    std::cout << "                    Users List (" << vUser.size() << ") User(s)./n";
    std::cout << "\n____________________________________________________________________________________________________________________\n";
    std::cout << "| " << std::setw(30) << std::left << "User Name" << " | " << std::setw(15) << "Password" << " | " << std::setw(5) << "Permissions" << std::endl;

    std::cout << "\n____________________________________________________________________________________________________________________\n";
    for (stUserInfo &user : vUser)
    {
        printUserCard(user);
    }
    std::cout << "\n\n____________________________________________________________________________________________________________________\n";
    // goBackToManageUserMainMenu
}
int main()
{
    userListScreen();
    // loginScreen();
}