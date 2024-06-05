#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILENAME_LENGTH 1000
#define MAX_LINE_LENGTH 1000

typedef struct Node
{
    char data[MAX_LINE_LENGTH];
    struct Node *next;
} Node;

typedef struct
{
    Node *head;
    Node *tail;
    int size;
} LinkedList;

void initializeLinkedList(LinkedList *list);
void insertNode(LinkedList *list, const char *data, int position);
void deleteNode(LinkedList *list, int position);
void displayFile(LinkedList *list);
void saveToFile(LinkedList *list, const char *filename);
void loadFromFile(LinkedList *list, const char *filename);
void clearList(LinkedList *list);
void undo(LinkedList *list, LinkedList *history, LinkedList *undoneHistory);
void redo(LinkedList *list, LinkedList *history, LinkedList *undoneHistory);
void copy(LinkedList *list, int start, int end, char *buffer);
void cut(LinkedList *list, int start, int end, char *buffer);
void paste(LinkedList *list, int position, const char *buffer);
int findAndReplace(LinkedList *list, const char *find, const char *replace);
void edit(LinkedList *list, int line);

int main()
{
    char filename[MAX_FILENAME_LENGTH];
    LinkedList fileContent;
    LinkedList history;
    LinkedList undoneHistory; // New linked list for undone operations
    initializeLinkedList(&fileContent);
    initializeLinkedList(&history);
    initializeLinkedList(&undoneHistory); // Initialize undoneHistory

    printf("Welcome to Simple Text Editor in C!\n");
    printf("Enter filename to open or create: ");
    scanf("%s", filename);

    loadFromFile(&fileContent, filename);

    int choice;
    do
    {
        printf("\nMenu:\n");
        printf("1. Insert text\n");
        printf("2. Delete text\n");
        printf("3. Display file content\n");
        printf("4. Save file\n");
        printf("5. Undo\n");
        printf("6. Redo\n");
        printf("7. Copy\n");
        printf("8. Cut\n");
        printf("9. Paste\n");
        printf("10. Find and Replace\n");
        printf("11. Edit\n");
        printf("12. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
        {
            char text[MAX_LINE_LENGTH];
            int position;
            printf("Enter position to insert: ");
            scanf("%d", &position);
            printf("Enter text to insert: ");
            getchar();
            fgets(text, MAX_LINE_LENGTH, stdin);
            insertNode(&fileContent, text, position);
            break;
        }
        case 2:
        {
            int position;
            printf("Enter line number to delete: ");
            scanf("%d", &position);
            deleteNode(&fileContent, position);
            break;
        }
        case 3:
            displayFile(&fileContent);
            break;
        case 4:
            saveToFile(&fileContent, filename);
            printf("File saved successfully!\n");
            break;
        case 5:
            undo(&fileContent, &history, &undoneHistory);
            break;
        case 6:
            redo(&fileContent, &history, &undoneHistory);
            break;
        case 7:
        {
            int start, end;
            printf("Enter start line number to copy: ");
            scanf("%d", &start);
            printf("Enter end line number to copy: ");
            scanf("%d", &end);
            char buffer[MAX_LINE_LENGTH];
            copy(&fileContent, start, end, buffer);
            break;
        }
        case 8:
        {
            int start, end;
            printf("Enter start line number to cut: ");
            scanf("%d", &start);
            printf("Enter end line number to cut: ");
            scanf("%d", &end);
            char buffer[MAX_LINE_LENGTH];
            cut(&fileContent, start, end, buffer);
            break;
        }
        case 9:
        {
            int position;
            printf("Enter position to paste: ");
            scanf("%d", &position);
            char buffer[MAX_LINE_LENGTH];
            paste(&fileContent, position, buffer);
            break;
        }
        case 10:
        {
            char find[MAX_LINE_LENGTH];
            char replace[MAX_LINE_LENGTH];
            printf("Enter string to find: ");
            getchar();
            fgets(find, MAX_LINE_LENGTH, stdin);
            printf("Enter string to replace with: ");
            fgets(replace, MAX_LINE_LENGTH, stdin);
            int count = findAndReplace(&fileContent, find, replace);
            printf("Replaced %d occurrences.\n", count);
            break;
        }
        case 11:
        {
            int line;
            printf("Enter line number to edit: ");
            scanf("%d", &line);
            edit(&fileContent, line);
            break;
        }
        case 12:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice!\n");
        }
    } while (choice != 12);

    clearList(&fileContent);
    clearList(&history);
    clearList(&undoneHistory); // Clear undoneHistory
    return 0;
}

void initializeLinkedList(LinkedList *list)
{
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void insertNode(LinkedList *list, const char *data, int position)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        printf("Memory allocation failed!\n");
        return;
    }
    strcpy(newNode->data, data);
    newNode->next = NULL;

    if (list->head == NULL)
    {
        list->head = newNode;
        list->tail = newNode;
    }
    else
    {
        if (position <= 1)
        {
            newNode->next = list->head;
            list->head = newNode;
        }
        else if (position >= list->size)
        {
            list->tail->next = newNode;
            list->tail = newNode;
        }
        else
        {
            Node *temp = list->head;
            for (int i = 1; i < position - 1; i++)
            {
                temp = temp->next;
            }
            newNode->next = temp->next;
            temp->next = newNode;
        }
    }
    list->size++;
}

void deleteNode(LinkedList *list, int position)
{
    if (list->head == NULL)
    {
        printf("List is empty. Nothing to delete.\n");
        return;
    }

    if (position < 1 || position > list->size)
    {
        printf("Invalid position!\n");
        return;
    }

    Node *temp = list->head;
    if (position == 1)
    {
        list->head = list->head->next;
        free(temp);
    }
    else
    {
        Node *prev = NULL;
        for (int i = 1; i < position; i++)
        {
            prev = temp;
            temp = temp->next;
        }
        prev->next = temp->next;
        free(temp);
    }
    list->size--;
}

void displayFile(LinkedList *list)
{
    printf("....................................");
    printf("\n\tPage 1\t\n");
    Node *current = list->head;
    int lineNumber = 1;
    while (current != NULL)
    {
        printf("%d. %s\n", lineNumber, current->data);
        current = current->next;
        lineNumber++;
    }
    printf("....................................");
}

void saveToFile(LinkedList *list, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    Node *current = list->head;
    while (current != NULL)
    {
        fprintf(file, "%s", current->data);
        current = current->next;
    }

    fclose(file);
}

void loadFromFile(LinkedList *list, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("File not found or could not be opened. Creating a new file.\n");
        return;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL)
    {
        insertNode(list, line, list->size + 1);
    }

    fclose(file);
}

void clearList(LinkedList *list)
{
    while (list->head != NULL)
    {
        Node *temp = list->head;
        list->head = list->head->next;
        free(temp);
    }
    list->tail = NULL;
    list->size = 0;
}

void undo(LinkedList *list, LinkedList *history, LinkedList *undoneHistory)
{
    if (history->size == 0)
    {
        printf("No more undo operations available.\n");
        return;
    }

    Node *lastOperation = history->tail;

    switch (lastOperation->data[0])
    {
    case 'I':
        deleteNode(list, list->size);
        break;
    case 'D':
        insertNode(list, lastOperation->data + 1, atoi(lastOperation->data + 2));
        break;
    }

    // Move the last operation from history to undoneHistory
    if (undoneHistory->head == NULL)
    {
        undoneHistory->head = lastOperation;
        undoneHistory->tail = lastOperation;
    }
    else
    {
        undoneHistory->tail->next = lastOperation;
        undoneHistory->tail = lastOperation;
    }
    undoneHistory->size++;

    // Remove the last operation from history
    Node *prev = NULL;
    Node *temp = history->head;
    while (temp != NULL)
    {
        if (temp == lastOperation)
        {
            if (prev != NULL)
            {
                prev->next = temp->next;
            }
            else
            {
                history->head = temp->next;
            }
            if (temp == history->tail)
            {
                history->tail = prev;
            }
            free(temp);
            history->size--;
            break;
        }
        prev = temp;
        temp = temp->next;
    }
}

void redo(LinkedList *list, LinkedList *history, LinkedList *undoneHistory)
{
    if (undoneHistory->size == 0)
    {
        printf("No more redo operations available.\n");
        return;
    }

    Node *lastUndoneOperation = undoneHistory->tail;

    switch (lastUndoneOperation->data[0])
    {
    case 'I':
        insertNode(list, lastUndoneOperation->data + 1, atoi(lastUndoneOperation->data + 2));
        break;
    case 'D':
        deleteNode(list, atoi(lastUndoneOperation->data + 2));
        break;
    }

    // Move the last undone operation from undoneHistory back to history
    if (history->head == NULL)
    {
        history->head = lastUndoneOperation;
        history->tail = lastUndoneOperation;
    }
    else
    {
        history->tail->next = lastUndoneOperation;
        history->tail = lastUndoneOperation;
    }
    history->size++;

    // Remove the last undone operation from undoneHistory
    Node *prev = NULL;
    Node *temp = undoneHistory->head;
    while (temp != NULL)
    {
        if (temp == lastUndoneOperation)
        {
            if (prev != NULL)
            {
                prev->next = temp->next;
            }
            else
            {
                undoneHistory->head = temp->next;
            }
            if (temp == undoneHistory->tail)
            {
                undoneHistory->tail = prev;
            }
            free(temp);
            undoneHistory->size--;
            break;
        }
        prev = temp;
        temp = temp->next;
    }
}

void copy(LinkedList *list, int start, int end, char *buffer)
{
    Node *current = list->head;
    int lineNumber = 1;
    buffer[0] = '\0';
    while (current != NULL && lineNumber <= end)
    {
        if (lineNumber >= start)
        {
            strcat(buffer, current->data);
        }
        current = current->next;
        lineNumber++;
    }
    printf("Copied:\n%s\n", buffer);
}

void cut(LinkedList *list, int start, int end, char *buffer)
{
    copy(list, start, end, buffer);
    for (int i = start; i <= end; i++)
    {
        deleteNode(list, start);
    }
}

void paste(LinkedList *list, int position, const char *buffer)
{
    insertNode(list, buffer, position);
}

int findAndReplace(LinkedList *list, const char *find, const char *replace)
{
    Node *current = list->head;
    int count = 0;
    while (current != NULL)
    {
        char *occurrence = strstr(current->data, find);
        while (occurrence != NULL)
        {
            count++;
            int pos = occurrence - current->data;
            for (int i = 0; i < strlen(replace); i++)
            {
                current->data[pos + i] = replace[i];
            }
            occurrence = strstr(current->data + pos + strlen(replace), find);
        }
        current = current->next;
    }
    return count;
}

void edit(LinkedList *list, int line)
{
    char text[MAX_LINE_LENGTH];
    printf("Enter new text: ");
    getchar();
    fgets(text, MAX_LINE_LENGTH, stdin);
    deleteNode(list, line);
    insertNode(list, text, line);
}
