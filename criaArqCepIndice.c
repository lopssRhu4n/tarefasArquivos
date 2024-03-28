#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SRC_FILE "cep.dat"
#define DESTINY_FILE "indice.dat"
typedef struct 
{
	char logradouro[72];
	char bairro[72];
	char cidade[72];
	char uf[72];
	char sigla[2];
	char cep[8];
	char lixo[2]; // Ao Espaço no final da linha + quebra de linha
} Endereco;

typedef struct 
{
    char cep[8];
    int indice;
} Registro;

int compare(const void *e1, const void *e2)
{
	return strncmp(((Registro*)e1)->cep,((Registro*)e2)->cep,8);
}

int main(int argc, char const *argv[])
{
    FILE *file;
	Endereco *e;
	long finalArquivo, quantidadeDeEnderecos;

    file = fopen(SRC_FILE, "rb");
	fseek(file,0,SEEK_END);

	finalArquivo = ftell(file);
    quantidadeDeEnderecos = finalArquivo/sizeof(Endereco);

    Endereco *endereco = (Endereco*) malloc(sizeof(Endereco));
    Registro *registros = (Registro*) malloc(sizeof (Registro) * quantidadeDeEnderecos);
    if(registros == NULL) {
        printf("Erro: Nao consegui alocar memoria REGISTROS");
        fclose(file);
        return -1;
    }
    if(endereco == NULL) {
		printf("Erro: Nao consegui alocar a memoria ENDERECOS\n");
		fclose(file);
		return -1;
	}
	rewind(file); // Vai para o inicio do arquivo

    for(int i = 0; i < quantidadeDeEnderecos; i++) {
        fread(endereco, sizeof(Endereco), 1, file);
        strncpy(registros[i].cep, endereco->cep, 8);
        registros[i].indice = i;
    }
    fclose(file);
    free(endereco);

    qsort(registros,quantidadeDeEnderecos, sizeof(Registro), compare);

    file = fopen(DESTINY_FILE, "wb");
    fwrite(registros, sizeof(Registro), quantidadeDeEnderecos, file);
    fclose(file);
    free(registros);

    printf("Final da escrita");


    return 0;
}
