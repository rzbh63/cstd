# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: human_face_report.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf.internal import enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='human_face_report.proto',
  package='rongyi.face.log',
  syntax='proto2',
  serialized_pb=_b('\n\x17human_face_report.proto\x12\x0frongyi.face.log\"\x94\x02\n\x0bUserProfile\x12\x11\n\tbegin_age\x18\x01 \x01(\x05\x12\x0f\n\x07\x65nd_age\x18\x02 \x01(\x05\x12\'\n\x06gender\x18\x03 \x01(\x0e\x32\x17.rongyi.face.log.Gender\x12\x11\n\tface_code\x18\x04 \x01(\x0c\x12\x33\n\nexpression\x18\x05 \x01(\x0e\x32\x1f.rongyi.face.log.UserExpression\x12\x1b\n\x0cwear_glasses\x18\x06 \x01(\x08:\x05\x66\x61lse\x12\x17\n\x0e\x61ge_confidence\x18\xe9\x07 \x01(\x01\x12\x1a\n\x11gender_confidence\x18\xea\x07 \x01(\x01\x12\x1e\n\x15\x65xpression_confidence\x18\xeb\x07 \x01(\x01\"\xc7\x01\n\x10\x43\x61pturedUserFace\x12-\n\x07profile\x18\x01 \x01(\x0b\x32\x1c.rongyi.face.log.UserProfile\x12\x12\n\nenter_time\x18\x02 \x01(\x03\x12\x12\n\nleave_time\x18\x03 \x01(\x03\x12/\n\x08position\x18\x04 \x01(\x0e\x32\x1d.rongyi.face.log.FacePosition\x12\x12\n\nface_image\x18\x05 \x01(\x0c\x12\x17\n\x0fimage_mime_type\x18\x06 \x01(\x0c\"A\n\x08ReportID\x12\x13\n\x0bterminal_id\x18\x01 \x01(\x0c\x12\x11\n\ttimestamp\x18\x02 \x01(\x03\x12\r\n\x05index\x18\x03 \x01(\x03\"\x84\x01\n\x0cUserFaceInfo\x12\n\n\x02id\x18\x01 \x02(\x0c\x12\x11\n\ttimestamp\x18\x02 \x01(\x03\x12\x0f\n\x07mall_id\x18\x03 \x01(\x0c\x12\x13\n\x0bterminal_id\x18\x04 \x01(\x0c\x12/\n\x04\x66\x61\x63\x65\x18\x05 \x03(\x0b\x32!.rongyi.face.log.CapturedUserFace*\x9c\x01\n\x0eUserExpression\x12\x19\n\x15UserExpressionUnknown\x10\x00\x12\t\n\x05Happy\x10\x01\x12\x0b\n\x07\x41\x66raifd\x10\x02\x12\r\n\tSurprised\x10\x03\x12\x07\n\x03Sad\x10\x04\x12\t\n\x05\x41ngry\x10\x05\x12\n\n\x06Serene\x10\x06\x12\r\n\tDepressed\x10\x07\x12\x19\n\x14UserExpressionOthers\x10\xe8\x07*W\n\x0c\x46\x61\x63\x65Position\x12\x18\n\x14\x46\x61\x63\x65PositionPositive\x10\x00\x12\x14\n\x10\x46\x61\x63\x65PositionSide\x10\x01\x12\x17\n\x12\x46\x61\x63\x65PositionOthers\x10\xe8\x07*@\n\x06Gender\x12\x12\n\x0eGENDER_UNKNOWN\x10\x00\x12\x0f\n\x0bGENDER_MALE\x10\x01\x12\x11\n\rGENDER_FEMALE\x10\x02\x42\x19\n\x17\x63om.rongyi.sirius.proto')
)
_sym_db.RegisterFileDescriptor(DESCRIPTOR)

_USEREXPRESSION = _descriptor.EnumDescriptor(
  name='UserExpression',
  full_name='rongyi.face.log.UserExpression',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='UserExpressionUnknown', index=0, number=0,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='Happy', index=1, number=1,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='Afraifd', index=2, number=2,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='Surprised', index=3, number=3,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='Sad', index=4, number=4,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='Angry', index=5, number=5,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='Serene', index=6, number=6,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='Depressed', index=7, number=7,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='UserExpressionOthers', index=8, number=1000,
      options=None,
      type=None),
  ],
  containing_type=None,
  options=None,
  serialized_start=728,
  serialized_end=884,
)
_sym_db.RegisterEnumDescriptor(_USEREXPRESSION)

UserExpression = enum_type_wrapper.EnumTypeWrapper(_USEREXPRESSION)
_FACEPOSITION = _descriptor.EnumDescriptor(
  name='FacePosition',
  full_name='rongyi.face.log.FacePosition',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='FacePositionPositive', index=0, number=0,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='FacePositionSide', index=1, number=1,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='FacePositionOthers', index=2, number=1000,
      options=None,
      type=None),
  ],
  containing_type=None,
  options=None,
  serialized_start=886,
  serialized_end=973,
)
_sym_db.RegisterEnumDescriptor(_FACEPOSITION)

FacePosition = enum_type_wrapper.EnumTypeWrapper(_FACEPOSITION)
_GENDER = _descriptor.EnumDescriptor(
  name='Gender',
  full_name='rongyi.face.log.Gender',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='GENDER_UNKNOWN', index=0, number=0,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='GENDER_MALE', index=1, number=1,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='GENDER_FEMALE', index=2, number=2,
      options=None,
      type=None),
  ],
  containing_type=None,
  options=None,
  serialized_start=975,
  serialized_end=1039,
)
_sym_db.RegisterEnumDescriptor(_GENDER)

Gender = enum_type_wrapper.EnumTypeWrapper(_GENDER)
UserExpressionUnknown = 0
Happy = 1
Afraifd = 2
Surprised = 3
Sad = 4
Angry = 5
Serene = 6
Depressed = 7
UserExpressionOthers = 1000
FacePositionPositive = 0
FacePositionSide = 1
FacePositionOthers = 1000
GENDER_UNKNOWN = 0
GENDER_MALE = 1
GENDER_FEMALE = 2



_USERPROFILE = _descriptor.Descriptor(
  name='UserProfile',
  full_name='rongyi.face.log.UserProfile',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='begin_age', full_name='rongyi.face.log.UserProfile.begin_age', index=0,
      number=1, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='end_age', full_name='rongyi.face.log.UserProfile.end_age', index=1,
      number=2, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='gender', full_name='rongyi.face.log.UserProfile.gender', index=2,
      number=3, type=14, cpp_type=8, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='face_code', full_name='rongyi.face.log.UserProfile.face_code', index=3,
      number=4, type=12, cpp_type=9, label=1,
      has_default_value=False, default_value=_b(""),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='expression', full_name='rongyi.face.log.UserProfile.expression', index=4,
      number=5, type=14, cpp_type=8, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='wear_glasses', full_name='rongyi.face.log.UserProfile.wear_glasses', index=5,
      number=6, type=8, cpp_type=7, label=1,
      has_default_value=True, default_value=False,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='age_confidence', full_name='rongyi.face.log.UserProfile.age_confidence', index=6,
      number=1001, type=1, cpp_type=5, label=1,
      has_default_value=False, default_value=float(0),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='gender_confidence', full_name='rongyi.face.log.UserProfile.gender_confidence', index=7,
      number=1002, type=1, cpp_type=5, label=1,
      has_default_value=False, default_value=float(0),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='expression_confidence', full_name='rongyi.face.log.UserProfile.expression_confidence', index=8,
      number=1003, type=1, cpp_type=5, label=1,
      has_default_value=False, default_value=float(0),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=45,
  serialized_end=321,
)


_CAPTUREDUSERFACE = _descriptor.Descriptor(
  name='CapturedUserFace',
  full_name='rongyi.face.log.CapturedUserFace',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='profile', full_name='rongyi.face.log.CapturedUserFace.profile', index=0,
      number=1, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='enter_time', full_name='rongyi.face.log.CapturedUserFace.enter_time', index=1,
      number=2, type=3, cpp_type=2, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='leave_time', full_name='rongyi.face.log.CapturedUserFace.leave_time', index=2,
      number=3, type=3, cpp_type=2, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='position', full_name='rongyi.face.log.CapturedUserFace.position', index=3,
      number=4, type=14, cpp_type=8, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='face_image', full_name='rongyi.face.log.CapturedUserFace.face_image', index=4,
      number=5, type=12, cpp_type=9, label=1,
      has_default_value=False, default_value=_b(""),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='image_mime_type', full_name='rongyi.face.log.CapturedUserFace.image_mime_type', index=5,
      number=6, type=12, cpp_type=9, label=1,
      has_default_value=False, default_value=_b(""),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=324,
  serialized_end=523,
)


_REPORTID = _descriptor.Descriptor(
  name='ReportID',
  full_name='rongyi.face.log.ReportID',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='terminal_id', full_name='rongyi.face.log.ReportID.terminal_id', index=0,
      number=1, type=12, cpp_type=9, label=1,
      has_default_value=False, default_value=_b(""),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='timestamp', full_name='rongyi.face.log.ReportID.timestamp', index=1,
      number=2, type=3, cpp_type=2, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='index', full_name='rongyi.face.log.ReportID.index', index=2,
      number=3, type=3, cpp_type=2, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=525,
  serialized_end=590,
)


_USERFACEINFO = _descriptor.Descriptor(
  name='UserFaceInfo',
  full_name='rongyi.face.log.UserFaceInfo',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='id', full_name='rongyi.face.log.UserFaceInfo.id', index=0,
      number=1, type=12, cpp_type=9, label=2,
      has_default_value=False, default_value=_b(""),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='timestamp', full_name='rongyi.face.log.UserFaceInfo.timestamp', index=1,
      number=2, type=3, cpp_type=2, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='mall_id', full_name='rongyi.face.log.UserFaceInfo.mall_id', index=2,
      number=3, type=12, cpp_type=9, label=1,
      has_default_value=False, default_value=_b(""),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='terminal_id', full_name='rongyi.face.log.UserFaceInfo.terminal_id', index=3,
      number=4, type=12, cpp_type=9, label=1,
      has_default_value=False, default_value=_b(""),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='face', full_name='rongyi.face.log.UserFaceInfo.face', index=4,
      number=5, type=11, cpp_type=10, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=593,
  serialized_end=725,
)

_USERPROFILE.fields_by_name['gender'].enum_type = _GENDER
_USERPROFILE.fields_by_name['expression'].enum_type = _USEREXPRESSION
_CAPTUREDUSERFACE.fields_by_name['profile'].message_type = _USERPROFILE
_CAPTUREDUSERFACE.fields_by_name['position'].enum_type = _FACEPOSITION
_USERFACEINFO.fields_by_name['face'].message_type = _CAPTUREDUSERFACE
DESCRIPTOR.message_types_by_name['UserProfile'] = _USERPROFILE
DESCRIPTOR.message_types_by_name['CapturedUserFace'] = _CAPTUREDUSERFACE
DESCRIPTOR.message_types_by_name['ReportID'] = _REPORTID
DESCRIPTOR.message_types_by_name['UserFaceInfo'] = _USERFACEINFO
DESCRIPTOR.enum_types_by_name['UserExpression'] = _USEREXPRESSION
DESCRIPTOR.enum_types_by_name['FacePosition'] = _FACEPOSITION
DESCRIPTOR.enum_types_by_name['Gender'] = _GENDER

UserProfile = _reflection.GeneratedProtocolMessageType('UserProfile', (_message.Message,), dict(
  DESCRIPTOR = _USERPROFILE,
  __module__ = 'human_face_report_pb2'
  # @@protoc_insertion_point(class_scope:rongyi.face.log.UserProfile)
  ))
_sym_db.RegisterMessage(UserProfile)

CapturedUserFace = _reflection.GeneratedProtocolMessageType('CapturedUserFace', (_message.Message,), dict(
  DESCRIPTOR = _CAPTUREDUSERFACE,
  __module__ = 'human_face_report_pb2'
  # @@protoc_insertion_point(class_scope:rongyi.face.log.CapturedUserFace)
  ))
_sym_db.RegisterMessage(CapturedUserFace)

ReportID = _reflection.GeneratedProtocolMessageType('ReportID', (_message.Message,), dict(
  DESCRIPTOR = _REPORTID,
  __module__ = 'human_face_report_pb2'
  # @@protoc_insertion_point(class_scope:rongyi.face.log.ReportID)
  ))
_sym_db.RegisterMessage(ReportID)

UserFaceInfo = _reflection.GeneratedProtocolMessageType('UserFaceInfo', (_message.Message,), dict(
  DESCRIPTOR = _USERFACEINFO,
  __module__ = 'human_face_report_pb2'
  # @@protoc_insertion_point(class_scope:rongyi.face.log.UserFaceInfo)
  ))
_sym_db.RegisterMessage(UserFaceInfo)


DESCRIPTOR.has_options = True
DESCRIPTOR._options = _descriptor._ParseOptions(descriptor_pb2.FileOptions(), _b('\n\027com.rongyi.sirius.proto'))
# @@protoc_insertion_point(module_scope)
