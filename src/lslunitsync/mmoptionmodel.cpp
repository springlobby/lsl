/* This file is part of the Springlobby (GPL v2 or later), see COPYING */

#include "mmoptionmodel.h"

namespace LSL {

mmOptionModel::mmOptionModel(std::string name_, std::string key_, std::string description_, Enum::OptionType type_,
                             std::string section_ , std::string style_ )
	: name(name_),key(key_),description(description_),type(type_),
      section(section_),ct_type_string(style_)
{
    //set style according to input string
    if ( style_ == "yaadda" )//TODO(koshi) the fuck?
		ct_type = Enum::oct_someothers;
    else
		ct_type = Enum::oct_undefined; //lobby will chooose best fit

    if ( section == std::string() )
        section = Constants::nosection_name;

    if ( ct_type_string == std::string() )
        ct_type_string  = Constants::nostyle_name;

}

mmOptionModel::mmOptionModel(std::string name_, std::string key_, std::string description_, Enum::OptionType type_,
							 std::string section_ , Enum::OptionControlType style_ ):
	name(name_),
	key(key_),
	description(description_),
	type(type_),
	ct_type(style_),
	section(section_)
{
}

mmOptionModel::~mmOptionModel()
{}

mmOptionModel::mmOptionModel():
	name(""),
	key(""),
	description(""),
	type(Enum::opt_undefined),
	ct_type(Enum::oct_undefined),
	section(Constants::nosection_name)
{
}

mmOptionBool::mmOptionBool(std::string name_, std::string key_, std::string description_, bool def_,
                           std::string section_ , std::string style_):
	mmOptionModel(name_,key_,description_,Enum::opt_bool,section_,style_),
	def(def_),value(def_)
{}

mmOptionBool::mmOptionBool():mmOptionModel()
{
	value = false;
	def = value;
}

mmOptionFloat::mmOptionFloat(std::string name_, std::string key_, std::string description_, float def_, float stepping_, float min_, float max_,
                             std::string section_ , std::string style_):
	mmOptionModel(name_,key_,description_,Enum::opt_float,section_,style_),
	def(def_),value(def_),stepping(stepping_),min(min_),max(max_)
{}

mmOptionFloat::mmOptionFloat() : mmOptionModel(),
	def(0.0), value(0.0), stepping(0.0), min(0.0), max(0.0)
{}

mmOptionString::mmOptionString(std::string name_, std::string key_, std::string description_, std::string def_, unsigned int max_len_,
                               std::string section_ , std::string style_):
	mmOptionModel(name_,key_,description_,Enum::opt_string,section_,style_),
	def(def_),value(def_),max_len(max_len_)
{}

mmOptionString::mmOptionString():
	mmOptionModel(),
	def(""),
	max_len(0)
{
	value = def;
}

mmOptionList::mmOptionList(std::string name_, std::string key_, std::string description_, std::string def_,
                           std::string section_ , std::string style_):
	mmOptionModel(name_,key_,description_,Enum::opt_list,section_,style_),
	def(def_),value(def_)
{
	cur_choice_index = 0;
}

mmOptionList::mmOptionList():
	mmOptionModel(),
	def(""),
	value(""),
	cur_choice_index(0)
{
}

void mmOptionList::addItem(std::string key_, std::string name_, std::string desc_)
{
	listitems.push_back(listItem(key_,name_,desc_));
	//make sure current choice is set to default
	if ( this->def == key_ )
        this->cur_choice_index = listitems.size() - 1;
    cbx_choices.push_back(name_);
}

listItem::listItem(std::string key_, std::string name_,std::string desc_):
	key(key_),name(name_),desc(desc_)
{

}


 mmOptionSection::mmOptionSection():mmOptionModel()
{
    key = Constants::nosection_name;
}

 mmOptionSection::mmOptionSection(std::string name_, std::string key_, std::string description_,std::string section_, std::string style_ )
	:mmOptionModel( name_, key_, description_,Enum::opt_section,section_, style_ )
{

}

} // namespace LSL {
