// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
//#include "CAttributes.h"
//#include "CAttributeImpl.h"
//#include "ITexture.h"
//#include "IXMLWriter.h"
//#include "IVideoDriver.h"
CAttributes::CAttributes(IVideoDriver* driver)
  : Driver(driver)
{
#ifdef _DEBUG
  setDebugName("CAttributes");
#endif
  if (Driver) {
    Driver->grab();
  }
}
CAttributes::~CAttributes()
{
  clear();
  if (Driver) {
    Driver->drop();
  }
}
//! Removes all attributes
void CAttributes::clear()
{
  int i;
  for (i = 0; i < Attributes_size; ++i) {
    Attributes[i]->drop();
  }
  Attributes.clear();
}
//! Sets a string attribute.
//! \param attributeName: Name for the attribute
//! \param value: Value for the attribute. Set this to 0 to delete the attribute
void CAttributes::setAttribute(const c8* attributeName, const c8* value)
{
  int i;
  for (i = 0; i < Attributes_size; ++i)
    if (Attributes[i]->Name == attributeName) {
      if (!value) {
        Attributes[i]->drop();
        Attributes.erase(i);
      }
      else {
        Attributes[i]->setString(value);
      }
      return;
    }
  if (value) {
    Attributes.push_back(new CStringAttribute(attributeName, value));
  }
}
//! Gets a string attribute.
//! \param attributeName: Name of the attribute to get.
//! \return Returns value of the attribute previously set by setStringAttribute()
//! or 0 if attribute is not set.
char* CAttributes::getAttributeAsString(const c8* attributeName)
{
  char* str;
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    return att->getString();
  }
  else {
    return str;
  }
}
//! Gets a string attribute.
//! \param attributeName: Name of the attribute to get.
//! \param target: Buffer where the string is copied to.
void CAttributes::getAttributeAsString(const c8* attributeName, char* target)
{
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    char* str = att->getString();
    strcpy(target, str);
  }
  else {
    target[0] = 0;
  }
}
//! Returns string attribute value by index.
//! \param index: Index value, must be between 0 and getAttributeCount()-1.
char* CAttributes::getAttributeAsString(s32 index)
{
  char* str;
  if ((u32)index < Attributes_size) {
    return Attributes[index]->getString();
  }
  return str;
}
//! Adds an attribute as an array of wide strings
void CAttributes::addArray(const c8* attributeName, const strv_t value)
{
  Attributes.push_back(new CStringWArrayAttribute(attributeName, value));
}
//! Sets an attribute value as an array of wide strings.
void CAttributes::setAttribute(const c8* attributeName, const strv_t value)
{
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    att->setArray(value);
  }
  else {
    Attributes.push_back(new CStringWArrayAttribute(attributeName, value));
  }
}
//! Gets an attribute as an array of wide strings.
strv_t CAttributes::getAttributeAsArray(const c8* attributeName)
{
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    return att->getArray();
  }
  else {
    return strv_t();
  }
}
//! Returns attribute value as an array of wide strings by index.
strv_t CAttributes::getAttributeAsArray(s32 index)
{
  strv_t ret;
  if (index >= 0 && index < (s32)Attributes_size) {
    ret = Attributes[index]->getArray();
  }
  return ret;
}
//! Sets an attribute as an array of wide strings
void CAttributes::setAttribute(s32 index, const strv_t value)
{
  if (index >= 0 && index < (s32)Attributes_size) {
    Attributes[index]->setArray(value);
  }
}
//! Returns attribute index from name, -1 if not found
s32 CAttributes::findAttribute(const c8* attributeName) const
{
  int i;
  for (i = 0; i < Attributes_size; ++i)
    if (Attributes[i]->Name == attributeName) {
      return i;
    }
  return -1;
}
IAttribute* CAttributes::getAttributeP(const c8* attributeName) const
{
  int i;
  for (i = 0; i < Attributes_size; ++i)
    if (Attributes[i]->Name == attributeName) {
      return Attributes[i];
    }
  return 0;
}
//! Sets a attribute as boolean value
void CAttributes::setAttribute(const c8* attributeName, bool value)
{
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    att->setBool(value);
  }
  else {
    Attributes.push_back(new CBoolAttribute(attributeName, value));
  }
}
//! Gets a attribute as boolean value
//! \param attributeName: Name of the attribute to get.
//! \return Returns value of the attribute previously set by setAttribute() as bool
//! or 0 if attribute is not set.
bool CAttributes::getAttributeAsBool(const c8* attributeName)
{
  bool ret = false;
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    ret = att->getBool();
  }
  _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
  return ret;
}
//! Sets a attribute as integer value
void CAttributes::setAttribute(const c8* attributeName, s32 value)
{
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    att->setInt(value);
  }
  else {
    Attributes.push_back(new CIntAttribute(attributeName, value));
  }
}
//! Gets a attribute as integer value
//! \param attributeName: Name of the attribute to get.
//! \return Returns value of the attribute previously set by setAttribute() as integer
//! or 0 if attribute is not set.
s32 CAttributes::getAttributeAsInt(const c8* attributeName) const
{
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    return att->getInt();
  }
  else {
    return 0;
  }
}
//! Sets a attribute as float value
void CAttributes::setAttribute(const c8* attributeName, f32 value)
{
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    att->setFloat(value);
  }
  else {
    Attributes.push_back(new CFloatAttribute(attributeName, value));
  }
}
//! Gets a attribute as integer value
//! \param attributeName: Name of the attribute to get.
//! \return Returns value of the attribute previously set by setAttribute() as float value
//! or 0 if attribute is not set.
f32 CAttributes::getAttributeAsFloat(const c8* attributeName)
{
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    return att->getFloat();
  }
  return 0.f;
}
//! Sets a attribute as color
void CAttributes::setAttribute(const c8* attributeName, SColor value)
{
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    att->setColor(value);
  }
  else {
    Attributes.push_back(new CColorAttribute(attributeName, value));
  }
}
//! Gets an attribute as color
//! \param attributeName: Name of the attribute to get.
//! \return Returns value of the attribute previously set by setAttribute()
SColor CAttributes::getAttributeAsColor(const c8* attributeName)
{
  SColor ret(0);
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    ret = att->getColor();
  }
  return ret;
}
//! Sets a attribute as floating point color
void CAttributes::setAttribute(const c8* attributeName, SColorf value)
{
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    att->setColor(value);
  }
  else {
    Attributes.push_back(new CColorfAttribute(attributeName, value));
  }
}
//! Gets an attribute as floating point color
//! \param attributeName: Name of the attribute to get.
//! \return Returns value of the attribute previously set by setAttribute()
SColorf CAttributes::getAttributeAsColorf(const c8* attributeName)
{
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    return att->getColorf();
  }
  else {
    return SColorf();
  }
}
//! Sets a attribute as 2d position
void CAttributes::setAttribute(const c8* attributeName, position2di value)
{
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    att->setPosition(value);
  }
  else {
    Attributes.push_back(new CPosition2DAttribute(attributeName, value));
  }
}
//! Gets an attribute as 2d position
//! \param attributeName: Name of the attribute to get.
//! \return Returns value of the attribute previously set by setAttribute()
position2di CAttributes::getAttributeAsPosition2d(const c8* attributeName)
{
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    return att->getPosition();
  }
  else {
    return position2di();
  }
}
//! Sets a attribute as rectangle
void CAttributes::setAttribute(const c8* attributeName, IRECT value)
{
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    att->setRect(value);
  }
  else {
    Attributes.push_back(new CRectAttribute(attributeName, value));
  }
}
//! Gets an attribute as rectangle
//! \param attributeName: Name of the attribute to get.
//! \return Returns value of the attribute previously set by setAttribute()
IRECT CAttributes::getAttributeAsRect(const c8* attributeName)
{
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    return att->getRect();
  }
  else {
    return IRECT();
  }
}
//! Sets a attribute as dimension2d
void CAttributes::setAttribute(const c8* attributeName, dimension2du value)
{
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    att->setDimension2d(value);
  }
  else {
    Attributes.push_back(new CDimension2dAttribute(attributeName, value));
  }
}
//! Gets an attribute as dimension2d
//! \param attributeName: Name of the attribute to get.
//! \return Returns value of the attribute previously set by setAttribute()
dimension2du CAttributes::getAttributeAsDimension2d(const c8* attributeName)
{
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    return att->getDimension2d();
  }
  else {
    return dimension2du();
  }
}
//! Sets a attribute as vector
void CAttributes::setAttribute(const c8* attributeName, float3 value)
{
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    att->setVector(value);
  }
  else {
    Attributes.push_back(new CVector3DAttribute(attributeName, value));
  }
}
//! Sets a attribute as vector
void CAttributes::setAttribute(const c8* attributeName, vector2df value)
{
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    att->setVector2d(value);
  }
  else {
    Attributes.push_back(new CVector2DAttribute(attributeName, value));
  }
}
//! Gets an attribute as vector
//! \param attributeName: Name of the attribute to get.
//! \return Returns value of the attribute previously set by setAttribute()
float3 CAttributes::getAttributeAsVector3d(const c8* attributeName)
{
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    return att->getVector();
  }
  else {
    return float3();
  }
}
//! Gets an attribute as vector
vector2df CAttributes::getAttributeAsVector2d(const c8* attributeName)
{
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    return att->getVector2d();
  }
  else {
    return vector2df();
  }
}
//! Sets an attribute as binary data
void CAttributes::setAttribute(const c8* attributeName, void* data, s32 dataSizeInBytes)
{
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    att->setBinary(data, dataSizeInBytes);
  }
  else {
    Attributes.push_back(new CBinaryAttribute(attributeName, data, dataSizeInBytes));
  }
}
//! Gets an attribute as binary data
//! \param attributeName: Name of the attribute to get.
void CAttributes::getAttributeAsBinaryData(const c8* attributeName, void* outData, s32 maxSizeInBytes)
{
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    att->getBinary(outData, maxSizeInBytes);
  }
}
//! Sets an attribute as enumeration
void CAttributes::setAttribute(const c8* attributeName, const char* enumValue, const char* const* enumerationLiterals)
{
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    att->setEnum(enumValue, enumerationLiterals);
  }
  else {
    Attributes.push_back(new CEnumAttribute(attributeName, enumValue, enumerationLiterals));
  }
}
//! Gets an attribute as enumeration
//! \param attributeName: Name of the attribute to get.
//! \return Returns value of the attribute previously set by setAttribute()
const char* CAttributes::getAttributeAsEnumeration(const c8* attributeName)
{
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    return att->getEnum();
  }
  else {
    return 0;
  }
}
//! Gets an attribute as enumeration
s32 CAttributes::getAttributeAsEnumeration(const c8* attributeName, const char* const* enumerationLiteralsToUse)
{
  IAttribute* att = getAttributeP(attributeName);
  if (enumerationLiteralsToUse && att) {
    const char* value = att->getEnum();
    if (value) {
      for (s32 i = 0; enumerationLiteralsToUse[i]; ++i)
        if (!strcmp(value, enumerationLiteralsToUse[i])) {
          return i;
        }
    }
  }
  return -1;
}
//! Gets the list of enumeration literals of an enumeration attribute
//! \param attributeName: Name of the attribute to get.
void CAttributes::getAttributeEnumerationLiteralsOfEnumeration(const c8* attributeName, strv_t& outLiterals)
{
  IAttribute* att = getAttributeP(attributeName);
  if (att && att->getType() == EAT_ENUM) {
    outLiterals = ((CEnumAttribute*)att)->EnumLiterals;
  }
}
//! Sets an attribute as texture reference
void CAttributes::setAttribute(const c8* attributeName, ITexture* value, const char* filename)
{
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    att->setTexture(value, filename);
  }
  else {
    Attributes.push_back(new CTextureAttribute(attributeName, value, Driver, filename));
  }
}
//! Gets an attribute as texture reference
//! \param attributeName: Name of the attribute to get.
ITexture* CAttributes::getAttributeAsTexture(const c8* attributeName)
{
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    return att->getTexture();
  }
  else {
    return 0;
  }
}
//! Gets an attribute as texture reference
//! \param index: Index value, must be between 0 and getAttributeCount()-1.
ITexture* CAttributes::getAttributeAsTexture(s32 index)
{
  if ((u32)index < Attributes_size) {
    return Attributes[index]->getTexture();
  }
  else {
    return 0;
  }
}
//! Returns amount of string attributes set in this scene manager.
u32 CAttributes::getAttributeCount() const
{
  return Attributes_size;
}
//! Returns string attribute name by index.
//! \param index: Index value, must be between 0 and getStringAttributeCount()-1.
const c8* CAttributes::getAttributeName(s32 index)
{
  if ((u32)index >= Attributes_size) {
    return 0;
  }
  return Attributes[index]->Name;
}
//! Returns the type of an attribute
E_ATTRIBUTE_TYPE CAttributes::getAttributeType(const c8* attributeName)
{
  E_ATTRIBUTE_TYPE ret = EAT_UNKNOWN;
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    ret = att->getType();
  }
  return ret;
}
//! Returns attribute type by index.
//! \param index: Index value, must be between 0 and getAttributeCount()-1.
E_ATTRIBUTE_TYPE CAttributes::getAttributeType(s32 index)
{
  if ((u32)index >= Attributes_size) {
    return EAT_UNKNOWN;
  }
  return Attributes[index]->getType();
}
//! Returns the type of an attribute
const char* CAttributes::getAttributeTypeString(const c8* attributeName)
{
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    return att->getTypeString();
  }
  else {
    return "unknown";
  }
}
//! Returns attribute type string by index.
//! \param index: Index value, must be between 0 and getAttributeCount()-1.
const char* CAttributes::getAttributeTypeString(s32 index)
{
  if ((u32)index >= Attributes_size) {
    return "unknown";
  }
  return Attributes[index]->getTypeString();
}
//! Gets an attribute as boolean value
//! \param index: Index value, must be between 0 and getAttributeCount()-1.
bool CAttributes::getAttributeAsBool(s32 index)
{
  bool ret = false;
  if ((u32)index < Attributes_size) {
    ret = Attributes[index]->getBool();
  }
  _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
  return ret;
}
//! Gets an attribute as integer value
//! \param index: Index value, must be between 0 and getAttributeCount()-1.
s32 CAttributes::getAttributeAsInt(s32 index) const
{
  if ((u32)index < Attributes_size) {
    return Attributes[index]->getInt();
  }
  else {
    return 0;
  }
}
//! Gets an attribute as float value
//! \param index: Index value, must be between 0 and getAttributeCount()-1.
f32 CAttributes::getAttributeAsFloat(s32 index)
{
  if ((u32)index < Attributes_size) {
    return Attributes[index]->getFloat();
  }
  else {
    return 0.f;
  }
}
//! Gets an attribute as color
//! \param index: Index value, must be between 0 and getAttributeCount()-1.
SColor CAttributes::getAttributeAsColor(s32 index)
{
  SColor ret(0);
  if ((u32)index < Attributes_size) {
    ret = Attributes[index]->getColor();
  }
  return ret;
}
//! Gets an attribute as floating point color
//! \param index: Index value, must be between 0 and getAttributeCount()-1.
SColorf CAttributes::getAttributeAsColorf(s32 index)
{
  if ((u32)index < Attributes_size) {
    return Attributes[index]->getColorf();
  }
  return SColorf();
}
//! Gets an attribute as 3d vector
//! \param index: Index value, must be between 0 and getAttributeCount()-1.
float3 CAttributes::getAttributeAsVector3d(s32 index)
{
  if ((u32)index < Attributes_size) {
    return Attributes[index]->getVector();
  }
  else {
    return float3();
  }
}
//! Gets an attribute as 2d vector
vector2df CAttributes::getAttributeAsVector2d(s32 index)
{
  if ((u32)index < Attributes_size) {
    return Attributes[index]->getVector2d();
  }
  else {
    return vector2df();
  }
}
//! Gets an attribute as position2d
//! \param index: Index value, must be between 0 and getAttributeCount()-1.
position2di CAttributes::getAttributeAsPosition2d(s32 index)
{
  if ((u32)index < Attributes_size) {
    return Attributes[index]->getPosition();
  }
  else {
    return position2di();
  }
}
//! Gets an attribute as rectangle
//! \param index: Index value, must be between 0 and getAttributeCount()-1.
IRECT  CAttributes::getAttributeAsRect(s32 index)
{
  if ((u32)index < Attributes_size) {
    return Attributes[index]->getRect();
  }
  else {
    return IRECT();
  }
}
//! Gets an attribute as dimension2d
//! \param index: Index value, must be between 0 and getAttributeCount()-1.
dimension2du  CAttributes::getAttributeAsDimension2d(s32 index)
{
  if ((u32)index < Attributes_size) {
    return Attributes[index]->getDimension2d();
  }
  else {
    return dimension2du();
  }
}
//! Gets an attribute as binary data
///! \param index: Index value, must be between 0 and getAttributeCount()-1.
void CAttributes::getAttributeAsBinaryData(s32 index, void* outData, s32 maxSizeInBytes)
{
  if ((u32)index < Attributes_size) {
    Attributes[index]->getBinary(outData, maxSizeInBytes);
  }
}
//! Gets an attribute as enumeration
//! \param index: Index value, must be between 0 and getAttributeCount()-1.
const char* CAttributes::getAttributeAsEnumeration(s32 index)
{
  if ((u32)index < Attributes_size) {
    return Attributes[index]->getEnum();
  }
  else {
    return 0;
  }
}
//! Gets an attribute as enumeration
//! \param index: Index value, must be between 0 and getAttributeCount()-1.
s32 CAttributes::getAttributeAsEnumeration(s32 index, const char* const* enumerationLiteralsToUse)
{
  if ((u32)index < Attributes_size) {
    IAttribute* att = Attributes[index];
    if (enumerationLiteralsToUse && att) {
      const char* value = att->getEnum();
      if (value) {
        for (s32 i = 0; enumerationLiteralsToUse[i]; ++i)
          if (!strcmp(value, enumerationLiteralsToUse[i])) {
            return i;
          }
      }
    }
  }
  return -1;
}
//! Gets the list of enumeration literals of an enumeration attribute
//! \param index: Index value, must be between 0 and getAttributeCount()-1.
void CAttributes::getAttributeEnumerationLiteralsOfEnumeration(s32 index, strv_t& outLiterals)
{
  if ((u32)index < Attributes_size &&
      Attributes[index]->getType() == EAT_ENUM) {
    outLiterals = ((CEnumAttribute*)Attributes[index])->EnumLiterals;
  }
}
//! Adds an attribute as integer
void CAttributes::addInt(const c8* attributeName, s32 value)
{
  Attributes.push_back(new CIntAttribute(attributeName, value));
}
//! Adds an attribute as float
void CAttributes::addFloat(const c8* attributeName, f32 value)
{
  Attributes.push_back(new CFloatAttribute(attributeName, value));
}
//! Adds an attribute as string
void CAttributes::addString(const c8* attributeName, const char* value)
{
  Attributes.push_back(new CStringAttribute(attributeName, value));
}
//! Adds an attribute as bool
void CAttributes::addBool(const c8* attributeName, bool value)
{
  Attributes.push_back(new CBoolAttribute(attributeName, value));
}
//! Adds an attribute as enum
void CAttributes::addEnum(const c8* attributeName, const char* enumValue, const char* const* enumerationLiterals)
{
  Attributes.push_back(new CEnumAttribute(attributeName, enumValue, enumerationLiterals));
}
//! Adds an attribute as enum
void CAttributes::addEnum(const c8* attributeName, s32 enumValue, const char* const* enumerationLiterals)
{
  addEnum(attributeName, "", enumerationLiterals);
  Attributes.getLast()->setInt(enumValue);
}
//! Adds an attribute as color
void CAttributes::addColor(const c8* attributeName, SColor value)
{
  Attributes.push_back(new CColorAttribute(attributeName, value));
}
//! Adds an attribute as floating point color
void CAttributes::addColorf(const c8* attributeName, SColorf value)
{
  Attributes.push_back(new CColorfAttribute(attributeName, value));
}
//! Adds an attribute as 3d vector
void CAttributes::addVector3d(const c8* attributeName, float3 value)
{
  Attributes.push_back(new CVector3DAttribute(attributeName, value));
}
//! Adds an attribute as 2d vector
void CAttributes::addVector2d(const c8* attributeName, vector2df value)
{
  Attributes.push_back(new CVector2DAttribute(attributeName, value));
}
//! Adds an attribute as 2d position
void CAttributes::addPosition2d(const c8* attributeName, position2di value)
{
  Attributes.push_back(new CPosition2DAttribute(attributeName, value));
}
//! Adds an attribute as rectangle
void CAttributes::addRect(const c8* attributeName, IRECT value)
{
  Attributes.push_back(new CRectAttribute(attributeName, value));
}
//! Adds an attribute as dimension2d
void CAttributes::addDimension2d(const c8* attributeName, dimension2du value)
{
  Attributes.push_back(new CDimension2dAttribute(attributeName, value));
}
//! Adds an attribute as binary data
void CAttributes::addBinary(const c8* attributeName, void* data, s32 dataSizeInBytes)
{
  Attributes.push_back(new CBinaryAttribute(attributeName, data, dataSizeInBytes));
}
//! Adds an attribute as texture reference
void CAttributes::addTexture(const c8* attributeName, ITexture* texture, const char* filename)
{
  Attributes.push_back(new CTextureAttribute(attributeName, texture, Driver, filename));
}
//! Returns if an attribute with a name exists
bool CAttributes::existsAttribute(const c8* attributeName)
{
  return getAttributeP(attributeName) != 0;
}
//! Sets an attribute value as string.
//! \param attributeName: Name for the attribute
void CAttributes::setAttribute(s32 index, const c8* value)
{
  if ((u32)index < Attributes_size) {
    Attributes[index]->setString(value);
  }
}
//! Sets an attribute as boolean value
void CAttributes::setAttribute(s32 index, bool value)
{
  if ((u32)index < Attributes_size) {
    Attributes[index]->setBool(value);
  }
}
//! Sets an attribute as integer value
void CAttributes::setAttribute(s32 index, s32 value)
{
  if ((u32)index < Attributes_size) {
    Attributes[index]->setInt(value);
  }
}
//! Sets a attribute as float value
void CAttributes::setAttribute(s32 index, f32 value)
{
  if ((u32)index < Attributes_size) {
    Attributes[index]->setFloat(value);
  }
}
//! Sets a attribute as color
void CAttributes::setAttribute(s32 index, SColor color)
{
  if ((u32)index < Attributes_size) {
    Attributes[index]->setColor(color);
  }
}
//! Sets a attribute as floating point color
void CAttributes::setAttribute(s32 index, SColorf color)
{
  if ((u32)index < Attributes_size) {
    Attributes[index]->setColor(color);
  }
}
//! Sets a attribute as vector
void CAttributes::setAttribute(s32 index, float3 v)
{
  if ((u32)index < Attributes_size) {
    Attributes[index]->setVector(v);
  }
}
//! Sets a attribute as vector
void CAttributes::setAttribute(s32 index, vector2df v)
{
  if ((u32)index < Attributes_size) {
    Attributes[index]->setVector2d(v);
  }
}
//! Sets a attribute as position
void CAttributes::setAttribute(s32 index, position2di v)
{
  if ((u32)index < Attributes_size) {
    Attributes[index]->setPosition(v);
  }
}
//! Sets a attribute as rectangle
void CAttributes::setAttribute(s32 index, IRECT v)
{
  if ((u32)index < Attributes_size) {
    Attributes[index]->setRect(v);
  }
}
//! Sets a attribute as dimension2d
void CAttributes::setAttribute(s32 index, dimension2du v)
{
  if ((u32)index < Attributes_size) {
    Attributes[index]->setDimension2d(v);
  }
}
//! Sets an attribute as binary data
void CAttributes::setAttribute(s32 index, void* data, s32 dataSizeInBytes)
{
  if ((u32)index < Attributes_size) {
    Attributes[index]->setBinary(data, dataSizeInBytes);
  }
}
//! Sets an attribute as enumeration
void CAttributes::setAttribute(s32 index, const char* enumValue, const char* const* enumerationLiterals)
{
  if ((u32)index < Attributes_size) {
    Attributes[index]->setEnum(enumValue, enumerationLiterals);
  }
}
//! Sets an attribute as texture reference
void CAttributes::setAttribute(s32 index, ITexture* texture, const char* filename)
{
  if ((u32)index < Attributes_size) {
    Attributes[index]->setTexture(texture, filename);
  }
}
//! Adds an attribute as matrix
void CAttributes::addMatrix(const c8* attributeName, const matrix4& v)
{
  Attributes.push_back(new CMatrixAttribute(attributeName, v));
}
//! Sets an attribute as matrix
void CAttributes::setAttribute(const c8* attributeName, const matrix4& v)
{
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    att->setMatrix(v);
  }
  else {
    Attributes.push_back(new CMatrixAttribute(attributeName, v));
  }
}
//! Gets an attribute as a matrix4
matrix4 CAttributes::getAttributeAsMatrix(const c8* attributeName)
{
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    return att->getMatrix();
  }
  else {
    return matrix4();
  }
}
//! Gets an attribute as matrix
matrix4 CAttributes::getAttributeAsMatrix(s32 index)
{
  if ((u32)index < Attributes_size) {
    return Attributes[index]->getMatrix();
  }
  else {
    return matrix4();
  }
}
//! Sets an attribute as matrix
void CAttributes::setAttribute(s32 index, const matrix4& v)
{
  if ((u32)index < Attributes_size) {
    Attributes[index]->setMatrix(v);
  }
}
//! Adds an attribute as quaternion
void CAttributes::addQuaternion(const c8* attributeName, quaternion v)
{
  Attributes.push_back(new CQuaternionAttribute(attributeName, v));
}
//! Sets an attribute as quaternion
void CAttributes::setAttribute(const c8* attributeName, quaternion v)
{
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    att->setQuaternion(v);
  }
  else {
    Attributes.push_back(new CQuaternionAttribute(attributeName, v));
  }
}
//! Gets an attribute as a quaternion
quaternion CAttributes::getAttributeAsQuaternion(const c8* attributeName)
{
  quaternion ret(0, 1, 0, 0);
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    ret = att->getQuaternion();
  }
  return ret;
}
//! Gets an attribute as quaternion
quaternion CAttributes::getAttributeAsQuaternion(s32 index)
{
  quaternion ret(0, 1, 0, 0);
  if (index >= 0 && index < (s32)Attributes_size) {
    ret = Attributes[index]->getQuaternion();
  }
  return ret;
}
//! Sets an attribute as quaternion
void CAttributes::setAttribute(s32 index, quaternion v)
{
  if (index >= 0 && index < (s32)Attributes_size) {
    Attributes[index]->setQuaternion(v);
  }
}
//! Adds an attribute as axis aligned bounding box
void CAttributes::addBox3d(const c8* attributeName, aabbox3df v)
{
  Attributes.push_back(new CBBoxAttribute(attributeName, v));
}
//! Sets an attribute as axis aligned bounding box
void CAttributes::setAttribute(const c8* attributeName, aabbox3df v)
{
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    att->setBBox(v);
  }
  else {
    Attributes.push_back(new CBBoxAttribute(attributeName, v));
  }
}
//! Gets an attribute as a axis aligned bounding box
aabbox3df CAttributes::getAttributeAsBox3d(const c8* attributeName)
{
  aabbox3df ret(0, 0, 0, 0, 0, 0);
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    ret = att->getBBox();
  }
  return ret;
}
//! Gets an attribute as axis aligned bounding box
aabbox3df CAttributes::getAttributeAsBox3d(s32 index)
{
  aabbox3df ret(0, 0, 0, 0, 0, 0);
  if (index >= 0 && index < (s32)Attributes_size) {
    ret = Attributes[index]->getBBox();
  }
  return ret;
}
//! Sets an attribute as axis aligned bounding box
void CAttributes::setAttribute(s32 index, aabbox3df v)
{
  if (index >= 0 && index < (s32)Attributes_size) {
    Attributes[index]->setBBox(v);
  }
}
//! Adds an attribute as 3d plane
void CAttributes::addPlane3d(const c8* attributeName, plane3df v)
{
  Attributes.push_back(new CPlaneAttribute(attributeName, v));
}
//! Sets an attribute as 3d plane
void CAttributes::setAttribute(const c8* attributeName, plane3df v)
{
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    att->setPlane(v);
  }
  else {
    Attributes.push_back(new CPlaneAttribute(attributeName, v));
  }
}
//! Gets an attribute as a 3d plane
plane3df CAttributes::getAttributeAsPlane3d(const c8* attributeName)
{
  plane3df ret(0, 0, 0, 0, 1, 0);
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    ret = att->getPlane();
  }
  return ret;
}
//! Gets an attribute as 3d plane
plane3df CAttributes::getAttributeAsPlane3d(s32 index)
{
  plane3df ret(0, 0, 0, 0, 1, 0);
  if (index >= 0 && index < (s32)Attributes_size) {
    ret = Attributes[index]->getPlane();
  }
  return ret;
}
//! Sets an attribute as 3d plane
void CAttributes::setAttribute(s32 index, plane3df v)
{
  if (index >= 0 && index < (s32)Attributes_size) {
    Attributes[index]->setPlane(v);
  }
}
//! Adds an attribute as 3d triangle
void CAttributes::addTriangle3d(const c8* attributeName, triangle3df v)
{
  Attributes.push_back(new CTriangleAttribute(attributeName, v));
}
//! Sets an attribute as 3d triangle
void CAttributes::setAttribute(const c8* attributeName, triangle3df v)
{
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    att->setTriangle(v);
  }
  else {
    Attributes.push_back(new CTriangleAttribute(attributeName, v));
  }
}
//! Gets an attribute as a 3d triangle
triangle3df CAttributes::getAttributeAsTriangle3d(const c8* attributeName)
{
  triangle3df ret;
  ret.pointA = ret.pointB = ret.pointC = float3(0, 0, 0);
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    ret = att->getTriangle();
  }
  return ret;
}
//! Gets an attribute as 3d triangle
triangle3df CAttributes::getAttributeAsTriangle3d(s32 index)
{
  triangle3df ret;
  ret.pointA = ret.pointB = ret.pointC = float3(0, 0, 0);
  if (index >= 0 && index < (s32)Attributes_size) {
    ret = Attributes[index]->getTriangle();
  }
  return ret;
}
//! Sets an attribute as 3d triangle
void CAttributes::setAttribute(s32 index, triangle3df v)
{
  if (index >= 0 && index < (s32)Attributes_size) {
    Attributes[index]->setTriangle(v);
  }
}
//! Adds an attribute as a 2d line
void CAttributes::addLine2d(const c8* attributeName, line2df v)
{
  Attributes.push_back(new CLine2dAttribute(attributeName, v));
}
//! Sets an attribute as a 2d line
void CAttributes::setAttribute(const c8* attributeName, line2df v)
{
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    att->setLine2d(v);
  }
  else {
    Attributes.push_back(new CLine2dAttribute(attributeName, v));
  }
}
//! Gets an attribute as a 2d line
line2df CAttributes::getAttributeAsLine2d(const c8* attributeName)
{
  line2df ret(0, 0, 0, 0);
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    ret = att->getLine2d();
  }
  return ret;
}
//! Gets an attribute as a 2d line
line2df CAttributes::getAttributeAsLine2d(s32 index)
{
  line2df ret(0, 0, 0, 0);
  if (index >= 0 && index < (s32)Attributes_size) {
    ret = Attributes[index]->getLine2d();
  }
  return ret;
}
//! Sets an attribute as a 2d line
void CAttributes::setAttribute(s32 index, line2df v)
{
  if (index >= 0 && index < (s32)Attributes_size) {
    Attributes[index]->setLine2d(v);
  }
}
//! Adds an attribute as a 3d line
void CAttributes::addLine3d(const c8* attributeName, line3df v)
{
  Attributes.push_back(new CLine3dAttribute(attributeName, v));
}
//! Sets an attribute as a 3d line
void CAttributes::setAttribute(const c8* attributeName, line3df v)
{
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    att->setLine3d(v);
  }
  else {
    Attributes.push_back(new CLine3dAttribute(attributeName, v));
  }
}
//! Gets an attribute as a 3d line
line3df CAttributes::getAttributeAsLine3d(const c8* attributeName)
{
  line3df ret(0, 0, 0, 0, 0, 0);
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    ret = att->getLine3d();
  }
  return ret;
}
//! Gets an attribute as a 3d line
line3df CAttributes::getAttributeAsLine3d(s32 index)
{
  line3df ret(0, 0, 0, 0, 0, 0);
  if (index >= 0 && index < (s32)Attributes_size) {
    ret = Attributes[index]->getLine3d();
  }
  return ret;
}
//! Sets an attribute as a 3d line
void CAttributes::setAttribute(s32 index, line3df v)
{
  if (index >= 0 && index < (s32)Attributes_size) {
    Attributes[index]->setLine3d(v);
  }
}
//! Adds an attribute as user pointner
void CAttributes::addUserPointer(const c8* attributeName, void* userPointer)
{
  Attributes.push_back(new CUserPointerAttribute(attributeName, userPointer));
}
//! Sets an attribute as user pointer
void CAttributes::setAttribute(const c8* attributeName, void* userPointer)
{
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    att->setUserPointer(userPointer);
  }
  else {
    Attributes.push_back(new CUserPointerAttribute(attributeName, userPointer));
  }
}
//! Gets an attribute as user pointer
//! \param attributeName: Name of the attribute to get.
void* CAttributes::getAttributeAsUserPointer(const c8* attributeName)
{
  void* value = 0;
  IAttribute* att = getAttributeP(attributeName);
  if (att) {
    value = att->getUserPointer();
  }
  return value;
}
//! Gets an attribute as user pointer
//! \param index: Index value, must be between 0 and getAttributeCount()-1.
void* CAttributes::getAttributeAsUserPointer(s32 index)
{
  void* value = 0;
  if (index >= 0 && index < (s32)Attributes_size) {
    value = Attributes[index]->getUserPointer();
  }
  return value;
}
//! Sets an attribute as user pointer
void CAttributes::setAttribute(s32 index, void* userPointer)
{
  if (index >= 0 && index < (s32)Attributes_size) {
    Attributes[index]->setUserPointer(userPointer);
  }
}
//! Reads attributes from a xml file.
//! \param readCurrentElementOnly: If set to true, reading only works if current element has the name 'attributes'.
//! IF set to false, the first appearing list attributes are read.
bool CAttributes::read(IXMLReader* reader, bool readCurrentElementOnly,
    const char* nonDefaultElementName)
{
  if (!reader) {
    return false;
  }
  clear();
  char* elementName = "attributes";
  if (nonDefaultElementName) {
    elementName = nonDefaultElementName;
  }
  if (readCurrentElementOnly) {
    if (elementName != reader->getNodeName()) {
      return false;
    }
  }
  while (reader->read()) {
    switch (reader->getNodeType()) {
    case EXN_ELEMENT:
      readAttributeFromXML(reader);
      break;
    case EXN_ELEMENT_END:
      if (elementName == reader->getNodeName()) {
        return true;
      }
      break;
    default:
      break;
    }
  }
  return true;
}
void CAttributes::readAttributeFromXML(IXMLReader* reader)
{
  char* element = reader->getNodeName();
  char* name = reader->getAttributeValue("name");
  if (element == "enum") {
    addEnum(name, (int)0, NULL);
    Attributes.getLast()->setString(reader->getAttributeValue("value"));
  }
  else if (element == "binary") {
    addBinary(name, 0, 0);
    Attributes.getLast()->setString(reader->getAttributeValue("value"));
  }
  else if (element == "color") {
    addColor(name, SColor());
    Attributes.getLast()->setString(reader->getAttributeValue("value"));
  }
  else if (element == "colorf") {
    addColorf(name, SColorf());
    Attributes.getLast()->setString(reader->getAttributeValue("value"));
  }
  else if (element == "float") {
    addFloat(name, 0);
    Attributes.getLast()->setString(reader->getAttributeValue("value"));
  }
  else if (element == "int") {
    addInt(name, 0);
    Attributes.getLast()->setString(reader->getAttributeValue("value"));
  }
  else if (element == "bool") {
    addBool(name, 0);
    Attributes.getLast()->setString(reader->getAttributeValue("value"));
  }
  else if (element == "string") {
    addString(name, "");
    Attributes.getLast()->setString(reader->getAttributeValue("value"));
  }
  else if (element == "texture") {
    addTexture(name, 0);
    Attributes.getLast()->setString(reader->getAttributeValue("value"));
  }
  else if (element == "vector3d") {
    addVector3d(name, float3());
    Attributes.getLast()->setString(reader->getAttributeValue("value"));
  }
  else if (element == "vector2d") {
    addVector2d(name, vector2df());
    Attributes.getLast()->setString(reader->getAttributeValue("value"));
  }
  else if (element == "position") {
    addPosition2d(name, position2di());
    Attributes.getLast()->setString(reader->getAttributeValue("value"));
  }
  else if (element == "rect") {
    addRect(name, IRECT());
    Attributes.getLast()->setString(reader->getAttributeValue("value"));
  }
  else if (element == "matrix") {
    addMatrix(name, matrix4());
    Attributes.getLast()->setString(reader->getAttributeValue("value"));
  }
  else if (element == "quaternion") {
    addQuaternion(name, quaternion());
    Attributes.getLast()->setString(reader->getAttributeValue("value"));
  }
  else if (element == "box3d") {
    addBox3d(name, aabbox3df());
    Attributes.getLast()->setString(reader->getAttributeValue("value"));
  }
  else if (element == "plane") {
    addPlane3d(name, plane3df());
    Attributes.getLast()->setString(reader->getAttributeValue("value"));
  }
  else if (element == "triangle") {
    addTriangle3d(name, triangle3df());
    Attributes.getLast()->setString(reader->getAttributeValue("value"));
  }
  else if (element == "line2d") {
    addLine2d(name, line2df());
    Attributes.getLast()->setString(reader->getAttributeValue("value"));
  }
  else if (element == "line3d") {
    addLine3d(name, line3df());
    Attributes.getLast()->setString(reader->getAttributeValue("value"));
  }
  else if (element == "stringwarray") {
    strv_t tmpArray;
    s32 count = reader->getAttributeValueAsInt("count");
    s32 n = 0;
    const char* tmpName("value");
    for (; n < count; ++n) {
      tmpArray.push_back(reader->getAttributeValue((tmpName + char * (n))));
    }
    addArray(name, tmpArray);
  }
  else if (element == "userPointer") {
    // It's debatable if a pointer should be set or not, but it's more likely that adding it now would wreck user-applications.
    // Also it probably doesn't makes sense setting this to a value when it comes from file.
  }
  else if (element == "dimension2d") {
    addDimension2d(name, dimension2du());
    Attributes.getLast()->setString(reader->getAttributeValue("value"));
  }
}
//! Write these attributes into a xml file
bool CAttributes::write(IXMLWriter* writer, bool writeXMLHeader,
    const char* nonDefaultElementName)
{
  if (!writer) {
    return false;
  }
  if (writeXMLHeader) {
    writer->writeXMLHeader();
  }
  char* elementName = "attributes";
  if (nonDefaultElementName) {
    elementName = nonDefaultElementName;
  }
  writer->writeElement(elementName, false);
  writer->writeLineBreak();
  s32 i = 0;
  for (; i < (s32)Attributes_size; ++i) {
    if (Attributes[i]->getType() == EAT_STRINGWARRAY) {
      strv_t arraynames, arrayvalues;
      strv_t arrayinput = Attributes[i]->getArray();
      // build arrays
      // name
      arraynames.push_back(char * ("name"));
      arrayvalues.push_back(char * (Attributes[i]->Name));
      // count
      arraynames.push_back(char * ("count"));
      arrayvalues.push_back(char * ((s32)arrayinput_size));
      // array...
      u32 n = 0;
      const char* tmpName("value");
      for (; n < arrayinput_size; ++n) {
        arraynames.push_back((tmpName + char * ((int)n)));
        arrayvalues.push_back(arrayinput[n]);
      }
      // write them
      writer->writeElement(Attributes[i]->getTypeString(), true, arraynames, arrayvalues);
    }
    else {
      writer->writeElement(
          Attributes[i]->getTypeString(), true,
          "name", char * (Attributes[i]->Name),
          "value", Attributes[i]->getString());
    }
    writer->writeLineBreak();
  }
  writer->writeClosingTag(elementName);
  writer->writeLineBreak();
  return true;
}

