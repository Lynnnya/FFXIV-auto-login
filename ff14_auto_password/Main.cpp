#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <string>
#include <fstream>
using namespace std;

string password;
const char* launcher_path;

void launch_game() {
	INPUT input[2];
	ZeroMemory(input, sizeof(input));
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_RETURN;
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = VK_RETURN;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(ARRAYSIZE(input), input, sizeof(INPUT));
}

void write_password() {
	INPUT* input = new INPUT[(password.length() + 1) * 2];
	ZeroMemory(input, (password.length() + 1) * 2 * sizeof(INPUT));

	for (int i = 0; i < password.length(); i++) {
		input[i * 2].type = INPUT_KEYBOARD;
		input[i * 2].ki.wScan = password[i];
		input[i * 2].ki.dwFlags = KEYEVENTF_UNICODE;
		input[i * 2 + 1].type = INPUT_KEYBOARD;
		input[i * 2 + 1].ki.wScan = password[i];
		input[i * 2 + 1].ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_UNICODE;
	}

	input[password.length() * 2].type = INPUT_KEYBOARD;
	input[password.length() * 2].ki.wVk = VK_RETURN;
	input[password.length() * 2 + 1].type = INPUT_KEYBOARD;
	input[password.length() * 2 + 1].ki.wVk = VK_RETURN;
	input[password.length() * 2 + 1].ki.dwFlags = KEYEVENTF_KEYUP;

	SendInput((password.length() + 1) * 2, input, sizeof(INPUT));
}

void open_FF14() {
	HINSTANCE status = ShellExecuteA(NULL, "open", launcher_path, NULL, NULL, SW_SHOWDEFAULT);

	cout << status << endl;
}

void create_file(string file_name, string content) {
	ofstream file;
	file.open(file_name, ios::out);
	file << content;
	file.close();
}

void read_password() {
	ifstream file;
	file.open("password.txt", ios::in);
	if (!file.is_open()) {
		string password;
		cout << "Please enter your password: " << endl;
		string password_file = "password.txt";
		getline(cin, password);
		create_file(password_file, password);
		read_password();
		return;
	}
	file >> password;
	file.close();
}

string read_path() {
	ifstream file;
	string p_str;
	file.open("launcher_path.txt", ios::in);
	if (!file.is_open()) {
		cout << "Please enter the full file path of your game launcher, make sure to double backslash before every subfolder: " << endl;
		string path_file = "launcher_path.txt";
		getline(cin, p_str);
		create_file(path_file, p_str);
		return p_str;
	}
	getline(file, p_str);
	file.close();
	return p_str;
}

int main() {
	string p_str = read_path();
	launcher_path = p_str.c_str();
	read_password();
	open_FF14();
	Sleep(10000);
	write_password();
	Sleep(5000);
	launch_game();
}