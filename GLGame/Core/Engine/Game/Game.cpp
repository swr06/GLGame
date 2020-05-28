#include "Game.h"

namespace GLGame
{
	static bool init_scene_editor = true;
	static Game* GameRef = nullptr;
	static GameInternal::_GlobalGameData GameData;

	void Game::Init(int w, int h, bool can_resize, string title, bool use_imgui, ImGuiStyle imgui_style)
	{
		static bool GameIsAlreadyInitialized = true;

		assert(GameIsAlreadyInitialized == true); // Assert if a Game Object was already created
		GameIsAlreadyInitialized = false;

		const bool init_all_events = true;
		int callbacks_initialized = 0;

		GameRef = this; // Current Game Object

		m_GameWindowWidth = w;
		m_GameWindowHeight = h;

		Log::_InitLog(glfwGetTime(), __FILE__, __LINE__);

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLGAME_OPENGL_VERSION_MAJOR);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLGAME_OPENGL_VERSION_MINOR);
		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

		m_GameWindow = glfwCreateWindow(w, h, "Tiny OpenGL Game Engine V0.0.1", nullptr, nullptr);
		Log::_LogWindowCreation(glfwGetTime(), m_GameWindowWidth, m_GameWindowHeight, __FILE__, __LINE__);
		glfwMakeContextCurrent(m_GameWindow);

		glewExperimental = GL_TRUE;
		glewInit();

		// This fixes a bug where glew sets the GL_INVALID_ENUM flag for some reason. real pain.
		glGetError();
		Log::_LogOpenGLInit(glfwGetTime(), GLGAME_OPENGL_VERSION_MINOR, GLGAME_OPENGL_VERSION_MAJOR, __FILE__, __LINE__);

		glViewport(0, 0, m_GameWindowWidth, m_GameWindowHeight);

		m_DisplayImGui = false;
		m_ImGuiInitialized = false;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (use_imgui)
		{
			m_ImGuiInitialized = true;
			m_DisplayImGui = true;
			m_IMContext = ImGui::CreateContext();
			ImGui::SetCurrentContext(m_IMContext);

			ImGui_ImplGlfw_InitForOpenGL(m_GameWindow, true);
			ImGui_ImplOpenGL3_Init((const char*)"#version 130");

			switch (imgui_style)
			{
			case ImGuiStyleDark:
				ImGui::StyleColorsDark();
				break;

			case ImGuiStyleLight:
				ImGui::StyleColorsLight();
				break;

			case ImGuiStyleClassic:
				ImGui::StyleColorsClassic();
				break;

			default:
				ImGui::StyleColorsClassic();
				break;
			}

			ImGuiIO& io = ImGui::GetIO();
			io.Fonts->AddFontDefault();
			io.Fonts->Build();
		}

		// Initialize the key event buffer and set the key callbacks
		GLGameEventInit();
		glfwSetKeyCallback(m_GameWindow, GLGameKeyCallback);
		glfwSetMouseButtonCallback(m_GameWindow, GLGameMouseCallback);
		glfwSetScrollCallback(m_GameWindow, GLGameScrollCallback);
		glfwSetDropCallback(m_GameWindow, GLGameDropCallback);
		glfwSetCursorPosCallback(m_GameWindow, GLGameCursorPosCallback);
		glfwSetFramebufferSizeCallback(m_GameWindow, GLGameFramebufferSizeCallback);
		glfwSetWindowPosCallback(m_GameWindow, GLGameWindowPositionCallback);
		glfwSetWindowMaximizeCallback(m_GameWindow, GLGameWindowMaximizedCallback);

		callbacks_initialized += 8;

		if (init_all_events)
		{
			glfwSetWindowSizeCallback(m_GameWindow, GLGameWindowSizeCallback);
			glfwSetCursorEnterCallback(m_GameWindow, GLGameCursorEnterCallback);
			glfwSetWindowCloseCallback(m_GameWindow, GLGameWindowCloseCallback);
			glfwSetWindowFocusCallback(m_GameWindow, GLGameWindowFocusCallback);
			glfwSetWindowIconifyCallback(m_GameWindow, GLGameWindowIconifiedCallback);

			callbacks_initialized += 5;
		}

		Log::_LogEventInit(glfwGetTime(), callbacks_initialized, __FILE__, __LINE__);
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

		GameData = this->_GetGlobalData();

		// Create the sprite batcher
		m_SpriteBatcher = new SpriteBatcher;
		m_LightBatcher = new LightBatcher;

		// Set the key hold buffer
		m_KeyHoldEventBuffer = GLGameInternalGetKeyVal();

		m_DefaultObjectShader.CreateShaderProgram(GLGAME_DEFAULT_OBJECT_VERTEX, GLGAME_DEFAULT_OBJECT_FRAGMENT);
		m_DefaultBackgroundShader.CreateShaderProgram(GLGAME_DEFAULT_BGSHADER_VERTEX, GLGAME_DEFAULT_BGSHADER_FRAGMENT);
	
		if (init_scene_editor)
		{
			if (SceneEditor::InitSceneEditor(&this->m_GlobalObjects, &this->m_GlobalSprites, &this->m_ObjectItemNames, &this->m_SpriteItemNames, m_GameWindow, m_IMContext) == nullptr)
			{
				init_scene_editor = false;
			}
		}
	}

	void Game::StartSceneEditor()
	{

	}

	Game::~Game()
	{
		// ToDo : Handle if GameRef = nullptr
		GameRef = nullptr;

		if (m_ImGuiInitialized)
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}

		glfwDestroyWindow(m_GameWindow);
		glfwTerminate();
	}

	void Game::Render(bool should_clear)
	{
		AABB camera_cull;
		glm::vec3 camera_pos;
		glm::vec4 camera_projection_coords;

		if (should_clear)
		{
			glClear(GL_COLOR_BUFFER_BIT);
		}

		glfwMakeContextCurrent(m_GameWindow);
		
		glfwPollEvents();
		PollEvents();

		// Handling Event_Key events (or key holds)
	
		{
			Event e;

			e.EventType = Event_Key;
			//GLGameSetDefaults(e, m_GameWindow);

			for (int i = 0; i < GLGAME_INT_GLFW_KEY_COUNT; i++)
			{
				if (m_KeyHoldEventBuffer[i])
				{
					e.KeyCode = i;
					m_EventQueue.push_back(e);
				}
			}
		}

		// Execute the frame advance callback
		OnFrameAdvance(m_FpsCount);

		// ImGui
		{
			if (m_DisplayImGui)
			{
				ImGui::SetCurrentContext(m_IMContext);
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();
				OnImGuiRender(m_FpsCount);
			}
		}

		// Setting up some variables
		camera_pos = m_CurrentScene->GetSceneCamera()->GetPosition();
		camera_projection_coords = m_CurrentScene->GetSceneCamera()->GetProjectionCoords();
		camera_cull.x = camera_pos.x;
		camera_cull.y = camera_pos.y;

		camera_cull.w = camera_projection_coords.y - camera_pos.x;
		camera_cull.h = camera_projection_coords.w - camera_pos.y;

		// Rendering the background data items

		_SceneData current_scene_data = m_CurrentScene->IntGetSceneData();

		int window_width = 0, window_height = 0;

		glfwGetFramebufferSize(m_GameWindow, &window_width, &window_height);

		for (auto background_iterator = current_scene_data.scene_backgrounds->begin(); background_iterator != current_scene_data.scene_backgrounds->end(); background_iterator++)
		{
			if (background_iterator->second.background != nullptr)
			{
				if (background_iterator->second.background->MovesWithCamera())
				{
					NormallyRenderBackgrounds(&background_iterator->second,
						m_DefaultBackgroundShader, window_width, window_height, glm::mat4(1.0f),
						glm::mat4(1.0f), m_CurrentScene->GetSceneCamera()->GetViewProjectionMatrix());
				}

				else
				{
					NormallyRenderBackgrounds(&background_iterator->second,
						m_DefaultBackgroundShader, window_width, window_height, glm::mat4(1.0f),
						glm::mat4(1.0f), m_CurrentScene->GetSceneCamera()->GetProjectionMatrix());
				}
			}
		}

		//Rendering the scene items

		// Get the scene data

		// Goes through each layer and batches a group of objects and then draws it 

		m_SpriteBatcher->StartSpriteBatch(m_CurrentScene->GetSceneCamera(), m_CurrentScene->GetSceneAmbientLight());

		for (auto layer_iterator = current_scene_data.scene_items->begin(); layer_iterator != current_scene_data.scene_items->end(); layer_iterator++)
		{
			for (auto e = layer_iterator->second.begin(); e != layer_iterator->second.end(); e++)
			{
				if (e->second.size() > 0)
				{
					if (e->second[0].ItemType == sitem_type_object && e->second[0].ItemObjectInstance.m_Object != nullptr)
					{
						e->second[0].ItemObjectInstance.m_Object->IntUpdate(m_FpsCount);

						if (e->second[0].ItemObjectInstance.m_Object->HasShader() == false)
						{
							if (e->second[0].ItemObjectInstance.m_Object->HasSprite() && e->second[0].ItemObjectInstance.m_Object->IsVisible())
							{
								for (int i = 0; i < e->second.size(); i++)
								{
									m_SpriteBatcher->AddGLGameItemToBatch(e->second[i]);
								}
							}
						}

						else
						{
							if (e->second[0].ItemObjectInstance.m_Object->HasSprite() && e->second[0].ItemObjectInstance.m_Object->IsVisible())
							{
								// TODO!

								//BatchRenderObjectInstances(e->second, *(e->second[0].ItemObjectInstance.m_Object->GetShader()), e->second[0].ItemObjectInstance.m_Object->GetModelMatrix(), m_CurrentScene->GetSceneCamera()->GetTransformMatrix(), m_CurrentScene->GetSceneCamera()->GetViewProjectionMatrix(), m_CurrentScene->GetSceneCamera()->GetScale(), camera_cull);
							}
						}
					}

					else if (e->second[0].ItemType == sitem_type_sprite && e->second[0].ItemSpriteInstance.m_Sprite != nullptr)
					{
						// Render the layer's sprites
					}
				}
			}
		}

		m_SpriteBatcher->EndSpriteBatch();

		double mx, my;
		int w = 0, h = 0;
		glfwGetCursorPos(m_GameWindow, &mx, &my);
		glfwGetFramebufferSize(m_GameWindow, &w, &h);

		// Set the lighting blend function
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

 		m_LightBatcher->StartLightBatch(m_CurrentScene->GetSceneCamera()->GetViewProjectionMatrix());

		// Draw lights 
		for (int i = 0; i < current_scene_data.scene_lights->size(); i++)
		{
			if (current_scene_data.scene_lights->at(i) != nullptr)
			{
				m_LightBatcher->AddLightToBatch(*(current_scene_data.scene_lights->at(i)));
			}
		}

		// Draw lights 
		for (int i = 0; i < current_scene_data.scene_blinking_lights->size(); i++)
		{
			if (current_scene_data.scene_blinking_lights->at(i) != nullptr)
			{
				m_LightBatcher->AddLightToBatch(current_scene_data.scene_blinking_lights->at(i)->GetCurrentLightFrame());
				current_scene_data.scene_blinking_lights->at(i)->UpdateLightBlink(m_FpsCount);
			}
		}

		m_LightBatcher->EndLightBatch();

		// Revert the lighting blend function


		// TODO : REVERT IT TO THE USER DEFINED BLEND FUNCTION
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// ImGui
		{
			if (m_DisplayImGui)
			{
				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			}
		}

		if (m_DisplayFPS)
		{
			DisplayFrameRate(m_GameWindow);
		}

		m_FpsCount += 1;

		glfwSwapBuffers(m_GameWindow);

		if (init_scene_editor)
		{
			if (SceneEditor::RenderSceneEditor() == false)
			{
				// For some reason, The vsync suddenly goes off when the scene editor closes.
				// This fixes the bug
				SetVSync(m_Vsync, false);
			}

			glfwMakeContextCurrent(m_GameWindow);
		}

		Log::_Log();
	}

	void Game::SetCurrentScene(Scene& scene)
	{
		m_CurrentScene = &scene;
	}

	Scene& Game::GetCurrentScene()
	{
		return *m_CurrentScene;
	}

	Camera* Game::GetCurrentCamera()
	{
		return m_CurrentScene->GetSceneCamera();
	}

	void Game::ConvertCoordinates(glm::vec3 coordinates, glm::vec3 range)
	{

	}

	void Game::SetVSync(bool vsync, bool log)
	{
		if (vsync)
		{
			m_Vsync = true;
			glfwSwapInterval(1);
		}

		else
		{
			m_Vsync = false;
			glfwSwapInterval(0);
		}

		if (log)
		{
			Log::_LogVSyncInit(glfwGetTime(), vsync, __FILE__, __LINE__);
		}
	}

	bool Game::GameWindowShouldClose()
	{
		return glfwWindowShouldClose(m_GameWindow);
	}

	void Game::SetBlend(bool blend)
	{
		if (blend)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		else
		{
			glDisable(GL_BLEND);
		}
	}

	void Game::SetBlendFunction(GLenum blendfuncS, GLenum blendfuncD)
	{
		glBlendFunc(blendfuncS, blendfuncD);
	}

	// To test the collision between 2 objects
	bool Game::IsThereCollision(Object& obj_1, Object& obj_2)
	{
		_SceneData curr_scene_data = m_CurrentScene->IntGetSceneData();
		bool return_val = false;

		// Reference the scene data
		map<int, unordered_map<string, vector<SceneDataItem>>>& scene_data = *(curr_scene_data.scene_items);
		const string& obj1_id = obj_1.GetObjectID();
		const string& obj2_id = obj_2.GetObjectID();
		int map_element_size = scene_data.size();
		
		bool specific_layer = true;
		vector<SceneDataItem*> obj1_item_list;
		vector<SceneDataItem*> obj2_item_list;

		AABB obj1_AABB;
		AABB obj2_AABB;

		// Get all the respective objects from the scene
		{ 
			// Object 1
			for (auto e = scene_data.begin(); e != scene_data.end(); ++e)
			{
				// Check if the element exists

				if (e->second.find(obj1_id) != e->second.end())
				{
					vector<SceneDataItem>& objs_1 = e->second.at(obj1_id);

					for (int i = 0; i < objs_1.size(); i++)
					{
						obj1_item_list.push_back(&objs_1[i]);
					}
				}
			}

			// Object 2
			for (auto e = scene_data.begin(); e != scene_data.end(); e++)
			{
				// Check if the element exists

				if (e->second.find(obj2_id) != e->second.end())
				{
					vector<SceneDataItem>& objs_2 = e->second.at(obj2_id);

					for (int i = 0; i < objs_2.size(); i++)
					{
						obj2_item_list.push_back(&objs_2[i]);
					}
				}
			}
		}
		
		if (obj1_item_list.size() > 0 && obj2_item_list.size() > 0)
		{
			// If both of the collision masks are rects, use the AABB collision method

			if (obj1_item_list[0]->ItemObjectInstance.m_Object->GetCollisionMaskType() == mask_rect &&
				obj2_item_list[0]->ItemObjectInstance.m_Object->GetCollisionMaskType() == mask_rect)
			{
				// Get each of their bounding boxes

				glm::vec4 obj1_bounding_box = obj1_item_list[0]->ItemObjectInstance.m_Object->GetBoundingBox();
				glm::vec4 obj2_bounding_box = obj2_item_list[0]->ItemObjectInstance.m_Object->GetBoundingBox();

				for (int i = 0; i < obj1_item_list.size(); i++)
				{
					obj1_AABB.x = obj1_item_list[i]->ItemPos.x + obj1_bounding_box.x;
					obj1_AABB.y = obj1_item_list[i]->ItemPos.y + obj1_bounding_box.y;
					obj1_AABB.w = obj1_bounding_box.z;
					obj1_AABB.h = obj1_bounding_box.w;

					for (int j = 0; j < obj2_item_list.size(); j++)
					{
						obj2_AABB.x = obj2_item_list[j]->ItemPos.x + obj2_bounding_box.x;
						obj2_AABB.y = obj2_item_list[j]->ItemPos.y + obj2_bounding_box.y;
						obj2_AABB.w = obj2_bounding_box.z;
						obj2_AABB.h = obj2_bounding_box.w;

						if (CheckAABBCollision(obj1_AABB, obj2_AABB))
						{
							return_val = true;
							break;
						}
					}
				}
			}
		}

		else
		{
			return_val = false;
		}

		return return_val;
	}

	void Game::DisplayFpsOnWindowTitleBar(bool display)
	{
		m_DisplayFPS = display;
	}

	bool Game::KeyIsBeingPressed(int Key)
	{
		return m_KeyHoldEventBuffer[Key];
	}

	void Game::PollEvents()
	{
		for (int i = 0; i < m_EventQueue.size(); i++)
		{
			OnEvent(m_EventQueue[i]);
			m_EventQueue.erase(m_EventQueue.begin() + i);
		}
	}

	// Register functions

	void Game::_RegisterObject(Object* object)
	{
		m_ObjectItemNames.push_back(object->GetObjectID());
		m_GlobalObjects[object->GetObjectID()] = object;
	}

	void Game::_RegisterSprite(Sprite* sprite)
	{
		m_SpriteItemNames.push_back(sprite->GetSpriteID());
		m_GlobalSprites[sprite->GetSpriteID()] = sprite;
	}

	void Game::_RegisterScene(Scene* scene)
	{
		m_GlobalScenes[scene->GetSceneID()] = scene;
	}

	void Game::_QueueEvent(Event e)
	{
		m_EventQueue.push_back(e);
	}

	GameInternal::_GlobalGameData Game::_GetGlobalData()
	{
		GameInternal::_GlobalGameData data;

		data.gBackgrounds = &m_GlobalBackgrounds;
		data.gObjects = &m_GlobalObjects;
		data.gSprites = &m_GlobalSprites;
		data.gScenes = &m_GlobalScenes;

		return data;
	}

	Object* Game::_GetObjectFromArr(const string& id)
	{
		return m_GlobalObjects[id];
	}

	// Internal Functions

	namespace GameInternal
	{
		void _UpdateObjectPosition(const string& id, const glm::vec3& pos)
		{
			_SceneData curr_scene_data = GameRef->GetCurrentScene().IntGetSceneData();

			map<int, std::unordered_map<string, vector<SceneDataItem>>>& scene_data = *(curr_scene_data.scene_items);
			size_t size = 0;

			for (auto layer = scene_data.begin(); layer != scene_data.end(); layer++)
			{
				size = layer->second[id].size();

				for (int element = 0; element < size; element++)
				{
					layer->second[id][element].ItemPos = pos;
				}
			}
		}

		Object* _GetObjectFromGlobalArray(const string& id)
		{
			return GameRef->_GetObjectFromArr(id);
		}

		void _GetObjectPosition(const string& id, uint32_t instance_id, int layer, glm::vec3& pos)
		{
			_SceneData curr_scene_data = GameRef->GetCurrentScene().IntGetSceneData();

			map<int, std::unordered_map<string, vector<SceneDataItem>>>& scene_data = *(curr_scene_data.scene_items);
			pos = scene_data[layer][id][instance_id].ItemPos;
		}

		void _IncrementObjectPosition(const string& id, const glm::vec3& amt)
		{
			_SceneData curr_scene_data = GameRef->GetCurrentScene().IntGetSceneData();

			map<int, std::unordered_map<string, vector<SceneDataItem>>>& scene_data = *(curr_scene_data.scene_items);
			size_t size = 0;

			for (auto layer = scene_data.begin(); layer != scene_data.end(); layer++)
			{
				size = layer->second[id].size();

				for (int element = 0; element < size; element++)
				{
					layer->second[id][element].ItemPos.x += amt.x;
					layer->second[id][element].ItemPos.y += amt.y;
					layer->second[id][element].ItemPos.z += amt.z;
				}
			}
		}

		void _SetBackgroundPosition(uint32_t id, const glm::vec3& pos)
		{
			_SceneData curr_scene_data = GameRef->GetCurrentScene().IntGetSceneData();

			map<int, SceneBackground>& background_data = *(curr_scene_data.scene_backgrounds);

			for (auto e = background_data.begin(); e != background_data.end(); e++)
			{
				if (e->second.background->GetBackgroundID() == id)
				{
					e->second.position = pos;
				}
			}
		}

		void _IncrementBackgroundPosition(uint32_t id, const glm::vec3& increment)
		{
			_SceneData curr_scene_data = GameRef->GetCurrentScene().IntGetSceneData();

			map<int, SceneBackground>& background_data = *(curr_scene_data.scene_backgrounds);

			for (auto e = background_data.begin(); e != background_data.end(); e++)
			{
				if (e->second.background->GetBackgroundID() == id)
				{
					e->second.position.x += increment.x;
					e->second.position.y += increment.y;
					e->second.position.z += increment.z;
				}
			}
		}

		// Register Functions 

		void _IntRegisterObject(Object* object)
		{
			GameRef->_RegisterObject(object);
		}

		void _IntRegisterSprite(Sprite* sprite)
		{
			GameRef->_RegisterSprite(sprite);
		}

		void _IntRegisterScene(Scene* scene)
		{
			GameRef->_RegisterScene(scene);
		}

		void _IntAddEventToQueue(Event e)
		{
			GameRef->_QueueEvent(e);
		}
	}
}