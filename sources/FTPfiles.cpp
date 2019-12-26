//Copyright 2019 <boikov3>
#include "FTPfiles.h"
string getLatestDate(const string &date1, const string &date2) {
    unsigned int d1 = stoi(date1);
    unsigned int d2 = stoi(date2);
    if (d1 >= d2)
        return date1;
    else
        return date2;
}

void printFinFile(const path &p,
                  map<string, pair <unsigned int, string>>& accounts) {
    file file;
    file.name = p.stem().string();
    file.balance = file.name.substr(0, 8);
    file.number = file.name.substr(8, 8);
    file.separator = file.name.substr(16, 1);
    file.date = file.name.substr(17, 8);
    file.type = p.extension().string();
    if (file.balance == "balance_" && stoi(file.number) && file.separator == "_"
    && stoi(file.date) && file.type == ".txt"
    && file.name.substr(25, 4) != ".old") {
        std::cout << p.parent_path().string().substr(pz) + " " +
        p.filename().string() + '\n';
        if (accounts.find(file.number) == accounts.end()) {
            accounts[file.number] = pair<int, string>(1, file.date);
        } else {
            accounts[file.number].first++;
            accounts[file.number].second =
                    getLatestDate(file.date, accounts[file.number].second);
        }
    }
}

void printAccountsInfo(const path &path_to_dir,
                       map<string, pair <unsigned int, string>>& accounts) {
    for (const directory_entry& obj :
    recursive_directory_iterator(path_to_dir)) {
        if (is_regular_file(obj.path())) {
            string name = obj.path().stem().string();
            string balance = name.substr(0, 8);
            string number = name.substr(8, 8);
            string separator = name.substr(16, 1);
            string date = name.substr(17, 8);
            string type = obj.path().extension().string();
            if (balance == "balance_" && stoi(number)
            && separator == "_"
            && stoi(date) && type == ".txt" && date ==
            accounts[number].second) {
                std::cout << "broker:" <<
                obj.path().string().substr(pz, obj.path().string().length()
                -obj.path().filename().string().length()-pz-1)
                          << " account:" +
                          number + " files:" << accounts[number].first <<
                          " lastdate:" + date + '\n';
            }
        }
    }
}

void analyse(const path& p,
             map<string, pair <unsigned int, string>>& accounts) {
    if (exists(p)) {
        if (is_regular_file(p)) {
            printFinFile(p, accounts);
        } else if (is_directory(p)) {
            std::cout << p.string().substr(pz) <<
                      " - директория, содержащая:\n";
        } else if (is_symlink(p)) {
            analyse(read_symlink(p), accounts);
        } else {
            std::cout << p <<
                      "существует, но не является директорией "
                      "или нужным файлом\n";
        }
    } else {
        std::cout << p << "не существует\n";
    }
}

void iterate(const path &pathToDir) {
    map<string, pair <unsigned int, string>> accounts;
    std::cout << std::endl;
    for (const directory_entry& obj : recursive_directory_iterator(pathToDir)) {
        analyse(obj.path(), accounts);
    }
    printAccountsInfo(pathToDir, accounts);
}

