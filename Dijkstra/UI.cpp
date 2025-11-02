#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <iostream>





// Статическая функция для рисования стрелки
static void drawArrow(sf::RenderTarget& target, sf::Vector2f from, sf::Vector2f to, float nodeRadius, sf::Color color, float size = 20.f)
{
    sf::Vector2f dir = to - from;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len == 0.f) return;
    dir /= len;

    // Точка конца линии, сдвинутая от центра узла
    sf::Vector2f tip = to - dir * nodeRadius;
    sf::Vector2f normal(-dir.y, dir.x);

    sf::ConvexShape arrow;
    arrow.setPointCount(3);
    arrow.setPoint(0, tip);  // вершина стрелки
    arrow.setPoint(1, tip - dir * size + normal * (size * 0.6f));
    arrow.setPoint(2, tip - dir * size - normal * (size * 0.6f));
    arrow.setFillColor(color);

    target.draw(arrow);
}

static void drawEdgeWeight(sf::RenderTarget& target, sf::Vector2f from, sf::Vector2f to, int weight, float nodeRadius, sf::Font& font, sf::Color color)
{
    // --- Рисуем линию ---
    sf::Vector2f dir = to - from;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len == 0.f) return;
    dir /= len;

    sf::RectangleShape line(sf::Vector2f(len - nodeRadius, 3.f));
    line.setFillColor(color);
    line.setPosition(from + dir * nodeRadius);
    line.setRotation(sf::degrees(std::atan2(dir.y, dir.x) * 180.f / 3.14159265f));
    target.draw(line);

    // --- Рисуем вес ---
    sf::Vector2f mid = (from + to) / 2.f;
    sf::Vector2f normal(-dir.y, dir.x);
    mid += normal * 10.f; // смещение от линии

    sf::Text text(font);
    text.setString(std::to_string(weight));
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    text.setOrigin({ text.getLocalBounds().getCenter().x, text.getLocalBounds().getCenter().y });
    text.setPosition(mid);
    target.draw(text);
}



int drawWindow(int *matrix, int node_count, int* path, int from, int to)
{
    sf::Clock clock;
    bool repelActive = true;

    constexpr float WIDTH = 840.f;
    constexpr float HEIGHT = 680.f;


    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode({ (unsigned)WIDTH, (unsigned)HEIGHT }), "Graph", sf::State::Windowed, settings);
    window.setFramerateLimit(60);

    // Пример ориентированного графа (оставил твой)
    std::vector<std::vector<int>> adjacency(node_count, std::vector<int>(node_count, 0));;

    for (int i = 0; i < node_count; i++)
    {
        for (int j = 0; j < node_count; j++)
        {
            adjacency[i][j] = matrix[i * node_count + j];
        }
    }

    char first = 1;

    std::vector<int> normPath;
    for (int i = 0; i < node_count; i++)
    {
        if (path[i] != INT_MAX)
        {
            if (first)
            {
                normPath.push_back(from - 1);
                first = 0;
            }
            else
            {
                normPath.push_back(path[i] - 1); // преобразуем номер узла с 1 → индекс с 0
            }
        }
    }
    normPath.push_back(to - 1);

    int n = static_cast<int>(adjacency.size());
    std::vector<sf::Vector2f> nodes(n);

    sf::Vector2f center(WIDTH / 2.f, HEIGHT / 2.f);
    std::default_random_engine gen(static_cast<unsigned>(std::time(nullptr)));
    std::uniform_real_distribution<float> offsetDist(-30.f, 30.f); // смещение ±30 пикселей

    for (int i = 0; i < n; ++i) 
    {
        nodes[i] = center + sf::Vector2f(offsetDist(gen), offsetDist(gen));
    }

    // Вектор скоростей для пошаговой анимации
    std::vector<sf::Vector2f> velocities(n, { 0.f, 0.f });

    const float nodeRadius = 25.f;

    // Параметры физики
    const float area = WIDTH * HEIGHT;
    const float k = std::sqrt(area / std::max(1, n)) * 0.3f;  // меньше естественная длина
    const float damping = 0.99f;                              // больше затухание
    const float maxStep = 2.5f;

    // Переменные для перетаскивания узлов
    bool isDragging = false;
    int draggedNode = -1;
    sf::Vector2f dragOffset{ 0.f, 0.f };

    // Основной цикл
    while (window.isOpen())
    {



        float elapsed = clock.getElapsedTime().asSeconds();
       

        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mousePressed->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2f mousePos(static_cast<float>(mousePressed->position.x), static_cast<float>(mousePressed->position.y));
                    for (int i = 0; i < n; ++i)
                    {
                        float dx = nodes[i].x - mousePos.x;
                        float dy = nodes[i].y - mousePos.y;
                        float distance = std::sqrt(dx * dx + dy * dy);
                        if (distance <= nodeRadius)
                        {
                            isDragging = true;
                            draggedNode = i;
                            dragOffset = nodes[i] - mousePos;
                            break;
                        }
                    }
                }
            }

            if (event->is<sf::Event::MouseButtonReleased>())
            {
                if (event->getIf<sf::Event::MouseButtonReleased>()->button == sf::Mouse::Button::Left)
                {
                    isDragging = false;
                    draggedNode = -1;
                }
            }

            if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
            {
                if (isDragging && draggedNode >= 0 && draggedNode < n)
                {
                    sf::Vector2f mousePos(static_cast<float>(mouseMoved->position.x), static_cast<float>(mouseMoved->position.y));
                    nodes[draggedNode] = mousePos + dragOffset;

                    // Ограничиваем позицию узла в пределах окна
                    nodes[draggedNode].x = std::min(WIDTH - nodeRadius, std::max(nodeRadius, nodes[draggedNode].x));
                    nodes[draggedNode].y = std::min(HEIGHT - nodeRadius, std::max(nodeRadius, nodes[draggedNode].y));

                    // Обнулим скорость перетаскиваемого узла, чтобы физика не дергала его
                    velocities[draggedNode] = { 0.f, 0.f };
                }
            }

  
        }

        if (repelActive)
        {
            std::vector<sf::Vector2f> disp(n, { 0.f, 0.f });

            // --- Отталкивание ---
            const float repK = 20000.f; // сила отталкивания
            for (int i = 0; i < n; ++i)
            {
                for (int j = i + 1; j < n; ++j)
                {
                    sf::Vector2f delta = nodes[i] - nodes[j];
                    float dist = std::sqrt(delta.x * delta.x + delta.y * delta.y) + 0.1f;
                    sf::Vector2f dir = delta / dist;

                    float force = repK / (dist * dist);
                    disp[i] += dir * force;
                    disp[j] -= dir * force;
                }
            }

            // --- Притяжение (по рёбрам) ---
            const float springK = 0.02f;      // сила пружины
            const float restLen = 250.f;      // естественная длина ребра
            for (int i = 0; i < n; ++i)
            {
                for (int j = 0; j < n; ++j)
                {
                    if (adjacency[i][j] != 0)
                    {
                        sf::Vector2f delta = nodes[i] - nodes[j];
                        float dist = std::sqrt(delta.x * delta.x + delta.y * delta.y) + 0.1f;
                        sf::Vector2f dir = delta / dist;

                        float force = springK * (dist - restLen);
                        disp[i] -= dir * force;
                        disp[j] += dir * force;
                    }
                }
            }

            // --- Применение смещения с демпфированием ---
            const float step = 0.01f;   // маленький шаг (скорость стабилизации)
            const float damping = 0.9f; // подавление колебаний

            for (int i = 0; i < n; ++i)
            {
                if (i == draggedNode) continue;

                velocities[i] = (velocities[i] + disp[i] * step) * damping;
                nodes[i] += velocities[i];

                // ограничиваем в окне
                nodes[i].x = std::min(WIDTH - nodeRadius, std::max(nodeRadius, nodes[i].x));
                nodes[i].y = std::min(HEIGHT - nodeRadius, std::max(nodeRadius, nodes[i].y));
            }
        }





        // --- Рендер ---
        window.clear(sf::Color(18, 18, 18));


        sf::Font font;

        try {
            std::filesystem::path filename = "C:\\WINDOWS\\Fonts\\Verdana.ttf";
            font.openFromFile(filename);
        }
        catch (std::exception& e) {
            return -1; // Не удалось загрузить шрифт
        }

        // Рёбра + стрелки
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                if (adjacency[i][j] != 0)
                {
                    bool bothDirections = adjacency[j][i] && i != j;
                    bool inPath = false;

                    // Проверка: ребро принадлежит пути?
                    for (int k = 0; k + 1 < normPath.size(); k++)
                    {
                        if ((normPath[k] == i && normPath[k + 1] == j) ||
                            (normPath[k] == j && normPath[k + 1] == i))
                        {
                            inPath = true;
                            break;
                        }
                    }

                    sf::Color color = inPath ? sf::Color(245, 105, 13) : sf::Color(47, 54, 77);

                    if (bothDirections)
                    {
                        if (i < j)
                            drawEdgeWeight(window, nodes[i], nodes[j], adjacency[i][j], nodeRadius, font, color);
                    }
                    else
                    {
                        drawEdgeWeight(window, nodes[i], nodes[j], adjacency[i][j], nodeRadius, font, color);
                        drawArrow(window, nodes[i], nodes[j], nodeRadius, color);
                    }
                }
            }
        }



        // Внутри цикла рендера узлов:
        for (int i = 0; i < n; ++i)
        {
            sf::CircleShape node(nodeRadius);
            node.setOrigin({ nodeRadius, nodeRadius });
            node.setPosition(nodes[i]);

            // Текст с номером узла
            sf::Text text(font);

            if (i == draggedNode) node.setFillColor(sf::Color(220, 101, 128));
            else
            {

                if (from - 1 == i)
                {
                    text.setFillColor(sf::Color(0, 255, 0));
                    node.setOutlineColor(sf::Color(0, 255, 0));
                }
                else if (to - 1 == i)
                {
                    text.setFillColor(sf::Color(247, 33, 0));
                    node.setOutlineColor(sf::Color(247, 33, 0));
                }
                else
                {
                    bool onPath = false;
                    for (int k = 0; k < normPath.size(); k++)
                    {
                        if (normPath[k] == i)
                        {
                            onPath = true;
                            break;
                        }
                    }

                    text.setFillColor(sf::Color(255, 255, 255));
                    if (onPath)
                        node.setOutlineColor(sf::Color(245, 105, 13)); // оранжевая обводка, если на пути
                    else
                        node.setOutlineColor(sf::Color(47, 54, 77));   // серая, если не на пути
                }

            }

            node.setFillColor(sf::Color(47, 54, 77));
            node.setOutlineThickness(3.f);
            window.draw(node);


  

            text.setString(std::to_string(i + 1)); 
            text.setCharacterSize(static_cast<unsigned int>(nodeRadius - 5.0));

            // Центрирование текста
            sf::FloatRect bounds = text.getLocalBounds();
            text.setOrigin(bounds.position + bounds.size / 2.f); // центрируем
            text.setPosition(nodes[i]);
            window.draw(text);


            window.draw(text);
        }


        window.display();
    }

    return 0;
}
