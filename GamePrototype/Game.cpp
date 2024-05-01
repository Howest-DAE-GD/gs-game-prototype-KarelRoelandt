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

	m_speed = 1500.f;
	
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
			walls.left = resultLeft.intersectPoint.x;
		if (utils::Raycast(m_Walls[idx], m_BurglarPos, Point2f{ m_BurglarPos.x + m_BurglarSize, m_BurglarPos.y }, resultRight))
			walls.right = resultRight.intersectPoint.x;
		if (utils::Raycast(m_Walls[idx], m_BurglarPos, Point2f{ m_BurglarPos.x, m_BurglarPos.y + m_BurglarSize }, resultUp))
			walls.top = resultUp.intersectPoint.y;
		if (utils::Raycast(m_Walls[idx], m_BurglarPos, Point2f{ m_BurglarPos.x, m_BurglarPos.y - m_BurglarSize }, resultDown))
			walls.bottom = resultDown.intersectPoint.y;
	}

	m_elapsedSec = elapsedSec;

	if (m_BurglarPos.x + m_BurglarSize < walls.right || walls.right == 0) m_BurglarPos.x += m_Steps.right;
	if (m_BurglarPos.x - m_BurglarSize > walls.left || walls.left == 0) m_BurglarPos.x -= m_Steps.left;
	if (m_BurglarPos.y + m_BurglarSize < walls.top || walls.top == 0) m_BurglarPos.y += m_Steps.top;
	if (m_BurglarPos.y - m_BurglarSize > walls.bottom || walls.bottom == 0) m_BurglarPos.y -= m_Steps.bottom;

	m_Steps = {0,0,0,0};
			

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

	utils::SetColor(Color4f{ 0,0,1,1 });
	for (size_t i = 0; i < m_Walls.size(); i++)
	{
		utils::FillPolygon(m_Walls[i]);
	}
	//utils::DrawPolygon(m_Walls[0]);

	utils::SetColor(Color4f{ .42f,.29f,0.22f,1.f });
	utils::FillEllipse(m_BurglarPos, m_BurglarSize, m_BurglarSize);

}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	
	float step{ m_elapsedSec * m_speed };

	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;

	switch ( e.keysym.sym )
	{
	case SDLK_LEFT:
		m_Steps.left = step;
		break;
	case SDLK_RIGHT:
		m_Steps.right = step;
		break;
	case SDLK_UP:
		m_Steps.top = step;
		break;
	case SDLK_DOWN:
		m_Steps.bottom = step;
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
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	//switch ( e.keysym.sym )
	//{
	//case SDLK_LEFT:
	//	//std::cout << "Left arrow key released\n";
	//	break;
	//case SDLK_RIGHT:
	//	//std::cout << "`Right arrow key released\n";
	//	break;
	//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	//}
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
