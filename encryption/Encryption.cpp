#include"Encryption.h"
using namespace Encryption;

/////////////////
///**************
//these 2 functions were taken from my old project
//so i think it's better to keep it as is,
//but they could be better
string Encryption::convert_bytes(const SecByteBlock& bytes)
{
	string curr = (const char*)bytes.data();
	string new_key;
	for (size_t i = 0; i < curr.size(); ++i)
	{
		auto ch = curr[i];
		int code = (int)ch;
		new_key += to_string(code) + '|';
	}
	return new_key;
}
CryptoPP::SecByteBlock Encryption::convert_to_bytes(const string& key)
{
	string bytes;

	string curr_ch;
	for (auto& ch : key)
	{
		if (ch != '|')curr_ch += ch;
		else
		{
			bytes += (char)atoi(curr_ch.c_str());
			curr_ch.clear();
		}
	}

	SecByteBlock _key(reinterpret_cast<CryptoPP::byte*>(&bytes[0]), CryptoPP::AES::DEFAULT_KEYLENGTH);
	return _key;
}
///**************
/////////////////
std::string Encryption::encrypt(const key_iv& key_iv, const string& text)
{
	if (text.empty())return "";
	try
	{
		CBC_Mode<CryptoPP::AES>::Encryption e;
		e.SetKeyWithIV(key_iv.first, key_iv.first.size(), key_iv.second);

		string _text = text;
		//while ((_text.size() % 16) != 0)_text +=(char)0;

		string cipher;
		StringSource s(_text, true,
			//new HexEncoder(
			new StreamTransformationFilter(e,
				new StringSink(cipher)
			) // StreamTransformationFilter
		//)
		); // StringSource

		return cipher;
	}
	catch (const Exception& e)
	{
		cout <<"AES::encryption:"<< e.what();
		exit(-1);
	}
}
std::string Encryption::decrypt(const key_iv& key_iv, const string& cipher)
{
	if (cipher.empty())return "";
	try
	{
		CBC_Mode<CryptoPP::AES>::Decryption d;
		d.SetKeyWithIV(key_iv.first, key_iv.first.size(), key_iv.second);

		string recovered;
		StringSource s(cipher, true,
			//new CryptoPP::HexDecoder(
			new StreamTransformationFilter(d,
				new StringSink(recovered)
			) // StreamTransformationFilter
		//)
		); // StringSource
		return recovered;
	}
	catch (const Exception& e)
	{
		cout << "AES::decryption:" << e.what();
		return "-1";
	}
}
