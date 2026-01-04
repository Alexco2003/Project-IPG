#pragma once

#include "components/primitives_rendering_scene.h"

namespace lab
{
    class Tema : public gfxc::PrimitivesRenderingScene
    {
     public:
        Tema();
        ~Tema();

        void Initialize() override;

     protected:
         void Exercise1();
         void Exercise2();
         void Exercise3();
         void Exercise4();
         void Exercise5();
         void Exercise6();
    };
}   // namespace lab
