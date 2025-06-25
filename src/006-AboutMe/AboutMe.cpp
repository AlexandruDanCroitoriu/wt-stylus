#include "006-AboutMe/AboutMe.h"
#include <Wt/WText.h>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

AboutMe::AboutMe()
{
    addNew<Wt::WText>("This is the Portofolio Page. It will contain information about my projects, skills, and experiences. Stay tuned for updates!");

    cpr::Response r = cpr::Get(cpr::Url{"https://swapi.info/api/"});
    r.status_code;                  // 200
    r.header["content-type"];       // application/json; charset=utf-8
    r.text;   

    auto star_wars_wrapper = addNew<Wt::WContainerWidget>();
    star_wars_wrapper->setStyleClass("flex flex-col");
    star_wars_wrapper->addNew<Wt::WText>("Star Wars API response status code: " + std::to_string(r.status_code));
    star_wars_wrapper->addNew<Wt::WText>("Star Wars API response content type: " + r.header["content-type"]);
    star_wars_wrapper->addNew<Wt::WText>("Star Wars API response text: " + r.text);

    nlohmann::json json_response = nlohmann::json::parse(r.text);

    for (auto it = json_response.begin(); it != json_response.end(); ++it) {
        std::string key = it.key();
        std::string value = it.value();
        star_wars_wrapper->addNew<Wt::WText>(key + ": " + value);
    }

    
    // addNew<Wt::WText>("GitHub API response cookies: " + r.;
}