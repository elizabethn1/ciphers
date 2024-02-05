/*********************************************************************
 * This program allows users to encrypt with a Caesar Cipher, decrypt
 * a Caesar Cipher, and/or encrypt with a Vigenere Cipher.
 * Class: CS 251, Spring 2024
 * System: VSCode
 * @author Elizabeth Ng
 * @version January 31, 2024
 ********************************************************************/
#include <iostream>
#include <cctype>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

const string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

// Function prototypes
void printMenu();
int findIndexInAlphabet(char c);
char rot(char c, int amount);
string rot(string line, int amount);
string makeLineUpper(string line);
void encryptCaesar();
void vigenere();
string removeNonLetters(string line);
void breakText(string line, vector <string> &list);
void decryptCaesar(vector <string> &dictionary);

int main() {
    string command;

    cout << "Welcome to Ciphers!" << endl;
    cout << "-------------------" << endl;
    cout << endl;

    do {
        printMenu();
        cout << endl;
        cout << "Enter a command (case does not matter): ";

        getline(cin, command);
        cout << endl;

        // User wants to encrypt with Caesar Cipher.
        if (command == "C" || command == "c") {
            encryptCaesar();
        }

        // User wants to encrypt with Vigènere.
        if (command == "V" || command == "v") {
            vigenere();
        }

        // User wants to decrpt Caesar Cipher.
        if (command == "D" || command == "d") {   
            vector <string> dictionary;
            string tempWord;

            ifstream inFS;

            inFS.open("dictionary.txt");

            if (!inFS.is_open()) {
                cout << "Could not open file dictionary.txt." << endl;
                return 1;
            }

            while (!inFS.eof()) {
                getline(inFS, tempWord);
                dictionary.push_back(tempWord);
            }

            inFS.close();

            decryptCaesar(dictionary);
        }

        cout << endl;

    } while (!(command == "x" || command == "X"));

    return 0;
}

/* 
 * Description: Print instructions for using the program.
 * Input: N/A
 * Output: The return type is void, so the function doesn't return anything. However, it prints instructions.
 */
void printMenu() {
    cout << "Ciphers Menu" << endl;
    cout << "------------" << endl;
    // Caesar Ciphers
    cout << "C - Encrypt with Caesar Cipher" << endl;
    cout << "D - Decrypt Caesar Cipher" << endl;
    // Vigenere
    cout << "V - Encrypt with Vigenère" << endl;
    // Utility
    cout << "X - Exit Program" << endl;
}

/*
 * Description: Returns the 0-based index in the English alphabet where 'c' appears,
 * or -1 if 'c' is not an uppercase letter in the alphabet.
 * Input: c (A character that this function will try to find the index of from the ALPHABET string.)
 * Output: index (The index of the character that's in the ALPHABET string. The index could be -1 if the character isn't found.)
 */
int findIndexInAlphabet(char c) {
    int index = 0;

    // The character is in ALPHABET, so its index can be identified in ALPHABET.
    if (ALPHABET.find(c) != string::npos) {
        index = ALPHABET.find(c);
    }
    // The character isn't in ALPHABET.
    else {
        index = -1;
    }

    return index;
}

/*
 * Description: Returns 'c' rotated by 'amount' many characters forward. If we run out
 * of letters in the alphabet, wrap around back to 'A'. This method 
 * assumes that 'c' is an uppercase letter.
 * Input: c (The character that will be rotated.), amount (The number of forward rotations that will occur.)
 * Output: newLetter (The new character after it was rotated.)
 */
char rot(char c, int amount) {
    int index = findIndexInAlphabet(c);
    int newIndex = index += amount;

    // Wrap back to the beginning of the alphabet.
    if (newIndex > 25) {
        newIndex -= 26;
    }

    char newLetter = ALPHABET.at(newIndex);

    return newLetter;
}

/*
 * Description: Returns a new string in which every character has been rotated by 'amount'
 * letters. Lowercase letters are converted to uppercase and rotated.
 * Non-alphabetic characters are left alone.
 * Input: line (Text that will be rotated.), amount (The number of forward rotations that will occur.)
 * Output: updatedLine (The new text after it was rotated.)
 */
string rot(string line, int amount) {
    string updatedLine = "";
    string tempLine = "";

    // Converts lowercase letters to uppercase.
    for (int i = 0; i < line.size(); ++i) {
        if (isalpha(line.at(i)) != 0) {
            tempLine += toupper(line.at(i));
        }
        else {
            tempLine += line.at(i);
        }
    }

    // Rotates each letter of the line by the amount.
    // Updates the updatedLine string to contain rotated letters and non-alphabetic characters.
    for (int j = 0; j < tempLine.size(); ++j) {
        if (isalpha(tempLine.at(j)) != 0) {
            char tempLetter = rot(tempLine.at(j), amount);
            updatedLine += tempLetter;
        }
        else {
            updatedLine += tempLine.at(j);
        }
    }

    return updatedLine;
}

/*
 * Description: Returns a string that is a version of the input with capital letters.
 * Input: line (Text that will become fully capitalized.)
 * Output: newLine (Text that is fully capitalized.)
 */
string makeLineUpper(string line) {
    string newLine = "";

    // Loop through the line. Any characters that aren't capitalized will be capitalized.
    for (int i = 0; i < line.size(); ++i) {
        if(isalpha(line.at(i)) != 0) {
            newLine += toupper(line.at(i)); // Capitalize the lowercase letter.
        }
        else {
            newLine += line.at(i);
        }
    }

    return newLine;
}

/*
 * Description: Takes a string from the user and the number of characters to rotate by.
 * The result is an encrypted Caesar version of the user's string input. 
 * Input: N/A
 * Output: The return type is void, so the function doesn't return anything. However, it prints the encrypted text.
 */
void encryptCaesar() {
    string text;
    string amount;

    cout << "Enter the text to encrypt:" << endl;

    getline (cin, text);

    cout << "Enter the number of characters to rotate by:" << endl;

    getline(cin, amount);

    cout << endl;

    cout << rot(text, stoi(amount)) << endl; // Rotate the entire text by a certain amount of characters.
}

/*
 * Description: Takes a string that will be used for encryption and a string that is the key, which is for rotation.
 * Each letter of the plaintext is encoded with a different shift, which comes from the key. 
 * The result is an encrypted Vigenere version of the plaintext.
 * Input: N/A
 * Output: The return type is void, so the function doesn't return anything. However, it prints the encrypted text.
 */
void vigenere(){
    string text;
    string key;
    string newText = "";

    vector <int> amountList;

    cout << "Enter text to encrypt:" << endl;

    getline(cin, text);

    cout << "Enter the Vigenère key:" << endl;
    
    getline(cin, key);

    // Capitalizes the letters of the text and the key.
    string upperText = makeLineUpper(text);
    string upperKey = makeLineUpper(key);

    // The index of each letter of the capitalized version of the key is added to a vector.
    for (int i = 0; i < upperKey.size(); ++i) {
        if (isalpha(upperKey.at(i)) != 0) {
            int tempAmount = findIndexInAlphabet(upperKey.at(i));
            amountList.push_back(tempAmount);
        }
    }

    int vectorIndex = 0;

    // Loop through the text to encrypt each character by the key.
    for (int j = 0; j < upperText.size(); ++j) {
        if (isalpha(upperText.at(j)) != 0) {
            if (vectorIndex < amountList.size()) {
                newText += rot(upperText.at(j), amountList.at(vectorIndex)); // Rotates the character.
            }
            // Go back to the beginning of the key.
            else {
                vectorIndex = 0;
                newText += rot(upperText.at(j), amountList.at(vectorIndex)); // Rotates the character.
            }
            vectorIndex += 1;
        }
        else {
            newText += upperText.at(j);
        }
    }

    cout << newText << endl;
}

/*
 * Description: Removes non-letters from the input string and returns the string that only has letters.
 * Input: line (The original text that may contain a mix of letters and non-letters.)
 * Output: newLine (Text with its non-letters removed.)
 */
string removeNonLetters(string line) {
    string newLine = "";

    for (int i = 0; i < line.size(); ++i) {
        if (isalpha(line.at(i)) != 0) { // The character is an alphabet, so add it to the newLine string.
            newLine += line.at(i);
        }
        else if (line.at(i) == ' ') { // The character is a space, so add it to the newLine string.
            newLine += " ";
        }
    }

    return newLine;
}

/*
 * Description: Takes a string and separates individual words. These words will be added to and stored in a vector.
 * Input: line (The text that this function will break into words.), list (A string vector that is initially empty.)
 * Output: The return type is void, so the function doesn't return anything. 
 * However, it updates the list vector to contain words of the line.
 */
void breakText(string line, vector <string> &list) {
    string tempWord = " ";
    int index = 0;
    int spacePosition = 0;

    // Loop through the indices of the line to find individual words.
    while (index < line.size()) {
        if (line.find(' ', index) == string::npos) { // Indicates that the last word has been reached.
            tempWord = line.substr(index);
            index = line.size();
        }
        else {
            spacePosition = line.find(' ', index);
            tempWord = line.substr(index, spacePosition - index);
            index = spacePosition + 1;

            while (line.at(index) == ' ') {
                index += 1;
            }
        }

        list.push_back(tempWord); // Add the identified word to the vector.
    }
}

/*
 * Description: Takes a string to Caesar-cipher decrypt and returns rotated string(s) (or possibly no string)
 * in which more than half of the words in the string are in the dictionary.
 * Input: dictionary (A string vector that contains a list of words that are from dictionary.txt.)
 * Output: The return type is void, so the function doesn't return anything. However, it returns rotated string(s)
 * in which more than half of the words in the string are in the dictionary. It could also return zero strings.
 */
void decryptCaesar(vector <string> &dictionary) {
    vector <string> wordList;  
    string text = "";

    cout << "Enter the text to Caesar-cipher decrypt:" << endl;

    getline(cin, text);

    // An updated version of text that only contains capitalized letters.
    string updatedText = makeLineUpper(removeNonLetters(text)); 

    // Updatedtext is broken into words, which are stored in the wordList vector.
    breakText(updatedText, wordList);

    // Try every possible rotation shift on the vector's elements.
    for (int amount = 0; amount < 26; ++amount) {
        vector <string> rotatedWords;
        double counter = 0.0;

        // Each word in the list is rotated by an amount and is added to a vector that contains rotated words.
        for (int i = 0; i < wordList.size(); ++i) {
            rotatedWords.push_back(rot(wordList.at(i), amount));
        }

        // Compares each rotated word with each line of the dictionary to see if there is a match.
        for (int j = 0; j < rotatedWords.size(); ++j) {
            for (int k = 0; k < dictionary.size(); ++k) {
                if (rotatedWords.at(j) == dictionary.at(k)) {
                    counter += 1.0;
                }
            }
        }

        double halfRotated = rotatedWords.size() / 2.0;

        // Print out the rotated words if more than half of them are in the dictionary.
        if (counter > halfRotated) {
            for (int l = 0; l < rotatedWords.size(); ++l) {
                cout << rotatedWords.at(l) << " ";
            }
            cout << endl;
        }
    }
}