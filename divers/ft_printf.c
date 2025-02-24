#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>


int ft_printf(const char *str, ...);
void put_str(char *str, int *counter);
void put_nbr(int n, int *counter);
void put_hexa(int n, int *counter);
void	ft_putchar(char c);
void	ft_putstr(char *str);
int	ft_strlen(char *str);
int ft_nbrlen(int n);
void ft_putnbr(int n);


int main(){
	
	ft_printf("%s\n", "toto");
	
	ft_printf("Magic %s is %d\n", "number", 42);

	ft_printf("Hexadecimal for %d is %x  ", 42, 42);
}


int ft_printf(const char *str, ...){
	va_list list;
	int i = -1;
	int counter = 0;

	if (!str)
		return (counter - 1);
	va_start(list, str);
	while (str[++i])
	{
		if (str[i] == '%' && str[i + 1]){
			i++;
			switch (str[i])
			{
			case 's':
				put_str(va_arg(list, char *), &counter);
				break;
			case 'd':
				put_nbr(va_arg(list, int), &counter);
				break;
			case 'x':
				put_hexa(va_arg(list, int), &counter);
				break;
			default:
				break;
			}
		}else {
			write(1, &str[i], 1);
			counter++;
		}
	}
	va_end(list);
	return (counter);
}

void put_str(char *str, int *counter){
	if (!str)
	{
		ft_putstr("(null)");
		(*counter) += 6;
		return ;
	}
	for(int i = 0; i < ft_strlen(str) + 1; i++){
		write(1, &str[i], 1);
		(*counter)++;
	}
}

void put_nbr(int n, int *counter){
	if (n == -2147483648){
		ft_putstr("-2147483648");
		(*counter) += 11;
	}
	else {
		ft_putnbr(n);
		(*counter) += ft_nbrlen(n);
	}
	return ;
}

void put_hexa(int n, int *counter){
	char *base = "0123456789abcdef";

	if (n > 16)
		put_hexa(n / 16, counter);
	ft_putchar(base[n % 16]);
	(*counter)++;
}

void	ft_putchar(char c){
	write(1, &c, 1);
}
void	ft_putstr(char *str){
	write(1, str, ft_strlen(str));
}
int	ft_strlen(char *str){
	int i = 0;
	while (str[i])
		i++;
	return (i);
}

int ft_nbrlen(int n){
	int len = 0;
	if (n < 0){
		len = 1;
		n = -n;
	}
	while(n){
		len++;
		n /= 10;
	}
	return len;
}

void ft_putnbr(int n){
	if (n == -2147483648){
		write(1, "-2147483648", ft_strlen("-2147483648"));
	}else if (n < 0){
		ft_putchar('-');
		ft_putnbr(-n);
	}else if (n > 9){
		ft_putnbr(n / 10);
		ft_putnbr(n % 10);
	}else{
		ft_putchar(n + '0');
	}
}