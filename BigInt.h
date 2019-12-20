#include <iostream>
using namespace std;
class BigInt
{
public:
	BigInt();
	BigInt(int x);
	explicit BigInt(string x); //cannot be called implicitly!
	BigInt(const BigInt& arg);
	BigInt(BigInt&& arg); //move copy constructor
	BigInt operator=(const BigInt& arg);
	friend ostream& operator<<(ostream& out, const BigInt& right);
	BigInt operator+(const BigInt& right);
	BigInt operator-(const BigInt& right);
	//BigInt operator*(BigInt right);
	BigInt friend multHelper(BigInt a, BigInt b);
	BigInt operator*(const BigInt& right);
	BigInt operator&&(const BigInt& right);
	BigInt operator/(const BigInt& right);
	BigInt operator%(const BigInt& right);
	bool operator==(const BigInt& right) const;
	bool operator<(const BigInt& right) const;
	bool operator>(const BigInt& right) const;
	friend string stringAdder(const BigInt& left, const BigInt& right);
	friend string stringSubtractor(const BigInt& left, const BigInt& right);
	friend int dataComparison(const BigInt& left, const BigInt& right);
	friend BigInt multiplier(BigInt left, BigInt right);
	string getData() { return data; }
	bool getNegative() { return isNegative; }
private:
	string data;
	bool isNegative;
};