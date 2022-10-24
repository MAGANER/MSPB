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
	std::string encrypt_str;
	try {
		// Initialize the encoder The mode selected here is CBC_Mode
		CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption cbc_encription(key_iv.first, key_iv.first.size(), key_iv.second);
		// Set a stream format CryptoPP is a very useful module
			CryptoPP::StreamTransformationFilter stf_encription(cbc_encription,
				// Here the encrypted output stream is encoded in base64, if not needed, directly pass new CryptoPP::StringSink(encrypt_str)
				new CryptoPP::Base64Encoder(new CryptoPP::StringSink(encrypt_str)),
				// Fill mode is padding 0
				CryptoPP::BlockPaddingSchemeDef::ZEROS_PADDING);
		// put will pass the data that needs to be encoded
		stf_encription.Put(reinterpret_cast<const unsigned char*>(text.c_str()), text.length() + 1);
		stf_encription.MessageEnd();
	}
	catch (std::exception e) {
		std::cout << e.what() << std::endl;
	}
	return encrypt_str;
}
std::string Encryption::decrypt(const key_iv& key_iv, const string& cipher)
{
	try {
		// Limited ability, did not find the pre - processing interface of the input stream, here first do base64 decoding
			std::string aes_encrypt_data;
		CryptoPP::Base64Decoder decoder;
		decoder.Attach(new CryptoPP::StringSink(aes_encrypt_data));
		decoder.Put(reinterpret_cast<const unsigned char*>(cipher.c_str()), cipher.length());
		decoder.MessageEnd();

		// There is nothing to say below, similar to AES encryption, get raw data
		std::string decrypt_data;
		CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption cbc_description(key_iv.first, key_iv.first.size(), key_iv.second);
		CryptoPP::StreamTransformationFilter stf_description(cbc_description,
			new CryptoPP::StringSink(decrypt_data), CryptoPP::BlockPaddingSchemeDef::ZEROS_PADDING);
		stf_description.Put(reinterpret_cast<const unsigned char*>(aes_encrypt_data.c_str())
			, aes_encrypt_data.length());
		stf_description.MessageEnd();
		return decrypt_data;
	}
	catch (std::exception e) {
		std::cout << e.what() << std::endl;
		return "";
	}
}
