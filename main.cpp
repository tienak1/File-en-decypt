#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <functional>
#include <string>
#include <unordered_set>
#include <iomanip>
#include <math.h>

//#define infilename "d:\\temp\\my_avatar.jpg"
//#define outfilename "d:\\temp\\my_avatar.jpg"

using namespace std;

void encrypt(char* text, int len, string key, char* outfilename);
void decrypt(char* text, int len, string key, char* outfilename);
string hashfnc(string input);
string random(int n);
void check_password();


int main() {

	check_password();
	int choose;
	do
	{
		cout << "CHUONG TRINH MA HOA VA GIAI MA FILE" << endl;
		cout << "Chuc nang:\n";
		cout << "1. Ma hoa file.\n";
		cout << "2. Giai ma file.\n";
		cout << "3. Thoat.\n";
		cout << "Xin moi chon: ";
		cin >> choose;
		cin.ignore();
		if (choose == 1)
		{
			char filename_encrypt[50];
			cout << "Nhap duong dan den file can ma hoa: ";
			cin.getline(filename_encrypt, 50);
			cout << endl;
			
			string key;
			cout << "Enter password: ";
			cin.ignore();
			getline(cin, key);
			key = hashfnc(key);
			cout << key << endl;
			ifstream f_in(filename_encrypt, ios::ate | ios::binary);
			if (f_in.fail()) {
				cout << "Could not open the file\n";
				continue;
			}
			int filesize = f_in.tellg();
			f_in.seekg(0, ios::beg);
			char* content = new char[filesize];
			if (!content) {
				cout << "Empty File\n";
				f_in.close();
				continue;
				//return;
			}
			f_in.read((char*)content, filesize * sizeof(int));
			encrypt(content,filesize,key,filename_encrypt);
			f_in.close();
			delete[] content;
		}
		else if (choose == 2)
		{
			char filename_decrypt[50];
			cout << "Nhap duong dan den file can ma hoa: ";
			cin.getline(filename_decrypt, 50);
			cout << endl;
			string password;
			cout << "Enter password: ";
			cin.ignore();
			getline(cin, password);
			cout << endl;
			password = hashfnc(password);
			cout << password << endl;


			ifstream f_in(filename_decrypt, ios::ate | ios::binary);
			if (f_in.fail()) {
				cout << "Could not open the file\n";
				continue;
			}
			int filesize = f_in.tellg();
			f_in.seekg(0, ios::beg);
			char* content = new char[filesize];
			if (!content) {
				cout << "Empty File\n";
				f_in.close();
				continue;
				//return;
			}
			f_in.read((char*)content, filesize * sizeof(int));
			//cout << filesize << endl;
			//cout << content << endl << endl;
			decrypt(content, filesize, password,filename_decrypt);
			f_in.close();
			delete[] content;
		}
	} while (choose != 3);


	return 0;
}

string random(int n) {
	srand(time(NULL));
	string result = "";
	if (n == 0) {
		return result;
	}
	char letters[26] = {
	   'a', 'b', 'c', 'd', 'e', 'f', 'g',
	   'h', 'i', 'j', 'k', 'l', 'm', 'n',
	   'o', 'p', 'q', 'r', 's', 't', 'u',
	   'v', 'w', 'x', 'y', 'z'
	};
	for (int i = 0; i < n; i++) {
		result = result + letters[rand() % 26];
	}
	return result;
}

void encrypt(char* text, int len, string key, char* outfilename) {

	//cout << "len: " << len << endl;
	//cout << "text: " << text << endl;
	srand(time(NULL));
	int size_k = key.length();

	string iv = random(size_k);
	//cout << "iv: " << iv << endl;

	unsigned int pad = rand() % 30;
	string s_pad = random(pad);

	//cout << "n_pad: " << pad << endl;
	//cout << "s_padding: " << s_pad << endl;

	int n_pad = (pad ^ 2) + 1;

	ofstream f_out(outfilename, ios::binary);
	if (f_out.fail()) return;
	f_out << (char)n_pad;
	f_out << iv;

	int i = 0, j = 0, k = 0, p = 0;
	for (; i <= size_k; i++) {
		if (i == size_k) {
			i = 0;
		}
		if (j < len) {
			int tmp = (text[j] ^ key[i]) ^ iv[i];
			f_out << (char)tmp;
			//s += (char)tmp;
			iv[i] = (char)tmp;
			j++;
			continue;
		}
		if (k < size_k) {
			int tmp = (key[k] ^ key[i]) ^ iv[i];
			f_out << (char)tmp;
			//s += (char)tmp;
			iv[i] = (char)tmp;
			k++;
			continue;
		}
		if (p < pad) {
			int tmp = (s_pad[p] ^ key[i]) ^ iv[i];
			f_out << (char)tmp;
			//cout << (char)tmp;
			//s += (char)tmp;
			iv[i] = (char)tmp;
			p++;
			continue;
		}
		break;
	}
	cout << "====================================\n";
	cout << "Da ma hoa \n";
	cout << "====================================\n";
	//cout << endl << "s: " << s << endl;
	//f_out.write((char*)text, len * sizeof(int));
	//f_out.close();
//	cout << text[0] << endl << text[1] << endl << text[2] << endl;
}

void decrypt(char* text, int len, string key, char* outfilename) {

	int size_k = key.length();
	//cout << "len: " << len << endl;
	//int mod = (len - 1) % size_k;
	//string iv="";
	//for (int i = 0; i < mod; i++) {
	//	iv += text[len - mod + i];
	//}

	//cout << iv << endl;
	//cout << iv.length() << endl;
	string content = "";
	string key_val = "";
	//cout << key[0] << "\t" << key[1] << "\t" << key[2] << "\t" << key[3] << "\t" << key[4] << "\t"<< key[5] << "\t" << endl;
	unsigned int n_pad = ((int)text[0] - 1) ^ 2;
	//cout << "n_pad: " << n_pad << endl;
	//string r="";
	int mod = (len - 1 - n_pad) % size_k;
	//cout << mod << endl;
	int l = len - 1 - n_pad;
	int cnt = size_k;
	for (int i = mod - 1; i >= 0; i--) {
		//cout << s[l] << "\t" << s[l - size_k] << "\t" << key[i] << endl;
		//int tmp = (s[l] ^ s[l - size_k]) ^ key[i];
		int tmp = (text[l] ^ text[l - size_k]) ^ key[i];
		//cout << (char)tmp << endl;
		l--;
		char letter = (char)tmp;
		//cout << "letter: " << letter << endl;
		if (cnt > 0)
		{
			if (letter != key[--cnt]) {
				cout << "====================================\n";
				cout << "Password Incorrect!!!" << endl;
				cout << "====================================\n";
				return;
			}
		}
		else {
			content = letter + content;
		}

		if (i == 0)
			i = size_k;
		if (l <= size_k)
			break;
	}
	ofstream f_out(outfilename, ios::binary);
	if (f_out.fail()) return;
	f_out << content;
	//cout << endl << "content: " << content << endl;
	cout << "====================================\n";
	cout << "Da giai ma file\n";
	cout << "==================================\n\n";
	f_out.close();
}

string hashfnc(string input) {
	hash<string> hash_fn;
	size_t str_hash = hash_fn(input);
	stringstream ss;
	ss << hex << str_hash;
	string result = ss.str();

	return result;
}

void check_password() {
	do {
		int num;
		unsigned seed = time(0);
		srand(seed);
		num = 100 + rand() % 99;
		cout << "Xin hay xac thuc de su dung dich vu. Mat khau la: " << num << endl;
		string result;
		cout << "Xin hay nhap: ";
		getline(cin, result);
		int d = num % 10, c = (num / 10) % 10, t = num / 100;
		string ans = "matkhaula" + to_string((int)pow(t, d) + c) + to_string((int)pow(c, t) + (int)pow(c, d)) + to_string((int)pow(d, t) + c);
		if (result == ans)
		{
			cout << "Hay tiep tuc su dung dich vu";
			break;
		}
		else {
			cout << "ans: " << ans << endl;
		}
	} while (true);
}