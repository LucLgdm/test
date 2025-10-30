/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-merc <lde-merc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:27:41 by fatkeski          #+#    #+#             */
/*   Updated: 2025/10/30 12:11:02 by lde-merc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bigint.hpp"

int main(void)
{
	const Bigint a(57);
	Bigint b(43), c, d(1337), e(d);

	// base test
	cout << "a = " << a << endl;
	cout << "b = " << b << endl;
	cout << "c = " << c << endl;
	cout << "d = " << d << endl;
	cout << "e = " << e << endl;

	cout << "a + b = " << a + b << endl;
	cout << "a + c = " << a + c << endl;
	cout << "(c += a) = " << (c += a) << endl;

	cout << "b = " << b << endl;
	cout << "++b = " << ++b << endl;
	cout << "b++ = " << b++ << endl;
	cout << "b = " << b << endl;
	
	// b = 23, b << 10 -> 230000000000 + 42 = 230000000042
	cout << "(b << 10) + 42 = " << ((b << 10) + 42) << endl;
	cout << "b >> 9 = " << (b >> 9) << endl;
	cout << "b <<= 4 = " << (b <<= 4) << endl;
	cout << "b >> 3 = " << (b >> 3) << endl;
	cout << "(d <<= 4) = " << (d <<= 4) << ", d = " << d << endl;
	cout << "(d >>= 2) = " << (d >>= (const Bigint)2) << ", d: " << d << endl;

	cout << "a = " << a << endl; // a = 42
	cout << "d = " << d << endl; // d = 5348

	cout << "(d < a) = " << (d < a) << endl; // (d < a) = 0
	cout << "(d > a) = " << (d > a) << endl; // (d > a) = 1
	cout << "(d == d) = " << (d == d) << endl; // (d == d) = 1
	cout << "(d == a) = " << (d == a) << endl; // (d == d) = 1
	cout << "(d != a) = " << (d != a) << endl; // (d != a) = 1
	cout << "(d <= a) = " << (d <= a) << endl; // (d <= a) = 0
	cout << "(d >= a) = " << (d >= a) << endl; // (d >= a) = 1

	// extra
	Bigint x(12345678); Bigint y(5);
	cout << "(x << y) = " << (x << y) << ", x: " << x << ", y: " << y << endl;
	cout << "(x >>= y) = " << (x >>= y) << ", x: " << x << ", y: " << y << endl;
	cout << "(x >= y) = " << (x >= y) << ", x: " << x << ", y: " << y << endl;

	cout << "x= " << (x <<= 5) << ", y= " << (y <<= 12) << endl;
	cout << "(x >= y) = " << (x >= y) << ", x: " << x << ", y: " << y << endl;

	return (0);
}
