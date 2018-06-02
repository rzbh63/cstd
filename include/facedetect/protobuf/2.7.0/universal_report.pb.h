// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: universal_report.proto

#ifndef PROTOBUF_universal_5freport_2eproto__INCLUDED
#define PROTOBUF_universal_5freport_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2007000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2007000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace rongyi {
namespace report {

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_universal_5freport_2eproto();
void protobuf_AssignDesc_universal_5freport_2eproto();
void protobuf_ShutdownFile_universal_5freport_2eproto();

class UniversalReportRequest;

// ===================================================================

class UniversalReportRequest : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:rongyi.report.UniversalReportRequest) */ {
 public:
  UniversalReportRequest();
  virtual ~UniversalReportRequest();

  UniversalReportRequest(const UniversalReportRequest& from);

  inline UniversalReportRequest& operator=(const UniversalReportRequest& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const UniversalReportRequest& default_instance();

  void Swap(UniversalReportRequest* other);

  // implements Message ----------------------------------------------

  inline UniversalReportRequest* New() const { return New(NULL); }

  UniversalReportRequest* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const UniversalReportRequest& from);
  void MergeFrom(const UniversalReportRequest& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(UniversalReportRequest* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional string id = 1;
  bool has_id() const;
  void clear_id();
  static const int kIdFieldNumber = 1;
  const ::std::string& id() const;
  void set_id(const ::std::string& value);
  void set_id(const char* value);
  void set_id(const char* value, size_t size);
  ::std::string* mutable_id();
  ::std::string* release_id();
  void set_allocated_id(::std::string* id);

  // optional int64 timestamp = 2;
  bool has_timestamp() const;
  void clear_timestamp();
  static const int kTimestampFieldNumber = 2;
  ::google::protobuf::int64 timestamp() const;
  void set_timestamp(::google::protobuf::int64 value);

  // optional string message_name = 3;
  bool has_message_name() const;
  void clear_message_name();
  static const int kMessageNameFieldNumber = 3;
  const ::std::string& message_name() const;
  void set_message_name(const ::std::string& value);
  void set_message_name(const char* value);
  void set_message_name(const char* value, size_t size);
  ::std::string* mutable_message_name();
  ::std::string* release_message_name();
  void set_allocated_message_name(::std::string* message_name);

  // repeated bytes message_data = 4;
  int message_data_size() const;
  void clear_message_data();
  static const int kMessageDataFieldNumber = 4;
  const ::std::string& message_data(int index) const;
  ::std::string* mutable_message_data(int index);
  void set_message_data(int index, const ::std::string& value);
  void set_message_data(int index, const char* value);
  void set_message_data(int index, const void* value, size_t size);
  ::std::string* add_message_data();
  void add_message_data(const ::std::string& value);
  void add_message_data(const char* value);
  void add_message_data(const void* value, size_t size);
  const ::google::protobuf::RepeatedPtrField< ::std::string>& message_data() const;
  ::google::protobuf::RepeatedPtrField< ::std::string>* mutable_message_data();

  // repeated int64 check_sum = 5;
  int check_sum_size() const;
  void clear_check_sum();
  static const int kCheckSumFieldNumber = 5;
  ::google::protobuf::int64 check_sum(int index) const;
  void set_check_sum(int index, ::google::protobuf::int64 value);
  void add_check_sum(::google::protobuf::int64 value);
  const ::google::protobuf::RepeatedField< ::google::protobuf::int64 >&
      check_sum() const;
  ::google::protobuf::RepeatedField< ::google::protobuf::int64 >*
      mutable_check_sum();

  // @@protoc_insertion_point(class_scope:rongyi.report.UniversalReportRequest)
 private:
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_timestamp();
  inline void clear_has_timestamp();
  inline void set_has_message_name();
  inline void clear_has_message_name();

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::internal::ArenaStringPtr id_;
  ::google::protobuf::int64 timestamp_;
  ::google::protobuf::internal::ArenaStringPtr message_name_;
  ::google::protobuf::RepeatedPtrField< ::std::string> message_data_;
  ::google::protobuf::RepeatedField< ::google::protobuf::int64 > check_sum_;
  friend void  protobuf_AddDesc_universal_5freport_2eproto();
  friend void protobuf_AssignDesc_universal_5freport_2eproto();
  friend void protobuf_ShutdownFile_universal_5freport_2eproto();

  void InitAsDefaultInstance();
  static UniversalReportRequest* default_instance_;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// UniversalReportRequest

// optional string id = 1;
inline bool UniversalReportRequest::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void UniversalReportRequest::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void UniversalReportRequest::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void UniversalReportRequest::clear_id() {
  id_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_id();
}
inline const ::std::string& UniversalReportRequest::id() const {
  // @@protoc_insertion_point(field_get:rongyi.report.UniversalReportRequest.id)
  return id_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void UniversalReportRequest::set_id(const ::std::string& value) {
  set_has_id();
  id_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:rongyi.report.UniversalReportRequest.id)
}
inline void UniversalReportRequest::set_id(const char* value) {
  set_has_id();
  id_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:rongyi.report.UniversalReportRequest.id)
}
inline void UniversalReportRequest::set_id(const char* value, size_t size) {
  set_has_id();
  id_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:rongyi.report.UniversalReportRequest.id)
}
inline ::std::string* UniversalReportRequest::mutable_id() {
  set_has_id();
  // @@protoc_insertion_point(field_mutable:rongyi.report.UniversalReportRequest.id)
  return id_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* UniversalReportRequest::release_id() {
  // @@protoc_insertion_point(field_release:rongyi.report.UniversalReportRequest.id)
  clear_has_id();
  return id_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void UniversalReportRequest::set_allocated_id(::std::string* id) {
  if (id != NULL) {
    set_has_id();
  } else {
    clear_has_id();
  }
  id_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), id);
  // @@protoc_insertion_point(field_set_allocated:rongyi.report.UniversalReportRequest.id)
}

// optional int64 timestamp = 2;
inline bool UniversalReportRequest::has_timestamp() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void UniversalReportRequest::set_has_timestamp() {
  _has_bits_[0] |= 0x00000002u;
}
inline void UniversalReportRequest::clear_has_timestamp() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void UniversalReportRequest::clear_timestamp() {
  timestamp_ = GOOGLE_LONGLONG(0);
  clear_has_timestamp();
}
inline ::google::protobuf::int64 UniversalReportRequest::timestamp() const {
  // @@protoc_insertion_point(field_get:rongyi.report.UniversalReportRequest.timestamp)
  return timestamp_;
}
inline void UniversalReportRequest::set_timestamp(::google::protobuf::int64 value) {
  set_has_timestamp();
  timestamp_ = value;
  // @@protoc_insertion_point(field_set:rongyi.report.UniversalReportRequest.timestamp)
}

// optional string message_name = 3;
inline bool UniversalReportRequest::has_message_name() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void UniversalReportRequest::set_has_message_name() {
  _has_bits_[0] |= 0x00000004u;
}
inline void UniversalReportRequest::clear_has_message_name() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void UniversalReportRequest::clear_message_name() {
  message_name_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_message_name();
}
inline const ::std::string& UniversalReportRequest::message_name() const {
  // @@protoc_insertion_point(field_get:rongyi.report.UniversalReportRequest.message_name)
  return message_name_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void UniversalReportRequest::set_message_name(const ::std::string& value) {
  set_has_message_name();
  message_name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:rongyi.report.UniversalReportRequest.message_name)
}
inline void UniversalReportRequest::set_message_name(const char* value) {
  set_has_message_name();
  message_name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:rongyi.report.UniversalReportRequest.message_name)
}
inline void UniversalReportRequest::set_message_name(const char* value, size_t size) {
  set_has_message_name();
  message_name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:rongyi.report.UniversalReportRequest.message_name)
}
inline ::std::string* UniversalReportRequest::mutable_message_name() {
  set_has_message_name();
  // @@protoc_insertion_point(field_mutable:rongyi.report.UniversalReportRequest.message_name)
  return message_name_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* UniversalReportRequest::release_message_name() {
  // @@protoc_insertion_point(field_release:rongyi.report.UniversalReportRequest.message_name)
  clear_has_message_name();
  return message_name_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void UniversalReportRequest::set_allocated_message_name(::std::string* message_name) {
  if (message_name != NULL) {
    set_has_message_name();
  } else {
    clear_has_message_name();
  }
  message_name_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), message_name);
  // @@protoc_insertion_point(field_set_allocated:rongyi.report.UniversalReportRequest.message_name)
}

// repeated bytes message_data = 4;
inline int UniversalReportRequest::message_data_size() const {
  return message_data_.size();
}
inline void UniversalReportRequest::clear_message_data() {
  message_data_.Clear();
}
inline const ::std::string& UniversalReportRequest::message_data(int index) const {
  // @@protoc_insertion_point(field_get:rongyi.report.UniversalReportRequest.message_data)
  return message_data_.Get(index);
}
inline ::std::string* UniversalReportRequest::mutable_message_data(int index) {
  // @@protoc_insertion_point(field_mutable:rongyi.report.UniversalReportRequest.message_data)
  return message_data_.Mutable(index);
}
inline void UniversalReportRequest::set_message_data(int index, const ::std::string& value) {
  // @@protoc_insertion_point(field_set:rongyi.report.UniversalReportRequest.message_data)
  message_data_.Mutable(index)->assign(value);
}
inline void UniversalReportRequest::set_message_data(int index, const char* value) {
  message_data_.Mutable(index)->assign(value);
  // @@protoc_insertion_point(field_set_char:rongyi.report.UniversalReportRequest.message_data)
}
inline void UniversalReportRequest::set_message_data(int index, const void* value, size_t size) {
  message_data_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:rongyi.report.UniversalReportRequest.message_data)
}
inline ::std::string* UniversalReportRequest::add_message_data() {
  // @@protoc_insertion_point(field_add_mutable:rongyi.report.UniversalReportRequest.message_data)
  return message_data_.Add();
}
inline void UniversalReportRequest::add_message_data(const ::std::string& value) {
  message_data_.Add()->assign(value);
  // @@protoc_insertion_point(field_add:rongyi.report.UniversalReportRequest.message_data)
}
inline void UniversalReportRequest::add_message_data(const char* value) {
  message_data_.Add()->assign(value);
  // @@protoc_insertion_point(field_add_char:rongyi.report.UniversalReportRequest.message_data)
}
inline void UniversalReportRequest::add_message_data(const void* value, size_t size) {
  message_data_.Add()->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_add_pointer:rongyi.report.UniversalReportRequest.message_data)
}
inline const ::google::protobuf::RepeatedPtrField< ::std::string>&
UniversalReportRequest::message_data() const {
  // @@protoc_insertion_point(field_list:rongyi.report.UniversalReportRequest.message_data)
  return message_data_;
}
inline ::google::protobuf::RepeatedPtrField< ::std::string>*
UniversalReportRequest::mutable_message_data() {
  // @@protoc_insertion_point(field_mutable_list:rongyi.report.UniversalReportRequest.message_data)
  return &message_data_;
}

// repeated int64 check_sum = 5;
inline int UniversalReportRequest::check_sum_size() const {
  return check_sum_.size();
}
inline void UniversalReportRequest::clear_check_sum() {
  check_sum_.Clear();
}
inline ::google::protobuf::int64 UniversalReportRequest::check_sum(int index) const {
  // @@protoc_insertion_point(field_get:rongyi.report.UniversalReportRequest.check_sum)
  return check_sum_.Get(index);
}
inline void UniversalReportRequest::set_check_sum(int index, ::google::protobuf::int64 value) {
  check_sum_.Set(index, value);
  // @@protoc_insertion_point(field_set:rongyi.report.UniversalReportRequest.check_sum)
}
inline void UniversalReportRequest::add_check_sum(::google::protobuf::int64 value) {
  check_sum_.Add(value);
  // @@protoc_insertion_point(field_add:rongyi.report.UniversalReportRequest.check_sum)
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::int64 >&
UniversalReportRequest::check_sum() const {
  // @@protoc_insertion_point(field_list:rongyi.report.UniversalReportRequest.check_sum)
  return check_sum_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::int64 >*
UniversalReportRequest::mutable_check_sum() {
  // @@protoc_insertion_point(field_mutable_list:rongyi.report.UniversalReportRequest.check_sum)
  return &check_sum_;
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

}  // namespace report
}  // namespace rongyi

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_universal_5freport_2eproto__INCLUDED
