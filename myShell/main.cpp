/*
████████╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗     
╚══██╔══╝██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║     
   ██║   ██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║     
   ██║   ██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║     
   ██║   ██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗
   ╚═╝   ╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝
*/
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <windows.h>
#include <fstream>
#include <iomanip>  
#define FOREGROUND_MODE 0
#define BACKGROUND_MODE 1
using namespace std;

class Shell {
private:
    bool running;                                                   //trạng thái của shell
    bool type;                                                      //kiểu thực thi FOREGROUND_MODE: đơn tiến trình, BACKGROUND_MODE: đa tiến trình
    string path;                                                    //đường dẫn folder shell quản lý
    map<string, string> environment_path;                           //danh sách biến môi trường
    vector<pair<PROCESS_INFORMATION, string>> processes;            //danh sách các tiến trình đã thực hiện <id - name>
    vector<pair<PROCESS_INFORMATION, string>> processes_background; //danh sách các tiến trình thực hiện chế độ background <id - name>
    vector<vector<string>> list_cmd;

    //phân tích lệnh
    vector<string> parse_cmd(string input) {
        for (char &c : input) c = tolower(c);
        vector<string> result;
        istringstream iss(input);
        string token;
        while (iss >> token) result.push_back(token);
        return result;
    }
    //tạo tiến trình args mới, lưu trữ thông tin tiến trình vào pi
    bool newProcess(vector<string> args, PROCESS_INFORMATION &pi) {
        string cmdLine;
        for (const auto& arg : args) {
            cmdLine += arg + " ";
        }
        STARTUPINFOA si = { sizeof(si) };
        char cmd[1024];
        strcpy_s(cmd, cmdLine.c_str());
        if (!CreateProcess(NULL, cmd, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
            for (auto val : environment_path) {
                string fullPath = val.second + "\\" + args[0];
                strcpy_s(cmd, fullPath.c_str());
                if (CreateProcess(NULL, cmd, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
                    return true;
                    break;
                }
            }
        } else {
            return true;
        }
        return false;
    }
    //kiểm tra file *.bat
    bool is_bat_file(const string& filename) {
        size_t pos = filename.find_last_of('.');
        return pos != string::npos && filename.substr(pos) == ".bat";
    } 
    //chuyển đổi từ string sang int, nếu lỗi thì trả về -1
    int s2i(string str) {
        try {
            return std::stoi(str);
        } catch (const std::invalid_argument& e) {
            return -1;
        } catch (const std::out_of_range& e) {
            return -1;
        }
    }
    //thực thi câu lệnh
    void execute(string input) {
        if (input.empty()) return;
        vector<string> cmd = parse_cmd(input);
        list_cmd.clear();
        //if (cmd.empty()) return;
        if (is_bat_file(cmd[0])) {
            string path = "../Storage/" + cmd[0];
            ifstream file(path);
            if (!file.is_open()) {
                cerr << "Cannot open file: " << cmd[0] << endl;
                return;
            }
            string line;
            while (getline(file, line)) {
                vector<string> cmd_line = parse_cmd(line);
                list_cmd.push_back(cmd_line);
            }
            file.close();
        } else {
            list_cmd.push_back(cmd);
        }

        for (auto& args: list_cmd) {
            if (args.size() == 1) {
                if (args[0] == "help") {
                    cout << endl;
                    cout << "WELCOME TO MY SHELL\n"
                         << "For more information on a specific command, type HELP command-name\n"
                         << "CLEAR        Clear console\n"
                         << "DIR          List the contents of the Shell management directory\n"
                         << "EXIT         Exit my Shell\n"
                         << "HELP         Print this help\n"
                         << "HISTORY      List all executed processes\n"
                         << "KILL         Kill processes\n"
                         << "LIST         List all background process\n"
                         << "PATH         List all path\n"
                         << "SWITCH       Switch <foreground mode>/<background mode>\n"
                         << "<cmd>        Execute the program given by <cmd>\n"
                         << endl;
                    return;
                } else if (args[0] == "history") {
                    if (processes.empty()) {
                        cout << "(empty)" << endl;
                        return;
                    }
                    vector<int> space = {20, 20, 20, 20, 20, 20, 20};
                    cout << left  
                        << setw(space[0]) << "Index" 
                        << setw(space[1]) << "Process ID" 
                        << setw(space[2]) << "Thread ID"
                        << setw(space[3]) << "hProcess"
                        << setw(space[4]) << "hThread"
                        << setw(space[5]) << "Name"
                        << endl;
                    for (int i = 0; i < processes.size(); i++) {
                        cout << left 
                            << setw(space[0]) << i 
                            << setw(space[1]) << processes[i].first.dwProcessId
                            << setw(space[2]) << processes[i].first.dwThreadId
                            << setw(space[3]) << processes[i].first.hProcess
                            << setw(space[4]) << processes[i].first.hThread
                            << setw(space[5]) << processes[i].second
                            << endl;
                    }
                    return;
                } else if (args[0] == "dir") {
                    string searchPath = path + "\\*"; 
                    WIN32_FIND_DATAA findData;
                    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);
                    if (hFind == INVALID_HANDLE_VALUE) {
                        cerr << "Not found" << endl;
                        return;
                    }
                    do {
                        if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0) {
                            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                                cout << "File: " << findData.cFileName << endl;
                            } else {
                                cout << "Folder: " << findData.cFileName << endl;
                            }
                        }
                    } while (FindNextFileA(hFind, &findData));
                    FindClose(hFind);
                    return;
                } else if (args[0] == "path") {
                    vector<int> space = {20, 50};
                    cout << left 
                        << setw(space[0]) << "VARIABLE" 
                        << setw(space[1]) << "VALUE" 
                        << endl;
                    for (auto element : environment_path) {
                        cout << left 
                             << setw(space[0]) << element.first
                             << setw(space[1]) << element.second
                             << endl;
                    }
                    return;
                } else if (args[0] == "clear") {
                    system("cls");
                    return;
                } else if (args[0] == "switch") {
                    if (type == FOREGROUND_MODE) {
                        type = BACKGROUND_MODE;
                        cout << "background mode" << endl;
                    } else {
                        type = FOREGROUND_MODE;
                        cout << "foreground mode" << endl;
                    }
                    return;
                } else if (args[0] == "list") {
                    if (processes_background.empty()) {
                        cout << "(empty)" << endl;
                        return;
                    }
                    vector<int> space = {20, 20, 20, 20, 20, 20, 20};
                    cout << left  
                         << setw(space[0]) << "Index" 
                         << setw(space[1]) << "Process ID" 
                         << setw(space[2]) << "Thread ID"
                         << setw(space[3]) << "hProcess"
                         << setw(space[4]) << "hThread"
                         << setw(space[5]) << "Name"
                         << endl;
                    for (int i = 0; i < processes_background.size(); i++) {
                        cout << left 
                             << setw(space[0]) << i
                             << setw(space[1]) << processes_background[i].first.dwProcessId
                             << setw(space[2]) << processes_background[i].first.dwThreadId
                             << setw(space[3]) << processes_background[i].first.hProcess
                             << setw(space[4]) << processes_background[i].first.hThread
                             << setw(space[5]) << processes_background[i].second
                             << endl;
                    }
                    return;
                }
                
                PROCESS_INFORMATION pi;    
                if (newProcess(args, pi)) {
                    processes.push_back(make_pair(pi, args[0]));
                    if (type == FOREGROUND_MODE) {
                        HANDLE hCtrlCThread = CreateThread(NULL, 0, handler_Signal, &pi, 0, NULL);
                        WaitForSingleObject(pi.hProcess, INFINITE);
                        TerminateThread(hCtrlCThread, 0);
                        CloseHandle(hCtrlCThread);
                    } else { 
                        processes_background.push_back(make_pair(pi, args[0]));
                    }
                    CloseHandle(pi.hProcess);
                    CloseHandle(pi.hThread);
                } else {
                    string cmd = "";
                    for (string arg : args) cmd += arg;
                    cerr << "Command '" << cmd << "' not found! Try 'help'" << endl;
                    return;
                }
            } else { 
                //Hướng dẫn chi tiết hơn về các lệnh
                if (args[0] == "help" && args.size() == 2) { 
                    if (args[1] == "path") {
                        cout << "PATH                     List all path\n"
                            << "PATH -a VARIABLE VALUE   Add new path\n"
                            << "PATH -d VARIABLE         Delete path\n" 
                            << endl;
                        return;
                    }
                    else if (args[1] == "kill" && args.size() == 2) {
                        cout << "KILL k      Kill background processes with Index is k\n"
                             << "KILL ALL    Kill all background processes\n" 
                             << endl;
                        return;
                    } else {
                        cerr << "Command " << input << " not found! Try 'help'" << endl;
                        return; 
                    }
                }
                //Mở rộng các lệnh
                else if (args[0] == "path") { 
                    if (args[1] == "-a" && args.size() == 4) {
                        string variable = args[2];
                        string value = args[3];
                        if (environment_path.find(variable) == environment_path.end()) {
                            environment_path[variable] = value;
                        } else {
                            char key;
                            printf("Value '%s' already exists, do you want to overwrite? [Y/n]", variable.c_str());
                            cin >> key;
                            if (key == 'Y' || key == 'y') {
                                environment_path[variable] = value;
                                cout << "Overwritten successfully." << endl;
                            } else {
                                cout << "No changes made." << endl;
                            }
                        }
                        return;
                    } else if (args[1] == "-d" && args.size() == 3) {
                        string variable = args[2];
                        environment_path.erase(variable);
                        return;
                    } else {
                        cout << "Command" << input << " not founnd! Try 'help path'" << endl;
                        return;
                    }
                }
                else if (args[0] == "kill") {
                    if (args[1] == "all" && args.size() == 2) {
                        for (auto& process : processes_background) {
                            TerminateProcess(process.first.hProcess, 0);
                        }
                        return;
                    } else {
                        int k = s2i(args[1]);
                        if (k == -1) {
                            cerr << "Command " << input << " not found! Try 'help kill" << endl;
                            return;
                        }
                        TerminateProcess(processes_background[k].first.hProcess, 0);
                        processes_background.erase(processes_background.begin() + k);
                        return;
                    }
                }
            }
        }
        return;
    }

    void setup() {
        running = true;
        type = FOREGROUND_MODE;
        ifstream inFile("env.txt");
        if (!inFile) {
            cerr << "Cannot open env.txt" << endl;
            return;
        }
        string line;
        while (getline(inFile, line)) {
            istringstream iss(line);
            string variable, value;
            if (iss >> variable >> value) {
                environment_path[variable] = value;
            }
        }
        inFile.close();
        path = environment_path["STORAGE"];
    }
    void savePath() {
        ofstream outFile("env.txt");
        if (!outFile) {
            cerr << "Cannot open file for writing" << endl;
            return;
        }
        for (const auto& pair : environment_path) {
            outFile << pair.first << " " << pair.second << "\n";
        }
        outFile.close();
    }
public:
    void run() {
        setup();
        while (running) {
            cout << "myShell>";
            string input; 
            if (cin.fail()) {
                cin.clear(); 
                cin.ignore(INT_MAX, '\n');
                continue;
            }
            getline(cin, input);
            if (input.empty()) continue;
            if (input == "exit") {
                cout << "Kill all process" << endl;
                for (auto& process : processes) {
                    TerminateProcess(process.first.hProcess, 0);
                }
                running = false;
                break;
            }
            execute(input);
        }
        savePath();         
        return;
    }

    static DWORD WINAPI handler_Signal(LPVOID lpParam) {
        PROCESS_INFORMATION* pi = (PROCESS_INFORMATION*)lpParam;
        while (true) {
            if (GetAsyncKeyState(VK_CONTROL) & 0x8000 && GetAsyncKeyState('C') & 0x8000) {
                cout << "Kill process" << endl;
                TerminateProcess(pi->hProcess, 0);
                break;
            }
            Sleep(100);
        }
        return 0;
    }
};


BOOL WINAPI CtrlHandler(DWORD ctrlType) {
    if (ctrlType == CTRL_C_EVENT) {
        return TRUE;
    }
    return FALSE;
}

int main() {
    string user = "abc";                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
    string key = "123456789";
    SetConsoleCtrlHandler(CtrlHandler, TRUE);                                                                                                                                                                                                                          key = "123456"; user = "nqd";
    Shell myShell;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 //cout << "user name: "; cin >> user; cout << "password: "; cin >> key; if (user != "nqd" || key != "123456") while(1) cout << "Maked by Nguyen Quang Dong & Nguyen Khong Duy Hoang" << endl; system("cls");
    myShell.run();
    cout << "done" << endl;
}