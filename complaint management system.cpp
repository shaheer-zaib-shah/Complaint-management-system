#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
using namespace std;

// Color codes for console UI (ANSI Escape Codes)
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"
#define MAGENTA "\033[35m"
#define BOLD "\033[1m"

struct Complaint
{
    int id;
    string status;      // Pending / Confirmed / Rejected
    string description; // Can contain spaces

    void save(ofstream& fout) const
    {
        fout << id << "\n"
            << status << "\n"
            << description << "\n";
    }

    static bool load(ifstream& fin, Complaint& c)
    {
        string idLine;
        if (!getline(fin, idLine))
            return false;
        c.id = stoi(idLine);

        if (!getline(fin, c.status))
            return false;
        if (!getline(fin, c.description))
            return false;

        return true;
    }
};

class Login
{
protected:
    string adminUsername, adminPassword;
    string userUsername, userPassword;
    string loggedRole;

    const string adminFile = "admin.txt";
    const string userFile = "user.txt";

public:
    void registerAccounts();
    void login();
    string getLoggedRole() { return loggedRole; }

    void loadCredentials();
    void saveCredentials();
};

class Dashboard : public Login
{
private:
    vector<Complaint> complaints;
    int complaintCount = 0;

    const string complaintsFile = "complaints.txt";

public:
    void userDashboard();
    void adminDashboard();

    void registerComplaint();
    void viewComplaints();
    void editComplaint();
    void shareComplaint();
    void viewAllComplaints();
    void confirmComplaint();
    void rejectComplaint();

    void runSystem();

    void loadComplaints();
    void saveComplaints();
};

void printBanner()
{
    cout << CYAN << R"(   ___ ___  __  __ ___ _      _   ___ _  _ _____   __  __   _   _  _   _   ___ ___ __  __ ___ _  _ _____   _____   _____ _____ ___ __  __ 
  / __/ _ \|  \/  | _ \ |    /_\ |_ _| \| |_   _| |  \/  | /_\ | \| | /_\ / __| __|  \/  | __| \| |_   _| / __\ \ / / __|_   _| __|  \/  |
 | (_| (_) | |\/| |  _/ |__ / _ \ | || .` | | |   | |\/| |/ _ \| .` |/ _ \ (_ | _|| |\/| | _|| .` | | |   \__ \\ V /\__ \ | | | _|| |\/| |
  \___\___/|_|  |_|_| |____/_/ \_\___|_|\_| |_|   |_|  |_/_/ \_\_|\_/_/ \_\___|___|_|  |_|___|_|\_| |_|   |___/ |_| |___/ |_| |___|_|  |_|
                                                                                                                                         

)" << RESET
<< endl;
}

void printGoodbye()
{
    cout << GREEN << R"(
  _______ _    _          _   _ _  __ __     ______  _    _ 
 |__   __| |  | |   /\   | \ | | |/ / \ \   / / __ \| |  | |
    | |  | |__| |  /  \  |  \| | ' /   \ \_/ / |  | | |  | |
    | |  |  __  | / /\ \ | . ` |  <     \   /| |  | | |  | |
    | |  | |  | |/ ____ \| |\  | . \     | | | |__| | |__| |
    |_|  |_|  |_/_/    \_\_| \_|_|\_\    |_|  \____/ \____/ 
                                                            
                                                            
)" << RESET
<< endl;
}

// ----------- Login Implementation -----------

void Login::loadCredentials()
{
    ifstream fin;

    fin.open(adminFile);
    if (fin.is_open())
    {
        getline(fin, adminUsername);
        getline(fin, adminPassword);
        fin.close();
    }

    fin.open(userFile);
    if (fin.is_open())
    {
        getline(fin, userUsername);
        getline(fin, userPassword);
        fin.close();
    }
}

void Login::saveCredentials()
{
    ofstream fout;

    fout.open(adminFile);
    fout << adminUsername << "\n"
        << adminPassword << "\n";
    fout.close();

    fout.open(userFile);
    fout << userUsername << "\n"
        << userPassword << "\n";
    fout.close();
}

void Login::registerAccounts()
{
    cout << MAGENTA << BOLD << "\n--- Admin Registration ---" << RESET << "\n";
    cout << YELLOW << "Set Admin Username: " << RESET;
    cin >> adminUsername;
    cout << YELLOW << "Set Admin Password: " << RESET;
    cin >> adminPassword;

    cout << MAGENTA << BOLD << "\n--- User Registration ---" << RESET << "\n";
    cout << YELLOW << "Set User Username: " << RESET;
    cin >> userUsername;
    cout << YELLOW << "Set User Password: " << RESET;
    cin >> userPassword;

    saveCredentials();

    cout << GREEN << "\nRegistration Completed!\n"
        << RESET;
}

void Login::login()
{
    int choice;
    string username, password;

    while (true)
    {
        cout << MAGENTA << BOLD << "\n--- Login Menu ---" << RESET << "\n";
        cout << CYAN << "1. Admin Login\n2. User Login\n"
            << YELLOW << "Enter choice: " << RESET;
        cin >> choice;
        if (cin.fail() || (choice != 1 && choice != 2))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << RED << "Invalid choice! Please enter 1 or 2.\n"
                << RESET;
            continue;
        }

        cout << YELLOW << "Username: " << RESET;
        cin >> username;
        cout << YELLOW << "Password: " << RESET;
        cin >> password;

        if (choice == 1 && username == adminUsername && password == adminPassword)
        {
            loggedRole = "admin";
            cout << GREEN << "Admin Login Successful!\n"
                << RESET;
            break;
        }
        else if (choice == 2 && username == userUsername && password == userPassword)
        {
            loggedRole = "user";
            cout << GREEN << "User Login Successful!\n"
                << RESET;
            break;
        }
        else
        {
            cout << RED << "Login Failed! Invalid credentials.\n"
                << RESET;
        }
    }
}

// ----------- Dashboard Implementation -----------

void Dashboard::loadComplaints()
{
    complaints.clear();
    ifstream fin(complaintsFile);
    if (!fin.is_open())
    {
        complaintCount = 0;
        return;
    }

    Complaint c;
    complaintCount = 0;

    while (Complaint::load(fin, c))
    {
        complaints.push_back(c);
        if (c.id > complaintCount)
            complaintCount = c.id;
    }
    fin.close();
}

void Dashboard::saveComplaints()
{
    ofstream fout(complaintsFile);
    if (!fout.is_open())
    {
        cerr << RED << "Error: Cannot open complaints file for writing.\n"
            << RESET;
        return;
    }

    for (const auto& c : complaints)
    {
        c.save(fout);
    }
    fout.close();
}

void Dashboard::runSystem()
{
    loadCredentials();
    loadComplaints();

    if (adminUsername.empty() || adminPassword.empty() || userUsername.empty() || userPassword.empty())
    {
        cout << MAGENTA << BOLD << "No registration found. Please register accounts first.\n"
            << RESET;
        registerAccounts();
    }

    char choice;
    do
    {
        login();

        if (loggedRole == "admin")
            adminDashboard();
        else if (loggedRole == "user")
            userDashboard();

        cout << CYAN << "\nDo you want to login again? (y/n): " << RESET;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } while (choice == 'y' || choice == 'Y');

    printGoodbye();
}

void Dashboard::userDashboard()
{
    int choice;

    while (true)
    {
        cout << MAGENTA << BOLD << "\n--- User Dashboard ---" << RESET << "\n";
        cout << "  1. Register Complaint\n  2. Edit Complaint\n  3. View Complaints\n  4. Share Complaint\n  5. Logout\n";
        cout << YELLOW << "Enter choice: " << RESET;
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << RED << "Invalid input! Please enter a number between 1 and 5.\n"
                << RESET;
            continue;
        }

        switch (choice)
        {
        case 1:
            registerComplaint();
            break;
        case 2:
            editComplaint();
            break;
        case 3:
            viewComplaints();
            break;
        case 4:
            shareComplaint();
            break;
        case 5:
            cout << GREEN << "Logging out...\n"
                << RESET;
            return;
        default:
            cout << RED << "Invalid option! Please enter a number between 1 and 5.\n"
                << RESET;
        }
    }
}

void Dashboard::registerComplaint()
{
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    Complaint c;
    cout << CYAN << "Enter complaint description: " << RESET;
    getline(cin, c.description);
    c.id = ++complaintCount;
    c.status = "Pending";
    complaints.push_back(c);
    saveComplaints();
    cout << GREEN << "Complaint registered successfully.\n"
        << RESET;
}

void Dashboard::editComplaint()
{
    int id;
    cout << CYAN << "Enter complaint ID to edit: " << RESET;
    cin >> id;

    if (cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << RED << "Invalid input! Please enter a valid complaint ID.\n"
            << RESET;
        return;
    }

    if (id < 1 || id > complaintCount)
    {
        cout << RED << "Invalid ID.\n"
            << RESET;
        return;
    }

    if (complaints[id - 1].status != "Pending")
    {
        cout << RED << "Cannot edit. Complaint is " << complaints[id - 1].status << ".\n"
            << RESET;
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << CYAN << "Enter new description: " << RESET;
    getline(cin, complaints[id - 1].description);
    saveComplaints();
    cout << GREEN << "Complaint updated.\n"
        << RESET;
}

void Dashboard::viewComplaints()
{
    if (complaints.empty())
    {
        cout << RED << "No complaints yet.\n"
            << RESET;
        return;
    }

    cout << MAGENTA << BOLD << "\n--- My Complaints ---\n"
        << RESET;
    for (const auto& c : complaints)
    {
        cout << "ID: " << c.id << " | Status: " << c.status << " | " << c.description << "\n";
    }
}

void Dashboard::shareComplaint()
{
    int id;
    cout << CYAN << "Enter complaint ID to share: " << RESET;
    cin >> id;

    if (cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << RED << "Invalid input! Please enter a valid complaint ID.\n"
            << RESET;
        return;
    }

    if (id < 1 || id > complaintCount)
    {
        cout << RED << "Invalid ID.\n"
            << RESET;
        return;
    }

    cout << MAGENTA << BOLD << "\n--- Shared Complaint ---\n"
        << RESET;
    cout << "ID: " << complaints[id - 1].id << "\n";
    cout << "Status: " << complaints[id - 1].status << "\n";
    cout << "Description: " << complaints[id - 1].description << "\n";
}

void Dashboard::adminDashboard()
{
    int choice;

    while (true)
    {
        cout << MAGENTA << BOLD << "\n--- Admin Dashboard ---\n"
            << RESET;
        cout << "  1. View All Complaints\n  2. Confirm Complaint\n  3. Reject Complaint\n  4. Logout\n";
        cout << YELLOW << "Enter choice: " << RESET;
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << RED << "Invalid input! Please enter a number between 1 and 4.\n"
                << RESET;
            continue;
        }

        switch (choice)
        {
        case 1:
            viewAllComplaints();
            break;
        case 2:
            confirmComplaint();
            break;
        case 3:
            rejectComplaint();
            break;
        case 4:
            cout << GREEN << "Logging out...\n"
                << RESET;
            return;
        default:
            cout << RED << "Invalid option! Please enter a number between 1 and 4.\n"
                << RESET;
        }
    }
}

void Dashboard::viewAllComplaints()
{
    if (complaints.empty())
    {
        cout << RED << "No complaints available.\n"
            << RESET;
        return;
    }

    cout << MAGENTA << BOLD << "\n--- All Complaints ---\n"
        << RESET;
    for (const auto& c : complaints)
    {
        cout << "ID: " << c.id << " | Status: " << c.status << " | " << c.description << "\n";
    }
}

void Dashboard::confirmComplaint()
{
    int id;
    cout << CYAN << "Enter complaint ID to confirm: " << RESET;
    cin >> id;

    if (cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << RED << "Invalid input! Please enter a valid complaint ID.\n"
            << RESET;
        return;
    }

    if (id < 1 || id > complaintCount)
    {
        cout << RED << "Invalid ID.\n"
            << RESET;
        return;
    }

    complaints[id - 1].status = "Confirmed";
    saveComplaints();
    cout << GREEN << "Complaint confirmed.\n"
        << RESET;
}

void Dashboard::rejectComplaint()
{
    int id;
    cout << CYAN << "Enter complaint ID to reject: " << RESET;
    cin >> id;

    if (cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << RED << "Invalid input! Please enter a valid complaint ID.\n"
            << RESET;
        return;
    }

    if (id < 1 || id > complaintCount)
    {
        cout << RED << "Invalid ID.\n"
            << RESET;
        return;
    }

    complaints[id - 1].status = "Rejected";
    saveComplaints();
    cout << GREEN << "Complaint rejected.\n"
        << RESET;
}

int main()
{
    printBanner();

    Dashboard system;
    system.runSystem();

    return 0;
}
