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

   /**
    * Singleton template class
    */
   template <class T>
   class Singleton {
     public:
      static void Create();
      static std::shared_ptr<T> Get();

     private:
      inline static std::shared_ptr<T> instance_;
   };

   /**
    * Create new instance if none exists
    */
   template <class T>
   void Singleton<T>::Create() {
      if (!instance_) {
         instance_ = std::make_shared<T>();
         AddSingletonDisposeAction([&] { instance_.reset(); });
      }
   }

   /*
    * Get instance of object
    */
   template <class T>
   std::shared_ptr<T> Singleton<T>::Get() {
      if (!instance_) // Create new instance if none exists
         Create();
      return std::weak_ptr<T>(instance_).lock(); // Return weak_ptr to not increase ref count
   }

   void AddSingletonDisposeAction(std::function<void()> action);
   void DisposeAllSingletons();
}