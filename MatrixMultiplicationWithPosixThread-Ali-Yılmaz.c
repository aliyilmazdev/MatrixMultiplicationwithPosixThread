#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Değişkenlerimizi statik olarak tanımladık.
static int m1[10][10] = {{17, 11, 17, 1, 14, 17, 16, 0, 0, 0}, {19, 20, 3, 17, 4, 14, 16, 0, 0, 0}, {3, 20, 9, 19, 15, 7, 4, 0, 0, 0}, {19, 4, 19, 14, 1, 20, 10, 0, 0, 0}, {13, 20, 16, 16, 6, 1, 9, 0, 0, 0}, {2, 20, 20, 15, 1, 9, 13, 0, 0, 0}, {6, 10, 14, 8, 2, 8, 15, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
static int m2[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};

// Matrix datası oluşturmak için struct oluşturduk.
struct matrix_data
{
    int x;
    int y;
    int result;
};

// Matrix çarpımı yaptığımız fonksiyon.
void *multiply_matrix_element(void *arg)
{
    // Bu struct'ı threade argüman olarak veriyoruz.
    struct matrix_data *data = (struct matrix_data *)arg;

    int x = data->x;
    int y = data->y;
    data->result = 0;

    // Çarpımımız 10x4 olacağı için birinci matriximizin üzerinde 4 lü olan ikinci matriximiz gezinecek.
    int i, j;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            data->result += m1[i + x][j + y] * m2[i][j];
        }
    }
    int row = x + 1;
    int col = y + 1;
    // Threadlerin eş zamanlı olarak çalışmasıyla console'a bastırmaya yarar.
    printf("Thread %d,%d finished: %d\n", row, col, data->result);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    // Thread ve struct initialize edilir.
    pthread_t threads[49];
    struct matrix_data data[49];
    int rowCounter = 0;

    printf("M1 Matrix: ");
    printf("\n");
    for (int m = 0; m < 10; m++)
    {
        for (int n = 0; n < 10; n++)
        {
            int result = m1[m][n];
            printf("%d", result);
            printf(" ");
            rowCounter = rowCounter + 1;
            if (rowCounter == 10)
            {
                printf("\n");
                rowCounter = 0;
            }
        }
    }
    printf("\n");

    printf("M2 Matrix: ");
    printf("\n");
    for (int m = 0; m < 4; m++)
    {
        for (int n = 0; n < 4; n++)
        {
            int result = m2[m][n];
            printf("%d", result);
            printf(" ");
            rowCounter = rowCounter + 1;
            if (rowCounter == 4)
            {
                printf("\n");
                rowCounter = 0;
            }
        }
    }
    printf("\n");

    for (int i = 0; i < 49; i++)
    {
        data[i].result = -1;
    }

    // Threadin oluşturulduğu kod bloğu
    int i, j;
    for (i = 0; i < 7; i++)
    {
        for (j = 0; j < 7; j++)
        {
            int index = i * 7 + j;
            data[index].x = i;
            data[index].y = j;
            data[index].result = -1;
            int rc = pthread_create(&threads[index], NULL, multiply_matrix_element, (void *)&data[index]);
            printf("\n");

            /*Thread ile matrix çarpımı yapıldıktan sonra console'a
            yazdırmak için kullanılan kod bloğu */
            for (int i = 0; i < 49; i++)
            {
                printf("%d ", data[i].result);
                printf(" ");
                int rowCounter = rowCounter + 1;
                if (rowCounter == 7)
                {
                    printf("\n");
                    rowCounter = 0;
                }
            }
            printf("\n");

            if (rc)
            {
                printf("Error creating thread\n");
                exit(-1);
            }
        }
    }

    // Threadlerin birbirinin önüne geçmemesi için join kullanırız.
    for (i = 0; i < 49; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Sonuç outputu alınır.
    printf("\n");
    printf("Output Matrix(after multiplication):\n");
    for (i = 0; i < 7; i++)
    {
        for (j = 0; j < 7; j++)
        {
            int index = i * 7 + j;
            printf("%d ", data[index].result);
        }
        printf("\n");
    }

    return 0;
}
