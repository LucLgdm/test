#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>


void ft_put_str(const char *str, int *counter){
	int i = -1;

	while(str[++i]){
		write(1, &str[i], 1);
		(*counter)++;
	}
}

void ft_put_nbr(int d, int *counter){
	if (d == -2147483648){
		write(1, "-2147483648", 11);
		(*counter) += 11;
		return ;
	}
	if (d < 0){
		d = -d;
		write(1, "-", 1);
		(*counter)++;
	}
	if (d > 9){
		ft_put_nbr(d / 10, counter);
		ft_put_nbr(d % 10, counter);
	}else{
		char c = d + '0';
		write(1, &c, 1);
		(*counter)++;
	}
}

void ft_put_hexa(int d, int *counter){
	char *base = "0123456789abcdef";
	if (d >= 16)
		ft_put_hexa(d / 16, counter);
	write(1, &base[d % 16], 1);
	(*counter)++;
}



int ft_printf(const char *str, ...){
	va_list list;
	int i = 0;
	int counter = 0;
	int d;
	char *s;

	va_start(list, str);
	while(str[i]){
		if (str[i] == '%' && str[i + 1]){
			switch (str[i + 1]){
				case 's':
					s = va_arg(list, char *);
					ft_put_str(s, &counter);
					i += 2;
					break;
				case 'd':
					d = va_arg(list, int);
					ft_put_nbr(d, &counter);
					i += 2;
					break;
				case 'x':
					d = va_arg(list, int);
					ft_put_hexa(d, &counter);
					i += 2;
					break;
			}
		}
		else{
			write(1, &str[i], 1);
			counter++;
			i++;
		}
	}
	return counter;
}


int main(){
	int mine, orig;

	mine = ft_printf("%s\n", "toto");
	orig = printf("%s\n", "toto");
	if(mine == orig)
		ft_printf("1, TRUE\n");

	mine = ft_printf("Magic %s is %d\n", "number", 42);
	orig = printf("Magic %s is %d\n", "number", 42);
	if(mine == orig)
		ft_printf("2, TRUE\n");

	mine = ft_printf("Hexadecimal for %d is %x\n", 32, 32);
	orig = printf("Hexadecimal for %d is %x\n", 32, 32);
	if(mine == orig)
		ft_printf("3, TRUE\n");
	return 0;
}
