#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// Define a structure for a movie
typedef struct Movie {
    char title[100];
    int releaseYear;
    struct Movie* left;
    struct Movie* right;
} Movie;

// Function to create a new movie node
Movie* createMovie(char title[], int releaseYear) {
    Movie* newMovie = (Movie*)malloc(sizeof(Movie));
    strcpy(newMovie->title, title);
    newMovie->releaseYear = releaseYear;
    newMovie->left = NULL;
    newMovie->right = NULL;
    return newMovie;
}

// Function to insert a movie into the binary search tree
Movie* insertMovie(Movie* root, char title[], int releaseYear) {
    if (root == NULL) {
        return createMovie(title, releaseYear);
    }
    
    if (strcmp(title, root->title) < 0) {
        root->left = insertMovie(root->left, title, releaseYear);
    } else if (strcmp(title, root->title) > 0) {
        root->right = insertMovie(root->right, title, releaseYear);
    }
    
    return root;
}

// Function to recommend a movie
void recommendMovie(Movie* root, char title[]) {
    if (root == NULL) {
        printf("No movie recommendations available.\n");
        return;
    }
    
    if (strcmp(title, root->title) == 0) {
        printf("Recommended movie: %s (%d)\n", root->right->title, root->right->releaseYear);
    } else if (strcmp(title, root->title) < 0) {
        recommendMovie(root->left, title);
    } else {
        recommendMovie(root->right, title);
    }
}

int main() {
    Movie* root = NULL;
    char a,b,c,d,e;

    //Taking the personal recommendation
    scanf("Your Favorite movie %c\n",&a);
    scanf("Your Favorite movie %c\n",&b);
    scanf("Your Favorite movie %c\n",&c);
    scanf("Your Favorite movie %c\n",&d);
    scanf("Your Favorite movie %c\n",&e);
    
    
    // Insert some sample movies
    root = insertMovie(root, "a", 2000);
    root = insertMovie(root, "b", 2010);
    root = insertMovie(root, "c", 2015);
    root = insertMovie(root, "d", 2020);
    root = insertMovie(root, "e", 2025);
    
    // Recommend a movie based on user preference
    char userPreference[] = "b";
    printf("User's preference: %s\n", userPreference);
    recommendMovie(root, userPreference);
    
    // Free the memory used by the binary search tree
    
    // You should implement a proper memory management system for a complete application.
    
    return 0;
}
