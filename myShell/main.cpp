// ████████╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗     
// ╚══██╔══╝██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║     
//    ██║   ██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║     
//    ██║   ██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║     
//    ██║   ██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗
//    ╚═╝   ╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝
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



static bool running;                                     //trạng thái của shell
static bool type;                                        //FOREGROUND_MODE/BACKGROUND_MODE
static string path;                                      //đường dẫn folder shell quản lý
static vector<vector<string>> list_cmd;                  //danh sách lệnh thực thi
static map<string, string> environment_path;             //danh sách biến môi trường
static vector<PROCESS_INFORMATION> processes;            //danh sách các tiến trình đã thực hiện <id - name>
static vector<string> name_process;
static vector<PROCESS_INFORMATION> processes_background; //danh sách các tiến trình thực hiện chế độ background <id - name>
static vector<string> name_process_background;

class Shell {
private:
    //phân tích lệnh
    vector<string> parse_cmd(string input, bool lower = true) {
        for (char &c : input) c = tolower(c);
        vector<string> result;
        istringstream iss(input);
        string token;
        while (iss >> token) result.push_back(token);
        return result;
    }
    //tạo tiến trình args mới, lưu trữ thông tin tiến trình vào pi, nếu tạo thành công trả về true, thất bại trả về false
    bool CreateNewProcess(vector<string> args, PROCESS_INFORMATION &pi) {
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
                cout << line << endl;                               
            }
            file.close();
            printf("Do you want excute? [Y/n]: ");
            string confirm;
            getline(cin, confirm);
            if (confirm.empty() || tolower(confirm[0]) != 'y') return;
        } else {                                                    
            list_cmd.push_back(cmd);                                    
        }
        PROCESS_INFORMATION pi;                                     
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
                            << setw(space[1]) << processes[i].dwProcessId
                            << setw(space[2]) << processes[i].dwThreadId
                            << setw(space[3]) << processes[i].hProcess
                            << setw(space[4]) << processes[i].hThread
                            << setw(space[5]) << name_process[i]
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
                             << setw(space[1]) << processes_background[i].dwProcessId
                             << setw(space[2]) << processes_background[i].dwThreadId
                             << setw(space[3]) << processes_background[i].hProcess
                             << setw(space[4]) << processes_background[i].hThread
                             << setw(space[5]) << name_process_background[i]
                             << endl;
                    }
                    return;
                }
                //nếu như không phải phải nằm trong nhóm câu lệnh hệ thống, thì sẽ là câu lệnh thực thi chương trình
                if (CreateNewProcess(args, pi)) {                                                       
                    processes.push_back(pi);                                        
                    name_process.push_back(args[0]);
                    if (type == FOREGROUND_MODE) {                                                      
                        HANDLE hCtrlCThread = CreateThread(NULL, 0, handler_signal, &pi, 0, NULL);          
                        WaitForSingleObject(pi.hProcess, INFINITE);                                         
                        TerminateThread(hCtrlCThread, 0);                                                   
                        CloseHandle(hCtrlCThread);
                        CloseHandle(pi.hProcess);
                        CloseHandle(pi.hThread);
                    } else {                                                                            
                        PROCESS_INFORMATION* pInfo = new PROCESS_INFORMATION(pi);
                        HANDLE hCtrlCThread = CreateThread(NULL, 0, handler_finish, pInfo, 0, NULL);          
                        processes_background.push_back(pi);                            
                        name_process_background.push_back(args[0]);
                    }
                } else {                                                                           
                    string cmd = "";                                                               
                    for (string arg : args) cmd += arg;
                    cerr << "Command '" << cmd << "' not found! Try 'help'" << endl;
                    return;
                }
            } else { 
                if (args[0] == "help" && args.size() == 2) { 
                    if (args[1] == "path") {
                        cout << "PATH                     List all path\n"
                             << "PATH -a VARIABLE VALUE   Add new path\n"
                             << "PATH -d VARIABLE         Delete path\n" 
                             << endl;
                        return;
                    } else if (args[1] == "kill") {
                        cout << "KILL IDX    Kill background processes with Index is IDX\n"
                             << "KILL ALL    Kill all background processes\n" 
                             << endl;
                        return;
                    } else if (args[1] == "clear") {
                        cout << "CLEAR        Clear console\n"
                             << endl;
                        return;
                    } else if (args[1] == "dir") {
                        cout << "DIR          List the contents of the Shell management directory\n"
                             << endl;
                        return;
                    } else if (args[1] == "exit") {
                        cout << "EXIT         Exit my Shell\n"
                             << endl;
                        return;
                    } else if (args[1] == "history") {
                        cout << "HISTORY      List all executed processes\n"
                             << endl;
                        return;
                    } else if (args[1] == "list") {
                        cout << "LIST         List all background process\n"
                             << endl;
                        return;
                    } else if (args[1] == "switch") {
                        cout << "SWITCH       Switch <foreground mode>/<background mode>\n"
                             << "foreground mode: must wait for previous process to finish, Ctrl+C to cancel\n"
                             << "background mode: processes will run in the background\n"
                             << endl;
                        return;
                    } else if (args[1] == "<cmd>") {
                        cout << "<cmd>        Execute the program given by <cmd>\n"
                             << "Note: only *.exe and *.bat files can be executed!\n"
                             << endl;
                        return;
                    } else {
                        cerr << "Command " << input << " not found! Try 'help'" << endl;
                        return; 
                    }
                }
                //Mở rộng các lệnh
                //nhóm lệnh path
                else if (args[0] == "path") { 
                    if (args[1] == "-a" && args.size() == 4) {                                                      
                        string variable = args[2]; 
                        string value = args[3];
                        if (environment_path.find(variable) == environment_path.end()) {                            
                            environment_path[variable] = value;
                        } else {                                                                                    
                            printf("Value '%s' already exists, do you want to overwrite? [Y/n]: ", variable.c_str());
                            string confirm;
                            getline(cin, confirm);
                            if (confirm.empty() || tolower(confirm[0]) != 'y') return;

                            environment_path[variable] = value;
                            return;
                        }
                        return;
                    } else if (args[1] == "-d" && args.size() == 3) {                                              
                        string variable = args[2];
                        if (environment_path.find(variable) == environment_path.end()) {                            
                            cout << "Variable don't exist! Try 'path'" << endl;
                        } else {                                                                                   
                            printf("Are you sure? [Y/n]: ");
                            char key; cin >> key;
                            if (tolower(key) != 'y') return;
                            environment_path.erase(variable);
                        }
                        return;
                    } else {                                                                                       
                        cout << "Command" << input << " not founnd! Try 'help path'" << endl;
                        return;
                    }
                }
                //nhóm lệnh kill
                else if (args[0] == "kill") {
                    if (args[1] == "all" && args.size() == 2) {                                                     
                        printf("Are you sure? [Y/n]: ");
                        string confirm;
                        getline(cin, confirm);
                        if (confirm.empty() || tolower(confirm[0]) != 'y') return;

                        for (auto& pi : processes_background) {
                            TerminateProcess(pi.hProcess, 0);
                        }
                        processes_background.clear();
                        name_process_background.clear();
                        return;
                    } else {                                                                                       
                        int k = s2i(args[1]);                                                                          
                        if (k == -1 || k >= processes_background.size()) {                                                                                  
                            cerr << "Command " << input << " not found! Try 'help kill' or 'list'" << endl;
                            return;
                        }                                                                                               
                        
                        printf("Are you sure? [Y/n]: ");
                        string confirm;
                        getline(cin, confirm);
                        if (confirm.empty() || tolower(confirm[0]) != 'y') return;
                        
                        TerminateProcess(processes_background[k].hProcess, 0);                                        
                        processes_background.erase(processes_background.begin() + k);                                      
                        name_process_background.erase(name_process_background.begin() + k);
                        return;
                    }
                }
            }
        }
        return;
    }
    //giai đoạn thiết lập chung
    void setup() {
        running = true;                                 
        type = FOREGROUND_MODE;                             
        ifstream inFile("env.txt");                         
        if (!inFile) {                                      
            cerr << "Cannot open env.txt" << endl;                
            exit(1);                                              
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
    //xử lý sự kiện Ctrl+C
    static DWORD WINAPI handler_signal(LPVOID lpParam) {            
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
    //Xử lý trạng thái processes_background khi tiến trình kết thúc
    static DWORD WINAPI handler_finish(LPVOID lpParam) {            
        PROCESS_INFORMATION* pi = (PROCESS_INFORMATION*)lpParam;    
        WaitForSingleObject(pi->hProcess, INFINITE);
        for (size_t i = 0; i < processes_background.size(); ++i) {
            if (processes_background[i].dwProcessId == pi->dwProcessId) {
                processes_background.erase(processes_background.begin() + i);
                name_process_background.erase(name_process.begin() + i);
                break;
            }
        }
    
        CloseHandle(pi->hProcess);
        CloseHandle(pi->hThread);
        delete pi;
        return 0;
    }
public:
    //chạy Shell
    void run() {
        setup();                                                    
        while (running) {
            cout << "myShell>";
            string input; 
            if (cin.fail()) {                                       
                cin.clear();                                           
                cin.ignore(INT_MAX, '\n');                              
                cout << endl;
                continue;
            }
            getline(cin, input);
            if (input.empty()) continue;    
            if (input == "exit") {                                  
                cout << "Kill all process" << endl;                    
                for (auto& process : processes_background) {
                    TerminateProcess(process.hProcess, 0);
                }
                running = false;
                break;
            }
            execute(input);                                        
        }
        savePath();                                                 
        return;
    }
};

//bình thường Ctrl+C thì hệ thống mặc định sẽ đóng console, cần đăng ký sự kiện này để hệ điều hành không đóng
BOOL WINAPI CtrlHandler(DWORD ctrlType) {
    if (ctrlType == CTRL_C_EVENT) {
        return TRUE;
    }
    return FALSE;
}

int main() {
    string user = "abc";                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
    string key = "123456789";
    //
    SetConsoleCtrlHandler(CtrlHandler, TRUE);             //Đăng ký xử lý sự kiện Ctrl+C                                                                                                                                                                                                              key = "123456"; user = "nqd";
    Shell myShell;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 //cout << "user name: "; cin >> user; cout << "password: "; cin >> key; if (user != "nqd" || key != "123456") while(1) cout << "Maked by NQD & NKDH & HHD" << endl; system("cls");
    myShell.run();
    cout << "done" << endl;
}