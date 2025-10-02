# TinyShell

TinyShell là một **command-line shell** được phát triển bằng **C++** trên nền tảng **Windows**. Shell này hỗ trợ thực thi các lệnh cơ bản, quản lý tiến trình foreground và background, cũng như quản lý biến môi trường.

---

## Tính năng chính

- Thực thi các lệnh **.exe** và **.bat**.
- Quản lý tiến trình **foreground** và **background**:
  - Foreground: chờ tiến trình kết thúc, hỗ trợ Ctrl+C để dừng tiến trình.
  - Background: tiến trình chạy ngầm, có thể `stop`, `resume` hoặc `kill`.
- Quản lý **biến môi trường** với lệnh `PATH`.
- Lưu và nạp cấu hình từ file `env.txt`.
- Các lệnh hệ thống:
  - `help` – Hiển thị hướng dẫn.
  - `clear` – Xóa console.
  - `dir` – Liệt kê nội dung thư mục quản lý.
  - `exit` – Thoát shell và dừng tất cả tiến trình background.
  - `history` – Liệt kê các tiến trình đã thực hiện.
  - `list` – Liệt kê các tiến trình background đang chạy.
  - `switch` – Chuyển giữa **foreground mode** và **background mode**.
  - `kill <IDX|all>` – Dừng tiến trình background theo chỉ số hoặc tất cả.
  - `stop <IDX|name>` – Tạm dừng tiến trình background.
  - `resume <IDX|name>` – Tiếp tục tiến trình background.

---

## Yêu cầu hệ thống

- **Hệ điều hành:** Windows 10 trở lên
- **Compiler:** Visual Studio, MinGW, hoặc bất kỳ compiler C++ hỗ trợ Windows API
- **C++ Standard:** C++11 trở lên

---

## Cài đặt

1. Clone repository:

```bash
git clone https://github.com/DongNQ225284/TinyShell
```

2. Biên dịch project với compiler C++ hỗ trợ Windows:

```bash
g++ main.cpp -o myShell.exe -static
```

3. Chuẩn bị file `env.txt` chứa biến môi trường

ví dụ:

```yaml
STORAGE C:\MyShell\Storage
PATH C:\Windows\System32
```

4. Tạo thư mục Storage chứa các file `.bat` hoặc các chương trình muốn thực thi.
   Cách sử dụng

## Cách sử dụng

1. Chạy MyShell:

```bash
myShell.exe
```

Giao diện shell:

```bash
myShell>
```

Ví dụ lệnh:

```bash
myShell> dir
myShell> help
myShell> path -a MYVAR C:\MyFolder
myShell> <program.exe>
myShell> switch
myShell> list
myShell> stop 0
myShell> resume 0
myShell> kill all
myShell> exit
```

## Quản lý tiến trình

### Foreground mode:

Tiến trình phải kết thúc trước khi shell chấp nhận lệnh tiếp theo. `Ctrl+C` sẽ hủy tiến trình hiện tại.

### Background mode:

Tiến trình chạy ngầm, bạn có thể:

```bash
stop <IDX|name>   # Dừng tiến trình
resume <IDX|name> # Tiếp tục tiến trình
kill all          # Kết thúc tất cả các tiến trình
list              # Liệt kê các tiến trình đang chạy
```

## Quản lý PATH

```bash
path                    # Liệt kê biến môi trường
path -a VARIABLE VALUE  # Thêm biến
path -d VARIABLE        # Xóa biến:
```

## File cấu hình

`env.txt` – Lưu các biến môi trường.

Thư mục Storage (mặc định) – Chứa các file `.bat` hoặc chương trình cần chạy.

## Ghi chú

Chỉ hỗ trợ Windows do phụ thuộc Windows API (CreateProcess, SuspendThread, ResumeThread, ...).

File `.bat` sẽ được đọc từ các thư mục trong `env.txt` và yêu cầu xác nhận trước khi thực thi.

Các tiến trình background tự động được xóa khỏi danh sách khi kết thúc.
