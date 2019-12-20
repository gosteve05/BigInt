/*Steven Oslan 01396237
This has everything that Daily10 requires. I'm submitting it for Daily9 on 11/8.
Multiplication and division can no longer return -0. Resubmitting 11/17.
*/
#include "BigInt.h"
//#include<cmath>
#include<string>
//#include<vector>
#include<cstring>
using namespace std;

BigInt::BigInt()
{
	data = "0"; //Choose how to store 0
	isNegative = false;
}

BigInt::BigInt(int x)
{
	isNegative = x < 0;
	if (isNegative)
		x *= -1;
	data = to_string(x);
}

BigInt::BigInt(string x) //cannot be called implicitly!
{
	string temp = x;
	while (x.size() > 0 && isspace(x[0])) //while it's not empty and starts with a space
		x.erase(0, 1); //delete one space
	//leading whitespace has been removed
	isNegative = (x[0] == '-');

	if (x[0] == '-' || x[0] == '+')
		x.erase(0, 1); //gets rid of one sign character
	if (x.size() == 0)
	{
		cout << "EXITING" << endl;
		exit(1);
	}
	for (int i = 0; i < x.size(); i++)
		if (!isdigit(x[i]))
		{
			if (isspace(x[i]))
				x.erase(i, x.size() - i);
			else
			{
				cout << "Tried to construct an invalid BigInt from string: \n" << temp << endl << "EXITING" << endl;
				exit(1); //end the program
			}
		}
	data = x;
}
BigInt::BigInt(const BigInt& arg)
{
	cout << "Copy constructor called" << endl; //loud
	data = arg.data;
	isNegative = arg.isNegative;
}
BigInt::BigInt(BigInt&& arg)
{
	cout << "Move constructor called" << endl;
	data = move(arg.data);
	isNegative = arg.isNegative;
}
BigInt BigInt::operator=(const BigInt& arg)
{
	data = arg.data;
	isNegative = arg.isNegative;
	return *this;
}


ostream& operator<<(ostream& out, const BigInt& right)
{
	if (right.isNegative)
		out << '-';
	out << right.data;
	return out;
}

bool BigInt::operator<(const BigInt& right) const //The current code completely trusts that there are no leading zeros.
{
	if (isNegative != right.isNegative)
		return isNegative > right.isNegative; //true if LHS is neg and RHS is pos. otherwise, false as LHS is pos and RHS is neg.

	int dataComp = dataComparison(*this, right);
	if (dataComp == 1)
		return isNegative;
	if (dataComp == -1)
		return !isNegative;
	else
		return false;
}

bool BigInt::operator>(const BigInt& right) const
{
	return right < *this;
}

BigInt BigInt::operator+(const BigInt& right)
{
	BigInt ret;
	if (isNegative == right.isNegative) //they have the same sign
	{
		ret.isNegative = isNegative;
		ret.data = move(stringAdder(*this, right));
		return ret;
	} //they have opposite sign
	int dataComp = dataComparison(*this, right);
	if (isNegative) //negative plus positive. if left.data > right.data, result is negative.
		ret.isNegative = (dataComp == 1);
	else //positive plus negative. if left.data < right.data, result is negative.
		ret.isNegative = (dataComp == -1);
	ret.data = move(stringSubtractor(*this, right));

	return ret;
}

BigInt BigInt::operator-(const BigInt& right)
{
	BigInt ret;
	if (isNegative != right.isNegative)
	{
		ret.isNegative = (isNegative > right.isNegative);
		ret.data = move(stringAdder(*this, right));
		return ret;
	}
	//otherwise they have the same sign
	int d = dataComparison(*this, right);
	ret.isNegative = (d + isNegative == 2 || d + isNegative == -1);
	ret.data = move(stringSubtractor(*this, right));
	return ret;
}

string stringAdder(const BigInt& left, const BigInt& right)
{
	string ret = left.data.size() < right.data.size() ? right.data : left.data;
	string shorter = left.data.size() < right.data.size() ? left.data : right.data;
	int minSize = shorter.size();
	int maxSize = ret.size();
	ret.insert(0, "0");
	shorter.insert(0, 1+maxSize-minSize, '0'); //I commented out the below after submitting. Does it work??
	//for (int i = 0; i <= maxSize - minSize; i++)
	//	shorter.insert(0, "0");
	for (int i = maxSize; i >= 1; i--)//maxSize-minSize ; i--)
	{
		ret[i] += shorter[i] - '0';
		if (ret[i] > '9')
		{
			ret[i] -= 10;
			ret[i - 1]++;
		}
	}
	while (ret[0] == '0' && ret.size() > 1)
		ret.erase(0, 1);
	return ret;
}
string stringSubtractor(const BigInt& left, const BigInt& right)
{
	int dataComp = dataComparison(left, right);
	string ret = dataComp == 1 ? left.data : right.data;
	string shorter = dataComp != 1 ? left.data : right.data;
	int minSize = shorter.size();
	int maxSize = ret.size();
	ret.insert(0, "0");
	for (int i = 0; i <= maxSize - minSize; i++)
		shorter.insert(0, "0");
	for (int i = maxSize; i >= 1; i--)
	{
		ret[i] -= shorter[i] - '0';
		if (ret[i] < '0')
		{
			ret[i] += 10;
			ret[i - 1]--;
		}
	}
	while (ret[0] == '0' && ret.size() > 1)
		ret.erase(0, 1);
	return ret;
}

int dataComparison(const BigInt& left, const BigInt& right)
{
	if (left.data.size() < right.data.size())
		return -1; //left.data<right.data
	else if (left.data.size() > right.data.size())
		return 1; //left.data>right.data
	for (int i = 0; i < left.data.size(); i++)
	{
		if (left.data[i] < right.data[i])
			return -1; //left.data<right.data
		else if (left.data[i] > right.data[i])
			return 1; //left.data>right.data
	}
	return 0; //left.data==right.data
}

bool BigInt::operator==(const BigInt& right) const //Did I break something by adding const here?
{
	return *this < right == right < *this; //there's faster ways, but this is the least code
}


BigInt multHelper(BigInt left, BigInt right) //Preconditions: left and right data are the same length, a power of 2.
{
	int lSize = left.data.size();
	int rSize = right.data.size();
	int maxSize = lSize > rSize ? lSize : rSize;
	int length = 1;
	int temp = maxSize;
	while(temp > 0) //Example: 14->7->3->1->0; 4 steps. But 8->4->2->1->0 is also 4 steps.
	{
		temp >>= 1;
		length <<= 1;
	}
	if(maxSize << 1 == length)
	{
		length >>= 1;
	}
	//pad them with zeros so they're both length 2^n.
	//SEGMENTATION FAULT IF NEXT TWO LINES ARE IN
	left.data.insert(0, length-lSize, '0'); //padded with zeros.
	right.data.insert(0, length-rSize, '0');

	//cout << "left: " << left << "right: " << right << endl;
	if(left.data.size() != right.data.size())
	{

		int diff = left.data.size() - right.data.size();
		cout << "diff: " << diff << endl;
		if(diff > 0)
		{
			right.data.insert(0, diff, '0');
		}
		else
		{
			left.data.insert(0, -diff, '0');
		}
	}
	int leftSize = left.data.size();
	//cout << "LENGTH: " << length << endl;
	if(leftSize == 1) //base case
	{
		//cout << "left: " << left << "right: " << right << endl;;
		int first = left.data[0]-'0';
		int second = right.data[0]-'0';
		//cout << "First: " << first << "; Second: " << second << endl;
		return BigInt(first*second);
	}
	//recursion case
	BigInt lBack;
	lBack.data = left.data.substr(leftSize/2, leftSize/2);
	//cout << "lBack is: " << lBack << endl;
	BigInt lFront;
	lFront.data = left.data.substr(0, leftSize/2);
	//cout << "lFront is: " << lFront << endl;
	BigInt rBack;
	rBack.data = right.data.substr(leftSize/2, leftSize/2);
	//cout << "rBack is: " << rBack << endl;
	BigInt rFront;
	rFront.data = right.data.substr(0, leftSize/2);
	//cout << "rFront is: " << rFront << endl;

	BigInt front = multHelper(lFront, rFront);
	BigInt back = multHelper(lBack, rBack);
	//cout << "lsum: " << lFront+lBack << "rsum: " << rFront+rBack << endl;
	BigInt mid = multHelper(lFront+lBack, rFront+rBack) - front - back;
	//cout << "front: " << front << "; back: " << back << "; mid: " << mid << endl;
	front.data.insert(front.data.length(), leftSize , '0');
	mid.data.insert(mid.data.length(), leftSize/2, '0');
	return front + mid + back;
}

BigInt BigInt::operator*(const BigInt& right)
{
	BigInt copy(right);
	BigInt ret = multHelper(*this, copy);
	ret.isNegative = (isNegative != right.isNegative);
 	return multHelper(*this, copy);
}

BigInt BigInt::operator&&(const BigInt& right) //Slow multiplication
{
	if (data == "0" || right.data == "0")
		return BigInt();
	BigInt ret = BigInt();
	for(int i = 0 ; i < data.size() ; i++)
		for (int j = 0; j < right.data.size(); j++)
		{
			int prod = (data[i]-'0') * (right.data[j]-'0');
			int tens = prod / 10;
			int ones = prod % 10;
			string product;
			product.push_back('0'+tens);
			product.push_back('0'+ones);
			string zeros;
			zeros.insert(0, (data.size() - i - 1) + (right.data.size() - j - 1), '0');
			ret = ret + BigInt(product+zeros);
		}
	ret.isNegative = (isNegative != right.isNegative);
	return ret;
}

BigInt BigInt::operator/(const BigInt& right)
{
	BigInt result;
	BigInt zero;
	BigInt two = BigInt(2);
	if (right == zero)
	{
		cout << "Cannot divide by zero. LHS was: " << *this << endl;
		exit(1);
	}
	if (*this == zero)
		return zero;
	BigInt r = right;
	BigInt t = *this;
	t.isNegative = r.isNegative = false; //Yes, I just did that
	BigInt multiplier;
	int sizeDifference;
	string bigNum; //soon to be followed by a bunch of zeros
	while (r < t && r.data != t.data)
	{
		bigNum = "1";
		sizeDifference = t.data.size() - r.data.size() - 2; //COULD BE -1 if I handle everything really well :) Given a number m digits long and a number m-2 digits long, the m digit number is at least 10* the m-2 digit number
		if (t.data[0] < r.data[0])
			sizeDifference++;
		if(sizeDifference > 0)
			bigNum.insert(1, sizeDifference, '0');
		multiplier = BigInt(bigNum);
		while (r * multiplier * two < t) //messes up the multiplier=1 case; code below handles this
		{
			//cout << "Still working" << r * multiplier << ", " << t << endl;
			multiplier = multiplier * two;
		}
		//cout << "t before: " << t << endl << "x is: " << r * multiplier << endl;
		t = t - r * multiplier; //So instead I'll effectively halve multiplier here
		result = result + multiplier;
		//cout << "Current result: " << result << endl;
		multiplier = 1;
	}
	if (r == t || r < t)
		result = result + 1;
	if (result < zero != zero < result)
		result.isNegative = isNegative != right.isNegative;
	return result;
}
BigInt BigInt::operator%(const BigInt& right)
{
	if (right.data == "0")
	{
		cout << "Error: % by zero!" << endl;
		exit(42);
	}
	BigInt ret(*this - (*this / right) * right);
	if (ret.data == "0")
	{
		ret.isNegative = false; //Amusingly, my test doesn't differentiate these
		return ret;
	}
	ret.isNegative = isNegative;

	if (isNegative > right.isNegative)
		ret.isNegative = true;
	return ret;
}

//I did not use the below version of division.
/*
BigInt BigInt::operator/(const BigInt& right) //binary search.
{
	BigInt result;
	BigInt zero;
	BigInt two = BigInt(2);
	if (right == zero)
	{
		cout << "Cannot divide by zero. LHS was " << *this << endl;
		exit(1);
	}
	//bool isNeg = isNegative != right.isNegative;
	BigInt r = right; //Maybe the spec will allow me to mess with the right that was passed in.
	BigInt t = *this;
	t.isNegative = r.isNegative = false; //Yes, I just did that
	BigInt multiplier;
	while (r < t && r.data != t.data)
	{
		multiplier = BigInt(1);
		while (r * multiplier * two < t) //messes up the multiplier=1 case; code below handles this
		{
			cout << "Still working" << r * multiplier << ", " << t << endl;
			multiplier = multiplier * two;
		}
		cout << "t before: " << t << endl << "x is: " << r * multiplier << endl;
		t = t - r * multiplier; //So instead I'll effectively halve multiplier here
		result = result + multiplier;
		multiplier = 1;
	}
	if (r == t || r < t)
		result = result + 1;
	if (result < zero != zero < result)
		result.isNegative = isNegative != right.isNegative;
	return result;
}
*/
string getRand(int len)
{
	string s;
	for(int i = 0 ; i < len ; i++)
		s.push_back('0'+rand()%10);
	return s;
}
int main()
{
	BigInt x(getRand(4));
	BigInt y(getRand(4));
	BigInt a = x*y;
	cout << "Finished fast" << endl;
	/*BigInt b = x&&y;
	cout << "Finished slow" << endl;
	if(a==b)
		cout << "SUCCESS";
	else
		cout << "FAILURE";*/
	cout << "Finished!" << endl;
	return 1;
}