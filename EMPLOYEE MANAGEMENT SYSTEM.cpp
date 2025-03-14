#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;

// Maximum number of employees the system can handle
const int MAX_EMPLOYEES = 100;
const int MAX_ADMINS = 5;

// Admin structure for login
struct Admin {
    char username[50];
    char password[50];
    bool isEmpty;
};

// Structure to store employee details
struct Employee {
    int id;
    char name[50];
    char position[50];
    double salary;
    char joinDate[15];
    char department[20]; // HR, Admin, Marketing
    bool isEmpty;  // Flag to check if record is empty
};

// Function prototypes
void initializeEmployees(Employee employees[]);
void initializeAdmins(Admin admins[]);
bool adminLogin(Admin admins[], int adminCount);
void displayMenu();
void departmentMenu();
void addEmployee(Employee employees[], int &count);
void viewAllEmployees(Employee employees[], int count);
void viewByDepartment(Employee employees[], int count, const char* department);
void searchEmployee(Employee employees[], int count);
void updateEmployee(Employee employees[], int count);
void deleteEmployee(Employee employees[], int count);
void saveToFile(Employee employees[], int count);
void loadFromFile(Employee employees[], int &count);
void saveAdmins(Admin admins[], int count);
void loadAdmins(Admin admins[], int &count);
void addAdmin(Admin admins[], int &count);

int main() {
    Employee employees[MAX_EMPLOYEES];
    Admin admins[MAX_ADMINS];
    int employeeCount = 0;
    int adminCount = 0;
    int choice;
    
    // Initialize all records
    initializeEmployees(employees);
    initializeAdmins(admins);
    
    // Load existing data from files
    loadFromFile(employees, employeeCount);
    loadAdmins(admins, adminCount);
    
    // Create default admin if none exists
    if (adminCount == 0) {
        strcpy(admins[0].username, "admin");
        strcpy(admins[0].password, "admin123");
        admins[0].isEmpty = false;
        adminCount = 1;
        saveAdmins(admins, adminCount);
        cout << "Default admin created. Username: admin, Password: admin123" << endl;
    }
    
    cout << "===== Employee Management System =====" << endl;
    
    // Admin login
    if (!adminLogin(admins, adminCount)) {
        cout << "Login failed after 3 attempts. Exiting program." << endl;
        return 0;
    }
    
    cout << "Login successful!" << endl;
    
    do {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                addEmployee(employees, employeeCount);
                break;
            case 2:
                viewAllEmployees(employees, employeeCount);
                break;
            case 3:
                departmentMenu();
                int deptChoice;
                cin >> deptChoice;
                switch (deptChoice) {
                    case 1:
                        viewByDepartment(employees, employeeCount, "HR");
                        break;
                    case 2:
                        viewByDepartment(employees, employeeCount, "Admin");
                        break;
                    case 3:
                        viewByDepartment(employees, employeeCount, "Marketing");
                        break;
                    default:
                        cout << "Invalid department choice." << endl;
                }
                break;
            case 4:
                searchEmployee(employees, employeeCount);
                break;
            case 5:
                updateEmployee(employees, employeeCount);
                break;
            case 6:
                deleteEmployee(employees, employeeCount);
                break;
            case 7:
                addAdmin(admins, adminCount);
                break;
            case 8:
                saveToFile(employees, employeeCount);
                cout << "Data saved to file successfully!" << endl;
                break;
            case 9:
                cout << "Exiting program. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 9);
    
    return 0;
}

// Initialize all employee records as empty
void initializeEmployees(Employee employees[]) {
    for (int i = 0; i < MAX_EMPLOYEES; i++) {
        employees[i].isEmpty = true;
    }
}

// Initialize all admin records as empty
void initializeAdmins(Admin admins[]) {
    for (int i = 0; i < MAX_ADMINS; i++) {
        admins[i].isEmpty = true;
    }
}

// Admin login function
bool adminLogin(Admin admins[], int adminCount) {
    char username[50];
    char password[50];
    int attempts = 0;
    bool loginSuccess = false;
    
    while (attempts < 3 && !loginSuccess) {
        cout << "\n----- Admin Login -----" << endl;
        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;
        
        for (int i = 0; i < adminCount; i++) {
            if (!admins[i].isEmpty && 
                strcmp(admins[i].username, username) == 0 && 
                strcmp(admins[i].password, password) == 0) {
                loginSuccess = true;
                break;
            }
        }
        
        if (!loginSuccess) {
            attempts++;
            cout << "Invalid username or password. Attempts left: " << (3 - attempts) << endl;
        }
    }
    
    return loginSuccess;
}

// Display the menu options
void displayMenu() {
    cout << "\n----- Menu -----" << endl;
    cout << "1. Add New Employee" << endl;
    cout << "2. View All Employees" << endl;
    cout << "3. View Employees by Department" << endl;
    cout << "4. Search Employee" << endl;
    cout << "5. Update Employee" << endl;
    cout << "6. Delete Employee" << endl;
    cout << "7. Add New Admin" << endl;
    cout << "8. Save Data" << endl;
    cout << "9. Exit" << endl;
}

// Display department menu
void departmentMenu() {
    cout << "\n----- Department Selection -----" << endl;
    cout << "1. HR Department" << endl;
    cout << "2. Admin Department" << endl;
    cout << "3. Marketing Department" << endl;
    cout << "Enter choice: ";
}

// Add a new employee
void addEmployee(Employee employees[], int &count) {
    if (count >= MAX_EMPLOYEES) {
        cout << "Employee database is full! Cannot add more employees." << endl;
        return;
    }
    
    cin.ignore(); // Clear input buffer
    
    cout << "\n----- Add New Employee -----" << endl;
    
    Employee newEmp;
    newEmp.isEmpty = false;
    
    cout << "Enter Employee ID: ";
    cin >> newEmp.id;
    cin.ignore();
    
    cout << "Enter Name: ";
    cin.getline(newEmp.name, 50);
    
    cout << "Enter Position: ";
    cin.getline(newEmp.position, 50);
    
    cout << "Enter Salary: ";
    cin >> newEmp.salary;
    cin.ignore();
    
    cout << "Enter Join Date (YYYY-MM-DD): ";
    cin.getline(newEmp.joinDate, 15);
    
    cout << "Enter Department (HR/Admin/Marketing): ";
    cin.getline(newEmp.department, 20);
    
    // Convert department name to consistent case
    if (strcmp(newEmp.department, "hr") == 0 || strcmp(newEmp.department, "HR") == 0) {
        strcpy(newEmp.department, "HR");
    } else if (strcmp(newEmp.department, "admin") == 0 || strcmp(newEmp.department, "ADMIN") == 0) {
        strcpy(newEmp.department, "Admin");
    } else if (strcmp(newEmp.department, "marketing") == 0 || strcmp(newEmp.department, "MARKETING") == 0) {
        strcpy(newEmp.department, "Marketing");
    } else {
        cout << "Invalid department. Setting to Admin by default." << endl;
        strcpy(newEmp.department, "Admin");
    }
    
    employees[count] = newEmp;
    count++;
    
    cout << "Employee added successfully!" << endl;
    
    // Auto-save after adding
    saveToFile(employees, count);
}

// View all employees
void viewAllEmployees(Employee employees[], int count) {
    if (count == 0) {
        cout << "\nNo employees to display." << endl;
        return;
    }
    
    cout << "\n----- All Employees -----" << endl;
    cout << "ID\tName\t\tPosition\t\tSalary\t\tDepartment\tJoin Date" << endl;
    cout << "---------------------------------------------------------------------------------" << endl;
    
    for (int i = 0; i < count; i++) {
        if (!employees[i].isEmpty) {
            cout << employees[i].id << "\t" 
                 << employees[i].name << "\t\t"
                 << employees[i].position << "\t\t"
                 << employees[i].salary << "\t\t"
                 << employees[i].department << "\t\t"
                 << employees[i].joinDate << endl;
        }
    }
}

// View employees by department
void viewByDepartment(Employee employees[], int count, const char* department) {
    if (count == 0) {
        cout << "\nNo employees to display." << endl;
        return;
    }
    
    bool found = false;
    cout << "\n----- " << department << " Department Employees -----" << endl;
    cout << "ID\tName\t\tPosition\t\tSalary\t\tJoin Date" << endl;
    cout << "-------------------------------------------------------------------------" << endl;
    
    for (int i = 0; i < count; i++) {
        if (!employees[i].isEmpty && strcmp(employees[i].department, department) == 0) {
            found = true;
            cout << employees[i].id << "\t" 
                 << employees[i].name << "\t\t"
                 << employees[i].position << "\t\t"
                 << employees[i].salary << "\t\t"
                 << employees[i].joinDate << endl;
        }
    }
    
    if (!found) {
        cout << "No employees found in " << department << " department." << endl;
    }
}

// Search for an employee by ID
void searchEmployee(Employee employees[], int count) {
    if (count == 0) {
        cout << "\nNo employees in the database." << endl;
        return;
    }
    
    int searchId;
    bool found = false;
    
    cout << "\nEnter Employee ID to search: ";
    cin >> searchId;
    
    for (int i = 0; i < count; i++) {
        if (!employees[i].isEmpty && employees[i].id == searchId) {
            found = true;
            cout << "\n----- Employee Found -----" << endl;
            cout << "ID: " << employees[i].id << endl;
            cout << "Name: " << employees[i].name << endl;
            cout << "Position: " << employees[i].position << endl;
            cout << "Salary: " << employees[i].salary << endl;
            cout << "Department: " << employees[i].department << endl;
            cout << "Join Date: " << employees[i].joinDate << endl;
            break;
        }
    }
    
    if (!found) {
        cout << "Employee with ID " << searchId << " not found." << endl;
    }
}

// Update employee information
void updateEmployee(Employee employees[], int count) {
    if (count == 0) {
        cout << "\nNo employees in the database." << endl;
        return;
    }
    
    int updateId;
    bool found = false;
    
    cout << "\nEnter Employee ID to update: ";
    cin >> updateId;
    cin.ignore();
    
    for (int i = 0; i < count; i++) {
        if (!employees[i].isEmpty && employees[i].id == updateId) {
            found = true;
            cout << "\n----- Update Employee -----" << endl;
            cout << "Current Name: " << employees[i].name << endl;
            cout << "Enter New Name (or press Enter to keep current): ";
            char newName[50];
            cin.getline(newName, 50);
            if (strlen(newName) > 0) {
                strcpy(employees[i].name, newName);
            }
            
            cout << "Current Position: " << employees[i].position << endl;
            cout << "Enter New Position (or press Enter to keep current): ";
            char newPosition[50];
            cin.getline(newPosition, 50);
            if (strlen(newPosition) > 0) {
                strcpy(employees[i].position, newPosition);
            }
            
            cout << "Current Department: " << employees[i].department << endl;
            cout << "Enter New Department (HR/Admin/Marketing) (or press Enter to keep current): ";
            char newDepartment[20];
            cin.getline(newDepartment, 20);
            if (strlen(newDepartment) > 0) {
                if (strcmp(newDepartment, "hr") == 0 || strcmp(newDepartment, "HR") == 0) {
                    strcpy(employees[i].department, "HR");
                } else if (strcmp(newDepartment, "admin") == 0 || strcmp(newDepartment, "ADMIN") == 0) {
                    strcpy(employees[i].department, "Admin");
                } else if (strcmp(newDepartment, "marketing") == 0 || strcmp(newDepartment, "MARKETING") == 0) {
                    strcpy(employees[i].department, "Marketing");
                } else {
                    cout << "Invalid department. Department not changed." << endl;
                }
            }
            
            cout << "Current Salary: " << employees[i].salary << endl;
            cout << "Enter New Salary (or enter 0 to keep current): ";
            double newSalary;
            cin >> newSalary;
            cin.ignore();
            if (newSalary > 0) {
                employees[i].salary = newSalary;
            }
            
            cout << "Employee updated successfully!" << endl;
            
            // Auto-save after updating
            saveToFile(employees, count);
            break;
        }
    }
    
    if (!found) {
        cout << "Employee with ID " << updateId << " not found." << endl;
    }
}

// Delete an employee
void deleteEmployee(Employee employees[], int count) {
    if (count == 0) {
        cout << "\nNo employees in the database." << endl;
        return;
    }
    
    int deleteId;
    bool found = false;
    
    cout << "\nEnter Employee ID to delete: ";
    cin >> deleteId;
    
    for (int i = 0; i < count; i++) {
        if (!employees[i].isEmpty && employees[i].id == deleteId) {
            found = true;
            
            // Mark as empty instead of actually removing
            employees[i].isEmpty = true;
            
            cout << "Employee deleted successfully!" << endl;
            
            // Auto-save after deleting
            saveToFile(employees, count);
            break;
        }
    } if (!found) {
        cout << "Employee with ID " << deleteId << " not found." << endl;
    }
}

// Add a new admin user
void addAdmin(Admin admins[], int &count) {
    if (count >= MAX_ADMINS) {
        cout << "Admin database is full! Cannot add more admins." << endl;
        return;
    }
    
    cin.ignore(); // Clear input buffer
    
    cout << "\n----- Add New Admin -----" << endl;
    
    Admin newAdmin;
    newAdmin.isEmpty = false;
    
    cout << "Enter Username: ";
    cin.getline(newAdmin.username, 50);
    
    cout << "Enter Password: ";
    cin.getline(newAdmin.password, 50);
    
    // Check if username already exists
    bool usernameExists = false;
    for (int i = 0; i < count; i++) {
        if (!admins[i].isEmpty && strcmp(admins[i].username, newAdmin.username) == 0) {
            usernameExists = true;
            break;
        }
    }
    
    if (usernameExists) {
        cout << "Username already exists! Please choose a different username." << endl;
        return;
    }
    
    admins[count] = newAdmin;
    count++;
    
    cout << "Admin added successfully!" << endl;
    
    // Auto-save after adding
    saveAdmins(admins, count);
}

// Save employee data to file
void saveToFile(Employee employees[], int count) {
    ofstream outFile("employees.dat", ios::binary);
    
    if (!outFile) {
        cout << "Error opening file for writing!" << endl;
        return;
    }
    
    // Write number of employees first
    outFile.write(reinterpret_cast<char*>(&count), sizeof(count));
    
    // Write employee data
    for (int i = 0; i < count; i++) {
        outFile.write(reinterpret_cast<char*>(&employees[i]), sizeof(Employee));
    }
    
    outFile.close();
}

// Load employee data from file
void loadFromFile(Employee employees[], int &count) {
    ifstream inFile("employees.dat", ios::binary);
    
    if (!inFile) {
        cout << "Creating by Rukhshan Ahmed ." << endl;
        count = 0;
        return;
    }
    
    // Read number of employees first
    inFile.read(reinterpret_cast<char*>(&count), sizeof(count));
    
    // Read employee data
    for (int i = 0; i < count; i++) {
        inFile.read(reinterpret_cast<char*>(&employees[i]), sizeof(Employee));
    }
    
    inFile.close();
    cout << count << " employees loaded from file." << endl;
}

// Save admin data to file
void saveAdmins(Admin admins[], int count) {
    ofstream outFile("admins.dat", ios::binary);
    
    if (!outFile) {
        cout << "Error opening admin file for writing!" << endl;
        return;
    }
    
    // Write number of admins first
    outFile.write(reinterpret_cast<char*>(&count), sizeof(count));
    
    // Write admin data
    for (int i = 0; i < count; i++) {
        outFile.write(reinterpret_cast<char*>(&admins[i]), sizeof(Admin));
    }
    
    outFile.close();
}

// Load admin data from file
void loadAdmins(Admin admins[], int &count) {
    ifstream inFile("admins.dat", ios::binary);
    
    if (!inFile) {
        cout << "No existing admin data found. Will create default admin." << endl;
        count = 0;
        return;
    }
    
    // Read number of admins first
    inFile.read(reinterpret_cast<char*>(&count), sizeof(count));
    
    // Read admin data
    for (int i = 0; i < count; i++) {
        inFile.read(reinterpret_cast<char*>(&admins[i]), sizeof(Admin));
    }
    
    inFile.close();
    cout << count << " admins loaded from file." << endl;
}
