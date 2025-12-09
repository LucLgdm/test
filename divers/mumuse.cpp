#include <bits/stdc++.h>

void print_bits(unsigned char octet) {
	std::cout << std::setw(3) << (int)octet << " = ";
	for(int i = 7; i >= 0; i--)	std::cout << ((octet & (1 << i)) ? "1" : "0");
	std::cout << std::endl;
}

int main() {
	{
		int yes;
		std::cout << "1/0 pour tester les operateurs de bits : ";
		std::cin >> yes;
		if (yes){
			int num;
			std::cout << "\x1b[38;5;3mEntre un nombre et ca te donne le signe\033[0m" << std::endl;
			std::cin >> num;
			if (num & (1 << 31))
				std::cout << num << " est negatif" << std::endl;
			else
				std::cout << num << " est positif" << std::endl;
			
			std::cout << "\x1b[38;5;3mOn echange 2 nombres : \033[0m" << std::endl;
			int a, b;
			std::cout << "a = ";
			std::cin >> a;
			std::cout << "b = ";
			std::cin >> b;
			// Intervertir 2 nombres
			print_bits(a); print_bits(b);
			a = a^b;
			std::cout << "a = a^b = "; print_bits(a);
			b = a^b;
			std::cout << "b = a^b = "; print_bits(b);
			a = a^b;
			std::cout << "a = a^b = "; print_bits(a);
			std::cout << "\x1b[38;5;2ma = " << a << " b = " << b << "\033[0m" << std::endl;
			
			std::cout << std::endl;
			std::cout << "\x1b[38;5;3mDivers operateurs de bitshifting \033[0m" << std::endl;
			int n = 73;
			int m = 255;
			print_bits(n); print_bits(m);
			std::cout << "n | m : "; print_bits(n | m);
			std::cout << "n & m : "; print_bits(n & m);
			std::cout << "n ^ m : "; print_bits(n ^ m);
			std::cout << std::endl;
		}
	}
	{
		std::vector<char *> vec = {"HHello", "World", "Hello", "Mumu", "World", "C++", "Mumu"};
		std::sort(vec.begin(), vec.end());
		auto it = unique(vec.begin(), vec.end());
		vec.erase(it, vec.end());
		for (auto v : vec)
			std::cout << v << " ";
		std::cout << std::endl;

	}
	return 0;
}
