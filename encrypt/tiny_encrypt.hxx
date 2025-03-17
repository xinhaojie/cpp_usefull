#pragma once

#include <cstdint>
#include <string>
#include <openssl/crc32.h>
#include <openssl/crc64.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <openssl/ec.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/ecdsa.h>
#include <openssl/evp.h>
#include <openssl/base64.h>

namespace tiny_encrypt {
class EncryptionUtils {
public:
    // 计算 CRC32
    static void encrypt_crc32(const uint8_t *data, size_t len, uint32_t *crc);

    // 计算 CRC64
    static void encrypt_crc64(const uint8_t *data, uint64_t len, uint64_t magic, uint64_t *crc);

    // 计算 MD5
    static void encrypt_md5(const uint8_t *data, size_t len, uint8_t key[16]);

    // 计算 SHA1
    static void encrypt_sha1(const uint8_t *data, size_t len, uint8_t key[20]);

    // 计算文件的 MD5
    static int encrypt_md5_file(const std::string& filename, uint8_t key[16]);

    // 计算文件的 SHA1
    static int encrypt_sha1_file(const std::string& filename, uint8_t key[20]);

    // 将缓冲区内容转换为十六进制字符串
    static void read_buffer_hex(char *pc_buf_in, int n_len_buf_in, char *pc_buf_out, int *pn_len_buf_out);

    // 读取文件内容到缓冲区
    static int read_file_ecc(const std::string& sz_file_path, char **p_data_buffer, int *p_file_len);

    // 生成 ECDSA 公钥和私钥文件
    static int create_ecdsa_key(const std::string& p_file_path);

    // 根据私钥对数据进行签名
    static int ecdsa_sign_to_buffer(const std::string& sz_private_key_path, char *sz_buffer_data, const int n_buffer_data, const char *sz_licence_buffer, unsigned int *p_licence_len);

    // 根据私钥对文件进行签名并保存到文件
    static int ecdsa_sign_file_to_licence_file(const std::string& sz_private_key_path, const std::string& sz_data_file_path, const std::string& sz_licence_path);

    // 根据私钥对数据进行签名并保存到文件
    static int ecdsa_sign_buffer_to_licence_file(const std::string& sz_private_key_path, char *sz_buffer_data, const int n_buffer_data, const std::string& sz_licence_path);

    // 根据私钥对数据进行签名、编码并保存到文件
    static int ecdsa_sign_buffer_base16_to_licence_file(const std::string& sz_private_key_path, char *sz_buffer_data, const int n_buffer_data, const std::string& sz_licence_path);

    // 验证签名
    static int ecdsa_verify_licence_buffer(const std::string& sz_public_key_path, char *sz_buffer, int n_buff_len, char *sz_licence_data, int n_licence_len);

    // 验证文件签名
    static int ecdsa_verify_licence_file(const std::string& sz_public_key_path, const std::string& sz_data_file_path, const std::string& sz_licence_path);

    // 验证 Base16 编码的文件签名
    static int ecdsa_verify_base16_licence_file(const std::string& sz_public_key_path, const std::string& sz_data_file_path, const std::string& sz_licence_path);

    // 根据服务器公钥生成共享密钥和客户端公钥
    static int get_ecdh_share_key_from_srv_public_key(const std::string& sz_srv_public_key, const int n_srv_public_key_len, const char *sz_share_key, const char *sz_buf_client_pub_key, int *p_client_pub_key_len);
};

}