#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <queue>
#include <limits>

struct Edge { int a, b; float weight; };
struct Node { sf::Vector2f pos; bool selected = false; };

float distance(sf::Vector2f a, sf::Vector2f b) {
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

std::vector<int> dijkstra(int start, int goal,
    const std::vector<Node>& nodes,
    const std::vector<Edge>& edges) {
    const int n = nodes.size();
    std::vector<float> dist(n, std::numeric_limits<float>::infinity());
    std::vector<int> prev(n, -1);
    std::vector<bool> vis(n, false);
    dist[start] = 0;

    for (int i = 0; i < n; ++i) {
        int u = -1;
        for (int j = 0; j < n; ++j)
            if (!vis[j] && (u == -1 || dist[j] < dist[u]))
                u = j;
        if (dist[u] == std::numeric_limits<float>::infinity()) break;
        vis[u] = true;

        for (auto& e : edges) {
            if (e.a == u && !vis[e.b] && dist[u] + e.weight < dist[e.b]) {
                dist[e.b] = dist[u] + e.weight;
                prev[e.b] = u;
            }
            if (e.b == u && !vis[e.a] && dist[u] + e.weight < dist[e.a]) {
                dist[e.a] = dist[u] + e.weight;
                prev[e.a] = u;
            }
        }
    }

    std::vector<int> path;
    for (int v = goal; v != -1; v = prev[v]) path.push_back(v);
    std::reverse(path.begin(), path.end());
    if (path.size() < 2 || path.front() != start) return {};
    return path;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Интерактивный граф (SFML)");
    window.setFramerateLimit(60);

    std::vector<Node> nodes = {
        {{100, 100}}, {{300, 150}}, {{200, 300}}, {{500, 200}}, {{400, 400}}
    };
    std::vector<Edge> edges = {
        {0, 1, 1}, {1, 2, 1.5}, {0, 2, 2}, {1, 3, 2}, {2, 4, 2.5}, {3, 4, 1}
    };

    int selectedCount = 0;
    int startNode = -1, endNode = -1;
    std::vector<int> shortestPath;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                for (int i = 0; i < nodes.size(); ++i) {
                    float r = 20;
                    if (distance(mouse, nodes[i].pos) < r) {
                        if (selectedCount == 0) {
                            startNode = i;
                            nodes[i].selected = true;
                            selectedCount = 1;
                        }
                        else if (selectedCount == 1 && i != startNode) {
                            endNode = i;
                            nodes[i].selected = true;
                            shortestPath = dijkstra(startNode, endNode, nodes, edges);
                            selectedCount = 0;
                        }
                        else {
                            for (auto& n : nodes) n.selected = false;
                            shortestPath.clear();
                            selectedCount = 0;
                        }
                    }
                }
            }
        }

        window.clear(sf::Color(30, 30, 40));

        // Рёбра
        for (auto& e : edges) {
            sf::Vertex line[] = {
                sf::Vertex(nodes[e.a].pos, sf::Color::White),
                sf::Vertex(nodes[e.b].pos, sf::Color::White)
            };
            window.draw(line, 2, sf::Lines);
        }

        // Кратчайший путь (красный)
        for (size_t i = 1; i < shortestPath.size(); ++i) {
            int a = shortestPath[i - 1];
            int b = shortestPath[i];
            sf::Vertex line[] = {
                sf::Vertex(nodes[a].pos, sf::Color::Red),
                sf::Vertex(nodes[b].pos, sf::Color::Red)
            };
            window.draw(line, 2, sf::Lines);
        }

        // Узлы
        for (auto& n : nodes) {
            sf::CircleShape c(20);
            c.setOrigin(20, 20);
            c.setPosition(n.pos);
            c.setFillColor(n.selected ? sf::Color::Yellow : sf::Color(100, 180, 255));
            c.setOutlineThickness(2);
            c.setOutlineColor(sf::Color::Black);
            window.draw(c);
        }

        window.display();
    }

    return 0;
}
