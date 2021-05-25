//                    ELECTRO ENGINE
// Copyright(c) 2021 - Electro Team - All rights reserved
#pragma once
#include "Core/Base.hpp"
#include "Math/BoundingBox.hpp"
#include "Renderer/Camera/EditorCamera.hpp"
#include "Scene/Components.hpp"
#include "Scene/SceneCamera.hpp"

namespace Electro
{
    class RendererDebug
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const EditorCamera& camera);
        static void EndScene();
        static void Flush();

        static void SubmitCameraFrustum(SceneCamera& camera, glm::mat4& transform, glm::vec3& pos);
        static void SubmitLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
        static void DrawAABB(const BoundingBox& aabb, const glm::mat4& transform, const glm::vec4& color);
        static void RenderGrid();
        static Pair<bool*, bool*> GetToggles();
    private:
        static void StartBatch();
        static void NextBatch();
    private:
        struct SceneData
        {
            glm::mat4 ViewProjectionMatrix;
        };

        static Scope<SceneData> mSceneData;
    };
}
