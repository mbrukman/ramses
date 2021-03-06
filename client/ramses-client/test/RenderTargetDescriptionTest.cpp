//  -------------------------------------------------------------------------
//  Copyright (C) 2015 BMW Car IT GmbH
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#include <gtest/gtest.h>
#include "ramses-client-api/RenderBuffer.h"
#include "ramses-client-api/RenderTargetDescription.h"
#include "RenderBufferImpl.h"
#include "RenderTargetDescriptionImpl.h"
#include "ClientTestUtils.h"

using namespace testing;

namespace ramses
{
    class RenderTargetDescriptionTest : public LocalTestClientWithScene, public testing::Test
    {
    public:
    protected:
        RenderTargetDescription rtDesc;
    };

    TEST_F(RenderTargetDescriptionTest, initialState)
    {
        EXPECT_TRUE(rtDesc.impl.getRenderBuffers().empty());
    }

    TEST_F(RenderTargetDescriptionTest, validatesAsWarningIfEmpty)
    {
        EXPECT_NE(StatusOK, rtDesc.validate());
    }

    TEST_F(RenderTargetDescriptionTest, canAddRenderBuffer)
    {
        const RenderBuffer& colorRb = *m_scene.createRenderBuffer(1u, 1u, ERenderBufferType_Color, ERenderBufferFormat_RGBA8, ERenderBufferAccessMode_ReadWrite);
        EXPECT_EQ(StatusOK, rtDesc.addRenderBuffer(colorRb));

        ASSERT_EQ(1u, rtDesc.impl.getRenderBuffers().size());
        EXPECT_EQ(colorRb.impl.getRenderBufferHandle(), rtDesc.impl.getRenderBuffers()[0]);

        EXPECT_EQ(StatusOK, rtDesc.validate());
    }

    TEST_F(RenderTargetDescriptionTest, canAddMultipleColorRenderBuffers)
    {
        const RenderBuffer& colorRb1 = *m_scene.createRenderBuffer(1u, 1u, ERenderBufferType_Color, ERenderBufferFormat_RGBA8, ERenderBufferAccessMode_ReadWrite);
        const RenderBuffer& colorRb2 = *m_scene.createRenderBuffer(1u, 1u, ERenderBufferType_Color, ERenderBufferFormat_RGBA8, ERenderBufferAccessMode_ReadWrite);
        EXPECT_EQ(StatusOK, rtDesc.addRenderBuffer(colorRb1));
        EXPECT_EQ(StatusOK, rtDesc.addRenderBuffer(colorRb2));

        ASSERT_EQ(2u, rtDesc.impl.getRenderBuffers().size());
        EXPECT_EQ(colorRb1.impl.getRenderBufferHandle(), rtDesc.impl.getRenderBuffers()[0]);
        EXPECT_EQ(colorRb2.impl.getRenderBufferHandle(), rtDesc.impl.getRenderBuffers()[1]);

        EXPECT_EQ(StatusOK, rtDesc.validate());
    }

    TEST_F(RenderTargetDescriptionTest, canAddMultipleColorRenderBuffersWithDepthBuffer)
    {
        const RenderBuffer& colorRb1 = *m_scene.createRenderBuffer(1u, 1u, ERenderBufferType_Color, ERenderBufferFormat_RGBA8, ERenderBufferAccessMode_ReadWrite);
        const RenderBuffer& colorRb2 = *m_scene.createRenderBuffer(1u, 1u, ERenderBufferType_Color, ERenderBufferFormat_RGBA8, ERenderBufferAccessMode_ReadWrite);
        const RenderBuffer& depthRb = *m_scene.createRenderBuffer(1u, 1u, ERenderBufferType_Depth, ERenderBufferFormat_Depth24, ERenderBufferAccessMode_ReadWrite);
        EXPECT_EQ(StatusOK, rtDesc.addRenderBuffer(colorRb1));
        EXPECT_EQ(StatusOK, rtDesc.addRenderBuffer(colorRb2));
        EXPECT_EQ(StatusOK, rtDesc.addRenderBuffer(depthRb));

        ASSERT_EQ(3u, rtDesc.impl.getRenderBuffers().size());
        EXPECT_EQ(colorRb1.impl.getRenderBufferHandle(), rtDesc.impl.getRenderBuffers()[0]);
        EXPECT_EQ(colorRb2.impl.getRenderBufferHandle(), rtDesc.impl.getRenderBuffers()[1]);
        EXPECT_EQ(depthRb.impl.getRenderBufferHandle(), rtDesc.impl.getRenderBuffers()[2]);

        EXPECT_EQ(StatusOK, rtDesc.validate());
    }

    TEST_F(RenderTargetDescriptionTest, failsToAddSameRenderBufferTwice)
    {
        const RenderBuffer& colorRb = *m_scene.createRenderBuffer(1u, 1u, ERenderBufferType_Color, ERenderBufferFormat_RGBA8, ERenderBufferAccessMode_ReadWrite);
        EXPECT_EQ(StatusOK, rtDesc.addRenderBuffer(colorRb));
        EXPECT_NE(StatusOK, rtDesc.addRenderBuffer(colorRb));

        ASSERT_EQ(1u, rtDesc.impl.getRenderBuffers().size());
        EXPECT_EQ(colorRb.impl.getRenderBufferHandle(), rtDesc.impl.getRenderBuffers()[0]);
    }

    TEST_F(RenderTargetDescriptionTest, failsToAddRenderBufferFromAnotherScene)
    {
        const RenderBuffer& colorRb = *m_scene.createRenderBuffer(1u, 1u, ERenderBufferType_Color, ERenderBufferFormat_RGBA8, ERenderBufferAccessMode_ReadWrite);
        EXPECT_EQ(StatusOK, rtDesc.addRenderBuffer(colorRb));

        Scene& otherScene = *client.createScene(666u);
        const RenderBuffer& otherRb = *otherScene.createRenderBuffer(1u, 1u, ERenderBufferType_Color, ERenderBufferFormat_RGBA8, ERenderBufferAccessMode_ReadWrite);
        EXPECT_NE(StatusOK, rtDesc.addRenderBuffer(otherRb));

        ASSERT_EQ(1u, rtDesc.impl.getRenderBuffers().size());
        EXPECT_EQ(colorRb.impl.getRenderBufferHandle(), rtDesc.impl.getRenderBuffers()[0]);
    }

    TEST_F(RenderTargetDescriptionTest, failsToAddRenderBufferWithDifferentResolution)
    {
        const RenderBuffer& colorRb = *m_scene.createRenderBuffer(1u, 1u, ERenderBufferType_Color, ERenderBufferFormat_RGBA8, ERenderBufferAccessMode_ReadWrite);
        EXPECT_EQ(StatusOK, rtDesc.addRenderBuffer(colorRb));

        const RenderBuffer& otherRb = *m_scene.createRenderBuffer(1u, 2u, ERenderBufferType_Color, ERenderBufferFormat_RGBA8, ERenderBufferAccessMode_ReadWrite);
        EXPECT_NE(StatusOK, rtDesc.addRenderBuffer(otherRb));

        ASSERT_EQ(1u, rtDesc.impl.getRenderBuffers().size());
        EXPECT_EQ(colorRb.impl.getRenderBufferHandle(), rtDesc.impl.getRenderBuffers()[0]);
    }

    TEST_F(RenderTargetDescriptionTest, failsToAddMoreThanOneDepthBuffer)
    {
        const RenderBuffer& colorRb = *m_scene.createRenderBuffer(1u, 1u, ERenderBufferType_Color, ERenderBufferFormat_RGBA8, ERenderBufferAccessMode_ReadWrite);
        const RenderBuffer& depthRb1 = *m_scene.createRenderBuffer(1u, 1u, ERenderBufferType_Depth, ERenderBufferFormat_Depth24, ERenderBufferAccessMode_ReadWrite);
        const RenderBuffer& depthRb2 = *m_scene.createRenderBuffer(1u, 1u, ERenderBufferType_DepthStencil, ERenderBufferFormat_Depth24_Stencil8, ERenderBufferAccessMode_ReadWrite);
        EXPECT_EQ(StatusOK, rtDesc.addRenderBuffer(colorRb));
        EXPECT_EQ(StatusOK, rtDesc.addRenderBuffer(depthRb1));
        EXPECT_NE(StatusOK, rtDesc.addRenderBuffer(depthRb2));

        ASSERT_EQ(2u, rtDesc.impl.getRenderBuffers().size());
        EXPECT_EQ(colorRb.impl.getRenderBufferHandle(), rtDesc.impl.getRenderBuffers()[0]);
        EXPECT_EQ(depthRb1.impl.getRenderBufferHandle(), rtDesc.impl.getRenderBuffers()[1]);
    }

    TEST_F(RenderTargetDescriptionTest, failsToValidateAfterAddedRenderBufferDestroyed)
    {
        RenderBuffer& colorRb = *m_scene.createRenderBuffer(1u, 1u, ERenderBufferType_Color, ERenderBufferFormat_RGBA8, ERenderBufferAccessMode_ReadWrite);
        EXPECT_EQ(StatusOK, rtDesc.addRenderBuffer(colorRb));
        EXPECT_EQ(StatusOK, rtDesc.validate());

        m_scene.destroy(colorRb);
        EXPECT_NE(StatusOK, rtDesc.validate());
    }

    TEST_F(RenderTargetDescriptionTest, canAddRenderBuffersWithMsaaSampleCountNotZero)
    {
        const RenderBuffer& colorRb = *m_scene.createRenderBuffer(1u, 1u, ERenderBufferType_Color, ERenderBufferFormat_RGBA8, ERenderBufferAccessMode_WriteOnly, 4u);
        const RenderBuffer& depthRb = *m_scene.createRenderBuffer(1u, 1u, ERenderBufferType_Depth, ERenderBufferFormat_Depth24, ERenderBufferAccessMode_WriteOnly, 4u);
        EXPECT_EQ(StatusOK, rtDesc.addRenderBuffer(colorRb));
        EXPECT_EQ(StatusOK, rtDesc.addRenderBuffer(depthRb));

        ASSERT_EQ(2u, rtDesc.impl.getRenderBuffers().size());
        EXPECT_EQ(colorRb.impl.getRenderBufferHandle(), rtDesc.impl.getRenderBuffers()[0]);
        EXPECT_EQ(depthRb.impl.getRenderBufferHandle(), rtDesc.impl.getRenderBuffers()[1]);
    }

    TEST_F(RenderTargetDescriptionTest, canNotAddRenderBuffersWithDifferentMsaaSampleCount)
    {
        const RenderBuffer& colorRb = *m_scene.createRenderBuffer(1u, 1u, ERenderBufferType_Color, ERenderBufferFormat_RGBA8, ERenderBufferAccessMode_WriteOnly, 3u);
        const RenderBuffer& depthRb = *m_scene.createRenderBuffer(1u, 1u, ERenderBufferType_Depth, ERenderBufferFormat_Depth24, ERenderBufferAccessMode_WriteOnly, 4u);
        EXPECT_EQ(StatusOK, rtDesc.addRenderBuffer(colorRb));
        EXPECT_NE(StatusOK, rtDesc.addRenderBuffer(depthRb));

        ASSERT_EQ(1u, rtDesc.impl.getRenderBuffers().size());
        EXPECT_EQ(colorRb.impl.getRenderBufferHandle(), rtDesc.impl.getRenderBuffers()[0]);
    }
}
