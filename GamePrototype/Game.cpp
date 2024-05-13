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

	m_BurglarSize = 20.f;
	m_BurglarPos = Point2f{ 1450.f, 50.f};
	m_Hit = false;

	m_ArtefactSize = 20.f;
	m_ArtefactPos = Point2f{ 100.f, 100.f };
	m_Pickup = false;

	m_BoulderSize = 40.f;
	m_BoulderPos = Point2f{ -50.f, 800.f};

	m_speed = 200.f;
	
}

void Game::Cleanup( )
{
}

void Game::Update( float elapsedSec )
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

	if(utils::IsOverlapping(Rectf{ m_ArtefactPos.x, m_ArtefactPos.y, m_ArtefactSize, m_ArtefactSize }, Circlef{ m_BurglarPos, m_BurglarSize }))
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

	if (m_BoulderPos.x <= 1650.f)
	{
		m_BoulderPos.x += 600 * elapsedSec;
	}
	else
	{
		m_BoulderPos.x = rand() % 200;
		m_BoulderPos.y = 750 + rand() % 2 * 50;
	}

	if (utils::IsOverlapping(Circlef{ m_BoulderPos, m_BoulderSize }, Circlef{ m_BurglarPos, m_BurglarSize }))
	{
		m_Hit = true;
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
