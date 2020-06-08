#include "Game.h"

namespace GLGame
{
	static bool init_scene_editor = true;
	static Game* GameRef = nullptr;
	static GameInternal::_GlobalGameData GameData;
	static vector<Object*> RegisterObjectQueue;
	static vector<Sprite*> RegisterSpriteQueue;
	static vector<Scene*> RegisterSceneQueue;

	void Game::Init(int w, int h, bool can_resize, string title, bool start_SE, bool use_imgui, ImGuiStyle imgui_style)
	{
		static bool GameIsAlreadyInitialized = true;

		assert(GameIsAlreadyInitialized == true); // Assert if a Game Object was already created
		GameIsAlreadyInitialized = false;

		// Initialize class variables
		init_scene_editor = start_SE;
		m_GameWindowWidth = w;
		m_GameWindowHeight = h;
		m_CanResize = can_resize;
		m_WindowTitle = title;
		m_DisplayImGui = use_imgui;
		m_IMStyle = imgui_style;

		const bool init_all_events = true;
		int callbacks_initialized = 0;

		GameRef = this; // Current Game Object

		Log::_InitLog(glfwGetTime(), __FILE__, __LINE__);

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLGAME_OPENGL_VERSION_MAJOR);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLGAME_OPENGL_VERSION_MINOR);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

		m_GameWindow = glfwCreateWindow(w, h, "Tiny OpenGL Game Engine V0.0.1", nullptr, nullptr);
		
		if (m_GameWindow == NULL)
		{
			Log::LogToFile("Failed to initialize game window");
			Log::LogToConsole("Failed to initialize game window");

			return;
		}
		
		Log::_LogWindowCreation(glfwGetTime(), m_GameWindowWidth, m_GameWindowHeight, __FILE__, __LINE__);
		glfwMakeContextCurrent(m_GameWindow);

		glewExperimental = GL_TRUE;
		glewInit();

		// This fixes a bug where glew sets the GL_INVALID_ENUM flag for some reason. real pain.
		glGetError();
		Log::_LogOpenGLInit(glfwGetTime(), GLGAME_OPENGL_VERSION_MAJOR, GLGAME_OPENGL_VERSION_MINOR, __FILE__, __LINE__);

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

		// Log the info
		char* renderer = (char*)glGetString(GL_RENDERER);
		char* vendor = (char*)glGetString(GL_VENDOR);
		char* version = (char*)glGetString(GL_VERSION);

		Log::_LogInfo(vendor, version, renderer);

		Log::_LogEventInit(glfwGetTime(), callbacks_initialized, __FILE__, __LINE__);
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

		GameData = this->_GetGlobalData();

		// Create the sprite batcher
		m_SpriteBatcher = new SpriteBatcher;
		m_CustomShaderBatcher = new SpriteBatcher;
		m_LightBatcher = new LightBatcher;

		// Set the key hold buffer
		m_KeyHoldEventBuffer = GLGameInternalGetKeyVal();

		m_DefaultObjectShader.CreateShaderProgramFromFile(GLGAME_DEFAULT_OBJECT_VERTEX, GLGAME_DEFAULT_OBJECT_FRAGMENT);
		m_DefaultBackgroundShader.CreateShaderProgramFromFile(GLGAME_DEFAULT_BGSHADER_VERTEX, GLGAME_DEFAULT_BGSHADER_FRAGMENT);

		// Register all the items from the register queues
		for (int i = 0; i < RegisterObjectQueue.size(); i++)
		{
			if (RegisterObjectQueue[i] != nullptr)
			{
				this->_RegisterObject(RegisterObjectQueue[i]);
			}
		}

		for (int i = 0; i < RegisterSpriteQueue.size(); i++)
		{
			if (RegisterSpriteQueue[i] != nullptr)
			{
				this->_RegisterSprite(RegisterSpriteQueue[i]);
			}
		}

		for (int i = 0; i < RegisterSceneQueue.size(); i++)
		{
			if (RegisterSceneQueue[i] != nullptr)
			{
				this->_RegisterScene(RegisterSceneQueue[i]);
			}
		}

		if (init_scene_editor)
		{
			m_SceneEditorWindow = SceneEditor::InitSceneEditor(&this->m_DebugInfo ,&this->m_GlobalObjects, &this->m_GlobalSprites, &this->m_ObjectItemNames, &this->m_SpriteItemNames, m_GameWindow, m_IMContext);

			if (m_SceneEditorWindow == nullptr)
			{
				init_scene_editor = false;
			}
		}

		OnInitialize(glfwGetTime());
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
		static bool first_game_render = false;

		unsigned int objects_drawn = 0;
		unsigned int sprites_drawn = 0;

		if (!first_game_render)
		{
			first_game_render = true;
			OnGameStart(glfwGetTime());
			SceneLoader();
		}

		if (!m_GameAlreadyDestroyed)
		{
			if (!glfwWindowShouldClose(m_GameWindow))
			{
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
					GLGameSetDefaults(e, m_GameWindow);

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

				// Iterate through each layer
				for (auto layer_iterator = current_scene_data.scene_items->begin(); layer_iterator != current_scene_data.scene_items->end(); layer_iterator++)
				{
					//  Iterate through every group/vector of the same objects
					for (auto e = layer_iterator->second.begin(); e != layer_iterator->second.end(); e++)
					{
						// Check if the vector is not empty
						if (e->second.size() > 0)
						{
							if (e->second[0].ItemType == sitem_type_object
								&& e->second[0].ItemObjectInstance.m_Object != nullptr)
							{
								// Update the object based on the frame 
								// Usually used for animations
								e->second[0].ItemObjectInstance.m_Object->IntUpdate(m_FpsCount);

								// Check if the object does not have a custom shader
								if (e->second[0].ItemObjectInstance.m_Object->HasShader() == false)
								{
									if (e->second[0].ItemObjectInstance.m_Object->HasSprite() &&
										e->second[0].ItemObjectInstance.m_Object->IsVisible())
									{
										for (int i = 0; i < e->second.size(); i++)
										{
											objects_drawn++;
											m_SpriteBatcher->AddGLGameItemToBatch(e->second[i]);
										}
									}
								}

								// If the object has a custom shader, use the custom sprite batcher
								else if (e->second[0].ItemObjectInstance.m_Object->HasShader() == true)
								{
									if (e->second[0].ItemObjectInstance.m_Object->HasSprite() 
										&& e->second[0].ItemObjectInstance.m_Object->IsVisible())
									{
										m_CustomShaderBatcher->StartSpriteBatch(m_CurrentScene->GetSceneCamera(), m_CurrentScene->GetSceneAmbientLight(), e->second[0].ItemObjectInstance.m_Object->GetShader());

										for (int i = 0; i < e->second.size(); i++)
										{
											objects_drawn++;
											m_CustomShaderBatcher->AddGLGameItemToBatch(e->second[i]);
										}

										m_CustomShaderBatcher->EndSpriteBatch();
									}
								}
							}

							// If it is a sprite, add it as a generic texture to the batch
							else if (e->second[0].ItemType == sitem_type_sprite
								&& e->second[0].ItemSpriteInstance.m_Sprite != nullptr)
							{
								for (int i = 0; i < e->second.size(); i++)
								{
									sprites_drawn++;
									m_SpriteBatcher->AddGenericTextureToBatch(e->second[i].ItemSpriteInstance.m_Sprite->GetCurrentTexture(), e->second[i].ItemPos);
								}
							}
						}
					}
				}

				// Update the quad count
				m_DebugInfo.QuadCount = m_SpriteBatcher->EndSpriteBatch();

				// Set the lighting blend function
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);

				m_LightBatcher->StartLightBatch(m_CurrentScene->GetSceneCamera()->GetViewProjectionMatrix());

				// Draw lights using LightBatcher
				for (int i = 0; i < current_scene_data.scene_lights->size(); i++)
				{
					// Check if the light* is not null
					if (current_scene_data.scene_lights->at(i) != nullptr)
					{
						m_LightBatcher->AddLightToBatch(*(current_scene_data.scene_lights->at(i)));
					}
				}

				// Batch the animated lights
				for (int i = 0; i < current_scene_data.scene_blinking_lights->size(); i++)
				{
					if (current_scene_data.scene_blinking_lights->at(i) != nullptr)
					{
						m_LightBatcher->AddLightToBatch(current_scene_data.scene_blinking_lights->at(i)->GetCurrentLightFrame());
						current_scene_data.scene_blinking_lights->at(i)->UpdateLightBlink(m_FpsCount);
					}
				}

				// Update the debug info
				m_DebugInfo.LightsDrawn = m_LightBatcher->EndLightBatch();
				m_DebugInfo.QuadCount += m_DebugInfo.LightsDrawn;

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

				// Display the FPS if needed

				if (m_DisplayFPS)
				{
					DisplayFrameRate(m_GameWindow, m_WindowTitle);
				}

				// Update the debug info

				m_DebugInfo.ObjectsDrawn = objects_drawn;
				m_DebugInfo.SpritesDrawn = sprites_drawn;
				m_DebugInfo.VerticesCount = m_DebugInfo.QuadCount * 4;
				m_DebugInfo.IndicesCount = m_DebugInfo.QuadCount * 6;
				m_DebugInfo.CurrentFrame = m_FpsCount;
				m_DebugInfo.RenderTime = glfwGetTime() - m_DebugInfo.CurrentTS;
				m_DebugInfo.CurrentTS = glfwGetTime();
				
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

					int fx = 0, fy = 0;

					glfwMakeContextCurrent(m_GameWindow);
					glfwGetFramebufferSize(m_GameWindow, &fx, &fy);
					glViewport(0, 0, fx, fy);
				}
			}

			else
			{
				glfwMakeContextCurrent(m_GameWindow);
				OnGameDestroy(glfwGetTime());

				glfwDestroyWindow(m_GameWindow);

				if (SceneEditor::SceneEditorAlive() == false)
				{
					glfwDestroyWindow(m_SceneEditorWindow);
					SceneEditor::_SetSceneEditorCloseFlag(true);
				}

				m_GameAlreadyDestroyed = true;
			}
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

	Sprite* Game::_GetSpriteFromArr(const string& id)
	{
		unordered_map<string, Sprite*>::iterator chk = m_GlobalSprites.find(id);

		if (chk != m_GlobalSprites.end())
		{
			return m_GlobalSprites[id];
		}

		else
		{
			stringstream str;

			str << "UNREGISTERED SPRITE REQUESTED AT LINE : " << __LINE__ << " AND FILE : " << __FILE__ << "  " << "  | " << "ID : " << id << "  ";
			Log::LogToFile(str.str());

			return nullptr;
		}
	}

	Object* Game::_GetObjectFromArr(const string& id)
	{
		unordered_map<string, Object*>::iterator chk = m_GlobalObjects.find(id);

		if (chk != m_GlobalObjects.end())
		{
			return m_GlobalObjects[id];
		}

		else
		{
			stringstream str;

			str << "UNREGISTERED OBJECT REQUESTED AT LINE : " << __LINE__ << " AND FILE : " << __FILE__ << "  | " << "ID : " << id << "  ";
			Log::LogToFile(str.str());

			return nullptr;
		}
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

		Sprite* _GetSpriteFromGlobalArray(const string& id)
		{
			return GameRef->_GetSpriteFromArr(id);
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
			if (GameRef != nullptr)
			{
				GameRef->_RegisterObject(object);
			}

			else
			{
				RegisterObjectQueue.push_back(object);
			}
		}

		void _IntRegisterSprite(Sprite* sprite)
		{
			if (GameRef != nullptr)
			{
				GameRef->_RegisterSprite(sprite);
			}

			else
			{
				RegisterSpriteQueue.push_back(sprite);
			}
		}

		void _IntRegisterScene(Scene* scene)
		{
			if (GameRef != nullptr)
			{
				GameRef->_RegisterScene(scene);
			}

			else
			{
				RegisterSceneQueue.push_back(scene);
			}
		}

		void _IntAddEventToQueue(Event e)
		{
			GameRef->_QueueEvent(e);
		}
	}
}