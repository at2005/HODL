#pragma once
#include <string>
#include <math.h>
using namespace std;
// HELPER FUNCTIONS TO CONVERT BETWEEN NUMBER BASES

// reverse characters in a string
string reverse_string(string& str) {
	// find the middle character in a string
	int median = floor((str.size()-1) / 2);
	
	// iterate over all values from begin to middle
	for (int character = 0; character <= median; character++) {
		// swap with corresponding value on the "other side" of the middle character
		swap(str[character], str[(str.size()-1) - character]);
	}

	// return string
	return str;
}

// convert integer to binary string -> Base 10 to Base 2
// takes in integer and padding (adds leading zeroes)
string to_binary(unsigned long long base_10, unsigned long long padding = 0) {
	// add leading zeroes
	string padding_string = "";
	for (int i = 0; i < padding; i++) {
		padding_string += "0";
	}

	// binary string
	string binary = "";
	
	// case for integer being zero
	if (base_10 == 0) return binary += "0";
	
	// while the number is greater than zero
	while (base_10 > 0) {
		// append the number mod 2 to the binary string
		binary += to_string(base_10 % 2);
		// divide the number by 2
		base_10 /= 2;
	}

	// reverse bitstring
	reverse_string(binary);
	// add leading zeroes (padding)
	padding_string += binary;
	// return full bitstring
	return padding_string;
	
}

// to decimal from binary -> Base 2 to Base 10
unsigned long long to_decimal(string bitstring) {
	// our decimal value
	unsigned long long decimal = 0;
	
	// iterator for bitstring
	int i = 0;

	// iterate over each power of 2 in reverse, e.g. 2^(n-1), 2^(n-2) ... 2^(0)
	for (int power_of_two = bitstring.size() - 1; power_of_two > -1; power_of_two--) {
		decimal += (int)bitstring[i] * pow(2, power_of_two);
		// increment iterator for bitstring. Therefore, as we are moving forward in our 
		//bitstring we can decrement our indices
		i++;
	}

	// return decimal value
	return decimal;
}

