#include"Encryption.h"
using namespace Encryption;

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
