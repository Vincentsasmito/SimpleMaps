#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#define MAX_LEN 128
#define maxVal 9999

int V = 16;
char date[100];

void getTime(void)
{
    time_t now;

    time(&now);

    struct tm *local = localtime(&now);

    int min = local->tm_min;
    int hour = local->tm_hour;

    sprintf(date, "%d:%d", hour, min);

    printf("%s\n", date);

    return 0;
}

int jarakMin(int jarak[V], bool dikunjungi[V])
{
    int min = maxVal;
    int min_index = -1;
    for (int x = 0; x < V; x++)
    {
        if (dikunjungi[x] == false && jarak[x] <= min)
        {
            min = jarak[x];
            min_index = x;
        }
    }
    return min_index;
}

void dijkstra(int graph[V][V], int start, int target, int speed)
{
    int min, hour, newMin, newHour;
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);

    min = local->tm_min;
    hour = local->tm_hour;

    int jarak[V];
    bool dikunjungi[V];
    int kotaKeberangkatan[V];

    for (int i = 0; i < V; i++)
    {
        jarak[i] = maxVal;
        dikunjungi[i] = false;
        kotaKeberangkatan[i] = start;
    }

    jarak[start] = 0;

    for (int x = 0; x < V - 1; x++)
    {
        int index = jarakMin(jarak, dikunjungi);

        dikunjungi[index] = true;

        for (int v = 0; v < V; v++)
        {
            if (dikunjungi[v] == false && graph[index][v] != 0 && jarak[index] != maxVal && jarak[index] + graph[index][v] < jarak[v])
            {
                jarak[v] = jarak[index] + graph[index][v];
                kotaKeberangkatan[v] = index;
            }
        }
    }
    printf("Node\tJarak dari node %d\t\tKota keberangkatan\n", start);
    for (int v = 0; v < V; v++)
    {
        printf("%d\t\t %d\t\t\t\t\t%d\n", v, jarak[v], kotaKeberangkatan[v]);
    }
    int route[V];
    int node = target;
    route[0] = node;
    int i;
    for (i = 1; i < V; i++)
    {
        route[i] = kotaKeberangkatan[node];
        node = kotaKeberangkatan[node];
        if (route[i] == start)
        {
            break;
        }

    }
    printf("Route: ");
    for (int x = i; x >= 0; x--)
    {
        if (x == 0)
        {
            printf(" (%d)\n", route[x]);
        }
        else
            printf(" (%d) ->", route[x]);
    }
    printf("Distance from %d to %d: %dkm\n", start, target, jarak[target]);

    if (hour < 10 && min < 10)
    {
        printf("Starting at 0%d:0%d, ", hour, min);
    }
    else if (hour < 10)
    {
        printf("Starting at 0%d:%d, ", hour, min);
    }
    else if (min < 10)
    {
        printf("Starting at %d:0%d, ", hour, min);
    }
    else
    {
        printf("Starting at %d:%d, ", hour, min);
    }

    // Waktu perjalanan (Menit)
    int travelTime = jarak[target] * 60 / speed;

    // Menit sekarang ditambah menit waktu perjalanan
    newMin = min + travelTime;

    // Konstanta untuk mengecek apakah menit melebihi atau sama dengan 60 menit
    int constanta = newMin / 60;

    // Jika konstanta melebihi 0 maka menit melebihi atau sama dengan 60
    if (constanta > 0)
    {
        // Menambahkan jam sekarang dengan jam baru
        newHour = hour + constanta;

        // Jika jam melebihi 24 maka akan di ubah ke waktu pagi
        if (newHour > 24)
        {
            newHour = newHour % 24;
        }
        // Sisa menit
        newMin %= 60;
    }
    else
    {
        newHour = hour;
    }

    if (newHour < 10 && newMin < 10)
    {
        printf("You will arrive at 0%d:0%d in %dkm/h\n", newHour, newMin, speed);
    }
    else if (newHour < 10)
    {
        printf("You will arrive at 0%d:%d in %dkm/h\n", newHour, newMin, speed);
    }
    else if (newMin < 10)
    {
        printf("You will arrive at %d:0%d in %dkm/h\n", newHour, newMin, speed);
    }
    else
    {
        printf("You will arrive at %d:%d in %dkm/h\n", newHour, newMin, speed);
    }
}

int main()
{
    printf("Welcome to Simple Maps\n");
    FILE *fptr = fopen("img\\WorldMap.txt", "r");
    if (fptr == NULL)
    {
        printf("\nError Opening %s File.\n", "WorldMap");
        exit(1);
    }
    char read_string[MAX_LEN];
    while (fgets(read_string, sizeof(read_string), fptr) != NULL)
    {
        printf("%s", read_string);
    }
    printf("\n");
    fclose(fptr);
    getTime();
    printf("Would you like to create your own map or use one of our presets?\n>> Use my own (1) / Use a preset (2)\n>> ");
    int type;
    scanf("%d", &type);
    if (type == 1)
    {
        printf("Please enter your map in an adjacency matrix.\n");
        int gSize;
        printf("Enter graph size: ");
        scanf("%d", &gSize);
        int newGraph[gSize][gSize];
        for (int i = 0; i < gSize; i++)
        {
            for (int x = 0; x < gSize; x++)
            {
                printf("Graph[%d][%d]: ", i, x);
                scanf("%d", &newGraph[i][x]);
            }
            printf("\n");
        }
    }
    else if (type == 2)
    {
        printf("Here are some map presets available, pick them by the preset number.\n");
        printf("Preset 1\n");
        printf("                    (1) -1- (2) -4- (7)-\\ --15-- (10)       (13) --13-- (14)          \n");
        printf("                   /  \\      /       |   \\      /  | \\      /            |             \n");
        printf("                  3    4    2\        1    2    12  |  7    3             |            \n");
        printf("                 /      \\  /         |     \\  /    |   \\  /              |           \n");
        printf("              (0) --7--  (3)  --3-- (6) -1- (9)   13    (12)             16          \n");
        printf("               \\         / \\       / |     /  \\    |    /                |            \n");
        printf("                12     3    7    5   3   10    10  |   9                 |           \n");
        printf("                 \\    /      \\  /    |  /       \\  |  /                  |         \n");
        printf("                   (4) --8-- (5) -6- (8) ---7--- (11) ---------17------- (15)         \n");
        printf("Preset 2\n");
        printf("            (0) --4-- (1)                (7) --3-- (11) --4-- (14) --10-- (15)\n");
        printf("            |        /   \\              /  \\        |          |         /      \n");
        printf("            |       /     3            6    9       |          6        8      \n");
        printf("            |      /       \\          /      \\      |          |       /        \n");
        printf("            2     5       (2) --5-- (6)      (8)    10        (13) ---/         \n");
        printf("            |    /        / \\       /  \\       \\    |          |              \n");
        printf("            |   /        3   6     7    3       8   |          3               \n");
        printf("            |  /        /     \\   /      \\       \\  |          |             \n");
        printf("            (4) --3-- (3)      (5)      (10) --7--(9)  --3--  (12)              \n");
        printf(">> ");
        int preset;
        scanf("%d", &preset);
        int graph1[V][V], graph2[V][V];
        graph1[0][0] = 0;
        graph1[0][1] = 3;
        graph1[0][2] = 0;
        graph1[0][3] = 7;
        graph1[0][4] = 12;
        graph1[0][5] = 0;
        graph1[0][6] = 0;
        graph1[0][7] = 0;
        graph1[0][8] = 0;
        graph1[0][9] = 0;
        graph1[0][10] = 0;
        graph1[0][11] = 0;
        graph1[0][12] = 0;
        graph1[0][13] = 0;
        graph1[0][14] = 0;
        graph1[0][15] = 0;

        graph1[1][0] = 3;
        graph1[1][1] = 0;
        graph1[1][2] = 1;
        graph1[1][3] = 4;
        graph1[1][4] = 0;
        graph1[1][5] = 0;
        graph1[1][6] = 0;
        graph1[1][7] = 0;
        graph1[1][8] = 0;
        graph1[1][9] = 0;
        graph1[1][10] = 0;
        graph1[1][11] = 0;
        graph1[1][12] = 0;
        graph1[1][13] = 0;
        graph1[1][14] = 0;
        graph1[1][15] = 0;

        graph1[2][0] = 0;
        graph1[2][1] = 1;
        graph1[2][2] = 0;
        graph1[2][3] = 2;
        graph1[2][4] = 0;
        graph1[2][5] = 0;
        graph1[2][6] = 0;
        graph1[2][7] = 4;
        graph1[2][8] = 0;
        graph1[2][9] = 0;
        graph1[2][10] = 0;
        graph1[2][11] = 0;
        graph1[2][12] = 0;
        graph1[2][13] = 0;
        graph1[2][14] = 0;
        graph1[2][15] = 0;

        graph1[3][0] = 7;
        graph1[3][1] = 4;
        graph1[3][2] = 2;
        graph1[3][3] = 0;
        graph1[3][4] = 3;
        graph1[3][5] = 7;
        graph1[3][6] = 3;
        graph1[3][7] = 0;
        graph1[3][8] = 0;
        graph1[3][9] = 0;
        graph1[3][10] = 0;
        graph1[3][11] = 0;
        graph1[3][12] = 0;
        graph1[3][13] = 0;
        graph1[3][14] = 0;
        graph1[3][15] = 0;

        graph1[4][0] = 12;
        graph1[4][1] = 0;
        graph1[4][2] = 0;
        graph1[4][3] = 0;
        graph1[4][4] = 3;
        graph1[4][5] = 8;
        graph1[4][6] = 0;
        graph1[4][7] = 0;
        graph1[4][8] = 0;
        graph1[4][9] = 0;
        graph1[4][10] = 0;
        graph1[4][11] = 0;
        graph1[4][12] = 0;
        graph1[4][13] = 0;
        graph1[4][14] = 0;
        graph1[4][15] = 0;

        graph1[5][0] = 0;
        graph1[5][1] = 0;
        graph1[5][2] = 0;
        graph1[5][3] = 7;
        graph1[5][4] = 8;
        graph1[5][5] = 0;
        graph1[5][6] = 5;
        graph1[5][7] = 0;
        graph1[5][8] = 6;
        graph1[5][9] = 0;
        graph1[5][10] = 0;
        graph1[5][11] = 0;
        graph1[5][12] = 0;
        graph1[5][13] = 0;
        graph1[5][14] = 0;
        graph1[5][15] = 0;

        graph1[6][0] = 0;
        graph1[6][1] = 0;
        graph1[6][2] = 0;
        graph1[6][3] = 3;
        graph1[6][4] = 8;
        graph1[6][5] = 5;
        graph1[6][6] = 0;
        graph1[6][7] = 1;
        graph1[6][8] = 3;
        graph1[6][9] = 0;
        graph1[6][10] = 0;
        graph1[6][11] = 0;
        graph1[6][12] = 0;
        graph1[6][13] = 0;
        graph1[6][14] = 0;
        graph1[6][15] = 0;

        graph1[7][0] = 0;
        graph1[7][1] = 0;
        graph1[7][2] = 4;
        graph1[7][3] = 0;
        graph1[7][4] = 0;
        graph1[7][5] = 0;
        graph1[7][6] = 1;
        graph1[7][7] = 0;
        graph1[7][8] = 0;
        graph1[7][9] = 2;
        graph1[7][10] = 15;
        graph1[7][11] = 0;
        graph1[7][12] = 0;
        graph1[7][13] = 0;
        graph1[7][14] = 0;
        graph1[7][15] = 0;

        graph1[8][0] = 0;
        graph1[8][1] = 0;
        graph1[8][2] = 0;
        graph1[8][3] = 0;
        graph1[8][4] = 0;
        graph1[8][5] = 6;
        graph1[8][6] = 3;
        graph1[8][7] = 0;
        graph1[8][8] = 0;
        graph1[8][9] = 10;
        graph1[8][10] = 0;
        graph1[8][11] = 7;
        graph1[8][12] = 0;
        graph1[8][13] = 0;
        graph1[8][14] = 0;
        graph1[8][15] = 0;

        graph1[9][0] = 0;
        graph1[9][1] = 0;
        graph1[9][2] = 0;
        graph1[9][3] = 0;
        graph1[9][4] = 0;
        graph1[9][5] = 0;
        graph1[9][6] = 1;
        graph1[9][7] = 2;
        graph1[9][8] = 10;
        graph1[9][9] = 0;
        graph1[9][10] = 12;
        graph1[9][11] = 10;
        graph1[9][12] = 0;
        graph1[9][13] = 0;
        graph1[9][14] = 0;
        graph1[9][15] = 0;

        graph1[10][0] = 0;
        graph1[10][1] = 0;
        graph1[10][2] = 0;
        graph1[10][3] = 0;
        graph1[10][4] = 0;
        graph1[10][5] = 0;
        graph1[10][6] = 0;
        graph1[10][7] = 15;
        graph1[10][8] = 0;
        graph1[10][9] = 12;
        graph1[10][10] = 0;
        graph1[10][11] = 13;
        graph1[10][12] = 7;
        graph1[10][13] = 0;
        graph1[10][14] = 0;
        graph1[10][15] = 0;

        graph1[11][0] = 0;
        graph1[11][1] = 0;
        graph1[11][2] = 0;
        graph1[11][3] = 0;
        graph1[11][4] = 0;
        graph1[11][5] = 0;
        graph1[11][6] = 0;
        graph1[11][7] = 0;
        graph1[11][8] = 7;
        graph1[11][9] = 10;
        graph1[11][10] = 13;
        graph1[11][11] = 0;
        graph1[11][12] = 0;
        graph1[11][13] = 0;
        graph1[11][14] = 0;
        graph1[11][15] = 17;

        graph1[12][0] = 0;
        graph1[12][1] = 0;
        graph1[12][2] = 0;
        graph1[12][3] = 0;
        graph1[12][4] = 0;
        graph1[12][5] = 0;
        graph1[12][6] = 0;
        graph1[12][7] = 0;
        graph1[12][8] = 0;
        graph1[12][9] = 0;
        graph1[12][10] = 7;
        graph1[12][11] = 9;
        graph1[12][12] = 0;
        graph1[12][13] = 3;
        graph1[12][14] = 0;
        graph1[12][15] = 0;

        graph1[13][0] = 0;
        graph1[13][1] = 0;
        graph1[13][2] = 0;
        graph1[13][3] = 0;
        graph1[13][4] = 0;
        graph1[13][5] = 0;
        graph1[13][6] = 0;
        graph1[13][7] = 0;
        graph1[13][8] = 0;
        graph1[13][9] = 0;
        graph1[13][10] = 0;
        graph1[13][11] = 0;
        graph1[13][12] = 3;
        graph1[13][13] = 0;
        graph1[13][14] = 13;
        graph1[13][15] = 0;

        graph1[14][0] = 0;
        graph1[14][1] = 0;
        graph1[14][2] = 0;
        graph1[14][3] = 0;
        graph1[14][4] = 0;
        graph1[14][5] = 0;
        graph1[14][6] = 0;
        graph1[14][7] = 0;
        graph1[14][8] = 0;
        graph1[14][9] = 0;
        graph1[14][10] = 0;
        graph1[14][11] = 0;
        graph1[14][12] = 0;
        graph1[14][13] = 13;
        graph1[14][14] = 0;
        graph1[14][15] = 16;

        graph1[15][0] = 0;
        graph1[15][1] = 0;
        graph1[15][2] = 0;
        graph1[15][3] = 0;
        graph1[15][4] = 0;
        graph1[15][5] = 0;
        graph1[15][6] = 0;
        graph1[15][7] = 0;
        graph1[15][8] = 0;
        graph1[15][9] = 0;
        graph1[15][10] = 0;
        graph1[15][11] = 17;
        graph1[15][12] = 0;
        graph1[15][13] = 0;
        graph1[15][14] = 16;
        graph1[15][15] = 0;

        graph2[0][0] = 0;
        graph2[0][1] = 3;
        graph2[0][2] = 0;
        graph2[0][3] = 7;
        graph2[0][4] = 12;
        graph2[0][5] = 0;
        graph2[0][6] = 0;
        graph2[0][7] = 0;
        graph2[0][8] = 0;
        graph2[0][9] = 0;
        graph2[0][10] = 0;
        graph2[0][11] = 0;
        graph2[0][12] = 0;
        graph2[0][13] = 0;
        graph2[0][14] = 0;
        graph2[0][15] = 0;

        graph2[1][0] = 3;
        graph2[1][1] = 0;
        graph2[1][2] = 1;
        graph2[1][3] = 4;
        graph2[1][4] = 0;
        graph2[1][5] = 0;
        graph2[1][6] = 0;
        graph2[1][7] = 0;
        graph2[1][8] = 0;
        graph2[1][9] = 0;
        graph2[1][10] = 0;
        graph2[1][11] = 0;
        graph2[1][12] = 0;
        graph2[1][13] = 0;
        graph2[1][14] = 0;
        graph2[1][15] = 0;

        graph2[2][0] = 0;
        graph2[2][1] = 1;
        graph2[2][2] = 0;
        graph2[2][3] = 2;
        graph2[2][4] = 0;
        graph2[2][5] = 0;
        graph2[2][6] = 0;
        graph2[2][7] = 4;
        graph2[2][8] = 0;
        graph2[2][9] = 0;
        graph2[2][10] = 0;
        graph2[2][11] = 0;
        graph2[2][12] = 0;
        graph2[2][13] = 0;
        graph2[2][14] = 0;
        graph2[2][15] = 0;

        graph2[3][0] = 7;
        graph2[3][1] = 4;
        graph2[3][2] = 2;
        graph2[3][3] = 0;
        graph2[3][4] = 3;
        graph2[3][5] = 7;
        graph2[3][6] = 3;
        graph2[3][7] = 4;
        graph2[3][8] = 0;
        graph2[3][9] = 0;
        graph2[3][10] = 0;
        graph2[3][11] = 0;
        graph2[3][12] = 0;
        graph2[3][13] = 0;
        graph2[3][14] = 0;
        graph2[3][15] = 0;

        graph2[4][0] = 12;
        graph2[4][1] = 0;
        graph2[4][2] = 0;
        graph2[4][3] = 3;
        graph2[4][4] = 0;
        graph2[4][5] = 8;
        graph2[4][6] = 0;
        graph2[4][7] = 0;
        graph2[4][8] = 0;
        graph2[4][9] = 0;
        graph2[4][10] = 0;
        graph2[4][11] = 0;
        graph2[4][12] = 0;
        graph2[4][13] = 0;
        graph2[4][14] = 0;
        graph2[4][15] = 0;

        graph2[5][0] = 0;
        graph2[5][1] = 0;
        graph2[5][2] = 6;
        graph2[5][3] = 0;
        graph2[5][4] = 0;
        graph2[5][5] = 0;
        graph2[5][6] = 7;
        graph2[5][7] = 0;
        graph2[5][8] = 0;
        graph2[5][9] = 0;
        graph2[5][10] = 0;
        graph2[5][11] = 0;
        graph2[5][12] = 0;
        graph2[5][13] = 0;
        graph2[5][14] = 0;
        graph2[5][15] = 0;

        graph2[6][0] = 0;
        graph2[6][1] = 0;
        graph2[6][2] = 5;
        graph2[6][3] = 0;
        graph2[6][4] = 0;
        graph2[6][5] = 7;
        graph2[6][6] = 0;
        graph2[6][7] = 6;
        graph2[6][8] = 0;
        graph2[6][9] = 0;
        graph2[6][10] = 0;
        graph2[6][11] = 0;
        graph2[6][12] = 0;
        graph2[6][13] = 0;
        graph2[6][14] = 0;
        graph2[6][15] = 0;

        graph2[7][0] = 0;
        graph2[7][1] = 0;
        graph2[7][2] = 0;
        graph2[7][3] = 0;
        graph2[7][4] = 0;
        graph2[7][5] = 0;
        graph2[7][6] = 6;
        graph2[7][7] = 0;
        graph2[7][8] = 9;
        graph2[7][9] = 0;
        graph2[7][10] = 0;
        graph2[7][11] = 3;
        graph2[7][12] = 0;
        graph2[7][13] = 0;
        graph2[7][14] = 0;
        graph2[7][15] = 0;

        graph2[8][0] = 0;
        graph2[8][1] = 0;
        graph2[8][2] = 0;
        graph2[8][3] = 0;
        graph2[8][4] = 0;
        graph2[8][5] = 0;
        graph2[8][6] = 0;
        graph2[8][7] = 9;
        graph2[8][8] = 0;
        graph2[8][9] = 8;
        graph2[8][10] = 0;
        graph2[8][11] = 0;
        graph2[8][12] = 0;
        graph2[8][13] = 0;
        graph2[8][14] = 0;
        graph2[8][15] = 0;

        graph2[9][0] = 0;
        graph2[9][1] = 0;
        graph2[9][2] = 0;
        graph2[9][3] = 0;
        graph2[9][4] = 0;
        graph2[9][5] = 0;
        graph2[9][6] = 1;
        graph2[9][7] = 2;
        graph2[9][8] = 8;
        graph2[9][9] = 0;
        graph2[9][10] = 7;
        graph2[9][11] = 10;
        graph2[9][12] = 0;
        graph2[9][13] = 0;
        graph2[9][14] = 0;
        graph2[9][15] = 0;

        graph2[10][0] = 0;
        graph2[10][1] = 0;
        graph2[10][2] = 0;
        graph2[10][3] = 0;
        graph2[10][4] = 0;
        graph2[10][5] = 0;
        graph2[10][6] = 3;
        graph2[10][7] = 0;
        graph2[10][8] = 0;
        graph2[10][9] = 7;
        graph2[10][10] = 0;
        graph2[10][11] = 0;
        graph2[10][12] = 0;
        graph2[10][13] = 0;
        graph2[10][14] = 0;
        graph2[10][15] = 0;

        graph2[11][0] = 0;
        graph2[11][1] = 0;
        graph2[11][2] = 0;
        graph2[11][3] = 0;
        graph2[11][4] = 0;
        graph2[11][5] = 0;
        graph2[11][6] = 0;
        graph2[11][7] = 3;
        graph2[11][8] = 0;
        graph2[11][9] = 10;
        graph2[11][10] = 0;
        graph2[11][11] = 0;
        graph2[11][12] = 0;
        graph2[11][13] = 0;
        graph2[11][14] = 4;
        graph2[11][15] = 0;

        graph2[12][0] = 0;
        graph2[12][1] = 0;
        graph2[12][2] = 0;
        graph2[12][3] = 0;
        graph2[12][4] = 0;
        graph2[12][5] = 0;
        graph2[12][6] = 0;
        graph2[12][7] = 0;
        graph2[12][8] = 0;
        graph2[12][9] = 0;
        graph2[12][10] = 0;
        graph2[12][11] = 0;
        graph2[12][12] = 0;
        graph2[12][13] = 3;
        graph2[12][14] = 0;
        graph2[12][15] = 0;

        graph2[13][0] = 0;
        graph2[13][1] = 0;
        graph2[13][2] = 0;
        graph2[13][3] = 0;
        graph2[13][4] = 0;
        graph2[13][5] = 0;
        graph2[13][6] = 0;
        graph2[13][7] = 0;
        graph2[13][8] = 0;
        graph2[13][9] = 0;
        graph2[13][10] = 0;
        graph2[13][11] = 0;
        graph2[13][12] = 3;
        graph2[13][13] = 0;
        graph2[13][14] = 6;
        graph2[13][15] = 0;

        graph2[14][0] = 0;
        graph2[14][1] = 0;
        graph2[14][2] = 0;
        graph2[14][3] = 0;
        graph2[14][4] = 0;
        graph2[14][5] = 0;
        graph2[14][6] = 0;
        graph2[14][7] = 0;
        graph2[14][8] = 0;
        graph2[14][9] = 0;
        graph2[14][10] = 0;
        graph2[14][11] = 4;
        graph2[14][12] = 0;
        graph2[14][13] = 6;
        graph2[14][14] = 0;
        graph2[14][15] = 10;

        graph2[15][0] = 0;
        graph2[15][1] = 0;
        graph2[15][2] = 0;
        graph2[15][3] = 0;
        graph2[15][4] = 0;
        graph2[15][5] = 0;
        graph2[15][6] = 0;
        graph2[15][7] = 0;
        graph2[15][8] = 0;
        graph2[15][9] = 0;
        graph2[15][10] = 0;
        graph2[15][11] = 0;
        graph2[15][12] = 0;
        graph2[15][13] = 8;
        graph2[15][14] = 10;
        graph2[15][15] = 0;

        if (preset == 1)
        {
            printf("Preset 1\n");
            printf("                    (1) -1- (2) -4- (7)-\\ --15-- (10)       (13) --13-- (14)          \n");
            printf("                   /  \\      /       |   \\      /  | \\      /            |             \n");
            printf("                  3    4    2\        1    2    12  |  7    3             |            \n");
            printf("                 /      \\  /         |     \\  /    |   \\  /              |           \n");
            printf("              (0) --7--  (3)  --3-- (6) -1- (9)   13    (12)             16          \n");
            printf("               \\         / \\       / |     /  \\    |    /                |            \n");
            printf("                12     3    7    5   3   10    10  |   9                 |           \n");
            printf("                 \\    /      \\  /    |  /       \\  |  /                  |         \n");
            printf("                   (4) --8-- (5) -6- (8) ---7--- (11) ---------17------- (15)         \n");

            printf("Table Representation: \n");
            for (int i = 0; i < V; i++)
            {
                if(i <= 9)
                {
                    printf("Node %d : ", i);
                }
                else {
                    printf("Node %d: ", i);
                }

                for (int x = 0; x < V; x++)
                {

                    if (x == V - 1)
                    {
                        printf(" %3d |\n", graph1[i][x]);
                    }
                    else
                    {
                        printf(" %3d |", graph1[i][x]);
                    }
                }
            }
            printf("______________________________________\n");
            int src = -1;
            while (src < 0 || src >= V)
            {
                printf("Choose a starting point: ");
                scanf("%d", &src);
            }
            int tgt = -1;
            while (tgt < 0 || src >= V || tgt == src)
            {
                printf("Choose an end point: ");
                scanf("%d", &tgt);
            }

            int speed;
            printf("How fast will you be traveling: ");
            scanf("%d", &speed);

            dijkstra(graph1, src, tgt, speed);
        }
        else if (preset == 2)
        {
            printf("Preset 2\n");
            printf("            (0) --4-- (1)                (7) --3-- (11) --4-- (14) --10-- (15)\n");
            printf("            |        /   \\              /  \\        |          |         /      \n");
            printf("            |       /     3            6    9       |          6        8      \n");
            printf("            |      /       \\          /      \\      |          |       /        \n");
            printf("            2     5       (2) --5-- (6)      (8)    10        (13) ---/         \n");
            printf("            |    /        / \\       /  \\       \\    |          |              \n");
            printf("            |   /        3   6     7    3       8   |          3               \n");
            printf("            |  /        /     \\   /      \\       \\  |          |             \n");
            printf("            (4) --3-- (3)      (5)      (10) --7--(9)  --3--  (12)              \n");

            printf("Table Representation: \n");
            for (int i = 0; i < V; i++)
            {
                if(i <= 9)
                {
                    printf("Node %d : ", i);
                }
                else {
                    printf("Node %d: ", i);
                }

                for (int x = 0; x < V; x++)
                {

                    if (x == V - 1)
                    {
                        printf(" %3d |\n", graph2[i][x]);
                    }
                    else
                    {
                        printf(" %3d |", graph2[i][x]);
                    }
                }
            }
            printf("______________________________________\n");
            int src = -1;
            while (src < 0 || src >= V)
            {
                printf("Choose a starting point: ");
                scanf("%d", &src);
            }
            int tgt = -1;
            while (tgt < 0 || src >= V || tgt == src)
            {
                printf("Choose an end point: ");
                scanf("%d", &tgt);
            }

            int speed;
            printf("How fast will you be traveling: ");
            scanf("%d", &speed);

            dijkstra(graph2, src, tgt, speed);

        }
    }
}