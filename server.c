#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/evp.h>

#define PORT 1194
#define KEY "0123456789abcdef" // 16-byte key for AES-128
#define IV "abcdef0123456789" // 16-byte Initialization Vector

void encrypt(const char *plaintext, char *ciphertext, EVP_CIPHER_CTX *ctx) {
int len, ciphertext_len;

EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, (unsigned char *)KEY, (unsigned char
*)IV);
EVP_EncryptUpdate(ctx, (unsigned char *)ciphertext, &len, (unsigned char *)plaintext, 16);
ciphertext_len = len;
EVP_EncryptFinal_ex(ctx, (unsigned char *)ciphertext + len, &len);
}

void decrypt(const char *ciphertext, char *plaintext, EVP_CIPHER_CTX *ctx) {
int len, plaintext_len;
EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, (unsigned char *)KEY, (unsigned char
*)IV);
EVP_DecryptUpdate(ctx, (unsigned char *)plaintext, &len, (unsigned char *)ciphertext, 16);
plaintext_len = len;
EVP_DecryptFinal_ex(ctx, (unsigned char *)plaintext + len, &len);
}

int main() {
int server_fd, new_socket;
struct sockaddr_in server_addr, client_addr;
socklen_t addr_len = sizeof(client_addr);
char buffer[16], encrypted[16], decrypted[16];

EVP_CIPHER_CTX *enc_ctx = EVP_CIPHER_CTX_new();
EVP_CIPHER_CTX *dec_ctx = EVP_CIPHER_CTX_new();
server_fd = socket(AF_INET, SOCK_STREAM, 0);
server_addr.sin_family = AF_INET;
server_addr.sin_addr.s_addr = INADDR_ANY;
server_addr.sin_port = htons(PORT);

bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
listen(server_fd, 5);
printf("VPN Server listening on port %d...\n", PORT);

new_socket = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
printf("Client connected.\n");

while (1) {
memset(buffer, 0, 16);
recv(new_socket, encrypted, 16, 0);
decrypt(encrypted, decrypted, dec_ctx);
printf("Received (decrypted): %s\n", decrypted);

if (strncmp(decrypted, "exit", 4) == 0) break;

encrypt(decrypted, encrypted, enc_ctx);
send(new_socket, encrypted, 16, 0);

}

EVP_CIPHER_CTX_free(enc_ctx);
EVP_CIPHER_CTX_free(dec_ctx);
close(new_socket);
close(server_fd);
return 0;
}
