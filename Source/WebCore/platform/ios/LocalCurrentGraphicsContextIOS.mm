/*
 * Copyright (C) 2006-2020 Apple Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#import "config.h"
#import "LocalCurrentGraphicsContext.h"

#if PLATFORM(IOS_FAMILY)

#import <pal/ios/UIKitSoftLink.h>

namespace WebCore {

LocalCurrentContextSaver::LocalCurrentContextSaver(CGContextRef cgContext, bool)
{
    if (!cgContext) {
        ASSERT_NOT_REACHED();
        return;
    }
    if (cgContext == UIGraphicsGetCurrentContext())
        return;
    UIGraphicsPushContext(cgContext);
    m_didSetGraphicsContext = true;
}

LocalCurrentContextSaver::~LocalCurrentContextSaver()
{
    if (m_didSetGraphicsContext)
        UIGraphicsPopContext();
}

}

#endif // PLATFORM(IOS_FAMILY)
