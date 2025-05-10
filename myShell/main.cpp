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
    bool running; //trạng thái của shell
    bool type;  //kiểu thực thi FOREGROUND_MODE: đơn tiến trình, BACKGROUND_MODE: đa tiến trình
    string path; //đường dẫn folder shell quản lý
    vector<PROCESS_INFORMATION> processes;
    map<string, string> environment_path; //danh sách biến môi trường
    vector<PROCESS_INFORMATION> processes_background;
    vector<vector<string>> list_cmd;

    vector<string> parse_cmd(string input) {
        for (char &c : input) c = tolower(c);
        vector<string> result;
        istringstream iss(input);
        string token;
        while (iss >> token) result.push_back(token);
        return result;
    }
    
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

    void execute(string input) {
        vector<string> args = parse_cmd(input);
        if (args.empty()) return;
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
                    cout << "No processes are running" << endl;
                    return;
                }
                vector<int> space = {20, 20, 20, 20, 20};
                cout << left 
                     << setw(space[0]) << "Process" 
                     << setw(space[1]) << "Process ID" 
                     << setw(space[2]) << "Thread ID"
                     << setw(space[3]) << "hProcess"
                     << setw(space[4]) << "hThread"
                     << endl;
                for (int i = 0; i < processes.size(); i++) {
                    cout << left 
                         << setw(space[0]) << i 
                         << setw(space[1]) << processes[i].dwProcessId
                         << setw(space[2]) << processes[i].dwThreadId
                         << setw(space[3]) << processes[i].hProcess
                         << setw(space[4]) << processes[i].hThread
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
                }
                else {
                    type = FOREGROUND_MODE;
                    cout << "foreground mode" << endl;
                }
                return;
            } else if (args[0] == "list") {
                //in tất cả các tiến trình đang chạy ở background mode
                //sử dụng vector<PROCESS_INFORMATION> processes_background để lưu thông tin của các tiến trình
                //for (auto& pi : processes_background) {
                //  //TODO --> tham khao (args[0] == "history")
                //}
            }
            //tạo tiến trình mới
            //chưa xử lý file .bat
            //file .bat chứa nhiều câu lệnh liên tiếp cần thực thi -> dùng vector<vector<string>> list_cmd để lưu
            //Nếu là file .bat
            //   Thực hiện đọc file .bat theo line, chuyển đổi line sang câu lệnh arg bằng hàm parase_cmd(line)
            //   list_cmd.push_back(arg)
            //Không thì list_cmd.push_back(args)

            for (int i = 0; i < list_cmd.size(); i++) {
                PROCESS_INFORMATION pi;    
                if (newProcess(list_cmd[i], pi)) {
                    processes.push_back(pi); //history
                    if (type == FOREGROUND_MODE) {
                        HANDLE hCtrlCThread = CreateThread(NULL, 0, handler_Signal, &pi, 0, NULL);
                        WaitForSingleObject(pi.hProcess, INFINITE);
                        TerminateThread(hCtrlCThread, 0);
                        CloseHandle(hCtrlCThread);
                    } else { 
                        processes_background.push_back(pi);
                    }
                    CloseHandle(pi.hProcess);
                    CloseHandle(pi.hThread);
                } else {
                    cerr << "Command not found! Try 'help'" << endl;
                    return;
                }
            }
            return;
        } else { 
            if (args[0] == "help") { //Hướng dẫn chi tiết hơn về các lệnh
                if (args[1] == "path") {
                    cout << "PATH                     List all path\n"
                         << "PATH -a VARIABLE VALUE   Add new path\n"
                         << "PATH -d VARIABLE         Delete path\n" 
                         << endl;
                    return;
                }
                else if (args[1] == "kill") {
                    //kill id: kết thúc tiến trình id
                    //kill all: kết thúc tất cả các tiến trình background
                } else {
                    cerr << "Command not found! Try 'help path" << endl;
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
                    cout << "Command not founnd! Try 'help path'" << endl;
                    return;
                }
            }
            else if (args[0] == "kill") {
                if (args[1] == "all") {
                    //kết thúc tất cả các tiến trình trong processes_background
                    //dùng vòng for kết hợp lệnh TerminateProcess
                } else if (args[1] == "id") {
                    //chuyển đổi string -> int k
                    //cần xử lý các trường hợp k > processes_background.size(): thông báo process does not exist 
                    //kết thúc tiến trình có stt thứ k, từ stt k ánh xạ trong processes_background để tìm id của tiến trình 
                    //Kết thúc một tiến trình sử dụng lệnh TerminateProcess(id, 0)
                    //id: id của tiến trình
                    //0: để mặc định
                } else {
                    cerr << "Command not found! Try 'help kill" << endl;
                    return;
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
            string input; getline(cin, input);
            if (cin.fail()) {
                cin.clear(); 
                cin.ignore(INT_MAX, '\n');
                continue;
            }
            if (input == "exit") {
                cout << "Kill all process" << endl;
                for (auto& pi : processes) {
                    TerminateProcess(pi.hProcess, 0);
                }
                running = false;
                break;
            }
            execute(input);
        }
        savePath();         
        return;                                                                                                                                                                                                                                                                                                                                                                                  cout << "make by Nguyen Quang Dong & Nguyen Khong Duy Hoang" << endl;
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
    SetConsoleCtrlHandler(CtrlHandler, TRUE);
    Shell myShell;
    myShell.run();
    cout << "done" << endl;
}