#include <iostream>
using namespace std;
#include <bits/stdc++.h>

void print_bits(unsigned char octet) {
	cout << setw(3) << (int)octet << " = ";
	for(int i = 7; i >= 0; i--)	cout << ((octet & (1 << i)) ? "1" : "0");
	cout << endl;
}

int main() {
	int yes;

	cout << "bits ? 1/0" << endl;
	cin >> yes;
	if (yes) {
		int num;
		cout << "\x1b[38;5;3mEntre un nombre et ca te donne le signe\033[0m" << endl;
		cin >> num;
		if (num & (1 << 31))
			cout << num << " est negatif" << endl;
		else
			cout << num << " est positif" << endl;
		
		cout << "\x1b[38;5;3mOn echange 2 nombres : \033[0m" << endl;
		int a, b;
		cout << "a = ";
		cin >> a;
		cout << "b = ";
		cin >> b;
		// Intervertir 2 nombres
		print_bits(a); print_bits(b);
		a = a^b;
		cout << "a = a^b = "; print_bits(a);
		b = a^b;
		cout << "b = a^b = "; print_bits(b);
		a = a^b;
		cout << "a = a^b = "; print_bits(a);
		cout << "\x1b[38;5;2ma = " << a << " b = " << b << "\033[0m" << endl;
		
		cout << endl;
		cout << "\x1b[38;5;3mDivers operateurs de bitshifting \033[0m" << endl;
		int n = 73;
		int m = 255;
		print_bits(n); print_bits(m);
		cout << "n | m : "; print_bits(n | m);
		cout << "n & m : "; print_bits(n & m);
		cout << "n ^ m : "; print_bits(n ^ m);
		cout << endl;
	}

	cout << "Couleur ? 1/0" << endl;
	cin >> yes;
	if (yes) {
		for(int i = 1; i < 250; i += 10) {
			cout << setw(5) << left << "\x1b[38;5;" << i << "mHello World!\033[m" << " : " << setw(3) << i << " ";
			cout << setw(5) << left << "\x1b[38;5;" << i + 1 << "mHello World!\033[m" << " : " << setw(3) << i + 1 << " ";
			cout << setw(5) << left << "\x1b[38;5;" << i + 2 << "mHello World!\033[m" << " : " << setw(3) << i + 2 << " ";
			cout << setw(5) << left << "\x1b[38;5;" << i + 3 << "mHello World!\033[m" << " : " << setw(3) << i + 3 << " ";
			cout << setw(5) << left << "\x1b[38;5;" << i + 4 << "mHello World!\033[m" << " : " << setw(3) << i + 4 << " ";
			cout << setw(5) << left << "\x1b[38;5;" << i + 5 << "mHello World!\033[m" << " : " << setw(3) << i + 5 << " ";
			cout << setw(5) << left << "\x1b[38;5;" << i + 6 << "mHello World!\033[m" << " : " << setw(3) << i + 6 << " ";
			cout << setw(5) << left << "\x1b[38;5;" << i + 7 << "mHello World!\033[m" << " : " << setw(3) << i + 7 << " ";
			cout << setw(5) << left << "\x1b[38;5;" << i + 8 << "mHello World!\033[m" << " : " << setw(3) << i + 8 << " ";
			cout << setw(5) << left << "\x1b[38;5;" << i + 9 << "mHello World!\033[m" << " : " << setw(3) << i + 9 << endl;
		}
		for(int i = 0; i < 5; i++) {
			cout << setw(5) << left << "\x1b[38;5;" << 250 + i << "mHello World!\033[m" << " : " << setw(3) << 250 + i << " ";
		}
		cout << endl;
		cout << "Pour surligner le texte, changer le 38 en 48 !" << endl;

		// 0-15 : couleurs système
		cout << "=== Couleurs de base (0–15) ===" << endl;
		for (int i = 0; i < 16; i++) {
			cout << "\x1b[48;5;" << i << "m"
				<< setw(4) << i
				<< "\033[m ";
		}
		cout << "\n" << endl;

		// 16-231 : cube de couleurs 6×6×6
		cout << "=== Cube 6x6x6 (16-231) ===" << endl;
		for (int g = 0; g < 6; g++) {
			for (int r = 0; r < 6; r++) {
				for (int b = 0; b < 6; b++) {
					int i = 16 + (36 * r) + (6 * g) + b;
					cout << "\x1b[48;5;" << i << "m"
						<< setw(4) << i
						<< "\033[m ";
				}
				cout << "  ";
			}
			cout << endl;
		}
		cout << "\n" << endl;

		// 232-255 : niveaux de gris
		cout << "=== Niveaux de gris (232-255) ===" << endl;
		for (int i = 232; i < 256; i++) {
			cout << "\x1b[48;5;" << i << "m"
				<< setw(4) << i
				<< "\033[m ";
		}
		cout << "\n" << endl;
	}

	cout << "\x1b[38;5;3mMumuse est termine, au boulot !!\033[0m" << endl;
	return 0;
}
