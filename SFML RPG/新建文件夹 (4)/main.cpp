#include<iostream>
#include<vector>
#include<queue>
#include <random>
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<SFML/System.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>

#include"TileMap.hpp"

#define R 44
#define L 74
#define WALLPROB 47
int map[R][L];

std::mt19937 mt;
unsigned int seed = std::random_device()();

void resetseed() {
	mt.seed(seed);
}

int  getInt(int max) {
	return std::uniform_int_distribution<>(0, max - 1)(mt);
}

void CA3()
{
	int x = 0, y = 0;
	for (y = 1; y < L - 1; ++y)
	{
		for (x = 1; x < R - 1; ++x)
		{
			int r1 = 0;
			for (int dy = -1; dy <= 1; ++dy)
			{
				for (int dx = -1; dx <= 1; ++dx)
				{
					if (map[x + dx][y + dy] == 1)
						r1++;
				}
			}
			if (r1 >= 5)
				map[x][y] = 1;
			else
				map[x][y] = 0;
		}
	}
}
int Inmap(int x, int y)
{
	return x >= 0 && (x < R) && y >= 0 &&( y < L);
}
void CA5()
{
	int x = 0, y = 0;
	for (y = 1; y < L - 1; ++y)
	{
		for (x = 1; x < R - 1; ++x)
		{
			int r1 = 0, r2 = 0;
			for (int dy = -2; dy <= 2; ++dy)
			{
				for (int dx = -2; dx <= 2; ++dx)
				{
					int ax = std::abs(dx);
					int ay = std::abs(dy);
					if (ax == 2 && ay == 2)
						continue;
					if (Inmap(x + dx, y + dy) && map[x + dx][y + dy] == 1)
					{
						if (ax <= 1 && ay <= 1)
							r1 += 1;
						r2 += 1;
					}
				}
			}
			if (r1 >= 5 || r2 <= 2)
				map[x][y] = 1;
			else
				map[x][y] = 0;
		}
	}
}
void removeRegions(int removeProb = 100, int minSize = 0);
using Point = sf::Vector2i;
void removeRegions(int removeProb, int minSize)
{
	int currentRegion = -1;
	int regions[R][L] = { 0 };
	for (int i = 0; i < R; i++)
		for (int j = 0; j < L; j++)	regions[i][j] = currentRegion;
	std::vector<int> regionsSizes;

	for (int y = 1; y < L - 1; ++y)
		for (int x = 1; x < R - 1; ++x)
		{
			if (map[x][y] == 1 || regions[x][y] >= 0)
				continue;

			currentRegion += 1;
			regionsSizes.emplace_back(0);

			std::queue<Point> queue;
			queue.emplace(x, y);

			while (!queue.empty())
			{
				Point pos = queue.front();
				queue.pop();

				if (map[pos.x][pos.y] == 1 || regions[pos.x][pos.y] >= 0)
					continue;

				regions[pos.x][pos.y]= currentRegion;
				regionsSizes[currentRegion] += 1;

				Point dpos[4] = { {0,1},{0,-1},{1,0},{-1,0} };
				for (int i = 0; i < 4; i++)
				{
					if (!Inmap(pos.x + dpos[i].x, pos.y + dpos[i].y))
						continue;

					queue.emplace(pos.x + dpos[i].x, pos.y + dpos[i].y);
				}
			}
		}
	// Find the biggest region
	int biggestRegion = 0;
	std::vector<bool> regionsForRemoval(currentRegion + 2, false);

	for (int i = 0; i <= currentRegion; ++i)
	{
		if (regionsSizes[i] > regionsSizes[biggestRegion])
			biggestRegion = i;

		if (rand() % 100  < removeProb || regionsSizes[i] < minSize)
			regionsForRemoval[i] = true;
	}
	// Always do not remove the biggest region
	regionsForRemoval[biggestRegion] = false;

	for (int y = 1; y < L - 1; ++y)
		for (int x = 1; x < R - 1; ++x)
		{
			if (map[x][y] == 1)
				continue;

			int i = regions[x][y];

			if (regionsForRemoval[i])
				map[x][y] = 1;
		}
}
void removeRegions1(int removeProb, int minSize)
{
	int currentRegion = -1;
	int regions[R][L] = { 0 };
	for (int i = 0; i < R; i++)
		for (int j = 0; j < L; j++)	regions[i][j] = currentRegion;
	std::vector<int> regionsSizes;

	for (int y = 1; y < L - 1; ++y)
		for (int x = 1; x < R - 1; ++x)
		{
			if (map[x][y] == 0 || regions[x][y] >= 0)
				continue;

			currentRegion += 1;
			regionsSizes.emplace_back(0);

			std::queue<Point> queue;
			queue.emplace(x, y);

			while (!queue.empty())
			{
				Point pos = queue.front();
				queue.pop();

				if (map[pos.x][pos.y] == 0 || regions[pos.x][pos.y] >= 0)
					continue;

				regions[pos.x][pos.y] = currentRegion;
				regionsSizes[currentRegion] += 1;

				//Point dpos[4] = { {0,1},{0,-1},{1,0},{-1,0} };
				//for (int i = 0; i < 4; i++)
				//{
				//	if (!Inmap(pos.x + dpos[i].x, pos.y + dpos[i].y))
				//		continue;

				//	queue.emplace(pos.x + dpos[i].x, pos.y + dpos[i].y);
				//}
				Point dpos[8] = { {0,1},{0,-1},{1,0},{-1,0},{1,1},{1,-1},{-1,1},{-1,-1} };
				for (int i = 0; i < 8; i++)
				{
					if (!Inmap(pos.x + dpos[i].x, pos.y + dpos[i].y))
						continue;

					queue.emplace(pos.x + dpos[i].x, pos.y + dpos[i].y);
				}
			}
		}
	// Find the biggest region
	int biggestRegion = 0;
	std::vector<bool> regionsForRemoval(currentRegion + 2, false);

	for (int i = 0; i <= currentRegion; ++i)
	{
		if (regionsSizes[i] > regionsSizes[biggestRegion])
			biggestRegion = i;

		if (rand() % 100 < removeProb || regionsSizes[i] < minSize)
			regionsForRemoval[i] = true;
	}
	// Always do not remove the biggest region
	regionsForRemoval[biggestRegion] = false;

	for (int y = 1; y < L - 1; ++y)
		for (int x = 1; x < R - 1; ++x)
		{
			if (map[x][y] == 0)
				continue;

			int i = regions[x][y];

			if (regionsForRemoval[i])
				map[x][y] = 0;
		}
}

void removeWallChunks()
{
	//for (int y = 0; y < L; ++y)
	//{
	//	for (int x = 0; x < R; ++x)
	//	{
	//		if (map[x][y] == 1) map[x][y] = 0;
	//		else if (map[x][y] == 0) map[x][y] = 1;
	//	}
	//}
	removeRegions1(0, 5);
	//for (int y = 0; y < L; ++y)
	//{
	//	for (int x = 0; x < R; ++x)
	//	{
	//		if (map[x][y] == 1) map[x][y] = 0;
	//		else if (map[x][y] == 0) map[x][y] = 1;
	//	}
	//}
}

int main()
{
	
	int x = 0, y = 0, i = 0, j = 0;
	//resetseed();
	//srand((unsigned)time(NULL));
	//for (i = 0; i <100; i++) rand();
	int N = 100;
	while (N--)
	{
		//生成噪化图      WALLPROB  -----------------------------------------------------------------------------------------------
		for (y = 0; y < L; ++y)
			for (x = 0; x < R; ++x)
			{
				if (x == 0 || x == R - 1 || y == 0 || y == L - 1)
					map[x][y] = 1;
				else if (rand() % 100 < WALLPROB)
					map[x][y] = 1;
				else
					map[x][y] = 0;
			}
		//原孢 处理5*5-----------------------------------------------------------------------------------------------
		for (i = 0; i < 4; i++) CA5();
		//原孢 处理3*3-----------------------------------------------------------------------------------------------
		//for (i = 0; i < 3; i++) CA3();
		//for (i = 0; i < 3; i++) CA5();
		//洪水填充
		removeRegions();
		removeWallChunks();

		//打印-----------------------------------------------------------------------------------------------
		for (i = 0; i < R; i++)
		{
			for (j = 0; j < L; j++)
			{
				if (map[i][j] == 1) std::cout << "□";
				else if (map[i][j] == 0) std::cout << "  ";
			}
			printf("\n");
		}

		system("pause");
		system("cls");
	}

}
//int main()
//{
//	sf::RenderWindow window(sf::VideoMode(1184,704), "map11");
//	TileMap map1;
//	
//	while (window.isOpen())
//	{
//		sf::Event event;
//		while (window.pollEvent(event))
//		{
//			if (event.type == sf::Event::Closed)
//				window.close();
//		}
//
//		int x = 0, y = 0, i = 0, j = 0;
//		//srand((unsigned)time(NULL));
//		//for (i = 0; i <100; i++) rand();
//		int N = 100;
//
//		//生成噪化图      WALLPROB  -----------------------------------------------------------------------------------------------
//		for (y = 0; y < L; ++y)
//			for (x = 0; x < R; ++x)
//			{
//				if (x == 0 || x == R - 1 || y == 0 || y == L - 1)
//					map[x][y] = 1;
//				else if (rand() % 100 < WALLPROB)
//					map[x][y] = 1;
//				else
//					map[x][y] = 0;
//			}
//		//原孢 处理5*5-----------------------------------------------------------------------------------------------
//		for (i = 0; i < 4; i++) CA5();
//		//原孢 处理3*3-----------------------------------------------------------------------------------------------
//		//for (i = 0; i < 3; i++) CA3();
//		//洪水填充
//		removeRegions();
//		removeWallChunks();
//
//		if (!map1.load("01.png", sf::Vector2u(16, 16), (int*)map, 74, 44))
//			return -1;
//
//		window.clear();
//		window.draw(map1);
//		window.display();
//		system("pause");
//	}
//	return 0;
//}
