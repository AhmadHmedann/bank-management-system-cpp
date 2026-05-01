#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <cctype>
#include <cstdlib>


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
struct stUserInfo
{
    std::string userName;
    std::string password;
    int permissions;
    bool markToDelete = false;
};

stUserInfo currentUser;
enum enMainMenuOption
{
    eShowClientList = 1,
    eAddNewClient = 2,
    eDeleteClient = 3,
    eUpdateClient = 4,
    eFindClient = 5,
    eManageUserMenu = 6,
    eEndProgram = 7,
};
enum enMainMenuPermissions
{
    eAll = -1,
    pShowClientList = 1,
    pAddClientScreen = 2,
    pDeleteClient = 4,
    pUpdateClient = 8,
    pFindClient = 16,
    pManageUser = 32,
};
enum enManageUsersMenuOption
{
    eShowUserList = 1,
    eAddNewUser = 2,
    eDeleteUser = 3,
    eUpdateUser = 4,
    eFindUser = 5,
    eMainMenu = 6
};

void mainMenu();
void ManageUsersMenuScreen();
void DeniedScreen();
void loginScreen();
void performMainMenuOption(enMainMenuOption);
bool checkAccesspermission(enMainMenuPermissions);

short readNumberBetween(short from, short to)

{
    short num;
    do
    {
        std::cout << "Choose what do you want to do?  [" << from << " to " << to << "]?\n";
        std::cin >> num;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } while (num < from || num > to);
    return num;
}
std::string readAccountNumber()
{
    std::string s;
    std::cout << "Please Enter Account Number \n";
    getline(std::cin, s);
    return s;
}
std::string ReadUsername()
{
    std::string s;
    std::cout << "Please Enter username \n";
    getline(std::cin >> std::ws, s);
    return s;
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
    if (vString.size() != 5)
    {
        throw std::runtime_error("Invalid client record");
    }
    client.accountNumber = vString[0];
    client.PinCode = vString[1];
    client.name = vString[2];
    client.phone = vString[3];
    client.accountBalance = std::stod(vString[4]);
    return client;
}
stUserInfo convertLineUserToRecord(const std::string &line, std::string separator = "#//#")
{
    stUserInfo user;
    std::vector<std::string> vUserString = vSplit(line, separator);
    if (vUserString.size() != 3)
    {
        throw std::runtime_error("Invalid user record");
    }
    user.userName = vUserString[0];
    user.password = vUserString[1];
    user.permissions = std::stoi(vUserString[2]);
    return user;
};
std::string convertRecordToLine(const stClientInfo &client, const std::string &separator = "#//#")
{
    return client.accountNumber + separator + client.PinCode + separator + client.name + separator + client.phone + separator + std::to_string(client.accountBalance);
}
std::string convertUserRecordToLine(stUserInfo user, std::string separator = "#//#")
{
    return user.userName + separator + user.password + separator + std::to_string(user.permissions);
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
std::vector<stUserInfo> loadUserFromFile(const std::string &userFile)
{
    std::vector<stUserInfo> vUsersInfo;
    std::fstream myfile;
    myfile.open(userFile, std::ios::in);
    if (myfile.is_open())
    {
        std::string line;
        stUserInfo user;
        while (getline(myfile, line))
        {
            user = convertLineUserToRecord(line);
            vUsersInfo.push_back(user);
        }
        myfile.close();
    }
    return vUsersInfo;
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

void printClientInfo(const stClientInfo &client)
{
    std::cout << "| " << std::setw(15) << std::left << client.accountNumber
              << "| " << std::setw(10) << std::left << client.PinCode
              << "| " << std::setw(40) << std::left << client.name
              << "| " << std::setw(12) << std::left << client.phone
              << "| " << std::setw(12) << std::left << client.accountBalance;
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
void printUserCard(stUserInfo user)
{
    std::cout << "The Following are the user details\n";
    std::cout << "----------------------------------------------------------------\n";
    std::cout << "User Name   : " << user.userName
              << "\nPassword    : " << user.password
              << "\nPermissions : " << user.permissions;
    std::cout << "\n----------------------------------------------------------------\n";
}
void printUserInfo(stUserInfo user)
{
    std::cout << "| " << std::setw(30) << std::left << user.userName << " | " << std::setw(15) << user.password << " | " << std::setw(5) << user.permissions << std::endl;
}
void DeniedScreen()
{
    system("clear");
    std::cout << "------------------------------------------------------------\n";
    std::cout << "Access Denied,\nYou Do not have Permission To Do This\nPlease contact With Your Admin.\n";
    std::cout << "------------------------------------------------------------\n";
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
bool saveUsersToFile(std::string userFile, std::vector<stUserInfo> &vUser)
{
    std::fstream myfile;
    std::string line;
    myfile.open(userFile, std::ios::out);
    if (myfile.is_open())
    {
        for (const stUserInfo &user : vUser)
        {
            if (!user.markToDelete)
            {
                myfile << convertUserRecordToLine(user) << std::endl;
            }
        }
        myfile.close();
        return true;
    }
    myfile.close();
    return false;
}

void showClientsListScreen()
{
    if (!checkAccesspermission(enMainMenuPermissions::pShowClientList))
    {
        DeniedScreen();
        return;
    }
    std::vector<stClientInfo> vClients = loadClientsFromFile(fileName);
    std::cout << "\t\t\t\t\tClient List (" << vClients.size() << ") Client (s).\n";
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
void addClientsScreen()
{
    if (!checkAccesspermission(enMainMenuPermissions::pAddClientScreen))
    {
        DeniedScreen();
        return;
    }
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
            std::cout << "\nClient Added Successfully, do you want to add more clients? Y/N?";
            std::cin >> addMore;
        }
        else
        {
            std::cerr << "Error, try again\n";
        }
    } while (tolower(addMore) == 'y');
}
void deleteClientByAccountNumberScreen()
{
    if (!checkAccesspermission(enMainMenuPermissions::pDeleteClient))
    {
        DeniedScreen();
        return;
    }
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
void updateClientInfoByAccountNumberScreen()
{
    if (!checkAccesspermission(enMainMenuPermissions::pUpdateClient))
    {
        DeniedScreen();
        return;
    }
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
void findClientScreen()
{
    if (!checkAccesspermission(enMainMenuPermissions::pFindClient))
    {
        DeniedScreen();
        return;
    }
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
    std::cout << "\n\nPress any key to go back to main menu...";
    std::string line;
    std::getline(std::cin >> std::ws, line);
    system("clear");
    mainMenu();
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
    std::cout << "\t[6] Manage Users Menu.\n";
    std::cout << "\t[7] Logout.\n";
    std::cout << "====================================================================================================\n";
    performMainMenuOption((enMainMenuOption)readNumberBetween(1, 7));
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
    case enMainMenuOption::eManageUserMenu:
    {
        ManageUsersMenuScreen();
        goBackToMainMenu(); // if I delete that and no permission fo manage user I think it will end the program
        // if I added go back to main menu screen in Denied screen that will effect the call stack I will do debug later for both  cases
        break;
    }
    case enMainMenuOption::eEndProgram:
    {
        system("clear");
        loginScreen();
        break;
    }
    default:
        break;
    }
}

int setUserPermissions()
{
    int permissions = 0;
    char answer = 'n';
    std::cout << "Do you want to give full access? Y/N? ";
    std::cin >> answer;
    if (tolower(answer) == 'y')
    {
        return enMainMenuPermissions::eAll;
    }
    std::cout << "\nDo you want to give access to : \n";

    std::cout << "Show Client List? Y/N? ";
    std::cin >> answer;
    if (tolower(answer) == 'y')
    {
        permissions += enMainMenuPermissions::pShowClientList;
    }
    std::cout << "Add new Client? Y/N? ";
    std::cin >> answer;
    if (tolower(answer) == 'y')
    {
        permissions += enMainMenuPermissions::pAddClientScreen;
    }
    std::cout << "Delete Client? Y/N? ";
    std::cin >> answer;
    if (tolower(answer) == 'y')
    {
        permissions += enMainMenuPermissions::pDeleteClient;
    }
    std::cout << "Update Client? Y/N? ";
    std::cin >> answer;
    if (tolower(answer) == 'y')
    {
        permissions += enMainMenuPermissions::pUpdateClient;
    }
    std::cout << "find Client? Y/N? ";
    std::cin >> answer;
    if (tolower(answer) == 'y')
    {
        permissions += enMainMenuPermissions::pFindClient;
    }
    std::cout << "Manage User Menu? Y/N? ";
    std::cin >> answer;
    if (tolower(answer) == 'y')
    {
        permissions += enMainMenuPermissions::pManageUser;
    }
    return permissions;
}
bool checkAccesspermission(enMainMenuPermissions permission)
{
    if (currentUser.permissions == enMainMenuPermissions::eAll)
    {
        return true;
    }
    else if ((permission & currentUser.permissions) == permission)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool checkUser(std::string username, std::string password, stUserInfo &User)
{
    std::vector<stUserInfo> vUsers = loadUserFromFile(userFile);
    for (stUserInfo &user : vUsers)
    {

        if (user.userName == username)
        {
            if (user.password == password)
            {
                User = user;
                return true;
            }
            return false;
        }
    }

    return false;
}
bool findUser(std::string username, stUserInfo &user)
{
    std::vector<stUserInfo> vUsers = loadUserFromFile(userFile);
    for (stUserInfo &u : vUsers)
    {
        if (u.userName == username)
        {
            user = u;
            return true;
        }
    }
    return false;
}
bool findUser(const std::string username)
{
    std::vector<stUserInfo> vUsers = loadUserFromFile(userFile);
    for (stUserInfo &u : vUsers)
    {
        if (u.userName == username)
        {
            return true;
        }
    }
    return false;
}
stUserInfo ReadUserInfo()
{
    stUserInfo user;

    user.userName = ReadUsername();
    while (findUser(user.userName))
    {
        std::cout << "User with [" << user.userName << "] is already exists, Enter another user name? ";
        std::cin >> user.userName;
    }
    std::cout << "Enter Password?   ";
    std::getline(std::cin >> std::ws, user.password);
    user.permissions = setUserPermissions();
    return user;
}
bool AddUserToFile(const std::string &userFile, std::vector<stUserInfo> &vUsers, stUserInfo user)
{

    vUsers.push_back(user);
    std::fstream myfile;
    myfile.open(userFile, std::ios::app | std::ios::out);
    if (myfile.is_open())
    {
        myfile << convertUserRecordToLine(user) << std::endl;
        myfile.close();
        return true;
    }
    myfile.close();
    return false;
}
stUserInfo UpdateUserInfo(std::string username)
{
    stUserInfo user;
    user.userName = username;
    std::cout << "Enter Password?   ";
    std::getline(std::cin, user.password);
    user.permissions = setUserPermissions();
    return user;
}

void loginScreen()
{

    bool isValid = true;
    std::string username, password;
    do
    {
        system("clear");
        std::cout << "---------------------------------------------------------\n";
        std::cout << "                     Login Screen                        \n";
        std::cout << "---------------------------------------------------------\n";
        if (!isValid)
        {
            std::cout << "invalid Username/Password!\n";
        }

        std::cout << "Enter Username?   ";
        std::getline(std::cin >> std::ws, username);
        std::cout << "Enter Password?   ";
        std::getline(std::cin, password);
        isValid = checkUser(username, password, currentUser);
        // I must do trim for the inputs before checking
        // also later I must refactor this function
    } while (!isValid);
    mainMenu();
}
void deleteUserByUsernameScreen()
{

    std::vector<stUserInfo> vUsers = loadUserFromFile(userFile);

    std::cout
        << "--------------------------------------------------------------------\n";
    std::cout << "\t\t\t Delete User Screen\n";
    std::cout << "--------------------------------------------------------------------\n\n";
    stUserInfo user;
    std::string username = ReadUsername();

    if (username == "Admin" || (username == currentUser.userName))
    {
        std::cout << "\n\nYou cannot Delete This User.";
        return;
    }
    if (findUser(username, user))
    {
        printUserCard(user);
        char confirm = 'n';
        std::cout << "Are you sure you want delete this User? Y/N? ";
        std::cin >> confirm;
        if (tolower(confirm) == 'y')
        {
            for (stUserInfo &U : vUsers)
            {
                if (U.userName == username)
                {
                    U.markToDelete = true;
                    break;
                }
            }
            if (saveUsersToFile(userFile, vUsers))
            {
                std::cout << "User deleted successfully.\n";
            }
            else
            {
                std::cerr << "Error, Delete User goes wrong\n";
            }
        }
    }
    else
    {
        std::cout << "User with Username (" << username << ") is not found!\n";
    }
}
void userListScreen()
{
    std::vector<stUserInfo> vUser = loadUserFromFile(userFile);

    std::cout << "                    Users List (" << vUser.size() << ") User(s).\n";
    std::cout << "\n____________________________________________________________________________________________________________________\n";
    std::cout << "| " << std::setw(30) << std::left << "User Name" << " | " << std::setw(15) << "Password" << " | " << std::setw(5) << "Permissions" << std::endl;

    std::cout << "\n____________________________________________________________________________________________________________________\n";
    if (vUser.size() == 0)
    {
        std::cout << "\t\t\t No Users Available In The System!";
    }
    else
    {
        for (const stUserInfo &user : vUser)
        {
            printUserInfo(user);
        }
        std::cout << "\n\n____________________________________________________________________________________________________________________\n";
    }
}
void findUserScreen()
{

    std::vector<stUserInfo> vUsers = loadUserFromFile(userFile);

    std::cout
        << "--------------------------------------------------------------------\n";
    std::cout << "\t\t\t Find User Screen\n";
    std::cout << "--------------------------------------------------------------------\n\n";
    stUserInfo user;
    std::string username = ReadUsername();
    if (findUser(username, user))
    {
        printUserCard(user);
    }
    else
    {
        std::cout << "User with Username (" << username << ") is not found!\n";
    }
}
void GoBackToManageUsersMenu()
{
    std::cout << "\n\nPress any key to go back to Manage Users Menu...\n";
    std::string s;
    getline(std::cin >> std::ws, s);
    ManageUsersMenuScreen();
}
void AddUsersScreen()
{
    std::vector<stUserInfo> vUsers = loadUserFromFile(userFile);
    char addMore = 'y';
    do
    {
        std::cout
            << "--------------------------------------------------------------------\n";
        std::cout << "\t\t\t Add User Screen\n";
        std::cout << "--------------------------------------------------------------------\n\n";
        stUserInfo user = ReadUserInfo();
        AddUserToFile(userFile, vUsers, user);
        std::cout << "\nDo you want to add more Users? Y/N?";
        std::cin >> addMore;

    } while (tolower(addMore) == 'y');
}
void UpdateUserScreen()
{

    std::cout
        << "--------------------------------------------------------------------\n";
    std::cout << "\t\t\t update User Screen\n";
    std::cout << "--------------------------------------------------------------------\n\n";

    std::vector<stUserInfo> vUsers = loadUserFromFile(userFile);
    std::string username = ReadUsername();
    stUserInfo user;
    if (username == "Admin" || (username == currentUser.userName))
    {
        std::cout << "\n\nYou cannot Update This User.";
        return;
    }
    if (findUser(username, user))
    {
        printUserCard(user);
        char confirm = 'n';
        std::cout << "Are you sure you want update this User? Y/N? ";
        std::cin >> confirm;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (tolower(confirm) == 'y')
        {
            for (stUserInfo &u : vUsers)
            {
                if (u.userName == username)
                {
                    u = UpdateUserInfo(username);
                    break;
                }
            }
            if (saveUsersToFile(userFile, vUsers))
            {

                std::cout << "User updated successfully.\n";
            }
            else
            {
                std::cerr << "Error, update User goes wrong\n";
            }
        }
    }
    else
    {
        std::cout << "User with Username (" << username << ") is not found!\n";
    }
}
void PerformManageUsersMenu(enManageUsersMenuOption option)
{
    switch (option)
    {
    case enManageUsersMenuOption::eShowUserList:
        system("clear");
        userListScreen();
        GoBackToManageUsersMenu();
        break;
    case enManageUsersMenuOption::eDeleteUser:
        system("clear");
        deleteUserByUsernameScreen();
        GoBackToManageUsersMenu();
        break;
    case enManageUsersMenuOption::eAddNewUser:
        system("clear");
        AddUsersScreen();
        GoBackToManageUsersMenu();
        break;
    case enManageUsersMenuOption::eFindUser:
        system("clear");
        findUserScreen();
        GoBackToManageUsersMenu();
        break;
    case enManageUsersMenuOption::eUpdateUser:
        system("clear");
        UpdateUserScreen();
        GoBackToManageUsersMenu();
        break;
    case enManageUsersMenuOption::eMainMenu:

        mainMenu();
        break;
    default:
        break;
    }
}
void ManageUsersMenuScreen()
{
    if (!checkAccesspermission(enMainMenuPermissions::pManageUser))
    {
        DeniedScreen();
        // here add go back to main menu
        return;
    }
    system("clear");
    std::cout << "====================================================================================================\n";
    std::cout << "\t\t\t\tManage Users Menu Screen\n";
    std::cout << "====================================================================================================\n";
    std::cout << "\t[1] Show Users List.\n";
    std::cout << "\t[2] Add New User(s).\n";
    std::cout << "\t[3] Delete User.\n";
    std::cout << "\t[4] Update User.\n";
    std::cout << "\t[5] Find User.\n";
    std::cout << "\t[6] Main Menu.\n";
    std::cout << "====================================================================================================\n";
    PerformManageUsersMenu((enManageUsersMenuOption)readNumberBetween(1, 6));
}

// permissions: I need a denied screen(with back to main menu ... ) and I need function to check permissions the input fot that function should be enum based on what screen user choose
//  and the output will be true if he allowed or false if he don't have a permission fot that choose

// something I did wrong I put goBackToMainMenu inside DeniedScreen that mean I  create infinity of call stack loop Just for remember> I must user return In each choose and that will solve the issue this bug case me two hours.
int main()
{
    loginScreen();
}