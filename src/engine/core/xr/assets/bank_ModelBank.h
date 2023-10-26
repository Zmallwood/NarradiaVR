/* Copyright 2023 Zmallwood
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#pragma once

namespace nar {
   /** Loads and provides 3D models used in the rendering.
    */
   class ModelBank : public Singleton<ModelBank> {
     public:
      VertexCube vertex_cube() {
         return vertex_cube_;
      }

     private:
      VertexCube vertex_cube_ = VertexCube(1.0f);
   };
}