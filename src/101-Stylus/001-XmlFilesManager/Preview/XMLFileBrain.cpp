#include "101-Stylus/001-XmlFilesManager/Preview/XMLFileBrain.h"

namespace Stylus
{
    XMLFileBrain::XMLFileBrain(std::shared_ptr<StylusState> state, std::string file_path)
    : state_(state),
    doc_(std::make_shared<tinyxml2::XMLDocument>())
    {
        setFile(file_path);
    }
    
    void XMLFileBrain::setFile(std::string file_path)
    {
        file_path_ = file_path;
        // Load the XML file
        tinyxml2::XMLError eResult = doc_->LoadFile(file_path.c_str());
        if (eResult != tinyxml2::XML_SUCCESS) {
            std::cout << "\n\nError loading XML file: " << doc_->ErrorIDToName(eResult) << "\n\n";
            std::cout << "File not found: " << file_path << "\n\n";
            selected_node_ = nullptr;
        }else {
            selected_node_ = doc_->RootElement();
            xml_node_selected_.emit(selected_node_, false);
        }
    }





}
