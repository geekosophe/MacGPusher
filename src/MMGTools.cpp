#include "MMGTools.hpp"
#include <cstdio>
#include <cstring>


#define INT_DIGITS 19


/**
 * @brief Convert a std::string to an integer
 * @param str [in] : String to convert
 * @return the string as an integer
 */
const int MMGTools::StringToInteger(const std::string& str)
{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-value"
	char* s = const_cast<char*>(str.c_str());
	int res = 0, n = 1;
	unsigned int c;
	if (*s == '-')
	{
		n = -1;
		*s++;
	}
	while ((unsigned int)(c = (unsigned int)*s++ - 48) < 10u)
		res = res * 10 + (int)c;
	return res * n;
#pragma clang diagnostic pop
}

/**
 * @brief Convert a std::string to an unsigned integer
 * @param str [in] : String to convert
 * @return the string as an unsigned integer
 */
const unsigned int MMGTools::StringToUnsignedInteger(const std::string& str)
{
	const char* s = str.c_str();
	unsigned int res = 0;
	unsigned int c;
	while ((unsigned int)(c = (unsigned int)*s++ - 48) < 10u)
		res = res * 10 + c;
	return res;
}

/**
 * @brief Convert an integer to a std::string, Taken from : http://opensource.apple.com/source/groff/groff-10/groff/libgroff/itoa.c
 * @param i [in] : Integer to convert
 * @return The integer as a std::string
 */
const std::string MMGTools::IntegerToString(int i)
{
	static char buf[INT_DIGITS + 2];
	char* p = buf + INT_DIGITS + 1;
	if (i >= 0)
	{
		do
		{
			*--p = 48 + (i % 10);
			i /= 10;
		} while (i != 0);
		return std::string(p);
	}
	else
	{
		do
		{
			*--p = 48 - (i % 10);
			i /= 10;
		} while (i != 0);
		*--p = '-';
	}
	return std::string(p);
}

/**
 * @brief Convert an unsigned integer to a std::string, Taken from : http://opensource.apple.com/source/groff/groff-10/groff/libgroff/itoa.c
 * @param i [in] : Unsigned integer to convert
 * @return The unsigned integer as a std::string
 */
const std::string MMGTools::UnsignedIntegerToString(unsigned int i)
{
	static char buf[INT_DIGITS + 2];
	char* p = buf + INT_DIGITS + 1;
	do
	{
		*--p = 48 + (i % 10);
		i /= 10;
	} while (i != 0);
	return std::string(p);
}

/**
 * @brief Convert a float to a std::string
 * @param f [in] : Float to convert
 * @return The float as a std::string
 */
const std::string MMGTools::FloatToString(const float f)
{
	char array[32] = {0x00};
	sprintf(array, "%f", f);
	return std::string(array);
}

/**
 * @brief Split a std::string into a std::vector<std::string>
 * @param str [in] : String to split
 * @param vec [out] : Vector where to split the string
 * @param sep [in] : String separator
 */
void MMGTools::StringToVector(const std::string& str, std::vector<std::string>& vec, const char* sep)
{
	char* cStr = const_cast<char*>(str.c_str());
	char* p = strtok(cStr, sep);
	while (p)
	{
		vec.push_back(p);
		p = strtok(NULL, sep);
	}
}

/**
 * @brief Replace a part of a std::string with another
 * @param str [out] : String
 * @param from [in] : String to search
 * @param to [in] : String to replace
 * @return true if success, false otherwise
 */
const bool MMGTools::StringReplace(std::string& str, const std::string& from, const std::string& to)
{
	const std::string::size_type startPos = str.find(from);
	if (startPos == std::string::npos)
		return false;
	str.replace(startPos, from.length(), to);
	return true;
}

/**
 * @brief Decode a base64 encoded std::string
 * @param encoded_string [in] : Base64 encoded string
 * @return decode std::string
 */
const std::string MMGTools::Base64Decode(const std::string& encoded_string)
{
	static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	std::string::size_type in_len = encoded_string.size();
	std::string::size_type in_ = 0;
	int i = 0, j = 0;
	unsigned char uchar_array_4[4], uchar_array_3[3];
	std::string ret;

	auto isbase64 = [](const unsigned char c) -> bool { return (isalnum(c) || (c == '+') || (c == '/'));};
	while ((in_len--) && (encoded_string[in_] != '=') && (isbase64((unsigned char)encoded_string[in_])))
	{
		uchar_array_4[i++] = static_cast<unsigned char>(encoded_string[in_]);
		in_++;
		if (i == 4)
		{
			for (i = 0; i < 4; i++)
				uchar_array_4[i] = static_cast<unsigned char>(base64_chars.find(static_cast<char>(uchar_array_4[i])));

			uchar_array_3[0] = static_cast<unsigned char>((uchar_array_4[0] << 2) + ((uchar_array_4[1] & 0x30) >> 4));
			uchar_array_3[1] = static_cast<unsigned char>(((uchar_array_4[1] & 0xf) << 4) + ((uchar_array_4[2] & 0x3c) >> 2));
			uchar_array_3[2] = static_cast<unsigned char>(((uchar_array_4[2] & 0x3) << 6) + uchar_array_4[3]);

			for (i = 0; (i < 3); i++)
				ret += static_cast<char>(uchar_array_3[i]);
			i = 0;
		}
	}

	if (i)
	{
		for (j = i; j < 4; j++)
			uchar_array_4[j] = 0;
		
		for (j = 0; j < 4; j++)
			uchar_array_4[j] = static_cast<unsigned char>(base64_chars.find(static_cast<char>(uchar_array_4[j])));

		uchar_array_3[0] = static_cast<unsigned char>((uchar_array_4[0] << 2) + ((uchar_array_4[1] & 0x30) >> 4));
		uchar_array_3[1] = static_cast<unsigned char>(((uchar_array_4[1] & 0xf) << 4) + ((uchar_array_4[2] & 0x3c) >> 2));
		uchar_array_3[2] = static_cast<unsigned char>(((uchar_array_4[2] & 0x3) << 6) + uchar_array_4[3]);
		
		for (j = 0; (j < i - 1); j++)
			ret += static_cast<char>(uchar_array_3[j]);
	}

	return ret;
}