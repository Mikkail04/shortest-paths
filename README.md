# Shortest Paths Finder (Floyd-Warshall Algorithm)

This program reads a weighted directed graph from a file and computes the shortest paths between all pairs of vertices using the **Floyd-Warshall algorithm**. It outputs the distance matrix, path lengths, intermediate vertices, and detailed shortest path routes for every vertex pair.

---

## Features
- Reads graph data from a text file
- Supports **up to 26 vertices** (`A` through `Z`)
- Handles **infinite distances** for unreachable vertices
- Validates input format and reports errors with line numbers
- Displays:
  - Distance matrix
  - Path lengths
  - Intermediate vertices
  - Detailed paths for each vertex pair

---

## Input Format
The input file must follow this structure:
- **`<number_of_vertices>`**: integer between 1 and 26
- **`<start_vertex>`** and **`<end_vertex>`**: single capital letters within the range of the vertices
- **`<weight>`**: positive integer (edge weight)

