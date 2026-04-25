#ifndef EDITORLAYER_H
#define EDITORLAYER_H

#include <GCGameEngine.h>

namespace Editor{
    class EditorLayer : public GCGameEngine::Layer{
        public:
            EditorLayer();

            void OnUpdate();
        private:
    };
}

#endif 