// TODO File block comment

// TODO If you'd like to include any other header files, do so right below
// this comment


/* BEGINNING OF TEMPLATE CODE; DO NOT MODIFY */

// By Dylan Fox
// This code asks a user to input a word, uses outside code to then define, link a url pronounciation, formal prounounce, exit, or use word in a sentence (extra)


#include <iostream>
#include <string>
#include <sstream>

// Include the required header files
#include "json11/json11.hpp"
#include "cpp-httplib/httplib.h"

//connects Json11 to code
using namespace std;
using json11::Json;

// Define constants for API endpoint and path prefix
const string DICTIONARY_API_HOST = "https://api.dictionaryapi.dev";
const string DICTIONARY_API_PATH_PREFIX = "/api/v2/entries/en/";

// Function to build request endpoint
string build_request_endpoint(const string& word) {
    stringstream endpoint_ss;
    endpoint_ss << DICTIONARY_API_PATH_PREFIX << word;
    return endpoint_ss.str();
}

// Function to query definition
string query_definition(const string& word) {
    httplib::Client cli(DICTIONARY_API_HOST);
    auto res = cli.Get(build_request_endpoint(word));
    if (res && res->status == 200) {
        string err;
        const Json words = Json::parse(res->body, err);
        if (err.empty()) {
            const auto& meanings = words.array_items();
            if (!meanings.empty()) {
                const auto& definitions = meanings[0]["meanings"].array_items();
                if (!definitions.empty()) {
                    return definitions[0]["definition"].string_value();
                }
            }
        }
    }
    return "";
}

// Function to query audio URL
string query_audio_url(const string& word) {
    httplib::Client cli(DICTIONARY_API_HOST);
    auto res = cli.Get(build_request_endpoint(word));
    if (res && res->status == 200) {
        string err;
        const Json words = Json::parse(res->body, err);
        if (err.empty()) {
            const auto& phonetics = words.array_items();
            if (!phonetics.empty()) {
                return phonetics[0]["phonetics"][0]["audio"].string_value();
            }
        }
    }
    return "";
}

// Function to query pronunciation
string query_pronunciation(const string& word) {
    httplib::Client cli(DICTIONARY_API_HOST);
    auto res = cli.Get(build_request_endpoint(word));
    if (res && res->status == 200) {
        string err;
        const Json words = Json::parse(res->body, err);
        if (err.empty()) {
            const auto& phonetics = words.array_items();
            if (!phonetics.empty()) {
                return phonetics[0]["phonetic"].string_value();
            }
        }
    }
    return "";
}

// function to query example sentence
string query_example_sentence(const Json words) {
    // For each word
    const Json::array& word_arr = words.array_items();
    for (const Json& word : word_arr) {
        // Get examples
        const Json examples = word["examples"];

        // For each example entry
        const Json::array& example_arr = examples.array_items();
        for (const Json& example : example_arr) {
            // Get example sentence
            const string example_sentence = example["text"].string_value();

            if (!example_sentence.empty()) {
                // If example sentence is not empty, return it.
                return example_sentence;
            }

            // Otherwise, keep looking for a non-empty example sentence.
        }
    }

    // Failed to find example sentence in response. Return empty string.
    return "";
}

int main() {
    char choice;
    string word;
    
//prompts user for code
    
    do {
        cout << "Select an option:" << endl;
        cout << "(a) Lookup the definition of a given word" << endl;
        cout << "(b) Lookup a URL to an audio sample of a given word being spoken" << endl;
        cout << "(c) Lookup the formal pronunciation of a given word" << endl;
        cout << "(d) Exit the program" << endl;
        cout << "Your choice: ";
        cin >> choice;

//does the functions depending on the user input
        
        switch (choice) {
            
//gives word's definition
            
            case 'a':
                cout << "Enter the word to lookup: ";
                cin >> word;
                {
                    string definition = query_definition(word);
                    if (!definition.empty()) {
                        cout << "Definition: " << definition << endl;
                    } else {
                        cout << "Definition not found." << endl;
                    }
                }
                break;

//gives word's url audio
            
            case 'b':
                cout << "Enter the word to lookup: ";
                cin >> word;
                {
                    string audio_url = query_audio_url(word);
                    if (!audio_url.empty()) {
                        cout << "Audio URL: " << audio_url << endl;
                    } else {
                        cout << "Audio URL not found." << endl;
                    }
                }
                break;

//gives word's search result on google
            
            case 'c':
                cout << "Enter the word to lookup: ";
                cin >> word;
                {
                    string pronunciation = query_pronunciation(word);
                    if (!pronunciation.empty()) {
                        cout << "Pronunciation: " << pronunciation << endl;
                    } else {
                        cout << "Pronunciation not found." << endl;
                    }
                }
                break;

//Leaves program
            
            case 'd':
                cout << "Exiting program..." << endl;
                return 0;

//gives word example sentence
            
            case 'e':
                cout << "Enter the word to use in example sentence: ";
                cin >> word;
                {
                    string example_sentence = query_example_sentence(word);
                    if (!example_sentence.empty()) {
                        cout << "Example Sentence with word: " << example_sentence << endl;
                    } else {
                        cout << "Example sentence not found." << endl;
                    }
                }
                break;

//loops user with the 5 options unless d is chosen to exit
            
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 'd');

    return 0;
}
