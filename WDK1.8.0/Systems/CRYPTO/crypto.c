#include <stdio.h>
#include <string.h>

#define ENCODE 0
#define DECODE 1

// Algoritmo simétrico extremamente simples (didático)
//
// cipher[i] = plaintext[i] XOR key[i % key_len] XOR round
//
// Para descriptografar, basta aplicar o mesmo algoritmo novamente.

void hash_key(const unsigned char *expanded, int len, unsigned char *hash_out)
{
    unsigned int prime = 0x01000193;
    unsigned int hash = 0x811c9dc5;

    for (int i = 0; i < len; i++) {
        hash ^= expanded[i];
        hash *= prime;
        hash_out[i] = hash & 0xFF; // 1 byte da hash por posição
    }
}

int crypto(char* data_out, char *data, const char *key, int decrypt) {
    unsigned char expanded_key[64];
	unsigned char key_hash[64];
	int data_len = strlen(data);
    int key_len  = strlen(key);
    int rounds = 6;
	
	for (int i = 0; i < data_len; i++)
		expanded_key[i] = key[i % key_len];
    hash_key(expanded_key, data_len, key_hash);
    
    printf("Hashkey: ");
    for (int i = 0; i < data_len; i++)
    	printf("%02X ", key_hash[i]);
    printf("\n");
    
    if(decrypt){
    	for(int i = 0; i < data_len; i++){
    		printf("%02X ", data[i * 2 + 1]);
    		if(data[i * 2 + 1] != key_hash[i])
				return 0;	
		}	
	}
	
    for (int i = 0; i < data_len; i++){
    	if(decrypt){
    		data_out[i] = data[i];
		}else{
			data_out[i * 2] = data[i];
    		data_out[i * 2 + 1] = key_hash[i];	
		}	
	}
    
    for (int r = 0; r < rounds; r++) {
        for (int i = 0; i < data_len; i++) {
        	int x = (decrypt) ? i : i * 2;
            data_out[x] ^= expanded_key[i]; // mistura com a chave
            data_out[x] ^= r * i;            // mistura com número da rodada
        }
    }
    return 1;
}

int main() {
    char input[256];
    char output[256];
    char key[64];

    printf("Digite o texto: ");
    fgets(input, 256, stdin);

    // Remover \n
    input[strcspn(input, "\n")] = 0;

    printf("Digite a chave secreta: ");
    fgets(key, 64, stdin);
    key[strcspn(key, "\n")] = 0;
    
    // Mostrar resultados
    printf("Texto.........: %s\n", input);
    printf("Chave.........: %s\n", key);

    crypto(output, input, key, ENCODE);
    printf("Criptografado: %s\n", output);

    // Descriptografando (mesmo algoritmo)
   	if(crypto(input, output, key, DECODE))
    	printf("Descriptografado: %s\n", input);
    else
    	printf("Chave incorreta!\n");

    return 0;
}

