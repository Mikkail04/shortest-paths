/*******************************************************************************
 * Name        : shortestpaths.cpp
 * Author      : Mikkail Allen
 * Version     : 
 * Date        : 12/8/23
 * Description : Implementation of Floyd's Algorithm 
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <limits>

using namespace std;
const long INF = numeric_limits<long>::max();

int getDigitCount(long num) {
    if (num == 0)
        return 1;
    int count = 0;
    while (num > 0) {
        num /= 10;
        count++;
    }
    return count;
}

void display_table(vector<vector<long>> &matrix, const string &label, const bool useLetters = false) {
    cout << label << endl;
    long maxVal = 0;
    for (size_t i = 0; i < matrix.size(); i++) {
        for (size_t j = 0; j < matrix.size(); j++) {
            long cell = matrix[i][j];
            if (cell < INF && cell > maxVal) {
                maxVal = matrix[i][j];
            }
        }
    }

    int maxCellWidth = useLetters ? getDigitCount(maxVal) : getDigitCount(max(static_cast<long>(matrix.size()), maxVal));
    cout << ' ';
    for (size_t j = 0; j < matrix.size(); j++) {
        cout << setw(maxCellWidth + 1) << static_cast<char>(j + 'A');
    }
    cout << endl;
    for (size_t i = 0; i < matrix.size(); i++) {
        cout << static_cast<char>(i + 'A');
        for (size_t j = 0; j < matrix[0].size(); j++) {
            cout << " " << setw(maxCellWidth);
            if (matrix[i][j] == INF) {
                cout << "-";
            } else if (useLetters) {
                cout << static_cast<char>(matrix[i][j] + 'A');
            } else {
                cout << matrix[i][j];
            }
        }
        cout << endl;
    }
    cout << endl;
}

void printPath(const vector<vector<long>> &matrix, size_t matrixSize, size_t i, size_t j) {
    if (i >= matrixSize || j >= matrixSize) {
        cerr << "Error: Invalid indices in printPath function. i=" << i << ", j=" << j << endl;
        return;
    }

    if (matrix[i][j] == INF) {
        cout << (char)(i + 'A') << " -> " << (char)(j + 'A');
        return;
    } else {
        long intermediate = matrix[i][j];
        if (intermediate != static_cast<long>(i)) {
            printPath(matrix, matrixSize, i, intermediate);
            cout << " -> ";
        }
        cout << (char)(j + 'A');
    }
}

int main(int argc, const char *argv[]) {
    vector<vector<long>> distanceMatrix;

    // Validate command line arguments
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }

    // Open the input file
    ifstream inputFile(argv[1]);
    if (!inputFile) {
        cerr << "Error: Cannot open file '" << argv[1] << "'." << endl;
        return 1;
    }

    // Enable exceptions for file handling
    inputFile.exceptions(ifstream::badbit);
    string line;

    try {
    unsigned int lineNumber = 1;
    int vertexCount;

    // Read input file line by line
    while (getline(inputFile, line)) {
        if (lineNumber == 1) {
            // Process the first line to determine the number of vertices
            try {
                vertexCount = stoi(line);

                if (vertexCount < 1 || vertexCount > 26) {
                    cerr << "Error: Invalid number of vertices '" << line << "' on line 1." << endl;
                    return 1;
                }
            } catch (exception &err) {
                cerr << "Error: Invalid number of vertices '" << line << "' on line 1." << endl;
                return 1;
            }

            // Resize the distance matrix and initialize diagonal elements to 0
            distanceMatrix.resize(vertexCount, vector<long>(vertexCount, INF));
            for (int i = 0; i < vertexCount; i++) {
                distanceMatrix[i][i] = 0;
            }
        } else {
            // Process edges and weights
            stringstream ss(line);
            string s;
            int elementCount = 0;
            int firstVertex, secondVertex, weight;

            while (getline(ss, s, ' ')) {
                // Validate starting vertex
                if (elementCount == 0 && (s.length() != 1 || s[0] < 'A' || s[0] > char('A' + vertexCount - 1))) {
                    cerr << "Error: Starting vertex '" << s << "' on line " << lineNumber
                         << " is not among valid values A-" << (char)('A' + vertexCount - 1) << "." << endl;
                    return 1;
                }

                // Validate ending vertex
                if (elementCount == 1 && (s.length() != 1 || s[0] < 'A' || s[0] > char('A' + vertexCount - 1))) {
                    cerr << "Error: Ending vertex '" << s << "' on line " << lineNumber
                         << " is not among valid values A-" << (char)('A' + vertexCount - 1) << "." << endl;
                    return 1;
                }

                // Validate edge weight
                if (elementCount == 2) {
                    try {
                        weight = stoi(s);
                        if (weight < 1) {
                            throw invalid_argument("Invalid edge weight");
                        }
                    } catch (const invalid_argument& e) {
                        cerr << "Error: Invalid edge weight '" << s << "' on line " << lineNumber << "." << endl;
                        return 1;
                    }
                }

                // Assign values to variables based on component count
                if (elementCount == 0) {
                    firstVertex = s[0] - 'A';
                } else if (elementCount == 1) {
                    secondVertex = s[0] - 'A';
                }

                elementCount++;
            }

            // Validate the number of components in each line
            if (elementCount != 3) {
                cerr << "Error: Invalid edge data '" << line << "' on line " << lineNumber << "." << endl;
                return 1;
            }

            // Assign edge weight to the distance matrix
            distanceMatrix[firstVertex][secondVertex] = weight;
        }

        ++lineNumber;
    }

    if (vertexCount == 1) {
        cout << "Distance matrix:" << endl;
        cout << "  A" << endl;
        cout << "A 0" << endl;
        cout << "\nPath lengths:" << endl;
        cout << "  A" << endl;
        cout << "A 0" << endl;
        cout << "\nIntermediate vertices:" << endl;
        cout << "  A" << endl;
        cout << "A -" << endl;
        cout << "\nA -> A, distance: 0, path: A" << endl;
        // Close the input file
        inputFile.close();
        return 0;
    }
    // Process the distance matrix
for (int k = 0; k < vertexCount; k++) {
    for (int i = 0; i < vertexCount; i++) {
        for (int j = 0; j < vertexCount; j++) {
            if (distanceMatrix[i][k] != INF && distanceMatrix[k][j] != INF) {
                long sum = distanceMatrix[i][k] + distanceMatrix[k][j];
                if (sum < distanceMatrix[i][j]) {
                    distanceMatrix[i][j] = sum;
                }
            }
        }
    }
}

// Output the results
for (int i = 0; i < vertexCount; ++i) {
    for (int j = 0; j < vertexCount; ++j) {
        char origVert = 'A' + i;
        char destVert = 'A' + j;
        long distance = distanceMatrix[i][j];

        if (distance != INF) {
            cout << origVert << " -> " << destVert << ", distance: " << distance << ", path: ";
            if (i != j) {
                printPath(distanceMatrix, vertexCount, i, j);
            } else {
                cout << origVert;
            }
            cout << endl;
        } else {
            cout << origVert << " -> " << destVert << ", distance: infinity, path: none" << endl;
        }
    }
}

} catch (const ifstream::failure &f) {
    cerr << "Error: An I/O error occurred reading '" << argv[1] << "'.";
    return 1;
}

return 0;
}
