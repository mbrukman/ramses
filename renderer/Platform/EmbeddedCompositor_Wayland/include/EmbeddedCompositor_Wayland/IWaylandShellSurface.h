//  -------------------------------------------------------------------------
//  Copyright (C) 2017 BMW Car IT GmbH
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#ifndef RAMSES_IWAYLANDSHELLSURFACE_H
#define RAMSES_IWAYLANDSHELLSURFACE_H

namespace ramses_internal
{
    class IWaylandClient;
    class IWaylandResource;
    class String;

    class IWaylandShellSurface
    {
    public:
        virtual ~IWaylandShellSurface(){}
        virtual void resourceDestroyed() = 0;
        virtual void shellSurfacePong(IWaylandClient& client, uint32_t serial) = 0;
        virtual void shellSurfaceMove(IWaylandClient& client, IWaylandResource& seatResource, uint32_t serial) = 0;
        virtual void shellSurfaceResize(IWaylandClient& client, IWaylandResource& seatResource, uint32_t serial, uint32_t edges) = 0;
        virtual void shellSurfaceSetToplevel(IWaylandClient& client) = 0;
        virtual void shellSurfaceSetTransient(IWaylandClient& client, IWaylandResource& parentSurfaceResource, int32_t x, int32_t y, uint32_t flags) = 0;
        virtual void shellSurfaceSetFullscreen(IWaylandClient& client, uint32_t method, uint32_t framerate) = 0;
        virtual void shellSurfaceSetPopup(IWaylandClient& client, IWaylandResource& seatResource, uint32_t serial, IWaylandResource& parentSurfaceResource, int32_t x, int32_t y, uint32_t flags) = 0;
        virtual void shellSurfaceSetMaximized(IWaylandClient& client) = 0;
        virtual void shellSurfaceSetTitle(IWaylandClient& client, const char* title) = 0;
        virtual void shellSurfaceSetClass(IWaylandClient& client, const char* className) = 0;
        virtual void surfaceWasDeleted() = 0;
        virtual const String& getTitle() const = 0;

    };
}

#endif
