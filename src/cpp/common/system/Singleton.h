/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once
#include <memory>

namespace nar {
    /**
     * Singleton templated class.
     */
    template <class T>
    class Singleton {
      public:
        static auto Touch() -> void;
        static auto Get() -> std::shared_ptr<T>;

      private:
        inline static std::shared_ptr<T> instance_;
    };

    /**
     * Create new instance if none exists.
     */
    template <class T>
    auto Singleton<T>::Touch() -> void {
        if (!instance_) {
            instance_ = std::make_shared<T>();
            AddSingletonDisposeAction([&] { instance_.reset(); });
        }
    }

    /**
     * Get instance of object.
     */
    template <class T>
    auto Singleton<T>::Get() -> std::shared_ptr<T> {
        if (!instance_) // Create new instance if none exists
            Touch();
        return std::weak_ptr<T>(instance_).lock(); // Return weak_ptr to not increase ref count
    }
}
