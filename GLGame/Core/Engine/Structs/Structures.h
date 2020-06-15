#pragma once

namespace GLGame
{
	// Info returned by SpriteBatcher, LightBatcher and ParticleBatcher

	struct BatcherInfo
	{
		int m_QuadCount = 0;
		int m_DrawCalls = 0;
		int m_MaximumQuads = 0;
	};

	struct GameDebugInfo
	{
		long VerticesCount = 0;
		long IndicesCount = 0;
		unsigned int QuadCount = 0;
		long long CurrentFrame = 0;
		double CurrentTS = 0;
		unsigned int ObjectsDrawn = 0;
		unsigned int SpritesDrawn = 0;
		unsigned int ParticlesDrawn = 0;
		unsigned int LightsDrawn = 0;
		unsigned int TotalQuads = 0;
		unsigned int TotalVertices = 0;
		unsigned int TotalIndices = 0;
		unsigned int TotalDrawCalls = 0;
		double RenderTime;
	};

	namespace SceneEditor
	{

	}
}