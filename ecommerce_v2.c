#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 10

typedef struct User {
    int user_id;
    char name[50];
    struct User *next;
} User;

typedef struct Product {
    int product_id;
    char name[50];
    int stock;
    struct Product *next;
} Product;

typedef struct GraphNode {
    int id;
    struct GraphNode *next;
} GraphNode;

typedef struct InteractionGraph {
    GraphNode *adj_list[TABLE_SIZE];
} InteractionGraph;

int hashFunction(int id) {
    return id % TABLE_SIZE;
}

void addUser(User *table[], int user_id, char name[]) {
    int index = hashFunction(user_id);
    User *new_user = (User *)malloc(sizeof(User));
    new_user->user_id = user_id;
    strcpy(new_user->name, name);
    new_user->next = table[index];
    table[index] = new_user;
}

User *getUser(User *table[], int user_id) {
    User *current = table[hashFunction(user_id)];
    while (current && current->user_id != user_id)
        current = current->next;
    return current;
}

void addProduct(Product *table[], int product_id, char name[], int stock) {
    int index = hashFunction(product_id);
    Product *new_product = (Product *)malloc(sizeof(Product));
    new_product->product_id = product_id;
    strcpy(new_product->name, name);
    new_product->stock = stock;
    new_product->next = table[index];
    table[index] = new_product;
}

void addInteraction(InteractionGraph *graph, int user_id, int product_id) {
    int index = hashFunction(user_id);
    GraphNode *new_node = (GraphNode *)malloc(sizeof(GraphNode));
    new_node->id = product_id;
    new_node->next = graph->adj_list[index];
    graph->adj_list[index] = new_node;
}

void displayHistory(InteractionGraph *graph, int user_id) {
    GraphNode *current = graph->adj_list[hashFunction(user_id)];
    printf("History for User ID %d:\n", user_id);
    while (current) {
        printf("- Product ID: %d\n", current->id);
        current = current->next;
    }
}

void recommendProducts(User *user_table[], Product *product_table[], InteractionGraph *graph, int user_id) {
    User *user = getUser(user_table, user_id);
    if (!user) {
        printf("User not found.\n");
        return;
    }

    int interacted[TABLE_SIZE] = {0}, i = 0, index = hashFunction(user_id);
    GraphNode *current = graph->adj_list[index];
    while (current) {
        interacted[i++] = current->id;
        current = current->next;
    }

    printf("Recommendations for %s (User ID: %d):\n", user->name, user_id);
    int found = 0;
    for (i = 0; i < TABLE_SIZE; i++) {
        Product *product = product_table[i];
        while (product) {
            int skip = 0;
            for (int j = 0; j < TABLE_SIZE && interacted[j]; j++)
                if (interacted[j] == product->product_id) {
                    skip = 1;
                    break;
                }
            if (!skip) {
                printf("- %s (ID: %d, Stock: %d)\n", product->name, product->product_id, product->stock);
                found = 1;
            }
            product = product->next;
        }
    }
    if (!found)
        printf("No new recommendations available.\n");
}

int main() {
    User *user_table[TABLE_SIZE] = {0};
    Product *product_table[TABLE_SIZE] = {0};
    InteractionGraph graph = {0};
    int num_users, num_products, choice;

    printf("Enter the number of users: ");
    scanf("%d", &num_users);
    for (int i = 0; i < num_users; i++) {
        int user_id;
        char name[50];
        printf("Enter User ID and Name: ");
        scanf("%d %s", &user_id, name);
        addUser(user_table, user_id, name);
    }

    printf("Enter the number of products: ");
    scanf("%d", &num_products);
    for (int i = 0; i < num_products; i++) {
        int product_id, stock;
        char name[50];
        printf("Enter Product ID, Name, and Stock: ");
        scanf("%d %s %d", &product_id, name, &stock);
        addProduct(product_table, product_id, name, stock);
    }

    do {
        printf("\n1. Record Interaction\n2. View History\n3. Get Recommendations\n4. Exit\nEnter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: {
                int user_id, product_id;
                printf("Enter User ID and Product ID: ");
                scanf("%d %d", &user_id, &product_id);
                addInteraction(&graph, user_id, product_id);
                printf("Interaction recorded for User ID %d and Product ID %d.\n", user_id, product_id);
                break;
            }
            case 2: {
                int user_id;
                printf("Enter User ID: ");
                scanf("%d", &user_id);
                displayHistory(&graph, user_id);
                break;
            }
            case 3: {
                int user_id;
                printf("Enter User ID: ");
                scanf("%d", &user_id);
                recommendProducts(user_table, product_table, &graph, user_id);
                break;
            }
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 4);

    return 0;
}
