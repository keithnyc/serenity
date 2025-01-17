/*
 * Copyright (c) 2018-2020, Andreas Kling <kling@serenityos.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <AK/ByteBuffer.h>
#include <AK/URL.h>
#include <LibCore/File.h>
#include <LibWeb/DOM/Document.h>
#include <LibWeb/DOM/HTMLLinkElement.h>
#include <LibWeb/Parser/CSSParser.h>
#include <LibWeb/Loader/ResourceLoader.h>

namespace Web {

HTMLLinkElement::HTMLLinkElement(Document& document, const FlyString& tag_name)
    : HTMLElement(document, tag_name)
{
}

HTMLLinkElement::~HTMLLinkElement()
{
}

void HTMLLinkElement::inserted_into(Node&)
{
    if (rel() == "stylesheet") {
        URL url = document().complete_url(href());
        ResourceLoader::the().load(url, [&](auto data, auto&) {
            if (data.is_null()) {
                dbg() << "HTMLLinkElement: Failed to load stylesheet: " << href();
                return;
            }
            auto sheet = parse_css(data);
            if (!sheet) {
                dbg() << "HTMLLinkElement: Failed to parse stylesheet: " << href();
                return;
            }
            document().add_sheet(*sheet);
            document().update_style();
        });
    }
}

}
