#include "mmoptionmodel.h"

mmOptionModel::mmOptionModel(std::string name_, std::string key_, std::string description_, OptionType type_,
                             std::string section_ , std::string style_ )
	: name(name_),key(key_),description(description_),type(type_),
      section(section_),ct_type_string(style_)
{
    //set style according to input string
    if ( style_ == _T("yaadda") )
        ct_type = ct_someothers;
    else
        ct_type = ct_undefined; //lobby will chooose best fit

    if ( section == wxEmptyString )
        section = SLGlobals::nosection_name;

    if ( ct_type_string == wxEmptyString )
        ct_type_string  = SLGlobals::nostyle_name;

}

mmOptionModel::mmOptionModel(std::string name_, std::string key_, std::string description_, OptionType type_,
                             std::string section_ , ControlType style_ )
	: name(name_),key(key_),description(description_),type(type_),
      ct_type(style_), section(section_)
{
}

mmOptionModel::~mmOptionModel()
{}

mmOptionModel::mmOptionModel()
{
	type = opt_undefined;
	name = _T("");
	key = name;
	description = name;
	section = SLGlobals::nosection_name;
	ct_type = ct_undefined;
}

mmOptionBool::mmOptionBool(std::string name_, std::string key_, std::string description_, bool def_,
                           std::string section_ , std::string style_):
	mmOptionModel(name_,key_,description_,opt_bool,section_,style_),
	def(def_),value(def_)
{}

mmOptionBool::mmOptionBool():mmOptionModel()
{
	value = false;
	def = value;
}

mmOptionFloat::mmOptionFloat(std::string name_, std::string key_, std::string description_, float def_, float stepping_, float min_, float max_,
                             std::string section_ , std::string style_):
	mmOptionModel(name_,key_,description_,opt_float,section_,style_),
	def(def_),value(def_),stepping(stepping_),min(min_),max(max_)
{}

mmOptionFloat::mmOptionFloat() : mmOptionModel(),
	def(0.0), value(0.0), stepping(0.0), min(0.0), max(0.0)
{}

mmOptionString::mmOptionString(std::string name_, std::string key_, std::string description_, std::string def_, unsigned int max_len_,
                               std::string section_ , std::string style_):
	mmOptionModel(name_,key_,description_,opt_string,section_,style_),
	def(def_),value(def_),max_len(max_len_)
{}

mmOptionString::mmOptionString():mmOptionModel()
{
	def = _T("");
	value = def;
	max_len = 0;
}

mmOptionList::mmOptionList(std::string name_, std::string key_, std::string description_, std::string def_,
                           std::string section_ , std::string style_):
	mmOptionModel(name_,key_,description_,opt_list,section_,style_),
	def(def_),value(def_)
{
	cur_choice_index = 0;
}

mmOptionList::mmOptionList():mmOptionModel()
{
	value = _T("");
	def = _T("");
}

void mmOptionList::addItem(std::string key_, std::string name_, std::string desc_)
{
	listitems.push_back(listItem(key_,name_,desc_));
	//make sure current choice is set to default
	if ( this->def == key_ )
        this->cur_choice_index = listitems.size() - 1;
	cbx_choices.Add(name_);
}

listItem::listItem(std::string key_, std::string name_,std::string desc_):
	key(key_),name(name_),desc(desc_)
{

}


 mmOptionSection::mmOptionSection():mmOptionModel()
{
    key = SLGlobals::nosection_name;
}

 mmOptionSection::mmOptionSection(std::string name_, std::string key_, std::string description_,std::string section_, std::string style_ )
    :mmOptionModel( name_, key_, description_, opt_section,section_, style_ )
{

}

