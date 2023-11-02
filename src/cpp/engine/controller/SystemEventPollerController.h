/* Copyright (c) 2017-2023, The Khronos Group Inc.

PDX-License-Identifier: Apache-2.0

This file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

#pragma once

namespace nar {
    /**
     * Responsible for polling the system events.
     */
    class SystemEventPollerController : public Singleton<SystemEventPollerController> {
      public:
        void PollSystemEvents();

      private:
        const XrEventDataBaseHeader *TryReadNextEvent();
    };
}
