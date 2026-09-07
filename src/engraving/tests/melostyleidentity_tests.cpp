/*
 * SPDX-FileCopyrightText: 2026 Jim Plamondon
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include <array>

#include <gtest/gtest.h>

#include "global/io/buffer.h"
#include "engraving/style/style.h"

using namespace mu::engraving;

namespace {
struct StyleCase {
    Sid id;
    const char* xmlName;
    PropertyValue sample;
};

const std::array<StyleCase, 10> STYLE_CASES { {
    { Sid::meloDoLineColor, "jimsDoLineColor", muse::Color("#123456") },
    { Sid::meloMidFrameLineColor, "jimsMidFrameLineColor", muse::Color("#123456") },
    { Sid::meloJiLimit3Color, "jimsJiLimit3Color", muse::Color("#123456") },
    { Sid::meloJiLimit5Color, "jimsJiLimit5Color", muse::Color("#123456") },
    { Sid::meloJiLimit7Color, "jimsJiLimit7Color", muse::Color("#123456") },
    { Sid::meloJiLimit11Color, "jimsJiLimit11Color", muse::Color("#123456") },
    { Sid::meloChangeArrowColor, "jimsChangeArrowColor", muse::Color("#123456") },
    { Sid::meloMidBarFlankColor, "jimsMidBarFlankColor", muse::Color("#123456") },
    { Sid::meloElideEmptyOctaves, "jimsElideEmptyOctaves", true },
    { Sid::meloShowAllOctavesInFirstSystem, "jimsShowAllOctavesInFirstSystem", false },
} };
}

TEST(MeloStyleIdentity, StoredNamesKeepTheirExistingMeaning)
{
    for (const StyleCase& item : STYLE_CASES) {
        SCOPED_TRACE(item.xmlName);
        EXPECT_STREQ(MStyle::valueName(item.id), item.xmlName);
        EXPECT_EQ(MStyle::styleIdx(muse::String::fromAscii(item.xmlName)), item.id);
    }
}

TEST(MeloStyleIdentity, NonDefaultValuesSaveAndReopenWithExistingXmlNames)
{
    MStyle before;
    for (const StyleCase& item : STYLE_CASES) {
        before.set(item.id, item.sample);
    }

    muse::io::Buffer output;
    ASSERT_TRUE(output.open(muse::io::IODevice::WriteOnly));
    ASSERT_TRUE(before.write(&output));
    output.close();
    const muse::String xml = muse::String::fromUtf8(output.data());
    for (const StyleCase& item : STYLE_CASES) {
        SCOPED_TRACE(item.xmlName);
        EXPECT_TRUE(xml.contains(u"<" + muse::String::fromAscii(item.xmlName)));
    }

    muse::ByteArray bytes = output.data();
    muse::io::Buffer input(&bytes);
    ASSERT_TRUE(input.open(muse::io::IODevice::ReadOnly));
    MStyle after;
    ASSERT_TRUE(after.read(&input));
    for (const StyleCase& item : STYLE_CASES) {
        SCOPED_TRACE(item.xmlName);
        EXPECT_EQ(after.value(item.id), item.sample);
    }
}
