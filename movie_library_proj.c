#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define min(a, b) ((a) < (b) ? (a) : (b))

// Define a structure for a movie
typedef struct Movie
{
    char title[100];
    int releaseYear;
    char genre[100];
    char language[100];
    struct Movie *left;
    struct Movie *right;
} Movie;

// Defining a structure for kepping the count of each genre
typedef struct
{
    char *genre;
    int count;
} GenreCount;

int drama = 0, crime = 0, action = 0, fantasy = 0, sci_fi = 0, animation = 0, thriller = 0, horror = 0, adventure = 0, musical = 0, romance = 0, comedy = 0, sports = 0;
int num = 1;

Movie *createMovie(char title[], int releaseYear, char genre[], char language[]);
Movie *insertMovie(Movie *root, char title[], int releaseYear, char genre[], char language[], int database);
Movie *searchMovie(Movie *root, char title[]);
Movie *deleteMovie(Movie *root, char title[]);
int createTreeFromFile(Movie **root, char *filename, int database);
void listMovies(Movie *root, char *key, char *value, int *count, int maxMovies, Movie *list);
void genreCounter(char genre[], int key, int dataBase);
void recommendMovie(Movie **root, Movie *movieDataBase);
void shuffle(Movie *list, int n);
void saveProfile(Movie *root, FILE *file);
void inorder(Movie *p);
void destroy(Movie *p);

int main()
{
    Movie *root = NULL;
    Movie *movieDataBase = NULL;
    int count = 0;
    int choice = 0;
    char title[100];
    int releaseYear;
    char genre[100];
    char language[100];
    char option[100];
    char openprofile[100];

    int wrong = 1;

    // Creating a BST from the movie database
    createTreeFromFile(&movieDataBase, "movies.csv", 1);

    // user interface
    printf("\n__________________________________________________________\n\n");
    printf("Welcome to the Movie Library!\n\n");

    while (wrong)
    {
        printf("Do you want to Open existing Profile/ Create new Profile: (open/create): ");
        scanf("%s", option);
        if (strcmp(option, "open") == 0)
        {
            int wrong3 = 1;
            Movie *temp = NULL;
            // Creating a BST from the user profile
            while (1)
            {
                printf("\nEnter name of your profile (filename.csv): ");
                scanf("%s", openprofile);
                int len = strlen(openprofile);
                if (strcmp(openprofile, "movies.csv") == 0)
                {
                    printf("\nThis is the name of the database! Please enter a different name\n");
                    continue;
                }
                else if (!createTreeFromFile(&temp, openprofile, 0) || len < 4 || strcmp(openprofile + len - 4, ".csv") != 0)
                {
                    
                    printf("\nInvalid filename! The filename must exist in current directory and end with .csv\n");
                    continue;
                }
                else
                    break;
            }
            root = temp;
            wrong = 0;
        }
        else if (strcmp(option, "create") == 0)
        {
            wrong = 0;
            root = NULL;
        }
        else
        {
            printf("\nWrong input! Please try again\n");
        }
    }

    while (choice != 6)
    {
        printf("\nWhat would you like to do:\n");
        printf("1. Add a Movie to your Profile\n2. Delete a Movie from your Profile/DataBase\n3. Search for a Movie in your Profile\n4. List Movies from your Profile\n5. Get Movie Recommendations\n6. Exit\n>");
        scanf("%d", &choice);
        getchar(); // Consume the newline character

        switch (choice)
        {
        case 1:
            printf("\nEnter the title of the Movie: ");
            fgets(title, sizeof(title), stdin);
            title[strcspn(title, "\n")] = 0; // Remove the newline character

            printf("Enter the release year of the Movie: ");
            scanf(" %d", &releaseYear);
            getchar(); // Consume the newline character

            printf("Enter the genre of the Movie: ");
            fgets(genre, sizeof(genre), stdin);
            genre[strcspn(genre, "\n")] = 0; // Remove the newline character

            printf("Enter the language of the Movie: ");
            fgets(language, sizeof(language), stdin);
            language[strcspn(language, "\n")] = 0; // Remove the newline character

            printf("\nMovie added to your profile!\n");
            root = insertMovie(root, title, releaseYear, genre, language, 0);
            printf("__________________________________________________________\n\n");
            break;

        case 2:
            if (root == NULL)
            {
                printf("\nYour profile is empty! Please add Movies or get Recommendations\n");
                printf("__________________________________________________________\n\n");
                break;
            }
            else
            {
                printf("\nEnter the title of the Movie to be Deleted: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = 0;

                if (searchMovie(root, title) == NULL)
                {
                    printf("\nMovie not found in your profile!\n");
                    printf("__________________________________________________________\n\n");
                    break;
                }
                else
                {
                    root = deleteMovie(root, title);
                    printf("\nMovie deleted from your profile!\n");
                    printf("__________________________________________________________\n\n");
                    break;
                }
            }
        case 3:
            if (root == NULL)
            {
                printf("\nYour profile is empty! Please add Movies or get Recommendations\n");
                printf("__________________________________________________________\n\n");
                break;
            }
            else
            {
                printf("\nEnter the title of the Movie to be Searched: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = 0;

                if (searchMovie(root, title))
                    printf("\nMovie found!\n");
                else
                    printf("\nMovie not found!\n");
                printf("__________________________________________________________\n\n");
                break;
            }
        case 4:
            char selection[100];
            printf("\nDo you want to list movies from Database or your Profile? (database/profile): ");
            scanf("%s", selection);
            if (strcmp(selection, "database") == 0)
            {
                char key[100];
                char value[100];
                printf("\nDo you want to list all movies alphabetically or movies based on a key? (all/key): ");
                scanf("%s", key);
                if (strcmp(key, "all") == 0)
                {
                    inorder(movieDataBase);
                    printf("__________________________________________________________\n\n");
                    num = 1;
                    break;
                }
                else if (strcmp(key, "key") == 0)
                {
                    printf("\nEnter the key according to which Movies will be listed (genre/releaseyear/language): ");
                    scanf("%s", key);
                    printf("\nEnter the actual genre name/releaseyear/language name: ");
                    scanf("%s", value);
                    listMovies(movieDataBase, key, value, &count, -1, NULL);
                    printf("__________________________________________________________\n\n");
                    num = 1;
                    break;
                }
            }
            else if (strcmp(selection, "profile") == 0)
            {
                if (root == NULL)
                {
                    printf("\nYour profile is empty! Please add Movies or get Recommendations\n");
                    printf("__________________________________________________________\n\n");
                    break;
                }
                else
                {
                    char key[100];
                    char value[100];
                    printf("\nDo you want to list all movies alphabetically or movies based on a key? (all/key): ");
                    scanf("%s", key);
                    if (strcmp(key, "all") == 0)
                    {
                        inorder(root);
                        printf("__________________________________________________________\n\n");
                        num = 1;
                        break;
                    }
                    else if (strcmp(key, "key") == 0)
                    {
                        printf("\nEnter the key according to which Movies will be listed (genre/releaseyear/language): ");
                        scanf("%s", key);
                        printf("\nEnter the actual genre name/releaseyear/language name based on ur previous selection respectively: ");
                        scanf("%s", value);
                        listMovies(root, key, value, &count, -1, NULL);
                        printf("__________________________________________________________\n\n");
                        num = 1;
                        break;
                    }
                }
            }

        case 5:
            printf("\n");
            recommendMovie(&root, movieDataBase);
            printf("__________________________________________________________\n\n");
            break;

        case 6:
            break;
        default:
            printf("\nWrong input! Please try again\n");
            printf("__________________________________________________________\n\n");
            break;
        }
    }

    char ch;
    char selection[100];
    char saveprofile[100];
    int wrong2 = 1;
    char input[100];

    while (wrong2)
    {
        printf("\nDo you want to save your profile? (y/n): ");
        fgets(input, sizeof(input), stdin); // Read the entire line of input
        ch = input[0];
        if (ch == 'y')
        {
            wrong2 = 0;
            printf("\nSave to existing profile or create a new profile? (existing/new): ");
            scanf("%s", selection);
            if (strcmp(selection, "existing") == 0)
            {
                while (1)
                {
                    printf("Enter name for your file (filename.csv): ");
                    scanf("%s", saveprofile);
                    int len = strlen(saveprofile);
                    if (strcmp(saveprofile, "movies.csv") == 0)
                    {
                        printf("\nThis is the name of the database! Please enter a different name\n");
                        continue;
                    }
                    else if (len < 4 || strcmp(saveprofile + len - 4, ".csv") != 0)
                    {
                        printf("\nInvalid filename! The filename must end with .csv\n");
                        continue;
                    }
                    else
                        break;
                }
                FILE *file = fopen(saveprofile, "r");
                if (file == NULL)
                {
                    printf("File does not exist!\n");
                    fclose(file);
                }
                else
                {
                    if (strcmp(openprofile, saveprofile) == 0)
                    {
                        file = fopen(saveprofile, "w"); // Reopen the file in write mode
                        fprintf(file, "title,genre,releaseyear,language\n");
                    }
                    else
                        file = fopen(saveprofile, "a"); // Reopen the file in append mode

                    if (file == NULL)
                    {
                        printf("Error opening file!\n");
                    }
                    else
                    {
                        saveProfile(root, file);
                        printf("\n\nProfile saved to %s successfully!\n", saveprofile);
                        fclose(file);
                    }
                }
            }
            else if (strcmp(selection, "new") == 0)
            {
                while (1)
                {
                    printf("Enter name for your file (filename.csv): ");
                    scanf("%s", saveprofile);
                    int len = strlen(saveprofile);
                    if (strcmp(saveprofile, "movies.csv") == 0)
                    {
                        printf("\nThis is the name of the database! Please enter a different name\n");
                        continue;
                    }
                    else if (len < 4 || strcmp(saveprofile + len - 4, ".csv") != 0)
                    {
                        printf("\nInvalid filename! The filename must end with .csv\n");
                        continue;
                    }
                    else
                        break;
                }
                FILE *file = fopen(saveprofile, "w");
                if (file == NULL)
                    printf("Error opening file!\n");

                else
                {
                    fprintf(file, "title,genre,releaseyear,language\n");
                    saveProfile(root, file);
                    printf("\n\nProfile saved successfully!\n");
                }
                fclose(file);
            }
        }
        else if (ch == 'n')
        {
            wrong2 = 0;
            printf("\nProfile not saved!\n");
        }
        else
        {
            printf("\nWrong input! Please try again\n");
            continue;
        }
    }

    destroy(root);
    destroy(movieDataBase);
    root = NULL;
    movieDataBase = NULL;

    printf("Thank you for using our Movie Library!\n");
    printf("__________________________________________________________\n\n");
    return 0;
}

// Function to create a new movie node
Movie *createMovie(char title[], int releaseYear, char genre[], char language[])
{
    Movie *newMovie = (Movie *)malloc(sizeof(Movie));
    strcpy(newMovie->title, title);
    strcpy(newMovie->genre, genre);
    strcpy(newMovie->language, language);
    newMovie->releaseYear = releaseYear;
    newMovie->left = NULL;
    newMovie->right = NULL;
    return newMovie;
}

// Function to insert a movie into the binary search tree
Movie *insertMovie(Movie *root, char title[], int releaseYear, char genre[], char language[], int database)
{
    if (root == NULL)
    {
        genreCounter(genre, 1, database);
        return createMovie(title, releaseYear, genre, language);
    }

    int comparison = strcmp(title, root->title);
    if (comparison < 0)
    {
        root->left = insertMovie(root->left, title, releaseYear, genre, language, database);
    }
    else if (comparison > 0)
    {
        root->right = insertMovie(root->right, title, releaseYear, genre, language, database);
    }
    else
    {
        printf("Movie already exists in the library.\n");
    }

    return root;
}

// Function to search for a movie in the BST
Movie *searchMovie(Movie *root, char title[])
{

    if (root == NULL)
        return root;

    if (strcmp(root->title, title) == 0)
    {
        genreCounter(root->genre, 1, 0);
        printf("\n%d. %s:-\n    Genre: %s\n    Released Year: %d\n    Language: %s\n", num, root->title, root->genre, root->releaseYear, root->language);
        return root;
    }

    if (strcmp(title, root->title) < 0)
    {
        return searchMovie(root->left, title);
    }
    else
    {
        return searchMovie(root->right, title);
    }
}

// function to traverse through the BST in inorder fashion and print the movies in alphabetical order
void inorder(Movie *root)
{
    if (root != NULL)
    {
        inorder(root->left);
        printf("\n%d. %s:-\n    Genre: %s\n    Released Year: %d\n    Language: %s\n", num, root->title, root->genre, root->releaseYear, root->language);
        num++;
        inorder(root->right);
    }
}

// Function to delete a movie from the BST
Movie *deleteMovie(Movie *root, char title[])
{

    Movie *temp;
    if (root == NULL)
    {
        return root;
    }

    if (strcmp(title, root->title) < 0)
    {
        root->left = deleteMovie(root->left, title);
    }
    else if (strcmp(title, root->title) > 0)
    {
        root->right = deleteMovie(root->right, title);
    }

    else
    {
        // Node with only one child or no child
        if (root->left == NULL)
        {
            temp = root->right;
            genreCounter(root->genre, -1, 0);
            free(root);
            return temp;
        }
        else if (root->right == NULL)
        {
            temp = root->left;
            genreCounter(root->genre, -1, 0);
            free(root);
            return temp;
        }

        // Node with 2 children, Replace with inorder successor
        else
        {
            temp = root->right;
            while (temp->left != NULL)
                temp = temp->left;

            strcpy(root->title, temp->title);
            strcpy(root->genre, temp->genre);
            strcpy(root->language, temp->language);
            root->releaseYear = temp->releaseYear;
            root->right = deleteMovie(root->right, temp->title);
        }
    }
    return root;
}

// Function to create a movie BST from a csv file
int createTreeFromFile(Movie **root, char *filename, int database)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Could not open file %s.\n", filename);
        return 0;
    }

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        char *title = strtok(line, ",");
        char *genre = strtok(NULL, ",");
        int releaseYear = atoi(strtok(NULL, ","));
        char *language = strtok(NULL, ",");

        if (title && genre && releaseYear && language)
        {
            *root = insertMovie(*root, title, releaseYear, genre, language, database);
        }
    }

    fclose(file);
    return 1;
}

// Function to list movies based on a key, value and a maximum number of movies to be listed
void listMovies(Movie *root, char *key, char *value, int *count, int maxMovies, Movie *list)
{
    if (root == NULL || (*count >= maxMovies && maxMovies != -1))
    {
        return;
    }

    listMovies(root->left, key, value, count, maxMovies, list);

    // Trimming leading and trailing whitespace from language
    char *trimmedLanguage = strdup(root->language);
    trimmedLanguage = strtok(trimmedLanguage, " \t\n\r\f\v"); // Trim leading whitespace
    char *end = trimmedLanguage + strlen(trimmedLanguage) - 1;
    while (end > trimmedLanguage && isspace((unsigned char)*end))
        end--; // Trim trailing whitespace
    *(end + 1) = 0;

    if ((strcmp(key, "releaseyear") == 0 && root->releaseYear == atoi(value)) ||
        (strcmp(key, "genre") == 0 && strcmp(root->genre, value) == 0) ||
        (strcmp(key, "language") == 0 && strcmp(trimmedLanguage, value) == 0))
    {
        if (list == NULL)
            printf("\n%d. %s:-\n    Genre: %s\n    Released Year: %d\n    Language: %s\n", num, root->title, root->genre, root->releaseYear, root->language);
        else
            memcpy(&list[*count], root, sizeof(Movie));

        (*count)++;
        num++;
    }

    free(trimmedLanguage);

    listMovies(root->right, key, value, count, maxMovies, list);
}

// Function to keep count of each genre
void genreCounter(char genre[], int key, int dataBase)
{
    if (dataBase)
    {
        return;
    }

    if (strcmp(genre, "drama") == 0)
        drama += key;
    else if (strcmp(genre, "crime") == 0)
        crime += key;
    else if (strcmp(genre, "action") == 0)
        action += key;
    else if (strcmp(genre, "fantasy") == 0)
        fantasy += key;
    else if (strcmp(genre, "sci-fi") == 0)
        sci_fi += key;
    else if (strcmp(genre, "animation") == 0)
        animation += key;
    else if (strcmp(genre, "thriller") == 0)
        thriller += key;
    else if (strcmp(genre, "horror") == 0)
        horror += key;
    else if (strcmp(genre, "adventure") == 0)
        adventure += key;
    else if (strcmp(genre, "musical") == 0)
        musical += key;
    else if (strcmp(genre, "romance") == 0)
        romance += key;
    else if (strcmp(genre, "comedy") == 0)
        comedy += key;
    else if (strcmp(genre, "sports") == 0)
        sports += key;
}

void shuffle(Movie *list, int n)
{
    if (n > 1)
    {
        srand(time(NULL));
        for (int i = n - 1; i > 0; i--)
        {
            int j = rand() % (i + 1);
            Movie temp = list[j];
            list[j] = list[i];
            list[i] = temp;
        }
    }
}

// Function to recommend a movie from the movie database based on the genre count of user profile
void recommendMovie(Movie **root, Movie *movieDataBase)
{
    Movie *list = (Movie *)malloc(100 * sizeof(Movie));
    Movie displayList[30];
    int j = 0;
    if (drama == 0 && crime == 0 && action == 0 && fantasy == 0 && sci_fi == 0 &&
        animation == 0 && thriller == 0 && horror == 0 && adventure == 0 && musical == 0 &&
        romance == 0 && comedy == 0 && sports == 0)
    {
        srand(time(NULL));
        GenreCount genres[] = {
            {"drama", rand() % 100},
            {"crime", rand() % 100},
            {"action", rand() % 100},
            {"fantasy", rand() % 100},
            {"sci-fi", rand() % 100},
            {"animation", rand() % 100},
            {"thriller", rand() % 100},
            {"horror", rand() % 100},
            {"adventure", rand() % 100},
            {"musical", rand() % 100},
            {"romance", rand() % 100},
            {"comedy", rand() % 100},
            {"sports", rand() % 100}};

        int n = sizeof(genres) / sizeof(GenreCount);

        // Sort the genres array in descending order of count
        for (int i = 0; i < n; i++)
        {
            for (int j = i + 1; j < n; j++)
            {
                if (genres[j].count > genres[i].count)
                {
                    GenreCount temp = genres[i];
                    genres[i] = genres[j];
                    genres[j] = temp;
                }
            }
        }

        for (int i = 0; i < 5; i++)
        {
            int count = 0;
            listMovies(movieDataBase, "genre", genres[i].genre, &count, -1, list);
            shuffle(list, count);
            for (int k = 0; k < min(5, count) && j < sizeof(displayList) / sizeof(Movie); k++)
            {
                displayList[j++] = list[k];
            }
            printf("\n");
            free(list);
            list = (Movie *)malloc(100 * sizeof(Movie));
        }
    }

    else
    {
        GenreCount genres[] = {
            {"drama", drama},
            {"crime", crime},
            {"action", action},
            {"fantasy", fantasy},
            {"sci-fi", sci_fi},
            {"animation", animation},
            {"thriller", thriller},
            {"horror", horror},
            {"adventure", adventure},
            {"musical", musical},
            {"romance", romance},
            {"comedy", comedy},
            {"sports", sports}};

        int n = sizeof(genres) / sizeof(GenreCount);

        // Sort the genres array in descending order of count
        for (int i = 0; i < n; i++)
        {
            for (int j = i + 1; j < n; j++)
            {
                if (genres[j].count > genres[i].count)
                {
                    GenreCount temp = genres[i];
                    genres[i] = genres[j];
                    genres[j] = temp;
                }
            }
        }

        for (int i = 0; i < 5; i++)
        {
            int count = 0;
            listMovies(movieDataBase, "genre", genres[i].genre, &count, -1, list);
            shuffle(list, count);
            for (int k = 0; k < min(5, count) && j < sizeof(displayList) / sizeof(Movie); k++)
            {
                displayList[j++] = list[k];
            }
            printf("\n");
            free(list);
            list = (Movie *)malloc(100 * sizeof(Movie));
        }
    }
    for (int i = 0; i < j - 1; i++)
    {
        printf("\n%d. %s:-\n    Genre: %s\n    Released Year: %d\n    Language: %s\n", i + 1, displayList[i].title, displayList[i].genre, displayList[i].releaseYear, displayList[i].language);
    }

    int serial;
    while (1)
    {
        printf("\nEnter the Serial number of the movie you want to add to your profile (-1 to skip): ");
        scanf("%d", &serial);
        if (serial == -1)
            break;

        if (serial > j - 1 || serial < 1)
        {
            printf("\nWrong input! Please enter the right number again\n");
            continue;
        }
        else
            break;
    }
    printf("\nMovie added to your profile!\n");
    *root = insertMovie(*root, displayList[serial - 1].title, displayList[serial - 1].releaseYear, displayList[serial - 1].genre, displayList[serial - 1].language, 0);
    num = 1;
    free(list);
}

// Funtion to save the user profile in a csv file
void saveProfile(Movie *root, FILE *file)
{
    if (root == NULL)
    {
        return;
    }

    saveProfile(root->left, file);

    fprintf(file, "%s,%s,%d,%s\n", root->title, root->genre, root->releaseYear, root->language);

    saveProfile(root->right, file);
}

// Function to destroy the BST after the program is terminated
void destroy(Movie *p)
{
    if (p != NULL)
    {
        destroy(p->left);
        destroy(p->right);
        free(p);
    }
}
