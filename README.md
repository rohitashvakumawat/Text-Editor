# Simple Text Editor in C

This repository contains a simple text editor implemented in C. The editor supports basic text operations like insert, delete, display, save, undo, redo, copy, cut, paste, find and replace, and edit. The editor reads from and writes to a file and uses a linked list to manage the file's content.

## Features

- Insert text at a specified position
- Delete text at a specified position
- Display the content of the file
- Save the content to a file
- Undo and redo the last operations
- Copy and cut text between specified lines
- Paste text at a specified position
- Find and replace text within the file
- Edit a specific line in the file

## Data Structures

### Node
The `Node` structure represents a line of text in the linked list.

```c
typedef struct Node {
    char data[MAX_LINE_LENGTH];
    struct Node *next;
} Node;
```

## LinkedList
The LinkedList structure manages the nodes in the list and keeps track of the head, tail, and size of the list.

```c
typedef struct {
    Node *head;
    Node *tail;
    int size;
} LinkedList;
```

## Functions

# Linked List Operations

- initializeLinkedList: Initializes an empty linked list.
= insertNode: Inserts a new node at the specified position.
- deleteNode: Deletes a node at the specified position.
- displayFile: Displays the content of the linked list.
- saveToFile: Saves the content of the linked list to a file.
- loadFromFile: Loads content from a file into the linked list.
- clearList: Clears all nodes in the linked list.
  
# Editing Operations

- undo: Undoes the last operation and moves it to the undone history.
- redo: Redoes the last undone operation and moves it back to the history.
- copy: Copies text between specified lines into a buffer.
- cut: Cuts text between specified lines into a buffer and deletes it from the list.
- paste: Pastes text from a buffer at a specified position.
- findAndReplace: Finds and replaces occurrences of a string in the list.
- edit: Edits a specific line in the list.

# Usage

-Clone the repository.
-Compile the main.c file.
- Run the executable and follow the menu prompts to edit a text file.

```sh
git clone https://github.com/yourusername/simple-text-editor-in-c.git
cd simple-text-editor-in-c
gcc main.c -o texteditor
./texteditor
```

## Contributing
Feel free to submit issues and pull requests for improvements and additional features.
