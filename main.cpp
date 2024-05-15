#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "Graph.h"

Graph processInputFile(const std::string& fileName);
void processLine(const std::vector<std::string>& lines, int lineNumber, Graph& graph, std::vector<int>& lastAssignment);
void processWhileLoop(const std::vector<std::string>& lines, int whileLine, Graph& graph, std::vector<int>& lastAssignment);
int findLoopEnd(const std::vector<std::string>& lines, int loopStart);
void updateLastAssignment(const std::string& variable, int lineNumber, std::vector<int>& lastAssignment);
void addEdgesFromTokens(const std::vector<std::string>& tokens, int lineNumber, Graph& graph, const std::vector<int>& lastAssignment);
void writeOutputFile(const Graph& graph, const std::string& fileName);

int main() {
    std::string inputFileName = "untitled3/inputx.txt";
    std::string outputFileName = "src/outputx.txt";

    try {
        Graph graph = processInputFile(inputFileName);
        writeOutputFile(graph, outputFileName);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

Graph processInputFile(const std::string& fileName) {
    std::ifstream file(fileName);
    std::vector<std::string> lines;
    std::string line;

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + fileName);
    }

    while (getline(file, line)) {
        if (line.find('=') != std::string::npos || line.find("while") != std::string::npos) {
            lines.push_back(line);
        }
    }
    file.close();

    Graph graph(lines.size());
    std::vector<int> lastAssignment(26, -1);  // Track the last assignment line of variables a-z

    for (int i = 0; i < lines.size(); i++) {
        processLine(lines, i, graph, lastAssignment);
    }

    return graph;
}

void processLine(const std::vector<std::string>& lines, int lineNumber, Graph& graph, std::vector<int>& lastAssignment) {
    std::string line = lines[lineNumber];
    if (line.empty()) return;

    if (line.find("while") != std::string::npos) {
        processWhileLoop(lines, lineNumber, graph, lastAssignment);
        return;
    }

    auto pos = line.find('=');
    if (pos == std::string::npos) return;
    std::string variable = line.substr(0, pos);
    std::string expression = line.substr(pos + 1);

    std::istringstream tokenStream(expression);
    std::vector<std::string> tokens;
    std::string token;
    while (tokenStream >> token) {
        tokens.push_back(token);
    }

    updateLastAssignment(variable, lineNumber, lastAssignment);
    addEdgesFromTokens(tokens, lineNumber, graph, lastAssignment);
}

void processWhileLoop(const std::vector<std::string>& lines, int whileLine, Graph& graph, std::vector<int>& lastAssignment) {
    // Extract while condition and parse loop bounds
    std::string whileCondition = lines[whileLine].substr(5);
    auto colonPos = whileCondition.find(':');
    std::string condition = whileCondition.substr(0, colonPos);

    std::istringstream conditionStream(condition);
    std::string part;
    while (conditionStream >> part) {
        if (isalpha(part[0]) && part.length() == 1) {
            int index = part[0] - 'a';
            if (lastAssignment[index] != -1) {
                graph.addEdge(lastAssignment[index], whileLine);
            }
        }
    }

    int loopStart = whileLine + 1;
    int loopEnd = findLoopEnd(lines, loopStart);
    for (int i = loopStart; i <= loopEnd; i++) {
        processLine(lines, i, graph, lastAssignment);
    }
}

int findLoopEnd(const std::vector<std::string>& lines, int loopStart) {
    int i = loopStart;
    while (i < lines.size() && !lines[i].empty() && lines[i].find("while") == std::string::npos) {
        i++;
    }
    return i - 1;
}

void updateLastAssignment(const std::string& variable, int lineNumber, std::vector<int>& lastAssignment) {
    if (variable.length() == 1 && isalpha(variable[0])) {
        int index = variable[0] - 'a';
        lastAssignment[index] = lineNumber;
    }
}

void addEdgesFromTokens(const std::vector<std::string>& tokens, int lineNumber, Graph& graph, const std::vector<int>& lastAssignment) {
    for (const std::string& token : tokens) {
        if (isalpha(token[0]) && token.length() == 1) {
            int index = token[0] - 'a';
            if (lastAssignment[index] != -1 && lastAssignment[index] != lineNumber) {
                graph.addEdge(lastAssignment[index], lineNumber);
            }
        }
    }
}

void writeOutputFile(const Graph& graph, const std::string& fileName) {
    std::ofstream file(fileName);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + fileName);
    }

    int totalEdges = 0;
    for (int i = 0; i < graph.vertexCount; i++) {
        Edge* current = graph.edges[i].head;
        while (current != nullptr) {
            totalEdges++;
            current = current->next;
        }
    }

    file << graph.vertexCount << "\n";
    file << totalEdges << "\n";

    for (int i = 0; i < graph.vertexCount; i++) {
        for (Edge* e = graph.edges[i].head; e != nullptr; e = e->next) {
            file << e->from << " " << e->to << "\n";
        }
    }
    file.close();
}
