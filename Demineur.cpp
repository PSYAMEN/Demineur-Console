#include <iostream>
#include <string.h>
#include <time.h>
#include <math.h>
using namespace std;

int const TAILLE = 102;

struct terre
{
    bool bombe = 0;//change to bool
    bool retournee = 0;//change to bool
    int nbnear = 0;
};

struct field
{
    int tx;
    int ty;
    int nbcase;
    bool mort = false;
    struct terre field[TAILLE][TAILLE];
};

void difficulte(struct field& T, int& bombe)
{
    int x;
    cout << "combien de bombe" << endl;
    cin >> bombe;
    cout << "taille de la grille? <=100\n";
    cin >> x;
    T.tx = T.ty = x + 1;
    T.nbcase = x * x - bombe;//fix it by not +1 but +1 everywhere else
}

void initTableau(struct field& T, int nbbombe)
{
    while (nbbombe != 0)//cremen
    {
        int r1 = rand() % (T.tx - 1) + 1;
        int r2 = rand() % (T.ty - 1) + 1;
        if (T.field[r1][r2].bombe == 0 && T.field[r1][r2].retournee == 0)
        {
            T.field[r1][r2].bombe = 1;
            T.field[r1][r2].nbnear = -1;
            for (int i = -1; i <= 1; i++)
            {
                for (int j = -1; j <= 1; j++)
                {
                    T.field[r1 + i][r2 + j].nbnear++;
                }
            }
            nbbombe--;
        }
    }
}

void affichage(struct field T)
{
    // coordonees.,..
    for (int i = 1; i < T.tx; i++)
    {
        for (int j = 1; j < T.ty; j++)
        {
            if (T.field[i][j].retournee == 0)
            {
                cout << "* ";
            }
            else
            {
                if (T.field[i][j].bombe == 0) { cout << T.field[i][j].nbnear << ' '; }
                else { cout << "B "; }
            }

        }
        cout << '\n';
    }
}

void spread(struct field& T, int x, int y)
{
    if (T.field[x][y].retournee == 0)
    {
        T.nbcase--;
    }
    T.field[x][y].retournee = 1;

    if (T.field[x][y].nbnear == 0 && T.field[x - 1][y + 1].retournee == 0)
    {
        T.field[x - 1][y + 1].retournee = 1;
        spread(T, x - 1, y + 1);
    }

    if (T.field[x][y].nbnear == 0 && T.field[x - 1][y].retournee == 0)
    {
        T.field[x - 1][y].retournee = 1;
        spread(T, x - 1, y);
    }

    if (T.field[x][y].nbnear == 0 && T.field[x - 1][y - 1].retournee == 0)
    {
        T.field[x - 1][y - 1].retournee = 1;
        spread(T, x - 1, y - 1);
    }

    if (T.field[x][y].nbnear == 0 && T.field[x][y - 1].retournee == 0)
    {
        T.field[x][y - 1].retournee = 1;
        spread(T, x, y - 1);
    }

    if (T.field[x][y].nbnear == 0 && T.field[x + 1][y - 1].retournee == 0)
    {
        T.field[x + 1][y - 1].retournee = 1;
        spread(T, x + 1, y - 1);
    }

    if (T.field[x][y].nbnear == 0 && T.field[x + 1][y].retournee == 0)
    {
        T.field[x + 1][y].retournee = 1;
        spread(T, x + 1, y);
    }

    if (T.field[x][y].nbnear == 0 && T.field[x + 1][y + 1].retournee == 0)
    {
        T.field[x + 1][y + 1].retournee = 1;
        spread(T, x + 1, y + 1);
    }

    if (T.field[x][y].nbnear == 0 && T.field[x][y + 1].retournee == 0)
    {
        T.field[x][y + 1].retournee = 1;
        spread(T, x, y + 1);
    }
}

void spread2(struct field& T, int x, int y)
{

    if (T.field[x][y].retournee == 1) return;
    T.field[x][y].retournee = 1;
    T.nbcase--;
    if (T.field[x][y].nbnear != 0) return;
    spread2(T, x - 1, y - 1); spread2(T, x, y - 1);  spread2(T, x + 1, y - 1);
    spread2(T, x - 1, y);                      spread2(T, x + 1, y);
    spread2(T, x - 1, y + 1); spread2(T, x, y + 1);  spread2(T, x + 1, y + 1);
}

void spread3(struct field& T, int x, int y)
{
    int stacke[TAILLE * TAILLE][2];
    stacke[0][0] = x;
    stacke[0][1] = y;
    int compte = 1;
    do
    {
        compte--;
        int savex = stacke[compte][0];
        int savey = stacke[compte][1];
        T.field[savex][savey].retournee = 1;
        T.nbcase--;
        if (T.field[savex][savey].nbnear == 0)
        {
            for (int i = -1; i <= 1; i++)
            {
                for (int j = -1; j <= 1; j++)
                {
                    if (T.field[savex - i][savey - j].retournee == 0)
                    {
                        T.field[savex - i][savey - j].retournee = 1;
                        stacke[compte][0] = savex - i; stacke[compte][1] = savey - j;
                        compte++;
                    }
                }
            }
        }


    } while (compte != 0 && compte != -1);
}

void first(struct field& T, int bombes)
{
    affichage(T);
    int x, y;
    cout << "ligne\n";
    cin >> x;
    cout << "colone\n";
    cin >> y;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            T.field[x + 1 + i][y + 1 + j].retournee = 1;
        }
    }
    initTableau(T, bombes);
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            T.field[x + 1 + i][y + 1 + j].retournee = 0;
        }
    }
    for (int i = 0; i <= T.tx; i++)
    {
        T.field[i][0].nbnear = T.field[0][i].nbnear = T.field[i][T.tx].nbnear = T.field[T.ty][i].nbnear = 1;
        T.field[i][0].retournee = T.field[0][i].retournee = T.field[i][T.tx].retournee = T.field[T.ty][i].retournee = 1;
    }
    spread3(T, x + 1, y + 1);
}

void affichetest(struct field T)
{
    for (int i = 1; i < T.tx; i++)
    {
        for (int j = 1; j < T.tx; j++)
        {
            cout << T.field[i][j].nbnear;
        }
        cout << '\n';
    }
}

void fin(struct field T)
{
    for (int i = 1; i < T.tx; i++)
    {
        for (int j = 1; j < T.ty; j++)
        {
            if (T.field[i][j].bombe == 1)
            {
                cout << "B ";
            }
            else { cout << T.field[i][j].nbnear << ' '; }
        }
        cout << '\n';
    }
}

int main()
{
    srand(time(NULL));
    struct field demineur;
    int x, y, nbbombe;
    difficulte(demineur, nbbombe);
    first(demineur, nbbombe);
    affichage(demineur);
    while (demineur.nbcase != 0 && demineur.mort != true)
    {
        cout << "ligne\n";
        cin >> x;
        cout << "colone\n";
        cin >> y;
        cout << '\n';
        affichetest(demineur);
        spread3(demineur, x + 1, y + 1);
        affichage(demineur);
        if (demineur.field[x + 1][y + 1].bombe == 1)
        {
            demineur.mort = true;
        }
    }
    if (demineur.mort == true) { cout << "perdu\n"; }
    else { cout << "gagner\n"; }
    fin(demineur);
    return 0;
}
