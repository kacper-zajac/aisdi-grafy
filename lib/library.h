//
// Created by kacpi on 23.12.18.
//

#ifndef MOSTY_LIBRARY_H
#define MOSTY_LIBRARY_H

#include <sstream>
#include <string>
#include <iostream>
#include <fstream>

class Graph {
public:
    class Vertex {
    public:
        Vertex *previous;
        Vertex *next;
        int index;       // który to jest
        int value;       // z którym jest powiązanie

        explicit Vertex(int index, int value, Vertex *previous = nullptr, Vertex *next = nullptr)
                : index(index), value(value), previous(previous), next(next) {};


        bool operator==(Vertex *other) {
            return (index == other->index && value == other->value);
        }
    };

    class Iterator {
    public:
        Vertex *current;

        explicit Iterator(Vertex *current = nullptr) : current(current) {}

        Iterator &operator++() {
            current = current->next;
            return *this;
        }

        Iterator &operator--() {
            current = current->previous;
            return *this;
        }

        int operator*() {
            return current->value;
        }

        bool operator==(Iterator other) {
            return current == other.current;
        }

        bool operator!=(Iterator other) {
            return !(*this == other);
        }

    };

    Iterator begin() {
        return Iterator(getFirst());
    }

    Iterator end() {
        return Iterator(nullptr);

    }

private:
    Vertex **graph;
    size_t graph_size;


public:

    ~Graph() {
        if (getFirst() != nullptr) {
            int i = getFirst()->index;
            if (graph[i] != nullptr) {
                Vertex *temp = graph[i];
                while (temp != nullptr) {
                    Vertex *remove = temp;
                    temp = temp->next;
                    delete remove;
                }
            }
            delete[] graph;
        }
    }

    void createGraph() {
        std::string name;
        std::string line;

        std::cout << "Podaj nazwe pliku - ";
        std::cin >> name;

        std::ifstream infile(name);
        std::getline(infile, line);
        std::istringstream iss(line);
        size_t x;
        iss >> x;
        if (!x) {
            std::cout << "Graf jest pusty!" << std::endl;
            return;
        }
        graph_size = x;
        graph = new Vertex *[graph_size]{           // z tymi - problemy liczne są
                nullptr};      // -2 bo size liczy się od 1 nie od 0 (-1) a ostatni element nie ma swojej przegródki
        while (std::getline(infile, line)) {        // spróbować to przed pętlą, jedno wywołane spisującę ilość
            std::istringstream iss2(line);
            int a, b;
            if (!(iss2 >> a >> b)) { break; }
            addVertex(a, b);

        }
    }

    void addVertex(int new_index, int new_value) {
        Vertex *prev = nullptr;
        Vertex *next = nullptr;
        if (graph[new_index] == nullptr) {
            for (int i = new_index - 1; i >= 0; --i) {
                if (graph[i] != nullptr) {
                    prev = graph[i];
                    while (prev->next != nullptr && prev->index == prev->next->index) prev = prev->next;
                    if (prev->next != nullptr) next = prev->next;
                    break;
                }
            }
            if (next == nullptr) {
                for (int j = new_index + 1; j < graph_size; ++j) {
                    if (graph[j] != nullptr) next = graph[j];
                    break;
                }
            }

            auto new_vertex = new Vertex(new_index, new_value, prev, next);
            if (prev != nullptr) prev->next = new_vertex;
            if (next != nullptr) next->previous = new_vertex;
            graph[new_index] = new_vertex;
        } else {
            if (new_value > graph[new_index]->value) {
                prev = graph[new_index];
                while (prev->next != nullptr && prev->index == prev->next->index) {
                    if (prev->next->index > new_value) break;
                    prev = prev->next;

                }
                if (prev->next != nullptr) next = prev->next;
                auto new_vertex = new Vertex(new_index, new_value, prev, next);
                prev->next = new_vertex;
                if (next != nullptr) next->previous = new_vertex;
            } else {
                next = graph[new_index];
                prev = graph[new_index]->previous;
                auto new_vertex = new Vertex(new_index, new_value, prev, next);
                if (graph[new_index]->previous != nullptr) {
                    prev->next = new_vertex;
                }
                graph[new_index]->previous = new_vertex;
                graph[new_index] = new_vertex;
            }
        }
        std::swap(new_index, new_value);
        prev = nullptr;
        next = nullptr;
        if (graph[new_index] == nullptr) {
            for (int i = new_index - 1; i >= 0; --i) {
                if (graph[i] != nullptr) {
                    prev = graph[i];
                    while (prev->next != nullptr && prev->index == prev->next->index) prev = prev->next;
                    if (prev->next != nullptr) next = prev->next;
                    break;
                }
            }
            if (next == nullptr) {
                for (int j = new_index + 1; j < graph_size; ++j) {
                    if (graph[j] != nullptr) next = graph[j];
                    break;
                }
            }

            auto new_vertex = new Vertex(new_index, new_value, prev, next);
            if (prev != nullptr) prev->next = new_vertex;
            if (next != nullptr) next->previous = new_vertex;
            graph[new_index] = new_vertex;
        } else {
            if (new_value > graph[new_index]->value) {
                prev = graph[new_index];
                while (prev->next != nullptr && prev->index == prev->next->index) {
                    if (prev->next->index > new_value) break;
                    prev = prev->next;

                }
                if (prev->next != nullptr) next = prev->next;
                auto new_vertex = new Vertex(new_index, new_value, prev, next);
                prev->next = new_vertex;
                if (next != nullptr) next->previous = new_vertex;
            } else {
                next = graph[new_index];
                prev = graph[new_index]->previous;
                auto new_vertex = new Vertex(new_index, new_value, prev, next);
                if (graph[new_index]->previous != nullptr) {
                    prev->next = new_vertex;
                }
                graph[new_index]->previous = new_vertex;
                graph[new_index] = new_vertex;
            }
        }
    }

    Vertex *getFirst() {
        if (!isEmpty()) {
            Vertex *temp = nullptr;
            for (auto i = 0; i < graph_size - 1; i++) {
                if (graph[i] != nullptr) {
                    temp = graph[i];
                    break;
                }
            }
            return temp;
        }
        return nullptr;
    }

    Vertex *getLast() {
        if (!isEmpty()) {
            Vertex *temp = nullptr;
            for (auto i = graph_size - 1; i >= 0; i--) {
                if (graph[i] != nullptr) {
                    temp = graph[i];
                    while (temp->next != nullptr) temp = temp->next;
                    break;
                }
            }
            return temp;
        }
        return nullptr;
    }

    bool isEmpty() const {
        return (graph_size == 0);
    };

    bool isEveryVertexConnected() {
        if (graph_size == 1) return true;
        for (int i = 0; i < graph_size; i++) {
            if (graph[i] == nullptr) return false;
        }
        return true;
    }

    size_t getSize() const {
        return graph_size;
    }

    void printGraphEdges() {
        Vertex *x = nullptr;
        for (int i = 0; i < graph_size; ++i) {
            if (graph[i] != nullptr) {
                x = graph[i];
                break;
            }
        }
        while (x != nullptr) {
            std::cout << x->index << ":  " << x->value;
            x = x->next;
            while (x != nullptr && x->previous->index == x->index) {
                std::cout << " " << x->value;
                x = x->next;
            }
            std::cout << std::endl;
        }
    }

    void isCoherent() {
        if (!isEveryVertexConnected()) {
            std::cout << "Graf nie jest spojny!" << std::endl;
            return;
        }
        for (auto graphIt = begin(); graphIt != end(); ++graphIt)
            if (graphIt.current->index > graphIt.current->value)
                if (!dftAtVertex(graphIt.current->index, graphIt.current->value))
                    std::cout << "Niespojny dla polaczenia " << graphIt.current->value << " " <<
                              graphIt.current->index << std::endl;

    }

    bool dftAtVertex(int first_vertex, int second_vertex) {
        bool return_value = true;
        bool *visited;
        visited = new bool[graph_size];
        for (int index = 0; index < graph_size; index++)
            visited[index] = false;
        visited[first_vertex] = true;
        visited[second_vertex] = true;
        int vertex;
        if (first_vertex != 0 && second_vertex != 0) vertex = 0;
        else if (first_vertex != 1 && second_vertex != 1) vertex = 1;
        else vertex = 2;
        dft(vertex, visited);
        for (int i = 0; i < graph_size; ++i) {
            if (!visited[i]) return_value = false;
        }
        delete[] visited;
        return return_value;
    }

    void dft(int v, bool visited[]) {
        visited[v] = true;
        Iterator graphIt(graph[v]);
        while (graphIt.current != nullptr && graphIt.current->index == v) {
            int w = *graphIt;
            if (!visited[w])
                dft(w, visited);
            ++graphIt;
        }
    }
};

#endif //MOSTY_LIBRARY_H