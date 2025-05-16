#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/evp.h>
#define SERVER_IP "127.0.0.1"
#define PORT 1194
#define KEY "0123456789abcdef"
#define IV "abcdef0123456789"
void encrypt(const char *plaintext, char *ciphertext, EVP_CIPHER_CTX *ctx) {
int len, ciphertext_len;
EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, (unsigned char *)KEY, (unsigned char
*)IV);
EVP_EncryptUpdate(ctx, (unsigned char *)ciphertext, &len, (unsigned char *)plaintext,
strlen(plaintext));
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
plaintext[plaintext_len + len] = '\0'; // Null-terminate the decrypted message
}
int main() {
int sock;
struct sockaddr_in server_addr;
char buffer[16], encrypted[16], decrypted[16];
EVP_CIPHER_CTX *enc_ctx = EVP_CIPHER_CTX_new();
EVP_CIPHER_CTX *dec_ctx = EVP_CIPHER_CTX_new();
sock = socket(AF_INET, SOCK_STREAM, 0);
server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(PORT);
inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);
connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
printf("Connected to VPN Server.\n");
while (1) {
printf("Enter message: ");
fgets(buffer, 16, stdin);
buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline
encrypt(buffer, encrypted, enc_ctx);
send(sock, encrypted, 16, 0);
if (strncmp(buffer, "exit", 4) == 0) break;
// Receive encrypted message from server
recv(sock, encrypted, 16, 0);
// Decrypt the received message
decrypt(encrypted, decrypted, dec_ctx);
printf("Received from server (decrypted): %s\n", decrypted);
}
EVP_CIPHER_CTX_free(enc_ctx);
EVP_CIPHER_CTX_free(dec_ctx);
close(sock);
return 0;
}
