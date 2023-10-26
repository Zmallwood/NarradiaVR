#pragma once

#define GET(x) (x::Get())

namespace nar {

   template <class T> class Singleton {
      //
      // Used for all classes that only need to be called once.
      //
     public:
      static void Create() {
         if (!instance_) { // Create new instance_ if none exists
            instance_ = std::make_shared<T>();
            AddSingletonDisposeAction([&] { instance_.reset(); });
         }
      }

      static std::shared_ptr<T> Get() {
         if (!instance_) // Create new instance if none exists
            Create();
         return std::weak_ptr<T>(instance_).lock(); // Return weak_ptr to not increase ref count
      }

     private:
      inline static std::shared_ptr<T> instance_;
   };
   
   //
   // Functions related to disposing of Singletons at game exit.
   //
   void AddSingletonDisposeAction(std::function<void()> action);
   void DisposeAllSingletons();
}