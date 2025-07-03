#include < iostream >
#include < fstream >
#include < string >
#include < windows.h >

using namespace std;

// Constants
#define MAX 100
#define MED_FILE "medicines.txt"
#define CUST_FILE "customers.txt"
#define ADMIN_FILE "admins.txt"
#define SALES_FILE "sales.txt"
#define PRESC_FILE "prescriptions.txt"

// Global Medicine Data
int medIds[MAX];
string medNames[MAX];
string medCats[MAX];
float medPrices[MAX];
int medQtys[MAX];
string medExps[MAX];
int medCount = 0;

// Global Cart
int cartIds[MAX];
string cartNames[MAX];
float cartPrices[MAX];
int cartQtys[MAX];
int cartCount = 0;

// Set Console text color
void setColor(int  c)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),c) ;
}

// Print Section Header with optional color
void printHeader(string  title,int  color=11) {
    setColor(color);
    cout <<endl<< "==========="<<title<<"==========="<<endl;
    setColor(15);
}

//                          Medicine Functions 


//Load all medicines from file into arrays
void loadMeds() {
    ifstream in(MED_FILE);
    medCount = 0;
    while (in >> medIds[medCount]) {
        in.ignore();
        getline(in, medNames[medCount]);
        getline(in, medCats[medCount]);
        in >> medPrices[medCount] >> medQtys[medCount];
        in.ignore();
        getline(in, medExps[medCount]);
        medCount++;
    }
    in.close();
}

//Save all medicines from arrays to file
void saveMeds() {
    ofstream out(MED_FILE);
    for(int i=0 ; i<medCount ; i++) 
    {
        out<<medIds[i]<<endl<<medNames[i]<<endl<<medCats[i]<<endl<<medPrices[i]<<endl<<medQtys[i]<<endl<<medExps[i]<<endl;
    }
    out.close();
}

//Add a new medicine to the system
void addMedicine() {
    if (medCount >= MAX) return;
    printHeader("Add Medicine ", 6);
    cout << "ID :";
    cin >> medIds[medCount]; cin.ignore();
    cout << "Name :";
    getline(cin, medNames[medCount]);
    cout << "Category :"; 
    getline(cin, medCats[medCount]);
    cout << "Price :";
    cin >> medPrices[medCount];
    cout << "Quantity :";
    cin >> medQtys[medCount];
    cin.ignore();
    cout << "Expiry :";
    getline(cin, medExps[medCount]);
    medCount++;
    saveMeds();
    setColor(10);
    cout << "Medicine Added"<<endl;
    setColor(15);
}

//Delete a medicine by its ID
void deleteMedicine() {
    int id;
    printHeader("Delete Medicine", 4);
    cout << "Enter ID: "; cin >> id;
    for(int i=0; i<medCount; i++)
    {
        if ( medIds[i] == id) {
            for (int j = i ; j < medCount - 1 ; j++) {
                medIds[j] =medIds[j + 1];
                medNames[j] =medNames[j + 1];
                medCats[j] =medCats[j + 1];
                medPrices[j] =medPrices[j + 1];
                medQtys[j] =medQtys[j + 1];
                medExps[j] =medExps[j + 1];
            }
            medCount--;
            saveMeds();
            setColor(10);
            cout << "Deleted"<<endl;
            setColor(15);
            return;
        }
    }
    setColor(12);
    cout << "Not Found"<<endl;
    setColor(15);
}

//Update quantity of an existing medicine
void updateMedicineQty() {
    int id, newQty;
    printHeader("Update Quantity", 3);
    cout << "Enter ID:";
    cin >> id;
    for(int i=0; i<medCount; i++)
    {
        if (medIds[i] == id) {
            cout << "Current Qty:" << medQtys[i] << endl<<"New Qty:";
            cin >> newQty;
            medQtys[i] = newQty;
            saveMeds();
            setColor(10);
            cout << "Updated"<<endl;
            setColor(15);
            return;
        }
    }
    setColor(12);
    cout << "ID not found."<<endl;
    setColor(15);
}

//Search medicine by name ,category, or ID
void searchMedicine() {
    string q;
    printHeader("Search Medicine", 3);
    cout << "Search:"; 
    cin.ignore();
    getline(cin, q);
    bool found = false;
    for(int i=0 ; i<medCount ; i++) 
    {
        string idStr = to_string(medIds[i]);
        if (medNames[i].find(q)!=string::npos||medCats[i].find(q)!=string::npos||idStr.find(q)!=string::npos) {
            found = true;
            setColor(11);
            cout << "ID:"<<medIds[i]<<" | "<<medNames[i]<<" | "<<medCats[i]<<" | Rs," <<medPrices[i]
            <<"| Qty:"<<medQtys[i]<<" | Exp:"<<medExps[i]<<endl;
                    setColor(15);
        }
    }
    if(!found)setColor(12),cout<<"No Results.\n",setColor(15);
}

//Display all avaliable medicines
void viewMedicines() {
    printHeader("Available Medicines", 9);
    cout << "ID   Name           Category       Price     Qty   Expiry\n";
    cout <<endl<<endl;
    for(int i=0 ; i<medCount ; i++)
    {
        if(medQtys[i]<5)
            setColor(12);
        else
            setColor(15);
        //formated output of each medicine
        cout << medIds[i];
        for(int s=to_string(medIds[i]).length();
            s<5 ; s++) cout << " ";

        cout << medNames[i];
        for (int s=medNames[i].length();
            s < 15 ; s++) cout << " ";

        cout << medCats[i];
        for (int s=medCats[i].length();
            s < 15 ; s++) cout << " ";

        string price = to_string(medPrices[i]).substr(0, 5);
        cout << price;
        for (int s= price.length();
            s < 9 ; s++) cout << " ";

        cout << medQtys[i];
        for (int s= to_string(medQtys[i]).length();
            s < 6 ; s++) cout << " ";

        cout << medExps[i] << endl;
    }
    setColor(15);
}

//Show list of medicines near or past expiry
void expiryAlert() {
    printHeader("Expiry Alert", 12);
    for (int i = 0 ;i < medCount ;i++) {
        if(medExps[i]<"2025-08-01") 
        {
            setColor(12);
            cout<<medNames[i]<<"| Expiry:"<<medExps[i]<<endl;
            setColor(15);
        }
    }
}

//                          Cart and Billing 


//Add selected medicines to cart
void addToCart() {
    int id, qty;
    printHeader("Add to Cart", 3);
    cout << "Enter ID:"; cin >> id;
    cout << "Qty:";cin>>qty;
    for(int i=0 ; i<medCount ; i++) 
    {
        if (medIds[i]==id) {
            if (medQtys[i]>=qty) {
                medQtys[i]-=qty;
                saveMeds();

                for (int j=0 ; j < cartCount ; j++) {
                    if (cartIds[j]==id) {
                        cartQtys[j]+=qty;
                        setColor(10) ;
                        cout <<"Updated in cart."<<endl;
                        setColor(15);
                        return;
                    }
                }

                cartIds[cartCount] =id;
                cartNames[cartCount] =medNames[i];
                cartPrices[cartCount] =medPrices[i];
                cartQtys[cartCount] =qty;
                cartCount++;
                setColor(10);
                cout<<"Added to cart."<<endl;
                setColor(15);
            }
            else {
                setColor(12);
                cout<<"Not enough stock."<<endl;
                setColor(15);
            }
            return;
        }
    }
    setColor(12);
    cout<<"ID not found."<<endl;
    setColor(15);
}

//Show current items in the cart
void viewCart() {
    printHeader("Your Cart", 14);
    for(int i=0 ; i<cartCount ; i++)
    {
        cout<<cartNames[i]<<"| Qty:"<<cartQtys[i]<<"| Price:"<<cartPrices[i]<<"| Subtotal:"<<(cartQtys[i]*cartPrices[i])<<endl;
    }
    if(cartCount==0)
        setColor(12),cout<<"Cart is empty.\n", setColor(15);
}

//Remove a medicine from cart and restore quantity
void removeFromCart() {
    int id;
    printHeader("Remove from Cart", 4);
    cout << "Enter ID to remove:";
    cin >> id;
    for(int i=0 ; i<cartCount ; i++)
    {
        if(cartIds[i]==id) {
            for(int j = 0 ; j < medCount ; j++) {
                if(medIds[j]==id) {
                    medQtys[j]+=cartQtys[i];
                    break;
                }
            }

            for (int j = i ; j < cartCount - 1 ; j++) {
                cartIds[j]=cartIds[j + 1];
                cartNames[j]=cartNames[j + 1];
                cartPrices[j]=cartPrices[j + 1];
                cartQtys[j]=cartQtys[j + 1];
            }
            cartCount--;
            saveMeds();
            setColor(10);
            cout<<"Removed."<<endl;
            setColor(15);
            return;
        }
    }
    setColor(12);
    cout<<"Not in cart."<<endl;
    setColor(15);
}

//View total bill and apply discount if applicable
void viewBill() {
    printHeader("Final Bill", 13);
    float total = 0;
    for(int i=0 ; i<cartCount ; i++) {
        float sub=cartPrices[i]*cartQtys[i];
        total+=sub;
        cout<<cartNames[i]<<" x "<< cartQtys[i] <<"=Rs." <<sub<<endl;
    }
    if (total>=1000) {
        float discount=total*0.05;
        total -= discount;
        setColor(13);
        cout << "Discount(5%)=Rs." << discount << endl;
    }
    cout << "Total Bill:Rs=" << total << endl;
    setColor(15);
}

//Finalize bill,write to sales file,clear cart
void makePayment() {
    viewBill();
    ofstream sales(SALES_FILE, ios::app);
    for(int i=0; i<cartCount; i++) 
    {
        sales<<"Item:"<<cartNames[i]<<".Qty:"<<cartQtys[i]
            <<", Subtotal:"<<(cartPrices[i]*cartQtys[i]) << endl;
    }
    sales <<endl<<endl;
    sales.close();
    cartCount = 0;
    setColor(10);
    cout <<"Payment Done"<<endl;
    setColor(15);
}

//Upload a text-based prescription to file
void uploadPrescription() {
    printHeader("Upload Prescription", 3);
    ofstream out(PRESC_FILE, ios::app);
    string content;
    cin.ignore();
    cout<<"Enter prescription:"<<endl;
    getline(cin, content);
    out << "Prescription="<<content<< endl;
    setColor(10);
    cout<<"Uploaded."<<endl;
    setColor(15);
}

//Display sales history from file
void viewSales() {
    printHeader("Sales Report", 14);
    ifstream in(SALES_FILE);
    string line;
    if (!in) {
        setColor(12);
        cout<<"No history."<<endl;
        setColor(15);
        return;
    }
    while (getline(in, line))cout<<line<<endl;
    in.close();
}

//                          Authentication 


//Perform login for admin or customer
bool login(string file) {
    string e, p;
    cin.ignore();
    cout << "Email:";
    getline(cin, e);
    cout << "Password:";
    getline(cin, p);
    ifstream in(file);
    string n, em, pw;
    while( getline(in,n)&&getline(in,em)&&getline(in,pw)) 
    {
        if(em==e&&pw==p) {
            setColor(10); cout << "Welcome " << n << "!\n"; setColor(15);
            return true;
        }
    }
    setColor(12);
    cout << "Login failed."<<endl;
    setColor(15);
    return false;
}

//Register a new admin or customer account 
void signup(string file) {
    string n, e, p;
    cin.ignore();
    cout << "Name:";
    getline(cin, n);
    cout << "Email:";
    getline(cin, e);
    cout << "Password:";
    getline(cin, p);
    ofstream out(file, ios::app);
    out << n << endl << e << endl << p << endl;
    setColor(10);
    cout << "Signup complete."<<endl;
    setColor(15);
}

//                          Menus 


//Admin dashboard with options
void adminMenu() {
    int c;
    do {
        printHeader("Admin Menu", 6);
        setColor(11);
        cout << "1. "; setColor(15);
        cout << "View Medicines\n";
        setColor(11);
        cout << "2. ";
        setColor(15);
        cout << "Add Medicines\n";
        setColor(11);
        cout << "3. ";
        setColor(15);
        cout << "Delete Medicines\n";
        setColor(11);
        cout << "4. ";
        setColor(15);
        cout << "Search Medicines Name\n";
        setColor(11);
        cout << "5. ";
        setColor(15);
        cout << "Update Qty\n";
        setColor(11);
        cout << "6. ";
        setColor(15);
        cout << "Expiry\n";
        setColor(11);
        cout << "7. ";
        setColor(15);
        cout << "Sales\n";
        setColor(11);
        cout << "0. ";
        setColor(15);
        cout << "Logout\n";
        setColor(14);
        cout << "Choose: ";
        setColor(15);
        cin >> c;
        if (c == 1) viewMedicines();
        else if (c == 2) addMedicine();
        else if (c == 3) deleteMedicine();
        else if (c == 4) searchMedicine();
        else if (c == 5) updateMedicineQty();
        else if (c == 6) expiryAlert();
        else if (c == 7) viewSales();
    } while (c != 0);
}

//Customer dashboard with options 
void customerMenu() {
    int c;
    do {
        printHeader("Customer Menu", 9);
        setColor(11);
        cout << "1. ";
        setColor(15); 
        cout << "View Medicines\n";
        setColor(11);
        cout << "2. ";
        setColor(15);
        cout << "Search\n";
        setColor(11);
        cout << "3. ";
        setColor(15);
        cout << "Add to Cart\n";
        setColor(11);
        cout << "4. ";
        setColor(15);
        cout << "View Cart\n";
        setColor(11);
        cout << "5. ";
        setColor(15);
        cout << "Remove from Cart\n";
        setColor(11);
        cout << "6. ";
        setColor(15);
        cout << "View Bill\n";
        setColor(11);
        cout << "7. ";
        setColor(15);
        cout << "Make Payment\n";
        setColor(11);
        cout << "8. ";
        setColor(15);
        cout << "Upload Prescription\n";
        setColor(11);
        cout << "0. ";
        setColor(15);
        cout << "Logout\n";
        setColor(14);
        cout << "Choose: "; setColor(15);
        cin >> c;
        if (c == 1) viewMedicines();
        else if (c == 2) searchMedicine();
        else if (c == 3) addToCart();
        else if (c == 4) viewCart();
        else if (c == 5) removeFromCart();
        else if (c == 6) viewBill();
        else if (c == 7) makePayment();
        else if (c == 8) uploadPrescription();
    } while (c != 0);
}

//Main program menu:login/signup & start
void mainMenu() {
    loadMeds();
    int c;
    do {
        printHeader("PharmaXPERT", 2);
        setColor(11);
        cout << "1. ";
        setColor(15);
        cout << "Admin Login\n";
        setColor(11);
        cout << "2. ";
        setColor(15);
        cout << "Customer Login\n";
        setColor(11);
        cout << "3. ";
        setColor(15);
        cout << "Admin Signup\n";
        setColor(11);
        cout << "4. ";
        setColor(15);
        cout << "Customer Signup\n";
        setColor(11);
        cout << "0. ";
        setColor(15);
        cout << "Exit\n";
        setColor(14);
        cout << "Choose: "; setColor(15);
        cin >> c;
        if(c==1&&login(ADMIN_FILE)) adminMenu();
        else if(c==2&&login(CUST_FILE)) customerMenu();
        else if(c==3)signup(ADMIN_FILE);
        else if(c==4)signup(CUST_FILE);
    } while (c!=0);
}

//entry point of program
int main() {
    mainMenu();
    return 0;
}

