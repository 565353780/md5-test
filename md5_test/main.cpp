#include <iostream>
#include <string.h>
#include <openssl/md5.h>


// source getMD5, only save data to unsigned char *md5
int sourceGetMD5(const char* data, unsigned char *md5, const int &mode) {
  if (mode == 0) {
    MD5(reinterpret_cast<const unsigned char*>(data), strlen(data), md5);

    return 1;
  }
  else if (mode == 1) {
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, data, strlen(data));
    MD5_Final(md5, &ctx);

    return 1;
  }

  return 0;
}

std::string getMD5(const unsigned char* data, const int &data_size, const int &mode=0) {
  unsigned char md5[MD5_DIGEST_LENGTH];

  if (mode == 0) {
    MD5(data, data_size, md5);
  }
  else if (mode == 1) {
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, data, data_size);
    MD5_Final(md5, &ctx);
  }

  std::string md5_string = "";
  for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
    md5_string += md5[i];
  }

  return md5_string;
}

std::string getMD5(const char* data, const int &data_size) {
  return getMD5(reinterpret_cast<const unsigned char*>(data), data_size);
}

std::string getMD5(const std::string &data, const int &data_size) {
  return getMD5(data.c_str(), data_size);
}

std::string HexToString(const unsigned char *md5) {
  std::string md5_hex;
  const char map[] = "0123456789abcdef";
  for (size_t i = 0; i < MD5_DIGEST_LENGTH; ++i) {
    md5_hex += map[md5[i] / 16];
    md5_hex += map[md5[i] % 16];
  }
  return md5_hex;
}

int mystrlen(const unsigned char* str)
{
	int i = 0;
	while('\0' != str[i]) {
    ++i;
  }
	return i;
}

std::string getUnsignedChar(const unsigned char* data, const int& size) {
  std::string data_string = "";
  for (int i = 0; i < size; ++i) {
    if (i > 0) {
      data_string += " ";
    }
    data_string += std::to_string(int(data[i]));
  }
  return data_string;
}

bool checkSize(const unsigned char* data) {
  return (int)strlen((const char*)data) == mystrlen(data);
}

bool checkSize(const char* data) {
  return (int)strlen(data) == mystrlen((unsigned char*)data);
}

int outputUnsignedChar(const unsigned char* data) {
  if (!checkSize(data)) {
    std::cout << "[ERROR][main::outputUnsignedChar]\n";
    std::cout << "\t size not equal: " << (int)strlen((const char*)data) << " vs " << mystrlen(data) << std::endl;
    return -1;
  }

  std::cout << "\t malloc at : " << &data << std::endl;
  std::cout << "\t size : " << strlen((const char*)data) << std::endl;
  std::cout << "\t data : '" << data << "'" << std::endl;
  std::cout << "\t data->int : '" << getUnsignedChar(data, strlen((const char*)data)) << "'" << std::endl;

  std::string data_hex = HexToString(data);
  std::cout << "\t hex : " << data_hex << std::endl;
  return 1;
}

int outputChar(const char* data) {
  if (!checkSize(data)) {
    std::cout << "[ERROR][main::outputUnsignedChar]\n";
    std::cout << "\t size not equal: " << (int)strlen(data) << " vs " << mystrlen((const unsigned char*)data) << std::endl;
    return -1;
  }

  std::cout << "\t malloc at : " << &data << std::endl;
  std::cout << "\t size : " << strlen(data) << std::endl;
  std::cout << "\t data : '" << data << "'" << std::endl;
  std::cout << "\t data->int : '" << getUnsignedChar((const unsigned char*)data, strlen(data)) << "'" << std::endl;

  std::string data_hex = HexToString((const unsigned char*)data);
  std::cout << "\t hex : " << data_hex << std::endl;
  return 1;
}



int testMD5(const std::string &data, const int &mode) {
  std::cout << "[TEST][main::testMD5]\n";
  std::cout << "\t data : " << data << std::endl;

  unsigned char md5[MD5_DIGEST_LENGTH];

  std::cout << "[TEST][md5 malloc]\n";
  if (!outputUnsignedChar(md5)) {
    return -1;
  }

  const char *p = data.c_str();

  std::cout << "[TEST][const char *p malloc]\n";
  if (!outputChar(p)) {
    return -1;
  }

  sourceGetMD5(p, md5, mode);

  std::cout << "[TEST][getMD5 finished]\n";
  if (!outputUnsignedChar(md5)) {
    return -1;
  }

  const char *value = reinterpret_cast<const char*>(md5);

  std::cout << "[TEST][convert md5 to char]\n";
  if (!outputChar(value)) {
    return -1;
  }

  std::cout << "[TEST][convert md5 to string]\n";
  std::string value_string = "";
  for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
    value_string += md5[i];
  }

  std::cout << "\t string size : " << value_string.size() << std::endl;
  std::cout << "\t string : '" << value_string.c_str() << "'" << std::endl;
  std::cout << "\t string->int : " << getUnsignedChar((const unsigned char*)(value_string.c_str()), value_string.size()) << std::endl;

  std::cout << "[TEST][convert string to md5-string]\n";
  std::string md5_string = getMD5(data, data.size());

  std::cout << "\t string size : " << md5_string.size() << std::endl;
  std::cout << "\t string : '" << md5_string.c_str() << "'" << std::endl;
  std::cout << "\t string->int : " << getUnsignedChar((const unsigned char*)(md5_string.c_str()), md5_string.size()) << std::endl;

  return 1;
}

int main(int argc, char *argv[]) {
  const std::string data_str1 = "$d1$e1$getdataforrange";
  const std::string data_str2 = "$d1$e1$getlastdayagg";
  const std::string data_str3 = "$d1$e1$getlastmonthagg";
  const std::string data_str4 = "$d1$e1$get\0lastmonthagg";

  std::cout << "---------------------------------\n";
  testMD5(data_str4, 0);
  std::cout << "---------------------------------\n";
  testMD5(data_str4, 0);
  std::cout << "---------------------------------\n";
  return 0;
}
