//BOLOCAN CRINA-MARIA - 311 CB
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _director
{
    char *name;
    struct _director *parent;
    struct _fisier *files;
    struct _director *directories;
    struct _director *left;
    struct _director *right;
} Dir; // strcutura de director

typedef struct _fisier
{
    char *name;
    struct _director *parent;
    struct _fisier *left;
    struct _fisier *right;
} Fisier; // structura de fisier

void initializareDIR(Dir *dir) // functie ce initializeaza directorul cu root
{
    if ((dir)->name == NULL)
    {
        (dir)->name = calloc(1, strlen("root") + 1);
        dir->name = "root";
        (dir)->parent = NULL;
        (dir)->directories = NULL;
        (dir)->files = NULL;
        (dir)->left = NULL;
        (dir)->right = NULL;
    }
    else
        printf("Eroare");
}
void cautarePWD(Dir *dir, char *nume, Dir **aux_dir)
{
    if (dir->parent != NULL)
        cautarePWD(dir->parent, nume, aux_dir);
    printf("/%s", dir->name);
} // functie ce mi cauta recursiv pentru pwd parent-ul directorului
// pentru a-mi afisa in ordinea ceruta(root->director)
// functia ajunge la root si mi afiseaza apoi toate directoarele pana la nume

int cautareDir(Dir *dir, char *nume, Dir **aux_dir, int contor)
{                   // functie de cautare director
    *aux_dir = dir; // auxiliar ce-mi salveaza la fiecare intrare directorul
    if (dir == NULL)
        return -1;
    else
    {
        if (strcmp(dir->name, nume) == 0)
        {
            if (contor == 1)
            {
                printf("Directory %s already exists!\n", nume);
                return 1; // aici mi-a gasit directorul
            }
            else if (contor == 0)
                return 1; // conditie separata pentru functia de rmdir
        }
        else if (strcmp(dir->name, nume) > 0)
        { // daca directorul e mai mic trebuie sa o ia in stanga
            if (dir->left == NULL)
            { // daca stanga e NULL imi iese
                return 0;
            }
            else // daca nu e NULL imi face iar cautarea
                // pentru directorul mai mic adica cel din stanga
                cautareDir(dir->left, nume, aux_dir, contor);
        }
        else if (strcmp(dir->name, nume) < 0)
        { // daca directorul e mai mare trebuie sa o ia in dreapta
            if (dir->right == NULL)
            { // imi iese daca nu exista dreapta
                return 0;
            }
            else // imi cauta iar, dar pentru directorul din dreapta
                cautareDir(dir->right, nume, aux_dir, contor);
        }
    }
}

int cautareFile(Fisier *files, char *nume, Fisier **aux_file, int contor)
{ // o functie ce-mi cauta prin fisiere fisierul dat
    if (files != NULL)
        *aux_file = files; // auxiliar ce-mi salveaza
    // de fiecare data fisierul cu care intru
    if (files == NULL)
        return -1;
    else
    {
        if (strcmp(files->name, nume) == 0)
        {
            if (contor == 1)
            { // fisierul deja exista
                printf("File %s already exists!\n", nume);
                return 1;
            }
            else if (contor == 0)
                return 1; // conditie separata pentru rm
        }
        else if (strcmp(files->name, nume) > 0)
        { // trebuie sa verific in stanga recursiv
            if (files->left == NULL)
            {
                return 0;
            }
            else
                cautareFile(files->left, nume, aux_file, contor);
        }
        else if (strcmp(files->name, nume) < 0)
        { // trebuie sa verific in dreapta recursiv
            if (files->right == NULL)
            {
                return 0;
            }
            else
                cautareFile(files->right, nume, aux_file, contor);
        }
    }
}

void touch(Dir *dir, char *nume)
{
    int contor = 1;
    if (dir->files == NULL)
    { // daca nu exista niciun fisier trebuie sa atribui pe cel cerut
        // touch creeaza un fisier
        Dir *aux_dir;
        if (cautareDir(dir->directories, nume, &aux_dir, contor) != 1)
        { // am cautat ca sa nu mi existe deja un director cu acelasi nume
            // daca nu exista structura de fisier trebuie initializata
            Fisier *file = calloc(1, sizeof(Fisier));
            file->name = calloc(1, strlen(nume) + 1);
            file->name = nume;
            file->parent = dir;
            file->right = NULL;
            file->left = NULL;
            dir->files = file;
        }
    }
    else
    { // daca exista deja fisiere
        Fisier *aux_file;
        Dir *aux_dir;
        if (cautareDir(dir->directories, nume, &aux_dir, contor) == 1)
            nume=nume; // verifica daca exista director cu acelasi nume
        else if ((cautareDir(dir->directories, nume, &aux_dir, contor) == 0
         || cautareDir(dir->directories, nume, &aux_dir, contor) == -1)
         && cautareFile(dir->files, nume, &aux_file, contor) == 0)
        { // verifica daca nu exista director cu acelasi nume
            // si daca nu exita fisier cu acelasi nume
            if (strcmp(aux_file->name, nume) > 0)
            { // se verifica daca numele e mai mic ca fisierul curent
                // se introduce in stanga
                Fisier *file = calloc(1, sizeof(Fisier));
                file->name = nume;
                file->parent = dir;
                file->left = NULL;
                file->right = NULL;
                aux_file->left = file;
            }
            else if (strcmp(aux_file->name, nume) < 0)
            { // se verifica daca numele e mai mare ca fisierul curent
                // se introduce in dreapta
                Fisier *file = calloc(1, sizeof(Fisier));
                file->name = nume;
                file->parent = dir;
                file->left = NULL;
                file->right = NULL;
                aux_file->right = file;
            }
        }
    }
}

void mkdir(Dir *dir, char *nume)
{ // functie ce mi introduce un nou director
    int contor = 1;
    if (dir->directories == NULL)
    {
        Fisier *aux_file;
        if (cautareFile(dir->files, nume, &aux_file, contor) != 1)
        {   // se verifica daca a exista un fisier cu acelasi nume
            // daca nu, capul directoarelor se initializeaza cu noul director
            Dir *director = calloc(1, sizeof(Fisier));
            director->name = calloc(1, strlen(nume));
            director->name = nume;
            director->parent = dir;
            director->right = NULL;
            director->left = NULL;
            dir->directories = director;
        }
    }
    else
    {
        Fisier *aux_file;
        Dir *aux_dir;
        if (cautareFile(dir->files, nume, &aux_file, contor) == 1)
            nume=nume; // se verifica daca exista fisier
        else if ((cautareFile(dir->files, nume, &aux_file, contor) == 0
         || cautareFile(dir->files, nume, &aux_file, contor) == -1)
         && cautareDir(dir->directories, nume, &aux_dir, contor) == 0)
        { // se verifica daca nu exista fisier cu acelasi nume sau director
            if (strcmp(aux_dir->name, nume) > 0)
            { // se introudce in stanga
                Dir *director = calloc(1, sizeof(Fisier));
                director->name = calloc(1, strlen(nume));
                director->name = nume;
                director->parent = dir;
                director->right = NULL;
                director->left = NULL;
                aux_dir->left = director;
            }
            else if (strcmp(aux_dir->name, nume) < 0)
            { // se introduce in dreapta
                Dir *director = calloc(1, sizeof(Fisier));
                director->name = calloc(1, strlen(nume));
                director->name = nume;
                director->parent = dir;
                director->right = NULL;
                director->left = NULL;
                aux_dir->right = director;
            }
        }
    }
}

void lsdir(Dir *dir, int *n)
{ // functie ce-mi afiseaza directoarele
    if (!dir)
    {
        if (*n == 1)
            printf("\n");
        return;
    }
    n++;
    if (!dir->left && !dir->right)
    { // daca si stanga, si dreapta sunt NULL
        // imi afiseaza directorul curent
        printf("%s ", dir->name);
    }
    else
    { // daca nu, o ia recursiv prin stanga
        lsdir(dir->left, n);
        // imi afiseaza directorul curent
        printf("%s ", dir->name);
        // si recursiv prin dreapta
        lsdir(dir->right, n);
    } // le-am apelat asa ca sa fie in ordine crescatoare
}

void lsfile(Fisier *file, int *n)
{ // asemantor ca la lsdir daor ca pentru fisiere
    if (!file)
    {
        if (*n == 1)
            printf("\n");
        return;
    }
    n++;
    if (!file->left && !file->right)
    {
        printf("%s ", file->name);
    }
    else
    { // printf("%s ", dir->directories->left->name);
        lsfile(file->left, n);
        printf("%s ", file->name);
        lsfile(file->right, n);
    }
}

Fisier *minvalue_file(Fisier *file)
{ // functie ce-mi calculeaza minimul de pe o latura
    // ca sa stiu ce introduc in nodul pe care l-am sters cu rm
    Fisier *aux = file;
    while (aux && aux->left != NULL)
        aux = aux->left; // se duce pe stanga
    return aux;          // returneaza valoarea cea mai mica
}

Dir *minvalue_dir(Dir *dir)
{ // asemanator ca la minvalue_file doar ca pentru director
    Dir *aux = dir;
    while (aux && aux->left != NULL)
        aux = aux->left;
    return aux;
}

Fisier *rm(Fisier *files, char *nume)
{ // functia de stergere a unui fisier
    if (files == NULL)
    { // fisierul cu numele nu exista
        printf("File %s doesn't exist!\n", nume);
        return files;
    }
    if (strcmp(files->name, nume) > 0)
        files->left = rm(files->left, nume);
    // daca numele e mai mic o ia prin stanga
    // si-mi apeleaza recursiv functia de rm
    // ca sa-mi caute fisierul pe care vreau sa-l sterg
    // ii atribui celui din stanga valoarea de return a functiei
    else if (strcmp(files->name, nume) < 0)
        files->right = rm(files->right, nume);
    // asemanator si in dreapta
    else
    { // cazurile in care ceea ce sterg e pe ultimul nivel
        if (files->left == NULL)
        { // atunci cand imi gaseste fisierul de sters
            //(fisier gasit pe latura din stanga) iau un auxiliar
            // ca sa salvez dreapta si sa refac legatura
            Fisier *aux;
            aux = files->right;
            free(files);//am dat free la ceea ce am sters
            return aux;
        }
        else if (files->right == NULL)
        { // la fel si aici doar ca pentru partea dreapta
            Fisier *aux;
            aux = files->left;
            free(files);
            return aux;
        }
        // daca nu inlocuiesc cu valoarea minima calculata
        Fisier *aux = minvalue_file(files->right);
        files->name = aux->name;
        files->right = rm(files->right, aux->name);
    }
    return files;
}

Dir *rmdir(Dir *dir, char *nume)
{ // asemanator cu rm doar ca pentru directoare
    if (dir == NULL)
    {
        printf("Directory %s doesn't exist!\n", nume);
        return dir;
    }
    if (strcmp(dir->name, nume) > 0)
        dir->left = rmdir(dir->left, nume);
    else if (strcmp(dir->name, nume) < 0)
        dir->right = rmdir(dir->right, nume);
    else
    {
        if (dir->left == NULL)
        {
            Dir *aux;
            aux = dir->right;
            return aux;
        }
        else if (dir->right == NULL)
        {
            Dir *aux;
            aux = dir->left;
            return aux;
        } // nu am mai putut sa dau free la ce stergeam ca mi afisa random
        // asa ca am renuntat la ele
        Dir *aux = minvalue_dir(dir->right);
        dir->name = aux->name;
        dir->right = rmdir(dir->right, aux->name);
    }
    return dir;
}

void cd(Dir **dir, char *nume)
{ // functie ce-mi schimba calea
    int contor = 0;
    Dir *aux_dir;
    if (strcmp(nume, "..") == 0)
        *dir = (*dir)->parent; // o ia inapoi cu un director
    else if (cautareDir((*dir)->directories, nume, &aux_dir, contor) == 1)
        *dir = aux_dir; //cauta directorul si atunci cand il gaseste il schimba
    else
        printf("Directory not found!\n"); // nu l-a gasit
}

void pwd(Dir *dir)
{ // apeleaza cautarePWD sa-mi afiseze calea
    Dir *aux = dir;
    Dir *aux_dir;
    cautarePWD(dir, aux->name, &aux_dir);
    printf("\n");
}

// void find_dir(Dir *root, char *nume)
// {
//     Dir *aux_dir;
//     int contor = 0;
//     int ok = 0;
//     while (ok == 0)
//     {
//         if (cautareDir(root, nume, &aux_dir, contor) == 1)
//             ok = 1;
//         if (cautareDir(root->directories, nume, &aux_dir, contor) == 1)
//             ok = 1;
//     }
//     if (ok == 0)
//         printf("Directory %s not found!\n", nume);
//     else
//         printf("Directory %s found!\n", nume);
// }

int main()
{ // citirea de la tastatura
    Dir *dir = malloc(sizeof(struct _director));
    initializareDIR(dir);
    char *comanda = calloc(50, sizeof(char));
    while (scanf("%s", comanda) == 1)
    {
        if (strcmp(comanda, "touch") == 0)
        {
            char *nume = calloc(50, sizeof(char));
            scanf("%s", nume);
            touch(dir, nume);
        }
        else if (strcmp(comanda, "mkdir") == 0)
        {
            char *nume = calloc(50, sizeof(char));
            scanf("%s", nume);
            mkdir(dir, nume);
        }
        else if (strcmp(comanda, "ls") == 0)
        {
            int n = 1;
            if (dir->directories == NULL && dir->files != NULL)
            {
                lsfile(dir->files, &n);
                printf("\n");
            }
            else if (dir->directories != NULL && dir->files == NULL)
            {
                lsdir(dir->directories, &n);
                printf("\n");
            }
            else if (dir->directories == NULL && dir->files == NULL)
                printf("\n");
            else
            {
                lsdir(dir->directories, &n);
                lsfile(dir->files, &n);
                printf("\n");
            }
        }
        else if (strcmp(comanda, "rm") == 0)
        {
            char *nume = calloc(50, sizeof(char));
            scanf("%s", nume);
            Fisier *aux_file;
            int contor = 0;
            if (cautareFile(dir->files, nume, &aux_file, contor) == 0)
                printf("File %s doesn't exist!\n", nume);
            else
                dir->files = rm(dir->files, nume);
        }
        else if (strcmp(comanda, "rmdir") == 0)
        {
            char *nume = calloc(50, sizeof(char));
            scanf("%s", nume);
            Dir *aux_dir;
            int contor = 0;
            if (cautareDir(dir->directories, nume, &aux_dir, contor) == 0)
                printf("Directory %s doesn't exist!\n", nume);
            else
                dir->directories = rmdir(dir->directories, nume);
        }
        else if (strcmp(comanda, "cd") == 0)
        {
            char *nume = calloc(50, sizeof(char));
            scanf("%s", nume);
            cd(&dir, nume);
        }
        else if (strcmp(comanda, "pwd") == 0)
        {
            pwd(dir);
        }
        // else if (strcmp(comanda, "find") == 0)
        // {
        //     char *conditie = calloc(100, sizeof(char));
        //     scanf("%s", conditie);
        //     char *nume = calloc(100, sizeof(char));
        //     scanf("%s", nume);
        //     if (strcmp(conditie, "-d") == 0)
        //         find_dir(dir, nume);
        // }
        else if (strcmp(comanda, "quit") == 0)
        {
            return 0;
        }
    }
}