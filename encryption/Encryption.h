/*
	special module to provide easy AES/RSA encrypting, SHA-256 
	and convertation between hex and decimal numbers
*/
#pragma once
#include"rijndael.h"
#include"osrng.h"
#include"modes.h"
#include "filters.h"
#include "cryptlib.h"
#include"sha.h"
#include<utility>
#include<string>
#include<vector>
#include<iostream>

namespace Encryption
{
	using namespace CryptoPP;
	using namespace std;

	typedef pair<SecByteBlock, SecByteBlock> key_iv;
	string encrypt(const key_iv& key_iv, const string& text);
	string decrypt(const key_iv& key_iv, const string& cipher);
	
};