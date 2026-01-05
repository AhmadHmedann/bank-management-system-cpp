for all 
struct for client Info
Main Menu function

first I will do Show Client List.
1. load clients form file into vector
1_1. convert line to vector
1.1.1. Split the string
2. Print client info e.x. | A150    | 1234      | Ahmad Hmedan   | 074801544444 | 500
3. Print all clients
done

second Add New Client
1. find client by account number (I have vector off all clients already)
1.1. read String function 
2. Enter Client info
3. Add new client (save to file)
3.1. readClientInfo
3.2. convert record to line

Third Delete Client Screen 
0. find a Client by Account Number
0.1 add mark to delete into stClinetInfo
1. Print Client card
2. save  clients to file after deleting 
 
Fourth Update Client Info
0. fid client by account number
0.1. print client card
0.2 save client after updating
1. Update Client Info

Finally I want a function to gather all Function
maybe switch will solve this issue
read number between 1 to 6

what I did to solve that issue
create enum contain the option 
performMainMen function only to do the switch depend on the argument  which is the enum (option);
goBackToMainMenu function to clear the screen and show mainMenu
mainMenu function show the fist screen and performMainMenu

                Bank extension
                 
