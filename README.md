# AutoPad

AutoPad is a multi-document text editor application with an automated text correction feature. It is developed using the Qt framework for C++.

## Features

- Create, open, save, and print text documents
- Undo/Redo functionality
- Word wrap, font, and color formatting options
- Automated text correction using dictionary-based string distance algorithm
- Recent file list for quickly opening previously edited documents
- Tabbed interface for managing multiple documents

## Requirements

- Qt 5.x

## Building

1. Clone the repository
3. Create a build directory: `mkdir build`
4. Enter the build directory: `cd build`
5. Run CMake to configure the project: `cmake ..`
6. Build the project: `make` 

## Usage

Upon launching AutoPad, you can create a new document or open an existing text file. The application provides a user-friendly interface with menus and toolbars for common actions such as saving, printing, undoing/redoing changes, and formatting text.

The automated text correction feature suggests similar words from a dictionary when you right-click on a misspelled word. This can help you quickly correct typographical errors while writing.

## Contributing

This project was developed as part of a university course and is not actively maintained. However, if you would like to contribute improvements or bug fixes, you can submit a pull request on the project's GitHub repository.

## Credits

Autopad was developed by the class of Programming Techniques 2020 under the supervision of Prof. Alessandro Bria at the University of Cassino and Southern Lazio.
