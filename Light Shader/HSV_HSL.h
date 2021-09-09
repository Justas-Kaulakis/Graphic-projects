#ifndef HSV_HSL_H_INCLUDED
#define HSV_HSL_H_INCLUDED

#include <SFML/Graphics/Color.hpp>


sf::Color HSLtoRGB(float H, float S, float L)
{
	float C = (1 - std::fabs(2 * L - 1)) * S; // Chroma
	float HPrime = H / 60; // H'
	float X = C * (1 - std::fabs(std::fmod(HPrime, 2.f) - 1));
	float M = L - C / 2;

	float R = 0.f;
	float G = 0.f;
	float B = 0.f;

	switch (static_cast<int>(HPrime))
	{
	case 0: R = C; G = X;        break; // [0, 1)
	case 1: R = X; G = C;        break; // [1, 2)
	case 2:        G = C; B = X; break; // [2, 3)
	case 3:        G = X; B = C; break; // [3, 4)
	case 4: R = X;        B = C; break; // [4, 5)
	case 5: R = C;        B = X; break; // [5, 6)
	}

	R += M;
	G += M;
	B += M;

	sf::Color color;
	color.r = static_cast<sf::Uint8>(std::round(R * 255));
	color.g = static_cast<sf::Uint8>(std::round(G * 255));
	color.b = static_cast<sf::Uint8>(std::round(B * 255));

	return color;
}

sf::Color HSVtoRGB(float H, float S, float V)
{
	float C = S * V; // Chroma
	float HPrime = std::fmod(H / 60, 6.f); // H'
	float X = C * (1 - std::fabs(std::fmod(HPrime, 2.f) - 1));
	float M = V - C;

	float R = 0.f;
	float G = 0.f;
	float B = 0.f;

	switch (static_cast<int>(HPrime))
	{
	case 0: R = C; G = X;        break; // [0, 1)
	case 1: R = X; G = C;        break; // [1, 2)
	case 2:        G = C; B = X; break; // [2, 3)
	case 3:        G = X; B = C; break; // [3, 4)
	case 4: R = X;        B = C; break; // [4, 5)
	case 5: R = C;        B = X; break; // [5, 6)
	}

	R += M;
	G += M;
	B += M;

	sf::Color color;
	color.r = static_cast<sf::Uint8>(std::round(R * 255));
	color.g = static_cast<sf::Uint8>(std::round(G * 255));
	color.b = static_cast<sf::Uint8>(std::round(B * 255));

	return color;
}


#endif // HSV_HSL_H_INCLUDED
