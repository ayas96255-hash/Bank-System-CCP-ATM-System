#include <iostream>
#include<string>
#include<vector>
#include<cctype>
#include<iomanip>
#include<algorithm>
#include<fstream>
using namespace std;
enum enMainMenuePermissions {
	eAll = -1,
	pListClients = 1,
	pAddNewClients = 2,
	pDeleteClients = 4,
	pUpdateClients = 8,
	pFindClients = 16,
	pTranactions = 32,
	pManageUsers = 64
};
struct sUser
{
	string UserName;
	string Password;
	int Permissions = 0;
	bool MarkForDelete = false;
};
sUser CurrentUser;

const string ClientsFileName = "Client.txt";
string FileUsersName = "Users.txt";
void ShowMainMenuScreen();
void ShowTransactionMenuScreen();
void ShowManageUsersScreen();
void Login();
bool CheckAccessPermission(enMainMenuePermissions Permission, sUser User);
void ShowAccessDeniedMessage();

struct sClient
{
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	float AccountBalance;
	bool MarkForDelete = false;

};
vector<string> SplitString(string S1, string Delim)
{
	string sWord;
	short pos = 0;
	int count = 0;
	vector<string>vString;
	while ((pos = S1.find(Delim)) != std::string::npos)
	{
		sWord = S1.substr(0, pos);
		if (sWord != "")
		{
			vString.push_back(sWord);
			count++;
		}
		S1.erase(0, pos + Delim.length());
	}
	if (S1 != Delim)
	{
		vString.push_back(S1);
		count++;
	}
	//cout << "Tokens: " << count << endl;
	return vString;
}
string TrimLeft(string S1)
{
	for (int i = 0;i < S1.length();i++)
	{
		if (S1[i] != ' ')
		{
			return S1.substr(i, S1.length() - i);
		}
	}
	return "";
}
string TrimRight(string S1)
{
	for (int i = S1.length() - 1;i >= 0;i--)
	{
		if (S1[i] != ' ')
		{
			return 	S1.substr(0, i + 1);

		}
	}
	return "";
}
string Trim(string S1)
{
	return TrimLeft(TrimRight(S1));
}
string ConvertRecordToLine(sClient Client, string Separator = "#//#")
{
	string stClientRecord = "";
	stClientRecord += Client.AccountNumber + Separator;
	stClientRecord += Client.PinCode + Separator;
	stClientRecord += Client.Name + Separator;
	stClientRecord += Client.Phone + Separator;
	stClientRecord += to_string(Client.AccountBalance);
	return stClientRecord;
}
sClient ConvertLineToRecord(string S1, string Separator = "#//#")
{
	sClient Client;
	vector<string> vClient = SplitString(S1, Separator);
	if (vClient.size() >= 5)
	{
		Client.AccountNumber = vClient[0];
		Client.PinCode = vClient[1];
		Client.Name = vClient[2];
		Client.Phone = vClient[3];
		Client.AccountBalance = stod(vClient[4]);
	}
	return Client;
}
bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{
	vector<sClient>vClient;
	fstream client;
	client.open(ClientsFileName, ios::in);
	if (client.is_open())
	{
		string Line;
		sClient Client;
		while (getline(client, Line))
		{
			Client = ConvertLineToRecord(Line);
			if (Client.AccountNumber == AccountNumber)
			{
				client.close();
				return true;
			}
			client.close();
		}
	}
	return false;
}
sClient ReadeNewClient()
{
	sClient Client;
	cout << "\nPlease Enter Client Data:\n";
	cout << "Enter Account Number? ";
	getline(cin >> ws, Client.AccountNumber);
	while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
	{
		cout << "\nClient with [" << Client.AccountNumber << "] already exists,Enter another Account Number: ";
		getline(cin >> ws, Client.AccountNumber);
	}
	cout << "Enter PinCode? ";
	getline(cin, Client.PinCode);
	cout << "Enter Name? ";
	getline(cin, Client.Name);
	cout << "Enter Phone? ";
	getline(cin, Client.Phone);
	cout << "Enter Account Balance? ";
	cin >> Client.AccountBalance;

	return Client;
}
void PrintClient(sClient Client)
{
	cout << "\n-----------------------------------------------------";
	cout << "\nThe following is the extracted client record:\n";
	cout << "Account Number : " << Client.AccountNumber << endl;
	cout << "PinCode        : " << Client.PinCode << endl;
	cout << "Name           : " << Client.Name << endl;
	cout << "Phone          : " << Client.Phone << endl;
	cout << "Account Balance: " << Client.AccountBalance << endl;
	cout << "-----------------------------------------------------\n";
}
void AddDataLineToFile(string FileName, string stDataLine)
{
	fstream client;
	client.open(ClientsFileName, ios::out | ios::app);
	if (client.is_open())
	{
		client << stDataLine << endl;
		client.close();
	}
}
void AddNewClient()
{
	sClient Client = ReadeNewClient();
	AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}
void AddClients()
{
	char AddClient = 'y';
	do
	{
		
		cout << "\nAdding New Client:\n";
		AddNewClient();
		cout << "\nClient Added Successfully, do you want more clients? ";
		cin >> AddClient;
	} while (tolower(AddClient) == 'y');
}
void PrintClientRecord(sClient Client)
{
	cout << "| " << left << setw(15) << Client.AccountNumber;
	cout << "| " << left << setw(20) << Client.PinCode;
	cout << "| " << left << setw(30) << Client.Name;
	cout << "| " << left << setw(20) << Client.Phone;
	cout << "| " << left << setw(15) << Client.AccountBalance << endl;
}
void PrintAllClientsData(vector<sClient>vClient)
{
	cout << "\n\t\t\t\t\t" << "Client List ( " << vClient.size() << " ) Client(s)\t";
	cout << "\n________________________________________________________________________________________________________\n";
	cout << "| " << left << setw(15) << "Account Number ";
	cout << "| " << left << setw(20) << "PinCode ";
	cout << "| " << left << setw(30) << "Name ";
	cout << "| " << left << setw(20) << "Phone ";
	cout << "| " << left << setw(15) << "Account Balance ";
	cout << "\n________________________________________________________________________________________________________\n";
	for (sClient& Client : vClient)
	{
		PrintClientRecord(Client);
	}
	cout << "\n________________________________________________________________________________________________________\n";
}
vector<sClient> LoadClientsDataFromFile(string FileName)
{
	sClient Client;
	vector<sClient>vClient;
	fstream client;
	client.open(ClientsFileName, ios::in);
	string Line;
	if (client.is_open())
	{
		while (getline(client, Line))
		{
			if (Line == " ")
				continue;
			Client = ConvertLineToRecord(Line);
			vClient.push_back(Client);
		}
		client.close();
	}
	return vClient;
}
string ReadClientAccountNumber()
{
	string AccountNumber;
	cout << "\nPlease Enter  Account Number:";
	cin >> AccountNumber;
	return AccountNumber;
}
bool FindClientByAccountNumber(string AccountNumber, vector<sClient>& vClient, sClient& Client)
{
	//vClient = LoadClientsDataFromFile(ClientsFileName);
	for (sClient C : vClient)
	{

		if (Trim(C.AccountNumber) == AccountNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;

}
bool MarkClintForDeleteByAccountNumber(string AccountNumber, vector<sClient>& vClient)
{

	for (sClient& C : vClient)
	{
		if (Trim(C.AccountNumber) == AccountNumber)
		{
			C.MarkForDelete = true;
			return true;
		}

	}
	return false;
}
vector<sClient>SaveClientsDataToFile(string FileName, vector<sClient>& vClient)
{

	fstream client;
	string Line;

	client.open(ClientsFileName, ios::out);
	if (client.is_open())
	{
		for (sClient& C : vClient)
		{
			if (C.MarkForDelete == false)
			{
				C.AccountNumber = Trim(C.AccountNumber);
				C.Name = Trim(C.Name);
				Line = ConvertRecordToLine(C);
				client << Line << endl;
			}
		}
		client.close();
	}
	return vClient;
}
bool DeleteClientByAccountNumber(string AccountNumber, vector<sClient>& vClient)
{
	sClient Client;
	char Answer = 'n';
	string DataLine;

	if (FindClientByAccountNumber(AccountNumber, vClient, Client))
	{
		PrintClient(Client);
		cout << "\nAre you sure you want delete this client? ";
		cin >> Answer;
		if (tolower(Answer) == 'y')
		{
			MarkClintForDeleteByAccountNumber(AccountNumber, vClient);
			SaveClientsDataToFile(ClientsFileName, vClient);
			vClient = LoadClientsDataFromFile(ClientsFileName);
			cout << "\n\nClient Delete Successfully. ";
			return true;
		}
	}
	else
	{
		cout << "\n\nClient with Account Number (" << AccountNumber << ") Not Found!";
		return false;
	}
}
sClient ChangeClientRecord(string AccountNumber)
{
	sClient C;
	C.AccountNumber = Trim(AccountNumber);
	cout << "\nEnter PinCode: ";
	getline(cin >> ws, C.PinCode);
	C.PinCode = Trim(C.PinCode);
	cout << "\nEnter Name: ";
	getline(cin, C.Name);
	C.Name = Trim(C.Name);
	cout << "\nEnter Phone: ";
	getline(cin, C.Phone);
	cout << "\nEnter Account Balance: ";
	cin >> C.AccountBalance;
	return C;

}
bool UpdateClientByAccountNumber(string AccountNumber, vector<sClient>& vClient)
{
	sClient Client;
	char Answer = 'n';
	string DataLine;

	if (FindClientByAccountNumber(AccountNumber, vClient, Client))
	{
		PrintClient(Client);
		cout << "\nAre you sure you want update this client? ";
		cin >> Answer;
		if (tolower(Answer) == 'y')
		{
			for (sClient& C : vClient)
			{
				if (Trim(C.AccountNumber) == AccountNumber)
				{
					C = ChangeClientRecord(AccountNumber);
					break;
				}
			}
			SaveClientsDataToFile(ClientsFileName, vClient);
			vClient = LoadClientsDataFromFile(ClientsFileName);
			cout << "\n\nClient Update Successfully. ";
			return true;
		}
	}
	else
	{
		cout << "\n\nClient with Account Number (" << AccountNumber << ") Not Found!";
		return false;
	}
}
void GoBackToMainMenu()
{
	cout << "\nPress any key to go back to main menu...";
	system("pause>nul");
	ShowMainMenuScreen();
}
void BackToManageMenu()
{
	cout << "\nPress any key to go back to main menu...";
	system("pause>nul");
	ShowManageUsersScreen();
}
void ShowClientList()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pListClients, CurrentUser))
	{
		ShowAccessDeniedMessage();
		return;
	}
    vector<sClient> vClient=LoadClientsDataFromFile(ClientsFileName);
	PrintAllClientsData(vClient);
}
void ShowAddNewClients()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pAddNewClients, CurrentUser))
	{
		ShowAccessDeniedMessage();
		return;
	}
	cout << "\n-----------------------------------------------\n";
	cout << "\t Add New Clients Screen ";
	cout << "\n-----------------------------------------------\n";
	AddClients();
}
void ShowDeleteClient()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pDeleteClients, CurrentUser))
	{
		ShowAccessDeniedMessage();
		return;
	}
	cout << "\n-----------------------------------------------\n";
	cout << "\t Delete Client Screen ";
	cout << "\n-----------------------------------------------\n";
	vector<sClient> vClient = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = Trim(ReadClientAccountNumber());
	DeleteClientByAccountNumber(AccountNumber, vClient);
}
void ShowUpdateClient()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pUpdateClients, CurrentUser))
	{
		ShowAccessDeniedMessage();
		return;
	}
	cout << "\n-----------------------------------------------\n";
	cout << "\t Update Client Screen ";
	cout << "\n-----------------------------------------------\n";
	vector<sClient> vClient = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = Trim(ReadClientAccountNumber());
	UpdateClientByAccountNumber(AccountNumber, vClient);
}
void ShowFindClient()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pFindClients, CurrentUser))
	{
		ShowAccessDeniedMessage();
		return;
	}
	cout << "\n-----------------------------------------------\n";
	cout << "\t Find Client Screen ";
	cout << "\n-----------------------------------------------\n";
	vector<sClient> vClient = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = Trim(ReadClientAccountNumber());
	sClient Client;
	if (FindClientByAccountNumber(AccountNumber, vClient, Client))
		PrintClient(Client);
	else
		cout << "\n\nClient with Account Number (" << AccountNumber << ") Not Found!";
}
void ShowExit()
{
	cout << "\n-----------------------------------------------\n";
	cout << "\t Program ends :). ";
	cout << "\n-----------------------------------------------\n";
}
void ShowLogin()
{
	Login();
}
enum enMainMenuOptions{eListClients=1,eAddNewClient=2,eDeletClient=3,eUpdateClient=4,eFindClient=5,eTrensaction=6,eManageUsers =7,eLogout=8};
enum enTransactionOption{eDeposit=1,eWithdraw=2,eTotalBalances=3,eMainMenu=4 };
bool DepositBalanceToClientByAccountNumber(string AccountNumber,float Amount, vector<sClient>vClient)
{
	char Answer = 'n';
	cout << "\nAre you sure you want preform this transaction? y/n ? ";
	cin >> Answer;
	if (tolower(Answer) == 'y')
	{
		for (sClient& C : vClient)
		{
			if (Trim(C.AccountNumber)==AccountNumber)
			{
				C.AccountBalance += Amount;
				SaveClientsDataToFile(ClientsFileName, vClient);
				cout << "\n\nDone Successfully.New balance: "<< C.AccountBalance;
				return true;
			}	
		}
	}
	return false;
}
 void PrintClientRecordAccountBalance(sClient Client)
{
	float Total;
	cout << "| " << left << setw(15) << Client.AccountNumber;
	cout << "| " << left << setw(30) << Client.Name;
	cout << "| " << left << setw(15) << Client.AccountBalance << endl;
}
void PrintClientsAccountBalance(vector<sClient>vClient)
{
	float TotalBalance = 0;
	cout << "\n\t\t\t\t\t" << "Client List ( " << vClient.size() << " ) Client(s)\t";
	cout << "\n________________________________________________________________________________________________________\n";
	cout << "| " << left << setw(15) << "Account Number ";
	cout << "| " << left << setw(30) << "Name ";
    cout << "| " << left << setw(15) << "Account Balance ";
	cout << "\n________________________________________________________________________________________________________\n";
	for (sClient& Client : vClient)
	{
		PrintClientRecordAccountBalance(Client);
		TotalBalance += Client.AccountBalance;
	}
	cout << "________________________________________________________________________________________________________\n";
	cout << "\t\t\t Total Balance : " << TotalBalance;
	cout << "\n________________________________________________________________________________________________________\n";
}
short ReadMainMenuOption()
{
	short Choose;
	cout << "\nChoose what do you want to do? [1 to 8]? ";
	cin >> Choose;
	return Choose;
}
short ReadTransactionMenuOption()
{
	short Choose;
	cout << "\nChoose what do you want to do? [1 to 4]? ";
	cin >> Choose;
	return Choose;
}
void ShowTotalBalance()
{
	vector<sClient> vClient = LoadClientsDataFromFile(ClientsFileName);
	PrintClientsAccountBalance(vClient);
}
void ShowDepositByAccountNumber()
{
	cout << "\n-----------------------------------------------\n";
	cout << "\t Deposit Screen ";
	cout << "\n-----------------------------------------------\n";

	sClient Client;
	float Amount;
	vector<sClient> vClient = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber;
	while (!FindClientByAccountNumber(AccountNumber, vClient, Client))
	{
		cout << "\n\nClient with  (" << AccountNumber << ") Not Exist.";
		AccountNumber = ReadClientAccountNumber();
	}
	PrintClient(Client);
	cout << "\nPlease Enter deposit amount? ";
	cin >> Amount;
	DepositBalanceToClientByAccountNumber(AccountNumber,Amount ,vClient);
}
void ShowWithdrawByAccountNumber()
{
	cout << "\n-----------------------------------------------\n";
	cout << "\t Withdraw Screen ";
	cout << "\n-----------------------------------------------\n";
	float Amount;
	sClient Client;
	vector<sClient> vClient = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber;
	while (!FindClientByAccountNumber(AccountNumber, vClient, Client))
	{
		cout << "\n\nClient with  (" << AccountNumber << ") Not Exist.";
		AccountNumber = ReadClientAccountNumber();
	}
	PrintClient(Client);
	cout << "\nPlease Enter Withdraw amount? ";
	cin >> Amount;
	DepositBalanceToClientByAccountNumber(AccountNumber, Amount*-1, vClient);
}
void GoBackToTransactionMenu()
{
	cout << "\nPress any key to go back to Transaction menu...";
	system("pause>nul");
	ShowTransactionMenuScreen();
}
void PreFromTransactionMenuOption(enTransactionOption TransactionOption)
{
	switch (TransactionOption)
	{
	case enTransactionOption:: eDeposit:
		system("cls");
		ShowDepositByAccountNumber();
		GoBackToTransactionMenu();
		break;
	case enTransactionOption:: eWithdraw:
		system("cls");
		ShowWithdrawByAccountNumber();
		GoBackToTransactionMenu();
		break;
	case enTransactionOption:: eTotalBalances:
		system("cls");
		ShowTotalBalance();
		GoBackToTransactionMenu();
		break;
	case enTransactionOption:: eMainMenu:
		system("cls");
		ShowMainMenuScreen();
		break;
	}
}
void ShowTransactionMenuScreen()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pTranactions, CurrentUser))
	{
		ShowAccessDeniedMessage();
		return;
	}
	system("cls");
	cout << "\n===================================================================\n";
	cout << "\t\t" << "Transaction Menu Screen";
	cout << "\n===================================================================\n";
	cout << " \t" << "[1] Deposit.\n ";
	cout << " \t" << "[2] Withdraw.\n ";
	cout << " \t" << "[3] Total Balances.\n ";
	cout << " \t" << "[4] Main Menu.\n ";
	cout << "=====================================================================\n";
	PreFromTransactionMenuOption(enTransactionOption(ReadTransactionMenuOption()));
}
void PreFromMainMenuOption(enMainMenuOptions MainMenuOptions)
{
	sClient Client;
	
	switch (MainMenuOptions)
	{
	case enMainMenuOptions::eListClients:
		system("cls");
		ShowClientList();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::eAddNewClient:
		system("cls");
		ShowAddNewClients();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::eDeletClient:
		system("cls");
		ShowDeleteClient();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::eUpdateClient:
		system("cls");
		ShowUpdateClient();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::eFindClient:
		system("cls");
		ShowFindClient();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::eTrensaction:
		system("cls");
		ShowTransactionMenuScreen();
		break;
	case enMainMenuOptions::eManageUsers:
		system("cls");
		ShowManageUsersScreen();
		break;
	case enMainMenuOptions::eLogout:
		system("cls");
		ShowLogin();
		break;
	}
}
void ShowMainMenuScreen()
{
	system( "cls");
	cout << "\n===================================================================\n";
	cout << "\t\t" << "Main Menu Screen";
	cout << "\n===================================================================\n";
	cout << " \t" << "[1] Show Client List.\n ";
	cout << " \t" << "[2] Add New Client.\n ";
	cout << " \t" << "[3] Delete Client.\n ";
	cout << " \t" << "[4] Update Client.\n ";
	cout << " \t" << "[5] Find Client.\n ";
	cout << " \t" << "[6] Transactions.\n ";
	cout << " \t" << "[7] Manage Users.\n ";
	cout << " \t" << "[8] Logout.\n ";
	cout << "=====================================================================\n";
	PreFromMainMenuOption(enMainMenuOptions(ReadMainMenuOption()));
} 


bool CheckAccessPermission(enMainMenuePermissions Permission, sUser User)
{
	if (User.Permissions ==-1)
		return true;
	if ((Permission & User.Permissions) == Permission)
		return true;
	else return false;
	
}
void ShowAccessDeniedMessage()
{
	cout << "\n-----------------------------------------------\n";
	cout << "Access Denied,\nYou Dont have permission to do this,\n please your Admin.";
	cout << "\n-----------------------------------------------\n";
}
int ReadPermissionsToSet()
{
	int Permissions = 0;
	char Answer = 'n';

	cout << "\nDo you want to give full access? y/n? ";
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y')
	{
		return enMainMenuePermissions::eAll;
	}

	cout << "\nDo you want to give access to : \n";

	cout << "\nShow Client List? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
		Permissions += enMainMenuePermissions::pListClients;

	cout << "\nAdd New Client? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
		Permissions += enMainMenuePermissions::pAddNewClients;

	cout << "\nDelete Client? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
		Permissions += enMainMenuePermissions::pDeleteClients;

	cout << "\nUpdate Client? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
		Permissions += enMainMenuePermissions::pUpdateClients;

	cout << "\nFind Client? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
		Permissions += enMainMenuePermissions::pFindClients;

	cout << "\nTransactions? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
		Permissions += enMainMenuePermissions::pTranactions;

	cout << "\nManage Users? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
		Permissions += enMainMenuePermissions::pManageUsers;

	return Permissions;
}
sUser ConvertUserLineToRecord(string S1, string Separator = "#//#")
{
	sUser User;
	vector<string> vUser = SplitString(S1, Separator);
	
	if (vUser.size()>=2)
	{
		User.UserName = vUser[0];
		User.Password = vUser[1];
		
	}
	if (vUser.size() >= 3)
	{
		User.Permissions = stoi(vUser[2]);
	}
	else User.Permissions = 0;
	return User;
}
vector<sUser> LoadUsersDataFromFile(string FileUsersName)
{
	sUser User;
	vector<sUser>vUser;
	fstream user;
	user.open(FileUsersName, ios::in);
	string Line;
	if (user.is_open())
	{
		while (getline(user, Line))
		{
			if (Line == " ")
				continue;
			User = ConvertUserLineToRecord(Line);
			vUser.push_back(User);
		}
		user.close();
	}
	return vUser;
}
bool FindUserByUserNameAndPassword(string UserName, sUser& User)
{
	vector<sUser> vUser = LoadUsersDataFromFile(FileUsersName);
	for (sUser U: vUser)
	{
		if (Trim(U.UserName) == UserName)
		{
			User = U;
			return true;
		}
	}
	return false;

}
void CheckUser()
{
	bool LoginFailed = false;
	string UserName, Password;
	
	do
	{
		if (LoginFailed)
		{
			cout << "\nInvalid UserName/Password!\n";
		}
		cout << "Enter UserName? ";
		cin >> UserName;
		cout << "Enter Password? ";
		cin >> Password;
		LoginFailed = !FindUserByUserNameAndPassword(UserName, CurrentUser);
	}while (LoginFailed);
	ShowMainMenuScreen();
}
void Login()
{
	cout << "\n===================================================================\n";
	cout << "\t\t" << "Login Screen";
	cout << "\n===================================================================\n";
	CheckUser();
}
void PrintUsersRecord(sUser User)
{
	cout << "| " << left << setw(15) << User.UserName;
	cout << "| " << left << setw(20) << User.Password;
	cout << "| " << left << setw(30) << User.Permissions;
	cout << "\n";
}
void ListUsers(vector<sUser>vUser)
{
	cout << "\n\t\t\t\t\t" << "User List ( " << vUser.size() << " ) User(s)\t";
	cout << "\n________________________________________________________________________________________________________\n";
	cout << "| " << left << setw(15) << "User Name ";
	cout << "| " << left << setw(20) << "Password ";
	cout << "| " << left << setw(30) << "Permissions ";
	
	cout << "\n________________________________________________________________________________________________________\n";
	for (sUser& User : vUser)
	{
		PrintUsersRecord(User);
	}
	cout << "\n________________________________________________________________________________________________________\n";
}
bool UserExistsByUserName(string userName, string FileName)
{
	vector<sUser>vUser;
	fstream user;
	user.open(FileUsersName, ios::in);
	if (user.is_open())
	{
		string Line;
		sUser User;
		while (getline(user, Line))
		{
			User = ConvertUserLineToRecord(Line);
			if (User.UserName==userName )
			{
				user.close();
				return true;
			}
			user.close();
		}
	}
	return false;
}
string ConvertUsersRecordToLine(sUser User, string Separator = "#//#")
{
	string stUserRecord = "";
	stUserRecord += User.UserName + Separator;
	stUserRecord += User.Password+ Separator;
	stUserRecord += to_string(User.Permissions);
	return stUserRecord;
}
sUser ReadeNewUser()
{
	sUser User;
	char Answer = 'y';
	int CountAccess = 0;
	cout << "\nPlease Enter User Data:\n";
	cout << "Enter User Name? ";
	getline(cin >> ws, User.UserName);
	while (UserExistsByUserName(User.UserName, FileUsersName))
	{
		cout << "\nUser with [" << User.UserName << "] already exists,Enter another User Name: ";
		getline(cin >> ws, User.UserName);

	}
	cout << "Enter Password? ";
	cin >> User.Password;
	
	User.Permissions = ReadPermissionsToSet();


	return User;
}
void AddDataUserLineToFile(string FileName, string stDataLine)
{
	fstream user;
	user.open(FileUsersName, ios::out | ios::app);
	if (user.is_open())
	{
		user << stDataLine << endl;
		user.close();
	}
}
void AddNewUser()
{
	sUser User = ReadeNewUser();
	AddDataUserLineToFile(FileUsersName, ConvertUsersRecordToLine(User));
}
void AddUsers()
{
	char AddUser = 'y';
	do
	{

		cout << "\nAdding New User:\n";
		AddNewUser();
		cout << "\nUser Added Successfully, do you want more Users? ";
		cin >> AddUser;
	} while (tolower(AddUser) == 'y');
}
void PrintUser(sUser user)
{
	cout << "the users are: \n";
	cout << "-----------------------------------\n";
	cout << "UserName : " << user.UserName;
	cout << "\nPassword : " << user.Password;
	cout << "\nPermission:" << user.Permissions;
	cout << "\n-----------------------------------\n";
}
bool MarkUserForDeleteByUserName(string UserName,vector<sUser> &vUser)
{
	for (sUser & U : vUser)
	{
		if (Trim(U.UserName) == UserName)
		{
			U.MarkForDelete = true;
			return true;
		}
	}
	return false;
}
vector<sUser>SaveUsersDataToFile(string FileName, vector<sUser>& vUser)
{

	fstream user;
	string Line;

	user.open(FileUsersName, ios::out);
	if (user.is_open())
	{
		for (sUser& C : vUser)
		{
			if (C.MarkForDelete == false)
			{
				C.UserName = Trim(C.UserName);
				C.Password = Trim(C.Password);
				Line = ConvertUsersRecordToLine(C);
				user << Line << endl;
			}
		}
		user.close();
	}
	return vUser;
}
bool DeleteUsersByUserName(string UserName,vector<sUser>&vUser)
{
	char Answer = 'n';
	 sUser User;
	 if (UserName=="Admin")
	 {
		 cout << "\n you cannot Delete this user.\n";
		 return false;
	 }
	if (FindUserByUserNameAndPassword(UserName,User))
	{
		PrintUser(User);
		cout << "\nAre you sure to want Delete this user? ";
		cin >> Answer;
		if (Answer=='y'||Answer=='Y')
		{
			MarkUserForDeleteByUserName(UserName, vUser);
			SaveUsersDataToFile(FileUsersName, vUser);
			vUser = LoadUsersDataFromFile(FileUsersName);
			cout << "\nUser Delete successfully";
			return true;

		}

	}
	else
	{
		cout << "\n\nUser with UserName (" << UserName << ") Not Found!";
		return false;
	}
}
sUser ChangeUserRecord(string UserName)
{
	sUser User;
	User.UserName=UserName;
	
	cout << "Password: ";
	getline(cin>>ws, User.Password);
	cout << "Permission: ";
	User.Permissions=ReadPermissionsToSet();
	return User;

}
bool UpdateUsersByUserName(string UserName, vector<sUser>vUser)
{
	sUser User;
	char Answer = 'n';
	if (FindUserByUserNameAndPassword(UserName,User))
	{
		PrintUser(User);
		cout << "\nAre you sure to want Update this user? ";
		cin >> Answer;
		if (tolower(Answer)=='y')
		{
			for (sUser& U : vUser)
			{
				U = ChangeUserRecord(UserName);
				break;
			}
			SaveUsersDataToFile(FileUsersName, vUser);
			vUser = LoadUsersDataFromFile(FileUsersName);
			cout << "\n\nUsers update successfully. ";
			return true;
		}
	}
	else
	{
		cout << "\n\nUser with UserName (" << UserName << ") Not Found!";
		return false;
	}

}
string ReadUserName()
{
	string UserName;
	cout << "\nEnter UserName: ";
	cin >> UserName;
	return UserName;
}
string ReadPassword()
{
	string Password;
	cout << "\nEnter Password: ";
	cin >> Password;
	return Password;
}
enum enMangeMenuOptions{eListUsers=1,eAddNewUsers=2,eDeleteUser=3,eUpdateUser=4,eFindUser=5,eMainMenu2=6};
short ReadMangeMenuOption()
{
	short Choose;
	cout << "\nChoose what do you want to do? [1 to 6]? ";
	cin >> Choose;
	return Choose;
}
void ShowListUsers()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pManageUsers, CurrentUser))
	{
		ShowAccessDeniedMessage();
		return;
	}
	vector<sUser>vUser = LoadUsersDataFromFile(FileUsersName);
	ListUsers(vUser);
}
void ShowAddUsers()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pManageUsers, CurrentUser))
	{
		ShowAccessDeniedMessage();
		return;
	}
	cout << "\n-----------------------------------------------\n";
	cout << "\t Add New Users Screen ";
	cout << "\n-----------------------------------------------\n";
	
	AddUsers();
}
void ShowDeleteUsers()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pManageUsers, CurrentUser))
	{
		ShowAccessDeniedMessage();
		return;
	}
	cout << "\n-----------------------------------------------\n";
	cout << "\t Delete Users Screen ";
	cout << "\n-----------------------------------------------\n";
	vector<sUser>vUser = LoadUsersDataFromFile(FileUsersName);
	string UserName = Trim(ReadUserName());
	
	DeleteUsersByUserName(UserName, vUser);
}
void ShowUpdateUser()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pManageUsers, CurrentUser))
	{
		ShowAccessDeniedMessage();
		return;
	}
	cout << "\n-----------------------------------------------\n";
	cout << "\t Update User Screen ";
	cout << "\n-----------------------------------------------\n";
	vector<sUser> vUser = LoadUsersDataFromFile(FileUsersName);
	string UserName = Trim(ReadUserName());
	UpdateUsersByUserName(UserName,vUser);
}
void ShowFindUser()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pManageUsers, CurrentUser))
	{
		ShowAccessDeniedMessage();
		return;
	}
	cout << "\n-----------------------------------------------\n";
	cout << "\t Find User Screen ";
	cout << "\n-----------------------------------------------\n";
	vector<sUser> vClient = LoadUsersDataFromFile(FileUsersName);
	string UserName = Trim(ReadUserName());
	sUser User;
	if (FindUserByUserNameAndPassword(UserName, User))
		PrintUser(User);
	else
		cout << "\n\nClient with UserName (" << UserName << ") Not Found!";
}
void PreFromMangeMenuOption(enMangeMenuOptions MangeMenuOptions)
{
	switch (MangeMenuOptions)
	{
	case  enMangeMenuOptions::eListUsers:
		system("cls");
		ShowListUsers();
		BackToManageMenu();
		break;
	case enMangeMenuOptions::eAddNewUsers:
		system("cls");
		ShowAddUsers();
		BackToManageMenu();
		break;
	case eDeleteUser:
		system("cls");
		ShowDeleteUsers();
		BackToManageMenu();
		break;
	case eUpdateUser:
		system("cls");
		ShowUpdateUser();
		BackToManageMenu();
		break;
	case eFindUser:
		system("cls");
		ShowFindUser();
		BackToManageMenu();
		break;
	case enMangeMenuOptions::eMainMenu2:
		system("cls");
		ShowMainMenuScreen();
		GoBackToMainMenu();
		break;
	default:
		break;
	}
}
void ShowManageUsersScreen()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pManageUsers, CurrentUser))
	{
		ShowAccessDeniedMessage();
		return;
	}
	system("cls");
	cout << "\n===================================================================\n";
	cout << "\t\t" << "Manage Users Main Menu Screen";
	cout << "\n===================================================================\n";
	cout << " \t" << "[1] List Users.\n ";
	cout << " \t" << "[2] Add New Users.\n ";
	cout << " \t" << "[3] Delete Users.\n ";
	cout << " \t" << "[4] Update Users.\n ";
	cout << " \t" << "[5] Find Users.\n ";
	cout << " \t" << "[6] Main Menu.\n ";
	cout << "=====================================================================\n";
	PreFromMangeMenuOption(enMangeMenuOptions(ReadMangeMenuOption()));
}
int main()
{
    Login();
	return 0;
}