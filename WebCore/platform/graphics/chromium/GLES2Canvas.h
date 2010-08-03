/*
 * Copyright (c) 2010, Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef GLES2Canvas_h
#define GLES2Canvas_h

#if USE(GLES2_RENDERING)

#include "AffineTransform.h"
#include "Color.h"
#include "ColorSpace.h"
#include "GLES2Texture.h"
#include "GraphicsTypes.h"
#include "ImageSource.h"

#include <wtf/HashMap.h>
#include <wtf/Noncopyable.h>

namespace WebCore {

class FloatRect;
class Color;
class GLES2Context;

typedef HashMap<NativeImagePtr, RefPtr<GLES2Texture> > TextureHashMap;

class GLES2Canvas : public Noncopyable {
public:
    GLES2Canvas(GLES2Context*, const IntSize&);
    ~GLES2Canvas();

    void fillRect(const FloatRect&, const Color&, ColorSpace);
    void fillRect(const FloatRect&);
    void clearRect(const FloatRect&);
    void setFillColor(const Color&, ColorSpace);
    void setAlpha(float alpha);
    void setCompositeOperation(CompositeOperator);
    void translate(float x, float y);
    void rotate(float angleInRadians);
    void scale(const FloatSize&);
    void concatCTM(const AffineTransform&);

    void save();
    void restore();

    // non-standard functions
    // These are not standard GraphicsContext functions, and should be pushed
    // down into a PlatformContextGLES2 at some point.
    void drawTexturedRect(GLES2Texture*, const FloatRect& srcRect, const FloatRect& dstRect, const AffineTransform&, float alpha, ColorSpace, CompositeOperator);
    void drawTexturedRect(GLES2Texture*, const FloatRect& srcRect, const FloatRect& dstRect, ColorSpace, CompositeOperator);
    GLES2Context* gles2Context() { return m_gles2Context; }
    GLES2Texture* createTexture(NativeImagePtr, GLES2Texture::Format, int width, int height);
    GLES2Texture* getTexture(NativeImagePtr);

private:
    void applyCompositeOperator(CompositeOperator);
    void checkGLError(const char* header);
    unsigned getQuadVertices();
    unsigned getQuadIndices();
    unsigned getSimpleProgram();
    unsigned getTexProgram();

    GLES2Context* m_gles2Context;
    struct State;
    WTF::Vector<State> m_stateStack;
    State* m_state;
    unsigned m_quadVertices;
    unsigned m_quadIndices;
    unsigned m_simpleProgram;
    unsigned m_texProgram;
    int m_simpleMatrixLocation;
    int m_simpleColorLocation;
    int m_simplePositionLocation;
    int m_texMatrixLocation;
    int m_texTexMatrixLocation;
    int m_texSamplerLocation;
    int m_texAlphaLocation;
    int m_texPositionLocation;
    AffineTransform m_flipMatrix;
    TextureHashMap m_textures;
    CompositeOperator m_lastCompositeOp; // This is the one last set, not necessarily the one in the state stack.
};

}

#endif

#endif // GLES2Canvas_h