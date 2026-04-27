#ifndef EDITORLAYER_H
#define EDITORLAYER_H

#include <GCGameEngine.h>

namespace Editor{
    class EditorLayer : public GCGameEngine::Layer{
        public:
            EditorLayer();
            ~EditorLayer();

            void OnUpdate();

            static GCGameEngine::Scene* scene;
    };
}

#endif 