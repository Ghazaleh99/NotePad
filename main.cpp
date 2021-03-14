#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <ctime>
#include <conio.h>
using namespace std;

struct Date {
    int day;
    int year;
    int month;
};

struct Time {
    int hour;
    int minute;
};

struct Program {
    Date date;
    Time time;
    char txt[80];
    int idx = -1;
};

int menu();
int start(Program[]);
void AddEve(Program[], Program, int);
bool SearchEve(Program[], int, int, int, int&, int);
void EditEve(Program[], int, int);
void DeleteEve(Program[], int, int);
void AddToFile(Program[], int, string);
void correctDATE(int&, int&, int&);
void SavingFS(Program[], char, int);
void Sort(Program[], int);
int ReportD(Program[], int, int, int, int);
void sortDate(Program[], int);

int main() {
    Program a[1000];
    fstream file;
    file.open("reminder.txt");
    if (file.is_open()) {//check for file opening
        int LastIndex = start(a);
        system("pause");
        system("cls");
        int option = menu();
        while (option != 5) {
            system("cls");
            int ind = LastIndex;
            sortDate(a, LastIndex);
            if (option == 1) {
                system("cls");
                Program a1;
                cout << "\t->To Add A New Event \n";
                correctDATE(a1.date.day, a1.date.month, a1.date.year);
                cout << "\n\tPlease Enter The Time : \n\tHour : ";
                cin >> a1.time.hour;
                cout << "\n\tMinute : ";
                cin >> a1.time.minute;
                cout << "\n\tEvent : ";
                cin.ignore();
                cin.getline(a1.txt, 80);
                bool flag = false;
                bool thereIS = SearchEve(a, a1.date.year, a1.date.month, a1.date.day, ind, 1);
                if (thereIS == true) {
                    cout << "\n\tThere Is Another Event In The Same Time , Are U Sure U Want To Add ? \n\tPress 'Y' To Add Press 'N' To Cancel";
                    char ch;
                    cout << "\n\t-->>";
                    cin >> ch;
                    if (ch == 'N')
                        flag = true;
                }
                if (flag == false) {
                    AddEve(a, a1, LastIndex);
                    LastIndex++;
                }
                system("pause");
            }

            else if (option == 2 || option == 3) {
                system("cls");
                int year1, month1, day1;
                cout << "\tEnter The Info To Search Events : \n";
                correctDATE(day1, month1, year1);
                bool thereIs = SearchEve(a, year1, month1, day1, ind, 2);
                if (thereIs == false)
                    cout << "\tThere Is No Event!";
                else if (option == 2)
                    EditEve(a, LastIndex, ind);
                else if (option == 3) {
                    LastIndex--;
                    DeleteEve(a, ind, LastIndex);
                }
                system("pause");
            }

            else if (option == 4) {
                system("cls");
                cout << "\tHow Do You Want To Save The Records ? \n\tPress F For File,Press S For Screen : ";
                SavingFS(a, getch(), LastIndex);
            }

            else {
                while (option > 0 && option < 6 && option != 1) {
                    cout << "Please Choose From 1-5'\n'";
                    cin >> option;
                    if (option == 1)
                        break;
                }

            }
            system("cls");
            option = menu();
        }
        AddToFile(a, LastIndex, "reminder.txt");
    }
    else
        cerr << "some thing wrong during opening file!" << endl;
    return 0;
}

void SavingFS(Program a[], char c, int index) {
    string fname;
    if (c == 'F' || c == 'f') {
        cout << "\n\tEnter The File Name : ";
        cin >> fname;
        fname += ".txt";
    }
    system("cls");
    int d, y, m;
    cout << "\tFirst Date:\n";
    correctDATE(d, m, y);
    cout << "\tSecond Date:\n";
    int d1, m1, y1;
    correctDATE(d1, m1, y1);
    Program b[1000];
    int k, s;
    sortDate(a, index);
    if (d == 0 && d1 != 0) {
        k = ReportD(a, d1, m1, y1, index);
        s = 0;
    }
    else if (d1 == 0 && d != 0) {
        s = ReportD(a, d, m, y, index);
        k = index;
    }
    else if (d == 0 && d1 == 0) {
        s = 0;
        k = index;
    }
    else {
        s = ReportD(a, d, m, y, index);
        k = ReportD(a, d1, m1, y1, index);
    }
    for (int i = s; i <= k; i++)
        b[i - s] = a[i];
    Sort(b, k - s + 1);
    if (c == 'F' || c == 'f')
        AddToFile(b, index, fname);
    else {
        system("cls");
        cout << "Event's List : \n";
        for (int i = 0; i < k - s + 1; i++) {
            if (i == k - s)
                break;
            cout << b[i].idx << " " << b[i].date.year << "/" << b[i].date.month << "/" << b[i].date.day << " "
                 << b[i].time.hour << ":" << b[i].time.minute;
            cout << endl << b[i].txt << endl;
        }
        system("pause");
    }
}

int start(Program a[]) {//write file info in Program array & today's events
    time_t set = time(0);//system time
    tm* tmptime = localtime(&set);
    int sys_year = tmptime->tm_year + 1900;
    int sys_mon = tmptime->tm_mon + 1;
    int sys_day = tmptime->tm_mday;
    cout << "***** WELCOME *****\n" << "\tToday Is : " << sys_year << " " << sys_mon << " " << sys_day << endl;//system time
    fstream file;
    file.open("reminder.txt", ios::in);
    int index = 0; int count = 0;
    while (!file.eof()) {
        file >> a[index].idx >> a[index].date.day >> a[index].date.month >> a[index].date.year >> a[index].time.hour >> a[index].time.minute;
        file.ignore();
        file.getline(a[index].txt, 80);
        if (a[index].date.day == sys_day && a[index].date.month == sys_mon && a[index].date.year == sys_year) {
            count++;
            cout << "\tEvent :" << count << ")" << a[index].txt << "\n\tTime -> " << a[index].time.hour << " : " << a[index].time.minute << endl;
        }
        index++;
    }
    return index;
}

void  AddEve(Program a[], Program a1, int index) {
    a[index].idx = index;
    a[index].date.day = a1.date.day;
    a[index].date.year = a1.date.year;
    a[index].date.month = a1.date.month;
    a[index].time.minute = a1.time.minute;
    a[index].time.hour = a1.time.hour;
    strcpy(a[index].txt, a1.txt);
    cout << "\n\tEvent Saved Successfully!";
}

int menu() {
    int cmd;
    cout << "\t  please press 1 - 5 for : " << '\n';
    cout << '\t' << '\t' << "1) Add " << '\n';
    cout << '\t' << '\t' << "2) Edit " << '\n';
    cout << '\t' << '\t' << "3) Delete " << '\n';
    cout << '\t' << '\t' << "4) Report " << '\n';
    cout << '\t' << '\t' << "5) Exit " << '\n' << '\t' << "-->>";
    cin >> cmd;
    return cmd;
}

bool SearchEve(Program a[], int year, int month, int day, int &index, int choice) {
    bool flag = false;
    for (int i = 0; i < index; i++)
        if (a[i].date.day == day && a[i].date.month == month && a[i].date.year == year) {
            flag = true;
            if (choice == 1)
                return flag;
            else {
                cout << "\n\tEvent " << i;
            }
        }
    if (flag == true) {
        cout << "\n\tChoose One Event ";
        int c;
        cin >> c;
        index = c;
    }
    return flag;
}

void EditEve(Program a[], int LastI, int index) {
    system("cls");
    cout << "\t->To Edit An Event \n";
    Program b;
    bool flag = true;
    do {
        correctDATE(b.date.day, b.date.month, b.date.year);
        cout << "\n\n\tPlease Enter The Time : \n\tHour : ";
        cin >> b.time.hour;
        cout << "\n\tMinute : ";
        cin >> b.time.minute;
        cout << "\n\n\tEvent : \n";
        cin.ignore();
        cin.getline(b.txt, 80);
        if (SearchEve(a, b.date.year, b.date.month, b.date.day, LastI, 1) == true) {
            cout << "\n\tThere Is Another Event In The Same Time, Are U Sure U Want To Add ? \n\tPress 'Y' To Add Press 'N' To Cancel";
            char ch;
            cin >> ch;
            if (ch == 'y' || ch == 'Y') {
                flag = false;
                a[index].date.day = b.date.day;
                a[index].date.month = b.date.month;
                a[index].date.year = b.date.year;
                a[index].time.hour = b.time.hour;
                a[index].time.minute = b.time.minute;
                strcpy(a[index].txt, b.txt);
                cout << "\n\tEvent Edited successfully!";
            }
        }
    } while (flag == true);
}

void DeleteEve(Program a[], int index, int Lindex) {
    for (int i = index; i < Lindex; i++) {
        if (Lindex == index + 1)
            break;
        a[index] = a[index + 1];
    }
    cout << "\n\tEvent Deleted Successfully!";
}

void AddToFile(Program a[], int LastIndex, string file1) {
    fstream file;
    file.open(file1, ios::out);
    for (int i = 0; i < LastIndex; i++) {
        if (a[i].idx == -1)
            continue;
        file << a[i].idx << " " << a[i].date.day << " " << a[i].date.month << " " << a[i].date.year << " " << a[i].time.hour << " " << a[i].time.minute << " ";
        file << a[i].txt << endl;
    }
}

void correctDATE(int &d, int &m, int &y) {
    d = 0;
    m = 0;
    y = 0;
    int count = 0;
    cout << "\tPlease Enter The Date : \n\tDay : ";
    do {
        if (count >= 1)
            cout << "\n\tPlease Enter Again : \n\tDay : ";
        cin >> d;
        if (d == 0)
            return;
        count++;
    } while (d < 0 || d > 31);
    count = 0;
    cout << "\tPlease Enter The Month : ";
    do {
        if (count >= 1)
            cout << "\n\tPlease Enter Again : \n\tMonth : ";
        cin >> m;
        if (m == 0)
            return;
        count++;
    } while (m < 0 || m > 12);
    count = 0;
    cout << "\tPlease Enter The Year : ";
    do {
        if (count >= 1)
            cout << "\n\tPlease Enter Again : \n\tYear : ";
        cin >> y;
        if (y == 0)
            return;
        count++;
    } while (y < 2019 && y > 10000);
}

void Sort(Program b[], int index) {
    for (int i = 0; i<index - 1; i++) {
        for (int j = 0; j<index - i - 1; j++) {
            int c = strcmp(b[j].txt, b[j + 1].txt);
            if (c > 0)
                swap(b[j], b[j + 1]);
        }
    }
}

int ReportD(Program a[], int d, int m, int y, int index) {
    int z = 0;
    for (int i = 0; i < index; i++) {
        if (a[i].date.year > y) {
            z = i - 1;
            break;
        }
        else if (a[i].date.year == y) {
            if (a[i].date.month > m) {
                z = i - 1;
                break;
            }
            else if (a[i].date.month == m) {
                if (a[i].date.day > d) {
                    z = i - 1;
                    break;
                }
                else
                    z = index - 1;
            }
        }
    }
    return z;
}

void sortDate(Program a[], int size) {
    for (int i = 0; i < size - 1; i++)
        for (int j = 0; j < size - i - 1; j++) {
            if (a[j].date.year > a[j + 1].date.year)
                swap(a[j], a[j + 1]);
            else if (a[j].date.year == a[j + 1].date.year) {
                if (a[j].date.month > a[j + 1].date.month)
                    swap(a[j], a[j + 1]);
                else if (a[j].date.month == a[j + 1].date.month) {
                    if (a[j].date.day > a[j+1].date.day)
                        swap(a[j], a[j + 1]);
                }
            }
        }
}