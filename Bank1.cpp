#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>

const std::string fileName = "HmedanBank.txt";
struct stClientInfo
{
    std::string accountNumber;
    std::string name;
    std::string phone;
    std::string PinCode;
    double accountBalance = 0;
};

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
    std::cout << "| " << std::setw(10) << std::left << client.accountNumber
              << "| " << std::setw(8) << std::left << client.PinCode
              << "| " << std::setw(20) << std::left << client.name
              << "| " << std::setw(12) << std::left << client.phone
              << "| " << std::setw(20) << std::left << client.accountBalance;
}

void showClientsList(const std::vector<stClientInfo> &vClients)
{
    std::cout << "\t\t\t\t\tClinet List (" << vClients.size() << ") Client (s).\n";
    std::cout << "\n____________________________________________________________________________________________________________________\n";
    std::cout << "| " << std::setw(10) << std::left << "Account Number"
              << "| " << std::setw(8) << std::left << "Pin Code"
              << "| " << std::setw(20) << std::left << "Client Name"
              << "| " << std::setw(12) << std::left << "Phone"
              << "| " << std::setw(20) << std::left << "Balance";
    std::cout << "\n____________________________________________________________________________________________________________________\n\n";
    for (const stClientInfo &c : vClients)
    {
        printClientInfo(c);
        std::cout << std::endl;
    }
    std::cout << "\n____________________________________________________________________________________________________________________\n";
}
int main()
{
    std::vector<stClientInfo> vClients = loadClientsFromFile(fileName);
    showClientsList(vClients);
}