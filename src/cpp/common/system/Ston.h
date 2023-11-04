/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once
#include <memory>

namespace nar {
    /**
     * Singleton templated class.
     */
    template <class T>
    class Ston {
      public:
        static void Touch();
        static std::shared_ptr<T> Get();

      private:
        inline static std::shared_ptr<T> instance_;
    };

    /**
     * Create new instance if none exists.
     */
    template <class T>
    void Ston<T>::Touch() {
        if (!instance_) {
            instance_ = std::make_shared<T>();
            AddSingletonDisposeAction([&] { instance_.reset(); });
        }
    }

    /**
     * Get instance of object.
     */
    template <class T>
    std::shared_ptr<T> Ston<T>::Get() {
        if (!instance_) // Create new instance if none exists
            Touch();
        return std::weak_ptr<T>(instance_).lock(); // Return weak_ptr to not increase ref count
    }
}
