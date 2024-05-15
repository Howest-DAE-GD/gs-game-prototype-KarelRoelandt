#pragma once
#include "BaseGame.h"
#include "SVGParser.h"

class Game : public BaseGame
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update( float elapsedSec ) override;
	void Draw( ) const override;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;

private:
	struct Border4f
	{
		float left;
		float right;
		float top;
		float bottom;
	};

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;

	void InitGame();

	std::vector<std::vector<Point2f>> m_Walls;

	float m_BurglarSize{};
	Point2f m_BurglarPos{};
	bool m_Hit{};

	float m_ArtefactSize{};
	Point2f m_ArtefactPos{};
	bool m_Pickup{};
	std::vector<Point2f> m_ArtefactColl{};

	float m_BoulderSize{};
	Point2f m_BoulderPos{};
	std::vector<Point2f> m_BoulderColl{};

	float m_ArrowWidth{};
	float m_ArrowLength{};
	float m_ArrowSpeed{};
	std::vector<Point2f> m_ArrowsUp;
	std::vector<Point2f> m_ArrowsDown;

	float m_HoleSize{};
	float m_HoleStep{};
	Point2f m_HolePos{};

	float m_SpiderSize{};
	std::vector<Point2f> m_SpiderColl{};


	float m_step{};
	float m_AccSec{};
	float m_speed{};
	float m_elapsedSec{};

	Border4f m_Steps{};

	
	
};