#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>  // for memset
#include "src/List/Graph/Graph.h"
#include "src/List/Graph/Edge.h"

using namespace std;
using namespace list;

/**
 * Processes a single line of code to determine dependencies and update the graph accordingly.
 * @param line The line of code to process.
 * @param lineNumber The current line number.
 * @param lastAssignment Array tracking the last line number each variable was assigned.
 * @param graph The graph representing the data flow dependencies.
 */
void processLine(const string& line, int lineNumber, int lastAssignment[], Graph& graph) {
    size_t pos = line.find('=');

    if (pos != string::npos) { // Assignment line
        char lhs = line[0];
        lastAssignment[lhs - 'a'] = lineNumber;

        string rhs = line.substr(pos + 1);
        for (char c : rhs) {
            if (c >= 'a' && c <= 'z') { // Ensure c is a valid variable
                int depIndex = lastAssignment[c - 'a'];
                if (depIndex != -1 && depIndex < lineNumber) { // Valid dependency
                    if (!graph.contains(depIndex + 1, lineNumber + 1)) {
                        graph.addEdge(depIndex + 1, lineNumber + 1);
                    }
                }
            }
        }
    } else if (line.find("if") != string::npos || line.find("while") != string::npos) { // If or While statement
        string condition = line.substr(line.find_first_of("><") - 1, 3); // Extract the condition (e.g., "a < b")
        for (char c : condition) {
            if (c >= 'a' && c <= 'z') { // Ensure c is a valid variable
                int depIndex = lastAssignment[c - 'a'];
                if (depIndex != -1 && depIndex < lineNumber) { // Valid dependency
                    if (!graph.contains(depIndex + 1, lineNumber + 1)) {
                        graph.addEdge(depIndex + 1, lineNumber + 1);
                    }
                }
            }
        }
    }
}

int main() {
    ifstream inputFile("C:\\Users\\SUDE\\CLionProjects\\data-flow-graph-sudebzkrt\\inputa.txt");
    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open input file." << endl;
        return -1;
    }

    int vertexCount;
    inputFile >> vertexCount;
    inputFile.ignore();

    Graph graph(vertexCount);

    string lines[vertexCount];
    int lastAssignment[26];
    memset(lastAssignment, -1, sizeof(lastAssignment));

    string line;
    string whileLines[vertexCount];
    bool insideWhile = false;
    int whileStartLine = -1;
    int whileLineCount = 0;

    /**
     * Reads lines from the input file and processes them to build the data flow graph.
     */
    for (int i = 0; i < vertexCount; ++i) {
        getline(inputFile, line);
        lines[i] = line;

        if (line.find("while") != string::npos) {
            insideWhile = true;
            whileStartLine = i;
            whileLines[whileLineCount++] = line;
        } else if (insideWhile && (line.find_first_not_of(" \t") != string::npos)) {
            whileLines[whileLineCount++] = line;
        } else if (insideWhile && line.find_first_not_of(" \t") == string::npos) {
            insideWhile = false;
            for (int j = 0; j < whileLineCount; ++j) {
                processLine(whileLines[j], whileStartLine, lastAssignment, graph);
            }
            whileLineCount = 0;
        } else {
            processLine(line, i, lastAssignment, graph);
        }
    }
    if (insideWhile) {
        for (int j = 0; j < whileLineCount; ++j) {
            processLine(whileLines[j], whileStartLine, lastAssignment, graph);
        }
    }

    inputFile.close();

    ofstream outputFile("C:\\Users\\SUDE\\CLionProjects\\data-flow-graph-sudebzkrt\\outputa.txt");
    if (!outputFile.is_open()) {
        cerr << "Çıktı dosyası açılamadı." << endl;
        return -1;
    }

    outputFile << vertexCount << endl;
    outputFile << graph.edgeCount() << endl; // Write the number of edges
    for (int i = 0; i < vertexCount; ++i) {
        for (Edge* edge = graph.getEdges()[i].getHead(); edge != nullptr; edge = edge->getNext()) {
            outputFile << edge->getFrom() << " " << edge->getTo() << endl;
        }
    }

    outputFile.close(); // Close the output file
    return 0;
}


