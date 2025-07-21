#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
#include <cctype>
using namespace std;

struct Student {
    string name;
    int rank;
    string category;
    vector<string> preferences;
    string allocated = "None";
    int round = 0;
    string status = "FLOAT";
    bool withdrawn = false;
    vector<string> allocationHistory; 
};

struct College {
    string name;
    int totalSeats;
    map<string, int> seats;
    map<string, int> initialSeats;
};

bool compareRank(const Student &a, const Student &b) {
    return a.rank < b.rank;
}

map<string, int> getSeatDistribution(int total, map<string, float> &reservations) {
    map<string, int> seatCount;
    int assigned = 0;
    for (map<string, float>::iterator it = reservations.begin(); it != reservations.end(); ++it) {
        seatCount[it->first] = int(total * it->second);
        assigned += seatCount[it->first];
    }
    seatCount["GEN"] += total - assigned; 
    return seatCount;
}

void resetSeats(map<string, College> &colleges) {
    for (map<string, College>::iterator it = colleges.begin(); it != colleges.end(); ++it) {
        it->second.seats = it->second.initialSeats;
    }
}

void freeFloatingSeats(vector<Student> &students, map<string, College> &colleges) {
    for (size_t i = 0; i < students.size(); ++i) {
        Student &s = students[i];
        if (s.withdrawn) continue;
        if (s.status == "FLOAT" && s.allocated != "None") {
            string cat = s.category;
            string col = s.allocated;
            if (colleges[col].initialSeats.count(cat)) {
                colleges[col].seats[cat]++;
            } else if (cat != "GEN") {
                colleges[col].seats["GEN"]++;
            }
            s.allocated = "None";
        }
    }
}

void allocate(vector<Student> &students, map<string, College> &colleges, int round) {
    sort(students.begin(), students.end(), compareRank);
    for (size_t i = 0; i < students.size(); ++i) {
        Student &s = students[i];
        if (s.withdrawn || s.status == "FREEZE") continue;
        for (size_t j = 0; j < s.preferences.size(); ++j) {
            string pref = s.preferences[j];
            College &col = colleges[pref];
            if (col.seats[s.category] > 0) {
                s.allocated = pref;
                s.round = round;
                col.seats[s.category]--;
                break;
            } else if (s.category != "GEN" && col.seats["GEN"] > 0) {
                s.allocated = pref;
                s.round = round;
                col.seats["GEN"]--;
                break;
            }
        }
    }
}

void printUserProgressDetailed(const vector<Student> &students, const string &userName) {
    cout << "\n--- Allocation Progress for " << userName << " ---\n";
    for (const auto& s : students) {
        if (s.name == userName) {
            for (int r = 0; r < s.allocationHistory.size(); ++r) {
                cout << "Round " << r + 1 << ": " << s.allocationHistory[r] << "\n";
            }
            if (s.withdrawn) {
                cout << "\nStatus: You have withdrawn your seat.";
            } else if (s.status == "FREEZE") {
                cout << "\nYou have frozen your seat at " << s.allocated << ".\n";
            }
            break;
        }
    }
}

int main() {
    map<string, float> reservation;
    reservation["GEN"] = 0.50;
    reservation["OBC"] = 0.27;
    reservation["SC"] = 0.15;
    reservation["ST"] = 0.08;

    vector<pair<string, int> > collegeInfo = {
        {"IITB", 5}, {"IITD", 8}, {"IITK", 9}
    };

    map<string, College> colleges;
    for (auto &info : collegeInfo) {
        College c;
        c.name = info.first;
        c.totalSeats = info.second;
        c.seats = getSeatDistribution(c.totalSeats, reservation);
        c.initialSeats = c.seats;
        colleges[c.name] = c;
    }

    vector<Student> students = {
        {"Amit", 1, "GEN", {"IITB", "IITD", "IITK"}},
        {"Bhavna", 2, "OBC", {"IITD", "IITB", "IITK"}},
        {"Chirag", 3, "SC", {"IITK", "IITD"}},
        {"Divya", 4, "GEN", {"IITD", "IITB"}},
        {"Eshan", 5, "OBC", {"IITB", "IITK"}},
        {"Farah", 6, "SC", {"IITB", "IITD"}},
        {"Gaurav", 7, "GEN", {"IITK", "IITD"}},
        {"Hina", 8, "OBC", {"IITD", "IITK"}},
        {"Ishaan", 9, "GEN", {"IITB"}},
        {"Jaya", 10, "SC", {"IITD", "IITK"}},
        {"Kunal", 11, "ST", {"IITB", "IITD"}},
        {"Lata", 12, "GEN", {"IITK", "IITD"}},
        {"Manoj", 13, "OBC", {"IITB", "IITD"}},
        {"Nikita", 14, "SC", {"IITD", "IITK"}},
        {"Om", 15, "GEN", {"IITB", "IITD"}},
        {"Pooja", 16, "OBC", {"IITB", "IITD"}},
        {"Qadir", 17, "GEN", {"IITD", "IITK"}},
        {"Riya", 18, "OBC", {"IITB", "IITK"}},
        {"Suraj", 19, "ST", {"IITB", "IITK"}},
        {"Tina", 20, "GEN", {"IITB"}}
    };

    string name, category, line;
    int rank;
    vector<string> prefs;

    cout << "\nEnter your name: ";
    getline(cin, name);
    cout << "Enter your rank: ";
    cin >> rank;
    cin.ignore();
    cout << "Enter your category (GEN/OBC/SC/ST): ";
    getline(cin, category);
    cout << "Enter your preferences separated by space (e.g. IITB IITD IITK): ";
    getline(cin, line);
    istringstream iss(line);
    string pref;
    while (iss >> pref) prefs.push_back(pref);

    Student newUser = {name, rank, category, prefs};
    students.push_back(newUser);

    for (int round = 1; round <= 4; ++round) {
        resetSeats(colleges);
        freeFloatingSeats(students, colleges);
        allocate(students, colleges, round);

        for (auto &s : students) {
            if (s.name == name) {
                string historyMsg = "Not allocated";
                if (s.allocated != "None" && s.round == round) {
                    cout << "\nRound " << round << ": You have been allotted " << s.allocated << " [" << s.category << "].\n";
                    cout << "Do you want to withdraw your seat? (Y/N): ";
                    char withdrawChoice;
                    cin >> withdrawChoice;
                    if (toupper(withdrawChoice) == 'Y') {
                        s.withdrawn = true;
                        s.allocated = "None";
                        s.allocationHistory.push_back("Withdrew after being allotted");
                        cout << "You have withdrawn your seat.\n";
                        break;
                    }
                    cout << "Do you want to FREEZE this seat? (Y/N): ";
                    char choice;
                    cin >> choice;
                    if (toupper(choice) == 'Y') {
                        s.status = "FREEZE";
                        cout << "You have frozen your seat at " << s.allocated << ".\n";
                        historyMsg = "Allotted " + s.allocated + " [" + s.category + "], Status: FREEZE";
                    } else {
                        if (round == 4) {
                            cout << "The Last round is over. You cannot FLOAT anymore.\n";
                            s.status = "FREEZE";
                            cout << "You have been forced to freeze your seat at " << s.allocated << ".\n";
                            historyMsg = "Allotted " + s.allocated + " [" + s.category + "], Status: FREEZE";
                        } else {
                            cout << "You chose to FLOAT. We'll try to upgrade you in the next round.\n";
                            historyMsg = "Allotted " + s.allocated + " [" + s.category + "], Status: FLOAT";
                        }
                    }
                } else if (s.status == "FREEZE" && s.allocated != "None") {
                    cout << "\nRound " << round << ": You have frozen your seat at " << s.allocated << ".\n";
                    historyMsg = "Seat frozen at " + s.allocated;
                } else if (!s.withdrawn && s.allocated == "None") {
                    cout << "\nRound " << round << ": You have not been allotted any seat.\n";
                    cout << "Do you want to withdraw your seat? (Y/N): ";
                    char withdrawChoice;
                    cin >> withdrawChoice;
                    if (toupper(withdrawChoice) == 'Y') {
                        s.withdrawn = true;
                        s.allocated = "None";
                        cout << "You have withdrawn your seat.\n";
                        s.allocationHistory.push_back("Withdrew after not being allotted");
                        break;
                    }
                }
                if (!s.withdrawn) s.allocationHistory.push_back(historyMsg);
            }
        }

        if (round == 1) {
            for (int i = 0; i < 5; ++i) students[i].status = "FREEZE";
        }
    }

    printUserProgressDetailed(students, name);
    return 0;
}
