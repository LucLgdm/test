#include <iostream>
using namespace std;
#include <bits/stdc++.h>

void print_bits(unsigned char octet) {
	cout << setw(3) << (int)octet << " = ";
	for(int i = 7; i >= 0; i--)	cout << ((octet & (1 << i)) ? "1" : "0");
	cout << endl;
}

int main() {
	int n = 0;
	
	n = n | 2;
	print_bits(n);
	n = n | 4;
	print_bits(n);
	n = n | 8;
	print_bits(n);
	
	cout << endl;
	n = 73;
	int m = 255;
	print_bits(n); print_bits(m);
	cout << "n | m : "; print_bits(n | m);
	cout << "n & m : "; print_bits(n & m);
	cout << "n ^ m : "; print_bits(n ^ m);
	
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
	return 0;
}
