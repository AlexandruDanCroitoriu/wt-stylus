#include "010-TestWidgets/Test.h"
#include <Wt/WText.h>
#include <Wt/WApplication.h>
#include <Wt/WPushButton.h>
#include <Wt/WApplication.h>

Test::Test()
{ 

// arctic
// minimal
// modern
// high-contrast
// neo-brutalism
// halloween
// zombie
// pastel
// 90s
// christmas
// prototype
// news
// industrial



    auto test_nav = addWidget(std::make_unique<Wt::WTemplate>(Wt::WString::tr("test-alpine-nav")));
    
    // auto test_alpine_carouserl = addWidget(std::make_unique<Wt::WTemplate>(Wt::WString::tr("test-alpine-carousel")));
    
    // test_alpine_carouserl->bindString("img-src-1", "https://picsum.photos/200/300");
    // test_alpine_carouserl->bindString("img-src-2", "https://picsum.photos/200/300?random=1");
    // test_alpine_carouserl->bindString("img-src-3", "https://picsum.photos/200/300?random=2");
}

