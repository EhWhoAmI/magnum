/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013 Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

#include <TestSuite/Tester.h>

#include "DebugTools/Implementation/CapsuleRendererTransformation.h"

namespace Magnum { namespace DebugTools { namespace Test {

class CapsuleRendererTest: public TestSuite::Tester {
    public:
        explicit CapsuleRendererTest();

        void zeroLength2D();
        void common2D();

        void zeroLength3D();
        void common3D();
};

CapsuleRendererTest::CapsuleRendererTest() {
    addTests({&CapsuleRendererTest::zeroLength2D,
              &CapsuleRendererTest::common2D,

              &CapsuleRendererTest::zeroLength3D,
              &CapsuleRendererTest::common3D});
}

void CapsuleRendererTest::zeroLength2D() {
    const Vector2 a(0.5f, 3.0f);
    std::array<Matrix3, 3> transformation = Implementation::capsuleRendererTransformation<2>(a, a, 3.5f);

    const auto scaling = Math::Matrix<2, Float>::fromDiagonal(Vector2(3.5f));
    CORRADE_COMPARE(transformation[0].rotationScaling(), scaling);
    CORRADE_COMPARE(transformation[1].rotationScaling(), (Math::Matrix<2, Float>::fromDiagonal({3.5f, 0.0f})));
    CORRADE_COMPARE(transformation[2].rotationScaling(), scaling);

    CORRADE_COMPARE(transformation[0].translation(), a);
    CORRADE_COMPARE(transformation[1].translation(), a);
    CORRADE_COMPARE(transformation[2].translation(), a);
}

void CapsuleRendererTest::common2D() {
    const Vector2 a(0.5f, 3.0f);
    const Vector2 b(7.5f, -1.0f);
    std::array<Matrix3, 3> transformation = Implementation::capsuleRendererTransformation<2>(a, b, 3.5f);

    /* Vector from capsule center to top hemisphere center */
    const Vector2 up(3.5f, -2.0f);
    CORRADE_COMPARE(transformation[0].up(), up.resized(3.5f));
    CORRADE_COMPARE(transformation[1].up(), up);
    CORRADE_COMPARE(transformation[2].up(), up.resized(3.5f));

    const Vector2 right = Vector2(4.0f, 7.0f).resized(3.5f);
    CORRADE_COMPARE(transformation[0].right(), right);
    CORRADE_COMPARE(transformation[1].right(), right);
    CORRADE_COMPARE(transformation[2].right(), right);

    /* Orthogonality */
    CORRADE_COMPARE(Vector2::dot(transformation[0].up(), transformation[0].right()), 0.0f);

    const Vector2 capDistance = up.resized(3.5f);
    CORRADE_COMPARE(transformation[0].translation(), a+capDistance);
    CORRADE_COMPARE(transformation[1].translation(), 0.5f*(a + b));
    CORRADE_COMPARE(transformation[2].translation(), b-capDistance);
}

void CapsuleRendererTest::zeroLength3D() {
    const Vector3 a(0.5f, 3.0f, 7.0f);
    std::array<Matrix4, 3> transformation = Implementation::capsuleRendererTransformation<3>(a, a, 3.5f);

    const auto scaling = Math::Matrix<3, Float>::fromDiagonal(Vector3(3.5f));
    CORRADE_COMPARE(transformation[0].rotationScaling(), scaling);
    CORRADE_COMPARE(transformation[1].rotationScaling(), (Math::Matrix<3, Float>::fromDiagonal({3.5f, 0.0f, 3.5f})));
    CORRADE_COMPARE(transformation[2].rotationScaling(), scaling);

    CORRADE_COMPARE(transformation[0].translation(), a);
    CORRADE_COMPARE(transformation[1].translation(), a);
    CORRADE_COMPARE(transformation[2].translation(), a);
}

void CapsuleRendererTest::common3D() {
    const Vector3 a(0.5f, 3.0f, 7.0f);
    const Vector3 b(7.5f, -1.0f, 1.5f);
    std::array<Matrix4, 3> transformation = Implementation::capsuleRendererTransformation<3>(a, b, 3.5f);

    /* Vector from capsule center to top hemisphere center */
    const Vector3 up(3.5f, -2.0f, -2.75f);
    CORRADE_COMPARE(transformation[0].up(), up.resized(3.5f));
    CORRADE_COMPARE(transformation[1].up(), up);
    CORRADE_COMPARE(transformation[2].up(), up.resized(3.5f));

    const auto right = Vector3(-2.0f, -3.5f, 0.0f).resized(3.5f);
    CORRADE_COMPARE(transformation[0].right(), right);
    CORRADE_COMPARE(transformation[1].right(), right);
    CORRADE_COMPARE(transformation[2].right(), right);

    const auto backward = Vector3(9.625f, -5.5f, 16.25f).resized(3.5f);
    CORRADE_COMPARE(transformation[0].backward(), backward);
    CORRADE_COMPARE(transformation[1].backward(), backward);
    CORRADE_COMPARE(transformation[2].backward(), backward);

    /* Orthogonality */
    CORRADE_COMPARE(Vector3::dot(transformation[0].up(), transformation[0].right()), 0.0f);
    CORRADE_COMPARE(Vector3::dot(transformation[0].up(), transformation[0].backward()), 0.0f);
    CORRADE_COMPARE(Vector3::dot(transformation[0].right(), transformation[0].backward()), 0.0f);

    const Vector3 capDistance = up.resized(3.5f);
    CORRADE_COMPARE(transformation[0].translation(), a+capDistance);
    CORRADE_COMPARE(transformation[1].translation(), 0.5f*(a + b));
    CORRADE_COMPARE(transformation[2].translation(), b-capDistance);
}

}}}

CORRADE_TEST_MAIN(Magnum::DebugTools::Test::CapsuleRendererTest)
