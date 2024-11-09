#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>

std::vector<int> encodeNRZL(const std::string& data) {
    std::vector<int> signal;
    for (char bit : data) {
        signal.push_back(bit == '1' ? 1 : -1);
    }
    return signal;
}

std::vector<int> encodeNRZI(const std::string& data) {
    std::vector<int> signal;
    int last_voltage = 1;
    for (char bit : data) {
        if (bit == '1') {
            last_voltage = (last_voltage == 1) ? -1 : 1;
        }
        signal.push_back(last_voltage);
    }
    return signal;
}

std::vector<int> encodeAMI(const std::string& data) {
    std::vector<int> signal;
    int last_voltage = 1;
    for (char bit : data) {
        if (bit == '1') {
            signal.push_back(last_voltage);
            last_voltage = (last_voltage == 1) ? -1 : 1;
        } else {
            signal.push_back(0);
        }
    }
    return signal;
}

std::vector<int> encodePseudoternary(const std::string& data) {
    std::vector<int> signal;
    int last_voltage = 1;
    for (char bit : data) {
        if (bit == '0') {
            signal.push_back(last_voltage);
            last_voltage = (last_voltage == 1) ? -1 : 1;  
        }
        else {
            signal.push_back(0); 
        }
    }
    return signal;
}

std::vector<int> encodeManchester(const std::string& data) {
    std::vector<int> signal;
    for (char bit : data) {
        if (bit == '1') {
            signal.push_back(1);  
            signal.push_back(-1); 
        }
        else {
            signal.push_back(-1); 
            signal.push_back(1);  
        }
    }
    return signal;
}

std::vector<int> encodeDifferentialManchester(const std::string& data) {
    std::vector<int> signal;
    int last_voltage = 1; 
    for (char bit : data) {
        if (bit == '0') {
            last_voltage = (last_voltage == 1) ? -1 : 1;
            signal.push_back(last_voltage); 
            signal.push_back(last_voltage); 
        }
        else { 
            
            signal.push_back(last_voltage); 
            last_voltage = (last_voltage == 1) ? -1 : 1;
            signal.push_back(last_voltage); 
        }
    }
    return signal;
}


void drawSignal(const std::vector<int>& signal, const std::string& data, const std::string& label) {
    sf::RenderWindow window(sf::VideoMode(1000, 400), label + " Encoding");

    sf::Font font;
    if (!font.loadFromFile("C:/Users/shank/SFML projects/Digital Signals/fonts/Sansation_Regular.ttf")) {
        std::cerr << "Error loading font!\n";
        return;
    }

    const int windowWidth = 1000;
    const int windowHeight = 400;
    const int yMid = windowHeight / 2; 
    const int xStart = 50; 
    const int bitWidth = 40; 
    int segmentWidth = (label == "Manchester" || label == "Differential Manchester") ? bitWidth / 2 : bitWidth; // Half width for Manchester encoding

    while (window.isOpen()) {
        window.clear(sf::Color::White);

        
        sf::Text labelText;
        labelText.setFont(font);
        labelText.setString(label + " Encoding");
        labelText.setCharacterSize(24);
        labelText.setFillColor(sf::Color::Black);
        labelText.setPosition(windowWidth / 2 - labelText.getGlobalBounds().width / 2, 20); // Centered at top
        window.draw(labelText);

        
        int x = xStart;
        for (size_t i = 0; i < data.size(); ++i) {
            sf::Text bitText;
            bitText.setFont(font);
            bitText.setString(std::string(1, data[i]));
            bitText.setCharacterSize(20);
            bitText.setFillColor(sf::Color::Black);
            bitText.setPosition(x + 10, 70); 
            window.draw(bitText);
            x += bitWidth;
        }

        x = xStart;
        for (size_t i = 0; i <= data.size(); ++i) {
            sf::RectangleShape gridLine(sf::Vector2f(2, windowHeight - 100));
            gridLine.setPosition(x, 50); 
            gridLine.setFillColor(sf::Color(200, 200, 200));
            window.draw(gridLine);
            x += bitWidth;
        }

        int prevY = yMid - signal[0] * 40;
        x = xStart;
        for (size_t i = 0; i < signal.size(); ++i) {
            int currY = yMid - signal[i] * 40;

            sf::RectangleShape horizontalLine(sf::Vector2f(segmentWidth, 2));
            horizontalLine.setPosition(x, currY);
            horizontalLine.setFillColor(sf::Color::Black);
            window.draw(horizontalLine);

            if (i > 0 && currY != prevY) {
                sf::RectangleShape verticalLine(sf::Vector2f(2, abs(currY - prevY)));
                verticalLine.setPosition(x, std::min(prevY, currY));
                verticalLine.setFillColor(sf::Color::Black);
                window.draw(verticalLine);
            }

            prevY = currY;
            x += segmentWidth;
        }

        window.display();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }
}


int main() {
    std::string data;
    int choice;

    std::cout << "Enter binary data: ";
    std::cin >> data;

    std::cout << "Select encoding type:\n";
    std::cout << "1. NRZ-L\n";
    std::cout << "2. NRZ-I\n";
    std::cout << "3. Bipolar AMI\n";
    std::cout << "4. Pseudoternary\n";
    std::cout << "5. Manchester\n";
    std::cout << "6. Differential Manchester\n"; 
    std::cout << "Enter choice: ";
    std::cin >> choice;

    std::vector<int> signal;
    std::string label;

    if (choice == 1) {
        signal = encodeNRZL(data);
        label = "NRZ-L";
    }
    else if (choice == 2) {
        signal = encodeNRZI(data);
        label = "NRZ-I";
    }
    else if (choice == 3) {
        signal = encodeAMI(data);
        label = "Bipolar AMI";
    }
    else if (choice == 4) {
        signal = encodePseudoternary(data);
        label = "Pseudoternary";
    }
    else if (choice == 5) {
        signal = encodeManchester(data);
        label = "Manchester";
    }
    else if (choice == 6) { 
        signal = encodeDifferentialManchester(data);
        label = "Differential Manchester";
    }
    else {
        std::cout << "Invalid choice! Exiting program.\n";
        return 1;
    }



    drawSignal(signal, data, label);

    return 0;
}
