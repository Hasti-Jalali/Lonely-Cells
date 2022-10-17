#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE * fp = fopen("c://map.bin","wb");
    int n;
    printf("Please Enter Coordinates of the Game : \n");
    scanf("%d",&n);
    fwrite(&n,sizeof(int),1,fp);
    int choice;
    char write;
    for(int j = 0 ; j < n ;j++){
        for(int i = 0 ; i < n; i++){
            system("cls");
            printf("Please Enter Feature of the Cell [%d][%d] :\n",i,j);
            printf("[ 1 ] Energy \n[ 2 ] Mitosis \n[ 3 ] Forbidden \n[ 4 ] Normal\n");
            scanf("%d",&choice);
            write = '0' + choice;
            fwrite(&write,sizeof(char),1,fp);
        }
    }
    return 0;
}
