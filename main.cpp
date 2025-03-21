/*
████████╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗     
╚══██╔══╝██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║     
   ██║   ██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║     
   ██║   ██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║     
   ██║   ██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗
   ╚═╝   ╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝
Chương trình tinyShell là một chương trình shell đơn giản, có khả năng thực thi các lệnh cơ bản như:
Nhóm lệnh hệ thống:
- exit: thoát khỏi chương trình.
- help: hiển thị hướng dẫn sử dụng.
- date: hiển thị ngày hiện tại.
- time: Hiển thị thời gian hiện tại.
- list: Hiển thị danh sách tiến trình tinyShell quản lý.
- kill <PID>: Kết thúc tiến trình có PID là <PID>.
- killall: Kết thúc tất cả tiến trình tinyShell đang quản
- clear: Xóa màn hình Shell
- path
- cd <path>: Di chuyển đến thư mục <path>
- pwd: Hiển thị đường dẫn thư mục hiện tại
- mkdir <path>: Tạo thư mục mới tại <path>
- rmdir <path>: Xóa thư mục tại <path>
- cp <source> <destination>: Sao chép file từ <source> đến <destination>
- mv <source> <destination>: Di chuyển file từ <source> đến <destination>
- rm <path>: Xóa file tại <path>
- ls: Hiển thị danh sách file và thư mục trong thư mục hiện tại

Nhóm lệnh thực thi:
- run <path>: Thực thi chương trình ở đường dẫn <path>

Chuyển chế độ sử dụng:
- mode foreground: Chuyển chế độ sử dụng là foreground
- mode background: Chuyển sang chế độ sử dụng là background  

Nhóm lệnh ẩn từ bàn phím:
- Ctrl + C: Kết thúc tiến trình đang chạy
- Ctrl + Q: Thoát tinyShell

*/
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <windows.h>
#include "Shell.h"
using namespace std;

int main() {
    
}