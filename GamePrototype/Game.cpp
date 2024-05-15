#include "pch.h"
#include "Game.h"

#include <iostream>
#include <utils.h>

Game::Game( const Window& window ) 
	:BaseGame{ window }
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	if (!SVGParser::GetVerticesFromSvgFile("map.svg", m_Walls)) std::cout << "Mapfile error!";

	InitGame();
	
}

void Game::Cleanup( )
{
}

void Game::Update(float elapsedSec)
{
	Border4f walls{};

	for (int idx{}; idx < m_Walls.size(); ++idx)
	{
		utils::HitInfo resultLeft{};
		utils::HitInfo resultRight{};
		utils::HitInfo resultUp{};
		utils::HitInfo resultDown{};

		if (utils::Raycast(m_Walls[idx], m_BurglarPos, Point2f{ m_BurglarPos.x - m_BurglarSize, m_BurglarPos.y }, resultLeft))
		{
			walls.left = resultLeft.intersectPoint.x;
			if (m_BurglarPos.x - m_BurglarSize > walls.left || walls.left == 0) m_BurglarPos.x -= m_Steps.left;
		}
		if (utils::Raycast(m_Walls[idx], m_BurglarPos, Point2f{ m_BurglarPos.x + m_BurglarSize, m_BurglarPos.y }, resultRight))
		{
			walls.right = resultRight.intersectPoint.x;
			if (m_BurglarPos.x + m_BurglarSize < walls.right || walls.right == 0) m_BurglarPos.x += m_Steps.right;
		}
		if (utils::Raycast(m_Walls[idx], m_BurglarPos, Point2f{ m_BurglarPos.x, m_BurglarPos.y + m_BurglarSize }, resultUp))
		{
			walls.top = resultUp.intersectPoint.y;
			if (m_BurglarPos.y + m_BurglarSize < walls.top || walls.top == 0) m_BurglarPos.y += m_Steps.top;
		}
		if (utils::Raycast(m_Walls[idx], m_BurglarPos, Point2f{ m_BurglarPos.x, m_BurglarPos.y - m_BurglarSize }, resultDown))
		{
			walls.bottom = resultDown.intersectPoint.y;
			if (m_BurglarPos.y - m_BurglarSize > walls.bottom || walls.bottom == 0) m_BurglarPos.y -= m_Steps.bottom;
		}
	}

	if (utils::IsOverlapping(Rectf{ m_ArtefactPos.x, m_ArtefactPos.y, m_ArtefactSize, m_ArtefactSize }, Circlef{ m_BurglarPos, m_BurglarSize }))
		m_Pickup = true;


	if (m_BurglarPos.x + m_BurglarSize < walls.right || walls.right == 0) m_BurglarPos.x += m_Steps.right * elapsedSec;
	if (m_BurglarPos.x - m_BurglarSize > walls.left || walls.left == 0) m_BurglarPos.x -= m_Steps.left * elapsedSec;
	if (m_BurglarPos.y + m_BurglarSize < walls.top || walls.top == 0) m_BurglarPos.y += m_Steps.top * elapsedSec;
	if (m_BurglarPos.y - m_BurglarSize > walls.bottom || walls.bottom == 0) m_BurglarPos.y -= m_Steps.bottom * elapsedSec;

	if (m_Pickup == true)
	{
		m_ArtefactPos.x = m_BurglarPos.x - m_ArtefactSize / 2;
		m_ArtefactPos.y = m_BurglarPos.y - m_ArtefactSize / 2;
	}

	if (m_Pickup)
	{
		if (m_BoulderPos.x <= 1650.f)
		{
			m_BoulderPos.x += 600 * elapsedSec;
		}
		else
		{
			m_BoulderPos.x = float(rand() % 200);
			m_BoulderPos.y = float(750 + rand() % 2 * 50);
		}
	}
	
	for (int i{}; i < 5; ++i)
	{
		int randArrow{ rand() % 5 };
		if (m_ArrowsDown[randArrow].y > 300)
		{
			m_ArrowsDown[randArrow].y -= m_ArrowSpeed * elapsedSec;
		}
		else
		{
			m_ArrowsDown[randArrow].y = 650;
		}
	}

	for (int i{}; i < 5; ++i)
	{
		int dirX{ rand() % 3 - 1 };
		int dirY{ rand() % 3 - 1 };
		int randSpider{ rand() % 5 };

		float spiderSpeed{ 400.f };

		if ((m_SpiderColl[randSpider].x + spiderSpeed * dirX * elapsedSec) < 1150 && (m_SpiderColl[randSpider].x + spiderSpeed * dirX * elapsedSec) > 750)
		{
			m_SpiderColl[randSpider].x += spiderSpeed * dirX * elapsedSec;
		}
		if ((m_SpiderColl[randSpider].y + spiderSpeed * dirY * elapsedSec) < 240 && (m_SpiderColl[randSpider].y + spiderSpeed * dirY * elapsedSec) > 60)
		{
			m_SpiderColl[randSpider].y += spiderSpeed * dirY * elapsedSec;
		}
	}

	if (m_BurglarPos.x < 700 && m_BurglarPos.y < 250 && m_Pickup)
	{
		for (int i{}; i < 5; ++i)
		{
			int randArrow{ rand() % 5 };
			if (m_ArrowsUp[randArrow].y < 250)
			{
				m_ArrowsUp[randArrow].y += m_ArrowSpeed * elapsedSec;
			}
			else
			{
				m_ArrowsUp[randArrow].y = 0;
			}
		}
	}
	else
	{
		for (int i{}; i < 5; ++i)
		{
			m_ArrowsUp[i].y = 0;
		}
	}

	for (int i{}; i < 5; ++i)
	{
		if (utils::IsOverlapping(Circlef{m_ArrowsDown[i].x, m_ArrowsDown[i].y, m_ArrowWidth }, Circlef{m_BurglarPos, m_BurglarSize}))
		{
			m_Hit = true;
		}
	}

	if (m_BurglarPos.x < 700 && m_BurglarPos.y > 200 && m_Pickup)
	{
		m_HoleSize += m_HoleStep * elapsedSec;

		if (m_HoleSize >= 120)
		{
			m_HoleStep *= -1;
		}
		else if (m_HoleSize < 0)
		{
			m_HoleStep *= -1;
		}
	}

	if (utils::IsOverlapping(Circlef{ m_BoulderPos, m_BoulderSize }, Circlef{ m_BurglarPos, m_BurglarSize }))
	{
		m_Hit = true;
	}

	if (utils::IsOverlapping(Circlef{ m_HolePos, m_HoleSize }, Circlef{ m_BurglarPos, m_BurglarSize }))
	{
		m_Hit = true;
	}

	for (int i{}; i < 5; ++i)
	{
		Rectf tempArrowBox{ m_ArrowsUp[i].x, m_ArrowsUp[i].y, m_ArrowWidth, m_ArrowLength };
		if (utils::IsOverlapping(tempArrowBox, Circlef{ m_BurglarPos, m_BurglarSize }))
		{
			m_Hit = true;
		}
	}

	for (int i{}; i < 5; ++i)
	{
		if (utils::IsOverlapping(Circlef{ m_SpiderColl[i], 10.f}, Circlef{m_BurglarPos, m_BurglarSize}))
		{
			m_Hit = true;
		}
	}

	if (m_Hit)
	{
		InitGame();
	}


	// Check keyboard state
	//const Uint8 *pStates = SDL_GetKeyboardState( nullptr );
	//if ( pStates[SDL_SCANCODE_RIGHT] )
	//{
	//	std::cout << "Right arrow key is down\n";
	//}
	//if ( pStates[SDL_SCANCODE_LEFT] && pStates[SDL_SCANCODE_UP])
	//{
	//	std::cout << "Left and up arrow keys are down\n";
	//}
}

void Game::Draw( ) const
{
	ClearBackground( );

	utils::SetColor(Color4f{ 0.f, 1.f, 0.f, 1.f });
	for (int i{}; i < 5; ++i)
	{
		utils::FillEllipse(m_SpiderColl[i], 10, 10);
	}

	utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
	for (int i{}; i < 5; ++i)
	{
		utils::FillRect(m_ArrowsUp[i], m_ArrowWidth, m_ArrowLength);
	}

	utils::SetColor(Color4f{ 1.0f, .5f, 0.f, .5f });
	for (int i{}; i < 5; ++i)
	{
		utils::FillEllipse(m_ArrowsDown[i], m_ArrowWidth, m_ArrowWidth);
	}

	utils::SetColor(Color4f{ 0.42f, 0.42f, 0.42f, 1.f });
	utils::FillEllipse(m_BoulderPos, m_BoulderSize, m_BoulderSize);

	utils::SetColor(Color4f{ .0f, .0f, .75f, 1 });
	for (size_t i = 0; i < m_Walls.size(); i++)
	{
		utils::FillPolygon(m_Walls[i]);
	}
	//utils::DrawPolygon(m_Walls[0]);

	utils::SetColor(Color4f{ 0.42f, 0.29f, 0.22f, 1.f });
	utils::FillEllipse(m_BurglarPos, m_BurglarSize, m_BurglarSize);

	utils::SetColor(Color4f{ 0.75f, 0.75f, 0.f, 1.f });
	utils::FillRect(m_ArtefactPos, m_ArtefactSize, m_ArtefactSize);

	utils::SetColor(Color4f{ .05f, .05f, .05f, 1.f });
	utils::FillEllipse(m_HolePos, m_HoleSize, m_HoleSize);

}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{

	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;

	switch ( e.keysym.sym )
	{
	case SDLK_LEFT:
		m_Steps.left = m_speed;
		break;
	case SDLK_RIGHT:
		m_Steps.right = m_speed;
		break;
	case SDLK_UP:
		m_Steps.top = m_speed;
		break;
	case SDLK_DOWN:
		m_Steps.bottom = m_speed;
		break;

		//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	}

	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	switch (e.keysym.sym)
	{
	case SDLK_LEFT:
		m_Steps.left = 0;
		break;
	case SDLK_RIGHT:
		m_Steps.right = 0;
		break;
	case SDLK_UP:
		m_Steps.top = 0;
		break;
	case SDLK_DOWN:
		m_Steps.bottom = 0;
		break;

		//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::InitGame()
{
	m_BurglarSize = 20.f;
	m_BurglarPos = Point2f{ 1450.f, 50.f };
	m_Hit = false;

	m_ArtefactSize = 20.f;
	m_ArtefactPos = Point2f{ 100.f, 100.f };
	m_Pickup = false;

	m_BoulderSize = 40.f;
	m_BoulderPos = Point2f{ -50.f, 800.f };

	m_speed = 200.f;

	m_ArrowWidth = 4.f;
	m_ArrowLength = 30.f;
	m_ArrowSpeed = 200.f;
	float arrowStartPosX{ 190.f };
	for (int i{}; i < 5; ++i)
	{
		m_ArrowsUp.push_back(Point2f{ arrowStartPosX + 70 * i, 20.f });
	}

	float dropStartPosX{ 950.f };
	for (int i{}; i < 5; ++i)
	{
		m_ArrowsDown.push_back(Point2f{ dropStartPosX + 105 * i, 650.f });
	}

	m_HoleSize = 0.f ;
	m_HoleStep = 40.f ;
	m_HolePos = Point2f{ 480.f, 480.f};

	for (int i{}; i < 5; ++i)
	{
		m_SpiderColl.push_back(Point2f{ 800.f + rand() % 200, 100.f + rand() % 100});
	}

}
