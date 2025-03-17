#include "encryption_utils.h"
#include <iostream>
#include <fstream>
#include <cstring>

namespace tiny_encrypt {
// 计算 CRC32
void EncryptionUtils::encrypt_crc32(const uint8_t *data, size_t len, uint32_t *crc) {
    *crc = crc32(0L, data, len);
}

// 计算 CRC64
void EncryptionUtils::encrypt_crc64(const uint8_t *data, uint64_t len, uint64_t magic, uint64_t *crc) {
    *crc = CRC64(magic, data, len);
}

// 计算 MD5
void EncryptionUtils::encrypt_md5(const uint8_t *data, size_t len, uint8_t key[16]) {
    MD5(data, len, key);
}

// 计算 SHA1
void EncryptionUtils::encrypt_sha1(const uint8_t *data, size_t len, uint8_t key[20]) {
    SHA1(data, len, key);
}

// 计算文件的 MD5
int EncryptionUtils::encrypt_md5_file(const std::string& filename, uint8_t key[16]) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        return -1;
    }
    MD5_CTX md5Context;
    MD5_Init(&md5Context);
    char buffer[1024];
    while (file.read(buffer, sizeof(buffer))) {
        MD5_Update(&md5Context, buffer, sizeof(buffer));
    }
    MD5_Update(&md5Context, buffer, file.gcount());
    MD5_Final(key, &md5Context);
    return 0;
}

// 计算文件的 SHA1
int EncryptionUtils::encrypt_sha1_file(const std::string& filename, uint8_t key[20]) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        return -1;
    }
    SHA_CTX shaContext;
    SHA1_Init(&shaContext);
    char buffer[1024];
    while (file.read(buffer, sizeof(buffer))) {
        SHA1_Update(&shaContext, buffer, sizeof(buffer));
    }
    SHA1_Update(&shaContext, buffer, file.gcount());
    SHA1_Final(key, &shaContext);
    return 0;
}

// 将缓冲区内容转换为十六进制字符串
void EncryptionUtils::read_buffer_hex(char *pc_buf_in, int n_len_buf_in, char *pc_buf_out, int *pn_len_buf_out) {
    static const char hex_chars[] = "0123456789ABCDEF";
    for (int i = 0; i < n_len_buf_in; ++i) {
        pc_buf_out[i * 2] = hex_chars[(pc_buf_in[i] & 0xF0) >> 4];
        pc_buf_out[i * 2 + 1] = hex_chars[pc_buf_in[i] & 0x0F];
    }
    *pn_len_buf_out = n_len_buf_in * 2;
}

// 读取文件内容到缓冲区
int EncryptionUtils::read_file_ecc(const std::string& sz_file_path, char **p_data_buffer, int *p_file_len) {
    std::ifstream file(sz_file_path, std::ios::binary | std::ios::ate);
    if (!file) {
        return -1;
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    *p_data_buffer = new char[size];
    if (!file.read(*p_data_buffer, size)) {
        delete[] *p_data_buffer;
        return -1;
    }
    *p_file_len = static_cast<int>(size);
    return 0;
}

// 生成 ECDSA 公钥和私钥文件
int EncryptionUtils::create_ecdsa_key(const std::string& p_file_path) {
    EC_GROUP *group = EC_GROUP_new_by_curve_name(NID_X9_62_prime256v1);
    if (!group) {
        return -1;
    }
    EC_KEY *ec_key = EC_KEY_new();
    if (!ec_key) {
        EC_GROUP_free(group);
        return -1;
    }
    if (EC_KEY_set_group(ec_key, group) != 1) {
        EC_KEY_free(ec_key);
        EC_GROUP_free(group);
        return -1;
    }
    if (EC_KEY_generate_key(ec_key) != 1) {
        EC_KEY_free(ec_key);
        EC_GROUP_free(group);
        return -1;
    }

    std::string public_key_path = p_file_path + "/public.pem";
    std::string private_key_path = p_file_path + "/private.pem";

    FILE *pub_file = fopen(public_key_path.c_str(), "wb");
    if (!pub_file) {
        EC_KEY_free(ec_key);
        EC_GROUP_free(group);
        return -1;
    }
    PEM_write_EC_PUBKEY(pub_file, ec_key);
    fclose(pub_file);

    FILE *priv_file = fopen(private_key_path.c_str(), "wb");
    if (!priv_file) {
        EC_KEY_free(ec_key);
        EC_GROUP_free(group);
        return -1;
    }
    PEM_write_ECPrivateKey(priv_file, ec_key, NULL, NULL, 0, NULL, NULL);
    fclose(priv_file);

    EC_KEY_free(ec_key);
    EC_GROUP_free(group);
    return 0;
}

// 根据私钥对数据进行签名
int EncryptionUtils::ecdsa_sign_to_buffer(const std::string& sz_private_key_path, char *sz_buffer_data, const int n_buffer_data, const char *sz_licence_buffer, unsigned int *p_licence_len) {
    FILE *key_file = fopen(sz_private_key_path.c_str(), "rb");
    if (!key_file) {
        return -1;
    }
    EC_KEY *ec_key = PEM_read_ECPrivateKey(key_file, NULL, NULL, NULL);
    fclose(key_file);
    if (!ec_key) {
        return -1;
    }

    ECDSA_SIG *sig = ECDSA_do_sign(reinterpret_cast<const unsigned char*>(sz_buffer_data), n_buffer_data, ec_key);
    if (!sig) {
        EC_KEY_free(ec_key);
        return -1;
    }

    *p_licence_len = i2d_ECDSA_SIG(sig, NULL);
    unsigned char *sig_der = new unsigned char[*p_licence_len];
    unsigned char *p = sig_der;
    i2d_ECDSA_SIG(sig, &p);

    memcpy(const_cast<char*>(sz_licence_buffer), sig_der, *p_licence_len);

    ECDSA_SIG_free(sig);
    EC_KEY_free(ec_key);
    delete[] sig_der;
    return 0;
}

// 根据私钥对文件进行签名并保存到文件
int EncryptionUtils::ecdsa_sign_file_to_licence_file(const std::string& sz_private_key_path, const std::string& sz_data_file_path, const std::string& sz_licence_path) {
    std::ifstream data_file(sz_data_file_path, std::ios::binary | std::ios::ate);
    if (!data_file) {
        return -1;
    }
    std::streamsize size = data_file.tellg();
    data_file.seekg(0, std::ios::beg);

    char *data = new char[size];
    if (!data_file.read(data, size)) {
        delete[] data;
        return -1;
    }

    FILE *key_file = fopen(sz_private_key_path.c_str(), "rb");
    if (!key_file) {
        delete[] data;
        return -1;
    }
    EC_KEY *ec_key = PEM_read_ECPrivateKey(key_file, NULL, NULL, NULL);
    fclose(key_file);
    if (!ec_key) {
        delete[] data;
        return -1;
    }

    ECDSA_SIG *sig = ECDSA_do_sign(reinterpret_cast<const unsigned char*>(data), static_cast<int>(size), ec_key);
    if (!sig) {
        EC_KEY_free(ec_key);
        delete[] data;
        return -1;
    }

    unsigned int sig_len = i2d_ECDSA_SIG(sig, NULL);
    unsigned char *sig_der = new unsigned char[sig_len];
    unsigned char *p = sig_der;
    i2d_ECDSA_SIG(sig, &p);

    std::ofstream licence_file(sz_licence_path, std::ios::binary);
    if (!licence_file) {
        ECDSA_SIG_free(sig);
        EC_KEY_free(ec_key);
        delete[] data;
        delete[] sig_der;
        return -1;
    }
    licence_file.write(reinterpret_cast<const char*>(sig_der), sig_len);

    ECDSA_SIG_free(sig);
    EC_KEY_free(ec_key);
    delete[] data;
    delete[] sig_der;
    return 0;
}

// 根据私钥对数据进行签名并保存到文件
int EncryptionUtils::ecdsa_sign_buffer_to_licence_file(const std::string& sz_private_key_path, char *sz_buffer_data, const int n_buffer_data, const std::string& sz_licence_path) {
    FILE *key_file = fopen(sz_private_key_path.c_str(), "rb");
    if (!key_file) {
        return -1;
    }
    EC_KEY *ec_key = PEM_read_ECPrivateKey(key_file, NULL, NULL, NULL);
    fclose(key_file);
    if (!ec_key) {
        return -1;
    }

    ECDSA_SIG *sig = ECDSA_do_sign(reinterpret_cast<const unsigned char*>(sz_buffer_data), n_buffer_data, ec_key);
    if (!sig) {
        EC_KEY_free(ec_key);
        return -1;
    }

    unsigned int sig_len = i2d_ECDSA_SIG(sig, NULL);
    unsigned char *sig_der = new unsigned char[sig_len];
    unsigned char *p = sig_der;
    i2d_ECDSA_SIG(sig, &p);

    std::ofstream licence_file(sz_licence_path, std::ios::binary);
    if (!licence_file) {
        ECDSA_SIG_free(sig);
        EC_KEY_free(ec_key);
        delete[] sig_der;
        return -1;
    }
    licence_file.write(reinterpret_cast<const char*>(sig_der), sig_len);

    ECDSA_SIG_free(sig);
    EC_KEY_free(ec_key);
    delete[] sig_der;
    return 0;
}

// 根据私钥对数据进行签名、编码并保存到文件
int EncryptionUtils::ecdsa_sign_buffer_base16_to_licence_file(const std::string& sz_private_key_path, char *sz_buffer_data, const int n_buffer_data, const std::string& sz_licence_path) {
    FILE *key_file = fopen(sz_private_key_path.c_str(), "rb");
    if (!key_file) {
        return -1;
    }
    EC_KEY *ec_key = PEM_read_ECPrivateKey(key_file, NULL, NULL, NULL);
    fclose(key_file);
    if (!ec_key) {
        return -1;
    }

    ECDSA_SIG *sig = ECDSA_do_sign(reinterpret_cast<const unsigned char*>(sz_buffer_data), n_buffer_data, ec_key);
    if (!sig) {
        EC_KEY_free(ec_key);
        return -1;
    }

    unsigned int sig_len = i2d_ECDSA_SIG(sig, NULL);
    unsigned char *sig_der = new unsigned char[sig_len];
    unsigned char *p = sig_der;
    i2d_ECDSA_SIG(sig, &p);

    char *encoded = new char[sig_len * 2 + 1];
    read_buffer_hex(reinterpret_cast<char*>(sig_der), sig_len, encoded, &sig_len);
    encoded[sig_len] = '\0';

    std::ofstream licence_file(sz_licence_path);
    if (!licence_file) {
        ECDSA_SIG_free(sig);
        EC_KEY_free(ec_key);
        delete[] sig_der;
        delete[] encoded;
        return -1;
    }
    licence_file << encoded;

    ECDSA_SIG_free(sig);
    EC_KEY_free(ec_key);
    delete[] sig_der;
    delete[] encoded;
    return 0;
}

// 验证签名
int EncryptionUtils::ecdsa_verify_licence_buffer(const std::string& sz_public_key_path, char *sz_buffer, int n_buff_len, char *sz_licence_data, int n_licence_len) {
    FILE *key_file = fopen(sz_public_key_path.c_str(), "rb");
    if (!key_file) {
        return -1;
    }
    EC_KEY *ec_key = PEM_read_EC_PUBKEY(key_file, NULL, NULL, NULL);
    fclose(key_file);
    if (!ec_key) {
        return -1;
    }

    const unsigned char *p = reinterpret_cast<const unsigned char*>(sz_licence_data);
    ECDSA_SIG *sig = d2i_ECDSA_SIG(NULL, &p, n_licence_len);
    if (!sig) {
        EC_KEY_free(ec_key);
        return -1;
    }

    int result = ECDSA_do_verify(reinterpret_cast<const unsigned char*>(sz_buffer), n_buff_len, sig, ec_key);

    ECDSA_SIG_free(sig);
    EC_KEY_free(ec_key);
    return result;
}

// 验证文件签名
int EncryptionUtils::ecdsa_verify_licence_file(const std::string& sz_public_key_path, const std::string& sz_data_file_path, const std::string& sz_licence_path) {
    std::ifstream data_file(sz_data_file_path, std::ios::binary | std::ios::ate);
    if (!data_file) {
        return -1;
    }
    std::streamsize data_size = data_file.tellg();
    data_file.seekg(0, std::ios::beg);

    char *data = new char[data_size];
    if (!data_file.read(data, data_size)) {
        delete[] data;
        return -1;
    }

    std::ifstream licence_file(sz_licence_path, std::ios::binary | std::ios::ate);
    if (!licence_file) {
        delete[] data;
        return -1;
    }
    std::streamsize licence_size = licence_file.tellg();
    licence_file.seekg(0, std::ios::beg);

    char *licence = new char[licence_size];
    if (!licence_file.read(licence, licence_size)) {
        delete[] data;
        delete[] licence;
        return -1;
    }

    int result = ecdsa_verify_licence_buffer(sz_public_key_path, data, static_cast<int>(data_size), licence, static_cast<int>(licence_size));

    delete[] data;
    delete[] licence;
    return result;
}

// 验证 Base16 编码的文件签名
int EncryptionUtils::ecdsa_verify_base16_licence_file(const std::string& sz_public_key_path, const std::string& sz_data_file_path, const std::string& sz_licence_path) {
    std::ifstream data_file(sz_data_file_path, std::ios::binary | std::ios::ate);
    if (!data_file) {
        return -1;
    }
    std::streamsize data_size = data_file.tellg();
    data_file.seekg(0, std::ios::beg);

    char *data = new char[data_size];
    if (!data_file.read(data, data_size)) {
        delete[] data;
        return -1;
    }

    std::ifstream licence_file(sz_licence_path);
    if (!licence_file) {
        delete[] data;
        return -1;
    }
    std::string licence_str;
    std::getline(licence_file, licence_str);

    int decoded_len = licence_str.length() / 2;
    char *licence = new char[decoded_len];
    for (int i = 0; i < decoded_len; ++i) {
        sscanf(licence_str.substr(i * 2, 2).c_str(), "%2hhx", &licence[i]);
    }

    int result = ecdsa_verify_licence_buffer(sz_public_key_path, data, static_cast<int>(data_size), licence, decoded_len);

    delete[] data;
    delete[] licence;
    return result;
}

// 根据服务器公钥生成共享密钥和客户端公钥
int EncryptionUtils::get_ecdh_share_key_from_srv_public_key(const std::string& sz_srv_public_key, const int n_srv_public_key_len, const char *sz_share_key, const char *sz_buf_client_pub_key, int *p_client_pub_keylen) {
    EC_GROUP *group = EC_GROUP_new_by_curve_name(NID_X9_62_prime256v1);
    if (!group) {
        return -1;
    }

    EC_KEY *ec_key = EC_KEY_new();
    if (!ec_key) {
        EC_GROUP_free(group);
        return -1;
    }
    if (EC_KEY_set_group(ec_key, group) != 1) {
        EC_KEY_free(ec_key);
        EC_GROUP_free(group);
        return -1;
    }
    if (EC_KEY_generate_key(ec_key) != 1) {
        EC_KEY_free(ec_key);
        EC_GROUP_free(group);
        return -1;
    }

    const unsigned char *pub_key_data = reinterpret_cast<const unsigned char*>(sz_srv_public_key.c_str());
    EC_KEY *srv_ec_key = d2i_EC_PUBKEY(NULL, &pub_key_data, n_srv_public_key_len);
    if (!srv_ec_key) {
        EC_KEY_free(ec_key);
        EC_GROUP_free(group);
        return -1;
    }

    size_t share_key_len = ECDH_compute_key(const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(sz_share_key)), ECDH_size(ec_key), EC_KEY_get0_public_key(srv_ec_key), ec_key, NULL);
    if (share_key_len == 0) {
        EC_KEY_free(ec_key);
        EC_KEY_free(srv_ec_key);
        EC_GROUP_free(group);
        return -1;
    }

    *p_client_pub_keylen = i2o_ECPublicKey(ec_key, NULL);
    unsigned char *client_pub_key = new unsigned char[*p_client_pub_keylen];
    unsigned char *p = client_pub_key;
    i2o_ECPublicKey(ec_key, &p);
    memcpy(const_cast<char*>(sz_buf_client_pub_key), client_pub_key, *p_client_pub_keylen);

    delete[] client_pub_key;
    EC_KEY_free(ec_key);
    EC_KEY_free(srv_ec_key);
    EC_GROUP_free(group);
    return 0;
}

}