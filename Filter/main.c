#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#define HI(num) (((num) & 0x0000FF00) >> 8)
#define LO(num) ((num) & 0x000000FF)
void commentFinder(FILE *fp);
typedef struct _PGMData {
    int height;
    int width;
    int maxTone;
    int matrix[512][512];
} PGMData;
int matrix2[520][520];
PGMData readPGM();
void meanFilter(int filterSize);
void gaussianFilter(int filterSize, double sigma);
void writePGM();
double filterMatrix[7][7];
int image[512][512];
PGMData data;

int main()
{
    
    data =readPGM();
    int size = 3;
    printf("Write filter size:");
    //scanf("%d",&size);
    meanFilter(size);
    //gaussianFilter(size,1);
    
    
    
    return 0;
}
void meanFilter(int filterSize){
    
    int i,j,k,l;
    int toplam;
    /*
     image = (int **)calloc(data.height,sizeof(int));
     if (image == NULL) {
     perror("memory allocation failure");
     exit(EXIT_FAILURE);
     }
     
     for (i = 0; i < data.height; i++){
     image[i] = (int*)calloc(data.width,sizeof(int));
     if (image[i] == NULL) {
     perror("memory allocation failure");
     exit(EXIT_FAILURE);
     }
     }
     */
    for(i=0;i<data.height;i++) {
        for(j=0;j<data.width;j++){
            image[i][j] = data.matrix[i][j];
        }
    }
    int indis = filterSize/2;
    for(i=indis;i<data.height-indis;i++) {
        for(j=indis;j<data.width-indis;j++){
            toplam=0;
            for(k=i-indis;k<i+indis;k++){
                for(l=j-indis;l<j+indis;l++){
                    toplam += data.matrix[k][l];
                }
            }
            toplam = toplam /(filterSize*filterSize);
            image[i][j] = toplam;
        }
    }
    for(i=0;i<data.height;i++) {
        for(j=0;j<data.width;j++){
            data.matrix[i][j] = image[i][j];
        }
    }
    writePGM(data);
}


void gaussianFilter(int filterSize, double sigma){
    
    int i,j,k,l;
    double toplam=0;
    /*
     filterMatrix = (int **)calloc(filterSize,sizeof(int));
     if (filterMatrix == NULL) {
     printf("Warning!! Memory allocation failure");
     exit(EXIT_FAILURE);
     }
     
     for (i = 0; i < filterSize; i++){
     filterMatrix[i] = (int*)calloc(filterSize,sizeof(int));
     if (filterMatrix[i] == NULL) {
     printf("Warning!! Memory allocation failure");
     exit(EXIT_FAILURE);
     }
     }
     
     image = (int **)calloc(data.height,sizeof(int));
     if (image == NULL) {
     perror("memory allocation failure");
     exit(EXIT_FAILURE);
     }
     
     for (i = 0; i < data.height; i++){
     image[i] = (int*)calloc(data.width,sizeof(int));
     if (image[i] == NULL) {
     perror("memory allocation failure");
     exit(EXIT_FAILURE);
     }
     }
     */
    for(i=0;i<data.height;i++) {
        for(j=0;j<data.width;j++){
            image[i][j] = data.matrix[i][j];
        }
    }
    
    double under = 2.0 * sigma * sigma;
    int first = ((1-filterSize)/2);
    int last = ((filterSize-1)/2);
    
    for(i=first;i<=last;i++){
        for(j=first;j<=last;j++){
            filterMatrix[i+last][j+last]=(exp(-1*(i*i+j*j)/under));
        }
    }
    double constant = 1/filterMatrix[0][0];
    for(i=0;i<filterSize;i++){
        for(j=0;j<filterSize;j++){
            filterMatrix[i][j]=filterMatrix[i][j]*constant;
            toplam+=filterMatrix[i][j];
        }
    }
    
    for(i=0;i<filterSize;i++){
        for(j=0;j<filterSize;j++){
            filterMatrix[i][j]=filterMatrix[i][j]/toplam;
            printf("%f ",filterMatrix[i][j]);
        }
        printf("\n");
    }
    
    
    for (i = 1; i < data.height-1; i++){
        for (j = 1; j < data.width-1; j++) {
            toplam = 0.0;
            for (k = 0; k<filterSize;k++){
                for (l = 0; l<filterSize;l++){
                    toplam += filterMatrix[k][l] * data.matrix[i-1+k][j-1+l];
                }
            }
            image[i][j]=toplam;
        }
    }
    for(i=0;i<data.height;i++) {
        for(j=0;j<data.width;j++){
            data.matrix[i][j] = image[i][j];
        }
    }
    
    writePGM(data);
    
    
}
void writePGM()
{
    
    FILE *pgmFile;
    int i, j;
    
    pgmFile = fopen("/Users/nevzat/Downloads/images/gauss(5*5)(11).pgm", "wb");
    if (pgmFile == NULL) {
        perror("cannot open file to write");
        exit(EXIT_FAILURE);
    }
    
    fprintf(pgmFile, "P2\n");
    fprintf(pgmFile, "%d %d\n",data.height,data.width);
    fprintf(pgmFile, "%d\n", data.maxTone);
    
    if (data.maxTone > 255) {
        for (i = 0; i < data.height; ++i) {
            for (j = 0; j < data.width; ++j) {
                //hi = HI(matrix2[i][j]);
                //lo = LO(matrix2[i][j]);
                //fputc(hi, pgmFile);
                //fputc(lo, pgmFile);
                fprintf(pgmFile, "%d ",data.matrix[i][j]);
            }
            
        }
    }
    else {
        for (i = 0; i < data.height; ++i){
            for (j = 0; j < data.width; ++j) {
                fprintf(pgmFile, "%d ",data.matrix[i][j]);
            }
            fprintf(pgmFile, "\n");
        }
    }
    
    fclose(pgmFile);
    
}
PGMData readPGM()
{
    FILE *pgmFile;
    char version[3];
    int i, j;
    int lo, hi;
    pgmFile = fopen("/Users/nevzat/Downloads/images/baboon.512.pgm", "rb");
    if (pgmFile == NULL) {
        perror("cannot open file to read");
        exit(EXIT_FAILURE);
    }
    
    fgets(version, sizeof(version), pgmFile);
    if (strcmp(version, "P5")) {
        fprintf(stderr, "Wrong file type!\n");
        exit(EXIT_FAILURE);
    }
    commentFinder(pgmFile);
    fscanf(pgmFile, "%d", &data.width);
    commentFinder(pgmFile);
    fscanf(pgmFile, "%d", &data.height);
    commentFinder(pgmFile);
    fscanf(pgmFile, "%d", &data.maxTone);
    fgetc(pgmFile);
    /*
     data.matrix = (int **)calloc(data.height,sizeof(int));
     if (data.matrix == NULL) {
     printf("Warning!! Memory allocation failure");
     exit(EXIT_FAILURE);
     }
     
     for (i = 0; i < data.height; i++){
     data.matrix[i] = (int*)calloc(data.width,sizeof(int));
     if (data.matrix[i] == NULL) {
     printf("Warning!! Memory allocation failure");
     exit(EXIT_FAILURE);
     }
     }
     */
    if (data.maxTone > 255)
        for (i = 0; i < data.height; ++i)
            for (j = 0; j < data.width; ++j) {
                hi = fgetc(pgmFile);
                lo = fgetc(pgmFile);
                data.matrix[i][j] = (hi << 8) + lo;
            }
    else
        for (i = 0; i < data.height; ++i)
            for (j = 0; j < data.width; ++j) {
                lo = fgetc(pgmFile);
                data.matrix[i][j] = lo;
                //printf("%d ",lo);
            }
    fclose(pgmFile);
    return data;
    
}
void commentFinder(FILE *fp)
{
    int ch;
    char line[100];
    
    while ((ch = fgetc(fp)) != EOF && isspace(ch))
        ;
    if (ch == '#') {
        fgets(line, sizeof(line), fp);
        commentFinder(fp);
    } else
        fseek(fp, -1, SEEK_CUR);
    
    
    
}
